#include "AnaTreeConverterEvent.hxx"
#include "InputManager.hxx"
#include "BasicUtils.hxx"
#include "Units.hxx"
#include "ND280UPAnalysisUtils.hxx"
#include "PIDUtils.hxx"
//********************************************************************
AnaTreeConverterEvent::AnaTreeConverterEvent():InputConverter("ND280upEvents"){
  //********************************************************************

  _spill = NULL;

  _isMC = false;
  _softwareVersion = "";

  _previousFile = "";
  _previousRunID = -1;
  _previousSubrunID = -1;
  _previousRefEventID = -1;
  hefficiency_target=NULL;

}

//********************************************************************
bool AnaTreeConverterEvent::Initialize(){
  //********************************************************************
  AddChain("ND280upEvents");

  ND280upEvents = GetChain("ND280upEvents");

  fChain = ND280upEvents;

  // Set branch addresses and branch pointers
  nd280UpEvent = new TND280UpEvent();
  if (!ND280upEvents) {
    std::cout<<"non fChain"<<std::endl;
    return false;}
  fCurrent = -1;
  //-------------- Header ---------------------------------
  if (ND280upEvents) {
    ND280upEvents->SetBranchAddress("Event", &nd280UpEvent);;
  }

  fefficiency_target = new TFile("$HIGHLANDIOROOT/data/target_recon_efficiency.root");
  hefficiency_target = (TH2D*)fefficiency_target->Get("theta_length_Eff");

  _ECal_reco_eff = new BinnedParams(std::string(getenv("HIGHLANDIOROOT")) + "/data",
				    "ECal_recoEff", BinnedParams::k1D_SYMMETRIC); 
  
  _ECal_FGDmatch_eff = new BinnedParams(std::string(getenv("HIGHLANDIOROOT")) + "/data",
					"ECal_FGDmatchEff", BinnedParams::k1D_SYMMETRIC);
  
  _randomGen = new TRandom3();
  
  return true;
}

//********************************************************************
AnaTreeConverterEvent::~AnaTreeConverterEvent(){
  //********************************************************************

  if (!fChain) return;
  delete nd280UpEvent;
  if (ND280upEvents         ) delete   ND280upEvents          ->GetCurrentFile();
  if(hefficiency_target) delete hefficiency_target;
 
  if(fefficiency_target) delete fefficiency_target;
  
}

//****************************************************************************
bool AnaTreeConverterEvent::AddFileToTChain(const std::string& inputString){
  //****************************************************************************

  std::cout << "AnaTreeConverterEvent::AddFileToTChain(). Adding file: " << inputString << std::endl;


  // Chain only the directories we are interested in

  if (ND280upEvents            )          ND280upEvents->AddFile(inputString.c_str());

  // Read one entry from the tree tree such that Run and Subrun are available
  ND280upEvents->GetEntry(ND280upEvents->GetEntries() - 1);


  // The previous attributes
  _previousFile         = inputString;
  _previousRunID        = 0;
  _previousSubrunID     = 0;
  
  
  return 1;
}


//*****************************************************************************
Int_t AnaTreeConverterEvent::ReadEntries(Long64_t& entry) {
  //*****************************************************************************
  
  Int_t entry_temp = ND280upEvents->GetEntry(entry);

  //-------- Increase the cache size to speed up reading Branches ----------
  static bool first = false;
  if (first){
    if( ND280upEvents ) {
      ND280upEvents->SetCacheSize(200000000);
      ND280upEvents->AddBranchToCache("*",kTRUE);
    }
    first=false;
  }

  return entry_temp;
}

//*****************************************************************************
Int_t AnaTreeConverterEvent::GetSpill(Long64_t& entry, AnaSpillC*& spill){
  //*****************************************************************************

  static std::string currentfilename("");

  // Read contents of entry (which correspond to one Spill)
  if (!fChain) return 0;

  std::string filename(ND280upEvents->GetFile()->GetName());

  if( filename != currentfilename ) {
    std::cout << " Running on file: " << filename << std::endl;
    currentfilename = filename;
    
    //load geometry 
    //    ND::hgman().LoadGeometry(filename);
  }

  Int_t entry_temp = ReadEntries(entry);

  if (entry_temp > 0) {
    
    // Create an instance of the Spill
    spill = MakeSpill();
    
    // Cast it to AnaSpillB
    _spill = dynamic_cast<AnaSpill*>(spill);
    
    FillInfo(_spill,entry);
  }
  else{
    std::cout << "Failed in reading entry " << entry << std::endl;
  }
  

  entry++;

  if (entry%1000==0 || entry == _nentries)
    std::cout << "entry: " << entry << " of " << _nentries << " (" << (100*entry/_nentries) << "%)" << std::endl;
  return entry_temp;
}

//********************************************************************
void AnaTreeConverterEvent::IncrementPOTBySpill() {
  //********************************************************************
  
}

