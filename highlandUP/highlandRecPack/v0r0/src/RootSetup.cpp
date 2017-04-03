#include "RootSetup.h"
#include <TList.h>
#include <Units.hxx>



//*****************************************************************************
void RootSetup::BuildSetup(){
//*****************************************************************************

  _nodeIDs.clear();
  _surfaces_nodeIDs.clear();

  std::vector<RootVolume*>::const_iterator rv;
  std::vector<RootVirtualVolume*>::const_iterator rvv;
  std::vector<std::string> nodeNameStack;
  
  nodeNameStack.reserve(20000);

  std::string baseDir(_mother_name); 


  // loop over all volumes in the ROOT geometry file
  for(nodeNameStack.push_back(baseDir);nodeNameStack.size()>0;) {

    // Get the current node by name.
    std::string name = nodeNameStack.back();    
    nodeNameStack.pop_back();
    
    if( !_GeoManager->cd(name.c_str()) ){
      std::cout << " GeoManager error: path " << name << " not valid " << std::endl; 
      continue;
    }
    
    // get the TGeoVolume
    TGeoNode* node = _GeoManager->GetCurrentNode();
    TGeoVolume* volume = node->GetVolume();

    if(GetVerbosity()>2)
      std::cout << "volume: " << name << "  "  << gGeoManager->GetCurrentNodeId() << std::endl;
        
    // Push all daughters onto the stack.
    for (int i=0; i<node->GetNdaughters(); ++i) {
      std::string dau_name = node->GetDaughter(i)->GetName();

      //      std::string dau_temp = name+"/"+ (node->GetDaughter(i))->GetName();
      //      _GeoManager->cd(dau_temp.c_str());

      if(GetVerbosity()>3)
	std::cout << "   dau: " << dau_name << "  "  << gGeoManager->GetCurrentNodeId() << std::endl;

      // skip all volumes in _excluded_volumes
      bool skip = false;
      for (rv=_excluded_volumes.begin(); rv != _excluded_volumes.end(); rv++){	
	if((*rv)->is_in_name(dau_name)){
	  //	  std::cout << "skip: " << name << "  "  << dau_name << std::endl;
	  skip = true;
	  break;
	}
      }
      if (skip) continue;

      std::string daugh = name+"/"+ (node->GetDaughter(i))->GetName();
      nodeNameStack.push_back(daugh);
    }

    
    // Build the mother volume
    if(_root_mother->is_in_name(name)){
      Volume* vol = BuildRecPackVolume(node, name);
      vol->set_name("setup_name2",volume->GetName());
      Setup::set_mother(vol);

    }

    // create all RecPack volumes and surfaces
    dict::Key volname;
    for (rv=_volumes.begin(); rv != _volumes.end(); rv++){
      // Check that this name matches all conditions in the volume
      if((*rv)->is_in_name(name)){
	const dict::Key father_name = get_father_name(name);
	const double tol = get_tolerance(father_name);	
	Volume* vol = BuildRecPackVolume(node, name, tol);
	vol->set_name("setup_name2",volume->GetName());
	// check whether this is a volume or a surface
	if ((*rv)->has_volume()){
	  volname = name;
	  _nodeIDs[_GeoManager->GetCurrentNodeId()] = name;

	  bool ok = Setup::add_volume(father_name,volname,vol);
	  if (ok){ 
	    // Save the path of some specific volumes
	    std::map<Key, Key>::iterator it;
	    for (it = _saved_volumes.begin(); it!=_saved_volumes.end();it++){
	      if ((*rv)->is_in_name(it->first))
		_saved_volumes[it->first] = name;
	    }
	  }
	  else{
	    std::cout << "RootSetup::BuildSetup(). Unable to add volume !!!" << std::endl;
	  }
	}
	else{
	  // this is a surface
	  volname=father_name;
	}

	if ((*rv)->has_surface()){
	  const dict::Key surf_name = name+"_surf";
	  _surfaces_nodeIDs[_GeoManager->GetCurrentNodeId()] = surf_name;  
	  Surface* surf = BuildRecPackSurface(*vol, (*rv)->axes()[0],(*rv)->axes()[1]);	    
	  //	  std::cout << volname << " " << surf_name << " " << print(vol->position())<< std::endl;
	  bool ok = Setup::add_surface(volname,surf_name,surf);
	  if (!ok){
	    std::cout << "RootSetup::BuildSetup(). Unable to add surface !!!" << std::endl;
	  }
	}
      }

    }    


    // ----- update all virtual volumes -----
    for (rvv=_virtual_volumes.begin(); rvv != _virtual_volumes.end(); rvv++){
      // Check that this name matches all conditions in the volume
      if((*rvv)->is_in_name(name)){
	const std::string& father_name = (*rvv)->father_name();

	// if the father volume is also virtual it will not exist yet
  if(GetVerbosity()>3){
	  std::cout << (*rvv)->name() << std::endl;	  
	  std::cout <<  " - is in name: " << name << std::endl;
	  std::cout <<  " - father: " << father_name << std::endl;
	  std::cout <<  " - father exists: " << has_volume(father_name) << std::endl;
	}

	Volume* vol = BuildRecPackVolume(node, name);
	vol->set_name("setup_name2",volume->GetName());

	(*rvv)->update(vol);
      }
    }

  }

  if(GetVerbosity()>2)
    std::cout << "Add virtual volumes to the setup:" << std::endl;

  // Add all virtual volumes to the setup
  for (rvv=_virtual_volumes.begin(); rvv != _virtual_volumes.end(); rvv++){
    
    const std::string& volname = (*rvv)->name();
    const std::string& father_name = (*rvv)->father_name();
    
    if(GetVerbosity()>3){
      std::cout <<  " - name:   " << volname << std::endl;
      std::cout <<  " - father: " << father_name << std::endl;
      std::cout <<  " - father exists: " << has_volume(father_name) << std::endl;
    }

    const double tol = get_tolerance(father_name);	
    (*rvv)->apply_tolerance(tol);

    if(GetVerbosity()>3){
      std::cout <<  " - tolerance: " << tol << std::endl;
    }


    Volume* vol = (*rvv)->get_RecPack_volume();
    if (!vol){
      std::cout << "RootSetup::BuildSetup(). Unable to add virtual volume. No real volume included in this virtual volume !!!" << std::endl;
      continue;
    }


    bool ok = Setup::add_volume(father_name,volname,vol);

    if (!ok){
      std::cout << "RootSetup::BuildSetup(). Unable to add virtual volume !!!" << std::endl;
    }

  }


}

