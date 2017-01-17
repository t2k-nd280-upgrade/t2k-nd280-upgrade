#include "baseSelection.hxx"
#include "trueCCMultiPiSelection.hxx"
#include "stdlib.h"
#include "SampleId.hxx"
#include "ConstituentsUtils.hxx"
#include "EventBoxUtils.hxx"

//********************************************************************
trueCCMultiPiSelection::trueCCMultiPiSelection(bool forceBreak): SelectionBase(forceBreak, EventBoxId::ktrueEventBoxNDUP) {
//********************************************************************
            char filename[256];

            sprintf(filename, "%s/data/efficiencies.root", getenv("PSYCHESELECTIONSROOT"));
            _file0 = TFile::Open(filename);

  // Initialize systematic tuning parameters
  //  systTuning::Initialize();
}
//**************************************************
void trueCCMultiPiSelection::InitializeEvent(AnaEventC& eventBB) {
//**************************************************

  AnaEventB& event = *dynamic_cast<AnaEventB*>(&eventBB);

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxNDUP])
    event.EventBoxes[EventBoxId::kEventBoxNDUP] = new EventBox();

//  trueboxUtils::FillLongTracks(event,       static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
}

//********************************************************************
void trueCCMultiPiSelection::DefineSteps() {
//********************************************************************
   std::cout<<"define"<<std::endl;
    AddStep(StepBase::kAction, "Find  lepton track",      new trueUtils::LeptonTrackAction());
   AddStep(StepBase::kCut, "Efficiency",      new trueUtils::EfficiencyCut( _file0), true);
    AddStep(StepBase::kAction, "fill_summary", new FillSummaryAction_trueCCMultiPi());
    AddStep(StepBase::kAction, "Pion efficiency",      new trueUtils::PionEfficiencyAction( _file0), true);
    AddStep(StepBase::kAction, "Smearing",      new trueUtils::SmearingAction(), true);
    
    AddSplit(3);
    
    //First branch is for CC-0pi
    AddStep(0, StepBase::kCut, "CC-0pi", new trueUtils::CC0piCut(), true);
    
    AddSplit(2,0);
    AddStep(0,0, StepBase::kCut, "CC0pi Target1", new trueUtils::inTarget1Cut(), true);
    AddStep(0,1, StepBase::kCut, "CC0pi Target2", new trueUtils::inTarget2Cut(), true);
    
    //Second branch is for CC-1pi
    AddStep(1, StepBase::kCut, "CC-1pi", new trueUtils::CC1piCut(), true);
    AddSplit(2,1);
    AddStep(1,0, StepBase::kCut, "CC-1pi Target1", new trueUtils::inTarget1Cut(), true);
    AddStep(1,1, StepBase::kCut, "CC-1pi Target2", new trueUtils::inTarget2Cut(), true);
    
    //Third branch is for CC-Other
    AddStep(2, StepBase::kCut, "CC-Other", new trueUtils::CCOtherCut(), true);
    AddSplit(2,2);
    AddStep(2,0, StepBase::kCut, "CC-Other Target1", new trueUtils::inTarget1Cut(), true);
    AddStep(2,1, StepBase::kCut, "CC-Other Target2", new trueUtils::inTarget2Cut(), true);
    
    
    
    SetBranchAlias(0,"CC-0pi Target1",0,0);
   SetBranchAlias(1,"CC-0pi Target2",0, 1);
    SetBranchAlias(2,"CC-1pi Target1",1, 0);
    SetBranchAlias(3,"CC-1pi Target2",1, 1);
    SetBranchAlias(4,"CC-Other Target1",2, 0);
    SetBranchAlias(5,"CC-Other Target2",2, 1);
 
}

//********************************************************************
void trueCCMultiPiSelection::DefineDetectorFV() {
//********************************************************************

  // The detector in which the selection is applied
  SetDetectorFV(SubDetId::kTarget1);
}

//**************************************************
bool trueCCMultiPiSelection::IsRelevantRecObjectForSystematic(const AnaEventC& event, AnaRecObjectC* recObj, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)recObj;

  return true;
}

