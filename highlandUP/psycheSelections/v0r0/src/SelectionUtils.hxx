#ifndef SelectionUtils_h
#define SelectionUtils_h

#include "CutUtils.hxx"
#include "EventBoxId.hxx"

namespace SelUtils{

  /// Fill the leading tracks in the event
  bool FindLeadingTracks(AnaEventC& event, ToyBoxB& ToyBox);
}

#endif
