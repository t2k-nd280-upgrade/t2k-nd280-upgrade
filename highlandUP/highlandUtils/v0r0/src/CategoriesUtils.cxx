#include "ND280UPAnalysisUtils.hxx"
#include "BasicUtils.hxx"
#include "CategoriesUtils.hxx"
#include "TreeConverterUtils.hxx"


// PUT HERE ONLY COMMON CATEGORIES!
// YOU CAN CREATE YOUR OWN CATEGORY ON THE FLY IN A MACRO, LIKE THIS:
// draw.ChangeCategory(<microtree_variable>, <ntypes>, <cat_types>, <cat_codes>, <cat_colors>);
// where the last 3 arguments are vectors similar to those defined here below and ntypes is their lenght

// "no truth" and "sand #mu" types are added automatically to each category (see AddCategory)

std::string nutype_types[] = {"#nu_{#mu}", "#bar{#nu}_{#mu}", "#nu_{e}", "#bar{#nu}_{e}", NAMEOTHER};
int nutype_codes[]         = {14         , -14              , 12       , -12            , CATOTHER};
int nutype_colors[]        = {2          , 3                , 4        , 5              , COLOTHER};
const int NNUTYPE = sizeof(nutype_types)/sizeof(nutype_types[0]);

std::string part_types[] = {"#mu^{-}", "e^{-}", "#pi^{-}", "#mu^{+}", "e^{+}", "#pi^{+}", "p" , NAMEOTHER};
int pdg[]                = {13       , 11     , -211     , -13      , -11    , 211      , 2212, CATOTHER};
int part_colors[]        = {2        , 3      , 4        , 7        , 6      , 31       , 8   , COLOTHER};
const int NPART = sizeof(part_types)/sizeof(part_types[0]);

std::string parent_types[] = {"#mu^{-}", "e^{-}", "#pi^{-}", "#mu^{+}", "e^{+}", "#pi^{+}", "p",  "nu", "gamma", "#pi^{0}", "n" , NAMEOTHER};
int parent_pdg[]           = {13       , 11     , -211     , -13      , -11    , 211      , 2212, 0   , 22     , 111      , 2112, CATOTHER};
int parent_colors[]        = {2        , 3      , 4        , 7        , 6      , 31       , 1   , 8   , 9      , 40       , 95  , COLOTHER};
const int NPARENT = sizeof(parent_types)/sizeof(parent_types[0]);

std::string reac_types[] = {"CCQE", NAME2P2H, "RES", "DIS", "COH", "NC", "#bar{#nu}_{#mu}", "#nu_{e}, #bar{#nu}_{e}", NAMEOTHER, NAMEOUTFV};
int reac_codes[]         = {0     , CAT2P2H , 1    , 2    , 3    , 4   , 5                , 6                       , CATOTHER , CATOUTFV};
int reac_colors[]        = {2     , COL2P2H , 3    , 4    , 7    , 6   , 31               , 65                      , COLOTHER , COLOUTFV};
const int NREAC = sizeof(reac_types)/sizeof(reac_types[0]);

std::string reacCC_types[] = {"CC", "NC", "#bar{#nu}_{#mu}", "#nu_{e}, #bar{#nu}_{e}", NAMEOTHER, NAMEOUTFV};
int reacCC_codes[]         = {1   , 4   , 5                , 6                       , CATOTHER , CATOUTFV};
int reacCC_colors[]        = {2   , 6   , 31               , 65                      , COLOTHER , COLOUTFV};
const int NREACCC = sizeof(reacCC_types)/sizeof(reacCC_types[0]);

std::string reacnofv_types[] = {"CCQE", NAME2P2H,  "RES", "DIS", "COH", "NC", "#bar{#nu}_{#mu}", "#nu_{e}, #bar{#nu}_{e}", NAMEOTHER};
int reacnofv_codes[]         = {0     , CAT2P2H , 1    , 2    , 3    , 4   , 5                , 6                        , CATOTHER};
int reacnofv_colors[]        = {2     , COL2P2H , 3    , 4    , 7    , 6   , 31               , 65                       , COLOTHER};
const int NREACNOFV = sizeof(reacnofv_types)/sizeof(reacnofv_types[0]);

