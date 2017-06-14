
// 
// Class to get inputs other than GEANT4 ones
// 
// 4/7/16 Davide Sgalaberna
//

#include "ExN02ND280XML.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

//#include "UtlXML.h"

using namespace std;

ExN02ND280XML::ExN02ND280XML(const G4String inxml){
  fXMLFile = inxml;
  SetInputs();
}

ExN02ND280XML::~ExN02ND280XML(){
  //delete ...;
}

void ExN02ND280XML::SetInputs(){
  
  G4cout << G4endl;
  G4cout << "Reading inputs from XML file: " 
	 << GetXMLFileName() 
	 << G4endl;
  G4cout << G4endl;
  
  string line;
  ifstream myfile (fXMLFile);
  if(myfile.is_open()){    
    
    XML2Bool("IsRandomSeed",fIsRandomSeed);
    XML2Bool("StoreGeometry",fStoreGeometry);
    XML2String("Generator",fGenerType);
    XML2String("Path2file",fPathFiles);
    XML2String("filename",fGenerFileName);  
    XML2String("treename",fGenerTreeName);      
    
    // Forward TPCs 
    XML2Bool("forwTPCdefault" ,fForwTPCdefault);
    XML2Double("forwTPCPos1_X" ,fForwTPCPos1_X);
    XML2Double("forwTPCPos1_Y" ,fForwTPCPos1_Y);
    XML2Double("forwTPCPos1_Z" ,fForwTPCPos1_Z);
    XML2Double("forwTPCPos2_X" ,fForwTPCPos2_X);
    XML2Double("forwTPCPos2_Y" ,fForwTPCPos2_Y);
    XML2Double("forwTPCPos2_Z" ,fForwTPCPos2_Z);
    XML2Double("forwTPCPos3_X" ,fForwTPCPos3_X);
    XML2Double("forwTPCPos3_Y" ,fForwTPCPos3_Y);
    XML2Double("forwTPCPos3_Z" ,fForwTPCPos3_Z);

    // Side TPCs 1
    XML2Bool("sideTPCdefault1" ,fSideTPCdefault1);
    XML2Double("sideTPClength1",fSideTPClength1);
    XML2Double("sideTPCwidth1" ,fSideTPCwidth1);
    XML2Double("sideTPCheight1",fSideTPCheight1);
    XML2Double("sideTPCUpPos1_X" ,fSideTPCUpPos1_X);
    XML2Double("sideTPCUpPos1_Y" ,fSideTPCUpPos1_Y);
    XML2Double("sideTPCUpPos1_Z" ,fSideTPCUpPos1_Z);
    XML2Double("sideTPCDownPos1_X" ,fSideTPCDownPos1_X);
    XML2Double("sideTPCDownPos1_Y" ,fSideTPCDownPos1_Y);
    XML2Double("sideTPCDownPos1_Z" ,fSideTPCDownPos1_Z);
	
    // Side TPCs 2
    XML2Bool("sideTPCdefault2" ,fSideTPCdefault2);
    XML2Double("sideTPClength2",fSideTPClength2);
    XML2Double("sideTPCwidth2" ,fSideTPCwidth2);
    XML2Double("sideTPCheight2",fSideTPCheight2);
    XML2Double("sideTPCUpPos2_X" ,fSideTPCUpPos2_X);
    XML2Double("sideTPCUpPos2_Y" ,fSideTPCUpPos2_Y);
    XML2Double("sideTPCUpPos2_Z" ,fSideTPCUpPos2_Z);
    XML2Double("sideTPCDownPos2_X" ,fSideTPCDownPos2_X);
    XML2Double("sideTPCDownPos2_Y" ,fSideTPCDownPos2_Y);
    XML2Double("sideTPCDownPos2_Z" ,fSideTPCDownPos2_Z);
    
    // Target 1
    XML2Bool("Targetdefault1"   ,fTargetdefault1);
    XML2String("TargetMaterial1",fTargetMaterial1);
    XML2Double("Targetlength1"  ,fTargetlength1);
    XML2Double("Targetwidth1"   ,fTargetwidth1);
    XML2Double("Targetheight1"  ,fTargetheight1);
    XML2Double("TargetPos1_X"   ,fTargetPos1_X);
    XML2Double("TargetPos1_Y"   ,fTargetPos1_Y);
    XML2Double("TargetPos1_Z"   ,fTargetPos1_Z);
    
    // Target 2
    XML2Bool("Targetdefault2"   ,fTargetdefault2);
    XML2String("TargetMaterial2",fTargetMaterial2);
    XML2Double("Targetlength2"  ,fTargetlength2);
    XML2Double("Targetwidth2"   ,fTargetwidth2);
    XML2Double("Targetheight2"  ,fTargetheight2);
    XML2Double("TargetPos2_X"   ,fTargetPos2_X);
    XML2Double("TargetPos2_Y"   ,fTargetPos2_Y);
    XML2Double("TargetPos2_Z"   ,fTargetPos2_Z);

    // FGD 1
    XML2Bool("FGDdefault1"   ,fFGDdefault1);
    XML2String("FGDMaterial1",fFGDMaterial1);
    XML2Double("FGDlength1"  ,fFGDlength1);
    XML2Double("FGDwidth1"   ,fFGDwidth1);
    XML2Double("FGDheight1"  ,fFGDheight1);
    XML2Double("FGDPos1_X"   ,fFGDPos1_X);
    XML2Double("FGDPos1_Y"   ,fFGDPos1_Y);
    XML2Double("FGDPos1_Z"   ,fFGDPos1_Z);
    
    // FGD 2
    XML2Bool("FGDdefault2"   ,fFGDdefault2);
    XML2String("FGDMaterial2",fFGDMaterial2);
    XML2Double("FGDlength2"  ,fFGDlength2);
    XML2Double("FGDwidth2"   ,fFGDwidth2);
    XML2Double("FGDheight2"  ,fFGDheight2);
    XML2Double("FGDPos2_X"   ,fFGDPos2_X);
    XML2Double("FGDPos2_Y"   ,fFGDPos2_Y);
    XML2Double("FGDPos2_Z"   ,fFGDPos2_Z);
    
    // SuperFGD 1
    XML2Double("SuperFGDCubeEdge1" ,fSuperFGDCubeEdge1);
    XML2Int("SuperFGDCubeNum1_X",fSuperFGDCubeNum1_X);
    XML2Int("SuperFGDCubeNum1_Y",fSuperFGDCubeNum1_Y);
    XML2Int("SuperFGDCubeNum1_Z",fSuperFGDCubeNum1_Z);
    XML2Double("SuperFGDPos1_X"    ,fSuperFGDPos1_X);
    XML2Double("SuperFGDPos1_Y"    ,fSuperFGDPos1_Y);
    XML2Double("SuperFGDPos1_Z"    ,fSuperFGDPos1_Z);
    // SuperFGD 2
    XML2Double("SuperFGDCubeEdge2" ,fSuperFGDCubeEdge2);
    XML2Int("SuperFGDCubeNum2_X",fSuperFGDCubeNum2_X);
    XML2Int("SuperFGDCubeNum2_Y",fSuperFGDCubeNum2_Y);
    XML2Int("SuperFGDCubeNum2_Z",fSuperFGDCubeNum2_Z);
    XML2Double("SuperFGDPos2_X"    ,fSuperFGDPos2_X);
    XML2Double("SuperFGDPos2_Y"    ,fSuperFGDPos2_Y);
    XML2Double("SuperFGDPos2_Z"    ,fSuperFGDPos2_Z);

    // WAGASCI 1
    XML2String("WAGASCIInactMaterial1",fWAGASCIInactMaterial1);
    XML2Double("WAGASCIlength1"   ,fWAGASCIlength1);
    XML2Double("WAGASCIwidth1"    ,fWAGASCIwidth1);
    XML2Double("WAGASCIheight1"   ,fWAGASCIheight1);
    XML2Double("WAGASCIPos1_X"    ,fWAGASCIPos1_X);
    XML2Double("WAGASCIPos1_Y"    ,fWAGASCIPos1_Y);
    XML2Double("WAGASCIPos1_Z"    ,fWAGASCIPos1_Z);
    // WAGASCI 2
    XML2String("WAGASCIInactMaterial2",fWAGASCIInactMaterial2);
    XML2Double("WAGASCIlength2"   ,fWAGASCIlength2);
    XML2Double("WAGASCIwidth2"    ,fWAGASCIwidth2);
    XML2Double("WAGASCIheight2"   ,fWAGASCIheight2);
    XML2Double("WAGASCIPos2_X"    ,fWAGASCIPos2_X);
    XML2Double("WAGASCIPos2_Y"    ,fWAGASCIPos2_Y);
    XML2Double("WAGASCIPos2_Z"    ,fWAGASCIPos2_Z);

    // FGD3D 1
    XML2Double("FGD3Dlength1"   ,fFGD3Dlength1);
    XML2Double("FGD3Dwidth1"    ,fFGD3Dwidth1);
    XML2Double("FGD3Dheight1"   ,fFGD3Dheight1);
    XML2Double("FGD3DPos1_X"    ,fFGD3DPos1_X);
    XML2Double("FGD3DPos1_Y"    ,fFGD3DPos1_Y);
    XML2Double("FGD3DPos1_Z"    ,fFGD3DPos1_Z);
    // FGD3D 2
    XML2Double("FGD3Dlength2"   ,fFGD3Dlength2);
    XML2Double("FGD3Dwidth2"    ,fFGD3Dwidth2);
    XML2Double("FGD3Dheight2"   ,fFGD3Dheight2);
    XML2Double("FGD3DPos2_X"    ,fFGD3DPos2_X);
    XML2Double("FGD3DPos2_Y"    ,fFGD3DPos2_Y);
    XML2Double("FGD3DPos2_Z"    ,fFGD3DPos2_Z);

    // SciFi XZ
    XML2Double("SciFiFiberEdge",fSciFiFiberEdge);
    XML2Int("SciFiNum_AlongX"  ,fSciFiNum_AlongX);
    XML2Int("SciFiNum_AlongZ"  ,fSciFiNum_AlongZ);
    XML2Int("SciFiNum_Layer"   ,fSciFiNum_Layer);
    XML2Double("SciFiPos_X"    ,fSciFiPos_X);
    XML2Double("SciFiPos_Y"    ,fSciFiPos_Y);
    XML2Double("SciFiPos_Z"    ,fSciFiPos_Z);

    // FGDlike XZ
    XML2Double("FGDlikeBarEdge",fFGDlikeBarEdge);
    XML2Int("FGDlikeNum_AlongX"  ,fFGDlikeNum_AlongX);
    XML2Int("FGDlikeNum_AlongZ"  ,fFGDlikeNum_AlongZ);
    XML2Int("FGDlikeNum_Layer"   ,fFGDlikeNum_Layer);
    XML2Double("FGDlikePos_X"    ,fFGDlikePos_X);
    XML2Double("FGDlikePos_Y"    ,fFGDlikePos_Y);
    XML2Double("FGDlikePos_Z"    ,fFGDlikePos_Z);

    // ToF Top Downstream 
    XML2Bool("ToFdefault_TopDown"      ,fToFdefault_TopDown);
    XML2Int("ToFPlaneXYNum_TopDown"    ,fToFPlaneXYNum_TopDown);
    XML2Int("ToFLayerHorizNBar_TopDown",fToFLayerHorizNBar_TopDown);
    XML2Int("ToFLayerVertNBar_TopDown" ,fToFLayerVertNBar_TopDown);
    XML2Double("ToFBarwidth_TopDown"      ,fToFBarwidth_TopDown);
    XML2Double("ToFBarheight_TopDown"     ,fToFBarheight_TopDown);
    XML2Double("ToFRotX_TopDown"         ,fToFRotX_TopDown);
    XML2Double("ToFRotY_TopDown"         ,fToFRotY_TopDown);
    XML2Double("ToFRotZ_TopDown"         ,fToFRotZ_TopDown);
    XML2Double("ToFPosX_TopDown"         ,fToFPosX_TopDown);
    XML2Double("ToFPosY_TopDown"         ,fToFPosY_TopDown);
    XML2Double("ToFPosZ_TopDown"         ,fToFPosZ_TopDown);

    // ToF Bottom Downstream 
    XML2Bool("ToFdefault_BotDown"      ,fToFdefault_BotDown);
    XML2Int("ToFPlaneXYNum_BotDown"    ,fToFPlaneXYNum_BotDown);
    XML2Int("ToFLayerHorizNBar_BotDown",fToFLayerHorizNBar_BotDown);
    XML2Int("ToFLayerVertNBar_BotDown" ,fToFLayerVertNBar_BotDown);
    XML2Double("ToFBarwidth_BotDown"      ,fToFBarwidth_BotDown);
    XML2Double("ToFBarheight_BotDown"     ,fToFBarheight_BotDown);
    XML2Double("ToFRotX_BotDown"         ,fToFRotX_BotDown);
    XML2Double("ToFRotY_BotDown"         ,fToFRotY_BotDown);
    XML2Double("ToFRotZ_BotDown"         ,fToFRotZ_BotDown);
    XML2Double("ToFPosX_BotDown"         ,fToFPosX_BotDown);
    XML2Double("ToFPosY_BotDown"         ,fToFPosY_BotDown);
    XML2Double("ToFPosZ_BotDown"         ,fToFPosZ_BotDown);

    // ToF Right Downstream 
    XML2Bool("ToFdefault_RightDown"      ,fToFdefault_RightDown);
    XML2Int("ToFPlaneXYNum_RightDown"    ,fToFPlaneXYNum_RightDown);
    XML2Int("ToFLayerHorizNBar_RightDown",fToFLayerHorizNBar_RightDown);
    XML2Int("ToFLayerVertNBar_RightDown" ,fToFLayerVertNBar_RightDown);
    XML2Double("ToFBarwidth_RightDown"      ,fToFBarwidth_RightDown);
    XML2Double("ToFBarheight_RightDown"     ,fToFBarheight_RightDown);
    XML2Double("ToFRotX_RightDown"         ,fToFRotX_RightDown);
    XML2Double("ToFRotY_RightDown"         ,fToFRotY_RightDown);
    XML2Double("ToFRotZ_RightDown"         ,fToFRotZ_RightDown);
    XML2Double("ToFPosX_RightDown"         ,fToFPosX_RightDown);
    XML2Double("ToFPosY_RightDown"         ,fToFPosY_RightDown);
    XML2Double("ToFPosZ_RightDown"         ,fToFPosZ_RightDown);

    // ToF Left Downstream 
    XML2Bool("ToFdefault_LeftDown"      ,fToFdefault_LeftDown);
    XML2Int("ToFPlaneXYNum_LeftDown"    ,fToFPlaneXYNum_LeftDown);
    XML2Int("ToFLayerHorizNBar_LeftDown",fToFLayerHorizNBar_LeftDown);
    XML2Int("ToFLayerVertNBar_LeftDown" ,fToFLayerVertNBar_LeftDown);
    XML2Double("ToFBarwidth_LeftDown"      ,fToFBarwidth_LeftDown);
    XML2Double("ToFBarheight_LeftDown"     ,fToFBarheight_LeftDown);
    XML2Double("ToFRotX_LeftDown"         ,fToFRotX_LeftDown);
    XML2Double("ToFRotY_LeftDown"         ,fToFRotY_LeftDown);
    XML2Double("ToFRotZ_LeftDown"         ,fToFRotZ_LeftDown);
    XML2Double("ToFPosX_LeftDown"         ,fToFPosX_LeftDown);
    XML2Double("ToFPosY_LeftDown"         ,fToFPosY_LeftDown);
    XML2Double("ToFPosZ_LeftDown"         ,fToFPosZ_LeftDown);

    // ToF Back Downstream 
    XML2Bool("ToFdefault_BackDown"      ,fToFdefault_BackDown);
    XML2Int("ToFPlaneXYNum_BackDown"    ,fToFPlaneXYNum_BackDown);
    XML2Int("ToFLayerHorizNBar_BackDown",fToFLayerHorizNBar_BackDown);
    XML2Int("ToFLayerVertNBar_BackDown" ,fToFLayerVertNBar_BackDown);
    XML2Double("ToFBarwidth_BackDown"      ,fToFBarwidth_BackDown);
    XML2Double("ToFBarheight_BackDown"     ,fToFBarheight_BackDown);
    XML2Double("ToFRotX_BackDown"         ,fToFRotX_BackDown);
    XML2Double("ToFRotY_BackDown"         ,fToFRotY_BackDown);
    XML2Double("ToFRotZ_BackDown"         ,fToFRotZ_BackDown);
    XML2Double("ToFPosX_BackDown"         ,fToFPosX_BackDown);
    XML2Double("ToFPosY_BackDown"         ,fToFPosY_BackDown);
    XML2Double("ToFPosZ_BackDown"         ,fToFPosZ_BackDown);

    // ToF Front Downstream 
    XML2Bool("ToFdefault_FrontDown"      ,fToFdefault_FrontDown);
    XML2Int("ToFPlaneXYNum_FrontDown"    ,fToFPlaneXYNum_FrontDown);
    XML2Int("ToFLayerHorizNBar_FrontDown",fToFLayerHorizNBar_FrontDown);
    XML2Int("ToFLayerVertNBar_FrontDown" ,fToFLayerVertNBar_FrontDown);
    XML2Double("ToFBarwidth_FrontDown"      ,fToFBarwidth_FrontDown);
    XML2Double("ToFBarheight_FrontDown"     ,fToFBarheight_FrontDown);
    XML2Double("ToFRotX_FrontDown"         ,fToFRotX_FrontDown);
    XML2Double("ToFRotY_FrontDown"         ,fToFRotY_FrontDown);
    XML2Double("ToFRotZ_FrontDown"         ,fToFRotZ_FrontDown);
    XML2Double("ToFPosX_FrontDown"         ,fToFPosX_FrontDown);
    XML2Double("ToFPosY_FrontDown"         ,fToFPosY_FrontDown);
    XML2Double("ToFPosZ_FrontDown"         ,fToFPosZ_FrontDown);

    // ToF ECalP0D
    XML2Bool("ToFdefault_ECalP0D"      ,fToFdefault_ECalP0D);
    XML2Int("ToFPlaneXYNum_ECalP0D"    ,fToFPlaneXYNum_ECalP0D);
    XML2Int("ToFLayerHorizNBar_ECalP0D",fToFLayerHorizNBar_ECalP0D);
    XML2Int("ToFLayerVertNBar_ECalP0D" ,fToFLayerVertNBar_ECalP0D);
    XML2Double("ToFBarwidth_ECalP0D"      ,fToFBarwidth_ECalP0D);
    XML2Double("ToFBarheight_ECalP0D"     ,fToFBarheight_ECalP0D);
    XML2Double("ToFRotX_ECalP0D"         ,fToFRotX_ECalP0D);
    XML2Double("ToFRotY_ECalP0D"         ,fToFRotY_ECalP0D);
    XML2Double("ToFRotZ_ECalP0D"         ,fToFRotZ_ECalP0D);
    XML2Double("ToFPosX_ECalP0D"         ,fToFPosX_ECalP0D);
    XML2Double("ToFPosY_ECalP0D"         ,fToFPosY_ECalP0D);
    XML2Double("ToFPosZ_ECalP0D"         ,fToFPosZ_ECalP0D);

    // ToF Top Upstream 
    XML2Bool("ToFdefault_TopUp"      ,fToFdefault_TopUp);
    XML2Int("ToFPlaneXYNum_TopUp"    ,fToFPlaneXYNum_TopUp);
    XML2Int("ToFLayerHorizNBar_TopUp",fToFLayerHorizNBar_TopUp);
    XML2Int("ToFLayerVertNBar_TopUp" ,fToFLayerVertNBar_TopUp);
    XML2Double("ToFBarwidth_TopUp"      ,fToFBarwidth_TopUp);
    XML2Double("ToFBarheight_TopUp"     ,fToFBarheight_TopUp);
    XML2Double("ToFRotX_TopUp"         ,fToFRotX_TopUp);
    XML2Double("ToFRotY_TopUp"         ,fToFRotY_TopUp);
    XML2Double("ToFRotZ_TopUp"         ,fToFRotZ_TopUp);
    XML2Double("ToFPosX_TopUp"         ,fToFPosX_TopUp);
    XML2Double("ToFPosY_TopUp"         ,fToFPosY_TopUp);
    XML2Double("ToFPosZ_TopUp"         ,fToFPosZ_TopUp);

    // ToF Bottom Upstream 
    XML2Bool("ToFdefault_BotUp"      ,fToFdefault_BotUp);
    XML2Int("ToFPlaneXYNum_BotUp"    ,fToFPlaneXYNum_BotUp);
    XML2Int("ToFLayerHorizNBar_BotUp",fToFLayerHorizNBar_BotUp);
    XML2Int("ToFLayerVertNBar_BotUp" ,fToFLayerVertNBar_BotUp);
    XML2Double("ToFBarwidth_BotUp"      ,fToFBarwidth_BotUp);
    XML2Double("ToFBarheight_BotUp"     ,fToFBarheight_BotUp);
    XML2Double("ToFRotX_BotUp"         ,fToFRotX_BotUp);
    XML2Double("ToFRotY_BotUp"         ,fToFRotY_BotUp);
    XML2Double("ToFRotZ_BotUp"         ,fToFRotZ_BotUp);
    XML2Double("ToFPosX_BotUp"         ,fToFPosX_BotUp);
    XML2Double("ToFPosY_BotUp"         ,fToFPosY_BotUp);
    XML2Double("ToFPosZ_BotUp"         ,fToFPosZ_BotUp);

    // ToF Right Upstream 
    XML2Bool("ToFdefault_RightUp"      ,fToFdefault_RightUp);
    XML2Int("ToFPlaneXYNum_RightUp"    ,fToFPlaneXYNum_RightUp);
    XML2Int("ToFLayerHorizNBar_RightUp",fToFLayerHorizNBar_RightUp);
    XML2Int("ToFLayerVertNBar_RightUp" ,fToFLayerVertNBar_RightUp);
    XML2Double("ToFBarwidth_RightUp"      ,fToFBarwidth_RightUp);
    XML2Double("ToFBarheight_RightUp"     ,fToFBarheight_RightUp);
    XML2Double("ToFRotX_RightUp"         ,fToFRotX_RightUp);
    XML2Double("ToFRotY_RightUp"         ,fToFRotY_RightUp);
    XML2Double("ToFRotZ_RightUp"         ,fToFRotZ_RightUp);
    XML2Double("ToFPosX_RightUp"         ,fToFPosX_RightUp);
    XML2Double("ToFPosY_RightUp"         ,fToFPosY_RightUp);
    XML2Double("ToFPosZ_RightUp"         ,fToFPosZ_RightUp);

    // ToF Left Upstream 
    XML2Bool("ToFdefault_LeftUp"      ,fToFdefault_LeftUp);
    XML2Int("ToFPlaneXYNum_LeftUp"    ,fToFPlaneXYNum_LeftUp);
    XML2Int("ToFLayerHorizNBar_LeftUp",fToFLayerHorizNBar_LeftUp);
    XML2Int("ToFLayerVertNBar_LeftUp" ,fToFLayerVertNBar_LeftUp);
    XML2Double("ToFBarwidth_LeftUp"      ,fToFBarwidth_LeftUp);
    XML2Double("ToFBarheight_LeftUp"     ,fToFBarheight_LeftUp);
    XML2Double("ToFRotX_LeftUp"         ,fToFRotX_LeftUp);
    XML2Double("ToFRotY_LeftUp"         ,fToFRotY_LeftUp);
    XML2Double("ToFRotZ_LeftUp"         ,fToFRotZ_LeftUp);
    XML2Double("ToFPosX_LeftUp"         ,fToFPosX_LeftUp);
    XML2Double("ToFPosY_LeftUp"         ,fToFPosY_LeftUp);
    XML2Double("ToFPosZ_LeftUp"         ,fToFPosZ_LeftUp);

    // ToF Back Upstream 
    XML2Bool("ToFdefault_BackUp"      ,fToFdefault_BackUp);
    XML2Int("ToFPlaneXYNum_BackUp"    ,fToFPlaneXYNum_BackUp);
    XML2Int("ToFLayerHorizNBar_BackUp",fToFLayerHorizNBar_BackUp);
    XML2Int("ToFLayerVertNBar_BackUp" ,fToFLayerVertNBar_BackUp);
    XML2Double("ToFBarwidth_BackUp"      ,fToFBarwidth_BackUp);
    XML2Double("ToFBarheight_BackUp"     ,fToFBarheight_BackUp);
    XML2Double("ToFRotX_BackUp"         ,fToFRotX_BackUp);
    XML2Double("ToFRotY_BackUp"         ,fToFRotY_BackUp);
    XML2Double("ToFRotZ_BackUp"         ,fToFRotZ_BackUp);
    XML2Double("ToFPosX_BackUp"         ,fToFPosX_BackUp);
    XML2Double("ToFPosY_BackUp"         ,fToFPosY_BackUp);
    XML2Double("ToFPosZ_BackUp"         ,fToFPosZ_BackUp);

    // ToF Front Upstream 
    XML2Bool("ToFdefault_FrontUp"      ,fToFdefault_FrontUp);
    XML2Int("ToFPlaneXYNum_FrontUp"    ,fToFPlaneXYNum_FrontUp);
    XML2Int("ToFLayerHorizNBar_FrontUp",fToFLayerHorizNBar_FrontUp);
    XML2Int("ToFLayerVertNBar_FrontUp" ,fToFLayerVertNBar_FrontUp);
    XML2Double("ToFBarwidth_FrontUp"      ,fToFBarwidth_FrontUp);
    XML2Double("ToFBarheight_FrontUp"     ,fToFBarheight_FrontUp);
    XML2Double("ToFRotX_FrontUp"         ,fToFRotX_FrontUp);
    XML2Double("ToFRotY_FrontUp"         ,fToFRotY_FrontUp);
    XML2Double("ToFRotZ_FrontUp"         ,fToFRotZ_FrontUp);
    XML2Double("ToFPosX_FrontUp"         ,fToFPosX_FrontUp);
    XML2Double("ToFPosY_FrontUp"         ,fToFPosY_FrontUp);
    XML2Double("ToFPosZ_FrontUp"         ,fToFPosZ_FrontUp);


    //
    // Set detectors used in the simulation
    //
    // Use forward TPCs
    XML2Bool("UseForwTPC1" ,fUseForwTPC1);
    XML2Bool("UseForwTPC2" ,fUseForwTPC2);
    XML2Bool("UseForwTPC3" ,fUseForwTPC3);
    // Use horizontal targets
    XML2Bool("UseTarget1" ,fUseTarget1);
    XML2Bool("UseTarget2" ,fUseTarget2);
    // Use side TPCs
    XML2Bool("UseTPCDown1" ,fUseTPCDown1);
    XML2Bool("UseTPCUp1"   ,fUseTPCUp1);
    XML2Bool("UseTPCDown2" ,fUseTPCDown2);
    XML2Bool("UseTPCUp2"   ,fUseTPCUp2);
    // Use FGDs
    XML2Bool("UseFGD1",fUseFGD1);
    XML2Bool("UseFGD2",fUseFGD2);
    // Use SuperFGDs
    XML2Bool("UseSuperFGD1",fUseSuperFGD1);
    XML2Bool("UseSuperFGD2",fUseSuperFGD2);
    // Use WAGASCI 1
    XML2Bool("UseWAGASCI1",fUseWAGASCI1);
    // Use WAGASCI 2
    XML2Bool("UseWAGASCI2",fUseWAGASCI2);
    // Use FGD3D 1
    XML2Bool("UseFGD3D1",fUseFGD3D1);
    // Use FGD3D 2
    XML2Bool("UseFGD3D2",fUseFGD3D2);
    // Use SciFi XZ
    XML2Bool("UseSciFi",fUseSciFi);
    // Use FGDlike XZ
    XML2Bool("UseFGDlike",fUseFGDlike);
    // Use Downstream ToFs
    XML2Bool("UseToF_TopDown" ,fUseToF_TopDown);
    XML2Bool("UseToF_BotDown" ,fUseToF_BotDown);
    XML2Bool("UseToF_RightDown" ,fUseToF_RightDown);
    XML2Bool("UseToF_LeftDown" ,fUseToF_LeftDown);
    XML2Bool("UseToF_BackDown" ,fUseToF_BackDown);
    XML2Bool("UseToF_FrontDown" ,fUseToF_FrontDown);
    // Use ECalP0D ToF
    XML2Bool("UseToF_ECalP0D" ,fUseToF_ECalP0D); 
    // Use Upstream ToFs
    XML2Bool("UseToF_TopUp" ,fUseToF_TopUp);
    XML2Bool("UseToF_BotUp" ,fUseToF_BotUp);
    XML2Bool("UseToF_RightUp" ,fUseToF_RightUp);
    XML2Bool("UseToF_LeftUp" ,fUseToF_LeftUp);
    XML2Bool("UseToF_BackUp" ,fUseToF_BackUp);
    XML2Bool("UseToF_FrontUp" ,fUseToF_FrontUp);
    // Use Magnet
    XML2Bool("UseMagnet",fUseMagnet);
    //XML2Bool("UseSMRD",  fUseSMRD);
    // Use Basket
    XML2Bool("UseBasket",fUseBasket);
    // Use ECals
    XML2Bool("UseDsECal", fUseDsECal);
    XML2Bool("UseBrlECal",fUseBrlECal);
    XML2Bool("UseP0DECal",fUseP0DECal);
    // Use P0D
    XML2Bool("UseP0D",fUseP0D);
    
    // Set invisible detectors 
    XML2Bool("InvisForwTPC1"  ,fInvisForwTPC1);
    XML2Bool("InvisForwTPC2"  ,fInvisForwTPC2);
    XML2Bool("InvisForwTPC3"  ,fInvisForwTPC3);
    XML2Bool("InvisTarget1"  ,fInvisTarget1);
    XML2Bool("InvisTarget2"  ,fInvisTarget2);
    XML2Bool("InvisTPCDown1" ,fInvisTPCDown1);
    XML2Bool("InvisTPCUp1"   ,fInvisTPCUp1);
    XML2Bool("InvisTPCDown2" ,fInvisTPCDown2);
    XML2Bool("InvisTPCUp2"   ,fInvisTPCUp2);
    XML2Bool("InvisFGD1"     ,fInvisFGD1);
    XML2Bool("InvisFGD2"     ,fInvisFGD2);
    XML2Bool("InvisSuperFGD",fInvisSuperFGD);
    XML2Bool("InvisWAGASCI",fInvisWAGASCI);
    XML2Bool("InvisFGD3D",fInvisFGD3D);
    XML2Bool("InvisSciFi",fInvisSciFi);
    XML2Bool("InvisFGDlike",fInvisFGDlike);
    //
    XML2Bool("InvisToF_TopDown"     ,fInvisToF_TopDown);
    XML2Bool("InvisToF_BotDown"     ,fInvisToF_BotDown);
    XML2Bool("InvisToF_RightDown"     ,fInvisToF_RightDown);
    XML2Bool("InvisToF_LeftDown"     ,fInvisToF_LeftDown);
    XML2Bool("InvisToF_BackDown"     ,fInvisToF_BackDown);
    XML2Bool("InvisToF_FrontDown"     ,fInvisToF_FrontDown);
    //
    XML2Bool("InvisToF_ECalP0D"     ,fInvisToF_ECalP0D);
    //
    XML2Bool("InvisToF_TopUp"     ,fInvisToF_TopUp);
    XML2Bool("InvisToF_BotUp"     ,fInvisToF_BotUp);
    XML2Bool("InvisToF_RightUp"     ,fInvisToF_RightUp);
    XML2Bool("InvisToF_LeftUp"     ,fInvisToF_LeftUp);
    XML2Bool("InvisToF_BackUp"     ,fInvisToF_BackUp);
    XML2Bool("InvisToF_FrontUp"     ,fInvisToF_FrontUp);
    //
    XML2Bool("InvisMagnet"   ,fInvisMagnet);
    XML2Bool("InvisSMRD"     ,fInvisSMRD);
    XML2Bool("InvisBasket"   ,fInvisBasket);
    XML2Bool("InvisDsECal"   ,fInvisDsECal);
    XML2Bool("InvisBrlECal"  ,fInvisBrlECal);
    XML2Bool("InvisP0DECal"  ,fInvisP0DECal);
    XML2Bool("InvisP0D"      ,fInvisP0D);

    myfile.close();
  }
  else cout << "Unable to open file"; 
  myfile.close();
}