//*****************************************************************************
bool RootSetup::HasVirtualVolume(const std::string& name){
//*****************************************************************************

  std::vector<RootVirtualVolume*>::const_iterator rvv;
  for (rvv=_virtual_volumes.begin(); rvv != _virtual_volumes.end(); rvv++){
    if((*rvv)->name() == name) return true;
  }

  return false;

}

//*****************************************************************************
void RootSetup::AddVirtualVolume(const std::string& father_name, const std::string& virt_name, 
				 const std::vector<std::string>& volumes_to_include1, 
				 bool include_exclude){
//*****************************************************************************

  bool father_is_virtual = HasVirtualVolume(father_name);
  _virtual_volumes.push_back(new RootVirtualVolume(father_name, father_is_virtual, virt_name, volumes_to_include1, include_exclude));

}

//*****************************************************************************
void RootSetup::AddVirtualVolume(const std::string& father_name, const std::string& virt_name, 
				 const std::vector<std::string>& volumes_to_include1, 
				 const std::vector<std::string>& volumes_to_include2, 
				 bool include_exclude1, bool include_exclude2){
//*****************************************************************************


  bool father_is_virtual = HasVirtualVolume(father_name);
  _virtual_volumes.push_back(new RootVirtualVolume(father_name, father_is_virtual, virt_name, volumes_to_include1, volumes_to_include2, include_exclude1, include_exclude2));

}

