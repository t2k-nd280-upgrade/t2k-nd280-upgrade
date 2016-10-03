#include <Interaction.hxx>
#include <BasicUtils.hxx>
#include "stdlib.h"

Interaction::Interaction(){

  parentPDG = 0;
  parent=NULL;
  
}


Interaction::Interaction(AnaTrueParticleB* traj, AnaTrueParticleB* parent2){

  parentPDG = traj->ParentPDG;
  parentID = traj->ParentID;
  parent = parent2;
  for(int i = 0; i < 4; ++i){
      position[i] = traj->Position[i];
  }

  AddTrajectory(traj, parent2);
}

void Interaction::AddTrajectory(AnaTrueParticleB* traj, AnaTrueParticleB* parent2){

  trajectories.push_back(traj);  
  if (parent != parent2) std::cout << "Interaction::AddTrajectory().WARNING: add trajectory with a different parent !!!" << std::endl;
}

Bool_t Interaction::IncludesTrajectory(AnaTrueParticleB* traj){

  return traj->Position[3] == position[3];

}

Int_t Interaction::NumberOfParticleType(Int_t pdg){

  Int_t NParticle = 0;

  for (unsigned int i = 0; i < trajectories.size(); i++){

    if (trajectories[i]->PDG == pdg){

      NParticle++;

    }
  }

  return NParticle;

}

std::vector<UInt_t> Interaction::IndicesOfParticleType(Int_t pdg){

  std::vector<UInt_t> result;

  for (UInt_t i = 0; i < trajectories.size(); i++){

    if (trajectories[i]->PDG == pdg){
      result.push_back(i);
    }
  }

  return result;
}


Int_t Interaction::NPiZeroFromDecayProducts(){

  Int_t NPi0Decays = 0;

  //Get the indices of gammas, e+ and e-.
  std::vector<UInt_t> gammaIndices = IndicesOfParticleType(22);
  std::vector<UInt_t> ePlusIndices = IndicesOfParticleType(-11);
  std::vector<UInt_t> eMinusIndices = IndicesOfParticleType(11);


  //First, add up the 4-momentum of e+e- pairs.
  std::vector<TLorentzVector> momEPlusEMinusPairs;
  for (UInt_t em = 0; em < eMinusIndices.size(); em++){
    for(UInt_t ep = 0; ep < ePlusIndices.size(); ep++){
      
      TVector3 eMinusDir(trajectories[eMinusIndices[em]]->Direction[0],trajectories[eMinusIndices[em]]->Direction[1], trajectories[eMinusIndices[em]]->Direction[2]);
      TVector3 ePlusDir(trajectories[ePlusIndices[ep]]->Direction[0], trajectories[ePlusIndices[ep]]->Direction[1], trajectories[ePlusIndices[ep]]->Direction[2]);

      double eMinusMom = trajectories[eMinusIndices[em]]->Momentum;
      double ePlusMom = trajectories[ePlusIndices[em]]->Momentum;
      
      TLorentzVector eMinusFourMom = GetFourMomentum(eMinusDir,eMinusMom,0.5109989);
      TLorentzVector ePlusFourMom = GetFourMomentum(ePlusDir,ePlusMom,0.5109989);	

      momEPlusEMinusPairs.push_back(eMinusFourMom + ePlusFourMom);
    }
  }

  //Now check for a gamma and a e+e- pair, the other decay mode (~1% of the time.)
  for(UInt_t gi = 0; gi < gammaIndices.size(); gi++){
    for(UInt_t pi = 0; pi < momEPlusEMinusPairs.size(); pi++){

      TVector3 gammaDir(trajectories[gammaIndices[gi]]->Direction[0], trajectories[gammaIndices[gi]]->Direction[1], trajectories[gammaIndices[gi]]->Direction[2]);
      double gammaMom = trajectories[gammaIndices[gi]]->Momentum;

      TLorentzVector gammaFourMom = GetFourMomentum(gammaDir,gammaMom,0.0);

      TLorentzVector sumOfMoms =  gammaFourMom + momEPlusEMinusPairs[pi];
      
      Double_t invariantMassofPair  = sumOfMoms.M();
      if (invariantMassofPair > 134.96 && invariantMassofPair < 135.00){
	NPi0Decays++;
      }
    }
    
  }

  //Finally, check the gamma gamma decay mode.
  if(gammaIndices.size() > 1){
    for (UInt_t gi = 0; gi < gammaIndices.size() - 1; gi++){

      TVector3 gamma1Dir(trajectories[gammaIndices[gi]]->Direction[0], trajectories[gammaIndices[gi]]->Direction[1], trajectories[gammaIndices[gi]]->Direction[2]);
      double gamma1Mom = trajectories[gammaIndices[gi]]->Momentum;
      TLorentzVector gamma1FourMom = GetFourMomentum(gamma1Dir,gamma1Mom,0.0);

      for (UInt_t gj = gi + 1; gj < gammaIndices.size(); gj++){

	TVector3 gamma2Dir(trajectories[gammaIndices[gj]]->Direction[0], trajectories[gammaIndices[gj]]->Direction[1], trajectories[gammaIndices[gj]]->Direction[2]);
	double gamma2Mom = trajectories[gammaIndices[gj]]->Momentum;
	TLorentzVector gamma2FourMom = GetFourMomentum(gamma2Dir,gamma2Mom,0.0);


	TLorentzVector sumOfPhotonMoms = gamma1FourMom + gamma2FourMom;
	
	//If the invariant mass indicates a pi0, increment the NGammaGammaPi0 counter.
	Double_t invariantMassofPair = sumOfPhotonMoms.M();
	if (invariantMassofPair > 134.96 && invariantMassofPair < 135.00){
	  NPi0Decays++;
	}
	
      }
      
    }
  }
  
  //With all decay modes checked, return the number of Pi0 decays found.
  return NPi0Decays;

}


//For use in comparing two Interaction objects so can sort a vector of them according to the time
//they occurred at (from first to last.)
Bool_t CompareInteractions(Interaction a, Interaction b){
    
  return a.position[3] < b.position[3];

}

//Takes in the direction and momentum from highland, and a use supplied mass, and returns the
//4 momentum.
TLorentzVector GetFourMomentum(TVector3 direction, double momentum, Double_t mass){

  TVector3 momVec = momentum*direction;

  Double_t E = sqrt(momVec.Mag2() + pow(mass,2));

  TLorentzVector fourMom(momVec.Px(),momVec.Py(),momVec.Pz(),E);

  return fourMom;


}

//Counts all exotic particles, for shunting those events to "Other".
Int_t Interaction::CountExoticParticles(){

  Int_t NExotics = 0;

  for (unsigned int i = 0; i < trajectories.size(); i++){

    //Exotics are anything that we wouldn't expect
    //in the oaAnalysis saved trajectories.
    //i.e. not electrons, muons, pions, photons or nucleons.
    //Remember: neutrino trajectories are not saved.
    if (trajectories[i]->PDG != 11
	&& trajectories[i]->PDG != -11
	&& trajectories[i]->PDG != 13
	&& trajectories[i]->PDG != -13
	&& trajectories[i]->PDG != 111
	&& trajectories[i]->PDG != 211
	&& trajectories[i]->PDG != -211
	&& trajectories[i]->PDG != 22
	&& trajectories[i]->PDG != 2212
	&& trajectories[i]->PDG != 2112){

      NExotics++;

    }
  }

  return NExotics;

}