//*****************************************************************************
void AnaTreeConverterEvent::FillInfo(AnaSpill* spill, Int_t entry){
  //*****************************************************************************
  
  spill->EventInfo = MakeEventInfo();
  AnaEventInfo& info = *dynamic_cast<AnaEventInfo*>(spill->EventInfo);

  info.Run    = 0;
  info.SubRun = 0;
  info.Event  = entry;
  info.IsMC   = 1;

  spill->Beam = MakeBeam();

  
  // beam related information
  FillBeamInfo(dynamic_cast<AnaBeam*>(spill->Beam));


  // True vertex information
  FillTrueInfo(spill);

  AnaBunch* bunch = MakeBunch();
  // All information about each bunch
  FillBunchInfo(spill->TrueVertices, bunch);

  spill->Bunches.push_back(bunch);
}
//*****************************************************************************
void AnaTreeConverterEvent::FillBeamInfo(AnaBeam* beam){
  //*****************************************************************************

  beam->GoodSpill = true;
}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueInfo(AnaSpill* spill){
  //*****************************************************************************

  // Fill the true vertices vector
  spill->TrueVertices.clear();
  spill->TrueParticles.clear();
  int NTracks = nd280UpEvent->GetNTracks();
  int nParts = std::min((int)NMAXTRUEPARTICLES, NTracks);
  int NVertices = nd280UpEvent->GetNVertices();
  if(NVertices>1) return;

  int nVertices = std::min((int)NMAXTRUEVERTICES, 1);
  TND280UpVertex *nd280UpVertex = dynamic_cast<TND280UpVertex*>(nd280UpEvent->GetVertex(0));

  AnaTrueVertex* trueVertex = MakeTrueVertex();
  FillTrueVertexInfo(nd280UpVertex, trueVertex);
    
  trueVertex->nTrueParticles = 0;

  // allocate a big array. Once it is filled we will resize it
  anaUtils::CreateArray(trueVertex->TrueParticles, NMAXTRUEPARTICLES);

  AnaTrueParticleB* highestMomTrack=0;
  for (int i = 0; i < nd280UpEvent->GetNTracks(); i++) {
    TND280UpTrack *nd280UpTrack = dynamic_cast<TND280UpTrack*>(nd280UpEvent->GetTrack(i));

    AnaTrueParticleB* truePart = MakeTrueParticle();

    if (!highestMomTrack ||
	highestMomTrack->Momentum < truePart->Momentum)
      highestMomTrack = truePart;

    if ( (trueVertex->NuPDG == 14 && nd280UpTrack->GetPDG() == 13) ||
	 (trueVertex->NuPDG == 12 && nd280UpTrack->GetPDG() == 11) ||
	 (trueVertex->NuPDG == -14 && nd280UpTrack->GetPDG() == -13) ||
	 (trueVertex->NuPDG == -12 && nd280UpTrack->GetPDG() == -11) ) {
      if(nd280UpTrack->GetInitMom().Mag() > 0){
	//std::cout << "ERROR: LeptonMom is -999 (something wrong in RooTrackerVtx ?), setting it from TruthTrajectoryModule (" << truthTraj->InitMomentum.Vect().Mag() << "), for vertex ID " << true_vertex->ID << std::endl;
	trueVertex->LeptonMom = nd280UpTrack->GetInitMom().Mag();
	trueVertex->LeptonPDG = nd280UpTrack->GetPDG();
	trueVertex->Q2 = - (nd280UpTrack->GetInitMom() - nd280UpVertex->GetInTrack(0)->GetInitMom()).Mag2();
	trueVertex->LeptonMom = nd280UpTrack->GetInitMom().Mag();
           
      }
    } else if (  nd280UpTrack->GetPDG() == 2212 && nd280UpTrack->GetParentID() == 0) {
      // if many, take the first one, that should be the first primary one (not from FSI)
      if (trueVertex->ProtonMom < 0) {
	trueVertex->ProtonMom = nd280UpTrack->GetInitMom().Mag();
	if (trueVertex->ProtonMom > 0)
	  anaUtils::VectorToArray((1. / trueVertex->ProtonMom) * nd280UpTrack->GetInitMom(), trueVertex->ProtonDir);
      }
    } // end if proton
      // If no rooTrackerVtx, fill true pion vars, using the TruthTrajectoryModule (old way)
    else if (  (nd280UpTrack->GetPDG() == 211  || nd280UpTrack->GetPDG() == -211) && nd280UpTrack->GetParentID() == 0) {
      // if many, take the highest momentum one, that should be most likely to be reconstructed
      if (trueVertex->PionMom < nd280UpTrack->GetInitMom().Mag()) {
	trueVertex->PionMom = nd280UpTrack->GetInitMom().Mag();
	if (trueVertex->PionMom > 0)
	  anaUtils::VectorToArray((1. / trueVertex->PionMom) * nd280UpTrack->GetInitMom(), trueVertex->PionDir);
      }
    } // end if pion

    FillTrueParticleInfo(nd280UpVertex, nd280UpTrack, truePart);
    spill->TrueParticles.push_back(truePart);
    truePart->TrueVertex = trueVertex;
    trueVertex->TrueParticlesVect.push_back(truePart);

    int index = ParticleId::GetParticle(nd280UpTrack->GetPDG(),false);
    trueVertex->NPrimaryParticles[index]++;
    if (abs(nd280UpTrack->GetPDG()) > 1000 && abs(nd280UpTrack->GetPDG()) < 10000) trueVertex->NPrimaryParticles[ParticleId::kBaryons]++;
    if (abs(nd280UpTrack->GetPDG()) > 100 && abs(nd280UpTrack->GetPDG()) < 1000) trueVertex->NPrimaryParticles[ParticleId::kMesons]++;
    if (abs(nd280UpTrack->GetPDG()) > 10 && abs(nd280UpTrack->GetPDG()) < 19) trueVertex->NPrimaryParticles[ParticleId::kLeptons]++;
    if (nd280UpTrack->GetPDG() == +12 || nd280UpTrack->GetPDG() == +14 || nd280UpTrack->GetPDG() == +16) trueVertex->NPrimaryParticles[ParticleId::kNeutrinos]++;
    if (nd280UpTrack->GetPDG() == -12 || nd280UpTrack->GetPDG() == -14 || nd280UpTrack->GetPDG() == -16) trueVertex->NPrimaryParticles[ParticleId::kAntiNeutrinos]++;

    //delete nd280UpTrack;
  }

  if (highestMomTrack)
    if (highestMomTrack->DetCrossingsVect.size()>0)
      trueVertex->Detector = highestMomTrack->DetCrossingsVect[0]->Detector;

  trueVertex->NPrimaryParticles[ParticleId::kPions] = trueVertex->NPrimaryParticles[ParticleId::kPi0]   +
    trueVertex->NPrimaryParticles[ParticleId::kPiPos] +
    trueVertex->NPrimaryParticles[ParticleId::kPiNeg] ;
  trueVertex->NPrimaryParticles[ParticleId::kKaons] = trueVertex->NPrimaryParticles[ParticleId::kK0]     +
    trueVertex->NPrimaryParticles[ParticleId::kAntiK0] +
    trueVertex->NPrimaryParticles[ParticleId::kK0L]    +
    trueVertex->NPrimaryParticles[ParticleId::kK0S]    +
    trueVertex->NPrimaryParticles[ParticleId::kKPos]   +
    trueVertex->NPrimaryParticles[ParticleId::kKNeg]   ;
	  
  //delete nd280UpVertex;

  spill->TrueVertices.push_back(trueVertex);

}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueVertexInfo(TND280UpVertex* ndtrueVertex, AnaTrueVertex* trueVertex){
  //*****************************************************************************

  trueVertex->ID       = ndtrueVertex->GetVertexID();
  trueVertex->NuPDG    = ndtrueVertex->GetInTrack(0)->GetPDG();
  trueVertex->NuEnergy = ndtrueVertex->GetInTrack(0)->GetInitKinEnergy();
  trueVertex->NuMom    = ndtrueVertex->GetInTrack(0)->GetInitMom().Mag();
  trueVertex->NuDir[0] = ndtrueVertex->GetInTrack(0)->GetInitMom().X()/trueVertex->NuMom;
  trueVertex->NuDir[1] = ndtrueVertex->GetInTrack(0)->GetInitMom().Y()/trueVertex->NuMom;
  trueVertex->NuDir[2] = ndtrueVertex->GetInTrack(0)->GetInitMom().Z()/trueVertex->NuMom;
  trueVertex->TargetPDG= ndtrueVertex->GetInTrack(1)->GetPDG();

  trueVertex->Position[0] = ndtrueVertex->GetPosition().X(); //*units::mm;
  trueVertex->Position[1] = ndtrueVertex->GetPosition().Y();//*units::mm;
  trueVertex->Position[2] = ndtrueVertex->GetPosition().Z();//*units::mm;
  trueVertex->Position[3] = ndtrueVertex->GetTime();//*units::s;
    
  trueVertex->ReacCode  =ndtrueVertex->GetReacMode();

}


//*****************************************************************************
void AnaTreeConverterEvent::FillBunchInfo(std::vector<AnaTrueVertexB*>& TrueVertices, AnaBunch* bunch){
  //*****************************************************************************

  bunch->Bunch  = 1;
  bunch->Weight = 1;
 
  for (std::vector<AnaTrueVertexB*>::const_iterator it = TrueVertices.begin(); it != TrueVertices.end(); ++it) {

    std::vector<AnaTrueParticleB*> trueParticles = (*it)->TrueParticlesVect;
  
    for (std::vector<AnaTrueParticleB*>::const_iterator itp = trueParticles.begin(); itp != trueParticles.end(); ++itp) {
      AnaTrackB* part = NULL;
      if ( fabs( (*itp)->Charge )<1e-3 ) continue;
      if (*itp) {
        AnaTrack* part = dynamic_cast<AnaTrack*> (MakeTrack());

        Fill_Tracks_Recon_From_True((*itp), part);
        bunch->Particles.push_back(part);

      }
    }

    AnaVertexB* vertex = MakeVertex();
    FillVertexInfo(*it, vertex, 1);
    vertex->TrueVertex = *it;

    vertex->ParticlesVect = bunch->Particles;
    bunch->Vertices.push_back(vertex);
  }
}



