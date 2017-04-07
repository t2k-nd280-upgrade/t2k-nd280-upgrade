#include "AnaRecPackManager.hxx"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>
#include <recpack/KalmanFitter.h>
#include <recpack/RayTool.h>
#include <recpack/Ring.h>



//// The static member pointer to the singleton.
AnaRecPackManager* AnaRecPackManager::fAnaRecPackManager = NULL;


//*****************************************************************************
AnaRecPackManager& ND::tman(){
  //*****************************************************************************

  return AnaRecPackManager::Get();

}

//*****************************************************************************
AnaRecPackManager& ND::tman(const std::string& m){
  //*****************************************************************************

  return AnaRecPackManager::Get(m);

}

//*****************************************************************************
RecPackManager& ND::rpman(){
  //*****************************************************************************

  return AnaRecPackManager::Get().man();

}

//*****************************************************************************
RecPackManager& ND::rpman(const std::string& m){
  //*****************************************************************************

  return AnaRecPackManager::Get(m).man();

}



//*****************************************************************************
AnaRecPackManager::AnaRecPackManager() {
  //*****************************************************************************

  fGeoManager = NULL;

  // select the fitter
  _fitter = RP::kalman;    // the fitting algorithm

  // clear the vector of managers
  _man_map.clear();

  // a a manager
  add_manager("default");
  select_manager("default");
  select_manager_temp("default");

  // momentum by range trim parameters
  fTarget1TrimZ_start    = ND::params().GetParameterD("highlandRecPack.PRange.Target1TrimZ_start");
  fTarget1TrimZ_end      = ND::params().GetParameterD("highlandRecPack.PRange.Target1TrimZ_end");
  fTarget2TrimZ          = ND::params().GetParameterD("highlandRecPack.PRange.Target2TrimZ");
  fFGD1TrimZ_start    = ND::params().GetParameterD("highlandRecPack.PRange.FGD1TrimZ_start");
  fFGD1TrimZ_end      = ND::params().GetParameterD("highlandRecPack.PRange.FGD1TrimZ_end");
  fFGD2TrimZ          = ND::params().GetParameterD("highlandRecPack.PRange.FGD2TrimZ");

  fP0DTrimZ           = ND::params().GetParameterD("highlandRecPack.PRange.P0DTrimZ");
  fDsECalTrimZ        = ND::params().GetParameterD("highlandRecPack.PRange.DsECalTrimZ");
  fBrlECalTrimXY      = ND::params().GetParameterD("highlandRecPack.PRange.BrlECalTrimXY");
  fP0DECalTrimXY      = ND::params().GetParameterD("highlandRecPack.PRange.P0DECalTrimXY");

  fDebug = (bool)ND::params().GetParameterI("highlandRecPack.Debug");

  fProd5SurfaceMaking = (bool)ND::params().GetParameterI("highlandRecPack.Surfaces.UseProd5SurfaceMaking");



}

//*****************************************************************************
void AnaRecPackManager::InitializeManager(const std::string& m, const std::string& model) {
  //*****************************************************************************

  // temporarily select this manager in order to set properties
  select_manager_temp(m);

  // select the model
  man(m).model_svc().select_model(model);

  // select the Kalman Filter as fitting algorithm
  man(m).fitting_svc().select_fitter(_fitter);

  //select the default representation
  RP::rep().set_default_rep_name(RP::pos_dir_curv);

  // default fitting representation (should be selected before trying to retrieve a fitter)
  man(m).fitting_svc().set_fitting_representation(RP::slopes_curv);

  // set the maximum local chi2 allowed
  man(m).fitting_svc().retrieve_fitter<KalmanFitter>(RP::kalman,model).set_max_local_chi2ndf(EGeo::inf());

  // set the maximum number of outliers allowed
  man(m).fitting_svc().retrieve_fitter<KalmanFitter>(RP::kalman,model).set_number_allowed_outliers(10000);


  // initialize geometrical limits
  man(m).geometry_svc().set_zero_length(1e-3 * mm);
  man(m).geometry_svc().set_infinite_length(1e12 * mm);

  // enable multiple scattering by default
  man(m).model_svc().enable_noiser(model, RP::ms, true);

  // disable energy loss fluctuations by default
  man(m).model_svc().enable_noiser(model, RP::eloss, false);

  // disable electron energy loss fluctuations (bremsstrahlung) by default
  man(m).model_svc().enable_noiser(model, RP::electron_eloss, false);

  // disable electron energy loss correction (bremsstrahlung) by default
  man(m).model_svc().enable_correction(model, RP::brem_eloss, false);

  // enable energy loss correction by default
  man(m).model_svc().enable_correction(model, RP::eloss, true);

  // By default no preselected length sign is used when intersecting a surface
  man(m).model_svc().model(model).intersector().set_length_sign(0);
  man(m).model_svc().model(model).intersector().allow_zero_length();

  // The geometry is not initialized for this manager
  man(m).geometry_svc().set_status("ready",false);

  // Initialize the Setup for this manager
  InitializeManagerGeometry(m);

  // set the RecPack verbosity  for this manager (IT NEEDS THE SETUP)
  SetVerbosity(5,7,5,5,0);    

  // Set the model corresponding to this manager
  SetModel(model);

  // The default algorithm name 
  SetAlgName("oaRecPack");

}

//*****************************************************************************
void AnaRecPackManager::InitializeManagerGeometry(const std::string& m) {
  //*****************************************************************************

  select_manager_temp(m);

  // get the model corresponding to this manager
  std::string model = GetModel();

  // get the RecPack setup from the RecPackGeometry manager
  Setup& setup =  ND::gman().GetSetup();


  // remove the setup "main" if it already exists
  man(m).geometry_svc().remove_setup("main");

  // set the RecPack geometry to the manager. Call it "main"
  man(m).geometry_svc().add_setup("main", setup);

  // select the setup to be used by the geometry service
  man(m).geometry_svc().select_setup("main");

  // only surfaces with measurements are tried inside a volume (NEEDS THE SETUP)
  man(m).navigation_svc().navigator(model).set_unique_surface(true);

  // this is the maximum distance allowed when propagating to a surface 
  // and the propagation being ouside the final volume
  man(m).navigation_svc().navigator(model).set_max_prop_distance_outside_finalvolume(10.);

  // Allow the extrapolation to a infinite surface to be outside the parent volume
  man(m).navigation_svc().navigator(model).set_allow_extrap_outside_volume(true);

  //dont fill intersection map by default
  man(m).navigation_svc().navigator(model).set_fill_surface_intersection_list(false);

  // The geometry is initialized for this manager
  man(m).geometry_svc().set_status("ready",true);
}

