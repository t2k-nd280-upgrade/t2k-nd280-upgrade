
void DrawTarget
( 
 int histonum = 0,
 string infilename = "Sel_Step-Targ1-MuM-ParID0-CC_Evt0_NEvt1.root",
 bool doPrint = false,
 string tag = "SuperFGD"
  )
{
  gROOT->SetStyle("T2K");                  
  gROOT->ForceStyle();                 
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadRightMargin(0.15);
  //gStyle->SetStatX(.4); gStyle->SetStatY(.6);

  TFile *infile = new TFile(infilename.c_str(),"READ");
  
  cout << histonum << endl;

  TH2D *hMPPCHits_XY = (TH2D*)infile->Get(TString::Format("hMPPCHits_XY_%d",histonum));
  hMPPCHits_XY->SetTitle("Projection on the XY readout plane");
  TH2D *hMPPCHits_XZ = (TH2D*)infile->Get(TString::Format("hMPPCHits_XZ_%d",histonum));
  hMPPCHits_XZ->SetTitle("Projection on the XZ readout plane");
  TH2D *hMPPCHits_YZ = (TH2D*)infile->Get(TString::Format("hMPPCHits_YZ_%d",histonum));
  hMPPCHits_YZ->SetTitle("Projection on the YZ readout plane");
  
  TCanvas *cMPPCHits_XY = new TCanvas("cMPPCHits_XY","cMPPCHits_XY");
  hMPPCHits_XY->GetXaxis()->SetTitle("X position (mm)");
  hMPPCHits_XY->GetYaxis()->SetTitle("Y position (mm)");
  hMPPCHits_XY->SetStats(0);
  hMPPCHits_XY->DrawClone("colz");
  if(doPrint) cMPPCHits_XY->Print(TString::Format("%s_MPPCHits_XY.pdf",tag.c_str()).Data());

  TCanvas *cMPPCHits_XZ = new TCanvas("cMPPCHits_XZ","cMPPCHits_XZ");
  hMPPCHits_XZ->GetXaxis()->SetTitle("X position (mm)");
  hMPPCHits_XZ->GetYaxis()->SetTitle("Z position (mm)");
  hMPPCHits_XZ->SetStats(0);
  hMPPCHits_XZ->DrawClone("colz");
  if(doPrint) cMPPCHits_XZ->Print(TString::Format("%s_MPPCHits_XZ.pdf",tag.c_str()).Data());

  TCanvas *cMPPCHits_YZ = new TCanvas("cMPPCHits_YZ","cMPPCHits_YZ");
  hMPPCHits_YZ->GetXaxis()->SetTitle("Y position (mm)");
  hMPPCHits_YZ->GetYaxis()->SetTitle("Z position (mm)");
  hMPPCHits_YZ->SetStats(0);
  hMPPCHits_YZ->DrawClone("colz");
  if(doPrint) cMPPCHits_YZ->Print(TString::Format("%s_MPPCHits_YZ.pdf",tag.c_str()).Data());


  
  // Print output

  cout << endl;
  cout << "# of p.e. in each MPPC (event 0) XY: " << endl;
  double Nx = hMPPCHits_XY->GetXaxis()->GetNbins();
  double Ny = hMPPCHits_XY->GetYaxis()->GetNbins();
  for(int iy=0;iy<Ny;iy++){
    cout << "Y = " << hMPPCHits_XY->GetYaxis()->GetBinCenter(iy+1) << ": ";
    for(int ix=0;ix<Nx;ix++){
      
      double contXY = hMPPCHits_XY->GetBinContent(ix+1,iy+1);
      if(contXY>0) cout << contXY << ", ";
    }
    cout << endl;
  }
  cout << endl;
  
  cout << "# of p.e. in each MPPC (event 0) XZ: " << endl;
  Nx = hMPPCHits_XZ->GetXaxis()->GetNbins();
  double Nz = hMPPCHits_XZ->GetYaxis()->GetNbins();
  for(int ix=0;ix<Nx;ix++){
    cout << "X = " << hMPPCHits_XZ->GetXaxis()->GetBinCenter(ix+1) << ": ";
    for(int iz=0;iz<Nz;iz++){
      double contXZ = hMPPCHits_XZ->GetBinContent(ix+1,iz+1);
      if(contXZ>0) cout << contXZ << ", ";
    }
    cout << endl;
  }
  cout << endl;

  cout << "# of p.e. in each MPPC (event 0) YZ: " << endl;
  Ny = hMPPCHits_YZ->GetXaxis()->GetNbins();
  Nz = hMPPCHits_YZ->GetYaxis()->GetNbins();
  for(int iz=0;iz<Nz;iz++){
    cout << "Z = " << hMPPCHits_YZ->GetYaxis()->GetBinCenter(iz+1) << ": ";
    for(int iy=0;iy<Ny;iy++){
      double contYZ = hMPPCHits_YZ->GetBinContent(iy+1,iz+1);
      if(contYZ>0) cout << contYZ << ", ";
    }
    cout << endl;
  }
  cout << endl;


  ////


  cout << endl;
  cout << "# of p.e. on X projection:" << endl;
  cout << " - from XY: ";
  double Nx = hMPPCHits_XY->GetXaxis()->GetNbins();
  double Ny = hMPPCHits_XY->GetYaxis()->GetNbins();
  cout << "   Nx = " << Nx << ", Ny = " << Ny << endl;
  cout << "   ";
  for(int ix=0;ix<Nx;ix++){
    double bincont = 0;
    for(int iy=0;iy<Ny;iy++){
      bincont += hMPPCHits_XY->GetBinContent(ix+1,iy+1);
    }
    if(bincont>0.) cout << bincont << " ";
  }
  cout << endl;
  cout << " - from XZ: ";
  double Nx = hMPPCHits_XZ->GetXaxis()->GetNbins();
  double Nz = hMPPCHits_XZ->GetYaxis()->GetNbins();
  cout << "   Nx = " << Nx << ", Nz = " << Nz << endl;
  cout << "   ";
  for(int ix=0;ix<Nx;ix++){
    double bincont = 0;
    for(int iz=0;iz<Nz;iz++){
      bincont += hMPPCHits_XZ->GetBinContent(ix+1,iz+1);
    }
    if(bincont>0.) cout << bincont << " ";
  }
  cout << endl;

  cout << endl;
  cout << "# of p.e. on Y projection:" << endl;
  cout << " - from XY: ";
  double Nx = hMPPCHits_XY->GetXaxis()->GetNbins();
  double Ny = hMPPCHits_XY->GetYaxis()->GetNbins();
  cout << "   Nx = " << Nx << ", Ny = " << Ny << endl;
  cout << "   ";
  for(int iy=0;iy<Ny;iy++){
    double bincont = 0;
    for(int ix=0;ix<Nx;ix++){
      bincont += hMPPCHits_XY->GetBinContent(ix+1,iy+1);
    }
    if(bincont>0.) cout << bincont << " ";
  }
  cout << endl;
  cout << " - from YZ: ";
  double Ny = hMPPCHits_YZ->GetXaxis()->GetNbins();
  double Nz = hMPPCHits_YZ->GetYaxis()->GetNbins();
  cout << "   Ny = " << Ny << ", Nz = " << Nz << endl;
  cout << "   ";
  for(int iy=0;iy<Ny;iy++){
    double bincont = 0;
    for(int iz=0;iz<Nz;iz++){
      bincont += hMPPCHits_YZ->GetBinContent(iy+1,iz+1);
    }
    if(bincont>0.) cout << bincont << " ";
  }
  cout << endl;

 cout << endl;
  cout << "# of p.e. on Z projection:" << endl;
  cout << " - from YZ: ";
  double Ny = hMPPCHits_YZ->GetXaxis()->GetNbins();
  double Nz = hMPPCHits_YZ->GetYaxis()->GetNbins();
  cout << "   Ny = " << Ny << ", Nz = " << Nz << endl;
  cout << "   ";
  for(int iz=0;iz<Nz;iz++){
    double bincont = 0;
    for(int iy=0;iy<Ny;iy++){
      bincont += hMPPCHits_YZ->GetBinContent(iy+1,iz+1);
    }
    if(bincont>0.) cout << bincont << " ";
  }
  cout << endl;
  cout << " - from XZ: ";
  double Nx = hMPPCHits_XZ->GetXaxis()->GetNbins();
  double Nz = hMPPCHits_XZ->GetYaxis()->GetNbins();
  cout << "   Nx = " << Nx << ", Nz = " << Nz << endl;
  cout << "   ";
  for(int iz=0;iz<Nz;iz++){
    double bincont = 0;
    for(int ix=0;ix<Nx;ix++){
      bincont += hMPPCHits_XZ->GetBinContent(ix+1,iz+1);
    }
    if(bincont>0.) cout << bincont << " ";
  }
  cout << endl;
  return;
}
