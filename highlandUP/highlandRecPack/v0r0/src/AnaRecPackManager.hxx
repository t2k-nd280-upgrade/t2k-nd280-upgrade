#ifndef AnaRecPackManager_hxx_seen
#define AnaRecPackManager_hxx_seen

#include <iostream>
#include <exception>
#include <cmath>

#include <TVector3.h>
#include <TLorentzVector.h>

#include "recpack/RecPackManager.h"
#include "recpack/Setup.h"
#include <recpack/string_tools.h>
#include <recpack/stc_tools.h>
#include <recpack/ERandom.h>
#include <recpack/EAlgebra.h>
#include <recpack/HelixEquation.h>
#include <vector>
#include <map>
#include "RootSetup.h"

#include <TGeoManager.h>
#include <TGeoVolume.h>

#include <RecPackGeometry.hxx>
#include "BaseDataClasses.hxx"
#include "SubDetId.hxx"

#include "ParticleId.hxx"

const double LINMOM = 100. * units::GeV; // large momentum to have an sline inside helix model


class AnaRecPackManager {
public:
  enum TrackEndDef{
    kStart = 0, 
    kEnd
  };
  
  enum TrackDirDef{
    kFwd =  1, 
    kBwd = -1
  };

  
  virtual ~AnaRecPackManager() {};
  
  //------ Managers ----------------------------------------
  
  //// Get a pointer to the singleton instance of the geometry information.
  static AnaRecPackManager& Get(void);
  static AnaRecPackManager& Get(const std::string& m);
  
  RecPackManager& man(const std::string& m){return *_man_map[m];}
  RecPackManager& man(void){return *_man_map[_selected_man_temp];}
  RootSetup& GetSetup(void){return ND::gman().GetSetup();}
  
  void add_manager(const std::string& m, std::string model=RP::particle_helix);
  
  int NumberOfManagers(){return _man_map.size();}
  
  void select_manager(const std::string& m){_selected_man_temp = _selected_man = m;}
  std::string selected_manager(){return _selected_man;}
  
  void InitializeManager(const std::string& m, const std::string& model);
  void InitializeManagerGeometry(const std::string& m);
  
  void ResetGeometry();
   
  
  void SetAlgName(const std::string& name){fAlgNameMap[_selected_man_temp] = name;}
  std::string GetAlgName(){return fAlgNameMap[_selected_man_temp];}
  
  void SetModel(const std::string& name){fModelMap[_selected_man_temp] = name;}
  std::string GetModel(){return fModelMap[_selected_man_temp];}
  
  void SetVerbosity(int verb0=0, int verb1=0, int verb2=0, int verb3=0, int verb4=0);
  void SetVerbosity(const std::string& verbosity); 
  
  const std::string& volume_name(int ID){return ND::gman().volume_name(ID);}
  const std::string& surface_name(int ID){return ND::gman().surface_name(ID);}
 
  
  std::map<std::string, std::string> fAlgNameMap;
  std::map<std::string, std::string> fModelMap;
 
  
  //---------------------------- utility functions
  /// Get momentum from range
  /// Here we assume the linear distance between the start and stop points
  bool GetMomentumFromRangeLinear(const AnaParticleB& track, 
      double& prange, 
      ParticleId::ParticleEnum pid); 

  /// Get momentum from range from the end point to a point given (assuming it is a point along the trajectory)
  /// may be useful for checks of stopping tracks
  bool GetMomentumFromRangeLinear(const AnaParticleB& track, 
      const TVector3& point_pos,  
      double& prange, 
      ParticleId::ParticleEnum pid); 

  /// Get momentum from range from the end point to a point given (assuming it is a point along the trajectory)
  /// may be useful for checks of stopping tracks
  bool GetMomentumFromRangeLinear(const AnaTrueParticleB& track, 
      const TVector3& point_pos,  
      double& prange, 
      ParticleId::ParticleEnum pid); 

  /// Apply trim values
  void TrimPosition(TVector3& pos, const TVector3& dir, TrackEndDef endType);

  /// Build the propagation surface using the position and normal
  bool BuildPropagationSurface(const TVector3& pos, const TVector3& normal, RP::Surface& surf);
 