std::string topology_types[] = {"CC-0#pi", "CC-1#pi^{+}", "CC-other", NAMEBKG, NAMEOTHER, NAMEOUTFV};
int topology_codes[]         = {0        , 1            , 2         , 3      , CATOTHER,  CATOUTFV};
int topology_colors[]        = {2        , 4            , 7         , COLBKG , COLOTHER,  COLOUTFV};
const int NTOPOLOGY = sizeof(topology_types)/sizeof(topology_types[0]);

std::string topology_all_types[] = {"#nu_{#mu} CC-0#pi", "#nu_{#mu} CC-1#pi^{+}", "#nu_{#mu} CC-other", 
	"#bar{#nu}_{#mu} CC-0#pi", "#bar{#nu}_{#mu} CC-1#pi^{+}", "#bar{#nu}_{#mu} CC-other", 
	NAMEBKG, NAMEOTHER, NAMEOUTFV};
int topology_all_codes[]         = {0, 1, 2, 3, 4, 5, 6, CATOTHER, CATOUTFV};
int topology_all_colors[]        = {2, 4, 7, 6, 3, 8, COLBKG , COLOTHER,  COLOUTFV};
const int NTOPOLOGYALL = sizeof(topology_all_types)/sizeof(topology_all_types[0]);

std::string mectopology_types[] = {"CC-0#pi-0p", "CC-0#pi-1p", "CC-0#pi-Np", "CC-1#pi^{+}", "CC-other", NAMEBKG, NAMEOUTFV};
int mectopology_codes[]         = {0           , 1           , 2           , 3            , 4         , 5      , CATOUTFV};
int mectopology_colors[]        = {2           , 3           , 4           , 7            , 31        , COLBKG , COLOUTFV};
const int NMECTOPOLOGY = sizeof(mectopology_types)/sizeof(mectopology_types[0]);

std::string target_types[] = {"Carbon", "Oxygen", "Hydrogen", "Aluminium", "Iron", "Copper", "Zinc", "Lead", NAMEOTHER, "#splitline{coh on H}{bugzilla 1056}"};
int target_codes[]         = {6       , 8       , 1         , 13         , 26    , 29      , 30    , 82    , CATOTHER,  0 };
int target_colors[]        = {2       , 4       , 3         , 7          , 6     , 51      , 31    , 5     , COLOTHER,  40};
const int NTARGET = sizeof(target_types)/sizeof(target_types[0]);

//TMP: keep the enumeration from the AnaTrueVertex::Detector (the original oaAnalysis one)
std::string detector_types[] = {"TPC1", "TPC2", "TPC3", "TPCUp1", "TPCUp2", "TPCDown1", "TPCDown2", "FGD1", "FGD2", "Target1", "Target2", "DsECAL", "BrECAL", "P0DECAL", NAMEOTHER};
int detector_codes[] = {2, 3, 4,    // FwdTPC
			0, 1, 5, 6, // HTPC
                        9, 10,      // FGD
			7, 8,       // Target
                        12, 14, 13, // ECal 
			CATOTHER
                        };
int detector_colors[] = {2, 3, 4, 34, 37, 24, 28,
			 9, 46, 7, 6, 30, 31, 32, COLOTHER};
const int NDETECTOR = sizeof(detector_types)/sizeof(detector_types[0]);