void ExN02ND280XML::XML2String(string tmp_app,string &dest){
  
  string line;
  ifstream in(fXMLFile);
    
  bool begin_tag = false;
  while (getline(in,line)){
    
    std::string tmp; // strip whitespaces from the beginning
    for (int i = 0; i < line.length(); i++){
      if (line[i] == ' ' && tmp.size() == 0){
      }
      else{
	tmp += line[i];
      }
    }
    
    tmp = TrimSpaces(tmp); // remove spaces
    
    string First = Form("<%s>",tmp_app.c_str());
    string Last = Form("</%s>",tmp_app.c_str());
     
    if ( tmp == First ){
      //if( tmp.find(First) != std::string::npos ){
      begin_tag = true;
      //cout << "Found " << First << endl;
      //TXMLEngine * xml_eng;
      continue;
    }    
    else if ( tmp == Last ){
      //if( tmp.find(Last) != std::string::npos ){
      begin_tag = false;
      //cout << "Found " << Last << endl;
      //break;
    }
    
    if (begin_tag){
      dest = tmp;
      //cout << "dest = " << dest << endl;
    }
    
  } // end while
  if(dest==""){
    std::cout << "Path is : " << tmp_app << std::endl;
    std::cout << "Path not found" << std::endl;
    exit(1);
  }
  
  return;
}

