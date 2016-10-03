#include "PionInteractionSystematic.hxx"
#include "TFile.h"
#include "GeometryManager.hxx"

//JMDEBUGVERSION code:
//0 is 2013
//1 is OR'ed volumes of interest.  Should reproduce 2013 exactly with FGD1 only
//selection, and have a similar treatment for FGD2.
//2 is VOI extended to include both VOIs and the TPC between them.
//3 new: VOI is FGD1 for selections in FGD1 and FGD2 for selections in FGD2 (automatic)
#define JMDEBUGVERSION 1


//For easy access to the cross section data.
using namespace PiXSec;


//Empty, do nothing constructor, as we fill it manually.
//Loads the geometry, so a new object must be created before begin computations
//of the quantities that go in this object.
PionInteractionSystematic::PionInteractionSystematic(){
  
  nPions    = 0;
  pionType  = NULL;

  totalSteps  = 0;
  nSteps      = NULL;

  initMom     = NULL;
  
  stepLengths = NULL;

  nInteractions   = 0;
  pInteraction    = NULL;
  typeInteraction = NULL;

  pionID = NULL;
  TrueParticles = NULL;
  InteractionTrueParticles = NULL;
  
}

PionInteractionSystematic::~PionInteractionSystematic(){
  if(pionType)    
    delete [] pionType;
  pionType = NULL;
  
  if(nSteps)
    delete [] nSteps;
  nSteps = NULL;
  
  if(initMom)
    delete [] initMom;
  initMom = NULL;
  
  if(pInteraction)
    delete [] pInteraction;
  pInteraction = NULL;
  
  if(typeInteraction)  
    delete [] typeInteraction;
  typeInteraction = NULL;
  
  if(pionID)
    delete [] pionID;
  pionID = NULL;

  if(TrueParticles)
    delete [] TrueParticles;
  TrueParticles = NULL;

  if(InteractionTrueParticles)
    delete [] InteractionTrueParticles;
  InteractionTrueParticles = NULL;

  
  if(stepLengths)
    delete [] stepLengths;
  stepLengths = NULL;
}




//Define the constructor for the StepResult object.
SteppingResult::SteppingResult(Float_t mcSum, 
        Float_t dataSum,
        std::vector<Float_t> steps,
        Float_t momFinal,
        TLorentzVector posFinal){

    MCSumNSigmaStepLength = mcSum;
    DataSumNSigmaStepLength = dataSum;
    stepLengths = steps;
    finalMom = momFinal;
    finalPos = posFinal;
}

//===================================================================
//Stepping Code.
//===================================================================
//Density of scattering centres.
//i.e. the number of atoms per unit volume.
//Any ignored elements (H, Na, Ti) aren't considered.
//Also, the answer for FGDScintillator will differ slightly from the
//pre-calculated FGDScintillator value due to the inclusion of small amounts
//of N and O.
//N = (N_A/M_r)*rho
//Where N is the number density (particles/cm^3), N_A is Avogadro's constant,
//M_r is the molar mass, and rho is the density.
//For a composite, material, if w_j is the fraction by weight,
//rho*wj gives the density of that element, sub that in for rho in
//the above equation, and that gives the number of that element
//per unit volume.
//Could be calculated separately and tabulated in a function that returns the
//right one when supplied with a given material name.
//This function takes in a pointer to a
//material and computes it.
//Use it to fill a map at run time that can then consult instead of recalculating.
//The FGDScintillator value is pre-computed and hard-coded where needed.
Double_t DScattCentres(TGeoMixture* mixture){

  //Get the atomic number, mass (i.e. molar mass) and weight by mass arrays,
  //as well as the number of elements.
  Double_t* ZArray = mixture->GetZmixt();
  Double_t* AArray = mixture->GetAmixt();
  Double_t* WArray = mixture->GetWmixt();
  Int_t NElts = mixture->GetNelements();
  
  //Avogadro's constant (according to the PDG, last two decimal places removed).
  Double_t NA = 6.02214e23;
  
  //Go through and compute the number of scattering centres of each element per unit
  //volume.  Add them together to get the total number of scattering centres per
  //unit volume.
  Double_t result = 0;
  for(Int_t i = 0; i< NElts; i++){
    
    if(ZArray[i] != 1 //Exclude H
       && ZArray[i] != 11 //Exclude Na
       && ZArray[i] != 22){ //Exclude Ti
      
      //Extra factor required to convert density to g/cm^3
      result += (NA/AArray[i])*WArray[i]*((1000.0/(6.2415e21))*mixture->GetDensity());      
    }    
  }
  
  //All constituents having now been looped over, return the value.
  return result; 
}

//Takes in a material name and returns the "material Series" number,
//a multiple of 10 that is used to tabulate the cross sections.
//Key:
//Carbon: 0
//Oxygen: 10
//Aluminum: 20
//Iron: 30
//AlG10: 40
//WaterSystem: 50
//G10FGD1, G10: 60
//FGDGlue: 70
//G10Roha: 80
Int_t GetMaterialSeries(std::string matName){

  //Air, use O.
  if (matName == "Air") return 10;      
  //CO2, use O.
  else if (matName == "CO2") return 10;   
  //Aluminum
  else if (matName == "Aluminum") return 20;
  //Steel, use Fe.
  else if (matName == "Steel") return 30;
  //AlRoha2 or AlRoha, use Aluminum.
  else if (matName == "AlRoha" || matName == "AlRoha2") return 20;
  //AlG10
  else if (matName == "AlG10") return 40;
  //WaterSystem
  else if (matName == "WaterSystem") return 50;
  //G10FGD1/G10
  else if (matName == "G10FGD1" || matName == "G10") return 60;  
  //FGDScintillator (use Carbon)
  //There are some extra letters beyond "FGDScintillator" possible
  //in the name of this material.
  else if (matName.find("FGDScintillator") != std::string::npos) return 0;
  //FGDGlue
  else if (matName == "FGDGlue") return 70;
  //G10Roha
  else if (matName == "G10Roha") return 80;
  //-----------------------------
  //New materials added for 2014.
  //-----------------------------
  //G10FGD2 has the same composition as G10 and G10FGD1, as far as molar
  //fractions of elements are concerned.
  else if (matName == "G10FGD2") return 60;
  //ActiveWater gets approximated by Oxygen.
  else if (matName == "ActiveWater") return 10;
  //Polypropylene is just Carbon and (ignored) Hydrogen
  else if (matName == "Polypropylene") return 0;
  //FGDWaterModuleEpoxy has the same composition as FGDGlue (70 series).
  else if (matName == "FGDWaterModuleEpoxy") return 70;
  //GasMixtureTPC is mostly Argon.  The closest element we have to Argon is
  //Aluminum, which is not a great approximation, but with such a low density
  //of scattering centres in the TPC Gas, should be good enough, and is
  //certainly closer than the Carbon we would otherwise default to.
  //NB: The Material Series is only for which set of pion cross sections to
  //use.  Other properties (like density) are determined from data taken from
  //the geometry.
  else if (matName == "GasMixtureTPC") return 20;
  //If it was something we've somehow missed, just use Carbon.
  else return 0;
}

