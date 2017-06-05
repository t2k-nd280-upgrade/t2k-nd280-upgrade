#include "G4Run.hh"
#include "IngridRunAction.hh"
#include "CLHEP/Random/Random.h"
#include <time.h>

////////////////////////////
//IngridRunAction::IngridRunAction(char* output,float x0,float y0,double hiteff0)
IngridRunAction::IngridRunAction(char* output)
////////////////////////////
{
  time_t timer;
  strcpy(file,output);

  time(&timer);
  seed = long(timer);
  G4cout << "time: " << timer << "     seed: " << seed << G4endl; 
  x = 0.;
  y = 0.;

  hiteff = 100.;

  evt = new IngridEventSummary();

  //
  f = new TFile(file,"RECREATE");   
  tree = new TTree("tree","Tree");
  h1 = new TTree("h1","Tree");//for t2kreweight
  
  //
  //TBranch * EvtBr = tree->GetBranch("fDefaultReco.");
  tree->Branch("fDefaultReco.","IngridEventSummary",&evt,64000,99);
  
  //for T2KReWeight                                                                             
  h1->Branch("numnu",&numnu,"numnu/I");
  h1->Branch("mode",&mode,"mode/I");
  h1->Branch("ipnu",&ipnu,"ipnu[50]/I");
  h1->Branch("pnu",&pnu,"pnu[50]/F");
  h1->Branch("dirnu",&dirnu,"dirnu[50][3]/F");
  h1->Branch("Crsx",&Crsx,"Crsx/F");
  h1->Branch("Crsy",&Crsy,"Crsy/F");
  h1->Branch("Crsz",&Crsz,"Crsz/F");
  h1->Branch("Crsphi",&Crsphi,"Crsphi/F");
  h1->Branch("Nvert",&Nvert,"Nvert/I");
  h1->Branch("Posvert",&Posvert,"Posvert[150][3]/F");
  h1->Branch("Iflgvert",&Iflgvert,"Iflgvert[150]/I");
  h1->Branch("Nvcvert",&Nvcvert,"Nvcvert/I");
  h1->Branch("Dirvert",&Dirvert,"Dirvert[900][3]/F");
  h1->Branch("Abspvert",&Abspvert,"Abspvert[900]/F");
  h1->Branch("Abstpvert",&Abstpvert,"Abstpvert[900]/F");
  h1->Branch("Ipvert",&Ipvert,"Ipvert[900]/I");
  h1->Branch("Iverti",&Iverti,"Iverti[900]/I");
  h1->Branch("Ivertf",&Ivertf,"Ivertf[900]/I");
  h1->Branch("Fsiprob",&Fsiprob,"Fsiprob/F");
  h1->Branch("Numbndn",&Numbndn,"Numbndn/I");
  h1->Branch("Numbndp",&Numbndp,"Numbndp/I");
  h1->Branch("Numfrep",&Numfrep,"Numfrep/I");
  h1->Branch("Numatom",&Numatom,"Numatom/I");
  h1->Branch("Ibound",&Ibound,"Ibound/I");
  h1->Branch("nring",&nring,"nring/I");
  h1->Branch("nev",&nev,"nev/I");
  h1->Branch("nhitac",&nhitac,"nhitac/i");
  h1->Branch("wall",&wall,"wall/F");
  h1->Branch("evis",&evis,"evis/F");
  h1->Branch("ip",&ip,"ip[10]/i");
  h1->Branch("dir",&dir,"dir[10][3]/F");
  h1->Branch("amom",&amom,"amom[10]/F");
  h1->Branch("amome",&amome,"amome[10]/F");
  h1->Branch("amomm",&amomm,"amomm[10]/F");
  h1->Branch("pi0like",&pi0like,"pi0like[2]/F");
  h1->Branch("pi0_e",&pi0_e,"pi0_e[2][2]/F");
  h1->Branch("pi0_dir",&pi0_dir,"pi0_dir[2][2][3]/F");
  h1->Branch("pi0mass",&pi0mass,"pi0mass[2]/F");
  h1->Branch("nmue",&nmue,"nmue/I");
  h1->Branch("npar",&npar,"npar/I");
  h1->Branch("wallv",&wallv,"wallv/F");
  h1->Branch("ipv",&ipv,"ipv[50]/i");
  h1->Branch("posv",&posv,"posv[3]/F");
  h1->Branch("dirv",&dirv,"dirv[50][3]/F");
  h1->Branch("pmomv",&pmomv,"pmomv[50]/F");
  h1->Branch("nscndprt",&nscndprt,"nscndprt/I");
  h1->Branch("itrkscnd",&itrkscnd,"itrkscnd[200]/I");
  h1->Branch("vtxscnd",&vtxscnd,"vtxscnd[200][3]/F");
  h1->Branch("pscnd",&pscnd,"pscnd[200][3]/F");
  h1->Branch("iprtscnd",&iprtscnd,"iprtscnd[200]/I");
  h1->Branch("tscnd",&tscnd,"tscnd[200]/F");
  h1->Branch("iprntprt",&iprntprt,"iprntprt[200]/I");
  h1->Branch("lmecscnd",&lmecscnd,"lmecscnd[200]/I");
  h1->Branch("iprnttrk",&iprnttrk,"iprnttrk[200]/I");
  h1->Branch("iorgprt",&iorgprt,"iorgprt[200]/I");
  h1->Branch("iflgscnd",&iflgscnd,"iflgscnd[200]/I");
  h1->Branch("pprnt",&pprnt,"pprnt[200][3]/F");
  h1->Branch("Npvc",&Npvc,"Npvc/I");
  h1->Branch("Ipvc",&Ipvc,"Ipvc[100]/I");
  h1->Branch("Ichvc",&Ichvc,"Ichvc[100]/I");
  h1->Branch("Iorgvc",&Iorgvc,"Iorgvc[100]/I");
  h1->Branch("Iflvc",&Iflvc,"Iflvc[100]/I");
  h1->Branch("Abspvc",&Abspvc,"Abspvc[100]/F");
  h1->Branch("Pvc",&Pvc,"Pvc[100][3]/F");


  //
  CLHEP::HepRandom::setTheSeed(seed);  
}

/////////////////////////////
IngridRunAction::~IngridRunAction()
/////////////////////////////
{
	if(evt) delete evt;
}

//////////////////////////////////////////////////////
void IngridRunAction::BeginOfRunAction(const G4Run* aRun)
//////////////////////////////////////////////////////
{

}


////////////////////////////////////////////////////
void IngridRunAction::EndOfRunAction(const G4Run* aRun)
////////////////////////////////////////////////////
{ 
    G4cout << " Not Entry to INGRID " << NotEntry << G4endl;

    tree->Write();
    if(tree) delete tree;
    
    h1->Write();//for t2kreweight
    if(h1) delete h1;//for t2kreweight                                                            
    f->Close();
    if(f) delete f;

}

 