//*****************************************************************************
AnaRecPackManager& AnaRecPackManager::Get(void) {
  //*****************************************************************************

  if (!fAnaRecPackManager) {
    fAnaRecPackManager = new AnaRecPackManager;
  }

  if (! fAnaRecPackManager->man(fAnaRecPackManager->selected_manager()).geometry_svc().status("ready"))
    fAnaRecPackManager->InitializeManagerGeometry(fAnaRecPackManager->selected_manager());

  return *fAnaRecPackManager;
}

//*****************************************************************************
AnaRecPackManager& AnaRecPackManager::Get(const std::string& m) {
  //*****************************************************************************

  if (!fAnaRecPackManager) {
    fAnaRecPackManager = new AnaRecPackManager;
  }

  fAnaRecPackManager->select_manager_temp(m);

  if (! fAnaRecPackManager->man(m).geometry_svc().status("ready"))
    fAnaRecPackManager->InitializeManagerGeometry(m);

  return *fAnaRecPackManager;
}

//*****************************************************************************
void AnaRecPackManager::ResetGeometry() {
  //*****************************************************************************

  std::map<std::string, RecPackManager*>::iterator im;
  for (im = _man_map.begin();im!=_man_map.end();im++)   
    im->second->geometry_svc().set_status("ready",false);

}
//*****************************************************************************
void AnaRecPackManager::add_manager(const std::string& m, std::string model){    
  //*****************************************************************************

  if (_man_map.find(m) != _man_map.end()){
    std::cout << "AnaRecPackManager::add_manager(). Trying to add a manager '" << m << "' that already exits. ERROR !!!!!" << std::endl; 
    return;
  }

  _man_map[m]= new RecPackManager();
  InitializeManager(m,model);
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//*****************************************************************************
void AnaRecPackManager::SetVerbosity(const std::string& verbosity) {
  //*****************************************************************************

  // fitting, navigation, model, matching, Ray tool
  int verb[5]={0,0,0,0,0};  // default levels
  if (verbosity != ""){
    for (unsigned int i=0;i<5;i++){
      if (verbosity.size()>i)    
        verb[i] = atoi(verbosity.substr(i,1).c_str()); 
    }
  }

  SetVerbosity(verb[0],verb[1],verb[2],verb[3],verb[4]);
}

//*****************************************************************************
void AnaRecPackManager::SetVerbosity(int verb0, int verb1, int verb2, int verb3, int verb4) {
  //*****************************************************************************

  _verbosity[0] = verb0;
  _verbosity[1] = verb1;
  _verbosity[2] = verb2;
  _verbosity[3] = verb3;
  _verbosity[4] = verb4;

  std::string info[8] =  {"MUTE","ERROR","WARNING","INFO","NORMAL","DETAILED","VERBOSE","VVERBOSE"};

  Messenger::Level level0 = Messenger::str(info[verb0]);
  Messenger::Level level1 = Messenger::str(info[verb1]);
  Messenger::Level level2 = Messenger::str(info[verb2]);
  Messenger::Level level3 = Messenger::str(info[verb3]);
  Messenger::Level level4 = Messenger::str(info[verb4]);


  const std::string& modelname = man().model_svc().model_name();

  man().fitting_svc().fitter(modelname).set_verbosity(level0);

  man().navigation_svc().set_verbosity(level1);
  man().navigation_svc().navigator(modelname).set_verbosity(level1);
  man().navigation_svc().inspector(RP::X0).set_verbosity(level1);
  man().navigation_svc().navigator(modelname).master_inspector().set_verbosity(level1);
  man().navigation_svc().inspector(RP::BField).set_verbosity(level1);  
  man().navigation_svc().inspector(RP::eloss).set_verbosity(level1);     
  man().navigation_svc().inspector(RP::Nel).set_verbosity(level1);     
  man().navigation_svc().inspector(RP::elossMap).set_verbosity(level1);     
  man().navigation_svc().inspector(RP::BFieldMap).set_verbosity(level1);     
  man().model_svc().model(modelname).intersector("plane").set_verbosity(level1);
  man().model_svc().model(modelname).intersector("numerical").set_verbosity(level1);


  man().model_svc().model(modelname).equation().set_verbosity(level2);
  man().model_svc().model(modelname).propagator().set_verbosity(level2);
  man().model_svc().model(modelname).noiser().set_verbosity(level2);
  man().model_svc().model(modelname).tool("correction/eloss").set_verbosity(level2);
  man().model_svc().model(modelname).tool("correction/brem_eloss").set_verbosity(level2);
  man().model_svc().model(modelname).tool("noiser/brem_eloss").set_verbosity(level2);
  man().model_svc().model(modelname).tool("noiser/ms").set_verbosity(level2);
  man().model_svc().model(modelname).tool("noiser/eloss").set_verbosity(level2);
  man().model_svc().model(modelname).tool("intersector/numerical").set_verbosity(level2);

  man().conversion_svc().projector().set_verbosity(level2);
  man().conversion_svc().representation(RP::slopes_curv).set_verbosity(level2);

  man().matching_svc().set_verbosity(level3);

  RayTool::msg().set_verbosity(level4);

}

//*****************************************************************************
void AnaRecPackManager::treturn(){
  //*****************************************************************************

  select_manager_temp(_selected_man);

  return;
}

//*****************************************************************************
bool AnaRecPackManager::treturn(bool ok){
  //*****************************************************************************

  select_manager_temp(_selected_man);

  return ok;
}


//*****************************************************************************
std::string AnaRecPackManager::get_volume_name(const TVector3& pos){
  //*****************************************************************************

  std::string vol_name="";

  EVector r;
  bool ok = TVector_to_EVector3(pos,r);
  if (!ok) return vol_name;

  const Volume* vol = NULL;
  ok = ND::gman().GetSetup().volume(r,vol);
  if (!ok){
    if (fDebug){
      std::cout << "AnaRecPackManager::get_volume_name(). Volume not found at position = " << r << std::endl;
    }    
    return vol_name;
  }
  else{
    // the current volume will be the closest closed volume
    vol_name = vol->name("setup_name");
  }

  if (fDebug){
    std::cout << "AnaRecPackManager::get_volume_name(pos). name = " << vol_name << ", r = " << RP::print(r) << std::endl;  
  }

  // TODO: Temporary solution to avoid TPC hits lying in the pad plane (outside the Drift volume)
  if (vol_name.find("TPC") != std::string::npos && vol_name.find("Drift") == std::string::npos && pos.X()!=100){
    TVector3 pos2(100*mm, pos.Y(), pos.Z());
    vol_name = get_volume_name(pos2);
  }

  return vol_name;
}

//Utility converters
//*****************************************************************************
bool AnaRecPackManager::TVector_to_EVector3(const TVector3& lv, EVector& v){
  //*****************************************************************************

  v = EVector(3,0);

  v[0] = lv.X();
  v[1] = lv.Y();
  v[2] = lv.Z();

  return true;
}

//*****************************************************************************
bool AnaRecPackManager::TVector_to_EVector3(const TLorentzVector& lv, EVector& v){
  //*****************************************************************************

  v = EVector(3,0);

  v[0] = lv.X();
  v[1] = lv.Y();
  v[2] = lv.Z();

  return true;
}

//*****************************************************************************
bool AnaRecPackManager::EVector3_to_TVector(const EVector& v, TLorentzVector& lv){
  //*****************************************************************************

  if (v.num_row() != 3) return false;

  lv.SetX(v[0]);
  lv.SetY(v[1]);
  lv.SetZ(v[2]);

  return true;
}

//*****************************************************************************
bool AnaRecPackManager::TMatrix_to_EMatrix(const TMatrix& TM, EMatrix& M){
  //*****************************************************************************

  M = EMatrix(TM.GetNrows(),TM.GetNcols(),0);

  // retrieve the covariance matrix
  for(int ii = 0;ii < TM.GetNrows();ii++)
    for(int jj = 0;jj < TM.GetNcols();jj++)
      M[ii][jj] = TM[ii][jj];

  return true;
}

//*****************************************************************************
bool AnaRecPackManager::TMatrix_to_EMatrix(const TMatrix& TM, int nrows, int ncols, EMatrix& M){
  //*****************************************************************************

  M = EMatrix(nrows,ncols,0);

  // retrieve the covariance matrix
  for(int ii = 0;ii < nrows;ii++)
    for(int jj = 0;jj < ncols;jj++)
      M[ii][jj] = TM[ii][jj];

  return true;
}

//*****************************************************************************
bool AnaRecPackManager::EMatrix_to_TMatrix(const EMatrix& M, TMatrix& TM){
  //*****************************************************************************

  TM = TMatrix(M.num_row(), M.num_col());

  // retrieve the covariance matrix
  for(int ii = 0;ii < M.num_row();ii++)
    for(int jj = 0;jj < M.num_col();jj++)
      TM[ii][jj] = M[ii][jj];

  return true;
}


//*****************************************************************************
bool AnaRecPackManager::AnaParticleMomB_to_RPState(const AnaParticleMomB& track, 
    State& state, 
    ParticleId::ParticleEnum pid, 
    double prange){
  //*****************************************************************************

  state.clear();

  // Extract position, direction and momentum
  TVector3 pos(track.PositionStart[0],  track.PositionStart[1],  track.PositionStart[2]);
  TVector3 dir(track.DirectionStart[0], track.DirectionStart[1], track.DirectionStart[2]);

  double p      = track.Momentum;
  double charge = track.Charge;


  return treturn(CreateState(pos, dir, charge, p, state, pid, prange));
}

//*****************************************************************************
bool AnaRecPackManager::AnaTrueParticleB_to_RPState(const AnaTrueParticleB& track, 
    State& state, 
    double prange){
  //*****************************************************************************
  
  state.clear();

  // Extract position, direction and momentum
  TVector3 pos(track.Position[0],  track.Position[1],  track.Position[2]);
  TVector3 dir(track.Direction[0], track.Direction[1], track.Direction[2]);

  double p      = track.Momentum;
  double charge = track.Charge;


  return treturn(CreateState(pos, dir, charge, p, state, ParticleId::GetParticle(track.PDG), prange));
}


//*****************************************************************************
bool AnaRecPackManager::BuildPropagationDetSurface(const TVector3& pos, const TVector3& dir, RP::Surface& surf){
  //*****************************************************************************
  if(fDebug)
    std::cout << "AnaRecPackManager::BuildPropagationSurface()" << std::endl;

  //direction
  EVector u(3, 0);
  ND::tman().TVector_to_EVector3(dir, u);

  //having a position and direction select the normal
  double u_max = 0;
  double u_min = 1;
  int i_max=2;
  int i_min=0;
  for (int i=0;i<3;i++){
    if (fabs(u[i]) >= u_max){
      u_max = fabs(u[i]);
      i_max = i;
    }    
    if (fabs(u[i]) <= u_min ){
      u_min = fabs(u[i]);
      i_min = i;
    }    
  }


  if (fDebug) {
    std::cout <<  "    --> i_max, i_min = " << i_max << ", " << i_min << " u[i_max], u_max = " << u[i_max] << ", " << u_max << std::endl;
  }    

  EVector axis(3,0);

  //for ECal, P0D for now keep "pre-defined" surfaces with fixed normals based on detector geometry

  std::string volname = ND::tman().get_volume_name(pos);
  //make sure that measurement has an accosiated volume
  if (volname==""){
    if (fDebug)
      std::cout<<"AnaRecPackManager::BuildPropagationSurface(): not volume for position "<<std::endl;
    return treturn( false);
  }

  //FGD. Take as normal the one with maximum direction component if it is above 0.3
  if (volname.find("FGD") != std::string::npos){    
    if (u_max>0.3)
      axis[i_max] = u[i_max]/u_max;
    else
      axis[2] = 1;

    if (fProd5SurfaceMaking){
      axis[0] = 0;
      axis[1] = 0;
      axis[2] = 1;
    }
  }
  //FGD. Take as normal the one with maximum direction component if it is above 0.3
  if (volname.find("Target") != std::string::npos){    
    if (u_max>0.3)
      axis[i_max] = u[i_max]/u_max;
    else
      axis[2] = 1;

    if (fProd5SurfaceMaking){
      axis[0] = 0;
      axis[1] = 0;
      axis[2] = 1;
    }
  }

  //TPC. Take as normal the one with maximum direction component in the measurement (a TPC state) if it is above 0.3
  else if (volname.find("TPC") != std::string::npos){
    if (u_max>0.3) 
      axis[i_max] = u[i_max]/u_max;
    else
      axis[2] = 1;

    if (fProd5SurfaceMaking){
      axis[0] = 0;
      axis[1] = 0;
      axis[2] = 1;
    }

  }	
  //P0D. normal along z
  else if (volname.find("P0D") != std::string::npos){    
    axis[2] = 1;      
  }	
  //BrlECal
  else if (volname.find("BrlECal") != std::string::npos){
    //Top and Bottom. normal along y
    if(volname.find("Top") != std::string::npos || volname.find("Bottom") != std::string::npos){      
      axis[1] = 1;
    }	
    //Left and Right. normal along x
    else{
      axis[0] = 1;
    }
  }		
  //DsECal.  normal along z
  else if (volname.find("DsECal") != std::string::npos){
    axis[2] = 1;
  }	
  //P0DECal
  else if (volname.find("P0DECal") != std::string::npos){
    //Top and Bottom. normal along y
    if(volname.find("Top") != std::string::npos || volname.find("Bottom") != std::string::npos){
      axis[1] = 1;
    }
    //Left and Right. normal along x
    else{
      axis[0] = 1;
    }    
  }
  //well, "general" case
  else{    
    //use z normal by default
    axis[2] = 1;
  }

  TLorentzVector lv;
  ND::tman().EVector3_to_TVector(axis, lv);

  // build a surface with a selected normal  
  return ND::tman().BuildPropagationSurface(pos,lv.Vect(), surf);

}



//*****************************************************************************
bool AnaRecPackManager::BuildPropagationSurface(const TVector3& pos, const TVector3& normal, RP::Surface& surf){
  //*****************************************************************************
  if(fDebug)
    std::cout << "AnaRecPackManager::BuildPropagationSurface()" << std::endl;

  // convert TVector3 to EVector
  EVector pos2, normal2;
  bool ok = ND::tman().TVector_to_EVector3(pos, pos2);
  if (!ok) return treturn( false );
  ok = ND::tman().TVector_to_EVector3(normal, normal2);
  if (!ok) return treturn( false );

  if(fDebug){
    std::cout << "  pos:    " << RP::print(pos2) << std::endl;
    std::cout << "  normal: " << RP::print(normal2) << std::endl;
  }

  // build a surface
  surf = Ring(pos2,normal2, 1000*cm);

  std::string volname = ND::tman().get_volume_name(pos);

  if (volname == ""){
    std::cout << "ERROR: position " << RP::print(pos2) << " does not correspond to any volume in Setup !!!! " << std::endl;
    std::cout << "       A protection should be added in the algorithm calling tman().PropagateToSurface(..)" << std::endl;    
    return treturn( false );
  }

  if(fDebug){
    std::cout << "  vol:    " << volname << std::endl;
  }

  // set the volume name
  surf.set_name(RP::setup_name,volname);

  return treturn( true );



}

//*****************************************************************************
bool AnaRecPackManager::PropagateToSurface(const TVector3& pos, const TVector3& normal, 
    RP::State& state, double& length, 
    bool propagateCov){
  //*****************************************************************************
  if(fDebug)
    std::cout << "AnaRecPackManager::PropagateToSurface(State)" << std::endl;

  // Build the surface
  Surface surf;
  bool ok = BuildPropagationSurface(pos,normal,surf);
  if (!ok) return treturn( false );

  // add the surface to the setup
  const std::string surfname = "temp";  
  const std::string volname = surf.name(RP::setup_name);
  ok = ND::rpman().geometry_svc().setup().add_surface(volname,surfname, &surf);
  if (!ok) return treturn( false );

  // propagate the track to that surface
  // check whether one needs to propagate the covariance matrix
  if (propagateCov) 
    ok = ND::rpman().navigation_svc().propagate(surf, state, length);
  else
    ok = ND::rpman().navigation_svc().propagate_vector(surf, state, length); //covariance matrix is not propagated

  if(fDebug){
    std::cout << "  length: " << length << std::endl;
  }

  // remove the surface from the setup
  ND::rpman().geometry_svc().setup().remove_surface(surfname);

  return treturn( ok );


} 

//*****************************************************************************
bool AnaRecPackManager::PropagateToDetectorSurface(const TVector3& pos, const TVector3& dir, 
    RP::State& state, double& length, 
    bool propagateCov){
  //*****************************************************************************

  if(fDebug)
    std::cout << "AnaRecPackManager::PropagateToSurface(State)" << std::endl;

  // Build the surface
  Surface surf;
  bool ok = BuildPropagationDetSurface(pos,dir,surf);
  if (!ok) return treturn( false );

  // add the surface to the setup
  const std::string surfname = "temp";  
  const std::string volname = surf.name(RP::setup_name);
  ok = ND::rpman().geometry_svc().setup().add_surface(volname,surfname, &surf);
  if (!ok) return treturn( false );

  // propagate the track to that surface
  if (propagateCov)
    ok = ND::rpman().navigation_svc().propagate(surf, state, length);
  else
    ok = ND::rpman().navigation_svc().propagate_vector(surf, state, length); //covariance matrix is not propagated

  if(fDebug){
    std::cout << "  length: " << length << std::endl;
  }

  // remove the surface from the setup
  ND::rpman().geometry_svc().setup().remove_surface(surfname);

  return treturn( ok );

} 

//*****************************************************************************
bool AnaRecPackManager::ExtrapolateToDetector(SubDetId::SubDetEnum det, 
    const std::string& surfname, 
    RP::State& state, 
    double& length, 
    double lmax,
    bool propagateCov){ 
  //*****************************************************************************
  if(fDebug)
    std::cout << "AnaRecPackManager::ExtrapolateToDetector" << std::endl;

  //get the volume name
  std::string volname="";

  //two volumes needed to treat correctly top/bottom volumes for ECal, P0DECal and SMRD
  std::string volname1="";

  //TPCs
  if      (det==SubDetId::kForwTPC1)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/ForwTPC1_0/GasGap_0/Drift_0";
  else if (det==SubDetId::kForwTPC2)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/ForwTPC2_0/GasGap_0/Drift_0";
  else if (det==SubDetId::kForwTPC3)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/ForwTPC3_0/GasGap_0/Drift_0";
  else if (det==SubDetId::kTPCUp2)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/TPCUp1_0/GasGap_0/Drift_0";
  else if (det==SubDetId::kTPCUp1)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/TPCUp2_0/GasGap_0/Drift_0";
  else if (det==SubDetId::kTPCDown2)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/TPCDown1_0/GasGap_0/Drift_0";
  else if (det==SubDetId::kTPCDown1)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/TPCDown2_0/GasGap_0/Drift_0";

  //FGDs
  else if (det==SubDetId::kFGD1)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/FGD1_0";
  else if (det==SubDetId::kFGD2)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/FGD2_0";

  else if (det==SubDetId::kTarget1)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/Target1_0";
  else if (det==SubDetId::kTarget2)   volname = "/t2k_1/OA_0/Magnet_0/Basket_0/Target2_0";

  //P0D 
  //(needs some more care here because P0D is separated into several volumes along z)
  //to keep things "general" a user should specify surface name with a particular detector (for surfname):

  //1. "CECal_0/Active_0/S*"; 2. "Target_0/Active_0/S*"; 3. "USECal_0/Active_0/S*" 

  //here just keep the standart part for P0D volume names 
  else if (det==SubDetId::kP0D)    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/P0D_0";


  //ECALs
  else if (det==SubDetId::kDsECal) volname = "/t2k_1/OA_0/Magnet_0/Basket_0/DsECal_0/Active_0";
/*
  else if (det==SubDetId::kTopTECAL){

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_0/Top_0/Active_0";

    //this one to account for two volumes for BrlECal top
    volname1 = "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_0/Top_0/Active_0";	 
  }

  else if (det==SubDetId::kBottomTECAL){	

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_2/Bottom_0/Active_0";

    //this one to account for two volumes for BrlECal top
    volname1 = "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_2/Bottom_0/Active_0";	 
  }

  else if (det==SubDetId::kLeftTECAL)
    volname = "/t2k_1/OA_0/Magnet_0/LeftClam_0/BrlECal_1/Side_0/Active_0";

  else if (det==SubDetId::kRightTECAL)
    volname = "/t2k_1/OA_0/Magnet_0/RightClam_0/BrlECal_1/Side_0/Active_0";	


  //P0DECALs
  else if (det==SubDetId::kTopPECAL){

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_0/Top_0/Active_0";

    //this one to account for two volumes for BrlECal top
    volname1 = "/t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_0/Top_0/Active_0";	 
  }

  else if (det==SubDetId::kBottomPECAL){	

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_2/Bottom_0/Active_0";

    //this one to account for two volumes for BrlECal top
    volname1 = "/t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_2/Bottom_0/Active_0";	 
  }

  else if (det==SubDetId::kLeftPECAL)
    volname = "/t2k_1/OA_0/Magnet_0/LeftClam_0/P0DECal_1/Side_0/Active_0";

  else if (det==SubDetId::kRightPECAL)
    volname = "/t2k_1/OA_0/Magnet_0/RightClam_0/P0DECal_1/Side_0/Active_0";	
*/
  else if (det==SubDetId::kToFTopUp)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/TopUp_0";

  else if (det==SubDetId::kToFBotUp)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/BotUp_0";
 
  else if (det==SubDetId::kToFBotDown)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/BotDown_0";

  else if (det==SubDetId::kToFTopDown)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/TopDown_0";

  else if (det==SubDetId::kToFRightUp)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/RightUp_0";
  
  else if (det==SubDetId::kToFLeftUp)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/LeftUp_0";
  
  else if (det==SubDetId::kToFRightDown)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/RightDown_0";
  
  else if (det==SubDetId::kToFLeftDown)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/LeftDown_0";

  else if (det==SubDetId::kToFFrontDown)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/FrontDown_0";
  
  else if (det==SubDetId::kToFBackDown)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/BackDown_0";

  else if (det==SubDetId::kToFFrontUp)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/FrontUp_0";
  
  else if (det==SubDetId::kToFBackUp)

    //this one is used to build an extrapolated surface of an infinite side
    volname = "/t2k_1/OA_0/Magnet_0/Basket_0/BackUp_0";

  //need some care in case of two volumes used and S2 surface 

  if(volname1.size()!=0 && surfname=="S2"){
    std::string volnametmp = volname;
    volname = volname1;
    volname1 = volnametmp;
  }


  std::string fullsurfname = volname+"/"+surfname;	



  if(fDebug)
    std::cout << "AnaRecPackManager::ExtrapolateToDetector. full surfname = " << fullsurfname << std::endl;


  if (!ND::gman().GetSetup().has_surface(fullsurfname)){
    if(fDebug)
      std::cout << "AnaRecPackManager::ExtrapolateToDetector. Surface does not exists: " << fullsurfname << std::endl;    
    return treturn(false);
  }

  // Get the surface
  const Surface& surf = ND::gman().GetSetup().surface(fullsurfname);

  // build a second surface with the same position and normal, but with infinite size
  Ring surf2(surf.position(),surf.normal(),EGeo::inf_l());
  surf2.set_name(RP::setup_name,volname);

  const std::string temp_surfname = "temp_CheckDetectorExtrap";  
  bool ok = ND::rpman().geometry_svc().setup().add_surface(volname,temp_surfname, &surf2);
  if (!ok) return treturn(false);

  // propagate the track to that surface
  if (propagateCov)
    ok = ND::rpman().navigation_svc().propagate(surf2, state, length);
  else
    ok = ND::rpman().navigation_svc().propagate_vector(surf2, state, length); //covariance matrix is not propagated



  // remove the surface from the setup
  ND::rpman().geometry_svc().setup().remove_surface(temp_surfname);

  if (!ok){
    if(fDebug)
      std::cout << "AnaRecPackManager::ExtrapolateToDetector. Fail in extrapolation !! " << std::endl;    
    return treturn(false);
  }


  // get the extrapolated position
  EVector pos = box(state.vector(),0,3);

  // check if the extrapolated position is inside the finite surface

  //if two surfaces are used: SMRD/ECal/P0DECals top/bottom
  if(volname1.size()!=0){

    if(fDebug)
      std::cout << "AnaRecPackManager::ExtrapolateToDetector. (SMRD,ECal,P0DECal Top/Bottom case) consider two surfaces "<< std::endl;

    bool ok1 = false;
    bool ok2 = false;        

    //check first surface for Top/Bottom SMRD, ECal, P0DECal
    ok1 = surf.is_inside(pos);

    if(ok1)
      if(fDebug)
        std::cout << "	AnaRecPackManager::ExtrapolateToDetector. extrap. position is in first surface "<<fullsurfname<< std::endl;


    std::string fullsurfname1 = volname1+"/"+surfname;

    if(fDebug)
      std::cout << "AnaRecPackManager::ExtrapolateToDetector. (SMRD,ECal,P0DECal Top/Bottom case) second surface  full surfname = " << fullsurfname1 << std::endl;


    if (!ND::gman().GetSetup().has_surface(fullsurfname1)){
      if(fDebug)
        std::cout << "AnaRecPackManager::ExtrapolateToDetector. Second surface does not exists: " << fullsurfname1 << std::endl;    
      ok2 = false;
    }
    else{
      // Get the surface
      const Surface& surf1 = ND::gman().GetSetup().surface(fullsurfname1);
      ok2 = surf1.is_inside(pos); 
    }

    if(ok2)
      if(fDebug)
        std::cout << "	AnaRecPackManager::ExtrapolateToDetector. extrap. position is in second surface "<<fullsurfname1<< std::endl;

    ok = ok1 || ok2; 

  }
  else 
    ok = surf.is_inside(pos);

  if(fDebug)
    std::cout << "AnaRecPackManager::ExtrapolateToDetector. pos = " << RP::print(pos) << ", ok = " << ok << ", l = " << length << std::endl;


  if (fabs(length)>lmax){
    ok = false;
    if(fDebug)
      std::cout << "AnaRecPackManager::ExtrapolateToDetector. Length too large. Maximum is " << lmax << std::endl;
  }

  return treturn(ok);  


}

//*****************************************************************************
bool AnaRecPackManager::GetMomentumFromRangeLinear(const AnaParticleB& track,  
    double& prange, 
    ParticleId::ParticleEnum pid){
  //*****************************************************************************
  
  TVector3 pos_start; // for prange -- end of the track
  TVector3 pos_end; // for prange -- start of the track
  
  if (!GetTrimPosition(track, pos_start, kEnd))
    pos_start = TVector3(track.PositionEnd[0], track.PositionEnd[1], track.PositionEnd[2]); 
 
  if (!GetTrimPosition(track, pos_end, kStart))
    pos_end = TVector3(track.PositionStart[0], track.PositionStart[1], track.PositionStart[2]); 
  
  double prange_tmp = 0.;
  
  if (!CorrectEnergyLossLinear(pos_start, 
        pos_end, 
        prange_tmp, 
        pid, 
        kBwd) // go in bwd direction
     )
    return treturn(false);
  
  prange = prange_tmp;   

  if(fDebug){
    std::cout << "AnaRecPackManager::GetMomentumFromRangeLinear() -- prange is  \t"  << prange          << " MeV " << std::endl;
    track.Print();
  }

  return treturn(true);
}

//*****************************************************************************
bool AnaRecPackManager::GetMomentumFromRangeLinear(const AnaParticleB& track, const TVector3& point_pos, 
    double& prange, 
    ParticleId::ParticleEnum pid){
  //*****************************************************************************
  
  TVector3 pos_start; // for prange -- end of the track
  
  if (!GetTrimPosition(track, pos_start, kEnd))
    pos_start = TVector3(track.PositionEnd[0], track.PositionEnd[1], track.PositionEnd[2]); 
 
  double prange_tmp = 0.;
  
  if (!CorrectEnergyLossLinear(pos_start, 
        point_pos, // up to a given point
        prange_tmp, 
        pid, 
        kBwd) // go in bwd direction
     )
    return treturn(false);
  
  prange = prange_tmp;   

  if(fDebug){
    std::cout << "AnaRecPackManager::GetMomentumFromRangeLinear(point) -- prange is  \t"  << prange << " MeV " << std::endl;
  }

  return treturn(true);
}




//*****************************************************************************
bool AnaRecPackManager::GetMomentumFromRangeLinear(const AnaTrueParticleB& track, const TVector3& point_pos, 
    double& prange, 
    ParticleId::ParticleEnum pid){
  //*****************************************************************************
  
  TVector3 pos_start; // for prange -- end of the track
  
  //if (!GetTrimPosition(track, pos_start, kEnd))
    pos_start = TVector3(track.Position[0], track.Position[1], track.Position[2]); 
 
  double prange_tmp = 0.;
  
  if (!CorrectEnergyLossLinear(pos_start, 
        point_pos, // up to a given point
        prange_tmp, 
        pid, 
        kBwd) // go in bwd direction
     )
    return treturn(false);
  
  prange = prange_tmp;   

  if(fDebug){
    std::cout << "AnaRecPackManager::GetMomentumFromRangeLinear(point) -- prange is  \t"  << prange << " MeV " << std::endl;
  }

  return treturn(true);
}


//*****************************************************************************
bool AnaRecPackManager::CorrectEnergyLossLinear(const TVector3& start, const TVector3& end, 
    double& momentum, 
    ParticleId::ParticleEnum pid, 
    TrackDirDef dirType){
  //*****************************************************************************

  if(fDebug){
    std::cout << "AnaRecPackManager::CorrectEnergyLossLinear() -- begin  " << momentum << "  MeV "<<std::endl;
  }

  // Create the state
  RP::State state;
  if (!CreateSLineState(start, end, state, pid, momentum)) return treturn(false);

  // If requested to go bwd,  reverse the state
  if (dirType == kBwd)
    ReverseStateSenseAndCharge(state);
  
  double sign = (dirType == kFwd) ? 1. : -1.; 

  // It should be enough to propagate to a given length
  if (!ND::rpman().navigation_svc().propagate_vector(sign * (start-end).Mag(), state)){
    std::cout << "AnaRecPackManager::CorrectEnergyLossLinear() -- failed to propagate " << std::endl;
    return treturn(false);
  }
  if (!GetPRange(state, momentum))
    return treturn(false); 

  
  if(fDebug)
    std::cout << "AnaRecPackManager::CorrectEnergyLossLinear() -- end " << momentum << " MeV "<<std::endl;
  
  return treturn(true); 

} 


//*****************************************************************************
void AnaRecPackManager::TrimPosition(TVector3& pos, 
    const TVector3& dir, 
    TrackEndDef endType){
  //*****************************************************************************
  TVector3 pos_tmp(pos);
  std::string volname = ND::tman().get_volume_name(pos);

  if (volname == "")
    return treturn();


  double trim; 
  double sign = (endType == kStart) ? 1. : -1.; 
  
  if (volname.find("Target1_") != std::string::npos){

    trim = (endType == kStart) ? fTarget1TrimZ_start : fTarget1TrimZ_end; 
    
    sign *= (dir.Z() > 0) ? -1. : 1.;
    
    pos_tmp.SetZ(pos.Z() + sign * trim);

  }
  else if (volname.find("Target2_") != std::string::npos){
    
    trim = fTarget2TrimZ; 
    
    sign *= (dir.Z() > 0) ? -1. : 1.;
    
    pos_tmp.SetZ(pos.Z() + sign * trim);

  }
  if (volname.find("FGD1_") != std::string::npos){

    trim = (endType == kStart) ? fFGD1TrimZ_start : fFGD1TrimZ_end; 
    
    sign *= (dir.Z() > 0) ? -1. : 1.;
    
    pos_tmp.SetZ(pos.Z() + sign * trim);

  }
  else if (volname.find("FGD2_") != std::string::npos){
    
    trim = fFGD2TrimZ; 
    
    sign *= (dir.Z() > 0) ? -1. : 1.;
    
    pos_tmp.SetZ(pos.Z() + sign * trim);

  }

  else if (volname.find("P0D_") != std::string::npos){
    
    // Should probably also consider three different volumes in P0D --> TODO
    trim = fP0DTrimZ; 
    
    sign *= (dir.Z() > 0) ? -1. : 1.;
    
    pos_tmp.SetZ(pos.Z() + sign * trim);
 
  }
  else if (volname.find("DsECal_") != std::string::npos){
   
    trim = fDsECalTrimZ; 
    
    sign *= (dir.Z() > 0) ? -1. : 1.;
    
    pos_tmp.SetZ(pos.Z() + sign * trim);
 
  }
  
  else if (volname.find("BrlECal_") != std::string::npos){
    //Top and Bottom
    if (volname.find("Top") != std::string::npos || 
        volname.find("Bottom") != std::string::npos){  
    
      trim = fBrlECalTrimXY; 
    
      sign *= (dir.Y() > 0) ? -1. : 1.;
    
      pos_tmp.SetY(pos.Y() + sign * trim);
    }

    //Left and Right
    else if (volname.find("Left") != std::string::npos || 
        volname.find("Right") != std::string::npos){  
      
      trim = fBrlECalTrimXY; 
    
      sign *= (dir.X() > 0) ? -1. : 1.;
    
      pos_tmp.SetX(pos.X() + sign * trim);

    } 
  } 

  else if (volname.find("P0DECal_") != std::string::npos){ 
    //Top and Bottom
    if (volname.find("Top") != std::string::npos || 
        volname.find("Bottom") != std::string::npos){
      
      trim = fP0DECalTrimXY; 
    
      sign *= (dir.Y() > 0) ? -1. : 1.;
    
      pos_tmp.SetY(pos.Y() + sign * trim);
    
    }

    //Left and Right
    else if (volname.find("Left") != std::string::npos || 
        volname.find("Right") != std::string::npos){
      
      trim = fP0DECalTrimXY; 
    
      sign *= (dir.X() > 0) ? -1. : 1.;
    
      pos_tmp.SetX(pos.X() + sign * trim);
    }

  }
   
  // Don`t change the volume if a point is outside the world
  volname = ND::tman().get_volume_name(pos_tmp);

  if (volname == "") 
    return treturn();

  // Copy the vector
  pos = pos_tmp;
  
  return treturn();

}



//*****************************************************************************
bool AnaRecPackManager::ReverseStateSenseAndCharge(RP::State& state){
  //*****************************************************************************

  // Try convert to rep with dir and pos

  std::string rep_ini  = state.name(RP::representation);

  // Do the reverse in pos_dir_curv rep
  RP::rep().convert(state, RP::pos_dir_curv);

  // Fail to convert
  if (state.name(RP::representation) != RP::pos_dir_curv)
    return treturn( false );

  EVector v = state.vector();
  EMatrix C = state.matrix();

  // Reverse the direction vector 
  v[3] *= -1.;
  v[4] *= -1.;
  v[5] *= -1.;

  // Reverse the charge
  v[6] *= -1.;

  // Set the new HV
  state.set_hv(HyperVector(v,C));

  // Convert back
  RP::rep().convert(state, rep_ini);

  // If fail to convert
  if (state.name(RP::representation) != rep_ini)
    return treturn( false );
    
  return treturn(true); 

}

//*****************************************************************************
bool AnaRecPackManager::CreateSLineState(const TVector3& start, const TVector3& end, 
    RP::State& state,  
    ParticleId::ParticleEnum pid, 
    double prange){
  //*****************************************************************************

  // Clear the state
  state.clear();

  // Connect points with a straight line and estimate the direction
  double deno = (end - start).Mag();
  if(deno==0){
    if(fDebug)
      std::cout<<"AnaRecPackManager::CreateSLineState() -- zero length"<<std::endl;
    return false;
  }

  TVector3 dir(end - start);
  dir *= 1. / deno;


  // Build a state for start pos and calculated direction
  RP::EVector v1(7,0);
  RP::EMatrix C1(7,7,0);//zero covariance

  v1[0] = start.X();
  v1[1] = start.Y();
  v1[2] = start.Z();

  v1[3] = dir.X();
  v1[4] = dir.Y();
  v1[5] = dir.Z();   

  // Straight-line approximation (independent on model)
  // to be independent of model just set very big momentum

  v1[6] = 1. / LINMOM;   

  state.set_hv(HyperVector(v1,C1,RP::pos_dir_curv));
  state.set_name(RP::representation,RP::pos_dir_curv);

  // Make sure to set pid name correctly
  state.set_name(RP::PID, GetRecPackPIDName(pid));

  // Set the momentum for momentum from range propagation
  if (prange >= 0)
    state.set_hv("prange",HyperVector(prange, 0.)); // zero cov   


  return true;
}

//*****************************************************************************
bool AnaRecPackManager::CreateState(const TVector3& pos, const TVector3& dir,
    double q, double p, 
    RP::State& state,  
    ParticleId::ParticleEnum pid, double prange){
  //*****************************************************************************

  // Clear the state
  state.clear();

  if (p == 0.){
    if(fDebug)
      std::cout << " AnaRecPackManager::CreateState() -- zero momentum " << std::endl;
    return treturn(false);
  }

  if ( q == 0){
    if(fDebug)
      std::cout << " AnaRecPackManager::CreateState() -- zero charge " << std::endl;
    return treturn(false);
  }

  double deno = dir.Mag();

  if ( deno == 0.){
    if(fDebug)
      std::cout << " AnaRecPackManager::CreateState() -- zero direction " << std::endl;
    return treturn(false);
  }

  RP::EVector v1(7,0);
  RP::EMatrix C1(7,7,0); //zero covariance

  v1[0] = pos.X();
  v1[1] = pos.Y();
  v1[2] = pos.Z();

  v1[3] = dir.X() / deno;
  v1[4] = dir.Y() / deno;
  v1[5] = dir.Z() / deno;   

  v1[6] = q / p;   

  state.set_hv(HyperVector(v1, C1, RP::pos_dir_curv));
  state.set_name(RP::representation, RP::pos_dir_curv);

  // Make sure to set pid name correctly
  state.set_name(RP::PID, GetRecPackPIDName(pid));

  // Set the momentum for momentum from range propagation
  if (prange >= 0)
    state.set_hv("prange",HyperVector(prange, 0.)); // zero cov   

  return treturn(true);
}

//*****************************************************************************
bool  AnaRecPackManager::GetTrimPosition(const AnaParticleB& track, 
    TVector3& pos, 
    AnaRecPackManager::TrackEndDef endType){
  //*****************************************************************************
  
  RP::State state;
  bool ok;
  if (endType == kStart){
    ok = CreateState(
        TVector3(track.PositionStart[0],  track.PositionStart[1],   track.PositionStart[2]), 
        TVector3(track.DirectionStart[0], track.DirectionStart[1],  track.DirectionStart[2]),
        1., 
        LINMOM, 
        state, 
        ParticleId::kMuon
      );
  }
  else {
    ok = CreateState(
        TVector3(track.PositionEnd[0],  track.PositionEnd[1],   track.PositionEnd[2]), 
        TVector3(track.DirectionEnd[0], track.DirectionEnd[1],  track.DirectionEnd[2]),
        1.,
        LINMOM, 
        state, 
        ParticleId::kMuon
      );
  }
  
  if (!ok)
    treturn(false);
  
  return GetTrimPosition(state, pos, endType); 

}

//*****************************************************************************
bool  AnaRecPackManager::GetTrimPosition(const RP::State& state, 
    TVector3& pos, 
    AnaRecPackManager::TrackEndDef endType){
  //*****************************************************************************

  RP::State state_tmp(state);
  
  // Position and direction of states
  TVector3  state_pos;
  TVector3  state_dir;
  
  bool ok = GetPosition(state_tmp, state_pos);
  
  ok = GetDirection(state_tmp, state_dir);

  if (!ok)
    return treturn(false);


  TrimPosition(state_pos, state_dir, endType); 
 
  double length;

  ok = PropagateToDetectorSurface(state_pos, state_dir, state_tmp, length, false);
  
  if (!ok)
    return treturn(false);
   
  return treturn(GetPosition(state_tmp, pos));

   
  
}
