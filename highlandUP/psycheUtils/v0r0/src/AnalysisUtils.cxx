#include "AnalysisUtils.hxx"
#include <unistd.h>

//**************************************************
Float_t anaUtils::GetSeparationSquared(const Float_t* pos1, const Float_t* pos2){
//**************************************************
    return ((pos1[0] - pos2[0])*(pos1[0] - pos2[0]) + (pos1[1] - pos2[1])*(pos1[1] - pos2[1]) + (pos1[2] - pos2[2])*(pos1[2] - pos2[2]));
}

//********************************************************************
std::vector<Float_t> anaUtils::ComputeWeights(const AnaEventB& event){
//********************************************************************

    std::vector<Float_t> weights;

    //  Only beam flux reweight for the moment
    weights.push_back(event.Weight);

    return weights;
}

//********************************************************************
std::string anaUtils::GetSoftwareVersionFromPath(const std::string& path){
//********************************************************************

    std::string version="";
    size_t lastSlash = path.find_last_of("/");
    version = path.substr(lastSlash+1);

    return version;
}

//********************************************************************
std::string anaUtils::GetPackageNameFromProgram(const std::string& program){
//********************************************************************

  // Imagine we run all an executable (../../../numuCCAnalysis/v1r12/Darwin/RunNumuCCAnalysis.exe) from 
  // another package (PWD=/hep/T2K/nd280rep/ANALYSIS/HIGHLAND2/SIFS2/highland2/numuCCMultiPiAnalysis/v1r12/cmt)

  char the_path[256];

  // Get the current working directory 
  // i.e. /hep/T2K/nd280rep/ANALYSIS/HIGHLAND2/SIFS2/highland2/numuCCMultiPiAnalysis/v1r12/cmt
  getcwd(the_path, 255);

  // Add the relative path to the executable 
  // i.e. /hep/T2K/nd280rep/ANALYSIS/HIGHLAND2/SIFS2/highland2/numuCCMultiPiAnalysis/v1r12/cmt/../../../numuCCAnalysis/v1r12/Darwin/RunNumuCCAnalysis.exe
  strcat(the_path, "/");
  strcat(the_path, program.c_str());  

  return GetPackageNameFromPath(std::string(the_path));
}

//********************************************************************
std::string anaUtils::GetPackageNameFromPath(const std::string& full_path){
//********************************************************************

  // another package (PWD=/hep/T2K/nd280rep/ANALYSIS/HIGHLAND2/SIFS2/highland2/numuCCMultiPiAnalysis/v1r12/cmt)

  char real_path[256];

  // convert into a canonical path (resolving .., env variables and symbolic links) 
  // i.e. /hep/T2K/nd280rep/ANALYSIS/HIGHLAND2/SIFS2/highland2/numuCCAnalysis/v1r12/Darwin/RunNumuCCAnalysis.exe
  realpath(full_path.c_str(), real_path);

  // convert into string
  std::string path(real_path);

  // Get all path up to the binary directory (excluded)
  // i.e. /hep/T2K/nd280rep/ANALYSIS/HIGHLAND2/SIFS2/highland2/numuCCAnalysis/v1r12
  path = path.substr(0, path.rfind(getenv("CMTCONFIG"))-1);

  // Check that the next substring is a package version and not a package name 
  // This is needed because in some configurations (i.e. jenkins validation) the package version 
  // is not present
  std::string version = path.substr(path.find_last_of("/")+1);

  if (version[0]=='v' && (version[2]=='r' || version[3]=='r') &&
      version.find("Analysis")==std::string::npos && 
      version.find("highland")==std::string::npos && 
      version.find("psyche")  ==std::string::npos){

    // Get all path up to the last "/" (excluded)
    // i.e. /hep/T2K/nd280rep/ANALYSIS/HIGHLAND2/SIFS2/highland2/numuCCAnalysis
    path = path.substr(0,path.find_last_of("/"));
  }

  // Get all path from the last "/" (excluded)
  // i.e. numuCCAnalysis
  path = path.substr(path.find_last_of("/")+1);

  return path;
}


//********************************************************************
std::vector<std::string> anaUtils::GetPackageHierarchy(){
//********************************************************************

  std::vector<std::string> packages;
  
  std::string PATHENV = getenv("PATH");

  // Get a maximum of 50 packages
  for (Int_t i=0;i<50;i++){
    size_t first = PATHENV.find_first_of(":");
    std::string path= PATHENV.substr(0,first);
    if (path.find("nd280AnalysisTools") ==std::string::npos){
      std::string package = GetPackageNameFromPath(path);
      packages.push_back(package);
      if (package == "psycheCore") break;
    }
    PATHENV= PATHENV.substr(first+1);
  }
  return packages;
}

//********************************************************************
AnaTrueParticleB* anaUtils::GetTrueParticleByID(const AnaEventB& event, int ID) {
  //********************************************************************

  for (int i = 0; i < event.nTrueParticles; i++) {
    if (event.TrueParticles[i]->ID == ID) {
      return event.TrueParticles[i];
    }
  }

  return NULL;
}

//**************************************************
std::vector<AnaTrackB*> anaUtils::MergeUniqueTracks(const std::vector<AnaTrackB*>& tracks1, const std::vector<AnaTrackB*>& tracks2) {
//**************************************************
    std::vector<AnaTrackB*> merged;

    for (std::vector<AnaTrackB*>::const_iterator it = tracks1.begin(); it != tracks1.end(); it++) {
        if (std::find(merged.begin(), merged.end(), *it) == merged.end()) {
            merged.push_back(*it);
        }
    }

    for (std::vector<AnaTrackB*>::const_iterator it = tracks2.begin(); it != tracks2.end(); it++) {
        if (std::find(merged.begin(), merged.end(), *it) == merged.end()) {
            merged.push_back(*it);
        }
    }

    return merged;
}
