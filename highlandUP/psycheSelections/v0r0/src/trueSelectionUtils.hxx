#ifndef trueSelectionUtils_h
#define trueSelectionUtils_h

#include "CutUtils.hxx"
#include "EventBoxId.hxx"

namespace trueSelUtils{

  /// Fill the leading tracks in the event
  bool FindLeadingTracks(AnaEventC& event, ToyBoxB& ToyBox);
}

#endif
