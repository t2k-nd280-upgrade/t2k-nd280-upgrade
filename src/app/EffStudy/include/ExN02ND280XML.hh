// 
// Class to get inputs other than GEANT4 ones
// 
// 4/7/16 Davide Sgalaberna
//

#ifndef ExN02ND280XML_h
#define ExN02ND280XML_h 1

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

#include <TTree.h>
#include <TFile.h>
#include <TBits.h>
#include <TObjString.h>
#include <vector>
#include <TXMLEngine.h>

using namespace std;

class ExN02ND280XML
{
public :

  ExN02ND280XML(const G4String inxmlname="");
  ~ExN02ND280XML();
    
  G4String GetXMLFileName()     {return fXMLFile;};
  
  G4bool   GetXMLIsRandomSeed(){return fIsRandomSeed;};
  G4bool   GetXMLStoreGeometry(){return fStoreGeometry;};
  G4String GetXMLGenerTypeName(){return fGenerType;};
  G4String GetXMLPathFiles()    {return fPathFiles;};
  G4String GetXMLGenerTreeName(){return fGenerTreeName;};
  G4String GetXMLGenerFileName(){return fGenerFileName;};
  G4int    GetXMLStepEvent()    {return fGenerStepEvent;};
  
  G4bool   GetXMLForwTPCdefault(){return fForwTPCdefault;};
  G4double GetXMLForwTPCPos1_X()  {return fForwTPCPos1_X;};
  G4double GetXMLForwTPCPos1_Y()  {return fForwTPCPos1_Y;};
  G4double GetXMLForwTPCPos1_Z()  {return fForwTPCPos1_Z;};
  G4double GetXMLForwTPCPos2_X()  {return fForwTPCPos2_X;};
  G4double GetXMLForwTPCPos2_Y()  {return fForwTPCPos2_Y;};
  G4double GetXMLForwTPCPos2_Z()  {return fForwTPCPos2_Z;};
  G4double GetXMLForwTPCPos3_X()  {return fForwTPCPos3_X;};
  G4double GetXMLForwTPCPos3_Y()  {return fForwTPCPos3_Y;};
  G4double GetXMLForwTPCPos3_Z()  {return fForwTPCPos3_Z;};
  
  G4bool   GetXMLSideTPCdefault1(){return fSideTPCdefault1;};
  G4double GetXMLSideTPClength1() {return fSideTPClength1;};
  G4double GetXMLSideTPCwidth1()  {return fSideTPCwidth1;};
  G4double GetXMLSideTPCheight1() {return fSideTPCheight1;};

  G4double GetXMLSideTPCUpPos1_X()  {return fSideTPCUpPos1_X;};
  G4double GetXMLSideTPCUpPos1_Y()  {return fSideTPCUpPos1_Y;};
  G4double GetXMLSideTPCUpPos1_Z()  {return fSideTPCUpPos1_Z;};

  G4double GetXMLSideTPCDownPos1_X(){return fSideTPCDownPos1_X;};
  G4double GetXMLSideTPCDownPos1_Y(){return fSideTPCDownPos1_Y;};
  G4double GetXMLSideTPCDownPos1_Z(){return fSideTPCDownPos1_Z;};

  G4bool   GetXMLSideTPCdefault2(){return fSideTPCdefault2;};
  G4double GetXMLSideTPClength2() {return fSideTPClength2;};
  G4double GetXMLSideTPCwidth2()  {return fSideTPCwidth2;};
  G4double GetXMLSideTPCheight2() {return fSideTPCheight2;};
  
  G4double GetXMLSideTPCUpPos2_X()  {return fSideTPCUpPos2_X;};
  G4double GetXMLSideTPCUpPos2_Y()  {return fSideTPCUpPos2_Y;};
  G4double GetXMLSideTPCUpPos2_Z()  {return fSideTPCUpPos2_Z;};

  G4double GetXMLSideTPCDownPos2_X(){return fSideTPCDownPos2_X;};
  G4double GetXMLSideTPCDownPos2_Y(){return fSideTPCDownPos2_Y;};
  G4double GetXMLSideTPCDownPos2_Z(){return fSideTPCDownPos2_Z;};

  G4bool   GetXMLTargetdefault1() {return fTargetdefault1;};
  G4String GetXMLTargetMaterial1(){return fTargetMaterial1;};
  G4double GetXMLTargetlength1()  {return fTargetlength1;};
  G4double GetXMLTargetwidth1()   {return fTargetwidth1;};
  G4double GetXMLTargetheight1()  {return fTargetheight1;};
  G4double GetXMLTargetPos1_X()   {return fTargetPos1_X;};
  G4double GetXMLTargetPos1_Y()   {return fTargetPos1_Y;};
  G4double GetXMLTargetPos1_Z()   {return fTargetPos1_Z;};

  G4bool   GetXMLTargetdefault2() {return fTargetdefault2;};
  G4String GetXMLTargetMaterial2(){return fTargetMaterial2;};
  G4double GetXMLTargetlength2()  {return fTargetlength2;};
  G4double GetXMLTargetwidth2()   {return fTargetwidth2;};
  G4double GetXMLTargetheight2()  {return fTargetheight2;};
  G4double GetXMLTargetPos2_X()   {return fTargetPos2_X;};
  G4double GetXMLTargetPos2_Y()   {return fTargetPos2_Y;};
  G4double GetXMLTargetPos2_Z()   {return fTargetPos2_Z;};

