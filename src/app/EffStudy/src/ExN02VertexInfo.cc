#include <G4PrimaryVertex.hh>

#include "ExN02VertexInfo.hh"

ExN02VertexInfo::ExN02VertexInfo()
    : fInformationalVertex(NULL), 
      fInteractionNumber(0), fCrossSection(0.0), fDiffCrossSection(0.0),
      fWeight(1.0), fProbability(1.0) {}

ExN02VertexInfo::~ExN02VertexInfo() {
    if (fInformationalVertex != NULL) delete fInformationalVertex;
}

const G4PrimaryVertex* ExN02VertexInfo::GetInformationalVertex(G4int i) const {
    if (i == 0) {
        return fInformationalVertex;
    }
    if (i<0) return NULL;
    const G4PrimaryVertex* infoVertex = fInformationalVertex;
    for (G4int j=0; j < i; j++) {
        if (infoVertex == NULL) return NULL;
        infoVertex = infoVertex->GetNext();
    }
    return infoVertex;
}

void ExN02VertexInfo::AddInformationalVertex(G4PrimaryVertex* vtx) {
    if (!fInformationalVertex) {
        fInformationalVertex = vtx; 
    }
    else { 
        fInformationalVertex->SetNext(vtx);
    }
}

G4int ExN02VertexInfo::GetNumberOfInformationalVertex() const {
    G4int number = 0;
    const G4PrimaryVertex* infoVertex = fInformationalVertex;
    for (G4int j=0; j < 1000; j++) {
        if (infoVertex == NULL) return number;
        ++number;
        infoVertex = infoVertex->GetNext();
    }
    return number;
}
