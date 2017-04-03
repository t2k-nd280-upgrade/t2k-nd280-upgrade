#include "DeDxMap.hxx"
#include <recpack/Definitions.h>

//*******************************************************
Recpack::DeDxMap::DeDxMap(double dedx_min):DynamicProperty(RP::de_dx_map) {
//*******************************************************

  _de_dx_min = dedx_min;

   //  _de_dx_min_def1 = 0.195*units::MeV/units::mm;

  _melectron = 0.51099906 * units::MeV;
  _mproton = 938.27231 * units::MeV;
  _mmuon = 105.65837 * units::MeV;
  _mpion = 139.57018 * units::MeV;
  _mkaon = 493.677 * units::MeV;

  // map for muons in scintillator
  //one actually needs a map here with consistent values with respect to particular MIP one
  //de_dx values for different materials will be achieved by scaling the MIP value
  _pbins[0]  = 0.;               _de_dx[0]  = 18.44*units::MeV/units::mm;
  _pbins[1]  = 5*units::MeV;      _de_dx[1]  = 15.5*units::MeV/units::mm;  
  _pbins[2]  = 10*units::MeV;     _de_dx[2]  = 6.93*units::MeV/units::mm;
  _pbins[3]  = 15*units::MeV;     _de_dx[3]  = 4.02*units::MeV/units::mm;
  _pbins[4]  = 20*units::MeV;     _de_dx[4]  = 2.671*units::MeV/units::mm;
  _pbins[5]  = 25*units::MeV;     _de_dx[5]  = 1.933*units::MeV/units::mm;
  _pbins[6]  = 30*units::MeV;     _de_dx[6]  = 1.482*units::MeV/units::mm;
  _pbins[7]  = 35*units::MeV;     _de_dx[7]  = 1.187*units::MeV/units::mm;
  _pbins[8]  = 40*units::MeV;     _de_dx[8]  = 0.982*units::MeV/units::mm;
  _pbins[9]  = 45*units::MeV;     _de_dx[9]  = 0.833*units::MeV/units::mm;
  _pbins[10] = 50*units::MeV;     _de_dx[10] = 0.723*units::MeV/units::mm;
  _pbins[11] = 55*units::MeV;     _de_dx[11] = 0.638*units::MeV/units::mm;  
  _pbins[12] = 60*units::MeV;     _de_dx[12] = 0.572*units::MeV/units::mm;
  _pbins[13] = 65*units::MeV;     _de_dx[13] = 0.519*units::MeV/units::mm;
  _pbins[14] = 70*units::MeV;     _de_dx[14] = 0.476*units::MeV/units::mm;
  _pbins[15] = 75*units::MeV;     _de_dx[15] = 0.441*units::MeV/units::mm;
  _pbins[16] = 80*units::MeV;     _de_dx[16] = 0.412*units::MeV/units::mm;
  _pbins[17] = 85*units::MeV;     _de_dx[17] = 0.387*units::MeV/units::mm;
  _pbins[18] = 90*units::MeV;     _de_dx[18] = 0.366*units::MeV/units::mm;
  _pbins[19] = 95*units::MeV;     _de_dx[19] = 0.349*units::MeV/units::mm; 	
  _pbins[20] = 100*units::MeV;    _de_dx[20] = 0.310*units::MeV/units::mm;
  _pbins[21] = 120*units::MeV;    _de_dx[21] = 0.270*units::MeV/units::mm;
  _pbins[22] = 140*units::MeV;    _de_dx[22] = 0.244*units::MeV/units::mm;
  _pbins[23] = 160*units::MeV;    _de_dx[23] = 0.228*units::MeV/units::mm;
  _pbins[24] = 180*units::MeV;    _de_dx[24] = 0.217*units::MeV/units::mm;
  _pbins[25] = 200*units::MeV;    _de_dx[25] = 0.202*units::MeV/units::mm;
  _pbins[26] = 300*units::MeV;    _de_dx[26] = 0.195*units::MeV/units::mm;
  _pbins[27] = 400*units::MeV;    _de_dx[27] = 0.196*units::MeV/units::mm;
  _pbins[28] = 500*units::MeV;    _de_dx[28] = 0.199*units::MeV/units::mm;
  _pbins[29] = 600*units::MeV;    _de_dx[29] = 0.202*units::MeV/units::mm;
  _pbins[30] = 700*units::MeV;    _de_dx[30] = 0.206*units::MeV/units::mm;
  _pbins[31] = 800*units::MeV;    _de_dx[31] = 0.209*units::MeV/units::mm;
  _pbins[32] = 900*units::MeV;    _de_dx[32] = 0.212*units::MeV/units::mm;
  _pbins[33] = 1000*units::MeV;   _de_dx[33] = 0.224*units::MeV/units::mm;
  _pbins[34] = 2000*units::MeV;   _de_dx[34] = 0.242*units::MeV/units::mm;
  _pbins[35] = 3000*units::MeV;   _de_dx[35] = 0.253*units::MeV/units::mm;
  _pbins[36] = 4000*units::MeV;   _de_dx[36] = 0.261*units::MeV/units::mm;
  _pbins[37] = 5000*units::MeV;   _de_dx[37] = 0.274*units::MeV/units::mm;

  

  /// map for electrons in scintillator

  _de_dx_elec[0] = 2.400*units::MeV/units::mm;
  _de_dx_elec[1] = 0.245*units::MeV/units::mm;
  _de_dx_elec[2] = 0.249*units::MeV/units::mm;  
  _de_dx_elec[3] = 0.257*units::MeV/units::mm;
  _de_dx_elec[4] = 0.266*units::MeV/units::mm;
  _de_dx_elec[5] = 0.271*units::MeV/units::mm;
  _de_dx_elec[6] = 0.276*units::MeV/units::mm;
  _de_dx_elec[7] = 0.279*units::MeV/units::mm;
  _de_dx_elec[8] = 0.283*units::MeV/units::mm;
  _de_dx_elec[9] = 0.286*units::MeV/units::mm;
  _de_dx_elec[10] = 0.289*units::MeV/units::mm;
  _de_dx_elec[11] = 0.291*units::MeV/units::mm;
  _de_dx_elec[12] = 0.293*units::MeV/units::mm;
  _de_dx_elec[13] = 0.295*units::MeV/units::mm;
  _de_dx_elec[14] = 0.297*units::MeV/units::mm;
  _de_dx_elec[15] = 0.298*units::MeV/units::mm;
  _de_dx_elec[16] = 0.299*units::MeV/units::mm;
  _de_dx_elec[17] = 0.301*units::MeV/units::mm;
  _de_dx_elec[18] = 0.302*units::MeV/units::mm;
  _de_dx_elec[19] = 0.303*units::MeV/units::mm;

  _de_dx_elec[20] = 0.304*units::MeV/units::mm;
  _de_dx_elec[21] = 0.307*units::MeV/units::mm;
  _de_dx_elec[22] = 0.309*units::MeV/units::mm;
  _de_dx_elec[23] = 0.311*units::MeV/units::mm;
  _de_dx_elec[24] = 0.312*units::MeV/units::mm;


  _de_dx_elec[25] = 0.315*units::MeV/units::mm;
  _de_dx_elec[26] = 0.318*units::MeV/units::mm;
  _de_dx_elec[27] = 0.319*units::MeV/units::mm;
  _de_dx_elec[28] = 0.320*units::MeV/units::mm;
  _de_dx_elec[29] = 0.321*units::MeV/units::mm;
  _de_dx_elec[30] = 0.321*units::MeV/units::mm;
  _de_dx_elec[31] = 0.321*units::MeV/units::mm;
  _de_dx_elec[32] = 0.322*units::MeV/units::mm;

  _de_dx_elec[33] = 0.322*units::MeV/units::mm;
  _de_dx_elec[34] = 0.323*units::MeV/units::mm;
  _de_dx_elec[35] = 0.323*units::MeV/units::mm;
  _de_dx_elec[36] = 0.323*units::MeV/units::mm;
  _de_dx_elec[37] = 0.327*units::MeV/units::mm;



}