  G4bool   GetXMLFGDdefault1()   {return fFGDdefault1;};
  G4String GetXMLFGDMaterial1()  {return fFGDMaterial1;};
  G4double GetXMLFGDlength1()    {return fFGDlength1;};
  G4double GetXMLFGDwidth1()     {return fFGDwidth1;};
  G4double GetXMLFGDheight1()    {return fFGDheight1;};
  G4double GetXMLFGDPos1_X()     {return fFGDPos1_X;};
  G4double GetXMLFGDPos1_Y()     {return fFGDPos1_Y;};
  G4double GetXMLFGDPos1_Z()     {return fFGDPos1_Z;};

  G4bool   GetXMLFGDdefault2()   {return fFGDdefault2;};
  G4String GetXMLFGDMaterial2()  {return fFGDMaterial2;};
  G4double GetXMLFGDlength2()    {return fFGDlength2;};
  G4double GetXMLFGDwidth2()     {return fFGDwidth2;};
  G4double GetXMLFGDheight2()    {return fFGDheight2;};
  G4double GetXMLFGDPos2_X()     {return fFGDPos2_X;};
  G4double GetXMLFGDPos2_Y()     {return fFGDPos2_Y;};
  G4double GetXMLFGDPos2_Z()     {return fFGDPos2_Z;};

  // ToF Downstream

  G4bool   GetXMLToFdefault_TopDown()       {return fToFdefault_TopDown;};
  G4int    GetXMLToFLayerHorizNBar_TopDown()   {return fToFLayerHorizNBar_TopDown;};  
  G4int    GetXMLToFLayerVertNBar_TopDown()    {return fToFLayerVertNBar_TopDown;};
  G4int    GetXMLToFPlaneXYNum_TopDown()       {return fToFPlaneXYNum_TopDown;};
  G4double GetXMLToFBarwidth_TopDown()      {return fToFBarwidth_TopDown;};
  G4double GetXMLToFBarheight_TopDown()     {return fToFBarheight_TopDown;};
  G4double GetXMLToFRotX_TopDown()          {return fToFRotX_TopDown;};
  G4double GetXMLToFRotY_TopDown()          {return fToFRotY_TopDown;};
  G4double GetXMLToFRotZ_TopDown()          {return fToFRotZ_TopDown;};
  G4double GetXMLToFPosX_TopDown()         {return fToFPosX_TopDown;};
  G4double GetXMLToFPosY_TopDown()         {return fToFPosY_TopDown;};
  G4double GetXMLToFPosZ_TopDown()         {return fToFPosZ_TopDown;};

  G4bool   GetXMLToFdefault_BotDown()       {return fToFdefault_BotDown;};
  G4int    GetXMLToFLayerHorizNBar_BotDown()   {return fToFLayerHorizNBar_BotDown;};  
  G4int    GetXMLToFLayerVertNBar_BotDown()    {return fToFLayerVertNBar_BotDown;};
  G4int    GetXMLToFPlaneXYNum_BotDown()       {return fToFPlaneXYNum_BotDown;};
  G4double GetXMLToFBarwidth_BotDown()      {return fToFBarwidth_BotDown;};
  G4double GetXMLToFBarheight_BotDown()     {return fToFBarheight_BotDown;};
  G4double GetXMLToFRotX_BotDown()          {return fToFRotX_BotDown;};
  G4double GetXMLToFRotY_BotDown()          {return fToFRotY_BotDown;};
  G4double GetXMLToFRotZ_BotDown()          {return fToFRotZ_BotDown;};
  G4double GetXMLToFPosX_BotDown()         {return fToFPosX_BotDown;};
  G4double GetXMLToFPosY_BotDown()         {return fToFPosY_BotDown;};
  G4double GetXMLToFPosZ_BotDown()         {return fToFPosZ_BotDown;};

  G4bool   GetXMLToFdefault_RightDown()       {return fToFdefault_RightDown;};
  G4int    GetXMLToFLayerHorizNBar_RightDown()   {return fToFLayerHorizNBar_RightDown;};  
  G4int    GetXMLToFLayerVertNBar_RightDown()    {return fToFLayerVertNBar_RightDown;};
  G4int    GetXMLToFPlaneXYNum_RightDown()       {return fToFPlaneXYNum_RightDown;};
  G4double GetXMLToFBarwidth_RightDown()      {return fToFBarwidth_RightDown;};
  G4double GetXMLToFBarheight_RightDown()     {return fToFBarheight_RightDown;};
  G4double GetXMLToFRotX_RightDown()          {return fToFRotX_RightDown;};
  G4double GetXMLToFRotY_RightDown()          {return fToFRotY_RightDown;};
  G4double GetXMLToFRotZ_RightDown()          {return fToFRotZ_RightDown;};
  G4double GetXMLToFPosX_RightDown()         {return fToFPosX_RightDown;};
  G4double GetXMLToFPosY_RightDown()         {return fToFPosY_RightDown;};
  G4double GetXMLToFPosZ_RightDown()         {return fToFPosZ_RightDown;};