  /// Build the propagation surface using the position and direction at the position
  bool BuildPropagationDetSurface(const TVector3& pos, const TVector3& dir, RP::Surface& surf);
 
  /// Propagate a given state to a surface with a given position and direction 
  bool PropagateToSurface(const TVector3& pos, const TVector3& normal, RP::State& state, double& length, bool propagateCov = true);
  
  /// Propagate a given state to a surface with a given position, require to provide the direction info 
  /// (e.g. the one from the state or the one at the point we propagate to) so to make 
  /// the selection of normal (sub-detector specific rules etc)
  bool PropagateToDetectorSurface(const TVector3& pos, const TVector3& dir, RP::State& state, double& length, bool propagateCov = true);
  
  /// Propagate a given state to a particular sub-detector surface (from active volume) 
  /// considering a box w/o any rotation w.r.t. to ND280 coordinate system 
  /// one can specify the following surfaces: S1 - left , S2 - right , S3 - down , S4 - up, S5 - upstream, S6 - downstream
  /// with one exception for P0D since simplified RP volume is generally separated into three ones:
  /// "CECal_0/Active_0/S*"; 2. "Target_0/Active_0/S*"; 3. "USECal_0/Active_0/S*" 
  bool ExtrapolateToDetector(SubDetId::SubDetEnum det, const std::string& surfname, 
      RP::State& state, double& length, 
      double lmax = RP::EGeo::inf_l(), bool propagateCov = true); 
  
  /// Reverse sense and charge
  bool ReverseStateSenseAndCharge(RP::State& state);
  
  std::string get_volume_name(const TVector3& pos);

  
  /// Some converters
  bool TVector_to_EVector3( const TVector3&       lv, EVector&        v);
  bool TVector_to_EVector3( const TLorentzVector& lv, EVector&        v);
  bool EVector3_to_TVector( const EVector&        v,  TLorentzVector& lv);
  bool TMatrix_to_EMatrix(  const TMatrix&        TM, EMatrix&        M);
  bool TMatrix_to_EMatrix(  const TMatrix&        TM, int             nrows, int ncols, EMatrix& M);
  bool EMatrix_to_TMatrix(  const EMatrix&        M,  TMatrix&        TM);
  

  /// Convert AnaParticleB* to RECPACK state,  front pos,  dir and momentum used 
  bool AnaParticleMomB_to_RPState(     const AnaParticleMomB&     track,   
      State& state, 
      ParticleId::ParticleEnum pid, 
      double prange = -999.);

  /// Convert AnaTrueParticleB* to RECPACK state,  start pos, dir and momentum used 
  bool AnaTrueParticleB_to_RPState( const AnaTrueParticleB& track, 
      State& state, 
      double prange = -999.);

   
  /*!
   * Kinematics 
   */
  /// A general function that takes the momentum and changes it by propagating through two given points, 
  /// assuming the first given is a start one and the second is the end
  /// straight-line assumption is made and the one also needs to provide the PID hypothesis
  /// the correction is done using momentum-from-range functionality
  bool CorrectEnergyLossLinear(const TVector3& start, const TVector3& end, 
      double& momentum, 
      ParticleId::ParticleEnum pid,
      TrackDirDef dirType);
  
  /// Propagate a given track to a surface with a given position and direction 
  bool PropagateToSurface(const AnaParticleB*     track, const TVector3& pos, const TVector3& normal,
      RP::State& state, double& length, 
      ParticleId::ParticleEnum pid,
      bool prange = -999.);
  
  /// Propagate a given true track to a surface with a given position and direction 
  bool PropagateToSurface(const AnaTrueParticleB* track, const TVector3& pos, const TVector3& normal, 
      RP::State& state, double& length, 
      ParticleId::ParticleEnum pid,
      bool prange = -999.);
  
  std::string GetRecPackPIDName(ParticleId::ParticleEnum pid){
    switch (pid) {
      case ParticleId::kMuon:
        return "Muon";
        break;
      case ParticleId::kElectron:
        return "Electron";
        break;
      case ParticleId::kProton:
        return "Proton";
        break;
      case ParticleId::kPionPos:
        return "Pion";
        break;
      case ParticleId::kKaonPos:
        return "Kaon";
        break;
      default:
        return "Muon";
        break;
    }  
  }
  
