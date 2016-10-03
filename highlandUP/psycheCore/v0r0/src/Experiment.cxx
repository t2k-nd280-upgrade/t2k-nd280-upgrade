#include "Experiment.hxx"

#include <sstream>
#include <iostream>
#include <fstream>

//********************************************************************
SampleGroup::SampleGroup(const std::string& name){
//********************************************************************

    _name = name; 
    _dataSamples.clear();
}

//********************************************************************
SampleGroup::SampleGroup(const std::string& name, const std::string& dataFile, std::map<std::string, std::string>& mcFiles){
//********************************************************************

    _name = name; 
    AddDataSample(dataFile);
    AddMCSamples(mcFiles);
}

//********************************************************************
SampleGroup::SampleGroup(const std::string& name, DataSample* dataSample, std::map<std::string, DataSample*>& mcSamples){
//********************************************************************

    _name = name; 
    AddDataSample(dataSample);
    AddMCSamples(mcSamples);
}

//**************************************************
void SampleGroup::AddDataSample(const std::string& file){
//**************************************************

    _dataSamples.push_back(new DataSample(file));
}

//**************************************************
void SampleGroup::AddDataSample(DataSample* sample){
//**************************************************

    _dataSamples.push_back(sample);
}

//**************************************************
void SampleGroup::AddMCSample(const std::string& name, const std::string& file){
//**************************************************

    _mcSamples[name] = new DataSample(file);
}

//**************************************************
void SampleGroup::AddMCSample(const std::string& name, DataSample* sample){
//**************************************************

    _mcSamples[name] = sample;
}


//**************************************************
void SampleGroup::AddMCSamples(std::map<std::string, std::string>& mcFiles){
//**************************************************

    std::map< std::string, std::string>::iterator it;
    for (it = mcFiles.begin(); it != mcFiles.end(); it++) {
        std::string name = it->first;
        std::string file = it->second;
        AddMCSample(name,file);
    }
}

//**************************************************
void SampleGroup::AddMCSamples(std::map<std::string, DataSample*>& mcSamples){
//**************************************************

    std::map< std::string, DataSample*>::iterator it;
    for (it = mcSamples.begin(); it != mcSamples.end(); it++) {
        std::string name = it->first;
        DataSample* sample = it->second;
        AddMCSample(name,sample);
    }
}

//********************************************************************
bool SampleGroup::HasMCSample(const std::string& name){
//********************************************************************

    if (_mcSamples.find(name)==_mcSamples.end()){
        std::cout << "MC sample '" << name << "' does not exists in SampleGroup '" << _name << "' !!!" << std::endl; 
        return false;
    }
    return true;
}

//**************************************************
void SampleGroup::GetPOT(Float_t& POTdata, Float_t& POTmc) {
//**************************************************

    Float_t POTsand;
    GetPOT(POTdata, POTmc, POTsand); 
}

//**************************************************
void SampleGroup::GetPOT(Float_t& POTdata, Float_t& POTmc, Float_t& POTsand) {
//**************************************************

    if(GetDataSample() != NULL){
        POTdata = GetDataSample()->GetPOT();
    }

    POTmc = 0;
    std::map< std::string, DataSample*>::iterator mit;
    std::map< std::string, DataSample*>& mcSamples = GetMCSamples();

    for (mit = mcSamples.begin(); mit != mcSamples.end(); mit++) {
        if((mit->first).find("and") != std::string::npos){
            DataSample* sample = mit->second;
            Float_t pot = sample->GetPOT();
            if(sample->GetPOT() < 1e17) pot = pot*2.5e17;
            POTsand += pot;
        }
        else{
            DataSample* sample = mit->second;
            POTmc += sample->GetPOT();
        }
    }
}

//**************************************************
void SampleGroup::SetCurrentTree(const std::string& name){
//**************************************************

    std::vector<DataSample*>::iterator it;
    for (it = _dataSamples.begin(); it != _dataSamples.end(); it++) {
        (*it)->SetCurrentTree(name);
    }


    std::map< std::string, DataSample*>::iterator mit;
    for (mit = _mcSamples.begin(); mit != _mcSamples.end(); mit++) {
        DataSample* sample = mit->second;
        sample->SetCurrentTree(name);
    }
}