void ExN02ND280XML::XML2Int(string tmp_app,int &dest){
  string dest_string;
  XML2String(tmp_app,dest_string);
  dest = atoi(dest_string.c_str());
  return;
}

void ExN02ND280XML::XML2Double(string tmp_app,double &dest){
  string dest_string;
  XML2String(tmp_app,dest_string);
  dest = atof(dest_string.c_str());
  return;
}

void ExN02ND280XML::XML2Bool(string tmp_app,bool &dest){
  string dest_string;
  XML2String(tmp_app,dest_string);
    
  if(dest_string == "true" || 
     dest_string == "TRUE" || 
     dest_string == "True" || 
     dest_string == "1"    || 
     dest_string == "I") dest=true;
  
  if(dest_string == "false" || 
     dest_string == "FALSE" || 
     dest_string == "False" || 
     dest_string == "0"    || 
     dest_string == "O") dest=false;
  return;
}






// XMLNodePointer_t ExN02ND280XML::FindNode(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Find & return the XMLNodePointer_t at the specified node_path
// //
//   vector<string> node_path_vec = Tokens(node_path,"/");

//   unsigned int ndepth = node_path_vec.size();
//   unsigned int idepth = 0;

//   XMLNodePointer_t curr_node = xml_eng->GetChild(top_node);
//   while(curr_node) {
//     string curr_node_name = xml_eng->GetNodeName(curr_node);
//     if(node_path_vec[idepth] == curr_node_name) {
//         idepth++;
//         if(idepth == ndepth) {
//           return curr_node;
//         } else {
//           curr_node = xml_eng->GetChild(curr_node);
//         }
//     } else {
//        curr_node = xml_eng->GetNext(curr_node);
//     }
//   }
//   return 0;
// }
// //____________________________________________________________________________________________________
// bool ExN02ND280XML::NodeExists(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Find & return the XMLNodePointer_t at the specified node_path
// //
//   vector<string> node_path_vec = Tokens(node_path,"/");