  /// Get kinematics from a state
  bool GetPosition(const RP::State& state, TVector3& pos){
    
    RP::State state_tmp(state);
    
    RP::rep().convert(state_tmp, RP::pos_dir_curv);

    // Fail to convert
    if(state_tmp.name(RP::representation) != RP::pos_dir_curv)
      return treturn(false);
 
    EVector v = state_tmp.vector();
    pos = TVector3(v[0], v[1], v[2]);
    
    return treturn(true); 

  }
  bool GetDirection(const RP::State& state, TVector3& dir){
    
    RP::State state_tmp(state);
    
    RP::rep().convert(state_tmp, RP::pos_dir_curv);

    // Fail to convert
    if(state_tmp.name(RP::representation) != RP::pos_dir_curv)
      return treturn(false);
 
    EVector v = state_tmp.vector();
    dir = TVector3(v[3], v[4], v[5]);
    
    return treturn(true); 
   
  }
  
  bool GetQoverP(const RP::State& state, double& q_over_p){
    RP::State state_tmp(state);
    
    RP::rep().convert(state_tmp, RP::pos_dir_curv);

    // Fail to convert
    if(state_tmp.name(RP::representation) != RP::pos_dir_curv)
      return treturn(false);
 
    EVector v = state_tmp.vector();
    q_over_p  = v[6];
    
    return treturn(true); 

  }
  
  bool GetPRange(const RP::State& state, double& prange){
    if (state.hvmap().has_key("prange")){
      prange = state.hv("prange").vector()[0];
      return treturn(true);
    }
    return treturn(false);
  }
 
  private:
  
  AnaRecPackManager();
  AnaRecPackManager(const AnaRecPackManager& man);
  AnaRecPackManager& operator=(const AnaRecPackManager& man);
  
  /// Fill the internal tables.
  void FillInformation(void);
  
  void select_manager_temp(const std::string& m){_selected_man_temp = m;}
  
  void treturn();
  bool treturn(bool);
  
  /// The static pointer to the singleton instance.
  static AnaRecPackManager* fAnaRecPackManager;
  
  std::map<std::string, RecPackManager*> _man_map;  
  std::string _selected_man;
  std::string _selected_man_temp;
  
  /// The geometry manager that is currently described in this class.
  TGeoManager* fGeoManager;
  
  /// The current fitter
  std::string _fitter;
  
  /// RecPack verbosity
  int _verbosity[5];
  
  /// ---- Momentum By Range ---- Trim parameters
  double fTarget1TrimZ_start;
  double fTarget1TrimZ_end;
  double fTarget2TrimZ;
  double fFGD1TrimZ_start;
  double fFGD1TrimZ_end;
  double fFGD2TrimZ;

  double fP0DTrimZ;
  double fDsECalTrimZ;
  double fBrlECalTrimXY;
  double fP0DECalTrimXY; 
  
  /// DEBUG
  bool fDebug;
  
  /// Use prod 5-like "predefined" surfaces
  bool fProd5SurfaceMaking;
  
  
  /*!
   * Create a "linear" (large momentum) state given start and end points, PID and
   * prange if requested
   */
   bool CreateSLineState(const TVector3& start, const TVector3& end, 
       RP::State& state, 
       ParticleId::ParticleEnum pid, 
       double prange = -999.);
 
   /*!
    *  Create a state given pos, dir, charge/momentum, PID and prange if requested 
    */ 
   bool CreateState(const TVector3& pos, const TVector3& dir,
       double q,
       double p, 
       RP::State& state, 
       ParticleId::ParticleEnum pid,
       double prange = -999.);
   
   /*!
    *  Apply trim position given a state
    */
    bool GetTrimPosition(const RP::State& state1, 
        TVector3& pos, 
        AnaRecPackManager::TrackEndDef endType);
     
    /*!
    *  Apply trim position given a track
    */
    bool GetTrimPosition(const AnaParticleB& track, 
        TVector3& pos, 
        AnaRecPackManager::TrackEndDef endType); 
};


namespace ND{
  
  AnaRecPackManager& tman();
  AnaRecPackManager& tman(const std::string& m);
  RecPackManager& rpman();
  RecPackManager& rpman(const std::string& m);
  
};

#endif