  G4bool   GetXMLToFdefault_LeftDown()       {return fToFdefault_LeftDown;};
  G4int    GetXMLToFLayerHorizNBar_LeftDown()   {return fToFLayerHorizNBar_LeftDown;};  
  G4int    GetXMLToFLayerVertNBar_LeftDown()    {return fToFLayerVertNBar_LeftDown;};
  G4int    GetXMLToFPlaneXYNum_LeftDown()       {return fToFPlaneXYNum_LeftDown;};
  G4double GetXMLToFBarwidth_LeftDown()      {return fToFBarwidth_LeftDown;};
  G4double GetXMLToFBarheight_LeftDown()     {return fToFBarheight_LeftDown;};
  G4double GetXMLToFRotX_LeftDown()          {return fToFRotX_LeftDown;};
  G4double GetXMLToFRotY_LeftDown()          {return fToFRotY_LeftDown;};
  G4double GetXMLToFRotZ_LeftDown()          {return fToFRotZ_LeftDown;};
  G4double GetXMLToFPosX_LeftDown()         {return fToFPosX_LeftDown;};
  G4double GetXMLToFPosY_LeftDown()         {return fToFPosY_LeftDown;};
  G4double GetXMLToFPosZ_LeftDown()         {return fToFPosZ_LeftDown;};

  G4bool   GetXMLToFdefault_BackDown()       {return fToFdefault_BackDown;};
  G4int    GetXMLToFLayerHorizNBar_BackDown()   {return fToFLayerHorizNBar_BackDown;};  
  G4int    GetXMLToFLayerVertNBar_BackDown()    {return fToFLayerVertNBar_BackDown;};
  G4int    GetXMLToFPlaneXYNum_BackDown()       {return fToFPlaneXYNum_BackDown;};
  G4double GetXMLToFBarwidth_BackDown()      {return fToFBarwidth_BackDown;};
  G4double GetXMLToFBarheight_BackDown()     {return fToFBarheight_BackDown;};
  G4double GetXMLToFRotX_BackDown()          {return fToFRotX_BackDown;};
  G4double GetXMLToFRotY_BackDown()          {return fToFRotY_BackDown;};
  G4double GetXMLToFRotZ_BackDown()          {return fToFRotZ_BackDown;};
  G4double GetXMLToFPosX_BackDown()         {return fToFPosX_BackDown;};
  G4double GetXMLToFPosY_BackDown()         {return fToFPosY_BackDown;};
  G4double GetXMLToFPosZ_BackDown()         {return fToFPosZ_BackDown;};

  G4bool   GetXMLToFdefault_FrontDown()       {return fToFdefault_FrontDown;};
  G4int    GetXMLToFLayerHorizNBar_FrontDown()   {return fToFLayerHorizNBar_FrontDown;};  
  G4int    GetXMLToFLayerVertNBar_FrontDown()    {return fToFLayerVertNBar_FrontDown;};
  G4int    GetXMLToFPlaneXYNum_FrontDown()       {return fToFPlaneXYNum_FrontDown;};
  G4double GetXMLToFBarwidth_FrontDown()      {return fToFBarwidth_FrontDown;};
  G4double GetXMLToFBarheight_FrontDown()     {return fToFBarheight_FrontDown;};
  G4double GetXMLToFRotX_FrontDown()          {return fToFRotX_FrontDown;};
  G4double GetXMLToFRotY_FrontDown()          {return fToFRotY_FrontDown;};
  G4double GetXMLToFRotZ_FrontDown()          {return fToFRotZ_FrontDown;};
  G4double GetXMLToFPosX_FrontDown()         {return fToFPosX_FrontDown;};
  G4double GetXMLToFPosY_FrontDown()         {return fToFPosY_FrontDown;};
  G4double GetXMLToFPosZ_FrontDown()         {return fToFPosZ_FrontDown;};

  // ToF Upstream

  G4bool   GetXMLToFdefault_TopUp()       {return fToFdefault_TopUp;};
  G4int    GetXMLToFLayerHorizNBar_TopUp()   {return fToFLayerHorizNBar_TopUp;};  
  G4int    GetXMLToFLayerVertNBar_TopUp()    {return fToFLayerVertNBar_TopUp;};
  G4int    GetXMLToFPlaneXYNum_TopUp()       {return fToFPlaneXYNum_TopUp;};
  G4double GetXMLToFBarwidth_TopUp()      {return fToFBarwidth_TopUp;};
  G4double GetXMLToFBarheight_TopUp()     {return fToFBarheight_TopUp;};
  G4double GetXMLToFRotX_TopUp()          {return fToFRotX_TopUp;};
  G4double GetXMLToFRotY_TopUp()          {return fToFRotY_TopUp;};
  G4double GetXMLToFRotZ_TopUp()          {return fToFRotZ_TopUp;};
  G4double GetXMLToFPosX_TopUp()         {return fToFPosX_TopUp;};
  G4double GetXMLToFPosY_TopUp()         {return fToFPosY_TopUp;};
  G4double GetXMLToFPosZ_TopUp()         {return fToFPosZ_TopUp;};

