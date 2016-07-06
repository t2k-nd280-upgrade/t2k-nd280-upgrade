
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

  ExN02ND280XML(const G4String inxmlname="../src/app/EffStudy/configuration.xml");
  ~ExN02ND280XML();
    
  G4String GetXMLFileName(){return fXMLFile;};
  G4String GetXMLGenerTypeName(){return fGenerType;};
  G4String GetXMLPathFiles(){return fPathFiles;};
  G4String GetXMLGenerTreeName(){return fGenerTreeName;};
  G4String GetXMLGenerFileName(){return fGenerFileName;};

  void SetInputs();

private :

  G4String fXMLFile; 
  G4String fGenerType;
  G4String fPathFiles;
  G4String fGenerTreeName;
  G4String fGenerFileName;

  void XML2String(string tmp_app,string &dest);
  
  
  // XMLNodePointer_t FindNode          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // bool             NodeExists        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  // bool             XML2Bool          (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // int              XML2Int           (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // double           XML2Dbl           (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // vector<double>   XML2DblArray      (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // string           XML2String        (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);
  // pair<double,double> XML2DblDblPair (TXMLEngine * xml_eng, XMLNodePointer_t top_node, string node_path);

  // vector<string> Tokens             (string input, string delimiter);
  // string TrimSpaces(string input);
  
};

#endif
