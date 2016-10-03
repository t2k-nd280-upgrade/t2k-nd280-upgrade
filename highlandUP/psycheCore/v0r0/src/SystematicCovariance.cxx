#include "SystematicCovariance.hxx"


//***********************************************************
SystematicCovariance::SystematicCovariance(){
//***********************************************************

}

//***********************************************************
SystematicCovariance::~SystematicCovariance() {
//***********************************************************

}


//********************************************************************
void SystematicCovariance::ConstructCovarianceMatrix(SystematicManager* man){
//********************************************************************
   
   std::cout << " constructing things! " << std::endl;

   int nTotalParameters = 0;

   int nVarSys = 0;
   EventVariationBase** varArr = man->GetVariationSystematics(nVarSys);
   std::cout << "nVarSys = " << nVarSys << std::endl;
   for (int it = 0; it < nVarSys; it++) 
      nTotalParameters += varArr[it]->GetNParameters();

   int nSys = 0;
   EventWeightBase** weightArr = man->GetWeightSystematics(nSys);
   for (int it = 0; it < nSys; it++) 
      nTotalParameters += weightArr[it]->GetNParameters();

   nSys = 0;
   weightArr = man->GetFluxSystematics(nSys);
   for (int it = 0; it < nSys; it++) 
      nTotalParameters += weightArr[it]->GetNParameters();


   std::cout << " Total parameters: " << nTotalParameters << std::endl;
   
   _covarianceMatrix = new TMatrixT<double>(nTotalParameters,nTotalParameters);
   
   nSys = 0;
   for (int it = 0; it < nVarSys; it++) 
      FillLinks(varArr[it]);

   nSys = 0;
   weightArr = man->GetWeightSystematics(nSys);
   for (int it = 0; it < nSys; it++) 
      FillLinks(weightArr[it]);

   nSys = 0;
   weightArr = man->GetFluxSystematics(nSys);
   for (int it = 0; it < nSys; it++) 
      FillLinks(weightArr[it]);

   MapIndices();

   for(unsigned int i=0; i<covariances.size(); i++)
      for(unsigned int j=0; j<covlinks[i].inputindex.size(); j++)
	 for(unsigned int k=0; k<covlinks[i].inputindex.size(); k++)
	    (*_covarianceMatrix)(covlinks[i].covarianceindex[j],covlinks[i].covarianceindex[k]) = (*covariances[i])(covlinks[i].inputindex[j],covlinks[i].inputindex[k]);
   
   for(unsigned int i=0; i<links.size(); i++)
      if(links[i].inputcov == -1)
	 (*_covarianceMatrix)(links[i].covarianceindex,links[i].covarianceindex) = 1;

//   _covarianceMatrix->Print();

}

//********************************************************************
bool SystematicCovariance::IsInList(std::string name, std::vector< std::string > list, int& index){
//********************************************************************
   
   index=0;
   for (std::vector< std::string >::iterator it = list.begin(); it != list.end(); it++, index++) 
      if( (*it) == name)
	 return true;
   return false;
}

//********************************************************************
void SystematicCovariance::FillLinks(SystematicBase* syst){
//********************************************************************
   
   std::cout << "filling links" << std::endl;
   
   TString parname = syst->GetName();
   parname.Remove(0,1);//get rid of the initial "k";
   char paramcov[200];
   sprintf(paramcov,"psycheSystematics.%s.CovFile",parname.Data());
//   std::cout << paramcov << std::endl;
   
   
   //if we don't assign an input covariance file
   if(!ND::params().HasParameter(paramcov))
   {
      for(int i=0; i<(int)syst->GetNParameters(); i++)
      {
	 indexlink temp;
	 temp.covarianceindex = links.size();
	 temp.inputindex = -1;
	 temp.inputcov=-1;
	 links.push_back(temp);
      }
      
   }
   else
   {
      int index;
      if(!IsInList(ND::params().GetParameterS(paramcov),listoffiles,index))
      {
	 listoffiles.push_back(ND::params().GetParameterS(paramcov));
	 char dirname[300];
	 sprintf(dirname,"%s/data/%s",getenv("PSYCHESYSTEMATICSROOT"),ND::params().GetParameterS(paramcov).c_str());
//	 std::cout << dirname << std::endl;
	 TFile file(dirname);
	 
	 char paramcovname[300];
	 sprintf(paramcovname,"psycheSystematics.%s.CovFileCov",parname.Data());
//	 std::cout << paramcovname << std::endl;
	 
	 TMatrixT<double>* tempcov = (TMatrixT<double>*)file.Get(ND::params().GetParameterS(paramcovname).c_str());
	 covariances.push_back(tempcov);
      }
      char paramcovlo[200];
      sprintf(paramcovlo,"psycheSystematics.%s.CovLowIndex",parname.Data());
//      std::cout << paramcovlo << std::endl;
      int loindex = ND::params().GetParameterI(paramcovlo);

      sprintf(paramcovlo,"psycheSystematics.%s.CovHighIndex",parname.Data());
//      std::cout << paramcovlo << std::endl;
      int hiindex = ND::params().GetParameterI(paramcovlo);
      
      if(hiindex-loindex+1 != (int) syst->GetNParameters())
	 std::cerr << " Parameter Number mismatch!!!!!!" << std::endl;
      
      for(int i=0; i<(int)syst->GetNParameters(); i++)
      {
	 indexlink temp;
	 temp.covarianceindex = (int) links.size();
	 temp.inputindex = loindex+i;
	 temp.inputcov=index;
	 links.push_back(temp);
      }
      
   }
      
      

}


//********************************************************************
void SystematicCovariance::MapIndices(){
//********************************************************************

   for(unsigned int i=0; i<covariances.size(); i++)
   {
      covlink temp;
      temp.inputindex = std::vector<double>();
      temp.covarianceindex = std::vector<double>();
      covlinks.push_back(temp);
   }
   
   
   for(std::vector<indexlink>::iterator it = links.begin(); it != links.end(); it++)
   {
      if((*it).inputcov!=-1)
      {
	 covlinks[(*it).inputcov].inputindex.push_back((*it).inputindex);
	 covlinks[(*it).inputcov].covarianceindex.push_back((*it).covarianceindex);
      }
   }



}