//********************************************************************
void anaUtils::AddStandardCategories(const std::string& prefix){
//********************************************************************

  // Add standard categories for color drawing
  _categ->AddCategory(prefix+"particle",           NPART,              part_types,               pdg,                      part_colors);
  _categ->AddCategory(prefix+"parent",             NPARENT,            parent_types,             parent_pdg,               parent_colors);
  _categ->AddCategory(prefix+"gparent",            NPARENT,            parent_types,             parent_pdg,               parent_colors);
  _categ->AddCategory(prefix+"primary",            NPARENT,            parent_types,             parent_pdg,               parent_colors);

  _categ->AddCategory(prefix+"nutype",             NNUTYPE,            nutype_types,             nutype_codes,             nutype_colors);
  _categ->AddCategory(prefix+"nuparent",           NPARENT,            parent_types,             parent_pdg,               parent_colors);
  _categ->AddCategory(prefix+"target",             NTARGET,            target_types,             target_codes,             target_colors);
  _categ->AddCategory(prefix+"detector",           NDETECTOR,          detector_types,           detector_codes,           detector_colors);

  _categ->AddCategory(prefix+"reaction",           NREAC,              reac_types,               reac_codes,               reac_colors);
  _categ->AddCategory(prefix+"reactionCC",         NREACCC,            reacCC_types,             reacCC_codes,             reacCC_colors);
  _categ->AddCategory(prefix+"reactionnofv",       NREACNOFV,          reacnofv_types,           reacnofv_codes,           reacnofv_colors);
  _categ->AddCategory(prefix+"topology",           NTOPOLOGY,          topology_types,           topology_codes,           topology_colors);
  _categ->AddCategory(prefix+"topologyall",        NTOPOLOGYALL,       topology_all_types,       topology_all_codes,       topology_all_colors);
  _categ->AddCategory(prefix+"mectopology",        NMECTOPOLOGY,       mectopology_types,        mectopology_codes,        mectopology_colors);
}

//********************************************************************
void anaUtils::AddStandardAntiNumuCategories(const std::string& prefix){
//********************************************************************

  // replace some naming
  reac_types[6]        = "#nu_{#mu}";
  reacCC_types[2]      = "#nu_{#mu}";
  reacnofv_types[6]    = "#nu_{#mu}";
  mectopology_types[0] = "CC-0#pi-0n";
  mectopology_types[1] = "CC-0#pi-1n";
  mectopology_types[2] = "CC-0#pi-Nn";
  mectopology_types[3] = "CC-1#pi^{-}";
  topology_types[1]    = "CC-1#pi^{-}";

  // Add standard anti-numu categories for color drawing
  AddStandardCategories(prefix);
}

//**************************************************
Int_t anaUtils::GetTopology(const AnaTrueVertex& trueVertex, const SubDetId::SubDetEnum det, bool IsAntinu){
//**************************************************

  /* Classify reaction types
    -1 = no true vertex
     0 = CC 0pi      = mu + X nucleons (+ 0 mesons)
     1 = CC 1pi+(-)  = mu + X nucleons + 1 pion +(-)
     2 = CC other
     3 = BKG: not (anti-)numu, NC
     7 = out of FV
   */

  // out of FGD1 FV
  if ( ! anaUtils::InFiducialVolume(det,trueVertex.Position)) return CATOUTFV;

  /// primary particles from the true vertex
  Int_t Nmuon =     trueVertex.NPrimaryParticles[ParticleId::kMuon];
  Int_t Nantimuon = trueVertex.NPrimaryParticles[ParticleId::kAntiMuon];
  Int_t Nmeson =    trueVertex.NPrimaryParticles[ParticleId::kMesons];
  Int_t Npipos =    trueVertex.NPrimaryParticles[ParticleId::kPiPos];
  Int_t Npineg =    trueVertex.NPrimaryParticles[ParticleId::kPiNeg];

  /*if (trueVertex.ReacCode > 30) {
    std::cout << trueVertex.ReacCode << " " << Nmuon << " " << Nantimuon << std::endl;
	if (Nmuon>0)
		for (int i=0; i<trueVertex.TrueParticlesVect.size(); i++) {
			if (trueVertex.TrueParticlesVect[i]->PDG == 13)
			std::cout << trueVertex.TrueParticlesVect[i]->Momentum << std::endl;
		}
  }*/

  // non numu CC, i.e. BKG
  if (!IsAntinu && Nmuon     <= 0) return BKG; // BKG
  if ( IsAntinu && Nantimuon <= 0) return BKG; // BKG

  // CC 0pi, i.e. mu & 0 mesons
  if (Nmeson == 0) return CC_0pi_0meson;

  // CC 1pi+, i.e. mu & 1 pi & no other mesons
  if (Nmeson == 1) {
    if      (!IsAntinu && Npipos == 1) return CC_1pi_0meson;
    else if ( IsAntinu && Npineg == 1) return CC_1pi_0meson;
  }

  if (trueVertex.ReacCode > 30)
    std::cout << "passed through???" << std::endl;

  // CC other
  return CC_other;
}