//**************************************************
bool trueCCMultiPiSelection::IsRelevantTrueObjectForSystematic(const AnaEventC& event, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)systId;
  (void)trueObj;
  return true;
}

//**************************************************
bool trueCCMultiPiSelection::IsRelevantRecObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaRecObjectC* track, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)track;



  return true;
}

//**************************************************
bool trueCCMultiPiSelection::IsRelevantTrueObjectForSystematicInToy(const AnaEventC& event, const ToyBoxB& boxB, AnaTrueObjectC* trueObj, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)boxB;
  (void)systId;
  (void)trueObj;

  return true;
}

//**************************************************
bool trueCCMultiPiSelection::IsRelevantSystematic(const AnaEventC& event, const ToyBoxB& box, SystId_h systId, Int_t branch) const {
//**************************************************

  (void)event;
  (void)branch;
  (void)box;
  (void)systId;

  return true;
}


//********************************************************************
bool trueCCMultiPiSelection::CheckRedoSelection(const AnaEventC& eventC, const ToyBoxB& PreviousToyBoxB, Int_t& redoFromStep) {
//********************************************************************

  (void)eventC;
  (void)PreviousToyBoxB;
  (void)redoFromStep;

  return true;
  return false;
}



namespace trueUtils{
//**************************************************
bool LeptonTrackAction::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
        AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 
   // std::cout<<event.nTrueParticles<<std::endl;
 
    int debug = 0;
    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    if (event.nTrueParticles == 0) return false;
    if (event.nTrueVertices == 0) return false;

    for (UInt_t i=0;i<event.nTrueVertices;i++){
        AnaTrueVertexB* vertex = event.TrueVertices[i];
                    std::cout<<vertex->NuPDG<<std::endl;

        if (!(vertex->NuPDG == 14)) continue;

      //  if (vertex->nTrueTracks == 0) continue;

        std::vector<AnaTrueParticleB*> TrueParticles = vertex->TrueParticlesVect;
                    std::cout<<(!(anaUtils::InFiducialVolume(SubDetId::kTarget1,vertex->Position) || anaUtils::InFiducialVolume(SubDetId::kTarget2,vertex->Position)))<<std::endl;
          std::cout<<"TrueParticles.size() "<<event.nTrueParticles<<std::endl;
      
        if (!(anaUtils::InFiducialVolume(SubDetId::kTarget1,vertex->Position) || anaUtils::InFiducialVolume(SubDetId::kTarget2,vertex->Position))) continue;
                std::cout<<"TrueParticles.size() "<<TrueParticles.size()<<std::endl;
        for (UInt_t j=0;j<TrueParticles.size();j++){
            AnaTrueParticleB* track = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);

            if (!(track->PDG == 13)) continue;
            std::cout<<truebox->flag_mom<<" "<<track->Momentum<<std::endl;
            if (pow(truebox->flag_mom - track->Momentum, 2)<0.1){
                continue;
            }
            truebox->flag_mom = track->Momentum;
            truebox->Leptontrack = track;
            truebox->Vertex = vertex;
            if(debug==1)std::cout<<"Muon "<< track->Momentum<<" "<<truebox->flag_mom<<std::endl;
            
            break;
        }
    }
    double highest_mom = 0;
    AnaTrueParticleB *high = NULL;
    for (UInt_t i=0;i<event.nTrueVertices;i++){
        AnaTrueVertexB* vertex = event.TrueVertices[i];
       // if (vertex->nTrueTracks == 0) continue;
                std::vector<AnaTrueParticleB*> TrueParticles = vertex->TrueParticlesVect;

       for (UInt_t j=0;j<TrueParticles.size();j++){
            AnaTrueParticleB* track = dynamic_cast<AnaTrueParticleB*>(TrueParticles[i]);
            if(track->Momentum > highest_mom  && track->Charge < 0 && (anaUtils::InFiducialVolume(SubDetId::kTarget1,vertex->Position) || anaUtils::InFiducialVolume(SubDetId::kTarget2,vertex->Position))){
                highest_mom = track->Momentum;
                high = track;
            }
        }
    }
    if(high){
        if(high->Momentum>0){
            if( ((high->TrueVertex->NuPDG!=14 || high->PDG !=13) && pow(truebox->flag_HMN_mom - high->Momentum, 2)>0.1) && high!= truebox->Leptontrack){
                truebox->flag_HMN_mom = high->Momentum;
                truebox->HMNtrack = high;
                if(debug==1) std::cout<<"HMNT "<< high->Momentum<<" "<<truebox->flag_HMN_mom<<std::endl;
            }
        }
    }
   // std::cout<<"true"<<std::endl;
    return true;
}

