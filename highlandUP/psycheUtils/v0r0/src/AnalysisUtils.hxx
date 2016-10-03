#ifndef AnalysisUtils_h
#define AnalysisUtils_h

#include "BaseDataClasses.hxx"
#include "BinnedParams.hxx"
#include "BasicUtils.hxx"
#include "SelectionBase.hxx"
#include "HEPConstants.hxx"
#include "KinematicsUtils.hxx"
#include "SubDetUtils.hxx"

/// This namespace contains useful functions for analyses. Functions include
/// those related to fiducial volumes, accessing tracks with specific
/// characteristics, and more.
namespace anaUtils{

    //----- Utility functions -------------


    /// Get The software version from the path of the package
    std::string GetSoftwareVersionFromPath(const std::string& path);

    /// Get the package name provided the name of the executable being run
    std::string GetPackageNameFromProgram(const std::string& program);

    /// Get the package name provided a path containing the package name
    std::string GetPackageNameFromPath(const std::string& path);
  
    /// Get the vector of packages ordered following the package hierarchy
    std::vector<std::string> GetPackageHierarchy();

    /// Calculate the distance between two points
    float GetSeparationSquared(const Float_t* pos1, const Float_t* pos2);

    /// Compute all event weights (FluxWeight, etc. not the systematic ones !!!) and return the vector
    std::vector<Float_t> ComputeWeights(const AnaEventB& event);


    /// Get the AnaTrueParticleB in the current spill with the given ID. Return NULL if it can't be found.
    AnaTrueParticleB* GetTrueParticleByID(const AnaEventB& event, int ID);


    /// Merge the two vectors of AnaTrackBs into a single vector, with no duplicated tracks.
    std::vector<AnaTrackB*> MergeUniqueTracks(const std::vector<AnaTrackB*>& tracks1, const std::vector<AnaTrackB*>& tracks2);
}

#endif