//   unsigned int ndepth = node_path_vec.size();
//   unsigned int idepth = 0;

//   XMLNodePointer_t curr_node = xml_eng->GetChild(top_node);
//   while(curr_node) {
//     string curr_node_name = xml_eng->GetNodeName(curr_node);
//     if(node_path_vec[idepth] == curr_node_name) {
//         idepth++;
//         if(idepth == ndepth) {
//           return true;
//         } else {
//           curr_node = xml_eng->GetChild(curr_node);
//         }
//     } else {
//        curr_node = xml_eng->GetNext(curr_node);
//     }
//   }
//   return false;
// }
// //____________________________________________________________________________________________________
// bool ExN02ND280XML::XML2Bool(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Get the specified node content as a boolean
// //
//   G4cout << "Reading info at XML node node_path: " << node_path << G4endl;

//   XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
//   if(!found_node) {
//     G4cout << "No node was found at input XML node node_path: " << node_path;
//     exit(1);
//   }
//   string content = TrimSpaces(xml_eng->GetNodeContent(found_node));

//   if(content == "true" || 
//      content == "TRUE" || 
//      content == "True" || 
//      content == "1"    || 
//      content == "I") return true;

//   if(content == "false" || 
//      content == "FALSE" || 
//      content == "False" || 
//      content == "0"    || 
//      content == "O") return false;