//Takes in an atomic number and returns the Mean Ionization Potential for
//that element, from http://physics.nist.gov/PhysRefData/XrayMassCoef/tab1.html
//Only filled for elements present in ND280UserDetectorConstruction.cc
//Uses units of eV.  Must be converted to MeV later.
Double_t GetIElement(Int_t Z){

  switch(Z){    
  case 1:     //Hydrogen
    return 19.2;    
  case 5:      //Boron
    return 76.0;    
  case 6:      //Carbon
    return 78.0;    
  case 7:     //Nitrogen
    return 82.0;
  case 8:     //Oxygen
    return 95.0;    
  case 9:     //Fluorine
    return 115.0;
  case 11:    //Sodium
    return 149.0;    
  case 13:    //Aluminum
    return 166.0;
  case 14:    //Silicon
    return 173.0;    
  case 17:    //Chlorine
    return 174.0;
  case 18:    //Argon
    return 188.0;
  case 22:    //Titanium
    return 233.0;    
  case 26:    //Iron
    return 286.0;    
  case 27:    //Cobalt
    return 297.0;    
  case 29:    //Copper
    return 322.0;    
  case 30:    //Zinc
    return 330.0;    
  case 50:    //Tin
    return 488.0;    
  case 82:    //Lead
    return 823.0;    
  default:    //If unrecognized, just assume FGD scintillator.
    return 68.7;
  }    
}

//Returns the ZoverA value for a given material.
//(Calculated from weights and content.)
Double_t GetZoverAMaterial(TGeoMixture* mat){
  
  Double_t ZoverA = 0;
  
  //Get the number of elements
  Int_t NElements = mat->GetNelements();
  
  //Get the Z values for the material constituents.
  Double_t* Zmixt = mat->GetZmixt();
  
  //Get the A values for the material constituents.
  Double_t* Amixt = mat->GetAmixt();
  
  //Get the weight by mass of the elements.
  Double_t* Wmixt = mat->GetWmixt();

  //Compute Z/A
  for(Int_t i = 0; i < NElements; i++){
    
    ZoverA += Wmixt[i]*(Zmixt[i]/Amixt[i]);
    
  }  
  return ZoverA;
}


//Calculates the effective mean excitation energy for
//a mixture.
///Equation taken from Leo p.29.
Double_t GetIMaterial(TGeoMixture* mat, Double_t ZoverA){

  Double_t lnI = 0;
  
  //Get the number of elements
  Int_t NElements = mat->GetNelements();
  
  //Get the Z values for the material constituents.
  Double_t* Zmixt = mat->GetZmixt();
  
  //Get the A values for the material constituents.
  Double_t* Amixt = mat->GetAmixt();
  
  //Get the weight by mass of the elements.
  Double_t* Wmixt = mat->GetWmixt();
  
  //lnIeff = sum(w_i*(Z_i/A_i)*lnI_i)/(Z/A)
  for(Int_t i = 0; i < NElements; i++){
    
    lnI += (Wmixt[i]*(Zmixt[i]/Amixt[i])*log(GetIElement((Int_t)Zmixt[i])))/ZoverA;
  }

  return exp(lnI);  
}


//Takes in the initial momentum value and returns the corresponding
//kinetic energy of a pi+/-.
//pi+/- mass = 139.57 MeV (PDG)
Double_t computeEkinFromMom(Double_t mom){
  
  Double_t Ekin = sqrt(pow(mom,2) + pow(139.57,2)) - 139.57;
  return Ekin;
}

Double_t Interpolate(double xsec1, double xsec2, double mom1, double mom){
  //We use 10MeV bins for the calculation
  return (((mom/10.)-mom1)*(xsec2-xsec1) + xsec1);
}


