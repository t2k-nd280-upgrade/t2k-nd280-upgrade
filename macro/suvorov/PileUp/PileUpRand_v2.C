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
  float beam_power = 1000;

  int Nevents   = 234546;

  long int bunches = (long int)(1E21 / (227024000000000 / 8 * beam_power / 440));

  float prob_neutron = 0.0170 * beam_power / 178.;
  float prob_photon  = 0.0010 * beam_power / 178.;
  float prob_sand    = 0.0071 * beam_power / 178.;
  float prob_events  = 1.*Nevents / bunches;

  long int pileuped       = 0;
  long int events_pileup  = 0;
  long int event_count    = 0;
  long int pileuped_sand  = 0;

  if (AntiNu) {
    prob_neutron = 0.0168 * beam_power / 385.;
    //prob_neutron = 0.00589 * beam_power / 385.;
    prob_photon  = 0.0009 * beam_power / 385.;
    // FIXME
    Nevents    = 60837;
    // Nneutrons  =
    // Ngamma     = 
  }

  //FIXME
  int Nneutrons = prob_neutron * bunches;
  int Ngamma    = prob_photon  * bunches;
  int Nsand     = prob_sand    * bunches;

  cout << "neutrons " << Nneutrons << endl;
  cout << "gammas   " << Ngamma << endl;
  cout << "sand     " << Nsand << endl;

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

    if (vectorE[i] > 1)
      ++events_pileup;

    if (vectorE[i])
      ++event_count;

    if (vectorS[i] * vectorE[i])
      ++pileuped_sand;
  }
  cout << "]" << endl;

  cout << "Prob e " << prob_events << endl;
  cout << "Prob n " << prob_neutron << endl;
  cout << "Prob g " << prob_photon << endl;

  cout << "total             : " << event_count << endl;
  cout << "Pile Up OOFV      : " << pileuped << "     rate " << 100. * pileuped / event_count  << "%" << endl;
  cout << "Pile Up events    : " << events_pileup << "     rate " << 100. * events_pileup / event_count  << "%" << endl;
  cout << "Pile uped sand    : " << pileuped_sand << "     rate " << 100. * pileuped_sand / event_count << "%" << endl;
}