//*****************************************************************************
bool RootSetup::GetAverageProperties(const std::string& father_name, 
				     std::vector<std::string> volumes_to_include1, 
				     std::vector<std::string> volumes_to_include2, 
				     bool include_exclude, bool include_father, double& X0_avg, double& DeDx_avg, double& nel_avg, double& cap_considered){
//*****************************************************************************

  // Go to the apropriate node (only if current node isn't correct!)
  if(father_name.compare(_GeoManager->GetPath()) != 0){   
    _GeoManager->cd(father_name.c_str());
  }

  // Get the node and the volume
  TGeoNode *node =_GeoManager->GetCurrentNode();
  TGeoVolume* volume = node->GetVolume();


  if (GetVerbosity()>2 ){
    if (include_father || node->GetNdaughters()>0)
      std::cout << "Computing Avg Properties of volume: " << father_name << ",  Recursive. Father included: " << include_father << ", # daugthers: " << node->GetNdaughters() << std::endl;
  }

  
  double DeDx_dau_avg = 0;
  double nel_dau_avg = 0;
  double X0inv_dau_avg = 0;
  double included_dau_cap = 0;    
  double excluded_dau_cap = 0;    

  ///----------- Loop over daugther  volumes --------------------
  for (int i=0; i<node->GetNdaughters(); ++i) {

    // single name of the daugther 
    std::string dau_name = node->GetDaughter(i)->GetName();

    // full name of daugther
    std::string dau_full_name = father_name+"/"+ dau_name;
    // Go to daugther node
    //    _GeoManager->cd(dau_full_name.c_str());
    _GeoManager->CdDown(i);  
    
    // Get  daugther node and volume
    volume =_GeoManager->GetCurrentVolume();

    // include/exclude volumes containing/not-containing volumes_to_include
    bool found1 = false;
    for (unsigned int j=0;j<volumes_to_include1.size();j++){
      if (dau_full_name.find(volumes_to_include1[j]) != std::string::npos){
	found1 = true;
	break;
      }
    }
    bool found2 = false;
    for (unsigned int j=0;j<volumes_to_include2.size();j++){
      if (dau_full_name.find(volumes_to_include2[j]) != std::string::npos){
	found2 = true;
	break;
      }
    }

    bool include = false;
    if (volumes_to_include1.size()==0 && volumes_to_include2.size()==0){
      include = true;
    }
    else if ((found1 || volumes_to_include1.size()==0) && (found2 || volumes_to_include2.size()==0)){
      if (include_exclude) include = true;
      else include = false;
    }
    else{
      if (include_exclude) include = false;
      else include = true;
    }


    // (include=false, include_exclude=true)  if the volume is not on the list of volumes to include we have to look anyway among his dauthers
    // (include=true,  include_exclude=false) if the volume is not on the list of volumes to exclude get its dE/dx recursively
    // (include=true,  include_exclude=true)  if the volume is on the list of volumes to include get its dE/dx recursively
    if (include || include_exclude){
      double DeDx_dau  = 0;
      double X0_dau  = 0;
      double nel_dau  = 0;
      double cap_dau  = 0;
      
      // Get de/dx. 
      bool ok  = GetAverageProperties(dau_full_name, 
				      volumes_to_include1, volumes_to_include2, 
				      include_exclude, 
				      include, // If include=true we must consider the current dauther and not only his grand dauthers
				      X0_dau, DeDx_dau, nel_dau, cap_dau);      
      if (ok){
	DeDx_dau_avg += cap_dau*DeDx_dau;
	nel_dau_avg += cap_dau*nel_dau;
	X0inv_dau_avg += cap_dau/X0_dau;
	included_dau_cap += cap_dau;
	if (GetVerbosity()>3){
	  std::cout << "   dE/dx, X0, Nel --> dau: " << dau_name << "  "  << DeDx_dau << "  " << X0_dau << "  " << nel_dau << "  " << cap_dau/cm3 << std::endl;
	}
      }
    }
    else{
      double cap_dau = volume->Capacity();
      excluded_dau_cap += cap_dau;
    }

    // Go back to father node
    _GeoManager->CdUp();        
  }
  //------------------------------------------------------------


  // compute the average properties of the daugthers by dividing by the total capacity 
  if (included_dau_cap!=0){
    DeDx_dau_avg /= included_dau_cap;
    nel_dau_avg /= included_dau_cap;
    X0inv_dau_avg /= included_dau_cap;
  }

  // default average properties when the father is not included
  DeDx_avg = DeDx_dau_avg;
  nel_avg = nel_dau_avg;
  X0_avg = 1/X0inv_dau_avg;
  cap_considered = included_dau_cap;

  // Get the capacity and the DeDx of the father volume
  double cap = 0;
  double DeDx = 0;
  double nel = 0;
  double X0 = 0;
  if (include_father){
    node =_GeoManager->GetCurrentNode();
    volume = node->GetVolume();
    cap = volume->Capacity();
    double DeDx;
    double nel;
    
    GetMixtureProperties(volume->GetMaterial(),DeDx,nel);
    X0  = volume->GetMaterial()->GetRadLen();  // this is already in mm
	// If the radiation length came from an old calculation, fix the unitss.
	if (X0 < 1E-6)
		X0  = volume->GetMaterial()->GetRadLen()*g/cm3*cm;

    if (GetVerbosity()>2 && DeDx!=0)
      std::cout << "   dE/dx, X0, Nel --> father: " << father_name << "  "  << DeDx << "  " << X0 << "  " << nel << "  " << cap/cm3 << std::endl;

    DeDx_avg =    (included_dau_cap*DeDx_dau_avg   + (cap-included_dau_cap-excluded_dau_cap)*DeDx)/(cap-excluded_dau_cap);
    nel_avg =    (included_dau_cap*nel_dau_avg   + (cap-included_dau_cap-excluded_dau_cap)*nel)/(cap-excluded_dau_cap);
    X0_avg   = 1/((included_dau_cap*X0inv_dau_avg  + (cap-included_dau_cap-excluded_dau_cap)/X0)  /(cap-excluded_dau_cap));
    cap_considered =cap-excluded_dau_cap;
  }



  if (GetVerbosity()>2 && DeDx_dau_avg!=0){
    std::cout << "Final Avg DeDx of volume: " << father_name << std::endl;
    if (include_father)
      std::cout << "  cap (cm3):        " << cap/cm3 << std::endl;
    std::cout << "  cap_considered  : " << cap_considered/cm3 << std::endl;
    std::cout << "  included_dau_cap: " << included_dau_cap/cm3 << std::endl;
    std::cout << "  excluded_dau_cap: " << excluded_dau_cap/cm3 << std::endl;
    if (include_father)
      std::cout << "  DeDx (MeV/mm):    " << DeDx << std::endl;
    std::cout << "  DeDx_dau_avg:     " << DeDx_dau_avg << std::endl;
    std::cout << "  DeDx_avg:         " << DeDx_avg << std::endl;

    if (include_father)
      std::cout << "  X0   (mm):        " << X0 << std::endl;

    std::cout << "  X0_dau_avg:       " << 1/X0inv_dau_avg  << std::endl;
    std::cout << "  X0_avg:           " << X0_avg            << std::endl;

    if (include_father)
      std::cout << "  Nel   (e-/mm3): " << nel << std::endl;

    std::cout << "  Nel_dau_avg:      " << nel_dau_avg  << std::endl;
    std::cout << "  Nel_avg:          " << nel_avg            << std::endl;

  }

  if (DeDx_avg==0) return false;
  else return true;

}