//**************************************************
bool EfficiencyCut::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    TCanvas *mydummycanvas=new TCanvas();
    
//    TFile *_file0 = TFile::Open("/sps/t2k/bienstock/T2K/T2KReWeight/GlobalAnalysisTools/T2KReWeight/efficiencies.root");
    double randm;
    
    int debug = 0;
    
    double eff = 0.0;
    double eff2 = 0.0;
        std::cout<<"EfficiencyCut"<<std::endl;
    
    if(truebox->Leptontrack && truebox->Vertex){
        std::cout<<"true_box"<<std::endl;
        AnaTrueParticleB* track = truebox->Leptontrack;
        TRandom3 *r3 = new TRandom3(int(track->Momentum));
        randm = r3->Rndm();
        if(anaUtils::InFiducialVolume(SubDetId::kTarget1,track->TrueVertex->Position)){
            Int_t nbin = muEff_Target1->FindBin(track->Direction[2],track->Momentum);
            eff = muEff_Target1->GetBinContent(nbin);
            std::cout<<"in fid vl"<<std::endl;
        }
        else if(anaUtils::InFiducialVolume(SubDetId::kTarget2,track->TrueVertex->Position)){
            Int_t nbin = muEff_Target2->FindBin(track->Direction[2],track->Momentum);
            eff = muEff_Target2->GetBinContent(nbin);
            std::cout<<"in fid vl"<<std::endl;
 
        }
        
        if(randm < eff){
            //if(debug==1)
             std::cout<<"Muon "<< track->Momentum<<" "<<randm<<std::endl;
            delete mydummycanvas;
            
            return true;
        }
    }
    if(truebox->HMNtrack){
        
        AnaTrueParticleB* sel = truebox->HMNtrack;
        TRandom3 *r4 = new TRandom3(int(sel->Momentum));
        randm = r4->Rndm();
        if(anaUtils::InFiducialVolume(SubDetId::kTarget1,sel->TrueVertex->Position)){
            Int_t nbin = HM_Other_Target1->FindBin(sel->Direction[2],sel->Momentum);
            eff2 = HM_Other_Target1->GetBinContent(nbin);
        }
        else if(anaUtils::InFiducialVolume(SubDetId::kTarget2,sel->TrueVertex->Position)){
            Int_t nbin = HM_Other_Target2->FindBin(sel->Direction[2],sel->Momentum);
            eff2 = HM_Other_Target2->GetBinContent(nbin);
        }
        randm = truebox->random_true->Rndm();
        if(randm < eff2){
            //if(debug==1)
             std::cout<<"HMNT "<< sel->Momentum<<" "<<randm<<std::endl;
            truebox->Leptontrack = sel;
            truebox->Vertex = sel->TrueVertex;
            
            delete mydummycanvas;
            return true;
        }
    }
    delete mydummycanvas;

    return false;
}

/*
 //**************************************************
 bool trueUtils::EfficiencyCut::Apply(const AnaEventB& event, AnaBoxB& box) const{
 //**************************************************
 
 AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
 
 if (!truebox->Vertex) return false;
 if(!truebox->Leptontrack) return false;
 
 return true;
 }
 
 */
