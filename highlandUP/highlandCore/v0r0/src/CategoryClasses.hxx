#ifndef CategoryClasses_h
#define CategoryClasses_h

#include <map>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <TObject.h>

/// A TrackTypeDefinition is one of the possiple options for a
/// TrackCategoryDefinition. The "particle" category, for example, has types
/// "muon", "electron" etc.
///
/// This class is very simple. See TrackCategoryDefinition for more details on
/// categories, and their use in drawing stacked histograms.
class TrackTypeDefinition: public TObject {

  public:

    TrackTypeDefinition() {}
    virtual ~TrackTypeDefinition() {}

    /// The name of this type ("muon", "electron", ...). It will be displayed in the legend.
    std::string _name;

    /// The color to be displayed for tracks of this type.
    int _color;

    /// The order in which it should appear when plotting.
    int _order;

    /// The unique code for this type ("muon==13", ...).
    int _code;

    /// Is this type enabled?
    bool _enabled;

  ClassDef(TrackTypeDefinition, 1);
};

/// A TrackCategoryDefinition allows the plotting of stacked histograms in the
/// DrawingTools. For example, a histogram may be broken down by the true
/// particle of the selected track. This "particle" category has types such as
/// "electron", "muon" and so on. The "particle" variable in the analysis
/// micro-tree would then be filled with the appropriate code.
///
/// Categories are managed using the CategoryManager, and are saved to the
/// "config" tree of the output file. This means that custom categories can be
/// defined by the analyser, and automatically used in the DrawingTools. See
/// CategoryManager::AddTrackCategory() for how to do this.
class TrackCategoryDefinition: public TObject {

  public:

    /// Create a new track category. Setting "multi" to true means that several
    /// types can coexist.
    TrackCategoryDefinition(bool multi = false) {
      _isMultiType = multi;
    }

    virtual ~TrackCategoryDefinition() {}

    /// Number of types defined for this category.
    unsigned int GetNTypes() {
      return _types.size();
    }

    /// Get the types defined for this category.
    std::vector<TrackTypeDefinition>& GetCategoryTypes() {
      return _types;
    }

    /// Add a new type to this category.
    void AddType(TrackTypeDefinition& type) {
      _types.push_back(type);
    }

    /// Is this a multi-type category ? (Can several types coexist?)
    bool IsMultiType() {
      return _isMultiType;
    }

    /// Get the code for the actual track in this category.
    int GetCode() {
      return _code;
    }

    /// Check if the actual track is of this type
    bool CheckCategoryType(int index) {
      return _typesOK[index];
    }

    /// Set the code for the actual track. Use defaultcode if code isn't
    /// defined for this category.
    void SetCode(int code, int defaultcode = -999) {
      bool ok = false;

      for (std::vector<TrackTypeDefinition>::iterator it = _types.begin(); it != _types.end(); it++) {
        if (it->_code == code) {
          ok = true;
          break;
        }
      }

      if (!ok) {
        code = defaultcode;
      }

      _code = code;
    }

    /// Set the type for the actual track.
    void SetCategoryType(int index, bool ok) {
      _typesOK[index] = ok;
    }

    void Reset(){_types.clear(); _isMultiType=false;}

    
    // Get an array with all type names for this category
    std::string* GetNames(std::string names[]);

    // Get an array with all colors for this category
    Int_t* GetColors(Int_t colors[]);

    // Get an array with all codes for this category
    Int_t* GetCodes(Int_t codes[]);


  protected:

    /// The types for the actual track.
    std::vector<bool> _typesOK; //!

    /// The code for the actual track.
    int _code; //!

  public:

    /// The name of this category (e.g. "particle").
    std::string _name;

    /// Whether this is this a multi-type category (can several types coexist?).
    bool _isMultiType;

    /// The types defined for this category.
    std::vector<TrackTypeDefinition> _types;

  ClassDef(TrackCategoryDefinition, 1);
};

//typedef TrackTypeDefinition     CategoryTypeDefinition;
//typedef TrackCategoryDefinition CategoryDefinition ;

#endif