  G4bool   GetXMLToFdefault_BotUp()       {return fToFdefault_BotUp;};
  G4int    GetXMLToFLayerHorizNBar_BotUp()   {return fToFLayerHorizNBar_BotUp;};  
  G4int    GetXMLToFLayerVertNBar_BotUp()    {return fToFLayerVertNBar_BotUp;};
  G4int    GetXMLToFPlaneXYNum_BotUp()       {return fToFPlaneXYNum_BotUp;};
  G4double GetXMLToFBarwidth_BotUp()      {return fToFBarwidth_BotUp;};
  G4double GetXMLToFBarheight_BotUp()     {return fToFBarheight_BotUp;};
  G4double GetXMLToFRotX_BotUp()          {return fToFRotX_BotUp;};
  G4double GetXMLToFRotY_BotUp()          {return fToFRotY_BotUp;};
  G4double GetXMLToFRotZ_BotUp()          {return fToFRotZ_BotUp;};
  G4double GetXMLToFPosX_BotUp()         {return fToFPosX_BotUp;};
  G4double GetXMLToFPosY_BotUp()         {return fToFPosY_BotUp;};
  G4double GetXMLToFPosZ_BotUp()         {return fToFPosZ_BotUp;};

  G4bool   GetXMLToFdefault_RightUp()       {return fToFdefault_RightUp;};
  G4int    GetXMLToFLayerHorizNBar_RightUp()   {return fToFLayerHorizNBar_RightUp;};  
  G4int    GetXMLToFLayerVertNBar_RightUp()    {return fToFLayerVertNBar_RightUp;};
  G4int    GetXMLToFPlaneXYNum_RightUp()       {return fToFPlaneXYNum_RightUp;};
  G4double GetXMLToFBarwidth_RightUp()      {return fToFBarwidth_RightUp;};
  G4double GetXMLToFBarheight_RightUp()     {return fToFBarheight_RightUp;};
  G4double GetXMLToFRotX_RightUp()          {return fToFRotX_RightUp;};
  G4double GetXMLToFRotY_RightUp()          {return fToFRotY_RightUp;};
  G4double GetXMLToFRotZ_RightUp()          {return fToFRotZ_RightUp;};
  G4double GetXMLToFPosX_RightUp()         {return fToFPosX_RightUp;};
  G4double GetXMLToFPosY_RightUp()         {return fToFPosY_RightUp;};
  G4double GetXMLToFPosZ_RightUp()         {return fToFPosZ_RightUp;};

  G4bool   GetXMLToFdefault_LeftUp()       {return fToFdefault_LeftUp;};
  G4int    GetXMLToFLayerHorizNBar_LeftUp()   {return fToFLayerHorizNBar_LeftUp;};  
  G4int    GetXMLToFLayerVertNBar_LeftUp()    {return fToFLayerVertNBar_LeftUp;};
  G4int    GetXMLToFPlaneXYNum_LeftUp()       {return fToFPlaneXYNum_LeftUp;};
  G4double GetXMLToFBarwidth_LeftUp()      {return fToFBarwidth_LeftUp;};
  G4double GetXMLToFBarheight_LeftUp()     {return fToFBarheight_LeftUp;};
  G4double GetXMLToFRotX_LeftUp()          {return fToFRotX_LeftUp;};
  G4double GetXMLToFRotY_LeftUp()          {return fToFRotY_LeftUp;};
  G4double GetXMLToFRotZ_LeftUp()          {return fToFRotZ_LeftUp;};
  G4double GetXMLToFPosX_LeftUp()         {return fToFPosX_LeftUp;};
  G4double GetXMLToFPosY_LeftUp()         {return fToFPosY_LeftUp;};
  G4double GetXMLToFPosZ_LeftUp()         {return fToFPosZ_LeftUp;};

  G4bool   GetXMLToFdefault_BackUp()       {return fToFdefault_BackUp;};
  G4int    GetXMLToFLayerHorizNBar_BackUp()   {return fToFLayerHorizNBar_BackUp;};  
  G4int    GetXMLToFLayerVertNBar_BackUp()    {return fToFLayerVertNBar_BackUp;};
  G4int    GetXMLToFPlaneXYNum_BackUp()       {return fToFPlaneXYNum_BackUp;};
  G4double GetXMLToFBarwidth_BackUp()      {return fToFBarwidth_BackUp;};
  G4double GetXMLToFBarheight_BackUp()     {return fToFBarheight_BackUp;};
  G4double GetXMLToFRotX_BackUp()          {return fToFRotX_BackUp;};
  G4double GetXMLToFRotY_BackUp()          {return fToFRotY_BackUp;};
  G4double GetXMLToFRotZ_BackUp()          {return fToFRotZ_BackUp;};
  G4double GetXMLToFPosX_BackUp()         {return fToFPosX_BackUp;};
  G4double GetXMLToFPosY_BackUp()         {return fToFPosY_BackUp;};
  G4double GetXMLToFPosZ_BackUp()         {return fToFPosZ_BackUp;};