//*******************************************************
double Recpack::DeDxMap::property(const State& state) const{
//*******************************************************


  double de_dx = 0;

  double qop = state.vector()[state.dim()-1];
  
  double p=10000;  // default momentum
  if (qop !=0) 
    p = 1/(fabs(qop));
  
  Key PID= "";
  if (state.names().has_key(RP::PID)) 
    PID = state.name(RP::PID);

  // Use momentum based on particle type

  double p_pid = p;
  
  // if none of this Hypothesis it will assume the Muon
  if (PID=="Muon")
    p_pid = p;
  else if (PID=="Electron")
    p_pid = p;
  else if (PID=="Pion")
    p_pid = p*_mmuon/_mpion;
  else if (PID=="Proton")
    p_pid = p*_mmuon/_mproton;
  else if (PID=="Kaon")
    p_pid = p*_mmuon/_mkaon;
  
  // Find the bin number
  int pbin = 0;
  if (p_pid>_pbins[NPBINS1-1])
    pbin = NPBINS1-1;
  else if (p_pid>_pbins[0]){
    for (int i=0;i<NPBINS1-1;i++){
      if (p_pid>_pbins[i] && p_pid<_pbins[i+1]){
        pbin = i;
        break;
      }
    }
  }
  
  //if it is an elctron
  if (PID=="Electron"){
    //some bin found
    if( pbin < NPBINS1-1 ) 
      de_dx = - (  _de_dx_elec[pbin]+ (p_pid - _pbins[pbin])*(_de_dx_elec[pbin]-_de_dx_elec[pbin+1])/(_pbins[pbin]-_pbins[pbin+1]))*_de_dx_min/_de_dx_min_def1;
    else 
      de_dx = - _de_dx_elec[pbin]*_de_dx_min/_de_dx_min_def1;
 
  }
  //all other --> mass scaling from muon map
  else{	 
    //some bin found
    if( pbin < NPBINS1-1 ) 
      de_dx = - (  _de_dx[pbin]+ (p_pid - _pbins[pbin])*(_de_dx[pbin]-_de_dx[pbin+1])/(_pbins[pbin]-_pbins[pbin+1]))*_de_dx_min/_de_dx_min_def1;
    else 
      de_dx = - _de_dx[pbin]*_de_dx_min/_de_dx_min_def1;
 

  }    
  
  if (verbosity(VERBOSE))
    std::cout << "DeDxMap: dE/dx min = " << _de_dx_min << ", pid=" << PID << ", p=" << p << ", p'=" << p_pid << ", pbin=" << pbin << ", p0=" << _pbins[pbin] << " --> " << de_dx << std::endl;

  return de_dx;

}
