
// Node <--> logical volume in GEANT4

#include "TGeoManager.h"
#include "TGeoNode.h"

#include <vector>

void CheckGeometryROOT
(
 string filename = "../bin/geometry.root",
 )
{
  TGeoManager::Import(filename.c_str());
  TGeoVolume *world = (TGeoVolume*) gGeoManager->GetTopVolume();
  //world->Draw();

  int Nvolumes = world->GetNtotal(); // # of logical volumes

  cout << "# of nodes = " << Nvolumes << endl;
  
  //cout << world->Sizeof() << endl;

  exit(1);
  
  std::vector<TGeoNode*> Basket;
  for(int i=0;i<world->GetNdaughters();i++){
    Basket.push_back(world->GetNode(i));
    cout << Basket[i]->GetName() << endl;
  }
  
  Basket[0]->PrintVoxels();

  //cout << world->CountNodes() << endl;
  //cout << world->GetName() << endl;
  //cout << world->GetNtotal() << endl;

  //gGeoManager->GetTopVolume()->PrintVoxels();  

}
