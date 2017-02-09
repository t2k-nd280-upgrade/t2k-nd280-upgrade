#ifndef numuCC4piAnalysis_h
#define numuCC4piAnalysis_h

//#include "numuCC4piCanSelection.hxx"
#include "numuCC4piECalCanSelection.hxx"
#include "numuCC4piFwdCanSelection.hxx"
#include "numuCC4piBwdCanSelection.hxx"
#include "numuCC4piSimpleSelection.hxx"

#include "baseAnalysis.hxx"

class numuCC4piAnalysis: public baseAnalysis {
public:
	numuCC4piAnalysis(AnalysisAlgorithm* ana = NULL);
	virtual ~numuCC4piAnalysis() {}

	//---- These are mandatory functions
	void DefineSelections();
	void DefineCorrections();
	void DefineSystematics();
	void DefineConfigurations();
	void DefineMicroTrees(bool addBase = true);
	void DefineTruthTree();
	void FillMicroTrees(bool addBase = true);
	void FillToyVarsInMicroTrees(bool addBase = true);
	bool CheckFillTruthTree(const AnaTrueVertex & vtx);
	// Return the vertex from the ToyBox (some ToyBox derived classes could not have that member)
	virtual AnaVertexB * GetVertex() const { return new AnaVertexB();} //box().Vertex;};

	// Return the true vertex from the ToyBox (some ToyBox derived classes could not have that member)
	virtual AnaTrueVertexB * GetTrueVertex() const { return new AnaTrueVertexB();} //box().TrueVertex;};

	using baseAnalysis::FillTruthTree;
	using baseAnalysis::FillTruthTreeBase;
	void FillTruthTree(const AnaTrueVertex& vtx);
	void FillTruthTreeBase(const AnaTrueVertex& vtx, bool IsAntinu = false);

	bool Initialize();
	void AddCategories();
	void FillCategories();

	const ToyBoxCC4piSimple& cc4pibox(Int_t isel = -1) {return *static_cast<const ToyBoxCC4piSimple*>(&boxB(isel));}

public:

	enum enumStandardMicroTrees_numuCC4piAnalysis {
		truelepton_mom = enumStandardMicroTreesLast_baseAnalysis + 1,
		truelepton_PDG,
		truelepton_dir,
		truelepton_costheta,
		truelepton_pos,
		truelepton_fgdlayer,

		NLowAngle,
		NHighAngle,
		NFwdMuLike,
		NBwdMuLike,
		NHAFwdMuLike,
		NHABwdMuLike,
		Toy_CC4pi,
		selmu_HMNT,
		selmu_mom,
		selmu_MomRangeMuon,
		selmu_costheta,
		selmu_ToF_PoD,
		selmu_ToF_ECAL,
		selmu_ToF_FGD2,
		selmu_likemu,
		selmu_likemip,
		selmu_end_ecal,

		selmu_PDG,
		selmu_ID,
		selmu_truemom,
		selmu_truedir,
		selmu_truepos,
		selmu_trueendpos,
		selmu_truefgdlayer,

		selmu_detectors,
		selmu_charge,
		selmu_dir,
		selmu_enddir,
		selmu_pos,
		selmu_endpos,
		selmu_fgdlayer,

		selmu_1hit_pos,
		selmu_2hit_pos,
		selmu_1hit_charge,
		selmu_2hit_charge,
		selmu_1hit_fitpos,
		selmu_2hit_fitpos,
		selmu_1hit_deltapos,
		selmu_2hit_deltapos,
		selmu_1hit_deltapos2,
		selmu_2hit_deltapos2,

		selmu_pod_nnodes,

		selmu_ntpcs,
		selmu_tpc_det,
		selmu_tpc_nnodes,
		selmu_tpc_charge,

		selmu_ntargets,
		selmu_target_det,
		selmu_target_nnodes,
		selmu_target_x,

		selmu_necals,
		selmu_ecal_det,
		selmu_ecal_nnodes,
		selmu_ecal_length,
		selmu_ecal_EMenergy,
		selmu_ecal_mipem,
		/*
					NtargetSec,
					FGDSec_ToF_PoD,
					FGDSec_ToF_ECAL,
					FGDSec_ToF_FGD2,

					NTPCSec,
					TPCSec_MomGlobal,
					TPCSec_pos,
					TPCSec_endpos,
					TPCSec_ToF_PoD,
					TPCSec_ToF_ECAL,
					TPCSec_ToF_FGD2,
		*/

					    true_pdg,
    true_SDlength,
    true_Edep,
    true_mom,
    true_costheta,
    true_phi,
    true_charge,
    true_ekin,
    true_part,
    true_parentID,
    true_Nu_pdg,
    true_Nu_mom,
    true_reaction_code,
 true_vertex_position,
		enumStandardMicroTreesLast_numuCC4piAnalysis
	};


};

#endif