//    G4cout << "Could not interpret content (" << content 
//        << ") found at node_path: " << node_path << " as a boolean!";
//     exit(1);

//   return false;
// }
// //____________________________________________________________________________________________________
// int ExN02ND280XML::XML2Int(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Get the specified node content as a floating-point number
// //
//   G4cout << "Reading info at XML node node_path: " << node_path << G4endl;

//   XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
//   if(!found_node) {
//     G4cout << "No node was found at input XML node node_path: " << node_path;
//     exit(1);
//   }
//   string content = xml_eng->GetNodeContent(found_node);
//   int value = atoi(content.c_str());
//   return value;
// }
// //____________________________________________________________________________________________________
// double ExN02ND280XML::XML2Dbl(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Get the specified node content as a floating-point number
// //
//   G4cout << "Reading info at XML node node_path: " << node_path << G4endl;

//   XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
//   if(!found_node) {
//     G4cout << "No node was found at input XML node node_path: " << node_path;
//     exit(1);
//   }
//   string content = xml_eng->GetNodeContent(found_node);
//   double value = atof(content.c_str());
//   return value;
// }
// //____________________________________________________________________________________________________
// vector<double> ExN02ND280XML::XML2DblArray(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Get the specified node content as a vector floating-point number
// // The node content should be a comma-separed list of numbers