//Only propagates pi+/-  (i.e. assumes their mass, 139.57 MeV/c^2)
//Takes the pion, propagates it by the given step length.  (The step length
//should be small.)
//Output is a pair <finalPosition,finalMomentum>
std::pair<TLorentzVector,TVector3> TakeSmallStep(Int_t charge, 
						 TLorentzVector initPos, 
						 TVector3 initMom,
						 Double_t stepLength, //in mm
						 TGeoMaterial* material,
						 std::string materialName,
						 TGeoMixture* mixture){

  //Find the initial kinetic energy.
  Double_t initEkin = computeEkinFromMom(initMom.Mag());
  
  //Find the initial direction.
  TVector3 initDir = initMom*(1/initMom.Mag());
  
  
  //========Fill the information used for the energy loss calculation here.==========
  //To start, the \delta(\beta\gamma)/2 (density) correction is only needed at high energies (Leo, 25),
  //so we don't need to include that term.  The shell correction exists at low energies, but the effect
  //is typically small (Leo, 26) so we'll leave that out too.
  
  //Now, set up the values that we need for the formula. 
  Double_t K = 0.307075;
  Double_t z2 = 1.0; //Incident particle is a pi+ or pi-, either way charge^2 = 1.
  Double_t mec2 = 0.510998918;
  Double_t m_pi = 139.57;
  
  //Compute the momentum of the particle from the kinetic energy.
  Double_t p_pi = initMom.Mag();
  Double_t betagamma = p_pi/m_pi;
  Double_t beta = p_pi/(initEkin + m_pi);
  Double_t gamma = betagamma/beta;
  Double_t T_max = (2*mec2*pow(betagamma,2))/(1 + 2*gamma*(mec2/m_pi) + pow(mec2/m_pi,2));
  
  //==========================================
  //The following parameters are for FGD scintillator.  Change them later if not in FGD scintillator.
  Double_t ZoverA = 0.53768; //From http://pdg.lbl.gov/2011/reviews/rpp2011-rev-atomic-nuclear-prop.pdf
  Double_t I = 68.7E-6; //Mean Excitation energy, 68.7 eV from http://physics.nist.gov/cgi-bin/Star/compos.pl?refer=ap&matno=226
  Double_t rho = 1.032;
  //==========================================
  
  //If the material name does not contain "FGDScintillator", calculate different material based parameters.
  if (materialName.find("FGDScintillator") == std::string::npos){
    
    ZoverA = GetZoverAMaterial(mixture);
    I = GetIMaterial(mixture,ZoverA)*(1E-6); //Make sure to convert to MeV!
    
    //The conversion factor converts the density stored in the geometry to g/cm^3
    rho = (1000.0/(6.2415e21))*(material->GetDensity());
  }
  
  Double_t dEdx = K*z2*(ZoverA)*(1/pow(beta,2))*((1.0/2.0)*log(2*mec2*pow(betagamma,2)*T_max/pow(I,2)) - pow(beta,2));
  
  //Remember, it is dE/dx * length in material * density of material
  //Used 1.032 g/cm^3, the density of the scintillator bars (from TN-91), as opposed
  //to the full module density which took into account the G10, glue, and fiber.
  //Factor of 0.1 to convert the mm tracklength to cm.
  Double_t finalEkin = initEkin - dEdx*(0.1)*stepLength*rho;
  
  //Now, since we're taking small steps, we're assuming that the change in momentum is small, so we'll
  //just use the initial momentum as the feed through into the helix code.
  //The velocity of the pion in m/s)
  //PDG speed of light value: 299 792 458 m/s
  Double_t c = 299792458;
  TVector3 initVelocity = beta*c*initDir;
  
  //The magnetic field is 0.2 T in the x direction.
  TVector3 field(0.2,0.0,0.0);
  
  //dp/dt = q(v x B), where p = \beta\gamma*mc
  //1 unit of charge is 1.602176e-19 Coulombs
  TVector3 dpdt = charge*(1.602176e-19)*initVelocity.Cross(field);
  
  //So the change in momentum is therefore dpdt*dt, where dt = stepLength/initVelocity.Mag()
  //0.001 converts stepLength from mm to m.
  Double_t dt = ((0.001*stepLength)/initVelocity.Mag());
  TVector3 dp = dpdt*dt;
  
  //So that we're working all in the same units, use gamma*m*initVelocity to get the initial momentum back
  //(i.e. p = gamma*m*v)
  //Convert m_pi from MeV/c^2 to kg with factor of 1.782662e-30
  TVector3 pFinal = gamma*m_pi*(1.782662e-30)*initVelocity + dp;
  
  //Now we know the final momentum in kg*m/s.  The Lorentz force only changes the direction and not the
  //magnitude of the momentum.  So, all we care about is the direction.
  TVector3 finalDir = pFinal*(1/pFinal.Mag());
  
  //The final position of a step is going to be
  //the result of going the step length in the direction of the initial momentum.
  //The final momentum is going to be the final kinetic energy converted back to a momentum
  //and in the direction of the final direction.
  TLorentzVector positionChange(stepLength*initDir.X(),stepLength*initDir.Y(),stepLength*initDir.Z(),(1e9)*dt);
  TLorentzVector finalPosition = initPos + positionChange;
  
  //Declare the vector the final momentum of the step will go into.
  TVector3 finalMomVec;
  
  //If the final kinetic energy is <=0, set the finalMomVec to (0.0,0.0,0.0)
  if(finalEkin <= 0 ){
    TVector3 zeroVec(0.0,0.0,0.0);
    finalMomVec = zeroVec;
  }
  
  //Otherwise, use the actual computed Ekin to compute the momentum vector.
  else{
    Double_t finalMom = sqrt(pow(finalEkin + m_pi,2) - pow(m_pi,2));
    finalMomVec = finalMom*finalDir;
  }
  
  return std::make_pair(finalPosition,finalMomVec);
  
}

// Pion SI Manager business

Bool_t PionSIManager::InVOI(SubDetId_h det, Float_t* pos) const{  

#if   JMDEBUGVERSION == 0
  if (det == SubDetId::kFGD1 && InVOI1(pos)) return true;
#elif JMDEBUGVERSION == 1
  (void) det;
  if (InVOI1(pos) || InVOI2(pos)) return true;
#elif JMDEBUGVERSION == 2
  (void) det;
  if (InVOIext(pos)) return true;
#elif JMDEBUGVERSION == 3
  if      (det == SubDetId::kFGD1 && InVOI1(pos)) return true;
  else if (det == SubDetId::kFGD2 && InVOI2(pos)) return true;
#endif


  return false;  
}


Bool_t PionSIManager::InVOI1(Float_t* pos) const{
  
  //This is the edges of the FGD1 volume, plus all the way out to TPCGasMixture in downstream z.
  //(Rounded to the next integer value that includes the value from the geometry in its range).

  Bool_t xOK = -1150.0 < pos[0] && pos[0] < 1150.0;
  Bool_t yOK = -1170.0 < pos[1] && pos[1] < 1230.0;
  Bool_t zOK =    98.0 < pos[2] && pos[2] < 576.0;
  
  return xOK && yOK && zOK;
  
}

Bool_t PionSIManager::InVOI2(Float_t* pos) const{

  //This is from the TPCGasMixture in upstream z through FGD2 all the way out to TPCGasMixture in downstream z.
  //(Rounded to the next integer value that includes the value from the geometry in its range).
  Bool_t xOK = -1150.0 < pos[0] && pos[0] < 1150.0;
  Bool_t yOK = -1170.0 < pos[1] && pos[1] < 1230.0;
  Bool_t zOK =  1347.0 < pos[2] && pos[2] < 1934.0;
  
  return xOK && yOK && zOK;
  
}

Bool_t PionSIManager::InVOIext(Float_t* pos) const{
  //This is the edges of the FGD1 volume, plus all the way out to TPCGasMixture in TPC3 in downstream z.
  //(Rounded to the next integer value that includes the value from the geometry in its range).
  Bool_t xOK = -1150.0 < pos[0] && pos[0] < 1150.0;
  Bool_t yOK = -1170.0 < pos[1] && pos[1] < 1230.0;
  Bool_t zOK =    98.0 < pos[2] && pos[2] < 1934.0;
  
  return xOK && yOK && zOK;
}




