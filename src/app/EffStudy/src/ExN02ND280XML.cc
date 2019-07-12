
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

  fXMLFile = "";
  fIsRandomSeed = false;
  fStoreGeometry = false;
  fGenerType = "";
  fPathFiles = "";
  fGenerTreeName = "";
  fGenerFileName = "";
  fGenerStepEvent = 0;

  fForwTPCdefault = false;
  fForwTPCPos1_X = 0.;
  fForwTPCPos1_Y = 0.;
  fForwTPCPos1_Z = 0.;
  fForwTPCPos2_X = 0.;
  fForwTPCPos2_Y = 0.;
  fForwTPCPos2_Z = 0.;
  fForwTPCPos3_X = 0.;
  fForwTPCPos3_Y = 0.;
  fForwTPCPos3_Z = 0.;

  fTargetdefault1 = false;
  fTargetMaterial1 = "";
  fTargetlength1 = 0.;
  fTargetwidth1 = 0.;
  fTargetheight1 = 0.;
  fTargetPos1_X = 0.;
  fTargetPos1_Y = 0.;
  fTargetPos1_Z = 0.;

  fTargetdefault2 = false;
  fTargetMaterial2 = "";
  fTargetlength2 = 0.;
  fTargetwidth2 = 0.;
  fTargetheight2 = 0.;
  fTargetPos2_X = 0.;
  fTargetPos2_Y = 0.;
  fTargetPos2_Z = 0.;

  fFGDdefault1 = false;
  fFGDMaterial1 = "";
  fFGDlength1 = 0.;
  fFGDwidth1 = 0.;
  fFGDheight1 = 0.;
  fFGDPos1_X = 0.;
  fFGDPos1_Y = 0.;
  fFGDPos1_Z = 0.;

  fFGDdefault2 = false;
  fFGDMaterial2 = "";
  fFGDlength2 = 0.;
  fFGDwidth2 = 0.;
  fFGDheight2 = 0.;
  fFGDPos2_X = 0.;
  fFGDPos2_Y = 0.;
  fFGDPos2_Z = 0.;
  

 fSuperFGDCubeEdge1 = 0.;
 fSuperFGDCubeNum1_X = 0;
 fSuperFGDCubeNum1_Y = 0;
 fSuperFGDCubeNum1_Z = 0;
 fSuperFGDPos1_X = 0.;
 fSuperFGDPos1_Y = 0.;
 fSuperFGDPos1_Z = 0.;

 fSuperFGDCubeEdge2 = 0.;
 fSuperFGDCubeNum2_X = 0;
 fSuperFGDCubeNum2_Y = 0;
 fSuperFGDCubeNum2_Z = 0;
 fSuperFGDPos2_X = 0.;
 fSuperFGDPos2_Y = 0.;
 fSuperFGDPos2_Z = 0.;


 //CFBox parameters
 fCFBoxCFRPThickness = 0;
 fCFBoxAIREXThickness = 0;
 fCFBoxPlasticThickness = 0;
 fCFBoxInnerClearanceThickness = 0;
 fCFBoxOuterClearanceThickness = 0;


 //

 //FlatCable parameters
 fFlatCableThickness = 0;
 fFlatCableN = 0;

 //

 
 fHATPCFCName = "";

 fHATPCUpPos_X = 0.;
 fHATPCUpPos_Y = 0.;
 fHATPCUpPos_Z = 0.;

 fHATPCDownPos_X = 0.;
 fHATPCDownPos_Y = 0.;
 fHATPCDownPos_Z = 0.;

 fHATPCDownWidth = 0.;
 fHATPCDownHeight = 0.;
 fHATPCDownLength = 0.;
 fHATPCDownDriftWidth = 0.;
 fHATPCDownDriftHeight = 0.;
 fHATPCDownDriftLength = 0.;
 fHATPCDownCathodeThickness = 0.;
 fHATPCDownInnerBoxWall = 0.;
 fHATPCDownOuterBoxWall = 0.;
 fHATPCDownSteppingLimit = 0.;
 fActiveHATPCDownVerticalOffset = 0.;

  fFGD3Dlength1 = 0.;
  fFGD3Dwidth1 = 0.;
  fFGD3Dheight1 = 0.;
  fFGD3DPos1_X = 0.;
  fFGD3DPos1_Y = 0.;
  fFGD3DPos1_Z = 0.;

  fFGD3Dlength2 = 0.;
  fFGD3Dwidth2 = 0.;
  fFGD3Dheight2 = 0.;
  fFGD3DPos2_X = 0.;
  fFGD3DPos2_Y = 0.;
  fFGD3DPos2_Z = 0.;

  fFGDlikeBarEdge = 0.;
  fFGDlikeNum_AlongX = 0;
  fFGDlikeNum_AlongZ = 0;
  fFGDlikeNum_Layer = 0;
  fFGDlikePos_X = 0.;
  fFGDlikePos_Y = 0.;
  fFGDlikePos_Z = 0.;

  fUseDegrader    = 0.;
  fDegraderlength = 0.;
  fDegraderwidth  = 0.;
  fDegraderheight = 0.;
  fDegraderPos_X  = 0.;
  fDegraderPos_Y  = 0.;
  fDegraderPos_Z  = 0.;

  // ToF Upstream

  fToFdefault_TopUp = false;
  fToFPlaneXYNum_TopUp = 0;
  fToFBarHorizlength_TopUp = 0.;
  fToFBarVertlength_TopUp = 0.;
  fToFBarwidth_TopUp = 0.;
  fToFBarheight_TopUp = 0.;
  fToFBarlength_TopUp = 0.;
  fToFRotX_TopUp = 0.;
  fToFRotY_TopUp = 0.;
  fToFRotZ_TopUp = 0.;
  fToFPosX_TopUp = 0.;
  fToFPosY_TopUp = 0.;
  fToFPosZ_TopUp = 0.;

  fToFdefault_BotUp = false;
  fToFPlaneXYNum_BotUp = 0;
  fToFBarHorizlength_BotUp = 0.;
  fToFBarVertlength_BotUp = 0.;
  fToFBarwidth_BotUp = 0.;
  fToFBarheight_BotUp = 0.;
  fToFBarlength_BotUp = 0.;
  fToFRotX_BotUp = 0.;
  fToFRotY_BotUp = 0.;
  fToFRotZ_BotUp = 0.;
  fToFPosX_BotUp = 0.;
  fToFPosY_BotUp = 0.;
  fToFPosZ_BotUp = 0.;

  fToFdefault_RightUp = false;
  fToFPlaneXYNum_RightUp = 0;
  fToFBarHorizlength_RightUp = 0.;
  fToFBarVertlength_RightUp = 0.;
  fToFBarwidth_RightUp = 0.;
  fToFBarheight_RightUp = 0.;
  fToFBarlength_RightUp = 0.;
  fToFRotX_RightUp = 0.;
  fToFRotY_RightUp = 0.;
  fToFRotZ_RightUp = 0.;
  fToFPosX_RightUp = 0.;
  fToFPosY_RightUp = 0.;
  fToFPosZ_RightUp = 0.;

  fToFdefault_LeftUp = false;
  fToFPlaneXYNum_LeftUp = 0;
  fToFBarHorizlength_LeftUp = 0.;
  fToFBarVertlength_LeftUp = 0.;
  fToFBarwidth_LeftUp = 0.;
  fToFBarheight_LeftUp = 0.;
  fToFBarlength_LeftUp = 0.;
  fToFRotX_LeftUp = 0.;
  fToFRotY_LeftUp = 0.;
  fToFRotZ_LeftUp = 0.;
  fToFPosX_LeftUp = 0.;
  fToFPosY_LeftUp = 0.;
  fToFPosZ_LeftUp = 0.;

  fToFdefault_BackUp = false;
  fToFPlaneXYNum_BackUp = 0;
  fToFBarHorizlength_BackUp = 0.;
  fToFBarVertlength_BackUp = 0.;
  fToFBarwidth_BackUp = 0.;
  fToFBarheight_BackUp = 0.;
  fToFBarlength_BackUp = 0.;
  fToFRotX_BackUp = 0.;
  fToFRotY_BackUp = 0.;
  fToFRotZ_BackUp = 0.;
  fToFPosX_BackUp = 0.;
  fToFPosY_BackUp = 0.;
  fToFPosZ_BackUp = 0.;

  fToFdefault_FrontUp = false;
  fToFPlaneXYNum_FrontUp = 0;
  fToFBarHorizlength_FrontUp = 0.;
  fToFBarVertlength_FrontUp = 0.;
  fToFBarwidth_FrontUp = 0.;
  fToFBarheight_FrontUp = 0.;
  fToFBarlength_FrontUp = 0.;
  fToFRotX_FrontUp = 0.;
  fToFRotY_FrontUp = 0.;
  fToFRotZ_FrontUp = 0.;
  fToFPosX_FrontUp = 0.;
  fToFPosY_FrontUp = 0.;
  fToFPosZ_FrontUp = 0.;

  //
  
  fUseForwTPC1 = false;
  fUseForwTPC2 = false;
  fUseForwTPC3 = false;
  fUseTarget1 = false;
  fUseTarget2 = false;
  fUseFGD1 = false;
  fUseFGD2 = false;
  fUseSuperFGD1 = false;
  fUseSuperFGD2 = false;

  fUseFGD3D1 = false;
  fUseFGD3D2 = false;
  fUseFGDlike = false;
  fUseMagnet = false;
  fUseToF_TopUp = false;
  fUseToF_BotUp = false;
  fUseToF_RightUp = false;
  fUseToF_LeftUp = false;
  fUseToF_BackUp = false;
  fUseToF_FrontUp = false;
  // fUseSMRD = false;
  fUseBasket = false;
  fUseDsECal = false;
  fUseBrlECal = false;
  fUseP0DECal = false;
  fUseP0D = false;

  // CF Box
  fUseCFBox = false;
  //

  // PCB
  fUsePCB = false;
  //

  // PCB
  fUseFlatCable = false;
  //


  fInvisForwTPC1 = false;
  fInvisForwTPC2 = false;
  fInvisForwTPC3 = false;
  fInvisTarget1 = false;
  fInvisTarget2 = false;
  fInvisFGD1 = false;
  fInvisFGD2 = false;
  fInvisSuperFGD = false;
  fInvisFGD3D = false;
  fInvisFGDlike = false;
  fInvisToF_TopUp = false;
  fInvisToF_BotUp = false;
  fInvisToF_RightUp = false;
  fInvisToF_LeftUp = false;
  fInvisToF_BackUp = false;
  fInvisToF_FrontUp = false;
  fInvisMagnet = false;
  fInvisSMRD = false;
  fInvisBasket = false;
  fInvisDsECal = false;
  fInvisBrlECal = false;
  fInvisP0DECal = false;
  fInvisP0D = false;

}