//********************************************************************
Experiment::Experiment(const std::string& name){
//********************************************************************

    _name = name; 
    _filePath = "";
    _tree="default";
}

//**************************************************
Experiment::Experiment(const std::string& name, const std::string& file1, const std::string& file2) {
//**************************************************

  _name=name;
  std::map<std::string, std::string> files2;
  files2["mc"]=file2;
  _sampleGroups["group"] = SampleGroup("group", file1, files2);
  _filePath = file1;  
}

//**************************************************
Experiment::Experiment(const std::string& name, const std::string& configfile) {
//**************************************************
    _name = name;
    _tree="default";


    _dataFiles.clear();
    _mcFiles.clear();

    std::ifstream infile(configfile.c_str());

    while (infile) {
        std::string line;
        if (!getline(infile, line))
            break;

        std::istringstream ssline(line);
        std::vector<std::string> record;

        while (ssline) {
            std::string token;
            if (!getline(ssline, token, ' ')) break;
            record.push_back(token);
        }

        if (record.size() < 3) {
            std::cerr << "Invalid format (should be sampleName dataName:dataFile mcName:mcFile [mcName2:mcFile2])" << std::endl;
            std::cerr << "Saw: " << line << std::endl;
            continue;
        }

        std::string groupName = record[0];
        std::string dataFile = record[1].replace(0, record[1].find(":") + 1, "");
        std::cout << "Adding group " << groupName << " with data file " << dataFile << " and MC files ";

        std::map<std::string, std::string> mcFiles;
        for (unsigned int i = 2; i < record.size(); i++) {
            std::string mcName = record[i];
            std::string mcFile = record[i];
            mcName = mcName.replace(mcName.find(":"), mcName.size(), "");
            mcFile = mcFile.replace(0, mcFile.find(":") + 1, "");
            mcFiles[mcName] = mcFile;
            std::cout << mcFile << " ";
        }

        std::cout << std::endl;

        _sampleGroups[groupName] = SampleGroup(groupName, dataFile, mcFiles);
        _filePath = dataFile;
    }
}

//********************************************************************
bool Experiment::HasSampleGroup(const std::string& name){
//********************************************************************

    if (_sampleGroups.find(name)==_sampleGroups.end()){
        std::cout << "SampleGroup '" << name << "' does not exists in Experiment '" << _name << "' !!!" << std::endl; 
        return false;
    }
    return true;
}

//**************************************************
void Experiment::AddSampleFilesToVector(SampleGroup& sampleGroup){
//**************************************************

    std::map< std::string, DataSample*>& mcSamples = sampleGroup.GetMCSamples();
    for (std::map< std::string, DataSample*>::iterator it = mcSamples.begin(); it != mcSamples.end(); it++) {
        _mcFiles.push_back(it->second->GetFilePath());
    }      

    std::vector<DataSample*>& dataSamples = sampleGroup.GetDataSamples();
    for (std::vector<DataSample*>::iterator it2 = dataSamples.begin(); it2 != dataSamples.end(); it2++) {
        _dataFiles.push_back((*it2)->GetFilePath());
    }
}

//**************************************************
void Experiment::AddSampleGroup(const std::string& name, SampleGroup& sampleGroup){
//**************************************************

    AddSampleFilesToVector(sampleGroup);
    _sampleGroups[name] = sampleGroup;

}

//**************************************************
void Experiment::AddSampleGroup(const std::string& name, const std::string& dataFile, std::map<std::string, std::string>& mcFiles){
//**************************************************

    _sampleGroups[name] = SampleGroup(name, dataFile, mcFiles);
    AddSampleFilesToVector(_sampleGroups[name]);  
    _filePath = dataFile;
}