//*****************************************************************************
bool RootSetup::GetMixtureProperties(TGeoMaterial* mat, double& dedx, double& nel){
//*****************************************************************************



  /*
  TGeoMixture* mix = dynamic_cast<TGeoMixture*>(volume->GetMaterial());
  std::cout << "volume: " << name 
	    << "\n  Is mixture:   " << volume->GetMaterial()->IsMixture()  
	    << "\n  Weight:   " << volume->WeightA()/g 
	    << "\n  Capacity: " << volume->Capacity()/m3 
	    << "\n  Density:  " << volume->GetMaterial()->GetDensity()/(g/cm3) 
	    << "\n  Total Density:  " << volume->WeightA()/volume->Capacity()/(g/cm3) 
	    << "\n  Pressure: " << volume->GetMaterial()->GetPressure()  
	    << "\n  Temp:     " << volume->GetMaterial()->GetTemperature()  
	    << "\n  Rad len:  " << volume->GetMaterial()->GetRadLen()*(g/cm3) << std::endl;

  mix->Print();
  */


  // use density = 1. Correct by density later
  double rho = 1.;

  // MIP
  double p = 300*MeV;


  TGeoMixture* mix = dynamic_cast<TGeoMixture*>(mat);


  if (GetVerbosity()>3)
    mix->Print();

  // get the properties of the mixture
  double *A = mix->GetAmixt();  // Atomic mass
  double *Z = mix->GetZmixt();  // Atomic number
  double *W = mix->GetWmixt();  // weight in mixture


  const int ncomp=mix->GetNelements();
  dedx=0;
  nel=0;
  for (int i=0;i<ncomp;i++){

    double dedx_ele; 
    double nel_ele; 

    GetElementProperties(A[i],Z[i],rho,p, dedx_ele, nel_ele);  

    if (GetVerbosity()>3)
      std::cout << "  - " << i << ": A,Z,W = " << A[i] << ", " << Z[i] << ", " << W[i] << " --> DeDx, Nel = " <<  dedx_ele*(g/cm3) << ", " << nel_ele*(g/mm3) << std::endl;

    dedx += W[i]*dedx_ele;
    nel  += W[i]*nel_ele;
  }

  // correct by the density of the mixture
  dedx *= mix->GetDensity();
  nel  *= mix->GetDensity();

  if (GetVerbosity()>3){
    std::cout << " --> DeDx mixture = " <<  dedx << std::endl;
    std::cout << " --> Nel mixture  = " <<  nel << std::endl;
  }

  return true;


}