//**************************************************
bool PionEfficiencyAction::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    
    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if (!truebox->Vertex) return false;
    if(!truebox->Leptontrack) return false;
    //AnaTrueVertexB* vertex = truebox->Vertex;
    TCanvas *mydummycanvas = new TCanvas();
    
    
    double randm;
    TRandom3 *r3 = new TRandom3(int(truebox->Leptontrack->Momentum));
    
    double p_Target1 = 0.0101;
    double p_Target2 = 0.0146;
    double pi_Target1 = 0.565;
    double pi_Target2 = 0.414;
    double other_Target1 = 0.0143;
    double other_Target2 = 0.0246;
    double pi0_fake = 0.0173;
    
    int debug =0;
    
    
    Int_t nbin =0;
    double eff =0;
    for (UInt_t k=0;k<event.nTrueVertices;k++){
        AnaTrueVertexB& vertex = *static_cast<AnaTrueVertexB*>(event.TrueVertices[k]);
        if (!((anaUtils::InDetVolume(SubDetId::kTarget1,vertex.Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,truebox->Vertex->Position))||(anaUtils::InDetVolume(SubDetId::kTarget2,vertex.Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,truebox->Vertex->Position)))){
            continue;
        }
    std::vector<AnaTrueParticleB*> TrueParticles = vertex.TrueParticlesVect;

       for (UInt_t j=0;j<TrueParticles.size();j++){
            AnaTrueParticleB* track = dynamic_cast<AnaTrueParticleB*>(TrueParticles[j]);
            if((abs(track->Momentum - truebox->Leptontrack->Momentum)<0.1) && abs(track->Direction[2] - truebox->Leptontrack->Direction[2])<0.05) continue;
            //Pi0
            if ((track->PDG == 111 || (track->PDG == -111))) {
                randm = r3->Rndm();
                
                nbin = pi0Eff->FindBin(track->Direction[2],track->Momentum);
                eff = pi0Eff->GetBinContent(nbin);
                if(randm < eff){
                    truebox->npion++;
                    if(debug==1) std::cout<<"pi0 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                    continue;
                }
            }
            //Pi minus
            if ((track->PDG == -211)) {
                randm = r3->Rndm();
                if ((anaUtils::InDetVolume(SubDetId::kTarget1,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,track->PositionEnd))){
                    if(randm < pi_Target1){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"pim Target1 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< pi_Target1 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else if ((anaUtils::InDetVolume(SubDetId::kTarget2,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,track->PositionEnd))){
                    if(randm < pi_Target2){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"pim Target2 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< pi_Target2 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else{
                    for(int i = 0; i<track->nDetCrossings; i++){
                        if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                            nbin = pimEff->FindBin(track->Direction[2],track->Momentum);
                            eff = pimEff->GetBinContent(nbin);
                            if(randm < eff){
                                truebox->npion++;
                                if(debug==1) std::cout<<"pim TPC "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                break;
                            }
                        }
                    }
                }
            }
            //Pi+
            if ((track->PDG == 211)){
                randm = r3->Rndm();
                if ((anaUtils::InDetVolume(SubDetId::kTarget1,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,track->PositionEnd))){
                    if(randm < pi_Target1){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"pip Target1 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< pi_Target1 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else if ((anaUtils::InDetVolume(SubDetId::kTarget2,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,track->PositionEnd))){
                    if(randm < pi_Target2){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"pip Target2 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< pi_Target2 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else{
                    for(int i = 0; i<track->nDetCrossings; i++){
                        if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                            nbin = pipEff->FindBin(track->Direction[2],track->Momentum);
                            eff = pipEff->GetBinContent(nbin);
                            if(randm < eff){
                                truebox->npion_pos++;
                                if(truebox->npion_pos==1)  truebox->PositivePionTPCtracks= track;
                                if(debug==1) std::cout<<"pip TPC "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                break;
                            }
                            else{
                                eff = other_pimEff->GetBinContent(nbin);
                                if(randm<eff){
                                    truebox->npion++;
                                    if(debug==1) std::cout<<"pip TPC false pim "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            //proton
            if ((track->PDG == 2212)){
                randm = r3->Rndm();
                if ((anaUtils::InDetVolume(SubDetId::kTarget1,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,track->PositionEnd))){
                    if(randm < p_Target1){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"proton Target1 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< p_Target1 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else if ((anaUtils::InDetVolume(SubDetId::kTarget2,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,track->PositionEnd))){
                    if(randm < p_Target2){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"proton Target2 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< p_Target2 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else{
                    for(int i = 0; i<track->nDetCrossings; i++){
                        if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                            nbin = proEff->FindBin(track->Direction[2],track->Momentum);
                            eff = proEff->GetBinContent(nbin);
                            if(randm < eff){
                                truebox->npion_pos++;
                                if(debug==1) std::cout<<"proton TPC false pip "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                break;
                            }
                            else{
                                eff = other_pimEff->GetBinContent(nbin);
                                if(randm < eff){
                                    truebox->npion++;
                                    if(debug==1) std::cout<<"proton TPC false pim "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            //mu+
            if ((track->PDG == -13)){
                randm = r3->Rndm();
                if ((anaUtils::InDetVolume(SubDetId::kTarget1,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,track->PositionEnd))){
                    if(randm < other_Target1){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"mu+ Target1 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target1 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else if ((anaUtils::InDetVolume(SubDetId::kTarget2,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,track->PositionEnd))){
                    if(randm < other_Target2){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"mu+ Target2 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target2 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else{
                    for(int i = 0; i<track->nDetCrossings; i++){
                        if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                            nbin = other_pipEff->FindBin(track->Direction[2],track->Momentum);
                            eff = other_pipEff->GetBinContent(nbin);
                            if(randm < eff){
                                truebox->npion_pos++;
                                if(debug==1) std::cout<<"mu+ TPC false pip"<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                break;
                            }
                        }
                    }
                }
            }
            //e+
            if ((track->PDG == -11)){
                randm = r3->Rndm();
                if ((anaUtils::InDetVolume(SubDetId::kTarget1,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,track->PositionEnd))){
                    if(randm < other_Target1){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"e+ Target1 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target1 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else if ((anaUtils::InDetVolume(SubDetId::kTarget2,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,track->PositionEnd))){
                    if(randm < other_Target2){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"e+ Target2 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target2 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else{
                    for(int i = 0; i<track->nDetCrossings; i++){
                        if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                            nbin = other_pipEff->FindBin(track->Direction[2],track->Momentum);
                            eff = other_pipEff->GetBinContent(nbin);
                            if(randm < eff){
                                truebox->npion_pos++;
                                if(debug==1) std::cout<<"e+ TPC false pip "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                break;
                            }
                            else if(randm<pi0_fake && track->GParentPDG != 111){
                                truebox->npion++;
                                if(debug==1) std::cout<<"e+ false pi0 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< pi0_fake << " Rand "<<randm<<std::endl;
                                break;
                            }
                        }
                    }
                    
                }
            }
            //e-
            if ((track->PDG == -11)){
                randm = r3->Rndm();
                if ((anaUtils::InDetVolume(SubDetId::kTarget1,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,track->PositionEnd))){
                    if(randm < other_Target1){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"e- Target1 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target1 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else if ((anaUtils::InDetVolume(SubDetId::kTarget2,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,track->PositionEnd))){
                    if(randm < other_Target2){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"e- Target2 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target2 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else{
                    int test=0;
                    for(int i = 0; i<track->nDetCrossings; i++){
                        if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                            nbin = other_pimEff->FindBin(track->Direction[2],track->Momentum);
                            eff = other_pimEff->GetBinContent(nbin);
                            if(randm < eff){
                                truebox->npion++;
                                if(debug==1) std::cout<<"e- TPC false pim "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                test ++;
                                break;
                            }
                            else if(randm<pi0_fake && track->GParentPDG != 111){
                                truebox->npion++;
                                if(debug==1) std::cout<<"e- false pi0 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< pi0_fake << " Rand "<<randm<<std::endl;
                                break;
                            }
                        }
                    }
                }
            }
            //K+
            if ((track->PDG == 321)){
                randm = r3->Rndm();
                for(int i = 0; i<track->nDetCrossings; i++){
                    if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                        nbin = other_pipEff->FindBin(track->Direction[2],track->Momentum);
                        eff = other_pipEff->GetBinContent(nbin);
                        if(randm < eff){
                            truebox->npion_pos++;
                            if(debug==1) std::cout<<"K+ TPC false pip "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                            break;
                        }
                    }
                }
            }
            //mu-
            if ((track->PDG == 13)) {
                randm = r3->Rndm();
                if ((anaUtils::InDetVolume(SubDetId::kTarget1,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget1,track->PositionEnd))){
                    if(randm < other_Target1){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"mu- Target1 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target1 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else if ((anaUtils::InDetVolume(SubDetId::kTarget2,track->Position)&& anaUtils::InDetVolume(SubDetId::kTarget2,track->PositionEnd))){
                    if(randm < other_Target2){
                        truebox->npion_pos++;
                        if(debug==1) std::cout<<"mu- Target2 "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< other_Target2 << " Rand "<<randm<<std::endl;
                        continue;
                    }
                }
                else{
                    for(int i = 0; i<track->nDetCrossings; i++){
                        if ((SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 1)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 2)|| (SubDetId::GetTPC(track->DetCrossings[i]->Detector) == 3)) {
                            nbin = mupiEff->FindBin(track->Direction[2],track->Momentum);
                            eff = mupiEff->GetBinContent(nbin);
                            if(randm < eff){
                                truebox->npion++;
                                if(debug==1) std::cout<<"mu- TPC false pim "<< track->Direction[2]<<" "<<track->Momentum<< " Eff "<< eff << " Rand "<<randm<<std::endl;
                                break;
                            }
                        }
                    }
                }
            }
        }
        if(debug==1 && (truebox->npion+truebox->npion_pos)>0) std::cout<<truebox->npion+truebox->npion_pos<<std::endl;
        if(debug==1 && (truebox->npion+truebox->npion_pos)>0) std::cout<<"==============================="<<std::endl;
    }
    delete mydummycanvas;
    return true;
}
/*
 //**************************************************
 bool trueUtils::PionEfficiencyAction::Apply(const AnaEventB& event, AnaBoxB& box) const{
 //**************************************************
 
 
 AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
 
 if (!truebox->Vertex) return false;
 if(!truebox->Leptontrack) return false;
 AnaTrueVertexB* vertex = truebox->Vertex;
 
 for (UInt_t j=0;j<vertex->nTrueTracks;j++){
 AnaTrueParticleB* track = vertex->TrueTracks[j];
 if ((track->PDG == -211)|| (track->PDG == 111)) {
 truebox->npion++;
 }
 if ((track->PDG == 211)){
 truebox->npion_pos++;
 }
 }
 return true;
 }
 */
//**************************************************
bool SmearingAction::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if (!truebox->Vertex) return false;
    if(!truebox->Leptontrack) return false;
    AnaTrueParticleB* track = truebox->Leptontrack;
    
    double smears[10]= { 0.036, 0.043, 0.053,0.07,0.09,0.093,0.11,0.125,0.12,0.13 };
    double smearing = 0;
    if(track->Momentum<200) smearing= smears[0];
    else if((track->Momentum>=200) && (track->Momentum<400)) smearing= smears[1];
    else if((track->Momentum>=400) && (track->Momentum<600)) smearing= smears[2];
    else if((track->Momentum>=600) && (track->Momentum<800)) smearing= smears[3];
    else if((track->Momentum>=800) && (track->Momentum<1000)) smearing= smears[4];
    else if((track->Momentum>=1000) && (track->Momentum<1200)) smearing= smears[5];
    else if((track->Momentum>=1200) && (track->Momentum<1400)) smearing= smears[6];
    else if((track->Momentum>=1400) && (track->Momentum<1600)) smearing= smears[7];
    else if((track->Momentum>=1600) && (track->Momentum<1800)) smearing= smears[8];
    else if((track->Momentum>=1800)) smearing= smears[9];
    
    double mom_smeared =0;
    TRandom3 *r3 = new TRandom3(int(truebox->Leptontrack->Momentum));
    
    mom_smeared = r3->Gaus(track->Momentum, track->Momentum * smearing);
    double costheta_add = r3->Gaus(0.0, 0.004);
    
    //Used Direction[0] as a flag, should definitely change this
    if(track->Direction[0] != 0){
        track->Momentum = mom_smeared;
        if((track->Direction[2]+costheta_add)>-1.0 && (track->Direction[2]+costheta_add)<1.0 ){
            track->Direction[2]+=costheta_add;
        }
        track->Direction[0]=0;
    }
    
    
    return true;
}
/*
 
 //**************************************************
 bool trueUtils::SmearingAction::Apply(const AnaEventB& event, AnaBoxB& box) const{
 //**************************************************
 
 AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
 
 if (!truebox->Vertex) return false;
 if(!truebox->Leptontrack) return false;
 //std::cout<<"Number of positive pions : "<< truebox->npion_pos<<" pions : "<<truebox->npion<< " Muon kinematics "<< truebox->Leptontrack->Momentum << " "<<truebox->Leptontrack->Direction[2]<<std::endl;
 
 return true;
 }
 */

//**************************************************
bool CC0piCut::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if (!truebox->Vertex) return false;
    if (!truebox->Leptontrack) return false;
    TRandom3 *r3 = new TRandom3(int(truebox->Leptontrack->Momentum));
    
    if((truebox->npion + truebox->npion_pos) == 0) {
        double  randm = r3->Rndm();
        if(randm<0.001993){
            truebox->npion_pos = 1;
        }
        else{
            randm = r3->Rndm();
            if(randm<0.005361){
                truebox->npion = 2;
            }
            else if(randm<0.9325){
                //std::cout<<"Enu: "<< truebox->Vertex->NuEnergy<<"Number of positive pions : "<< truebox->npion_pos<<" pions : "<<truebox->npion<< " Muon kinematics "<< truebox->Leptontrack->Momentum << " "<<truebox->Leptontrack->Direction[2]<<std::endl;
                return true;
            }
        }
    }
    
    
    return false;
    //return true;
    
}

//**************************************************
bool CC1piCut::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if (!truebox->Vertex) return false;
    if (!truebox->Leptontrack) return false;
    
    if(truebox->npion_pos == 1 && truebox->npion == 0){
        //std::cout<<"Enu: "<< truebox->Vertex->NuEnergy<<"Number of positive pions : "<< truebox->npion_pos<<" pions : "<<truebox->npion<< " Muon kinematics "<< truebox->Leptontrack->Momentum << " "<<truebox->Leptontrack->Direction[2]<<std::endl;
        return true;
    }
    
    return false;
    //return true;
    
}

//**************************************************
bool CCOtherCut::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if (!truebox->Vertex) return false;
    if (!truebox->Leptontrack) return false;
    
    if(truebox->npion >0) return true;
    else if (truebox->npion_pos >1){
        double  randm = truebox->random_true->Rndm();
        return true;
        //std::cout<<"Enu: "<< truebox->Vertex->NuEnergy<<"Number of positive pions : "<< truebox->npion_pos<<" pions : "<<truebox->npion<< " Muon kinematics "<< truebox->Leptontrack->Momentum << " "<<truebox->Leptontrack->Direction[2]<<std::endl;
    }
    
    return false;
    //return true;
    
}

//**************************************************
bool inTarget1Cut::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if (!truebox->Vertex) return false;
    if (!truebox->Leptontrack) return false;
    
    AnaTrueVertexB* vertex = truebox->Vertex;
    if(anaUtils::InFiducialVolume(SubDetId::kTarget1,vertex->Position)) return true;
    
    return false;
}
//**************************************************
bool inTarget2Cut::Apply(AnaEventC & eventB, ToyBoxB & box) const{
    //**************************************************
            AnaEventB& event = *static_cast<AnaEventB*>(&eventB); 

    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if (!truebox->Vertex) return false;
    if (!truebox->Leptontrack) return false;
    
    AnaTrueVertexB* vertex = truebox->Vertex;
    if(anaUtils::InFiducialVolume(SubDetId::kTarget2,vertex->Position)) return true;
    
    return false;
}

}

bool trueCCMultiPiSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
    //********************************************************************
    //CCOther Target2
    //CCOther Target2
        //CC0pi
    if(allCutsPassed[0]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC0Pi;
    }  
    //CC1pi
    else if (allCutsPassed[1]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget2NuMuCC0Pi;
    }
    //CCOther
    else if (allCutsPassed[2]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCC1Pi;
    }
    else if (allCutsPassed[3]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget2NuMuCC1Pi;
    }
    else if (allCutsPassed[4]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget1NuMuCCOther;
    }
    else if (allCutsPassed[5]){
        static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kTarget2NuMuCCOther;
    }

    // otherwise kUnassigned is used from the EventSummary constructor
    return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}
//********************************************************************
bool FillSummaryAction_trueCCMultiPi::Apply(AnaEventC& event, ToyBoxB& box) const{
    //********************************************************************
    
    AnaTrueBox * truebox = static_cast<AnaTrueBox*>(&box);
    
    if(!truebox->Leptontrack) return 1;
    if(!truebox->Vertex) return 1;
    

//    static_cast<AnaEventSummaryB*>(event.Summary)->LeptonCandidate[SampleId::kTarget1NuMuCC0Pi] = truebox->Leptontrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCC0Pi] = truebox->Vertex;
//    static_cast<AnaEventSummaryB*>(event.Summary)->TrueLeptonCandidate[SampleId::kTarget1NuMuCC1Pi] = truebox->Leptontrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCC1Pi] = truebox->Vertex;
  //  static_cast<AnaEventSummaryB*>(event.Summary)->TrueLeptonCandidate[SampleId::kTarget1NuMuCCOther] = truebox->Leptontrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget1NuMuCCOther] = truebox->Vertex;
    //static_cast<AnaEventSummaryB*>(event.Summary)->TrueLeptonCandidate[SampleId::kTarget2NuMuCC0Pi] = truebox->Leptontrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget2NuMuCC0Pi] = truebox->Vertex;
   // static_cast<AnaEventSummaryB*>(event.Summary)->TrueLeptonCandidate[SampleId::kTarget2NuMuCC1Pi] = truebox->Leptontrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget2NuMuCC1Pi] = truebox->Vertex;
   // static_cast<AnaEventSummaryB*>(event.Summary)->TrueLeptonCandidate[SampleId::kTarget2NuMuCCOther] = truebox->Leptontrack;
    static_cast<AnaEventSummaryB*>(event.Summary)->TrueVertex[SampleId::kTarget2NuMuCCOther] = truebox->Vertex;
    
    /*if ((truebox->npion_pos == 1) && truebox->PositivePionTPCtracks) {
     event.Summary->PionCandidate[nd280Samples::kTarget1NuMuCC0Pi][0] = truebox->PositivePionTPCtracks->Momentum;
     event.Summary->PionCandidate[nd280Samples::kTarget1NuMuCC0Pi][1] = truebox->PositivePionTPCtracks->Direction[2];
     event.Summary->PionCandidate[nd280Samples::kTarget1NuMuCC1Pi][0] = truebox->PositivePionTPCtracks->Momentum;
     event.Summary->PionCandidate[nd280Samples::kTarget1NuMuCC1Pi][1] = truebox->PositivePionTPCtracks->Direction[2];
     event.Summary->PionCandidate[nd280Samples::kTarget1NuMuCCOther][0] = truebox->PositivePionTPCtracks->Momentum;
     event.Summary->PionCandidate[nd280Samples::kTarget1NuMuCCOther][1] = truebox->PositivePionTPCtracks->Direction[2];
     event.Summary->PionCandidate[nd280Samples::kTarget2NuMuCC0Pi][0] = truebox->PositivePionTPCtracks->Momentum;
     event.Summary->PionCandidate[nd280Samples::kTarget2NuMuCC0Pi][1] = truebox->PositivePionTPCtracks->Direction[2];
     event.Summary->PionCandidate[nd280Samples::kTarget2NuMuCC1Pi][0] = truebox->PositivePionTPCtracks->Momentum;
     event.Summary->PionCandidate[nd280Samples::kTarget2NuMuCC1Pi][1] = truebox->PositivePionTPCtracks->Direction[2];
     event.Summary->PionCandidate[nd280Samples::kTarget2NuMuCCOther][0] = truebox->PositivePionTPCtracks->Momentum;
     event.Summary->PionCandidate[nd280Samples::kTarget2NuMuCCOther][1] = truebox->PositivePionTPCtracks->Direction[2];
     }*/
    return 1;
}