//   G4cout << "Reading info at XML node node_path: " << node_path << G4endl;

//   XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
//   if(!found_node) {
//     G4cout << "No node was found at input XML node node_path: " << node_path;
//     exit(1);
//   }
//   vector<double> array;
//   const char * nc = xml_eng->GetNodeContent(found_node);
//   if (not nc) return array;
  
//   string content = TrimSpaces(nc);
  
//   vector<string> str_tokens = Tokens(content, ",");
//   vector<string>::const_iterator str_tokens_iter = str_tokens.begin();
//   for( ; str_tokens_iter != str_tokens.end(); ++str_tokens_iter) {
//      string token = TrimSpaces(*str_tokens_iter);
//      if (not token.size() ) continue;
//      array.push_back( atof(token.c_str()) );
//   }
  
//   return array;
// }
// //____________________________________________________________________________________________________
// string ExN02ND280XML::XML2String(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Get the specified node content as a floating-point number
// //
//   G4cout << "Reading info at XML node node_path: " << node_path << G4endl;
 
//   XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
//   if(!found_node) {
//     G4cout << "No node was found at input XML node node_path: " << node_path;
//     exit(1);
//   }
//   const char * nc = xml_eng->GetNodeContent(found_node);
//   if (not nc) return string("");
//   string content(nc);
//   return TrimSpaces(content);
// }
// //____________________________________________________________________________________________________
// pair<double,double> ExN02ND280XML::XML2DblDblPair(
//   TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path)
// {
// // Get the specified node content as a pair of floating-point numbers
// // The node content should be 2 comma-separed numbers