//Given the info from two saved trajectory points, steps between them and saves step positions
//and momenta at appropriate points.
//The initial and final position and momentum are as come from the oaAnalysis info.
//Also need to return MC and Data sum of n*sigma*stepLength.
//Returns an object containing the information that we need for weighting, plus some
//information for testing/validation purposes.
SteppingResult PionSIManager::StepBetweenPoints(SubDetId_h det, Int_t charge,
				  TLorentzVector initPos, 
				  TVector3 initMom, 
				  TLorentzVector finalPos,
				  Double_t stepLength,
				  TGeoManager* geom) const{
  
  //Set up a variable to store the step result.
  std::pair<TLorentzVector,TVector3> stepResult;
  
  //These are the variables to store the n*sigma*stepLength sums in for MC and Data.
  Float_t MCSumNSigmaStepLength = 0;
  Float_t DataSumNSigmaStepLength = 0;
  

  //Loop over taking steps until the separation from start to end point
  //is the is the same from oaAnalysis saved and from stepping.
  Double_t initPointSep = (finalPos - initPos).Vect().Mag();
  Double_t stepPointSep = 0.0;

  //Initialize the starting conditions of the step to those
  //supplied to this function.
  TLorentzVector stepStartPos = initPos;
  TVector3 stepStartMom = initMom;

  //A variable to store the step length that is to be saved.
  //(in mm, is the number of small steps in a saved step, multiplied by the step length.)
  Double_t totalStepLength = 0;
  std::vector<Float_t> totalStepLengths;

  //Also need to save the momentum of the particle at the last saved point.
  TVector3 lastSavedMomentum = initMom;

  //The stepEnd information.
  TLorentzVector stepEndPos;
  TVector3 stepEndMom;

  while(stepPointSep < initPointSep){
    //Grab the material information for this step.
    //We use the material at the step start position for the step's material.
    //(Steps are small enough that this should be pretty accurate.)
    TGeoNode *node = geom->FindNode(stepStartPos.X(),stepStartPos.Y(),stepStartPos.Z());
    TGeoVolume *volume = node->GetVolume();
    TGeoMaterial *material = volume->GetMaterial();
    std::string materialName = material->GetName();
    TGeoMixture *mixture = (TGeoMixture*)material;


    //Given the information supplied, take a small step.
    stepResult = TakeSmallStep(charge,stepStartPos,stepStartMom,stepLength,
			       material,materialName,mixture);

    //Store the information at the end of the step for usage and later copying to
    //the next stepStart.
    stepEndPos = stepResult.first;
    stepEndMom = stepResult.second;


    //Also compute the separation between the step end point and the initial point.
    stepPointSep = (stepEndPos - initPos).Vect().Mag();
    //std::cout<<" step point sep "<<stepPointSep<<std::endl;
    //Define the number density of scattering centres for scintillator here,
    //as will use it both inside and outside the "If not scintillator" check.
    Double_t NDScint = ((6.02214e23)/12.011)*0.89538563*1.03552; //nuclei/cm^3

    //=============================
    //Define some variables for easy looping over the maps of XSecs.
    //=============================
    Int_t intTypes[3] = {0,1,4};

    //If it is a pi-, must add 100 to get the correct XSecs.
    Int_t piT;
    if (charge == 1){

      piT = 0;

    }

    else {

      piT = 100;

    }
    //==================================

    //If we're not in FGD scintillator, scale the step length for different
    //number density of scattering centres and interaction cross sections.
    //If the material name does not contain "FGDScintillator", calculate different material based parameters.
    if (materialName.find("FGDScintillator") == std::string::npos){

      Double_t scaledStepLength = stepLength;

      //Two things to take into account: density of scattering centres,
      //and scaling of cross section with Z and A.

      //Density of scattering centres.
      //i.e. the number of atoms per unit volume.
      //Any ignored elements (H, Na, etc.) aren't considered.
      //N = (N_A/M_r)*rho
      //Where N is the number density (particles/cm^3), N_A is Avogadro's constant,
      //M_r is the molar mass, and rho is the density.
      //For a composite, material, if w_j is the fraction by weight,
      //rho*wj gives the density of that element, sub that in for rho in
      //the above equation, and that gives the number of that element
      //per unit volume.
      //These should really just be calculated separately and tabulated though.
      //The FGD Scintillator one needs to be pre-calculated for the BANFF interface.
      //NA = 6.02214e23;
      //Mr = 12.011  (Carbon, weighted average of isotopes from periodic table)
      //Fraction by weight of carbon: 0.89538563
      //Density of scintillator (from geometry file): 1.03552 g/cm^3

      //The number density of scattering centres in the material,
      //in number/cm^3
      Double_t NDMat = DScattCentres(mixture);

      //Scale the step length by (NDMat/NDScint)
      //(Going through a material with a denser number of scattering centres
      //is equivalent to going through a longer distance of the scintillator.)
      scaledStepLength *= (NDMat/NDScint);


      //Now handle the difference in cross section between the non-scintillator
      //material's constituents and Carbon.
      //Need the sum of the cross sections of interest on Carbon at the step momentum,
      //and the sum on whatever material this is.
      //The material series is a multiple of 10 corresponding to where cross sections
      //for the material are stored in a map.
      Int_t materialSeries = GetMaterialSeries(materialName);

      //Now, get the sum of the XSecs for Carbon and for this material.
      Double_t cXSecSum = 0;
      Double_t matXSecSum = 0;
      Double_t matMCXSecSum = 0;

      for(Int_t ti = 0; ti < 3; ti++){

        //.first is data, which is what we want.
        int momBin = int(stepStartMom.Mag()/10);
        if (momBin>800) momBin = 800;
        cXSecSum += Interpolate(PiXSec::xsec_data[piT + intTypes[ti]][momBin], PiXSec::xsec_data[piT + intTypes[ti]][momBin+1],momBin,stepStartMom.Mag());
        matXSecSum += Interpolate(PiXSec::xsec_data[materialSeries + piT + intTypes[ti]][momBin], PiXSec::xsec_data[materialSeries + piT + intTypes[ti]][momBin+1],momBin,stepStartMom.Mag());
        matMCXSecSum += Interpolate(PiXSec::xsec_MC[materialSeries + piT + intTypes[ti]][momBin], PiXSec::xsec_MC[materialSeries + piT + intTypes[ti]][momBin+1],momBin,stepStartMom.Mag());
        
      }

      //With the carbon and actual material cross section determined, scale the step
      //length.
      if(cXSecSum != 0.0){
        scaledStepLength *= (matXSecSum/cXSecSum);
      }

      //The step length has now been scaled properly.  Add it to the total step length.
      totalStepLength += scaledStepLength;
      //Add to the MC and Data N*sigma*stepLengths.
      //Make sure to convert the stepLength to cm (from mm) and the cross section to cm^2
      //(from mbarn)
      //1 barn = 10^-28 m^2 => 1 mbarn = 10^-31 m^2 = 10^-27 cm^2
      //Potential 2000 MeV/c cutoff: If wanted not to weight steps above 2000 MeV/c, would
      //delete the next two commented lines:
      //if (stepStartMom.Mag() < 2000.0){
      MCSumNSigmaStepLength += NDMat*(1E-27)*matMCXSecSum*(stepLength/10.0);
      DataSumNSigmaStepLength += NDMat*(1E-27)*matXSecSum*(stepLength/10.0);

      //}

    }

    //Otherwise, just use the actual step length, and use scintillator for cross section calculations.
    else{
      totalStepLength += stepLength;

      //Get the cross section information for weight calculation.
      Double_t cXSecSum = 0;
      Double_t cMCXSecSum = 0;
      for(Int_t ti = 0; ti < 3; ti++){
        int momBin = int(stepStartMom.Mag()/10);
        if (momBin>800) momBin = 800;
        cXSecSum += Interpolate(PiXSec::xsec_data[piT + intTypes[ti]][momBin], PiXSec::xsec_data[piT + intTypes[ti]][momBin+1],momBin,stepStartMom.Mag());
        cMCXSecSum += Interpolate(PiXSec::xsec_MC[piT + intTypes[ti]][momBin], PiXSec::xsec_MC[piT + intTypes[ti]][momBin+1],momBin,stepStartMom.Mag());
      }

      //Add to the sums now.
      //Potential 2000 MeV/c cutoff: If wanted not to weight steps above 2000 MeV/c, would
      //delete the next two commented lines:
      //if (stepStartMom.Mag() < 2000.0){
      MCSumNSigmaStepLength += NDScint*(1E-27)*cMCXSecSum*(stepLength/10.0);
      DataSumNSigmaStepLength += NDScint*(1E-27)*cXSecSum*(stepLength/10.0);

      //}

    }
    
    //If the pion has stopped or left the volume of interest, 
    //save its information and exit this function by returning the result.
    
    Float_t stepEndPosArray[4];
    anaUtils::VectorToArray(stepEndPos,stepEndPosArray);
    if(stepEndMom.Mag() == 0.0 || !InVOI(det, stepEndPosArray)){            
      totalStepLengths.push_back(totalStepLength);
      
      //Prepare the results to return.
      SteppingResult sr(MCSumNSigmaStepLength,
			DataSumNSigmaStepLength,
			totalStepLengths,
			stepEndMom.Mag(),
			stepEndPos);

      return sr;
    }
    
    //Only save a point if the momentum has decreased by approximately 10 MeV.
    //Also save a point if we're going to break out of the while loop.
    else if (lastSavedMomentum.Mag() - stepEndMom.Mag() >= 10.0
	     || stepPointSep >= initPointSep){
      
      lastSavedMomentum = stepEndMom;
      totalStepLengths.push_back(totalStepLength);
      
      //Reset the step length for the next set of small steps.
      totalStepLength = 0;
      
    }
    
    //Copy the results of the step to the start position for the next run through.
    stepStartPos = stepResult.first;
    stepStartMom = stepResult.second;
    
    
  }//End while loop.
    
  //The lastSavedMomentum is the momentum at the end of the last step
  //(i.e. the "final momentum") as far as
  //my code is concerned (for interaction momentum tagging purposes.)

  //Prepare the results to return.
  SteppingResult sr(MCSumNSigmaStepLength,
		    DataSumNSigmaStepLength,
		    totalStepLengths,
		    lastSavedMomentum.Mag(),
		    stepEndPos);
    
  return sr;
    
}



