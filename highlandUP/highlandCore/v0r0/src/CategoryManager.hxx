#ifndef CategoryManager_h
#define CategoryManager_h

#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <HLClonesArray.hxx>

#include "CategoryClasses.hxx"

/// This class manages TrackCategoryDefinition objects. It handles the creation
/// and usage of the categories, as well as reading and writing them in the
/// "config" header so they can be used in the DrawingTools.
///
/// See TrackCategoryDefinition for more details on track categories and their
/// usage in the creation of stacked histograms.
///

/// Maximum number of categories that can be added
const UInt_t NMAXCATEG=100;

const UInt_t NMAXCATEGCOUNTERS = 10;


// names for common types
const std::string NAMESAND    = "sand #mu";
const std::string NAMEOUTFV   = "out FV";
const std::string NAMENOTRUTH = "no truth";
const std::string NAMEOTHER   = "other";
const std::string NAME2P2H    = "2p2h";
const std::string NAMEBKG     = "BKG";

// codes for common types
const int CATSAND    = 777;
const int CATOUTFV   = 7;
const int CATNOTRUTH = -1;
const int CATOTHER   = 999;
const int CAT2P2H    = 9;
const int CATBKG     = 888;

// colors for common types
const int COLSAND    = 51;
const int COLOUTFV   = 1;
const int COLNOTRUTH = 92;
const int COLOTHER   = 48;
const int COL2P2H    = 874;
const int COLBKG     = 6;




class CategoryManager: public HLClonesArray {

  public:

    /// Private constructor as this is a singleton.
    CategoryManager();

    virtual ~CategoryManager() {}

    /// Reset the properties of the current track.
    void ResetCurrentCategories();

    /// Add a new track category. This will be automatically saved in the
    /// "config" tree, so can be used in the DrawingTools.
    ///
    /// name:   The name of this category (for example, "particle").
    /// ntypes: The number of types this category permits.
    /// names:  The names of the types this category permits (e.g. "muon"...).
    /// codes:  The codes that associate a track with a given type. See SetCode().
    /// colors: The colors to  be used when drawing the stacked histogram.
    /// multi:  Whether the category should allow multiple types to coexist.
    /// noWarning: avoid warning when replacing properties,
    ///            used for drawing with % on the legend (with PUR option)
    /// addNOTRUTH: atutomatically add the NOTRUTH type
    /// addSAND: atutomatically add the SAND muon type
    /// The array variables should all be ntypes in length.
    void AddCategory(const std::string& name, int ntypes, std::string* names, int* codes, int* colors, bool multi = false, bool noWarning = false, bool addNOTRUTH=true, bool addSAND=true);

    /// Copy an existing Category into another with a  different name
    void CopyCategory(const std::string& categ_name, const std::string& categ_name2);

    /// Has this category been added?
    bool HasCategory(const std::string& categ);

    /// Build the categories from a root file.
    void ReadCategories(const std::string& file);

    /// Get the map of track categories.
    std::map<std::string, TrackCategoryDefinition*>& GetCategories() {
      return _track_category_map;
    }

    /// Dump the map of track categories.
    void DumpCategories();

    /// Dump the options stored for the given category.
    void DumpCategory(const std::string& categ);

    /// Get a specific category.
    TrackCategoryDefinition& GetCategory(const std::string& categ) {
      return *_track_category_map[categ];
    }

    /// Get the vector of track types in a given category.
    std::vector<TrackTypeDefinition>& GetCategoryTypes(const std::string& categ) {
      return _track_category_map[categ]->GetCategoryTypes();
    }

    /// Set the actual code for this category. If the specified code
    /// isn't defined for the category, defaultcode is used instead.
    void SetCode(const std::string& categ, int code, int defaultcode = 0) {
      if (HasCategory(categ)) {
        GetCategory(categ).SetCode(code, defaultcode);
      } else {
        std::cout << "Code not set: category '" << categ << "' not found !!!" << std::endl;
      }
    }

    /// A category can fulfill multiple types. Set the different bits.
    /// For multitype categories only.
    void SetCategoryType(const std::string& categ, int type, bool ok) {
      if (HasCategory(categ)) {
        GetCategory(categ).SetCategoryType(type, ok);
      }
    }

    /// Get the actual code for this category.
    int GetCode(const std::string& categ);

    /// Check is the specified type is true for a given category (for multitype
    /// categories only)
    bool CheckCategoryType(const std::string& categ, int index);

    /// Whether the categories are ready.
    bool IsReady() const {
      return _ready;
    }

    /// Set that the categories are ready.
    void SetReady(bool ok) {
      _ready = ok;
    }

  protected:

    /// The internal map of categories and the names they were registered with.
    std::map<std::string, TrackCategoryDefinition*> _track_category_map;

    /// Whether the categories are ready.
    bool _ready;

};

namespace anaUtils{

  extern CategoryManager* _categ;

}

#endif