  G4bool   GetXMLToFdefault_FrontUp()       {return fToFdefault_FrontUp;};
  G4int    GetXMLToFLayerHorizNBar_FrontUp()   {return fToFLayerHorizNBar_FrontUp;};  
  G4int    GetXMLToFLayerVertNBar_FrontUp()    {return fToFLayerVertNBar_FrontUp;};
  G4int    GetXMLToFPlaneXYNum_FrontUp()       {return fToFPlaneXYNum_FrontUp;};
  G4double GetXMLToFBarwidth_FrontUp()      {return fToFBarwidth_FrontUp;};
  G4double GetXMLToFBarheight_FrontUp()     {return fToFBarheight_FrontUp;};
  G4double GetXMLToFRotX_FrontUp()          {return fToFRotX_FrontUp;};
  G4double GetXMLToFRotY_FrontUp()          {return fToFRotY_FrontUp;};
  G4double GetXMLToFRotZ_FrontUp()          {return fToFRotZ_FrontUp;};
  G4double GetXMLToFPosX_FrontUp()         {return fToFPosX_FrontUp;};
  G4double GetXMLToFPosY_FrontUp()         {return fToFPosY_FrontUp;};
  G4double GetXMLToFPosZ_FrontUp()         {return fToFPosZ_FrontUp;};
  
  //
  G4bool   GetXMLUseForwTPC1()    {return fUseForwTPC1;};
  G4bool   GetXMLUseForwTPC2()    {return fUseForwTPC2;};
  G4bool   GetXMLUseForwTPC3()    {return fUseForwTPC3;};
  G4bool   GetXMLUseTarget1()    {return fUseTarget1;};
  G4bool   GetXMLUseTarget2()    {return fUseTarget2;};
  G4bool   GetXMLUseTPCUp1()     {return fUseTPCUp1;};
  G4bool   GetXMLUseTPCDown1()   {return fUseTPCDown1;};
  G4bool   GetXMLUseTPCUp2()     {return fUseTPCUp2;};
  G4bool   GetXMLUseTPCDown2()   {return fUseTPCDown2;};
  G4bool   GetXMLUseFGD1()       {return fUseFGD1;};
  G4bool   GetXMLUseFGD2()       {return fUseFGD2;};
  G4bool   GetXMLUseToF_TopDown()       {return fUseToF_TopDown;};
  G4bool   GetXMLUseToF_BotDown()       {return fUseToF_BotDown;};
  G4bool   GetXMLUseToF_RightDown()       {return fUseToF_RightDown;};
  G4bool   GetXMLUseToF_LeftDown()       {return fUseToF_LeftDown;};
  G4bool   GetXMLUseToF_BackDown()       {return fUseToF_BackDown;};
  G4bool   GetXMLUseToF_FrontDown()       {return fUseToF_FrontDown;};
  G4bool   GetXMLUseToF_TopUp()       {return fUseToF_TopUp;};
  G4bool   GetXMLUseToF_BotUp()       {return fUseToF_BotUp;};
  G4bool   GetXMLUseToF_RightUp()       {return fUseToF_RightUp;};
  G4bool   GetXMLUseToF_LeftUp()       {return fUseToF_LeftUp;};
  G4bool   GetXMLUseToF_BackUp()       {return fUseToF_BackUp;};
  G4bool   GetXMLUseToF_FrontUp()       {return fUseToF_FrontUp;};
  //
  G4bool   GetXMLUseMagnet()     {return fUseMagnet;};
  //G4bool   GetXMLUseSMRD()       {return fUseSMRD;};
  G4bool   GetXMLUseBasket()     {return fUseBasket;};
  G4bool   GetXMLUseDsECal()     {return fUseDsECal;};
  G4bool   GetXMLUseBrlECal()    {return fUseBrlECal;};
  G4bool   GetXMLUseP0DECal()    {return fUseP0DECal;};  
  G4bool   GetXMLUseP0D()        {return fUseP0D;};

  G4bool   GetXMLInvisForwTPC1()  {return fInvisForwTPC1;};
  G4bool   GetXMLInvisForwTPC2()  {return fInvisForwTPC2;};
  G4bool   GetXMLInvisForwTPC3()  {return fInvisForwTPC3;};
  G4bool   GetXMLInvisTarget1()  {return fInvisTarget1;};
  G4bool   GetXMLInvisTarget2()  {return fInvisTarget2;};
  G4bool   GetXMLInvisTPCUp1()   {return fInvisTPCUp1;};
  G4bool   GetXMLInvisTPCDown1() {return fInvisTPCDown1;};
  G4bool   GetXMLInvisTPCUp2()   {return fInvisTPCUp2;};
  G4bool   GetXMLInvisTPCDown2() {return fInvisTPCDown2;};
  G4bool   GetXMLInvisFGD1()     {return fInvisFGD1;};
  G4bool   GetXMLInvisFGD2()     {return fInvisFGD2;};
  G4bool   GetXMLInvisToF_TopDown()     {return fInvisToF_TopDown;};  
  G4bool   GetXMLInvisToF_BotDown()     {return fInvisToF_BotDown;};
  G4bool   GetXMLInvisToF_RightDown()     {return fInvisToF_RightDown;};  
  G4bool   GetXMLInvisToF_LeftDown()     {return fInvisToF_LeftDown;};  
  G4bool   GetXMLInvisToF_BackDown()     {return fInvisToF_BackDown;};  
  G4bool   GetXMLInvisToF_FrontDown()     {return fInvisToF_FrontDown;};  
  G4bool   GetXMLInvisToF_TopUp()     {return fInvisToF_TopUp;};  
  G4bool   GetXMLInvisToF_BotUp()     {return fInvisToF_BotUp;};  
  G4bool   GetXMLInvisToF_RightUp()     {return fInvisToF_RightUp;};  
  G4bool   GetXMLInvisToF_LeftUp()     {return fInvisToF_LeftUp;};  
  G4bool   GetXMLInvisToF_BackUp()     {return fInvisToF_BackUp;};  
  G4bool   GetXMLInvisToF_FrontUp()     {return fInvisToF_FrontUp;};  