//*****************************************************************************
bool RootSetup::GetElementProperties(double A, double Z, double rho, double p, double& dedx, double& nel){
//*****************************************************************************


  // a constant
  double K = 0.307*cm2/g ;

  // Particle mass
  double M = 105.66*MeV;
  
  // electron mass
  double me = 0.511*MeV;

  // Relativistic variables
  double beta = p/sqrt(p*p+M*M);
  double beta2 = beta*beta;
  double gamma2 = 1/(1-beta2);
  double gamma = sqrt(gamma2);

  // Maximum kinetic energy
  double tmax = 2*me*beta2*gamma2/(1+2*gamma*me/M+pow(me/M,2) );

  // Mean Excitation Energy

  double I = 0;
  if (Z>13)
    I = (9.76 + 58.8 * pow(Z,-1.19))*Z*eV;
  else
    I = (12 + 7./Z)*Z*eV;

  // Density effect correction
  double X = log10(beta*gamma);
  double a = 0.19;
  double X1 = 4.5;
  double X0 = 1.764;
  double m = 2.982;
  double hnu = 28.8*sqrt(rho*Z/A)*eV;
  double C = -2*log(I/hnu)-1;

  double delta = 0;
  if (X>X1)
    delta = 2*log(10)*X + C;
  else if (X<X1 && X>X0)
    delta = 2*log(10)*X + a*pow(X1-X,m) + C;

  dedx = rho * K*Z/(A*beta2) * (0.5*log(2*me*beta2*gamma2*tmax/(I*I)) - beta2 - delta/2);

  double Avogadro = 6.0221367e+23/units::mole;  
  
  nel = rho*Avogadro*Z/A*(mm3/g);

  return true;

}

//*****************************************************************************
const Key  RootSetup::get_father_name(const Key& son_name){ 
//*****************************************************************************

  dict::Key father_name="";

  //  std::cout << "son name: " << son_name << std::endl;
  const key_vector& vols = volumes().keys();
  for (key_const_iterator it = vols.begin(); it != vols.end(); it++) {
    const Key& name = *it; 
    if (name == "mother") continue;
    //    std::cout << " - " << name << "\t" << volume(name).name("setup_name2")  << std::endl;


    if (son_name.find(name) != std::string::npos && name.size() > father_name.size()){
      father_name = name;
      //      std::cout << " current_father " << father_name << std::endl;
    }
  }

  if (father_name == "") father_name="mother";


  return father_name;

}

//*****************************************************************************
const double RootSetup::get_tolerance(const Key& name){ 
//*****************************************************************************

  const Volume& vol = volume(name);  
  return vol.parameter("tolerance") + _tolerance;

}

//*****************************************************************************
Volume* RootSetup::BuildRecPackVolume(TGeoNode* node,
				      const Key& name, 
				      const double tolerance){
//*****************************************************************************

  TGeoVolume* volume = node->GetVolume();

  int ini_node = _GeoManager->GetCurrentNodeId();

  
  EVector xaxis(3,0);
  EVector yaxis(3,0);
  EVector zaxis(3,0);
  xaxis[0] =1.; yaxis[1]=1.; zaxis[2] = 1.; 

  // Get the coordinate of this node.
  double local[3] = {0,0,0};
  double master[3];
  _GeoManager->LocalToMaster(local,master);        
  
  EVector pos(3,0);
  pos[0] = master[0]; pos[1] = master[1]; pos[2] = master[2];


  // Get the size of the box
  TGeoBBox *shape  =  dynamic_cast<TGeoBBox*>(volume->GetShape());  
  double xbox = shape->GetDX() - tolerance;
  double ybox = shape->GetDY() - tolerance;
  double zbox = shape->GetDZ() - tolerance;

  if (GetVerbosity()>2){
    std::cout << "RootSetup::BuildRecPackVolume()" << std::endl;
    std::cout << "    name:      " << name << std::endl; 
    std::cout << "    position:  " << print(pos) << std::endl; 
    std::cout << "    size ini:  " << shape->GetDX() << " " << shape->GetDY() << " " << shape->GetDZ() << std::endl;
    std::cout << "    tolerance: " << tolerance << std::endl;
  }


  EVector size0(3,0);
  size0[0]=xbox;
  size0[1]=ybox;
  size0[2]=zbox;

  // Rotate the axis and the size if necessary
  EVector size = size0;
  EVector axis1 = xaxis;
  EVector axis2 = yaxis;

  while (_GeoManager->GetCurrentNodeId() > 1){

    node =_GeoManager->GetCurrentNode();
        
    const Double_t *rot =  node->GetMatrix()->GetRotationMatrix();
    EMatrix A(3,3,0);
    for (int i=0;i<3;i++)
      for (int j=0;j<3;j++)
	A[i][j] = rot[i*3+j];
                   
    size = A*size;

    if (GetVerbosity()>3)
      std::cout << "     Rotate size: " << A << std::endl; 

    
    //    axis1 = A*axis1;
    //    axis2 = A*axis2;

    //    node->GetMatrix()->Print();
    //    std::cout << "name: " << name << ", node: " << _GeoManager->GetCurrentNodeId() << "\n" << size0 << A << size << std::endl;
    
    _GeoManager->CdUp();        

  }

  Volume* vol = new Box(pos,axis1,axis2,fabs(size[0]),fabs(size[1]),fabs(size[2]));

  vol->set_parameter("tolerance",tolerance);

  _GeoManager->CdNode(ini_node);

  //std::cout <<"add volume: " << name << "  " << _GeoManager->GetCurrentNodeId() << std::endl; 

  return vol;

}


