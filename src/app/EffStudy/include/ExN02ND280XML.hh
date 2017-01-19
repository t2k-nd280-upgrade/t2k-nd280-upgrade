
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

  G4bool   GetXMLUseTPCUp1()     {return fUseTPCUp1;};
  G4bool   GetXMLUseTPCDown1()   {return fUseTPCDown1;};
  G4bool   GetXMLUseTPCUp2()     {return fUseTPCUp2;};
  G4bool   GetXMLUseTPCDown2()   {return fUseTPCDown2;};
  G4bool   GetXMLUseFGD1()       {return fUseFGD1;};
  G4bool   GetXMLUseFGD2()       {return fUseFGD2;};
  G4bool   GetXMLUseTarget1()    {return fUseTarget1;};
  G4bool   GetXMLUseTarget2()    {return fUseTarget2;};
  G4bool   GetXMLUseMagnet()     {return fUseMagnet;};
  //G4bool   GetXMLUseSMRD()       {return fUseSMRD;};
  G4bool   GetXMLUseBasket()     {return fUseBasket;};
  G4bool   GetXMLUseDsECal()     {return fUseDsECal;};
  G4bool   GetXMLUseBrlECal()    {return fUseBrlECal;};
  G4bool   GetXMLUseP0DECal()    {return fUseP0DECal;};  
  G4bool   GetXMLUseP0D()        {return fUseP0D;};

  G4bool   GetXMLInvisTPCUp1()   {return fInvisTPCUp1;};
  G4bool   GetXMLInvisTPCDown1() {return fInvisTPCDown1;};
  G4bool   GetXMLInvisTPCUp2()   {return fInvisTPCUp2;};
  G4bool   GetXMLInvisTPCDown2() {return fInvisTPCDown2;};
  G4bool   GetXMLInvisFGD1()     {return fInvisFGD1;};
  G4bool   GetXMLInvisFGD2()     {return fInvisFGD2;};
  G4bool   GetXMLInvisTarget1()  {return fInvisTarget1;};
  G4bool   GetXMLInvisTarget2()  {return fInvisTarget2;};
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

  G4bool fUseTPCUp1;
  G4bool fUseTPCDown1;
  G4bool fUseTPCUp2;
  G4bool fUseTPCDown2;
  G4bool fUseFGD1;
  G4bool fUseFGD2;
  G4bool fUseTarget1;
  G4bool fUseTarget2;
  G4bool fUseMagnet;
  //G4bool fUseSMRD;
  G4bool fUseBasket;
  G4bool fUseDsECal;
  G4bool fUseBrlECal;
  G4bool fUseP0DECal;
  G4bool fUseP0D;
  
  G4bool fInvisTPCUp1;
  G4bool fInvisTPCDown1;
  G4bool fInvisTPCUp2;
  G4bool fInvisTPCDown2;
  G4bool fInvisFGD1;
  G4bool fInvisFGD2;
  G4bool fInvisTarget1;
  G4bool fInvisTarget2;
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