  G4bool   GetXMLInvisMagnet()   {return fInvisMagnet;};
  G4bool   GetXMLInvisSMRD()     {return fInvisSMRD;};
  G4bool   GetXMLInvisBasket()   {return fInvisBasket;};
  G4bool   GetXMLInvisDsECal()   {return fInvisDsECal;};
  G4bool   GetXMLInvisBrlECal()  {return fInvisBrlECal;};
  G4bool   GetXMLInvisP0DECal()  {return fInvisP0DECal;};
  G4bool   GetXMLInvisP0D()      {return fInvisP0D;};
  
  void SetInputs();

private :

  G4String fXMLFile;
  G4bool   fIsRandomSeed;
  G4bool   fStoreGeometry;
  G4String fGenerType;
  G4String fPathFiles;
  G4String fGenerTreeName;
  G4String fGenerFileName;
  G4int    fGenerStepEvent;

  G4bool   fForwTPCdefault;
  G4double fForwTPCPos1_X;
  G4double fForwTPCPos1_Y;
  G4double fForwTPCPos1_Z;
  G4double fForwTPCPos2_X;
  G4double fForwTPCPos2_Y;
  G4double fForwTPCPos2_Z;
  G4double fForwTPCPos3_X;
  G4double fForwTPCPos3_Y;
  G4double fForwTPCPos3_Z;

  G4bool   fSideTPCdefault1;
  G4double fSideTPClength1;
  G4double fSideTPCwidth1;
  G4double fSideTPCheight1;
  G4double fSideTPCUpPos1_X;
  G4double fSideTPCUpPos1_Y;
  G4double fSideTPCUpPos1_Z;
  G4double fSideTPCDownPos1_X;
  G4double fSideTPCDownPos1_Y;
  G4double fSideTPCDownPos1_Z;

  G4bool   fSideTPCdefault2;
  G4double fSideTPClength2;
  G4double fSideTPCwidth2;
  G4double fSideTPCheight2;
  G4double fSideTPCUpPos2_X;
  G4double fSideTPCUpPos2_Y;
  G4double fSideTPCUpPos2_Z;
  G4double fSideTPCDownPos2_X;
  G4double fSideTPCDownPos2_Y;
  G4double fSideTPCDownPos2_Z;

  G4bool   fTargetdefault1;
  G4String fTargetMaterial1;
  G4double fTargetlength1;
  G4double fTargetwidth1;
  G4double fTargetheight1;
  G4double fTargetPos1_X;
  G4double fTargetPos1_Y;
  G4double fTargetPos1_Z;

  G4bool   fTargetdefault2;
  G4String fTargetMaterial2;
  G4double fTargetlength2;
  G4double fTargetwidth2;
  G4double fTargetheight2;
  G4double fTargetPos2_X;
  G4double fTargetPos2_Y;
  G4double fTargetPos2_Z;

  G4bool   fFGDdefault1;
  G4String fFGDMaterial1;
  G4double fFGDlength1;
  G4double fFGDwidth1;
  G4double fFGDheight1;
  G4double fFGDPos1_X;
  G4double fFGDPos1_Y;
  G4double fFGDPos1_Z;

  G4bool   fFGDdefault2;
  G4String fFGDMaterial2;
  G4double fFGDlength2;
  G4double fFGDwidth2;
  G4double fFGDheight2;
  G4double fFGDPos2_X;
  G4double fFGDPos2_Y;
  G4double fFGDPos2_Z;
  
  // ToF Downstream

  G4bool   fToFdefault_TopDown;
  G4int    fToFLayerHorizNBar_TopDown;
  G4int    fToFLayerVertNBar_TopDown;
  G4int    fToFPlaneXYNum_TopDown;
  G4double fToFBarHorizlength_TopDown;
  G4double fToFBarVertlength_TopDown;
  G4double fToFBarwidth_TopDown;
  G4double fToFBarheight_TopDown;
  G4double fToFRotX_TopDown;
  G4double fToFRotY_TopDown;
  G4double fToFRotZ_TopDown;
  G4double fToFPosX_TopDown;
  G4double fToFPosY_TopDown;
  G4double fToFPosZ_TopDown;

  G4bool   fToFdefault_BotDown;
  G4int    fToFLayerHorizNBar_BotDown;
  G4int    fToFLayerVertNBar_BotDown;
  G4int    fToFPlaneXYNum_BotDown;
  G4double fToFBarHorizlength_BotDown;
  G4double fToFBarVertlength_BotDown;
  G4double fToFBarwidth_BotDown;
  G4double fToFBarheight_BotDown;
  G4double fToFRotX_BotDown;
  G4double fToFRotY_BotDown;
  G4double fToFRotZ_BotDown;
  G4double fToFPosX_BotDown;
  G4double fToFPosY_BotDown;
  G4double fToFPosZ_BotDown;

