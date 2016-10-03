#include "FluxWeighting.hxx"
#include "ND280UPAnalysisUtils.hxx"

//********************************************************************
FluxWeighting::FluxWeighting(std::string fluxfolder, std::string version, std::string tuning) {
//********************************************************************

    bool load_run5 = false;
    bool load_run6 = false;
    if(fluxfolder.find("13a") != std::string::npos){
      load_run5 = true;
      if(fluxfolder.find("av1.1") != std::string::npos)
        load_run6 = true;
    }

    std::cout << " ------------------------------------------------------ " << std::endl;
    std::cout << " ------------------------------------------------------ " << std::endl;
    std::cout << "   Flux reweight is enabled " << std::endl; 
    std::cout << "   Folder   : " <<  fluxfolder << std::endl; 
    std::cout << "   File     : " << fluxfolder + "/runXXX/nd5_" + version + "_runXXX.root" << std::endl; 
    std::cout << "   Tuning   : " << tuning << std::endl; 
    std::cout << " ------------------------------------------------------ " << std::endl;
    std::cout << " ------------------------------------------------------ " << std::endl;

    std::string file1 = "/run1/nd5_" + version + "_run1.root";
    std::string file2 = "/run2/nd5_" + version + "_run2.root";
    std::string file3b = "/run3b/nd5_" + version + "_run3b.root";
    std::string file3c = "/run3c/nd5_" + version + "_run3c.root";
    std::string file4 = "/run4/nd5_" + version + "_run4.root";
    std::string file5a = "/run5a/nd5_" + version + "_run5a.root";
    std::string file5b = "/run5b/nd5_" + version + "_run5b.root";
    std::string file5c = "/run5c/nd5_" + version + "_run5c_antinumode.root";
    std::string file6b = "/run6b/nd5_" + version + "_run6b_antinumode.root";
    std::string file6c = "/run6c/nd5_" + version + "_run6c_antinumode.root";
    std::string file6d = "/run6d/nd5_" + version + "_run6d_antinumode.root";
    std::string file6e = "/run6e/nd5_" + version + "_run6e_antinumode.root";

    run1_file = new TFile(file1.insert(0,fluxfolder).c_str(), "READ");
    run2_file = new TFile(file2.insert(0,fluxfolder).c_str(), "READ");
    run3b_file = new TFile(file3b.insert(0,fluxfolder).c_str(), "READ");
    run3c_file = new TFile(file3c.insert(0,fluxfolder).c_str(), "READ");
    run4_file = new TFile(file4.insert(0,fluxfolder).c_str(), "READ");

    if(load_run5){
        run5a_file = new TFile(file5a.insert(0,fluxfolder).c_str(), "READ");
        run5b_file = new TFile(file5b.insert(0,fluxfolder).c_str(), "READ");
        run5c_file = new TFile(file5c.insert(0,fluxfolder).c_str(), "READ");
    } 
    else{
        run5a_file = NULL; 
        run5b_file = NULL; 
        run5c_file = NULL; 
    }

    if(load_run6){
        run6b_file = new TFile(file6b.insert(0,fluxfolder).c_str(), "READ");
        run6c_file = new TFile(file6c.insert(0,fluxfolder).c_str(), "READ");
        run6d_file = new TFile(file6d.insert(0,fluxfolder).c_str(), "READ");
        run6e_file = new TFile(file6e.insert(0,fluxfolder).c_str(), "READ");
    }
    else{
        run6b_file = NULL; 
        run6c_file = NULL; 
        run6d_file = NULL; 
        run6e_file = NULL; 
    }

    // Get the histograms
    run1_weight_numu = (TH1D*) run1_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str());
    run1_weight_numubar = (TH1D*) run1_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str());
    run1_weight_nue = (TH1D*) run1_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str());
    run1_weight_nuebar = (TH1D*) run1_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str());

    run2_weight_numu = (TH1D*) (run2_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
    run2_weight_numubar = (TH1D*) (run2_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
    run2_weight_nue = (TH1D*) (run2_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
    run2_weight_nuebar = (TH1D*) (run2_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));

    run3b_weight_numu = (TH1D*) (run3b_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
    run3b_weight_numubar = (TH1D*) (run3b_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
    run3b_weight_nue = (TH1D*) (run3b_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
    run3b_weight_nuebar = (TH1D*) (run3b_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));

    run3c_weight_numu = (TH1D*) (run3c_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
    run3c_weight_numubar = (TH1D*) (run3c_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
    run3c_weight_nue = (TH1D*) (run3c_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
    run3c_weight_nuebar = (TH1D*) (run3c_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));

    run4_weight_numu = (TH1D*) (run4_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
    run4_weight_numubar = (TH1D*) (run4_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
    run4_weight_nue = (TH1D*) (run4_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
    run4_weight_nuebar = (TH1D*) (run4_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));

    if(load_run5){
        run5a_weight_numu = (TH1D*) (run5a_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
        run5a_weight_numubar = (TH1D*) (run5a_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
        run5a_weight_nue = (TH1D*) (run5a_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
        run5a_weight_nuebar = (TH1D*) (run5a_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));
    
        run5b_weight_numu = (TH1D*) (run5b_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
        run5b_weight_numubar = (TH1D*) (run5b_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
        run5b_weight_nue = (TH1D*) (run5b_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
        run5b_weight_nuebar = (TH1D*) (run5b_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));
    
        run5c_weight_numu = (TH1D*) (run5c_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
        run5c_weight_numubar = (TH1D*) (run5c_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
        run5c_weight_nue = (TH1D*) (run5c_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
        run5c_weight_nuebar = (TH1D*) (run5c_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));
    }
    else{
        run5a_weight_numu = NULL;
        run5a_weight_numubar = NULL;
        run5a_weight_nue = NULL;
        run5a_weight_nuebar = NULL;
    
        run5b_weight_numu = NULL;
        run5b_weight_numubar = NULL;
        run5b_weight_nue = NULL;
        run5b_weight_nuebar = NULL;
    
        run5c_weight_numu = NULL;
        run5c_weight_numubar = NULL;
        run5c_weight_nue = NULL;
        run5c_weight_nuebar = NULL;
    }
    if(load_run6){
        run6b_weight_numu = (TH1D*) (run6b_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
        run6b_weight_numubar = (TH1D*) (run6b_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
        run6b_weight_nue = (TH1D*) (run6b_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
        run6b_weight_nuebar = (TH1D*) (run6b_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));
        run6c_weight_numu = (TH1D*) (run6c_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
        run6c_weight_numubar = (TH1D*) (run6c_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
        run6c_weight_nue = (TH1D*) (run6c_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
        run6c_weight_nuebar = (TH1D*) (run6c_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));
        run6d_weight_numu = (TH1D*) (run6d_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
        run6d_weight_numubar = (TH1D*) (run6d_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
        run6d_weight_nue = (TH1D*) (run6d_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
        run6d_weight_nuebar = (TH1D*) (run6d_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));
        run6e_weight_numu = (TH1D*) (run6e_file->Get(("enu_nd5_" + tuning + "_numu_ratio").c_str()));
        run6e_weight_numubar = (TH1D*) (run6e_file->Get(("enu_nd5_" + tuning + "_numub_ratio").c_str()));
        run6e_weight_nue = (TH1D*) (run6e_file->Get(("enu_nd5_" + tuning + "_nue_ratio").c_str()));
        run6e_weight_nuebar = (TH1D*) (run6e_file->Get(("enu_nd5_" + tuning + "_nueb_ratio").c_str()));

    }
    else{
        run6b_weight_numu = NULL;
        run6b_weight_numubar = NULL;
        run6b_weight_nue = NULL;
        run6b_weight_nuebar = NULL;
        run6c_weight_numu = NULL;
        run6c_weight_numubar = NULL;
        run6c_weight_nue = NULL;
        run6c_weight_nuebar = NULL;
        run6d_weight_numu = NULL;
        run6d_weight_numubar = NULL;
        run6d_weight_nue = NULL;
        run6d_weight_nuebar = NULL;
        run6e_weight_numu = NULL;
        run6e_weight_numubar = NULL;
        run6e_weight_nue = NULL;
        run6e_weight_nuebar = NULL;
    }
       
    if (!run1_weight_numu || !run1_weight_numubar || !run1_weight_nue || !run1_weight_nuebar) {
        std::cerr << "ERROR: Flux weighting was requested, but could not be initialised." << std::endl;
        std::cerr << "  Specify a valid flux file and tuning in your parameters file, or" << std::endl;
        std::cerr << "  disable the flux weighting. Flux files can be downloaded from" << std::endl;
        std::cerr << "  http://www.t2k.org/beam/NuFlux/FluxRelease" << std::endl;
        exit(EXIT_FAILURE);
    }
}

//********************************************************************
FluxWeighting::~FluxWeighting() {
//********************************************************************
    if (run1_weight_numu) {
        delete run1_weight_numu;
        delete run2_weight_numu;
        delete run3b_weight_numu;
        delete run3c_weight_numu;
        delete run4_weight_numu;
        if(run5a_weight_numu){
            delete run5a_weight_numu;
            delete run5b_weight_numu;
            delete run5c_weight_numu;
            if(run6b_weight_numu){
              delete run6b_weight_numu;
              delete run6c_weight_numu;
              delete run6d_weight_numu;
              delete run6e_weight_numu;
            }
        }
    }
    if (run1_weight_numubar) {
        delete run1_weight_numubar;
        delete run2_weight_numubar;
        delete run3b_weight_numubar;
        delete run3c_weight_numubar;
        delete run4_weight_numubar;
        if(run5a_weight_numubar){
            delete run5a_weight_numubar;
            delete run5b_weight_numubar;
            delete run5c_weight_numubar;
            if (run6b_weight_numubar){
              delete run6b_weight_numubar;
              delete run6c_weight_numubar;
              delete run6d_weight_numubar;
              delete run6e_weight_numubar;
            }
        }
    }
    if (run1_weight_nue) {
        delete run1_weight_nue;
        delete run2_weight_nue;
        delete run3b_weight_nue;
        delete run3c_weight_nue;
        delete run4_weight_nue;
        if(run5a_weight_nue){
            delete run5a_weight_nue;
            delete run5b_weight_nue;
            delete run5c_weight_nue;
            if(run6b_weight_nue){
              delete run6b_weight_nue;
              delete run6c_weight_nue;
              delete run6d_weight_nue;
              delete run6e_weight_nue;
            }
        }
    }
    if (run1_weight_nuebar) {
        delete run1_weight_nuebar;
        delete run2_weight_nuebar;
        delete run3b_weight_nuebar;
        delete run3c_weight_nuebar;
        delete run4_weight_nuebar;
        if(run5a_weight_nuebar){
            delete run5a_weight_nuebar;
            delete run5b_weight_nuebar;
            delete run5c_weight_nuebar;
            if(run6b_weight_nuebar){
              delete run6b_weight_nuebar;
              delete run6c_weight_nuebar;
              delete run6d_weight_nuebar;
              delete run6e_weight_nuebar;
            }
        }
    }
    if (run1_file) {
        delete run1_file;
        delete run2_file;
        delete run3b_file;
        delete run3c_file;
        delete run4_file;
        if(run5a_file){
            delete run5a_file;
            delete run5b_file;
            delete run5c_file;
            if(run6b_file){
              delete run6b_file;
              delete run6c_file;
              delete run6d_file;
              delete run6e_file;
            }
        }
    }
}

//********************************************************************
void FluxWeighting::UpdateBunchWeight(AnaBunchB& bunch, AnaTrueVertexB* vertex, int RunPeriod) {
//********************************************************************
    Float_t weight = GetWeight(vertex, RunPeriod);
    bunch.Weight *= weight;
}

//********************************************************************
void FluxWeighting::UpdateEventWeight(AnaEventB& event, AnaTrueVertexB* vertex) {
//********************************************************************
    Float_t weight = GetWeight(vertex, anaUtils::GetRunPeriod(event.EventInfo.Run));
    event.Weight *= weight;
}


//********************************************************************
Float_t FluxWeighting::GetWeight(AnaTrueVertexB* vertex, int RunPeriod) {
//********************************************************************
    Float_t weight = 1.;

    if (vertex) {
        weight = GetWeight(*vertex, RunPeriod);
    }

    return weight;
}

//********************************************************************
Float_t FluxWeighting::GetWeight(const AnaTrueVertexB& vertex, int RunPeriod) {
//********************************************************************

    Float_t weight = 1.;
    TH1D* hist = NULL;

    switch (vertex.NuPDG) {
        case 14:
            switch (RunPeriod) {
                case 0:
                    hist = run1_weight_numu;
                    break;
                case 1:
                    hist = run2_weight_numu;
                    break;
                case 2:
                    hist = run2_weight_numu;
                    break;
                case 3:
                    hist = run3b_weight_numu;
                    break;
                case 4:
                    hist = run3c_weight_numu;
                    break;
                case 5:
                    hist = run4_weight_numu;
                    break;
                case 6:
                    hist = run4_weight_numu;
                    break;
                case 7:
                    hist = run5a_weight_numu;
                    break;
                case 8:
                    hist = run5c_weight_numu;
                    break;
                case 9:
                    hist = run6b_weight_numu;
                    break;
                case 10:
                    hist = run6c_weight_numu;
                    break;
                case 11:
                    hist = run6d_weight_numu;
                    break;
                case 12:
                    hist = run6e_weight_numu;
                    break;
                default:
                    std::cerr << "Unknown run number " << RunPeriod << ": setting event weight to 1." << std::endl;
                    break;
            }
            break;
        case -14:
            switch (RunPeriod) {
                case 0:
                    hist = run1_weight_numubar;
                    break;
                case 1:
                    hist = run2_weight_numubar;
                    break;
                case 2:
                    hist = run2_weight_numubar;
                    break;
                case 3:
                    hist = run3b_weight_numubar;
                    break;
                case 4:
                    hist = run3c_weight_numubar;
                    break;
                case 5:
                    hist = run4_weight_numubar;
                    break;
                case 6:
                    hist = run4_weight_numubar;
                    break;
                case 7:
                    hist = run5a_weight_numubar;
                    break;
                case 8:
                    hist = run5c_weight_numubar;
                    break;
                case 9:
                    hist = run6b_weight_numubar;
                    break;
                case 10:
                    hist = run6c_weight_numubar;
                    break;
                case 11:
                    hist = run6d_weight_numubar;
                    break;
                case 12:
                    hist = run6e_weight_numubar;
                    break;
                default:
                    std::cerr << "Unknown run number " << RunPeriod << ": setting event weight to 1." << std::endl;
                    break;
            }
            break;
        case 12:
            switch (RunPeriod) {
                case 0:
                    hist = run1_weight_nue;
                    break;
                case 1:
                    hist = run2_weight_nue;
                    break;
                case 2:
                    hist = run2_weight_nue;
                    break;
                case 3:
                    hist = run3b_weight_nue;
                    break;
                case 4:
                    hist = run3c_weight_nue;
                    break;
                case 5:
                    hist = run4_weight_nue;
                    break;
                case 6:
                    hist = run4_weight_nue;
                    break;
                case 7:
                    hist = run5a_weight_nue;
                    break;
                case 8:
                    hist = run5c_weight_nue;
                    break;
                case 9:
                    hist = run6b_weight_nue;
                    break;
                case 10:
                    hist = run6c_weight_nue;
                    break;
                case 11:
                    hist = run6d_weight_nue;
                    break;
                case 12:
                    hist = run6e_weight_nue;
                    break;
                default:
                    std::cerr << "Unknown run number " << RunPeriod << ": setting event weight to 1." << std::endl;
                    break;
            }
            break;
        case -12:
            switch (RunPeriod) {
                case 0:
                    hist = run1_weight_nuebar;
                    break;
                case 1:
                    hist = run2_weight_nuebar;
                    break;
                case 2:
                    hist = run2_weight_nuebar;
                    break;
                case 3:
                    hist = run3b_weight_nuebar;
                    break;
                case 4:
                    hist = run3c_weight_nuebar;
                    break;
                case 5:
                    hist = run4_weight_nuebar;
                    break;
                case 6:
                    hist = run4_weight_nuebar;
                    break;
                case 7:
                    hist = run5a_weight_nuebar;
                    break;
                case 8:
                    hist = run5c_weight_nuebar;
                    break;
                case 9:
                    hist = run6b_weight_nuebar;
                    break;
                case 10:
                    hist = run6c_weight_nuebar;
                    break;
                case 11:
                    hist = run6d_weight_nuebar;
                    break;
                case 12:
                    hist = run6e_weight_nuebar;
                    break;
                default:
                    std::cerr << "Unknown run number " << RunPeriod << ": setting event weight to 1." << std::endl;
                    break;
            }
            break;
        default:
            std::cerr << "Unknown neutrino flavour " << vertex.NuPDG << ": setting event weight to 1." << std::endl;
            break;
    }

    if (hist) {
        int bin = hist->FindBin(vertex.NuEnergy/1000.);
        weight = hist->GetBinContent(bin);
    }
    else{
        std::cerr << "Couldn't get correct flux weighting histogram - check you have the right tuning folder specified in the parameters file" << std::endl;
    }

    return weight;
}