void ExN02ND280XML::SetInputs(){

  //fXMLFile = ""; // already initialized in the constructor
  fIsRandomSeed = false;
  fStoreGeometry = false;
  fGenerType = "";
  fPathFiles = "";
  fGenerTreeName = "";
  fGenerFileName = "";
  fGenerStepEvent = 0;

  fForwTPCdefault = false;
  fForwTPCPos1_X = 0.;
  fForwTPCPos1_Y = 0.;
  fForwTPCPos1_Z = 0.;
  fForwTPCPos2_X = 0.;
  fForwTPCPos2_Y = 0.;
  fForwTPCPos2_Z = 0.;
  fForwTPCPos3_X = 0.;
  fForwTPCPos3_Y = 0.;
  fForwTPCPos3_Z = 0.;

  fTargetdefault1 = false;
  fTargetMaterial1 = "";
  fTargetlength1 = 0.;
  fTargetwidth1 = 0.;
  fTargetheight1 = 0.;
  fTargetPos1_X = 0.;
  fTargetPos1_Y = 0.;
  fTargetPos1_Z = 0.;

  fTargetdefault2 = false;
  fTargetMaterial2 = "";
  fTargetlength2 = 0.;
  fTargetwidth2 = 0.;
  fTargetheight2 = 0.;
  fTargetPos2_X = 0.;
  fTargetPos2_Y = 0.;
  fTargetPos2_Z = 0.;

  fFGDdefault1 = false;
  fFGDMaterial1 = "";
  fFGDlength1 = 0.;
  fFGDwidth1 = 0.;
  fFGDheight1 = 0.;
  fFGDPos1_X = 0.;
  fFGDPos1_Y = 0.;
  fFGDPos1_Z = 0.;

  fFGDdefault2 = false;
  fFGDMaterial2 = "";
  fFGDlength2 = 0.;
  fFGDwidth2 = 0.;
  fFGDheight2 = 0.;
  fFGDPos2_X = 0.;
  fFGDPos2_Y = 0.;
  fFGDPos2_Z = 0.;
  
 fSuperFGDCubeEdge1 = 0.;
 fSuperFGDCubeNum1_X = 0;
 fSuperFGDCubeNum1_Y = 0;
 fSuperFGDCubeNum1_Z = 0;
 fSuperFGDPos1_X = 0.;
 fSuperFGDPos1_Y = 0.;
 fSuperFGDPos1_Z = 0.;

 fSuperFGDCubeEdge2 = 0.;
 fSuperFGDCubeNum2_X = 0;
 fSuperFGDCubeNum2_Y = 0;
 fSuperFGDCubeNum2_Z = 0;
 fSuperFGDPos2_X = 0.;
 fSuperFGDPos2_Y = 0.;
 fSuperFGDPos2_Z = 0.;

  fFGD3Dlength1 = 0.;
  fFGD3Dwidth1 = 0.;
  fFGD3Dheight1 = 0.;
  fFGD3DPos1_X = 0.;
  fFGD3DPos1_Y = 0.;
  fFGD3DPos1_Z = 0.;

  fFGD3Dlength2 = 0.;
  fFGD3Dwidth2 = 0.;
  fFGD3Dheight2 = 0.;
  fFGD3DPos2_X = 0.;
  fFGD3DPos2_Y = 0.;
  fFGD3DPos2_Z = 0.;

  fFGDlikeBarEdge = 0.;
  fFGDlikeNum_AlongX = 0;
  fFGDlikeNum_AlongZ = 0;
  fFGDlikeNum_Layer = 0;
  fFGDlikePos_X = 0.;
  fFGDlikePos_Y = 0.;
  fFGDlikePos_Z = 0.;

  // ToF Upstream

  fToFdefault_TopUp = false;
  fToFPlaneXYNum_TopUp = 0;
  fToFBarHorizlength_TopUp = 0.;
  fToFBarVertlength_TopUp = 0.;
  fToFBarwidth_TopUp = 0.;
  fToFBarheight_TopUp = 0.;
  fToFBarlength_TopUp = 0.;
  fToFRotX_TopUp = 0.;
  fToFRotY_TopUp = 0.;
  fToFRotZ_TopUp = 0.;
  fToFPosX_TopUp = 0.;
  fToFPosY_TopUp = 0.;
  fToFPosZ_TopUp = 0.;

  fToFdefault_BotUp = false;
  fToFPlaneXYNum_BotUp = 0;
  fToFBarHorizlength_BotUp = 0.;
  fToFBarVertlength_BotUp = 0.;
  fToFBarwidth_BotUp = 0.;
  fToFBarheight_BotUp = 0.;
  fToFBarlength_BotUp = 0.;
  fToFRotX_BotUp = 0.;
  fToFRotY_BotUp = 0.;
  fToFRotZ_BotUp = 0.;
  fToFPosX_BotUp = 0.;
  fToFPosY_BotUp = 0.;
  fToFPosZ_BotUp = 0.;

  fToFdefault_RightUp = false;
  fToFPlaneXYNum_RightUp = 0;
  fToFBarHorizlength_RightUp = 0.;
  fToFBarVertlength_RightUp = 0.;
  fToFBarwidth_RightUp = 0.;
  fToFBarheight_RightUp = 0.;
  fToFBarlength_RightUp = 0.;
  fToFRotX_RightUp = 0.;
  fToFRotY_RightUp = 0.;
  fToFRotZ_RightUp = 0.;
  fToFPosX_RightUp = 0.;
  fToFPosY_RightUp = 0.;
  fToFPosZ_RightUp = 0.;

  fToFdefault_LeftUp = false;
  fToFPlaneXYNum_LeftUp = 0;
  fToFBarHorizlength_LeftUp = 0.;
  fToFBarVertlength_LeftUp = 0.;
  fToFBarwidth_LeftUp = 0.;
  fToFBarheight_LeftUp = 0.;
  fToFBarlength_LeftUp = 0.;
  fToFRotX_LeftUp = 0.;
  fToFRotY_LeftUp = 0.;
  fToFRotZ_LeftUp = 0.;
  fToFPosX_LeftUp = 0.;
  fToFPosY_LeftUp = 0.;
  fToFPosZ_LeftUp = 0.;

  fToFdefault_BackUp = false;
  fToFPlaneXYNum_BackUp = 0;
  fToFBarHorizlength_BackUp = 0.;
  fToFBarVertlength_BackUp = 0.;
  fToFBarwidth_BackUp = 0.;
  fToFBarheight_BackUp = 0.;
  fToFBarlength_BackUp = 0.;
  fToFRotX_BackUp = 0.;
  fToFRotY_BackUp = 0.;
  fToFRotZ_BackUp = 0.;
  fToFPosX_BackUp = 0.;
  fToFPosY_BackUp = 0.;
  fToFPosZ_BackUp = 0.;

  fToFdefault_FrontUp = false;
  fToFPlaneXYNum_FrontUp = 0;
  fToFBarHorizlength_FrontUp = 0.;
  fToFBarVertlength_FrontUp = 0.;
  fToFBarwidth_FrontUp = 0.;
  fToFBarheight_FrontUp = 0.;
  fToFBarlength_FrontUp = 0.;
  fToFRotX_FrontUp = 0.;
  fToFRotY_FrontUp = 0.;
  fToFRotZ_FrontUp = 0.;
  fToFPosX_FrontUp = 0.;
  fToFPosY_FrontUp = 0.;
  fToFPosZ_FrontUp = 0.;

  //
  
  fUseForwTPC1 = false;
  fUseForwTPC2 = false;
  fUseForwTPC3 = false;
  fUseTarget1 = false;
  fUseTarget2 = false;
  fUseFGD1 = false;
  fUseFGD2 = false;
  fUseSuperFGD1 = false;
  fUseSuperFGD2 = false;
  fUseFGD3D1 = false;
  fUseFGD3D2 = false;
  fUseFGDlike = false;
  fUseMagnet = false;
  fUseToF_TopUp = false;
  fUseToF_BotUp = false;
  fUseToF_RightUp = false;
  fUseToF_LeftUp = false;
  fUseToF_BackUp = false;
  fUseToF_FrontUp = false;
  // fUseSMRD = false;
  fUseBasket = false;
  fUseDsECal = false;
  fUseBrlECal = false;
  fUseP0DECal = false;
  fUseP0D = false;
  
  fInvisForwTPC1 = false;
  fInvisForwTPC2 = false;
  fInvisForwTPC3 = false;
  fInvisTarget1 = false;
  fInvisTarget2 = false;
  fInvisFGD1 = false;
  fInvisFGD2 = false;
  fInvisSuperFGD = false;
  fInvisFGD3D = false;
  fInvisFGDlike = false;
  fInvisToF_TopUp = false;
  fInvisToF_BotUp = false;
  fInvisToF_RightUp = false;
  fInvisToF_LeftUp = false;
  fInvisToF_BackUp = false;
  fInvisToF_FrontUp = false;
  fInvisMagnet = false;
  fInvisSMRD = false;
  fInvisBasket = false;
  fInvisDsECal = false;
  fInvisBrlECal = false;
  fInvisP0DECal = false;
  fInvisP0D = false;




  
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


    //CFBox
    XML2Double("CFBoxCFRPThickness", fCFBoxCFRPThickness);
    XML2Double("CFBoxAIREXThickness", fCFBoxAIREXThickness);
    XML2Double("CFBoxPlasticThickness", fCFBoxPlasticThickness);
    XML2Double("CFBoxInnerClearanceThickness", fCFBoxInnerClearanceThickness);
    XML2Double("CFBoxOuterClearanceThickness", fCFBoxOuterClearanceThickness);
    //
    //FlatCable
    XML2Double("FlatCableThickness", fFlatCableThickness);
    XML2Int("FlatCableN", fFlatCableN);
    
    
    //HATPC Up

    XML2String("HATPCFCName",fHATPCFCName);

    XML2Double("HATPCUpPos_X" ,fHATPCUpPos_X);
    XML2Double("HATPCUpPos_Y" ,fHATPCUpPos_Y);
    XML2Double("HATPCUpPos_Z" ,fHATPCUpPos_Z);
    XML2Double("HATPCDownPos_X" ,fHATPCDownPos_X);
    XML2Double("HATPCDownPos_Y" ,fHATPCDownPos_Y);
    XML2Double("HATPCDownPos_Z" ,fHATPCDownPos_Z);

    XML2Double("HATPCUpWidth", fHATPCUpWidth);
    XML2Double("HATPCUpHeight", fHATPCUpHeight);
    XML2Double("HATPCUpLength", fHATPCUpLength);
    XML2Double("HATPCUpDriftWidth", fHATPCUpDriftWidth);
    XML2Double("HATPCUpDriftHeight", fHATPCUpDriftHeight);
    XML2Double("HATPCUpDriftLength", fHATPCUpDriftLength);
    XML2Double("HATPCUpCathodeThickness", fHATPCUpCathodeThickness);
    XML2Double("HATPCUpInnerBoxWall", fHATPCUpInnerBoxWall);
    XML2Double("HATPCUpOuterBoxWall", fHATPCUpOuterBoxWall);
    XML2Double("HATPCUpSteppingLimit", fHATPCUpSteppingLimit);
    XML2Double("ActiveHATPCUpVerticalOffset", fActiveHATPCUpVerticalOffset);

    //HATPC Down
    XML2Double("HATPCDownWidth", fHATPCDownWidth);
    XML2Double("HATPCDownHeight", fHATPCDownHeight);
    XML2Double("HATPCDownLength", fHATPCDownLength);
    XML2Double("HATPCDownDriftWidth", fHATPCDownDriftWidth);
    XML2Double("HATPCDownDriftHeight", fHATPCDownDriftHeight);
    XML2Double("HATPCDownDriftLength", fHATPCDownDriftLength);
    XML2Double("HATPCDownCathodeThickness", fHATPCDownCathodeThickness);
    XML2Double("HATPCDownInnerBoxWall", fHATPCDownInnerBoxWall);
    XML2Double("HATPCDownOuterBoxWall", fHATPCDownOuterBoxWall);
    XML2Double("HATPCDownSteppingLimit", fHATPCDownSteppingLimit);
    XML2Double("ActiveHATPCDownVerticalOffset", fActiveHATPCDownVerticalOffset);

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

    // FGDlike XZ
    XML2Double("FGDlikeBarEdge",fFGDlikeBarEdge);
    XML2Int("FGDlikeNum_AlongX"  ,fFGDlikeNum_AlongX);
    XML2Int("FGDlikeNum_AlongZ"  ,fFGDlikeNum_AlongZ);
    XML2Int("FGDlikeNum_Layer"   ,fFGDlikeNum_Layer);
    XML2Double("FGDlikePos_X"    ,fFGDlikePos_X);
    XML2Double("FGDlikePos_Y"    ,fFGDlikePos_Y);
    XML2Double("FGDlikePos_Z"    ,fFGDlikePos_Z);

    // ToF Top Upstream 
    XML2Bool("ToFdefault_TopUp"      ,fToFdefault_TopUp);
    XML2Int("ToFPlaneXYNum_TopUp"    ,fToFPlaneXYNum_TopUp);
    XML2Double("ToFBarwidth_TopUp"      ,fToFBarwidth_TopUp);
    XML2Double("ToFBarheight_TopUp"     ,fToFBarheight_TopUp);
    XML2Double("ToFBarlength_TopUp"     ,fToFBarlength_TopUp);
    XML2Double("ToFRotX_TopUp"         ,fToFRotX_TopUp);
    XML2Double("ToFRotY_TopUp"         ,fToFRotY_TopUp);
    XML2Double("ToFRotZ_TopUp"         ,fToFRotZ_TopUp);
    XML2Double("ToFPosX_TopUp"         ,fToFPosX_TopUp);
    XML2Double("ToFPosY_TopUp"         ,fToFPosY_TopUp);
    XML2Double("ToFPosZ_TopUp"         ,fToFPosZ_TopUp);

    // ToF Bottom Upstream 
    XML2Bool("ToFdefault_BotUp"      ,fToFdefault_BotUp);
    XML2Int("ToFPlaneXYNum_BotUp"    ,fToFPlaneXYNum_BotUp);
    XML2Double("ToFBarwidth_BotUp"      ,fToFBarwidth_BotUp);
    XML2Double("ToFBarheight_BotUp"     ,fToFBarheight_BotUp);
    XML2Double("ToFBarlength_BotUp"     ,fToFBarlength_BotUp);
    XML2Double("ToFRotX_BotUp"         ,fToFRotX_BotUp);
    XML2Double("ToFRotY_BotUp"         ,fToFRotY_BotUp);
    XML2Double("ToFRotZ_BotUp"         ,fToFRotZ_BotUp);
    XML2Double("ToFPosX_BotUp"         ,fToFPosX_BotUp);
    XML2Double("ToFPosY_BotUp"         ,fToFPosY_BotUp);
    XML2Double("ToFPosZ_BotUp"         ,fToFPosZ_BotUp);

    // ToF Right Upstream 
    XML2Bool("ToFdefault_RightUp"      ,fToFdefault_RightUp);
    XML2Int("ToFPlaneXYNum_RightUp"    ,fToFPlaneXYNum_RightUp);
    XML2Double("ToFBarwidth_RightUp"      ,fToFBarwidth_RightUp);
    XML2Double("ToFBarheight_RightUp"     ,fToFBarheight_RightUp);
    XML2Double("ToFBarlength_RightUp"     ,fToFBarlength_RightUp);
    XML2Double("ToFRotX_RightUp"         ,fToFRotX_RightUp);
    XML2Double("ToFRotY_RightUp"         ,fToFRotY_RightUp);
    XML2Double("ToFRotZ_RightUp"         ,fToFRotZ_RightUp);
    XML2Double("ToFPosX_RightUp"         ,fToFPosX_RightUp);
    XML2Double("ToFPosY_RightUp"         ,fToFPosY_RightUp);
    XML2Double("ToFPosZ_RightUp"         ,fToFPosZ_RightUp);

    // ToF Left Upstream 
    XML2Bool("ToFdefault_LeftUp"      ,fToFdefault_LeftUp);
    XML2Int("ToFPlaneXYNum_LeftUp"    ,fToFPlaneXYNum_LeftUp);
    XML2Double("ToFBarwidth_LeftUp"      ,fToFBarwidth_LeftUp);
    XML2Double("ToFBarheight_LeftUp"     ,fToFBarheight_LeftUp);
    XML2Double("ToFBarlength_LeftUp"     ,fToFBarlength_LeftUp);
    XML2Double("ToFRotX_LeftUp"         ,fToFRotX_LeftUp);
    XML2Double("ToFRotY_LeftUp"         ,fToFRotY_LeftUp);
    XML2Double("ToFRotZ_LeftUp"         ,fToFRotZ_LeftUp);
    XML2Double("ToFPosX_LeftUp"         ,fToFPosX_LeftUp);
    XML2Double("ToFPosY_LeftUp"         ,fToFPosY_LeftUp);
    XML2Double("ToFPosZ_LeftUp"         ,fToFPosZ_LeftUp);

    // ToF Back Upstream 
    XML2Bool("ToFdefault_BackUp"      ,fToFdefault_BackUp);
    XML2Int("ToFPlaneXYNum_BackUp"    ,fToFPlaneXYNum_BackUp);
    XML2Double("ToFBarwidth_BackUp"      ,fToFBarwidth_BackUp);
    XML2Double("ToFBarheight_BackUp"     ,fToFBarheight_BackUp);
    XML2Double("ToFBarlength_BackUp"     ,fToFBarlength_BackUp);
    XML2Double("ToFRotX_BackUp"         ,fToFRotX_BackUp);
    XML2Double("ToFRotY_BackUp"         ,fToFRotY_BackUp);
    XML2Double("ToFRotZ_BackUp"         ,fToFRotZ_BackUp);
    XML2Double("ToFPosX_BackUp"         ,fToFPosX_BackUp);
    XML2Double("ToFPosY_BackUp"         ,fToFPosY_BackUp);
    XML2Double("ToFPosZ_BackUp"         ,fToFPosZ_BackUp);

    // ToF Front Upstream 
    XML2Bool("ToFdefault_FrontUp"      ,fToFdefault_FrontUp);
    XML2Int("ToFPlaneXYNum_FrontUp"    ,fToFPlaneXYNum_FrontUp);
    XML2Double("ToFBarwidth_FrontUp"      ,fToFBarwidth_FrontUp);
    XML2Double("ToFBarheight_FrontUp"     ,fToFBarheight_FrontUp);
    XML2Double("ToFBarlength_FrontUp"     ,fToFBarlength_FrontUp);
    XML2Double("ToFRotX_FrontUp"         ,fToFRotX_FrontUp);
    XML2Double("ToFRotY_FrontUp"         ,fToFRotY_FrontUp);
    XML2Double("ToFRotZ_FrontUp"         ,fToFRotZ_FrontUp);
    XML2Double("ToFPosX_FrontUp"         ,fToFPosX_FrontUp);
    XML2Double("ToFPosY_FrontUp"         ,fToFPosY_FrontUp);
    XML2Double("ToFPosZ_FrontUp"         ,fToFPosZ_FrontUp);


    //
    // Set detectors used in the simulation
    //

    XML2Bool("UseHATPCUp",fUseHATPCUp);
    XML2Bool("UseHATPCDown",fUseHATPCDown);
    // Use CFBox
    XML2Bool("UseCFBox" ,fUseCFBox);
   
     // Use PCB
    XML2Bool("UsePCB", fUsePCB);

    // Use PCB
    XML2Bool("UseFlatCable", fUseFlatCable);


    // Use forward TPCs
    XML2Bool("UseForwTPC1" ,fUseForwTPC1);
    XML2Bool("UseForwTPC2" ,fUseForwTPC2);
    XML2Bool("UseForwTPC3" ,fUseForwTPC3);
    // Use horizontal targets
    XML2Bool("UseTarget1" ,fUseTarget1);
    XML2Bool("UseTarget2" ,fUseTarget2);
    // Use FGDs
    XML2Bool("UseFGD1",fUseFGD1);
    XML2Bool("UseFGD2",fUseFGD2);
    // Use SuperFGDs
    XML2Bool("UseSuperFGD1",fUseSuperFGD1);
    XML2Bool("UseSuperFGD2",fUseSuperFGD2);
    // Use FGD3D 1
    XML2Bool("UseFGD3D1",fUseFGD3D1);
    // Use FGD3D 2
    XML2Bool("UseFGD3D2",fUseFGD3D2);
    // Use FGDlike XZ
    XML2Bool("UseFGDlike",fUseFGDlike);
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
    
    XML2Bool("UseDegrader",fUseDegrader);
    XML2Double("DegraderLength", fDegraderlength);
    XML2Double("DegraderWidth", fDegraderwidth);
    XML2Double("DegraderHeight", fDegraderheight);
    XML2Double("DegraderPositionX", fDegraderPos_X);
    XML2Double("DegraderPositionY", fDegraderPos_Y);
    XML2Double("DegraderPositionZ", fDegraderPos_Z);
    
    // Set invisible detectors 
    XML2Bool("InvisForwTPC1"  ,fInvisForwTPC1);
    XML2Bool("InvisForwTPC2"  ,fInvisForwTPC2);
    XML2Bool("InvisForwTPC3"  ,fInvisForwTPC3);
    XML2Bool("InvisTarget1"  ,fInvisTarget1);
    XML2Bool("InvisTarget2"  ,fInvisTarget2);
    XML2Bool("InvisFGD1"     ,fInvisFGD1);
    XML2Bool("InvisFGD2"     ,fInvisFGD2);
    XML2Bool("InvisSuperFGD",fInvisSuperFGD);
    XML2Bool("InvisFGD3D",fInvisFGD3D);
    XML2Bool("InvisFGDlike",fInvisFGDlike);
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
    for (uint i = 0; i < line.length(); i++){
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
    const char *msg = "Path to the GENIE file is empty";
    const char *origin = "ExN02ND280XML::XML2String";
    const char *code = "if(dest==""){";
    G4Exception(origin,code,FatalException,msg);
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

  //if(dest) cout << "dest = true" << endl;
  //else cout << "dest = false" << endl;
  //if(dest) exit(1);

  if(dest_string == "true" || 
     dest_string == "TRUE" || 
     dest_string == "True" || 
     dest_string == "1"    || 
     dest_string == "I") dest=true;
  
  else if(dest_string == "false" || 
	  dest_string == "FALSE" || 
	  dest_string == "False" || 
	  dest_string == "0"    || 
	  dest_string == "O") dest=false;
  
  else{
    G4ExceptionDescription msg;
    msg << "Use either True or False" << G4endl;
    G4Exception("ExN02ND280XML::XML2Bool(",
		"MyCode0002",FatalException, msg);
  }

  return;
}


string ExN02ND280XML::TrimSpaces(string input)
{
  if( input.find_first_not_of(" \n") != 0)
    input.erase( 0,  input.find_first_not_of(" \n")  );

  if( input.find_last_not_of(" \n") != input.length() )
    input.erase( input.find_last_not_of(" \n")+1, input.length() );

  return input;
}