//**************************************************
void Experiment::AddSampleGroup(const std::string& name, DataSample* dataSample, std::map<std::string, DataSample*>& mcSamples){
//**************************************************

    _sampleGroups[name] = SampleGroup(name, dataSample, mcSamples);
    AddSampleFilesToVector(_sampleGroups[name]);  

}

//**************************************************
void Experiment::SetCurrentTree(const std::string& name){
//**************************************************

    std::map< std::string, SampleGroup >::iterator sit;
    for (sit = _sampleGroups.begin(); sit != _sampleGroups.end(); sit++) {
        SampleGroup& sampleGroup = sit->second;
        sampleGroup.SetCurrentTree(name);
    }
    _tree = name;
}

//**************************************************
void Experiment::DumpPOT(const std::string& name){
//**************************************************


    SampleGroup& sampleGroup = _sampleGroups[name];

    std::cout << "---------------------------------------------" << std::endl;
    std::cout << "-------- POT info for sample pair '"<< name << "' ---------" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    if (sampleGroup.GetDataSample()){
      std::cout << "data sample:" << std::endl;
      sampleGroup.GetDataSample()->DumpPOT();
      std::cout << std::endl;
    }

    std::cout << "-------- MC samples -------------------------" << std::endl;
    std::map< std::string, DataSample*>::iterator it;

    std::map< std::string, DataSample*>& mcSamples = sampleGroup.GetMCSamples();
    for (it = mcSamples.begin(); it != mcSamples.end(); it++) {
        std::string name = it->first;
        DataSample* sample = it->second;

        std::cout << "MC sample: " << name << std::endl;
        sample->DumpPOT();
    }
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

}


//**************************************************
void Experiment::DumpPOTRatios(){
//**************************************************
    std::map< std::string, SampleGroup >::iterator sit;

    for (sit = _sampleGroups.begin(); sit != _sampleGroups.end(); sit++) {
        SampleGroup& sampleGroup = sit->second;
        Float_t POTdata, POTmc;
        sampleGroup.GetPOT(POTdata, POTmc);

        Float_t ratio;

        if (POTmc == 0) {
            std::cout << "Warning: MC POT is zero! Setting ratio to 1." << std::endl;
            ratio = 1.;
        } else if (POTdata == 0) {
            std::cout << "Warning: data POT is zero! Setting ratio to 1." << std::endl;
            ratio = 1.;
        } else {
            ratio = POTmc / POTdata;
        }

        std::cout << sit->first << ": \t" << ratio << " \t(" << POTmc << " / " << POTdata << " )" << std::endl;
    }
}

//**************************************************
Float_t Experiment::GetOverallPOTRatio() {
//**************************************************
    Float_t totalData = 0;
    Float_t totalMC = 0;

    std::map< std::string, SampleGroup >::iterator sit;

    for (sit = _sampleGroups.begin(); sit != _sampleGroups.end(); sit++) {
        SampleGroup& sampleGroup = sit->second;
        Float_t POTdata, POTmc;
        sampleGroup.GetPOT(POTdata, POTmc);
        totalData += POTdata;
        totalMC += POTmc;
    }

    return totalMC / totalData;
}


//**************************************************
void Experiment::Dump() {
//**************************************************

    std::map< std::string, SampleGroup >::iterator sit;
    for (sit = _sampleGroups.begin(); sit != _sampleGroups.end(); sit++) {
        SampleGroup& sampleGroup = sit->second;

        std::vector<DataSample*>& dataSamples = sampleGroup.GetDataSamples();
        std::vector<DataSample*>::iterator it;
        for (it = dataSamples.begin(); it != dataSamples.end(); it++) {
            DataSample* sample = *it;
            sample->DumpPOT();
        }      

        std::map< std::string, DataSample*>& mcSamples = sampleGroup.GetMCSamples();
        std::map< std::string, DataSample*>::iterator it2;
        for (it2 = mcSamples.begin(); it2 != mcSamples.end(); it2++) {
            std::string name = it2->first;
            DataSample* sample = it2->second;      
            sample->DumpPOT();
        }      
    }


}