  G4bool   fToFdefault_RightDown;
  G4int    fToFLayerHorizNBar_RightDown;
  G4int    fToFLayerVertNBar_RightDown;
  G4int    fToFPlaneXYNum_RightDown;
  G4double fToFBarHorizlength_RightDown;
  G4double fToFBarVertlength_RightDown;
  G4double fToFBarwidth_RightDown;
  G4double fToFBarheight_RightDown;
  G4double fToFRotX_RightDown;
  G4double fToFRotY_RightDown;
  G4double fToFRotZ_RightDown;
  G4double fToFPosX_RightDown;
  G4double fToFPosY_RightDown;
  G4double fToFPosZ_RightDown;

  G4bool   fToFdefault_LeftDown;
  G4int    fToFLayerHorizNBar_LeftDown;
  G4int    fToFLayerVertNBar_LeftDown;
  G4int    fToFPlaneXYNum_LeftDown;
  G4double fToFBarHorizlength_LeftDown;
  G4double fToFBarVertlength_LeftDown;
  G4double fToFBarwidth_LeftDown;
  G4double fToFBarheight_LeftDown;
  G4double fToFRotX_LeftDown;
  G4double fToFRotY_LeftDown;
  G4double fToFRotZ_LeftDown;
  G4double fToFPosX_LeftDown;
  G4double fToFPosY_LeftDown;
  G4double fToFPosZ_LeftDown;

  G4bool   fToFdefault_BackDown;
  G4int    fToFLayerHorizNBar_BackDown;
  G4int    fToFLayerVertNBar_BackDown;
  G4int    fToFPlaneXYNum_BackDown;
  G4double fToFBarHorizlength_BackDown;
  G4double fToFBarVertlength_BackDown;
  G4double fToFBarwidth_BackDown;
  G4double fToFBarheight_BackDown;
  G4double fToFRotX_BackDown;
  G4double fToFRotY_BackDown;
  G4double fToFRotZ_BackDown;
  G4double fToFPosX_BackDown;
  G4double fToFPosY_BackDown;
  G4double fToFPosZ_BackDown;

  G4bool   fToFdefault_FrontDown;
  G4int    fToFLayerHorizNBar_FrontDown;
  G4int    fToFLayerVertNBar_FrontDown;
  G4int    fToFPlaneXYNum_FrontDown;
  G4double fToFBarHorizlength_FrontDown;
  G4double fToFBarVertlength_FrontDown;
  G4double fToFBarwidth_FrontDown;
  G4double fToFBarheight_FrontDown;
  G4double fToFRotX_FrontDown;
  G4double fToFRotY_FrontDown;
  G4double fToFRotZ_FrontDown;
  G4double fToFPosX_FrontDown;
  G4double fToFPosY_FrontDown;
  G4double fToFPosZ_FrontDown;

  // ToF Upstream

  G4bool   fToFdefault_TopUp;
  G4int    fToFLayerHorizNBar_TopUp;
  G4int    fToFLayerVertNBar_TopUp;
  G4int    fToFPlaneXYNum_TopUp;
  G4double fToFBarHorizlength_TopUp;
  G4double fToFBarVertlength_TopUp;
  G4double fToFBarwidth_TopUp;
  G4double fToFBarheight_TopUp;
  G4double fToFRotX_TopUp;
  G4double fToFRotY_TopUp;
  G4double fToFRotZ_TopUp;
  G4double fToFPosX_TopUp;
  G4double fToFPosY_TopUp;
  G4double fToFPosZ_TopUp;

  G4bool   fToFdefault_BotUp;
  G4int    fToFLayerHorizNBar_BotUp;
  G4int    fToFLayerVertNBar_BotUp;
  G4int    fToFPlaneXYNum_BotUp;
  G4double fToFBarHorizlength_BotUp;
  G4double fToFBarVertlength_BotUp;
  G4double fToFBarwidth_BotUp;
  G4double fToFBarheight_BotUp;
  G4double fToFRotX_BotUp;
  G4double fToFRotY_BotUp;
  G4double fToFRotZ_BotUp;
  G4double fToFPosX_BotUp;
  G4double fToFPosY_BotUp;
  G4double fToFPosZ_BotUp;

  G4bool   fToFdefault_RightUp;
  G4int    fToFLayerHorizNBar_RightUp;
  G4int    fToFLayerVertNBar_RightUp;
  G4int    fToFPlaneXYNum_RightUp;
  G4double fToFBarHorizlength_RightUp;
  G4double fToFBarVertlength_RightUp;
  G4double fToFBarwidth_RightUp;
  G4double fToFBarheight_RightUp;
  G4double fToFRotX_RightUp;
  G4double fToFRotY_RightUp;
  G4double fToFRotZ_RightUp;
  G4double fToFPosX_RightUp;
  G4double fToFPosY_RightUp;
  G4double fToFPosZ_RightUp;

