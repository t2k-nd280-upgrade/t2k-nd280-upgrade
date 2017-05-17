
void DrawTarget
( 
 string infilename = "SelND280UpHit-PiP_1.root",
 int histonum = 0,
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

  //TH2D *hMPPCHitsVsTime_XY = (TH2D*)infile->Get(TString::Format("hMPPCHitsVsTime_XY_%d",histonum));
  //hMPPCHitsVsTime_XY->SetTitle("Projection on the XY readout plane Vs Time");
  //TH2D *hMPPCHitsVsTime_XZ = (TH2D*)infile->Get(TString::Format("hMPPCHitsVsTime_XZ_%d",histonum));
  //hMPPCHitsVsTime_XZ->SetTitle("Projection on the XZ readout plane Vs Time");
  //TH2D *hMPPCHitsVsTime_YZ = (TH2D*)infile->Get(TString::Format("hMPPCHitsVsTime_YZ_%d",histonum));
  //hMPPCHitsVsTime_YZ->SetTitle("Projection on the YZ readout plane Vs Time");

  TH2D *hMPPCHitsDelay100ns_XY = (TH2D*)infile->Get(TString::Format("hMPPCHitsDelay100ns_XY_%d",histonum));
  hMPPCHitsDelay100ns_XY->SetTitle("Projection on the XY readout plane (delay 100 ns)");
  TH2D *hMPPCHitsDelay100ns_XZ = (TH2D*)infile->Get(TString::Format("hMPPCHitsDelay100ns_XZ_%d",histonum));
  hMPPCHitsDelay100ns_XZ->SetTitle("Projection on the XZ readout plane (delay 100 ns)");
  TH2D *hMPPCHitsDelay100ns_YZ = (TH2D*)infile->Get(TString::Format("hMPPCHitsDelay100ns_YZ_%d",histonum));
  hMPPCHitsDelay100ns_YZ->SetTitle("Projection on the YZ readout plane (delay 100 ns)");

  TH2D *hPEVsTime_x = (TH2D*)infile->Get(TString::Format("hPEVsTime_x_%d",histonum));
  hPEVsTime_x->SetTitle("Time at end of X fiber");
  TH2D *hPEVsTime_y = (TH2D*)infile->Get(TString::Format("hPEVsTime_y_%d",histonum));
  hPEVsTime_y->SetTitle("Time at end of Y fiber");
  TH2D *hPEVsTime_z = (TH2D*)infile->Get(TString::Format("hPEVsTime_z_%d",histonum));
  hPEVsTime_z->SetTitle("Time at end of Z fiber");
  
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

  //TCanvas *cMPPCHitsVsTime_XY = new TCanvas("cMPPCHitsVsTime_XY","cMPPCHitsVsTime_XY");
  //hMPPCHitsVsTime_XY->GetXaxis()->SetTitle("X position (mm)");
  //hMPPCHitsVsTime_XY->GetYaxis()->SetTitle("Y position (mm)");
  //hMPPCHitsVsTime_XY->SetStats(0);
  //hMPPCHitsVsTime_XY->DrawClone("colz");
  //if(doPrint) cMPPCHitsVsTime_XY->Print(TString::Format("%s_MPPCHitsVsTime_XY.pdf",tag.c_str()).Data());
  //TCanvas *cMPPCHitsVsTime_XZ = new TCanvas("cMPPCHitsVsTime_XZ","cMPPCHitsVsTime_XZ");
  //hMPPCHitsVsTime_XZ->GetXaxis()->SetTitle("X position (mm)");
  //hMPPCHitsVsTime_XZ->GetYaxis()->SetTitle("Z position (mm)");
  //hMPPCHitsVsTime_XZ->SetStats(0);
  //hMPPCHitsVsTime_XZ->DrawClone("colz");
  //if(doPrint) cMPPCHitsVsTime_XZ->Print(TString::Format("%s_MPPCHitsVsTime_XZ.pdf",tag.c_str()).Data());
  //TCanvas *cMPPCHitsVsTime_YZ = new TCanvas("cMPPCHitsVsTime_YZ","cMPPCHitsVsTime_YZ");
  //hMPPCHitsVsTime_YZ->GetXaxis()->SetTitle("Y position (mm)");
  //hMPPCHitsVsTime_YZ->GetYaxis()->SetTitle("Z position (mm)");
  //hMPPCHitsVsTime_YZ->SetStats(0);
  //hMPPCHitsVsTime_YZ->DrawClone("colz");
  //if(doPrint) cMPPCHitsVsTime_YZ->Print(TString::Format("%s_MPPCHitsVsTime_YZ.pdf",tag.c_str()).Data());

  TCanvas *cMPPCHitsDelay100ns_XY = new TCanvas("cMPPCHitsDelay100ns_XY","cMPPCHitsDelay100ns_XY");
  hMPPCHitsDelay100ns_XY->GetXaxis()->SetTitle("X position after 100ns (mm)");
  hMPPCHitsDelay100ns_XY->GetYaxis()->SetTitle("Y position after 100ns (mm)");
  hMPPCHitsDelay100ns_XY->SetStats(0);
  hMPPCHitsDelay100ns_XY->DrawClone("colz");
  if(doPrint) cMPPCHitsDelay100ns_XY->Print(TString::Format("%s_MPPCHitsDelay100ns_XY.pdf",tag.c_str()).Data());
  TCanvas *cMPPCHitsDelay100ns_XZ = new TCanvas("cMPPCHitsDelay100ns_XZ","cMPPCHitsDelay100ns_XZ");
  hMPPCHitsDelay100ns_XZ->GetXaxis()->SetTitle("X position after 100ns (mm)");
  hMPPCHitsDelay100ns_XZ->GetYaxis()->SetTitle("Z position after 100ns (mm)");
  hMPPCHitsDelay100ns_XZ->SetStats(0);
  hMPPCHitsDelay100ns_XZ->DrawClone("colz");
  if(doPrint) cMPPCHitsDelay100ns_XZ->Print(TString::Format("%s_MPPCHitsDelay100ns_XZ.pdf",tag.c_str()).Data());
  TCanvas *cMPPCHitsDelay100ns_YZ = new TCanvas("cMPPCHitsDelay100ns_YZ","cMPPCHitsDelay100ns_YZ");
  hMPPCHitsDelay100ns_YZ->GetXaxis()->SetTitle("Y position after 100ns (mm)");
  hMPPCHitsDelay100ns_YZ->GetYaxis()->SetTitle("Z position after 100ns (mm)");
  hMPPCHitsDelay100ns_YZ->SetStats(0);
  hMPPCHitsDelay100ns_YZ->DrawClone("colz");
  if(doPrint) cMPPCHitsDelay100ns_YZ->Print(TString::Format("%s_MPPCHitsDelay100ns_YZ.pdf",tag.c_str()).Data());

  TCanvas *cPEVsTime_x = new TCanvas("cPEVsTime_x","cPEVsTime_x");
  hPEVsTime_x->GetXaxis()->SetTitle("Time at X fiber end (ns)");
  hPEVsTime_x->SetStats(0);
  hPEVsTime_x->DrawClone();
  if(doPrint) hPEVsTime_x->Print(TString::Format("%s_cPEVsTime_x.pdf",tag.c_str()).Data());
  TCanvas *cPEVsTime_y = new TCanvas("cPEVsTime_y","cPEVsTime_y");
  hPEVsTime_y->GetXaxis()->SetTitle("Time at Y fiber end (ns)");
  hPEVsTime_y->SetStats(0);
  hPEVsTime_y->DrawClone();
  if(doPrint) hPEVsTime_y->Print(TString::Format("%s_cPEVsTime_y.pdf",tag.c_str()).Data());
  TCanvas *cPEVsTime_z = new TCanvas("cPEVsTime_z","cPEVsTime_z");
  hPEVsTime_z->GetXaxis()->SetTitle("Time at Z fiber end (ns)");
  hPEVsTime_z->SetStats(0);
  hPEVsTime_z->DrawClone();
  if(doPrint) hPEVsTime_z->Print(TString::Format("%s_cPEVsTime_z.pdf",tag.c_str()).Data());
  
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
