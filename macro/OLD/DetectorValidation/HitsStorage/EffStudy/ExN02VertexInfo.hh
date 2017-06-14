
//
// This class has been copied from nd280mc code: ND280VertexInfo
// A few minor changes:
// - Include G4PrimaryVertex class
//

#ifndef ExN02VertexInfo_hh_seen
#define ExN02VertexInfo_hh_seen

#include <G4VUserPrimaryVertexInformation.hh>
#include <G4PrimaryVertex.hh>

/// Hold information that is associated with each primary vertex.  This holds
/// the particle reaction code that generated the vertex, the name of the
/// generator that created the vertex, and internal information from the
/// kinematics generator about the physics that generated the vertex.
class ExN02VertexInfo : public G4VUserPrimaryVertexInformation {
public:
    explicit ExN02VertexInfo();
    virtual ~ExN02VertexInfo();

    /// Set the generator name.  This is set in ExN02PrimaryGenerator after
    /// the vertex is generated so it should not be set when the vertex is
    /// created in a kinematics generator.
    void SetName(const G4String& name) {fName = name;}

    /// Get the generator name.  The name has the format of
    /// <kine>:<count>@<pos>-<time> where <kine> is the kinematics generator,
    /// <count> is the count generator, <pos> is the position generator, and
    /// <time> is the time generator.
    G4String GetName() const {return fName;}

    /// Set reaction that created this vertex.  This is defined by each
    /// kinematics generator.
    /// \todo We need to define a standard format for the reaction names.
    void SetReaction(const G4String& r) {fReaction = r;}

    /// Get the reaction code that created this vertex.
    const G4String& GetReaction() const {return fReaction;}

    /// Set reaction number (NEUT-like). Available both in GENIE and NEUT 
    void SetReactionNum(G4int num) {fReactionNum = num;} 

    /// Get the reaction number (NEUT-like) that created this vertex
    G4int GetReactionNum() {return fReactionNum;};

    /// Set the file that this vertex came from.  
    void SetFilename(const G4String& f) {fFilename = f;}

    /// Get the file that this vertex came from.
    const G4String& GetFilename() {return fFilename;}

    /// Set the index of the interaction within the input interaction file.
    void SetInteractionNumber(G4int i) {fInteractionNumber = i;}

    /// Get the index of the interaction within the input interaction file.
    G4int GetInteractionNumber() const {return fInteractionNumber;}

    /// Set the cross section for the reaction that created this vertex.  This
    /// should be in normal HEP units.
    void SetCrossSection(G4double xs) {fCrossSection = xs;}

    /// Get the cross section for the reaction that created this vertex.
    G4double GetCrossSection() const {return fCrossSection;}

    /// Set the differential cross section for the kinematics that created
    /// this vertex.  This should be in normal HEP units.
    void SetDiffCrossSection(G4double xs) {fDiffCrossSection = xs;}
    
    /// Get the differential cross section for the kinematics of the reaction
    /// that created this vertex.
    G4double GetDiffCrossSection() const {return fDiffCrossSection;}

    /// Set the weight for the vertex.  This is used if the generator is using
    /// a weighted sample (e.g. over sampling the high energy part of the
    /// neutrino spectrum smooth out the distributions).
    void SetWeight(G4double w) {fWeight = w;}
    
    /// Get the weight of the vertex.  This will be one if it's not filled.
    G4double GetWeight() const {return fWeight;}

    /// Set the probability of the interaction that created this vertex.  This
    /// includes the effect of the cross section, path length through the
    /// material, etc.
    void SetProbability(G4double p) {fProbability = p;}

    /// Get the probability of the interaction.
    G4double GetProbability() const {return fProbability;}

    /// Return the informational vertices that are associated with this event.
    /// These vertices usually contain information associated with the primary
    /// interaction, such as the incoming neutrino and nucleon.
    virtual const G4PrimaryVertex* GetInformationalVertex(G4int i = 0) const;

    /// Add an informational vertex to this event.
    virtual void AddInformationalVertex(G4PrimaryVertex* vtx);

    /// Return the number of informational vertices.
    virtual G4int GetNumberOfInformationalVertex() const;

    /// Required by G4UserPrimaryVertexInformation.
    void Print() const {};
   
private:
    /// The generator name.  
    G4String fName;

    /// The reaction code as defined by the kinematics generator.
    G4String fReaction;

    /// The reaction code number as defined by the kinematics generator.
    G4int fReactionNum;   
  
    /// The list of informational vertices associated with this primary
    /// vertex.
    G4PrimaryVertex* fInformationalVertex;

    /// The file that this vertex came from.
    G4String fFilename;

    /// The index of the interaction in the input neutrino file.
    G4int fInteractionNumber;

    /// The total cross section of the interaction creating this vertex.  This
    /// is zero if it's not filled.
    G4double fCrossSection;

    /// The differential cross section of the interaction kinematics creating
    /// this vertex.  This is zero if it's not filled.
    G4double fDiffCrossSection;
    
    /// The weight for the interaction creating this vertex.  This is one if
    /// it's not filled.
    G4double fWeight;

    /// The overall probability of this interaction given the cross sections,
    /// path lengths through materials, &c).  This is one if it is not filled.
    G4double fProbability;
};
#endif