//**************************************************
Int_t anaUtils::GetTopologyAll(const AnaTrueVertex& trueVertex, const SubDetId::SubDetEnum det, bool IsAntinu){
//**************************************************

  /* Classify reaction types
    -1 = no true vertex
     0 = numu CC 0pi
     1 = numu CC 1pi+
     2 = numu CC other
     3 = antinumu CC 0pi
     4 = antinumu CC 1pi+
     5 = antinumu CC other
     6 = BKG: not (anti-)numu, NC
     7 = out of FV
   */

  // out of FGD1 FV
  if ( ! anaUtils::InFiducialVolume(det,trueVertex.Position)) return CATOUTFV;

  /// primary particles from the true vertex
  Int_t Nmuon =     trueVertex.NPrimaryParticles[ParticleId::kMuon];
  Int_t Nantimuon = trueVertex.NPrimaryParticles[ParticleId::kAntiMuon];
  Int_t Nmeson =    trueVertex.NPrimaryParticles[ParticleId::kMesons];
  Int_t Npipos =    trueVertex.NPrimaryParticles[ParticleId::kPiPos];
  Int_t Npineg =    trueVertex.NPrimaryParticles[ParticleId::kPiNeg];

  // non numu CC, i.e. BKG
  if (Nmuon <= 0 && Nantimuon <= 0) return 6; // BKG

  if (Nmuon > 0) {
	  if (Nmeson == 0) return 0;
	  if (Nmeson == 1)
		if (Npipos == 1) return 1;
	  return 2;
  }
  
  else {
	  if (Nmeson == 0) return 3;
	  if (Nmeson == 1)
		if (Npineg == 1) return 4;
	  return 5;
  }
}


//**************************************************
Int_t anaUtils::GetMECTopology(const AnaTrueVertex& trueVertex, const SubDetId::SubDetEnum det, bool IsAntinu){
//**************************************************

  /* Classify reaction types
    -1 = no true vertex
     0 = CC 0pi 0p(n) = mu + 0 p(n) (+ 0 mesons)
     0 = CC 0pi 1p(n) = mu + 1 p(n) (+ 0 mesons)
     0 = CC 0pi Np(n) = mu + N>1 p(n) (+ 0 mesons)
     3 = CC 1pi+(-)   = mu + X nucleons + 1 pion +(-)
     4 = CC other : CC 1pi0, CC 1pi-, CCNpi+/-/0
     5 = BKG: not (anti-)numu, NC
     7 = out of FV
  */

  Int_t Nproton = trueVertex.NPrimaryParticles[ParticleId::kProton];
  Int_t Nneutron = trueVertex.NPrimaryParticles[ParticleId::kNeutron];

  // get GetTopology for (anti-)numu
  Int_t topo = GetTopology(trueVertex,det,IsAntinu);

  if (topo == CC_0pi_0meson) {
    Int_t Nnucleon;
    if ( ! IsAntinu) Nnucleon = Nproton;
    else             Nnucleon = Nneutron;
    if (Nnucleon == 0)            return 0; // CC 0pi-0proton
    if (Nnucleon == 1)            return 1; // CC 0pi-1proton
    if (Nnucleon > 1)             return 2; // CC 0pi-Nproton
  }
  else if (topo == CC_1pi_0meson) return 3;
  else if (topo == CC_other)      return 4;
  else if (topo == BKG)           return 5;
  else if (topo == CATOUTFV)      return CATOUTFV;

  else std::cout << "Error in getting topology codes!" << std::endl;
  return -999;
}


//********************************************************************
void anaUtils::FillCategories(AnaEventB* event, AnaParticleB* part, const SubDetId::SubDetEnum det, bool IsAntinu, bool useCATSAND){
//********************************************************************
  FillCategories(event, part,"",det,IsAntinu, useCATSAND);
}

