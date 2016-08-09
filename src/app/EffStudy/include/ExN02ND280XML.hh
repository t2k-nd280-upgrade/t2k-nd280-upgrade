
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

  G4bool   GetXMLTargetdefault1(){return fTargetdefault1;};
  G4double GetXMLTargetlength1() {return fTargetlength1;};
  G4double GetXMLTargetwidth1()  {return fTargetwidth1;};
  G4double GetXMLTargetheight1() {return fTargetheight1;};

  G4double GetXMLTargetPos1_X()  {return fTargetPos1_X;};
  G4double GetXMLTargetPos1_Y()  {return fTargetPos1_Y;};
  G4double GetXMLTargetPos1_Z()  {return fTargetPos1_Z;};

  G4bool   GetXMLTargetdefault2(){return fTargetdefault2;};
  G4double GetXMLTargetlength2() {return fTargetlength2;};
  G4double GetXMLTargetwidth2()  {return fTargetwidth2;};
  G4double GetXMLTargetheight2() {return fTargetheight2;};

  G4double GetXMLTargetPos2_X()  {return fTargetPos2_X;};
  G4double GetXMLTargetPos2_Y()  {return fTargetPos2_Y;};
  G4double GetXMLTargetPos2_Z()  {return fTargetPos2_Z;};

  void SetInputs();

private :

  G4String fXMLFile;
  G4bool   fIsRandomSeed;
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
  G4double fTargetlength1;
  G4double fTargetwidth1;
  G4double fTargetheight1;
  G4double fTargetPos1_X;
  G4double fTargetPos1_Y;
  G4double fTargetPos1_Z;

  G4bool   fTargetdefault2;
  G4double fTargetlength2;
  G4double fTargetwidth2;
  G4double fTargetheight2;
  G4double fTargetPos2_X;
  G4double fTargetPos2_Y;
  G4double fTargetPos2_Z;

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
