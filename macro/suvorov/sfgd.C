{
	string line;
  ifstream myfile ("sfgd.txt");
  TH1D* histo = new TH1D("h", "", 29, 10.1, 10.4);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) ) {
    	double d = atof(line.c_str());
      if (d >= 10.2 && d < 10.21)
      	cout << d << '\n';

      histo->Fill(d);
    }
    myfile.close();
  }
  histo->Fit("gaus", "", "", 10.15, 10.3);
  histo->Draw();
}