  G4bool   fToFdefault_LeftUp;
  G4int    fToFLayerHorizNBar_LeftUp;
  G4int    fToFLayerVertNBar_LeftUp;
  G4int    fToFPlaneXYNum_LeftUp;
  G4double fToFBarHorizlength_LeftUp;
  G4double fToFBarVertlength_LeftUp;
  G4double fToFBarwidth_LeftUp;
  G4double fToFBarheight_LeftUp;
  G4double fToFRotX_LeftUp;
  G4double fToFRotY_LeftUp;
  G4double fToFRotZ_LeftUp;
  G4double fToFPosX_LeftUp;
  G4double fToFPosY_LeftUp;
  G4double fToFPosZ_LeftUp;

  G4bool   fToFdefault_BackUp;
  G4int    fToFLayerHorizNBar_BackUp;
  G4int    fToFLayerVertNBar_BackUp;
  G4int    fToFPlaneXYNum_BackUp;
  G4double fToFBarHorizlength_BackUp;
  G4double fToFBarVertlength_BackUp;
  G4double fToFBarwidth_BackUp;
  G4double fToFBarheight_BackUp;
  G4double fToFRotX_BackUp;
  G4double fToFRotY_BackUp;
  G4double fToFRotZ_BackUp;
  G4double fToFPosX_BackUp;
  G4double fToFPosY_BackUp;
  G4double fToFPosZ_BackUp;

  G4bool   fToFdefault_FrontUp;
  G4int    fToFLayerHorizNBar_FrontUp;
  G4int    fToFLayerVertNBar_FrontUp;
  G4int    fToFPlaneXYNum_FrontUp;
  G4double fToFBarHorizlength_FrontUp;
  G4double fToFBarVertlength_FrontUp;
  G4double fToFBarwidth_FrontUp;
  G4double fToFBarheight_FrontUp;
  G4double fToFRotX_FrontUp;
  G4double fToFRotY_FrontUp;
  G4double fToFRotZ_FrontUp;
  G4double fToFPosX_FrontUp;
  G4double fToFPosY_FrontUp;
  G4double fToFPosZ_FrontUp;

  //
  
  G4bool fUseForwTPC1;
  G4bool fUseForwTPC2;
  G4bool fUseForwTPC3;
  G4bool fUseTarget1;
  G4bool fUseTarget2;
  G4bool fUseTPCUp1;
  G4bool fUseTPCDown1;
  G4bool fUseTPCUp2;
  G4bool fUseTPCDown2;
  G4bool fUseFGD1;
  G4bool fUseFGD2;
  G4bool fUseMagnet;
  G4bool fUseToF_TopDown;
  G4bool fUseToF_BotDown;
  G4bool fUseToF_RightDown;
  G4bool fUseToF_LeftDown;
  G4bool fUseToF_BackDown;
  G4bool fUseToF_FrontDown;
  G4bool fUseToF_TopUp;
  G4bool fUseToF_BotUp;
  G4bool fUseToF_RightUp;
  G4bool fUseToF_LeftUp;
  G4bool fUseToF_BackUp;
  G4bool fUseToF_FrontUp;
  //G4bool fUseSMRD;
  G4bool fUseBasket;
  G4bool fUseDsECal;
  G4bool fUseBrlECal;
  G4bool fUseP0DECal;
  G4bool fUseP0D;
  
  G4bool fInvisForwTPC1;
  G4bool fInvisForwTPC2;
  G4bool fInvisForwTPC3;
  G4bool fInvisTarget1;
  G4bool fInvisTarget2;
  G4bool fInvisTPCUp1;
  G4bool fInvisTPCDown1;
  G4bool fInvisTPCUp2;
  G4bool fInvisTPCDown2;
  G4bool fInvisFGD1;
  G4bool fInvisFGD2;
  G4bool fInvisToF_TopDown;
  G4bool fInvisToF_BotDown;
  G4bool fInvisToF_RightDown;
  G4bool fInvisToF_LeftDown;
  G4bool fInvisToF_BackDown;
  G4bool fInvisToF_FrontDown;
  G4bool fInvisToF_TopUp;
  G4bool fInvisToF_BotUp;
  G4bool fInvisToF_RightUp;
  G4bool fInvisToF_LeftUp;
  G4bool fInvisToF_BackUp;
  G4bool fInvisToF_FrontUp;
  G4bool fInvisMagnet;
  G4bool fInvisSMRD;
  G4bool fInvisBasket;
  G4bool fInvisDsECal;
  G4bool fInvisBrlECal;
  G4bool fInvisP0DECal;
  G4bool fInvisP0D;

  void XML2String(string tmp_app,string &dest);
  void XML2Double(string tmp_app,double &dest);
  void XML2Int   (string tmp_app,int &dest);
  void XML2Bool  (string tmp_app,bool &dest);
  
  // XMLNodePointer_t FindNode          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // bool             NodeExists        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  // bool             XML2Bool          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // int              XML2Int           (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // double           XML2Dbl           (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // vector<double>   XML2DblArray      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // string           XML2String        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // pair<double,double> XML2DblDblPair (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  // vector<string> Tokens             (string input, string delimiter);
  string TrimSpaces(string input);
  
};

#endif