//********************************************************************
void anaUtils::SetCategoriesDefaultCode(const std::string& prefix, const int code){
//********************************************************************

  _categ->SetCode(prefix + "nutype"            , code);
  _categ->SetCode(prefix + "nuparent"          , code);
  _categ->SetCode(prefix + "detector"          , code);
  _categ->SetCode(prefix + "target"            , code);

  _categ->SetCode(prefix + "reaction"          , code);
  _categ->SetCode(prefix + "reactionCC"        , code);
  _categ->SetCode(prefix + "reactionnofv"      , code);
  _categ->SetCode(prefix + "topology"          , code);
  _categ->SetCode(prefix + "topologyall"          , code);
  _categ->SetCode(prefix + "mectopology"       , code);
}

//********************************************************************
void anaUtils::FillCategories(AnaEventB* event, AnaParticleB* part, const std::string& prefix, const SubDetId::SubDetEnum det, bool IsAntinu, bool useCATSAND){
//********************************************************************

  if ( ! part) return;

  // Categories are previously initialized with -999 in CategoryManager::ResetCurrentCategories()

  // Check if this is sandMC (by run number) even if there is no truth association (see bug 1237)
  if (useCATSAND && event->GetIsSandMC()) {
    SetCategoriesDefaultCode(prefix, CATSAND);
    _categ->SetCode(prefix + "particle", CATSAND);
    _categ->SetCode(prefix + "parent"  , CATSAND);
    _categ->SetCode(prefix + "gparent" , CATSAND);
    _categ->SetCode(prefix + "primary" , CATSAND);
    return;
  }

  // First fill with "no truth" value
  SetCategoriesDefaultCode(prefix, CATNOTRUTH);
  _categ->SetCode(prefix + "particle", CATNOTRUTH);
  _categ->SetCode(prefix + "parent"  , CATNOTRUTH);
  _categ->SetCode(prefix + "gparent" , CATNOTRUTH);
  _categ->SetCode(prefix + "primary" , CATNOTRUTH);

  // ----- Particle ------------------------------
  if (part->TrueObject) {

    if (part->GetTrueParticle()->PDG != 0) {
      AnaTrueParticle* truePart = static_cast<AnaTrueParticle*>(part->TrueObject);
      AnaTrueParticleB* primary = anaUtils::GetTrueParticleByID(*event, truePart->PrimaryID);

      if (truePart->TrueVertex) {
        // Must be called before setting the particle category because by default it assumes the particle is the true lepton
        // Overwritten below when there is a candidate
        FillCategories(truePart->TrueVertex,prefix,det,IsAntinu);
      }

      _categ->SetCode(prefix + "particle",  truePart->PDG,        CATOTHER);
      _categ->SetCode(prefix + "parent",    truePart->ParentPDG , CATOTHER);
      _categ->SetCode(prefix + "gparent",   truePart->GParentPDG, CATOTHER);

      if (primary) {
        _categ->SetCode(prefix + "primary", primary->PDG,          CATOTHER);
      }

    }
  }
}

//********************************************************************
void anaUtils::FillCategories(const AnaTrueVertexB* trueVertexB, const std::string& prefix, const SubDetId::SubDetEnum det, bool IsAntinu, bool IsSand){
//********************************************************************

  if ( ! trueVertexB) return;

  // Categories are previously initialized with -999 in CategoryManager::ResetCurrentCategories()

  // if IsSand, assign CATSAND to categories
  if (IsSand){
    SetCategoriesDefaultCode(prefix, CATSAND);
    return;
  }

  SetCategoriesDefaultCode(prefix, CATNOTRUTH);

  const AnaTrueVertex* trueVertex = static_cast<const AnaTrueVertex*>(trueVertexB);

  // this only has effect when called initialy for a truth vertex (for TruthTree)
  _categ->SetCode(prefix + "particle", trueVertex->LeptonPDG       ,CATOTHER);

  _categ->SetCode(prefix + "nutype",   trueVertex->NuPDG         ,CATOTHER);
  _categ->SetCode(prefix + "nuparent", trueVertex->NuParentPDG   ,CATOTHER);

  int Detector_tmp;
  convUtils::ConvertBitFieldToTrueParticleDetEnum(trueVertex->Detector, Detector_tmp);
  if (Detector_tmp==-1) Detector_tmp  = CATOTHER;
  
  _categ->SetCode(prefix + "detector", Detector_tmp              ,CATOTHER);
  _categ->SetCode(prefix + "target",   GetTargetCode(trueVertex) ,CATOTHER);

  _categ->SetCode(prefix + "reaction",          GetReaction(*trueVertex,det,IsAntinu)       ,CATOTHER);
  _categ->SetCode(prefix + "reactionCC",        GetReactionCC(*trueVertex,det,IsAntinu)     ,CATOTHER);
  _categ->SetCode(prefix + "reactionnofv",      GetReactionNoTargetFv(*trueVertex,IsAntinu) ,CATOTHER);
  _categ->SetCode(prefix + "topology",          GetTopology(*trueVertex,det,IsAntinu)       ,CATOTHER);
  _categ->SetCode(prefix + "topologyall",       GetTopologyAll(*trueVertex,det,IsAntinu)    ,CATOTHER);
  _categ->SetCode(prefix + "mectopology",       GetMECTopology(*trueVertex,det,IsAntinu)    ,CATOTHER);
}

