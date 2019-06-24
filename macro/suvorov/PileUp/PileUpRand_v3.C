{
  //TODO
  // 1) project gRooTracker neutron tree with StdHepN == 3, 4, 5, ...  
  // 3 - 
  // 2) same for gammas
  // 3) Download AntiNu events
  // 3.1 number of Nu_Mu events
  // 3.2 project neutron
  // 3.3 project gamma 
  // 4) Run new hits compare for both nu/antu-nu

  bool AntiNu = true;
  float beam_power = 500;

  // 1E21 POT stat
  int Nevents   = 234546;
  int Nneutrons = 867128 / 0.947;
  int Ngamma    = 100783 / 0.947;
  int Nsand     = 671186 / 1.12;

  long int bunches = (long int)(1E21 / (227024000000000 / 8 * beam_power / 440));

  long int pileuped       = 0;
  long int pileuped_n     = 0;
  long int events_pileup  = 0;
  long int event_count    = 0;
  long int pileuped_sand  = 0;

  if (AntiNu) {
    Nevents    = 60837;
    Nneutrons  = 1066520 / 3.371;
    Ngamma     = 165558 / 3.371;
  }

  TRandom3* GenSeed = new TRandom3(0);
  UInt_t seedE = (UInt_t)GenSeed->Uniform(100000000000000);
  UInt_t seedN = (UInt_t)GenSeed->Uniform(100000000000000);
  UInt_t seedG = (UInt_t)GenSeed->Uniform(100000000000000);
  UInt_t seedS = (UInt_t)GenSeed->Uniform(100000000000000);

  cout << seedE << "    " << seedN << "    " << seedG << endl;

  TRandom3* genE = new TRandom3(0);
  genE->SetSeed(seedE);
  TRandom3* genN = new TRandom3(0);
  genN->SetSeed(seedN);
  TRandom3* genG = new TRandom3(0);
  genG->SetSeed(seedG);
  TRandom3* genS = new TRandom3(0);
  genS->SetSeed(seedS);

  cout << "thowing events" << endl;

  Int_t ibunch = 0;

  vector<int> vectorE;
  vectorE.resize(bunches, 0);
  for (UInt_t ev = 0; ev < Nevents; ++ev) {
    ibunch = (int)genE->Uniform(bunches);
    ++vectorE[ibunch];
  }

  cout << "thowing neutrons" << endl;

  vector<int> vectorN;
  vectorN.resize(bunches, 0);
  for (UInt_t neutrons = 0; neutrons < Nneutrons; ++neutrons) {
    ibunch = (int)genN->Uniform(bunches);
    ++vectorN[ibunch];
  }

  cout << "thowing gammas" << endl;

  vector<int> vectorG;
  vectorG.resize(bunches, 0);
  for (UInt_t gamma = 0; gamma < Ngamma; ++gamma) {
    ibunch = (int)genG->Uniform(bunches);
    ++vectorG[ibunch];
  }

  cout << "thowing sand muons" << endl;

  vector<int> vectorS;
  vectorS.resize(bunches, 0);
  for (UInt_t sand = 0; sand < Nsand; ++sand) {
    ibunch = (int)genS->Uniform(bunches);
    ++vectorS[ibunch];
  }

  cout << "[                                                  ] N = " << bunches << "\r[";
  for (long int i = 0; i < bunches; ++i) {
    if (i % (bunches/50) == 0 ) cout << "."  << std::flush;

    if (vectorE[i] * vectorN[i] || vectorE[i] * vectorG[i])
      ++pileuped;

    if (vectorE[i] * vectorN[i])
      ++pileuped_n;

    if (vectorE[i] > 1)
      ++events_pileup;

    if (vectorE[i])
      ++event_count;

    if (vectorS[i] * vectorE[i])
      ++pileuped_sand;
  }
  cout << "]" << endl;

  //cout << "Prob e " << prob_events << endl;
  //cout << "Prob n " << prob_neutron << endl;
  //cout << "Prob g " << prob_photon << endl;

  cout << "total             : " << event_count << endl;
  cout << "Pile Up OOFV n    : " << pileuped_n << "     rate " << 100. * pileuped_n / event_count  << "%" << endl;
  cout << "Pile Up OOFV all  : " << pileuped << "     rate " << 100. * pileuped / event_count  << "%" << endl;
  cout << "Pile Up events    : " << events_pileup << "     rate " << 100. * events_pileup / event_count  << "%" << endl;
  cout << "Pile uped sand    : " << pileuped_sand << "     rate " << 100. * pileuped_sand / event_count << "%" << endl;

  exit(1);
}