//   G4cout << "Reading info at XML node node_path: " << node_path << G4endl;

//   XMLNodePointer_t found_node = FindNode(xml_eng,top_node,node_path);
//   if(!found_node) {
//     G4cout << "No node was found at input XML node node_path: " << node_path;
//     exit(1);
//   }
//   pair<double,double> pair;
//   const char * nc = xml_eng->GetNodeContent(found_node);
//   if (not nc) return pair;
  
//   string content = TrimSpaces(nc);
  
//   vector<string> str_tokens = Tokens(content, ",");
//   if(str_tokens.size() != 2) {
//     G4cout << "XML2DblDblPair cannot get exactly two values from: " << content;
//     exit(1);
//   }
  
//   pair.first  = atof(str_tokens[0].c_str());
//   pair.second = atof(str_tokens[1].c_str());
//   return pair;
// }

// vector<string> ExN02ND280XML::Tokens (string input, string delimiter)
// {
// // split a string of 'delimiter' separated values and return each string
// // part as a vector<string> element
//   vector<string> tokens;

//   while(input.find_first_of(delimiter) < input.length()) {

//     tokens.push_back( input.substr(0, input.find_first_of(delimiter)) );
//     input = input.substr(input.find_first_of(delimiter)+1, input.length());
//   }
//   tokens.push_back(input);
//   return tokens;
// }

string ExN02ND280XML::TrimSpaces(string input)
{
  if( input.find_first_not_of(" \n") != 0)
    input.erase( 0,  input.find_first_not_of(" \n")  );

  if( input.find_last_not_of(" \n") != input.length() )
    input.erase( input.find_last_not_of(" \n")+1, input.length() );

  return input;
}