//********************************************************************
void anaUtils::FillCategories(const AnaTrueVertexB* trueVertexB, const SubDetId::SubDetEnum det, bool IsAntinu, bool IsSand){
//********************************************************************

  FillCategories(trueVertexB,"",det,IsAntinu,IsSand);
}

//**************************************************
Int_t anaUtils::GetReactionNoTargetFv(const AnaTrueVertex& trueVertex, bool IsAntinu){
//**************************************************
  /* Classify reaction types
    -1 = no true vertex
     0 = CCQE
     1 = CC RES
     2 = CC DIS
     3 = CC COH
     4 = NC
     5 = (anti-)numu
     6 = nue/anti-nue
     999 = other... nutau?
     -1 = no truth
  */

  Int_t reac   = trueVertex.ReacCode;
  Int_t nutype = trueVertex.NuPDG;

 // nue/anti-nue BKG
  if (abs(nutype) == 12)                 return 6;

  // (anti-)nu BKG
  else if (IsAntinu && nutype == 14)     return 5; // nu BKG if antinu
  else if ( ! IsAntinu && nutype == -14) return 5; // antinu BKG if nu

  // (anti-)numu
  else if (abs(nutype) == 14) {
    if (abs(reac) == 1)                  return 0;
    if (abs(reac) == 2)                  return CAT2P2H;
    if (abs(reac) == 70)                 return CAT2P2H;
    if (abs(reac)>10 && abs(reac)<14)    return 1;
    if (abs(reac)>16 && abs(reac)<30)    return 2;
    if (abs(reac) == 16)                 return 3;
    if (abs(reac) > 30)                  return 4;
  }

  return CATOTHER; // nu-tau??
}

//**************************************************
Int_t anaUtils::GetReaction(const AnaTrueVertex& trueVertex, const SubDetId::SubDetEnum det, bool IsAntinu){
//**************************************************

  /* Classify reaction types
     7 = out of FV
     else as GetReactionNoTargetFv
  */

  // out of FV
  if ( ! InFiducialVolume(det, trueVertex.Position)) return CATOUTFV;

  return GetReactionNoTargetFv(trueVertex,IsAntinu);
}

//**************************************************
Int_t anaUtils::GetReactionCC(const AnaTrueVertex& trueVertex, const SubDetId::SubDetEnum det, bool IsAntinu){
//**************************************************

  /* Classify reaction types
     1 = CC (reaction<4 || 2P2H)
     else = GetReaction
  */

  Int_t reac = GetReaction(trueVertex,det,IsAntinu);
  if (reac >= 0 && reac < 4) return 1;
  else if (reac == CAT2P2H)  return 1;
  else return reac;
}

//**************************************************
Int_t anaUtils::GetTargetCode(const AnaTrueVertex* vtx) {
//**************************************************
  Int_t code = -1;

  if (vtx) {
    // Nucleus PDG codes are 10LZZZAAAI, and we want to extract the Z value to
    // identify the element.
    if (vtx->TargetPDG == 2212) // fix for bug in oaAnalysis, see bugzilla 1015
      code = 1;
    else
      code = (vtx->TargetPDG / 10000) % 1000;

  }

  return code;
}

