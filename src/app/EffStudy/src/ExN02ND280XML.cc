
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
  
  //G4cout << G4endl;
  //G4cout << "Reading inputs from : " << this->GetXMLFileName() << G4endl;
  //G4cout << G4endl;

  string line;
  ifstream myfile (fXMLFile);
  if(myfile.is_open()){    
    XML2String("Generator",fGenerType);
    XML2String("Path2file",fPathFiles);
    XML2String("filename",fGenerFileName);  
    XML2String("treename",fGenerTreeName);      
    //XML2Int   ("firstevent",fGenerFirstEvent);      
    XML2Int   ("stepevent",fGenerStepEvent);      
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
    
    if ( tmp == Form("<%s>",tmp_app.c_str()) ){
      begin_tag = true;
      continue;
    }
    else if ( tmp == Form("</%s>",tmp_app.c_str()) ){
      begin_tag = false;
    }
    if (begin_tag){
      dest = tmp;
    }
  }
  return;
}

void ExN02ND280XML::XML2Int(string tmp_app,int &dest){
  string dest_string;
  XML2String(tmp_app,dest_string);
  dest = atoi(dest_string.c_str());
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

// string ExN02ND280XML::TrimSpaces(string input)
// {
//   if( input.find_first_not_of(" \n") != 0)
//     input.erase( 0,  input.find_first_not_of(" \n")  );

//   if( input.find_last_not_of(" \n") != input.length() )
//     input.erase( input.find_last_not_of(" \n")+1, input.length() );

//   return input;
// }