//##########################################################
//The weight computing code.
//##########################################################
PionInteractionSystematic* PionSIManager::ComputePionWeightInfo(const AnaEventB& event, Int_t det) const{

  // Get the geometry from the GeometryManager
  TGeoManager* pionSIGeom = ND::hgman().GeoManager();

  //Only load in the geometry if it has not already been loaded by the InputConverter. Use in this case the default file
  if (!pionSIGeom){
    ND::hgman().LoadGeometry();  // Use default file
    pionSIGeom = ND::hgman().GeoManager();
  }

  AnaTrueParticleB* allTrajInTPCFGD[NMAXTRUEPARTICLES];
  int nTraj = anaUtils::GetAllTrajInBunch(event, allTrajInTPCFGD);
  if((UInt_t) nTraj>NMAXTRUEPARTICLES) nTraj = NMAXTRUEPARTICLES;

  //Declare the object the result will be written into.
  //If the geometry has not already been loaded, this will load the geometry.
  PionInteractionSystematic* result = new PionInteractionSystematic();
  //Now, go through and select only those trajectories that start in the volume we're concerned with.
  //FGD1FV plus dead material between that and TPC gas.  
  //It is a trajectory of interest if it starts in the volume of interest, as defined by InVOI, or its parent ends
  //in the volume of interest.
  std::vector<AnaTrueParticleB*> trajOfInterest;
  for (Int_t it = 0; it < nTraj; it++){
    
    AnaTrueParticleB* track = allTrajInTPCFGD[it];

    //If the trajectory starts in the volume of interest, save it.
    //One contiguous VOI, just use the one function.
    if(InVOI(det, track->Position) ){
      trajOfInterest.push_back(track);    
    }
    else{      
      //Otherwise, find its parent, and if the parent ends in the volume of interest,
      //save this trajectory.
      for (Int_t jt = 0; jt < nTraj; jt++){
        AnaTrueParticleB* track2 = allTrajInTPCFGD[jt];
        if(track->ParentID == track2->ID){
          if(InVOI(det, track2->PositionEnd) )
            trajOfInterest.push_back(track);
          break;
        }
      }
    }
  }

  //So at this point we have a vector of all true trajectories that start in the volume of interest and those
  //whose parents end in the volume of interest.  (This is to guard against unsaved trajectories resulting
  //in an apparent child being created outside the volume of interest, but corresponding to the interaction
  //that ended the trajectory in the volume of interest.)

  //We want to go through this list of trajectories, picking out pion trajectories and taking all trajectories with
  //a pi+/- as a parent and assembling them into Interactions.
  std::vector<AnaTrueParticleB*> pionTrajs;
  std::vector<Interaction*> interactions;

  for(UInt_t oai = 0; oai < trajOfInterest.size(); oai++){
    //If it's a pi+/- trajectory beginning in the volume of interest, save to pionTrajs.
    if (trajOfInterest[oai]->PDG == 211 || trajOfInterest[oai]->PDG == -211){
      if(InVOI(det, trajOfInterest[oai]->Position) )
        pionTrajs.push_back(trajOfInterest[oai]);
    }
    
    //If it lists a pion as a parent, assign it to an Interaction.
    if(trajOfInterest[oai]->ParentPDG == 211 || trajOfInterest[oai]->ParentPDG == -211){
      
      AnaTrueParticleB* parent = GetParent(trajOfInterest[oai],allTrajInTPCFGD,nTraj);
      Bool_t matchFound = false;
      for (UInt_t jsi = 0; jsi < interactions.size(); jsi++){        
        if (interactions[jsi]->IncludesTrajectory(trajOfInterest[oai])){          
          interactions[jsi]->AddTrajectory(trajOfInterest[oai],parent);
          matchFound = true;
          break;          
        }
      }
            
      //If an existing interaction does not match the trajectory, make a new interaction for
      //it to reside in, and add that to the interactions vector.
      if(!matchFound){        
        Interaction *newInteraction = new Interaction(trajOfInterest[oai], parent);
        interactions.push_back(newInteraction);
      }              
    } 
  }
      
      
  //So at this point we have all pion trajectories identified, and all non-primary trajectories assembled into
  //interactions.
  //Now, loop through the pion trajectories and the interactions, and assign them to a specific parent pion.
  //Then, use those trajectories to determine the secondary interaction type of the pion.
  //Loop over the pions.
  std::vector< std::pair<AnaTrueParticleB*,Int_t> > pionTrajAndInteraction;
      
  //Might as well count the interactions we're interested in now, so we can fill arrays later.
  Int_t nInteractions = 0;
  for (UInt_t p = 0; p < pionTrajs.size(); p++){
    
    AnaTrueParticleB* pionTraj = pionTrajs[p];
    TLorentzVector pos(pionTraj->Position[0],pionTraj->Position[1],pionTraj->Position[2],pionTraj->Position[3]);
    TLorentzVector posEnd(pionTraj->PositionEnd[0],pionTraj->PositionEnd[1],pionTraj->PositionEnd[2],pionTraj->PositionEnd[3]);
    //Only look for the interaction type if the pion trajectory ends inside of the volume of interest.    
#if (JMDEBUGVERSION == 0) || (JMDEBUGVERSION == 2) || (JMDEBUGVERSION == 3)
    if(InVOI(sel.GetDetectorFV(branch), pionTraj->PositionEnd)){
      
      //If there are two separate VOIs, since tracking ends at the end of the VOI, we
      //want to consider any pion trajectory that ends outside the VOI it started in
      //as having "escaped" (what this if statement elses to.)
#elif JMDEBUGVERSION == 1      
    if( (InVOI1( pionTraj->Position ) && InVOI1( pionTraj->PositionEnd )) || (InVOI2( pionTraj->Position ) && InVOI2( pionTraj->PositionEnd ))){
#endif
            
      //Define a vector to store the (relevant) interactions belonging to this pion.
      std::vector<Interaction*> thisPionInteractions;
      
      //Loop over the interactions vector and fill thisPionInteractions with the ones corresponding to 
      //this pion trajectory.
      for (UInt_t i = 0; i < interactions.size(); i++){
        
        //If the interaction lists the pion as a parent, add it to the list.
        //Also, we're only interested in interactions at or after the end of the pion trajectory.
        if (interactions[i]->parentID == pionTraj->ID
            && interactions[i]->position[3] >= pionTraj->PositionEnd[3]){
          thisPionInteractions.push_back(interactions[i]);
        }
      }
      
      //So, at this point should have all the Interactions considered a result of this pion.
      //Use this vector of interactions and the pion's position to determine its interaction type.
      //Go through the interactions:
      //A mu of correct charge originating from the trajectory end point => decay.
      //A mu originating in a later interactions indicates decay of a pion that need to
      //count as part of the original secondary interactions.
      //If it's pi0 decay within 10^-4 ns of the end point, add a pi0.
      //If it's outside of that time range, attribute it to a pion of the same type as the
      //incoming pion undergoing a charge exchange (so add another pi+/-)
      //Once have the full count of relevant particle types, use that to determine the
      //secondary interaction type.
      //Start by defining the counters.
      Int_t NPiPlus = 0;
      Int_t NPiZero = 0;
      Int_t NPiMinus = 0;
      Int_t NMuMinus = 0;
      Int_t NMuPlus = 0;
      Int_t NExotic = 0;
      
      for (UInt_t i = 0; i < thisPionInteractions.size(); i++){
        
        TLorentzVector thisPos(thisPionInteractions[i]->position[0],thisPionInteractions[i]->position[1],thisPionInteractions[i]->position[2],thisPionInteractions[i]->position[3]);
        //First check the interaction that shares the position with the end of the pion trajectory.
        //note that this condition only works if posEnd is a TLorentzVector!
        if(posEnd == thisPos){
          
          NPiPlus += thisPionInteractions[i]->NumberOfParticleType(211);
          NPiZero += thisPionInteractions[i]->NumberOfParticleType(111) 
            + thisPionInteractions[i]->NPiZeroFromDecayProducts();
          NPiMinus += thisPionInteractions[i]->NumberOfParticleType(-211);
          NMuPlus += thisPionInteractions[i]->NumberOfParticleType(-13);
          NMuMinus += thisPionInteractions[i]->NumberOfParticleType(13);
          NExotic += thisPionInteractions[i]->CountExoticParticles();	  
        }
        
        //Otherwise it's time separated.
        //In the below case, assume the particles considered below originate from 
        //something coming out of the original pion trajectory ending vertex (as opposed
        //to some tertiary or beyond interaction of unsaved trajectories, which is unlikely.)
        else{
          
          //A time separated mu+ implies an unsaved pi+ trajectory.
          NPiPlus += thisPionInteractions[i]->NumberOfParticleType(-13);
          
          //A time separated mu- implies an unsaved pi- trajectory.
          NPiMinus += thisPionInteractions[i]->NumberOfParticleType(13);
          
          //If a pi0 decay was within 10^-4 ns of the pion trajectory end,
          //count it as being a product of the interaction the pion trajectory ended in.
          //If there was a saved pi0 trajectory in this time region, assume
          //that it originated from a time separated interaction. 
          Int_t TSPiZero = 0;
          if(thisPionInteractions[i]->position[3] - pionTraj->PositionEnd[3] < 10E-4){
            
            //Consider the decay products as originating from a pi0 from the
            //original pion's interaction.
            NPiZero += thisPionInteractions[i]->NPiZeroFromDecayProducts();
            
            //Consider any saved pi0 as originating from a time separated
            //interaction of an unsaved charged pion trajectory.
            TSPiZero += thisPionInteractions[i]->NumberOfParticleType(111); 
            
          }
          
          //Otherwise, count it or any saved pi0 as "Time Separated" pi0, which
          //we later assume originated from a time separated interaction of an
          //unsaved pi+/- trajectory.
          else{
            TSPiZero += thisPionInteractions[i]->NumberOfParticleType(111) 
              + thisPionInteractions[i]->NPiZeroFromDecayProducts();
          }
          
          //Any time separated pions or exotics are likely collectively due
          //to an unsaved pion trajectory of the same charge as the original pion
          //trajectory.  So count all pi+, time separated pi0, pi-, and exotics, and if the amount is
          //greater than 0, add a pion of the same charge as the parent of the
          //interaction.
          if (thisPionInteractions[i]->NumberOfParticleType(211)
              + TSPiZero
              + thisPionInteractions[i]->NumberOfParticleType(-211)
              + thisPionInteractions[i]->CountExoticParticles() > 0){
            
            if(pionTraj->PDG == 211){
              
              NPiPlus += 1;
            }
            
            else{
              NPiMinus += 1;
            }
            
            
          }
          
        }
        
      }
      
      //clean memory
      std::vector<Interaction*> interactions;
      for (std::vector<Interaction*>::iterator it = interactions.begin(); it != interactions.end(); it++){
        delete (*it); 
        
      }
      
      interactions.clear();
      
      
      
      //At this point we have all of the relevant particles in the pion's interaction counted.
      //Use these counts to determine the interaction type.
      Int_t interactionType;
      
      //First take care of decays.
      if((pionTraj->PDG == 211 && NMuPlus == 1)
         || (pionTraj->PDG == -211 && NMuMinus == 1)){
        
        interactionType = 3;
        
      }
      
      //Now slot anything with exotic particles in it into Other.
      else if(NExotic > 0){
        interactionType = 7;
      }
      
      //Multi-pi.
      //If it has more than 1 pion total.
      else if (NPiPlus + NPiZero + NPiMinus > 1){	
        interactionType = 5;
      }
      
      //CEX
      //Since we've excluded multi-pi events, only have to check the number
      //of the desired pion here.
      //This is CEX for both pi+ and pi-
      else if (NPiZero == 1){
        interactionType = 1;
      }
      
      //DCEX
      //Has a pi- if parent was pi+, or has a pi+ if parent was pi-
      else if ((pionTraj->PDG == 211 && NPiMinus == 1)
               || (pionTraj->PDG == -211 && NPiPlus == 1)){
        interactionType = 2;
      }
      
      //QE
      //If there is one pion of the same kind coming in going out.
      else if ((pionTraj->PDG == 211 && NPiPlus == 1)
               || (pionTraj->PDG == -211 && NPiMinus == 1)){
        interactionType = 4;
      }
      
      //ABS
      //If there's no pions or muons, it's absorption.
      else if (NPiPlus + NPiZero + NPiMinus
               + NMuMinus + NMuPlus == 0){
        interactionType = 0;
      }
      
      //As a catch all for weird stuff that shouldn't exist
      //(everything is expected to have been caught above.)
      else{
        interactionType = 7;
      } 
      
      
      //With the interaction type now determined, write this pion trajectory and its
      //interaction type out to the vector that stores them.
      pionTrajAndInteraction.push_back(std::make_pair(pionTraj,interactionType));
    } 
    
    //If it's not in the volume of interest, assign it the "Escaped" interaction type (8).
    else{
      pionTrajAndInteraction.push_back(std::make_pair(pionTraj,8));
      
    }
    
    //Now, check the recently just added pair to see whether it is one of the interaction
    //types we're interested in.  If it is, count as one of the nInteractions.
    Int_t thisIntType =  pionTrajAndInteraction.back().second;
    
    //If it's one of the interactions we're interested in, count it and
    //store its information for later processing.
    if(thisIntType == 0 || thisIntType == 1 || thisIntType == 4){
      
      nInteractions++;
    }
    
    
  } //Close loop over pion trajectories.
        
  //So, at this point we now have a vector of pion trajectories paired with the interaction that ends them
  //(or if this interaction occured outside the volume of interest, indication that they escaped.)
  //Now we need to step these pions through the detector.
  //Propagate from beginning to the end of the trajectory.  Elastic interactions aren't accounted for,
  //but the error due to that should be small.  Propagates out to length between start and end point,
  //either stopping when it hits that length or when it goes outside of the volume of interest.
        
  std::vector<Float_t> allPionSteps;
  
  Float_t AllPionsMCSumNSigmaStepLength = 0.0;
  Float_t AllPionsDataSumNSigmaStepLength = 0.0;
  
  //Set up as many variables as can.
  Int_t nPions = (Int_t)pionTrajAndInteraction.size();
  Bool_t* pionType = new Bool_t[(UInt_t)nPions];
  Int_t* nSteps = new Int_t[(UInt_t)nPions];
  Float_t* initMom = new Float_t[(UInt_t)nPions];
  Float_t* pInteraction =  new Float_t[(UInt_t)nInteractions];
  Int_t* typeInteraction = new Int_t[(UInt_t)nInteractions];
  Int_t interactionsSoFar = 0; // Since not every pion has an interaction to count.
  
  //Variables for validation purposes.
  std::vector<Float_t> testFinalMom;
  std::vector<TLorentzVector> testFinalPos;
  std::vector<Int_t> testInteractionType;
  Int_t* pionID = new Int_t[nPions];
  AnaTrueParticleB** TrueParticles = new AnaTrueParticleB*[nPions];
  AnaTrueParticleB** InteractionTrueParticles = new AnaTrueParticleB*[nInteractions];
  
  for(UInt_t ip = 0; ip < pionTrajAndInteraction.size(); ip++){
    
    AnaTrueParticleB* pionTraj = pionTrajAndInteraction[ip].first;
    Int_t intCode = pionTrajAndInteraction[ip].second;
    
    //Set whether it is a pi+ or a pi-.
    if(pionTraj->PDG == 211){
      
      pionType[ip] = true;
      
    }
    else{
      
      pionType[ip] = false;
      
    }
    
    //Store the initial momentum.
    //Remember: In highland the magnitude of the momentum
    //is stored as Momentum, and the direction elsewhere.
    initMom[ip] = pionTraj->Momentum;
    
    //Store the pion trajectory ID, for validation purposes.
    pionID[ip] = pionTraj->ID;

    TrueParticles[ip] = pionTraj;
    
    TVector3 dir(pionTraj->Direction[0], pionTraj->Direction[1], pionTraj->Direction[2]);
    TLorentzVector pos(pionTraj->Position[0],pionTraj->Position[1],pionTraj->Position[2],pionTraj->Position[3]);
    TLorentzVector posEnd(pionTraj->PositionEnd[0],pionTraj->PositionEnd[1],pionTraj->PositionEnd[2],pionTraj->PositionEnd[3]);
	
    SteppingResult sr = StepBetweenPoints(det, (pionTraj->PDG)/211, //Charge
                                          pos,
                                          (pionTraj->Momentum)*dir,
                                          posEnd,
                                          0.1, //Step length, 0.1 mm
                                          pionSIGeom); //The geometry.
	
    //Store the number of steps for this pion.
    nSteps[ip] = sr.stepLengths.size();
    //Fill the test variables.
    testFinalMom.push_back(sr.finalMom);
	
    testFinalPos.push_back(sr.finalPos);

    testInteractionType.push_back(intCode);
	
    //If it's one of the interactions we're interested in, store the information
    //we need to store to generate weights.
    if(intCode == 0 || intCode == 1 || intCode == 4){
      
	  pInteraction[interactionsSoFar] = sr.finalMom;
	  
	  //Get the material this interaction occurred in.
	  //(Use the true MC end as opposed to the propagated one, which is not exact.
	  //The interaction necessarily occurred at the end of the trajectory
	  TGeoNode *node = pionSIGeom->FindNode(pionTraj->PositionEnd[0],
                                            pionTraj->PositionEnd[1],
                                            pionTraj->PositionEnd[2]);
	  
	  TGeoVolume *volume = node->GetVolume();
	  TGeoMaterial *material = volume->GetMaterial();
	  std::string materialName = material->GetName();
	  
	  Int_t materialSeries = GetMaterialSeries(materialName);
	  
	  
	  //Get the pionType Series to add to the interaction type.
	  //0 for pi+, 100 for pi-
	  //Int_t pionTypeSeries = (100)*(1-pionType[ip]);
	  //pionType == true for pi+, false for pi-
	  Int_t pionTypeSeries = 0;
	  if(!pionType[ip]){
	    pionTypeSeries = 100;
	  }
	  
	  
	  typeInteraction[interactionsSoFar] = intCode + materialSeries + pionTypeSeries;
	  
      InteractionTrueParticles[interactionsSoFar] = pionTraj;
      
	  //Increase the interaction counter to move on to
      //the next element.
	  interactionsSoFar++;
    }
	
    
	
    //Loop over all the steps for this pion, and add them to allPionSteps
    for(UInt_t sIdx = 0; sIdx < sr.stepLengths.size() ; sIdx++){
      
	  allPionSteps.push_back(sr.stepLengths[sIdx]);
      
    }
	
    //Add this pion's MC and Data sum N*sigma*StepLength to the total for
    //all pions in the event.
    AllPionsMCSumNSigmaStepLength += sr.MCSumNSigmaStepLength;
    AllPionsDataSumNSigmaStepLength += sr.DataSumNSigmaStepLength;
	
	
  }

  
  //Can't fill some output variables until know everything there is to know about the event.  Do that here.
  Int_t totalSteps = (Int_t)allPionSteps.size();
  
  //Can now create and fill the stepLengths array.
  Float_t* stepLengths = new Float_t[totalSteps];
  
  for(UInt_t i = 0; i < allPionSteps.size(); i++){
    
    stepLengths[i] = allPionSteps[i];
  }
  
  
  //Use the Step sums and the interactions arrays to compute the MC to data weights.
  //Start with the weight portion from the stepping.
  Float_t weightMCToData = TMath::Exp(-(AllPionsDataSumNSigmaStepLength - AllPionsMCSumNSigmaStepLength));
  
  //Now, go through the interactions and multiply the exponential by their contributions.
  for(Int_t ni = 0; ni < nInteractions; ni++){
    
    Int_t siType = typeInteraction[ni];
    Float_t siMom = pInteraction[ni];
    
    int momBin = int(siMom/10);
    if (momBin>800) momBin = 800;
    
    Double_t dataXSec = Interpolate(PiXSec::xsec_data[siType][momBin], PiXSec::xsec_data[siType][momBin+1],momBin,siMom);
    Double_t mcXSec = Interpolate(PiXSec::xsec_MC[siType][momBin], PiXSec::xsec_MC[siType][momBin+1],momBin,siMom);
    
    //Potential 2000 MeV/c cutoff: If want not to weight pions above 2000 MeV/c,
    //uncomment the next two commented lines.
    //if(pInteraction[ni] < 2000.0){
    if(mcXSec != 0.0){
      weightMCToData *= dataXSec/mcXSec;
    }
    
    //}
  }
    
  //Now all the variables that are needed have been filled.  They need to be written out to the microtree.
  //Pass them to our ad-hoc object.  May have a better solution later.
  //PionInteractionSystematic result;
  //This is declared at the beginning of this function now, so that the
  //geometry initialization code is called before try to use the geometry.
  result->nPions = nPions;
  result->pionType = pionType;
  result->totalSteps = totalSteps;
  result->nSteps = nSteps;
  result->initMom = initMom;
  result->stepLengths = stepLengths;
  result->nInteractions = nInteractions;
  result->pInteraction = pInteraction;
  result->typeInteraction = typeInteraction;
  result->testFinalMom = testFinalMom;
  result->testFinalPos = testFinalPos;
  result->testInteractionType = testInteractionType;
  result->weightMCToData = weightMCToData;
  result->MCSumNSigmaStepLength = AllPionsMCSumNSigmaStepLength;
  result->DataSumNSigmaStepLength = AllPionsDataSumNSigmaStepLength;
  result->MCProbNoInt = TMath::Exp(-AllPionsMCSumNSigmaStepLength);
  result->DataProbNoInt = TMath::Exp(-AllPionsDataSumNSigmaStepLength);
  result->pionID = pionID;
  result->TrueParticles = TrueParticles;
  result->InteractionTrueParticles = InteractionTrueParticles;
  
  return result;
    
}

//*******************************************************************************
AnaTrueParticleB* GetParent(AnaTrueParticleB* track, AnaTrueParticleB* allTrajInTPCFGD[], Int_t nTraj){
//*******************************************************************************

  for (Int_t it = 0; it < nTraj; it++){    
    AnaTrueParticleB* track2 = allTrajInTPCFGD[it];

    if (track->ParentID == track2->ID) return  track2;
  }
  
  return NULL;
}