//*****************************************************************************
void AnaTreeConverterEvent::FillVertexInfo(AnaTrueVertexB * truevertex, AnaVertexB* vertex, int bunch){
  //*****************************************************************************

  anaUtils::CopyArray(truevertex->Position,vertex->Position,4);
  vertex->Bunch         = bunch;
  truevertex->Bunch=bunch;

}
//*****************************************************************************
AnaTrueObjectC* AnaTreeConverterEvent::FindTrueParticle(Int_t g4id, std::vector<AnaTrueParticleB*>& trueParticles){
  //*****************************************************************************


  for (UInt_t i=0;i<trueParticles.size();i++){
    if (trueParticles[i]->ID == g4id) return trueParticles[i];
  }

  return NULL;
}

//*****************************************************************************
void AnaTreeConverterEvent::FillTrueParticleInfo(TND280UpVertex* trueVertex, TND280UpTrack* upTrack, AnaTrueParticleB* truePart){
  //*****************************************************************************
  
  if (upTrack == NULL ) {
    truePart->ID = 0;
    truePart->PDG = 0;
    truePart->ParentID = 0;
    anaUtils::VectorToArray(TLorentzVector(0, 0, 0, 0), truePart->Position);
    truePart->Momentum = 0;
    anaUtils::VectorToArray(TVector3(0, 0, 0), truePart->Direction);
    truePart->Charge = 0;
    truePart->VertexID = -1;
    return;
  }
  truePart->ID = upTrack->GetTrackID();
  truePart->PDG = upTrack->GetPDG();
  truePart->ParentID = upTrack->GetParentID();
  truePart->CosTheta=upTrack->GetInitCosTheta();
  double tmpp=-9999;
  double tmpd=9999;
  int ti=-1;
  int td=-1;
  if (upTrack->GetNPoints() > 0) {
    for (int ipt = 0; ipt < upTrack->GetNPoints(); ipt++) {

      if (upTrack->GetPoint(ipt)->GetTime() > tmpp ){
        ti=ipt;
        tmpp=upTrack->GetPoint(ipt)->GetTime();
      }
      if (upTrack->GetPoint(ipt)->GetTime() < tmpd) {
        td=ipt;
        tmpd=upTrack->GetPoint(ipt)->GetTime();
      }
    }
  }
  if(ti>=0 && td>=0){
    anaUtils::VectorToArray(upTrack->GetPoint(td)->GetPostPosition(), truePart->Position);
 
    anaUtils::VectorToArray(upTrack->GetPoint(ti)->GetPostPosition(), truePart->PositionEnd);
    truePart->PositionEnd[3]=upTrack->GetPoint(ti)->GetTime();
    truePart->Position[3]=upTrack->GetPoint(td)->GetTime();
    if(upTrack->GetPoint(ti)->GetMomentum().Mag()>0){
      truePart->stopped=0;
    }else{
      truePart->stopped=1;
    }
  }
  //}
  truePart->Momentum = upTrack->GetInitMom().Mag();
  double mass = 0.;
  if     (abs(upTrack->GetPDG())==211)  mass = 139.570; // pi+-
  else if(abs(upTrack->GetPDG())==13)   mass = 105.658; // muon
  else if(abs(upTrack->GetPDG())==2212) mass = 938.272; // proton
  else if(abs(upTrack->GetPDG())==11)   mass = 0.511;   // electron

  anaUtils::VectorToArray((1 / truePart->Momentum)*upTrack->GetInitMom(), truePart->Direction);
  truePart->Charge = upTrack->GetCharge();
  truePart->VertexID = trueVertex->GetVertexID();
  truePart->Length=upTrack->GetSDLength();
  truePart->EDeposit=upTrack->GetSDTotalEnergyDeposit();
  truePart->EKin=mass+upTrack->GetInitKinEnergy();
  FindSegments(upTrack,truePart);

  
}
void AnaTreeConverterEvent::FindSegments(TND280UpTrack* upTrack,AnaTrueParticleB* truePart){

  double TPCUp1Length = 0;
  double TPCUp2Length = 0;
  double TPCDown1Length = 0;
  double TPCDown2Length = 0;
  double ForwTPC1Length = 0;
  double ForwTPC2Length = 0;
  double ForwTPC3Length = 0;
  double Target1Length = 0;
  double Target2Length = 0;
  double FGD1Length = 0;
  double FGD2Length = 0;
  double DsECalLength = 0;
  double P0DECalLength = 0;
  double BrlECalLength = 0;
  double ToFLength = 0;

  double TPCUp1Z = 9999;
  double TPCUp2Z = 9999;
  double TPCDown1Z = 9999;
  double TPCDown2Z = 9999;
  double ForwTPC1Z = 9999;
  double ForwTPC2Z = 9999;
  double ForwTPC3Z = 9999;
  double Target1Z = 9999;
  double Target2Z = 9999;
  double FGD1Z = 9999;
  double FGD2Z = 9999;
  double DsECalZ = 9999;
  double P0DECalZ = 9999;
  double BrlECalZ = 9999;
  //double ToFZ = 9999;
  double ToFTopUpZ = 9999;
  double ToFTopDownZ = 9999;
  double ToFBotUpZ = 9999;
  double ToFBotDownZ = 9999;
  double ToFLeftUpZ = 9999;
  double ToFLeftDownZ = 9999;
  double ToFRightUpZ = 9999;
  double ToFRightDownZ = 9999;
  double ToFBackUpZ = 9999;
  double ToFBackDownZ = 9999;
  double ToFFrontUpZ = 9999;
  double ToFFrontDownZ = 9999;

  double TPCUp1fZ = -9999;
  double TPCUp2fZ = -9999;
  double TPCDown1fZ = -9999;
  double TPCDown2fZ = -9999;
  double ForwTPC1fZ = -9999;
  double ForwTPC2fZ = -9999;
  double ForwTPC3fZ = -9999;
  double Target1fZ = -9999;
  double Target2fZ = -9999;
  double FGD1fZ = -9999;
  double FGD2fZ = -9999;
  double DsECalfZ = -9999;
  double P0DECalfZ = -9999;
  double BrlECalfZ = -9999;
  //double ToFfZ = -9999;
  double ToFTopUpfZ = -9999;
  double ToFTopDownfZ = -9999;
  double ToFBotUpfZ = -9999;
  double ToFBotDownfZ = -9999;
  double ToFLeftUpfZ = -9999;
  double ToFLeftDownfZ = -9999;
  double ToFRightUpfZ = -9999;
  double ToFRightDownfZ = -9999;
  double ToFBackUpfZ = -9999;
  double ToFBackDownfZ = -9999;
  double ToFFrontUpfZ = -9999;
  double ToFFrontDownfZ = -9999;

  double TPCUp1LengthYZ = 0;
  double TPCUp2LengthYZ = 0;
  double TPCDown1LengthYZ = 0;
  double TPCDown2LengthYZ = 0;
  double ForwTPC1LengthYZ = 0;
  double ForwTPC2LengthYZ = 0;
  double ForwTPC3LengthYZ = 0;
  double Target1LengthYZ = 0;
  double Target2LengthYZ = 0;
  double FGD1LengthYZ = 0;
  double FGD2LengthYZ = 0;
  double DsECalLengthYZ = 0;
  double P0DECalLengthYZ = 0;
  double BrlECalLengthYZ = 0;
  double ToFLengthYZ = 0;

  double TPCUp1Edep = 0;
  double TPCUp2Edep = 0;
  double TPCDown1Edep = 0;
  double TPCDown2Edep = 0;
  double ForwTPC1Edep = 0;
  double ForwTPC2Edep = 0;
  double ForwTPC3Edep = 0;
  double Target1Edep = 0;
  double Target2Edep = 0;
  double FGD1Edep = 0;
  double FGD2Edep = 0;
  double DsECalEdep = 0;
  double P0DECalEdep = 0;
  double BrlECalEdep = 0;
  double ToFEdep = 0;

  TND280UpTrackPoint* lastTPCUp1 = NULL;
  TND280UpTrackPoint* lastTPCUp2 = NULL;
  TND280UpTrackPoint* lastTPCDown1 = NULL;
  TND280UpTrackPoint* lastTPCDown2 = NULL;
  TND280UpTrackPoint* lastForwTPC1 = NULL;
  TND280UpTrackPoint* lastForwTPC2 = NULL;
  TND280UpTrackPoint* lastForwTPC3 = NULL;
  TND280UpTrackPoint* lastTarget1 = NULL;
  TND280UpTrackPoint* lastTarget2 = NULL;
  TND280UpTrackPoint* lastFGD1 = NULL;
  TND280UpTrackPoint* lastFGD2 = NULL;
  TND280UpTrackPoint* lastDsECal = NULL;
  TND280UpTrackPoint* lastP0DECal = NULL;
  TND280UpTrackPoint* lastBrlECal = NULL;
  //TND280UpTrackPoint* lastToF = NULL;
  TND280UpTrackPoint* lastToFTopUp = NULL;
  TND280UpTrackPoint* lastToFTopDown = NULL;
  TND280UpTrackPoint* lastToFBotUp = NULL;
  TND280UpTrackPoint* lastToFBotDown = NULL;
  TND280UpTrackPoint* lastToFLeftUp = NULL;
  TND280UpTrackPoint* lastToFLeftDown = NULL;
  TND280UpTrackPoint* lastToFRightUp = NULL;
  TND280UpTrackPoint* lastToFRightDown = NULL;
  TND280UpTrackPoint* lastToFBackUp = NULL;
  TND280UpTrackPoint* lastToFBackDown = NULL;
  TND280UpTrackPoint* lastToFFrontUp = NULL;
  TND280UpTrackPoint* lastToFFrontDown = NULL;

  TND280UpTrackPoint* firstTPCUp1 = NULL;
  TND280UpTrackPoint* firstTPCUp2 = NULL;
  TND280UpTrackPoint* firstTPCDown1 = NULL;
  TND280UpTrackPoint* firstTPCDown2 = NULL;
  TND280UpTrackPoint* firstForwTPC1 = NULL;
  TND280UpTrackPoint* firstForwTPC2 = NULL;
  TND280UpTrackPoint* firstForwTPC3 = NULL;
  TND280UpTrackPoint* firstTarget1 = NULL;
  TND280UpTrackPoint* firstTarget2 = NULL;
  TND280UpTrackPoint* firstFGD1 = NULL;
  TND280UpTrackPoint* firstFGD2 = NULL;
  TND280UpTrackPoint* firstDsECal = NULL;
  TND280UpTrackPoint* firstP0DECal = NULL;
  TND280UpTrackPoint* firstBrlECal = NULL;
  //TND280UpTrackPoint* firstToF = NULL;
  TND280UpTrackPoint* firstToFTopUp = NULL;
  TND280UpTrackPoint* firstToFTopDown = NULL;
  TND280UpTrackPoint* firstToFBotUp = NULL;
  TND280UpTrackPoint* firstToFBotDown = NULL;
  TND280UpTrackPoint* firstToFLeftUp = NULL;
  TND280UpTrackPoint* firstToFLeftDown = NULL;
  TND280UpTrackPoint* firstToFRightUp = NULL;
  TND280UpTrackPoint* firstToFRightDown = NULL;
  TND280UpTrackPoint* firstToFBackUp = NULL;
  TND280UpTrackPoint* firstToFBackDown = NULL;
  TND280UpTrackPoint* firstToFFrontUp = NULL;
  TND280UpTrackPoint* firstToFFrontDown = NULL;



  for (int ip = 0; ip < upTrack->GetNPoints(); ip++) {
    TND280UpTrackPoint* Tpoint = upTrack->GetPoint(ip);
//    std:;cout<<Tpoint->GetPhysVolName()<<std::endl;
    if (Tpoint->GetPhysVolName().find("TPCUp1") != std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < TPCUp1Z) {
        lastTPCUp1 = Tpoint;
        TPCUp1Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCUp1fZ) {
        firstTPCUp1 = Tpoint;
        TPCUp1fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("TPCUp2") != std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < TPCUp2Z) {
        lastTPCUp2 = Tpoint;
        TPCUp2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCUp2fZ) {
        firstTPCUp2 = Tpoint;
        TPCUp2fZ = Tpoint->GetMomentum().Mag();
      }

    }
     if (Tpoint->GetPhysVolName().find("TPCDown1") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < TPCDown1Z) {
        lastTPCDown1 = Tpoint;
        TPCDown1Z = Tpoint->GetMomentum().Mag();
      }
     if (Tpoint->GetMomentum().Mag() > TPCDown1fZ) {
        firstTPCDown1 = Tpoint;
        TPCDown1fZ = Tpoint->GetMomentum().Mag();
      }


    } 
    if (Tpoint->GetPhysVolName().find("TPCDown2") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < TPCDown2Z) {
        lastTPCDown2 = Tpoint;
        TPCDown2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > TPCDown2fZ) {
        firstTPCDown2 = Tpoint;
        TPCDown2fZ = Tpoint->GetMomentum().Mag();
      }


    }
    if (Tpoint->GetPhysVolName().find("ForwTPC1") != std::string::npos) {

      if (Tpoint->GetMomentum().Mag() < ForwTPC1Z) {
        lastForwTPC1 = Tpoint;
        ForwTPC1Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ForwTPC1fZ) {
        firstForwTPC1 = Tpoint;
        ForwTPC1fZ = Tpoint->GetMomentum().Mag();
      }


    } 
    if (Tpoint->GetPhysVolName().find("ForwTPC2") != std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < ForwTPC2Z) {
        lastForwTPC2 = Tpoint;
        ForwTPC2Z = Tpoint->GetMomentum().Mag();
      }

      if (Tpoint->GetMomentum().Mag() > ForwTPC2fZ) {
        firstForwTPC2 = Tpoint;
        ForwTPC2fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("ForwTPC3") != std::string::npos) {
   
      if (Tpoint->GetMomentum().Mag() < ForwTPC3Z) {
        lastForwTPC3 = Tpoint;
        ForwTPC3Z = Tpoint->GetMomentum().Mag();
      }

      if (Tpoint->GetMomentum().Mag() > ForwTPC3fZ) {
        firstForwTPC3 = Tpoint;
        ForwTPC3fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("Target1") != std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < Target1Z) {
        lastTarget1 = Tpoint;
        Target1Z = Tpoint->GetMomentum().Mag();
      }
    if (Tpoint->GetMomentum().Mag() > Target1fZ) {
        firstTarget1 = Tpoint;
        Target1fZ = Tpoint->GetMomentum().Mag();
      }

    } 
    if (Tpoint->GetPhysVolName().find("Target2") != std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < Target2Z) {
        lastTarget2 = Tpoint;
        Target2Z = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > Target2fZ) {
        firstTarget2 = Tpoint;
        Target2fZ = Tpoint->GetMomentum().Mag();
      }

    }
    if (Tpoint->GetPhysVolName().find("DsECal") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < DsECalZ) {
        lastDsECal = Tpoint;
        DsECalZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > DsECalfZ) {
        firstDsECal = Tpoint;
        DsECalfZ = Tpoint->GetMomentum().Mag();
      }

    }
    if (Tpoint->GetPhysVolName().find("P0DECal") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < P0DECalZ) {
        lastP0DECal = Tpoint;
        P0DECalZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > P0DECalfZ) {
        firstP0DECal = Tpoint;
        P0DECalfZ = Tpoint->GetMomentum().Mag();
      }

    }
    if (Tpoint->GetPhysVolName().find("BrlECal") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < BrlECalZ) {
        lastBrlECal = Tpoint;
        BrlECalZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > BrlECalfZ) {
        firstBrlECal = Tpoint;
        BrlECalfZ = Tpoint->GetMomentum().Mag();
      }

    }

    if (Tpoint->GetPhysVolName().find("ToF/TopUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < ToFTopUpZ) {
        lastToFTopUp = Tpoint;
        ToFTopUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFTopUpfZ) {
        firstToFTopUp = Tpoint;
        ToFTopUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

    if (Tpoint->GetPhysVolName().find("ToF/TopDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < ToFTopDownZ) {
        lastToFTopDown = Tpoint;
        ToFTopDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFTopDownfZ) {
        firstToFTopDown = Tpoint;
        ToFTopDownfZ = Tpoint->GetMomentum().Mag();
      }

    }
  if (Tpoint->GetPhysVolName().find("ToF/BotUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < ToFTopUpZ) {
        lastToFBotUp = Tpoint;
        ToFBotUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFTopUpfZ) {
        firstToFBotUp = Tpoint;
        ToFBotUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/BotDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < ToFTopDownZ) {
        lastToFBotDown = Tpoint;
        ToFBotDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFTopDownfZ) {
        firstToFBotDown = Tpoint;
        ToFBotDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/LeftUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
      if (Tpoint->GetMomentum().Mag() < ToFLeftUpZ) {
        lastToFLeftUp = Tpoint;
        ToFLeftUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFLeftUpfZ) {
        firstToFLeftUp = Tpoint;
        ToFLeftUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/RightUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < ToFRightUpZ) {
        lastToFRightUp = Tpoint;
        ToFRightUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFRightUpfZ) {
        firstToFRightUp = Tpoint;
        ToFRightUpfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/RightDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < ToFRightDownZ) {
        lastToFRightDown = Tpoint;
        ToFRightDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFRightDownfZ) {
        firstToFRightDown = Tpoint;
        ToFRightDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/LeftDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
    
      if (Tpoint->GetMomentum().Mag() < ToFLeftDownZ) {
        lastToFLeftDown = Tpoint;
        ToFLeftDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFLeftDownfZ) {
        firstToFLeftDown = Tpoint;
        ToFLeftDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/BackDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
   
      if (Tpoint->GetMomentum().Mag() < ToFBackDownZ) {
        lastToFBackDown = Tpoint;
        ToFBackDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFBackDownfZ) {
        firstToFBackDown = Tpoint;
        ToFBackDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/BackUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < ToFBackUpZ) {
        lastToFBackUp = Tpoint;
        ToFBackUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFBackUpfZ) {
        firstToFBackUp = Tpoint;
        ToFBackUpfZ = Tpoint->GetMomentum().Mag();
      }

    }



  if (Tpoint->GetPhysVolName().find("ToF/FrontDown") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
  
      if (Tpoint->GetMomentum().Mag() < ToFFrontDownZ) {
        lastToFFrontDown = Tpoint;
        ToFFrontDownZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFFrontDownfZ) {
        firstToFFrontDown = Tpoint;
        ToFFrontDownfZ = Tpoint->GetMomentum().Mag();
      }

    }

  if (Tpoint->GetPhysVolName().find("ToF/FrontUp") != std::string::npos && Tpoint->GetPhysVolName().find("Bar")!= std::string::npos) {
   
      if (Tpoint->GetMomentum().Mag() < ToFFrontUpZ) {
        lastToFFrontUp = Tpoint;
        ToFFrontUpZ = Tpoint->GetMomentum().Mag();
      }
      if (Tpoint->GetMomentum().Mag() > ToFFrontUpfZ) {
        firstToFFrontUp = Tpoint;
        ToFFrontUpfZ = Tpoint->GetMomentum().Mag();
      }

    }



}
  int nCrossers=0;
  if(firstTPCUp1){
        nCrossers++;
  }
  if(firstTPCUp2){
       nCrossers++;
  }
  if(firstTPCDown1){
       nCrossers++;
  }
   if(firstTPCDown2){
       nCrossers++;
  }
   if(firstForwTPC1){
       nCrossers++;
  }
   if(firstForwTPC2){
       nCrossers++;
  }
   if(firstForwTPC3){
       nCrossers++;
  }
  if(firstTarget1){
       nCrossers++;
  }
   if(firstTarget2){
       nCrossers++;
  }
   if(firstDsECal){
       nCrossers++;
  }
   if(firstP0DECal){
       nCrossers++;
  }

   if(firstBrlECal){
       nCrossers++;
  }

   if(firstToFTopUp){
       nCrossers++;
  }
   if(firstToFTopDown){
       nCrossers++;
  }

   if(firstToFBotUp){
       nCrossers++;
  }
   if(firstToFBotDown){
       nCrossers++;
  }
   if(firstToFLeftUp){
       nCrossers++;
  }
   if(firstToFLeftDown){
       nCrossers++;
  }
   if(firstToFFrontDown){
       nCrossers++;
  }
   if(firstToFFrontUp){
       nCrossers++;
  }

   if(firstToFBackDown){
       nCrossers++;
  }
   if(firstToFBackUp){
       nCrossers++;
  }

  truePart->nDetCrossings=0;

  //  anaUtils::CreateArray(truePart->DetCrossings, nCrossers);
  if(firstTPCUp1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    // firstTPCUp1->PrintTrackPoint();
    //lastTPCUp1->PrintTrackPoint();
    detCross->EntrancePosition[0] = firstTPCUp1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCUp1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCUp1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCUp1->GetTime();
    detCross->ExitPosition[0] = lastTPCUp1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCUp1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCUp1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCUp1->GetTime();
    detCross->EntranceMomentum[0] = firstTPCUp1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCUp1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCUp1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCUp1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCUp1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCUp1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCUp1();
    detCross->EDeposit=upTrack->GetEdepTPCUp1();
    detCross->SegLength=upTrack->GetLengthTPCUp1();
    detCross->Detector_name=TString(firstTPCUp1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCUp1);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTPCUp2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCUp2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCUp2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCUp2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCUp2->GetTime();
    detCross->ExitPosition[0] = lastTPCUp2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCUp2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCUp2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCUp2->GetTime();
    detCross->EntranceMomentum[0] = firstTPCUp2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCUp2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCUp2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCUp2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCUp2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCUp2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCUp2();
    detCross->EDeposit=upTrack->GetEdepTPCUp2();
    detCross->SegLength=upTrack->GetLengthTPCUp2();
    detCross->Detector_name=TString(firstTPCUp2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCUp2);
    truePart->DetCrossingsVect.push_back(detCross);
  }

  if (firstTPCDown1) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCDown1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCDown1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCDown1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCDown1->GetTime();
    detCross->ExitPosition[0] = lastTPCDown1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCDown1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCDown1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCDown1->GetTime();
    detCross->EntranceMomentum[0] = firstTPCDown1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCDown1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCDown1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCDown1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCDown1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCDown1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCDown1();
    detCross->EDeposit=upTrack->GetEdepTPCDown1();
    detCross->SegLength=upTrack->GetLengthTPCDown1();
    detCross->Detector_name=TString(firstTPCDown1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCDown1);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if(firstTPCDown2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
  
    detCross->EntrancePosition[0] = firstTPCDown2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTPCDown2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTPCDown2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTPCDown2->GetTime();
    detCross->ExitPosition[0] = lastTPCDown2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTPCDown2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTPCDown2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTPCDown2->GetTime();
    detCross->EntranceMomentum[0] = firstTPCDown2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTPCDown2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTPCDown2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTPCDown2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTPCDown2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTPCDown2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzTPCDown2();
    detCross->EDeposit=upTrack->GetEdepTPCDown2();
    detCross->SegLength=upTrack->GetLengthTPCDown2();
    detCross->Detector_name=TString(firstTPCDown2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTPCDown2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC1->GetTime();
    detCross->ExitPosition[0] = lastForwTPC1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC1->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC1->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC1();
    detCross->EDeposit=upTrack->GetEdepForwTPC1();
    detCross->SegLength=upTrack->GetLengthForwTPC1();
    detCross->Detector_name=TString(firstForwTPC1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC1);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC2->GetTime();
    detCross->ExitPosition[0] = lastForwTPC2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC2->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC2->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC2();
    detCross->EDeposit=upTrack->GetEdepForwTPC2();
    detCross->SegLength=upTrack->GetLengthForwTPC2();
    detCross->Detector_name=TString(firstForwTPC2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstForwTPC3){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstForwTPC3->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstForwTPC3->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstForwTPC3->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstForwTPC3->GetTime();
    detCross->ExitPosition[0] = lastForwTPC3->GetPostPosition().X();
    detCross->ExitPosition[1] = lastForwTPC3->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastForwTPC3->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastForwTPC3->GetTime();
    detCross->EntranceMomentum[0] = firstForwTPC3->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstForwTPC3->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstForwTPC3->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastForwTPC3->GetMomentum().X();
    detCross->ExitMomentum[1] = lastForwTPC3->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastForwTPC3->GetMomentum().Z();
    detCross->DeltaLYZ=upTrack->GetLyzForwTPC3();
    detCross->EDeposit=upTrack->GetEdepForwTPC3();
    detCross->Detector_name=TString(firstForwTPC3->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kForwTPC3);
    detCross->SegLength=upTrack->GetLengthForwTPC3();
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTarget1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstTarget1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTarget1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTarget1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTarget1->GetTime();
    detCross->ExitPosition[0] = lastTarget1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTarget1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTarget1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTarget1->GetTime();
    detCross->EntranceMomentum[0] = firstTarget1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTarget1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTarget1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTarget1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTarget1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTarget1->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthTarget1();
    detCross->DeltaLYZ=upTrack->GetLengthTarget1();
    detCross->EDeposit=upTrack->GetEdepTarget1();
    detCross->Detector_name=TString(firstTarget1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTarget1);

    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstTarget2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstTarget2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstTarget2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstTarget2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstTarget2->GetTime();
    detCross->ExitPosition[0] = lastTarget2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastTarget2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastTarget2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastTarget2->GetTime();
    detCross->EntranceMomentum[0] = firstTarget2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstTarget2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstTarget2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastTarget2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastTarget2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastTarget2->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthTarget2();
    detCross->DeltaLYZ=upTrack->GetLengthTarget2();
    detCross->EDeposit=upTrack->GetEdepTarget2();
    detCross->Detector_name=TString(firstTarget2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kTarget2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstFGD1){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstFGD1->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstFGD1->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstFGD1->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstFGD1->GetTime();
    detCross->ExitPosition[0] = lastFGD1->GetPostPosition().X();
    detCross->ExitPosition[1] = lastFGD1->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastFGD1->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastFGD1->GetTime();
    detCross->EntranceMomentum[0] = firstFGD1->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstFGD1->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstFGD1->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastFGD1->GetMomentum().X();
    detCross->ExitMomentum[1] = lastFGD1->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastFGD1->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthFGD1();
    detCross->DeltaLYZ=upTrack->GetLengthFGD1();
    detCross->EDeposit=upTrack->GetEdepFGD1();
    detCross->Detector_name=TString(firstFGD1->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kFGD1);

    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstFGD2){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstFGD2->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstFGD2->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstFGD2->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstFGD2->GetTime();
    detCross->ExitPosition[0] = lastFGD2->GetPostPosition().X();
    detCross->ExitPosition[1] = lastFGD2->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastFGD2->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastFGD2->GetTime();
    detCross->EntranceMomentum[0] = firstFGD2->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstFGD2->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstFGD2->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastFGD2->GetMomentum().X();
    detCross->ExitMomentum[1] = lastFGD2->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastFGD2->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthFGD2();
    detCross->DeltaLYZ=upTrack->GetLengthFGD2();
    detCross->EDeposit=upTrack->GetEdepFGD2();
    detCross->Detector_name=TString(firstFGD2->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kFGD2);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstDsECal){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstDsECal->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstDsECal->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstDsECal->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstDsECal->GetTime();
    detCross->ExitPosition[0] = lastDsECal->GetPostPosition().X();
    detCross->ExitPosition[1] = lastDsECal->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastDsECal->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastDsECal->GetTime();
    detCross->EntranceMomentum[0] = firstDsECal->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstDsECal->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstDsECal->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastDsECal->GetMomentum().X();
    detCross->ExitMomentum[1] = lastDsECal->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastDsECal->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthDsECal();
    detCross->DeltaLYZ=upTrack->GetLengthDsECal();
    detCross->EDeposit=upTrack->GetEdepDsECal();
    detCross->Detector_name=TString(firstDsECal->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kDsECal);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstP0DECal){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstP0DECal->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstP0DECal->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstP0DECal->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstP0DECal->GetTime();
    detCross->ExitPosition[0] = lastP0DECal->GetPostPosition().X();
    detCross->ExitPosition[1] = lastP0DECal->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastP0DECal->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastP0DECal->GetTime();
    detCross->EntranceMomentum[0] = firstP0DECal->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstP0DECal->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstP0DECal->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastP0DECal->GetMomentum().X();
    detCross->ExitMomentum[1] = lastP0DECal->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastP0DECal->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthP0DECal();
    detCross->DeltaLYZ=upTrack->GetLengthP0DECal();
    detCross->EDeposit=upTrack->GetEdepP0DECal();
    detCross->Detector_name=TString(firstP0DECal->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kP0DECal);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if(firstBrlECal){
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    //std::cout<<"firstBrlECal->GetPostPosition().X() "<<lastBrlECal->GetPostPosition().X()<<" "<<lastBrlECal->GetPostPosition().Y()<<" "<<lastBrlECal->GetPostPosition().Z()<<std::endl;
    detCross->EntrancePosition[0] = firstBrlECal->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstBrlECal->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstBrlECal->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstBrlECal->GetTime();
    detCross->ExitPosition[0] = lastBrlECal->GetPostPosition().X();
    detCross->ExitPosition[1] = lastBrlECal->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastBrlECal->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastBrlECal->GetTime();
    detCross->EntranceMomentum[0] = firstBrlECal->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstBrlECal->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstBrlECal->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastBrlECal->GetMomentum().X();
    detCross->ExitMomentum[1] = lastBrlECal->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastBrlECal->GetMomentum().Z();
    detCross->SegLength=upTrack->GetLengthBrlECal();
    detCross->DeltaLYZ=upTrack->GetLengthBrlECal();
    detCross->EDeposit=upTrack->GetEdepBrlECal();
    detCross->Detector_name=TString(firstBrlECal->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kBrlECal);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstToFTopUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFTopUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFTopUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFTopUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFTopUp->GetTime();
    detCross->ExitPosition[0] = lastToFTopUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFTopUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFTopUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFTopUp->GetTime();
    detCross->EntranceMomentum[0] = firstToFTopUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFTopUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFTopUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFTopUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFTopUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFTopUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFTopUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFTopUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstToFBotUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFBotUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFBotUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFBotUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFBotUp->GetTime();
    detCross->ExitPosition[0] = lastToFBotUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFBotUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFBotUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFBotUp->GetTime();
    detCross->EntranceMomentum[0] = firstToFBotUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFBotUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFBotUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFBotUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFBotUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFBotUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFBotUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFBotUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstToFTopDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFTopDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFTopDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFTopDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFTopDown->GetTime();
    detCross->ExitPosition[0] = lastToFTopDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFTopDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFTopDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFTopDown->GetTime();
    detCross->EntranceMomentum[0] = firstToFTopDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFTopDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFTopDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFTopDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFTopDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFTopDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFTopDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFTopDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstToFBotDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFBotDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFBotDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFBotDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFBotDown->GetTime();
    detCross->ExitPosition[0] = lastToFBotDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFBotDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFBotDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFBotDown->GetTime();
    detCross->EntranceMomentum[0] = firstToFBotDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFBotDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFBotDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFBotDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFBotDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFBotDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFBotDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFBotDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstToFLeftUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFLeftUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFLeftUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFLeftUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFLeftUp->GetTime();
    detCross->ExitPosition[0] = lastToFLeftUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFLeftUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFLeftUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFLeftUp->GetTime();
    detCross->EntranceMomentum[0] = firstToFLeftUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFLeftUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFLeftUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFLeftUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFLeftUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFLeftUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFLeftUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFLeftUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstToFLeftDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFLeftDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFLeftDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFLeftDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFLeftDown->GetTime();
    detCross->ExitPosition[0] = lastToFLeftDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFLeftDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFLeftDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFLeftDown->GetTime();
    detCross->EntranceMomentum[0] = firstToFLeftDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFLeftDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFLeftDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFLeftDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFLeftDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFLeftDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFLeftDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFLeftDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstToFRightUp) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFRightUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFRightUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFRightUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFRightUp->GetTime();
    detCross->ExitPosition[0] = lastToFRightUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFRightUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFRightUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFRightUp->GetTime();
    detCross->EntranceMomentum[0] = firstToFRightUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFRightUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFRightUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFRightUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFRightUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFRightUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFRightUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFRightUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstToFRightDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFRightDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFRightDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFRightDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFRightDown->GetTime();
    detCross->ExitPosition[0] = lastToFRightDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFRightDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFRightDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFRightDown->GetTime();
    detCross->EntranceMomentum[0] = firstToFRightDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFRightDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFRightDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFRightDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFRightDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFRightDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFRightDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFRightDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }


  if (firstToFBackDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFBackDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFBackDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFBackDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFBackDown->GetTime();
    detCross->ExitPosition[0] = lastToFBackDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFBackDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFBackDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFBackDown->GetTime();
    detCross->EntranceMomentum[0] = firstToFBackDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFBackDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFBackDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFBackDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFBackDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFBackDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFBackDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFBackDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstToFBackUp) {

    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFBackUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFBackUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFBackUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFBackUp->GetTime();
    detCross->ExitPosition[0] = lastToFBackUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFBackUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFBackUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFBackUp->GetTime();
    detCross->EntranceMomentum[0] = firstToFBackUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFBackUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFBackUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFBackUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFBackUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFBackUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFBackUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFBackUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }

  if (firstToFFrontDown) {
    AnaDetCrossingB* detCross = MakeAnaDetCrossing();

    detCross->EntrancePosition[0] = firstToFFrontDown->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFFrontDown->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFFrontDown->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFFrontDown->GetTime();
    detCross->ExitPosition[0] = lastToFFrontDown->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFFrontDown->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFFrontDown->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFFrontDown->GetTime();
    detCross->EntranceMomentum[0] = firstToFFrontDown->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFFrontDown->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFFrontDown->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFFrontDown->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFFrontDown->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFFrontDown->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFFrontDown->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFFrontDown);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  if (firstToFFrontUp) {

    AnaDetCrossingB* detCross = MakeAnaDetCrossing();
    detCross->EntrancePosition[0] = firstToFFrontUp->GetPostPosition().X();
    detCross->EntrancePosition[1] = firstToFFrontUp->GetPostPosition().Y();
    detCross->EntrancePosition[2] = firstToFFrontUp->GetPostPosition().Z();
    detCross->EntrancePosition[3] = firstToFFrontUp->GetTime();
    detCross->ExitPosition[0] = lastToFFrontUp->GetPostPosition().X();
    detCross->ExitPosition[1] = lastToFFrontUp->GetPostPosition().Y();
    detCross->ExitPosition[2] = lastToFFrontUp->GetPostPosition().Z();
    detCross->ExitPosition[3] = lastToFFrontUp->GetTime();
    detCross->EntranceMomentum[0] = firstToFFrontUp->GetMomentum().X();
    detCross->EntranceMomentum[1] = firstToFFrontUp->GetMomentum().Y();
    detCross->EntranceMomentum[2] = firstToFFrontUp->GetMomentum().Z();
    detCross->ExitMomentum[0] = lastToFFrontUp->GetMomentum().X();
    detCross->ExitMomentum[1] = lastToFFrontUp->GetMomentum().Y();
    detCross->ExitMomentum[2] = lastToFFrontUp->GetMomentum().Z();
    detCross->Detector_name = TString(firstToFFrontUp->GetPhysVolName());
    SubDetId::SetDetectorUsed(detCross->Detector, SubDetId::kToFFrontUp);
    truePart->DetCrossingsVect.push_back(detCross);

  }
  

}


bool AnaTreeConverterEvent::IsReconstructedTarget(double length,double theta){
  int bin =hefficiency_target->FindBin(theta,length);

  double prob=hefficiency_target->GetBinContent(bin);
 
  if(gRandom->Uniform()>prob)
    return 1;
  else
    return 0;
  
}


bool AnaTreeConverterEvent::IsReconstructedECal(TVector3 P, TString det){

  float reco_eff_Brl, FGDmatch_eff_Brl;
  float reco_eff_Ds,  FGDmatch_eff_Ds;

  TVector3 entryNormal_vect(0,0,0);
    
  if (det.Contains("RightClam") &&
      det.Contains("BotLeftTopRight"))
    entryNormal_vect.SetY(1);  // (+Y)
  else if (det.Contains("RightClam") &&
	   det.Contains("TopLeftBotRight"))
    entryNormal_vect.SetY(-1); // (-Y)
  else if (det.Contains("LeftClam") &&
	   det.Contains("BotLeftTopRight"))
    entryNormal_vect.SetY(-1); // (-Y)
  else if (det.Contains("LeftClam") &&
	   det.Contains("TopLeftBotRight"))
    entryNormal_vect.SetY(1);  // (+Y)
  else if (det.Contains("LeftSide"))
    entryNormal_vect.SetX(1);  // (+X)
  else if (det.Contains("RightSide"))
    entryNormal_vect.SetX(-1); // (-X)
  else if (det.Contains("POD/USECal"))
    entryNormal_vect.SetZ(-1); // (-Z)
  else
    entryNormal_vect.SetZ(1);  // (+Z)

  float mom = P.Mag();
  float cos = P.Dot(entryNormal_vect)/mom;
  
  if (!_ECal_reco_eff->GetBinValues(mom, reco_eff_Brl, reco_eff_Ds)) return false;
  if (!_ECal_FGDmatch_eff->GetBinValues(cos, FGDmatch_eff_Brl, FGDmatch_eff_Ds)) return false;

  // throw two random numbers between 0 and 1
  double r_eff[2];
  _randomGen->RndmArray(2, r_eff);

  // select artificially only a fraction of the events, 
  // by applying the reconstruction/FGD-ECal match efficiencies
  if (det.Contains("DsECal")) {
    if (r_eff[1] < FGDmatch_eff_Ds)
      return true;
  }
  else {
    if (r_eff[0] < reco_eff_Brl && r_eff[1] < FGDmatch_eff_Brl)
      return true;
  }
  return false;
  
}


void AnaTreeConverterEvent::Fill_Tracks_Recon_From_True(AnaTrueParticleB* trueParticle, AnaTrack* reconParticle) {
  
  reconParticle->UniqueID       = trueParticle->ID;
  reconParticle->Momentum       = trueParticle->Momentum;
  reconParticle->Charge         = trueParticle->Charge;
  reconParticle->Length         = trueParticle->Length;
  reconParticle->EDeposit       = trueParticle->EDeposit;
  reconParticle->EKin           = trueParticle->EKin;
  anaUtils::CopyArray(trueParticle->Direction, reconParticle->DirectionStart, 3);
  anaUtils::CopyArray(trueParticle->Position,  reconParticle->PositionStart, 4);
  anaUtils::CopyArray(trueParticle->PositionEnd,   reconParticle->PositionEnd, 4);
  reconParticle->MomentumEle = -10000;
  reconParticle->MomentumMuon = -10000;
  reconParticle->MomentumProton = -10000;
  reconParticle->MomentumErrorEle = -10000;
  reconParticle->MomentumErrorMuon = -10000;
  reconParticle->MomentumErrorProton = -10000;
  reconParticle->nTargetSegments = 0;
  reconParticle->nFGDSegments = 0;
  reconParticle->nTPCSegments = 0;
  reconParticle->TrueObject=dynamic_cast<AnaTrueObjectC*>(trueParticle);
  for (int i = 0; i < trueParticle->DetCrossingsVect.size(); i++) {

    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCUp2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTPCDown2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC2) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kForwTPC3)) {
      AnaTPCParticleB* seg = dynamic_cast<AnaTPCParticleB*>(MakeTPCTrack());
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
 
      seg->Detectors=trueParticle->DetCrossingsVect[i]->Detector;
 
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      seg->Momentum = mom;
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      //  anaUtils::ComputeTPCPull(*seg,*reconParticle);
      reconParticle->TPCSegments[reconParticle->nTPCSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTarget1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kTarget2)) {
      AnaTargetParticleB* seg = dynamic_cast<AnaTargetParticleB*>(MakeTargetTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      seg->IsReconstructed = IsReconstructedTarget(seg->DeltaLYZ, trueParticle->CosTheta);
      reconParticle->TargetSegments[reconParticle->nTargetSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kFGD1) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kFGD2)) {
      AnaFGDParticleB* seg = dynamic_cast<AnaFGDParticleB*>(MakeFGDTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      seg->IsReconstructed = IsReconstructedTarget(seg->DeltaLYZ, trueParticle->CosTheta);
      reconParticle->FGDSegments[reconParticle->nFGDSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kDsECal) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kBrlECal) || SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kP0DECal)) {
      AnaECalParticleB* seg = dynamic_cast<AnaECalParticleB*>(MakeECalTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      seg->IsReconstructed =
	IsReconstructedECal(anaUtils::ArrayToTVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum),
			    trueParticle->DetCrossingsVect[i]->Detector_name);
      reconParticle->ECalSegments[reconParticle->nECalSegments++] = seg;

    }
    if (SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFFrontUp)||
	SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFBackUp) ||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFTopUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFBotUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFLeftUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFRightUp)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFFrontDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFBackDown) ||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFTopDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFBotDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFLeftDown)||
        SubDetId::GetDetectorUsed(trueParticle->DetCrossingsVect[i]->Detector, SubDetId::kToFRightDown)
        ) {
          std::cout<<"tof"<<std::endl;
      AnaToFParticleB* seg = dynamic_cast<AnaToFParticleB*>(MakeToFTrack() );
      Float_t mom = sqrt(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] + trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] * trueParticle->DetCrossingsVect[i]->EntranceMomentum[2]);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->EntranceMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->EntranceMomentum[2] / mom), seg->DirectionStart);
      anaUtils::VectorToArray(TVector3(trueParticle->DetCrossingsVect[i]->ExitMomentum[0] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[1] / mom, trueParticle->DetCrossingsVect[i]->ExitMomentum[2] / mom), seg->DirectionEnd);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->EntrancePosition, seg->PositionStart, 4);
      anaUtils::CopyArray(trueParticle->DetCrossingsVect[i]->ExitPosition,  seg->PositionEnd, 4);
      seg->DeltaLYZ = trueParticle->DetCrossingsVect[i]->DeltaLYZ;
      seg->SegLength = trueParticle->DetCrossingsVect[i]->SegLength;
      seg->EDeposit = trueParticle->DetCrossingsVect[i]->EDeposit;
      SubDetId::SubDetEnum dsub = SubDetId::GetSubdetectorEnum(trueParticle->DetCrossingsVect[i]->Detector);
      SubDetId::SetDetectorUsed(reconParticle->Detectors, dsub);
      seg->Detectors = trueParticle->DetCrossingsVect[i]->Detector;
      reconParticle->ToFSegments[reconParticle->nToFSegments++] = seg;

    }
  }

}