//*****************************************************************************
Surface* RootSetup::BuildRecPackSurface(const Volume& volume,
					const EVector& axis1,
					const EVector& axis2){
//*****************************************************************************

  const EVector& pos = volume.position();
  const EVector size = volume.size();

  double size1 = dot(size,axis1);
  double size2 = dot(size,axis2);

  EVector normal = crossprod(axis1,axis2);

  /*
  std::cout << "size: " << size << std::endl;
  std::cout << "normal: " << normal << std::endl;
  std::cout << "axis1: " << axis1 << std::endl;
  std::cout << "axis2: " << axis2 << std::endl;
  */

  Surface* surf = new Rectangle(pos, normal, axis1, size1, size2);


  return surf;

}

//*****************************************************************************
void RootSetup::AddSurface(const Key& name, const EVector& axis1, 
			   const EVector& axis2){
//*****************************************************************************  
  
  RootVolume* vol = new RootVolume(name);
  vol->set_has_volume(false);
  vol->set_has_surface(true);
  vol->add_axis(axis1);
  vol->add_axis(axis2);

  _volumes.push_back(vol);

} 

//*****************************************************************************
void RootSetup::AddSurface(const Key& n1, const Key& n2, 
			   const EVector& axis1, const EVector& axis2){
//*****************************************************************************  
  
  RootVolume* vol = new RootVolume(n1,n2);
  vol->set_has_volume(false);
  vol->set_has_surface(true);
  vol->add_axis(axis1);
  vol->add_axis(axis2);

  _volumes.push_back(vol);

} 

//*****************************************************************************
void RootSetup::AddSurface(const Key& n1, const Key& n2, const Key& n3, 
			   const EVector& axis1, const EVector& axis2){
//*****************************************************************************  
  
  RootVolume* vol = new RootVolume(n1,n2,n3);
  vol->set_has_volume(false);
  vol->set_has_surface(true);
  vol->add_axis(axis1);
  vol->add_axis(axis2);

  _volumes.push_back(vol);

} 

//*****************************************************************************
void RootSetup::AddVolumeSurface(const Key& name, const EVector& axis1, 
				 const EVector& axis2){
//*****************************************************************************  
  
  RootVolume* vol = new RootVolume(name);

  vol->set_has_surface(true);
  vol->add_axis(axis1);
  vol->add_axis(axis2);

  _volumes.push_back(vol);

} 

//*****************************************************************************
void RootSetup::AddVolumeSurface(const Key& name1, const Key& name2, 
				 const EVector& axis1, const EVector& axis2){
//*****************************************************************************  
  
  RootVolume* vol = new RootVolume(name1,name2);

  vol->set_has_surface(true);
  vol->add_axis(axis1);
  vol->add_axis(axis2);

  _volumes.push_back(vol);

} 

//*****************************************************************************
void RootSetup::AddVolumeSurface(const Key& name1, const Key& name2, const Key& name3, 
				 const EVector& axis1, const EVector& axis2){
//*****************************************************************************  
  
  RootVolume* vol = new RootVolume(name1,name2,name3);

  vol->set_has_surface(true);
  vol->add_axis(axis1);
  vol->add_axis(axis2);

  _volumes.push_back(vol);

} 

