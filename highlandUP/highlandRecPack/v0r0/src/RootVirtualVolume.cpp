#include "RootVirtualVolume.h"
#include <recpack/Box.h>


//*****************************************************************************
RootVirtualVolume::RootVirtualVolume(const std::string& father_name, bool virtual_father, 
				     const std::string& virt_name, 
				     const std::vector<std::string>& volumes_to_include1,
				     const std::vector<std::string>& volumes_to_include2, 
				     bool include_exclude1, bool include_exclude2){
//*****************************************************************************      

  _volume = NULL;

  _virtual_father = virtual_father;
  _father_name = father_name;
  _virt_name = virt_name;
  _full_name = father_name+"/"+virt_name;
  _included_volumes1 = std::vector<std::string>(volumes_to_include1);
  _included_volumes2 = std::vector<std::string>(volumes_to_include2);
  _include1 = include_exclude1;
  _include2 = include_exclude2;
  

}

//*****************************************************************************
RootVirtualVolume::RootVirtualVolume(const std::string& father_name, 
				     bool virtual_father,
				     const std::string& virt_name, 
				     const std::vector<std::string>& volumes_to_include1, 
				     bool include_exclude){
//*****************************************************************************      
  _volume = NULL;

  _virtual_father = virtual_father;
  _father_name = father_name;
  _virt_name = virt_name;
  _full_name = father_name+"/"+virt_name;
  _included_volumes1 = std::vector<std::string>(volumes_to_include1);
  _included_volumes2 = std::vector<std::string>();
  _include1 = include_exclude;
  _include2 = false;
  

}


//*****************************************************************************
bool RootVirtualVolume::is_in_name(const Key& name){
//*****************************************************************************

  // the father volume has to be part of the name
  if (name.find(_father_name+"/") == std::string::npos && !_virtual_father) return false;



  int n1 = _included_volumes1.size();
  int n2 = _included_volumes2.size();

  bool found1 = true;  
  if (n1>0){
    found1 = false;  
    for (int i=0;i<n1; i++){
      //    std::cout << i << " " << _included_volumes[i] << std::endl;
      if (name.find(_included_volumes1[i]) != std::string::npos){
	found1 = true;
	break;
      }
    }
  }


  bool found2 = true;  
  if (n2>0){
    found2 = false;  
    for (int i=0;i<n2; i++){
      if (name.find(_included_volumes2[i]) != std::string::npos){
	found2 = true;
	break;
      }
    }
  }


  bool ok=false;
  /*
  if (found1 && found2){
    if (_include1) ok = true;
    else ok = false;
  }
  else{
    if (_include1) ok = false;
    else ok = true;
  }
  */

  if (found1 == _include1 && (found2==_include2 || n2==0) )
    ok = true;

  return ok;

}


//*****************************************************************************
void RootVirtualVolume::update(Volume* vol){
//*****************************************************************************

  EVector pos_max = vol->position() + vol->size();
  EVector pos_min = vol->position() - vol->size();

  // If there is no virtual volume yet, just make a copy of the input volume
  if (!_volume){
    _volume = new Box(vol->position(), vol->axis("U"), vol->axis("V"), 
		      vol->parameter("U"), vol->parameter("V"), vol->parameter("W"));
    _volume->set_name("setup_name2",_full_name);

    /*
    std::cout << "Update: " << _full_name << std::endl;
    std::cout << "  include: " << vol->name("setup_name2") << ", pos = " << print(vol->position()) << ", size = " << print(vol->size()) << std::endl;
    std::cout << print(pos_min) << std::endl;
    std::cout << print(pos_max) << std::endl;
    */
    return;
  }


  EVector old_pos_max = _volume->position() + _volume->size();
  EVector old_pos_min = _volume->position() - _volume->size();

  EVector new_pos_max = old_pos_max;
  EVector new_pos_min = old_pos_min;

  for (int i=0;i<3;i++){
    if (pos_max[i] > old_pos_max[i]) new_pos_max[i]= pos_max[i];
    if (pos_min[i] < old_pos_min[i]) new_pos_min[i]= pos_min[i];
  }

  EVector pos  = (new_pos_max+new_pos_min)*0.5;
  EVector size = (new_pos_max-new_pos_min)*0.5;

  EVector uaxis = _volume->axis("U");
  EVector vaxis = _volume->axis("V");

  /*
  std::cout << "Update: " << _full_name << std::endl;
  std::cout << "  include: " << vol->name("setup_name2") << ", pos = " << print(vol->position()) << ", size = " << print(vol->size()) << std::endl;

  std::cout << print(pos_min) << "  " << print(old_pos_min) << "  " << print(new_pos_min) << std::endl;
  std::cout << print(pos_max) << "  " << print(old_pos_max) << "  " << print(new_pos_max) << std::endl;
  */


  delete _volume;

  _volume = new Box(pos, uaxis, vaxis, size[0], size[1], size[2]);
  _volume->set_name("setup_name2",_full_name);

}

//*****************************************************************************
void RootVirtualVolume::apply_tolerance(double tol){
//*****************************************************************************


  double U = _volume->parameter("U");
  double V = _volume->parameter("V");
  double W = _volume->parameter("W");
  

  EVector pos = _volume->position();
  EVector uaxis = _volume->axis("U");
  EVector vaxis = _volume->axis("V");

  delete _volume;

  _volume = new Box(pos, uaxis, vaxis, U-tol, V-tol, W-tol);
  _volume->set_name("setup_name2",_full_name);
  _volume->set_parameter("tolerance",tol);

}
