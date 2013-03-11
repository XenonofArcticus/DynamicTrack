

#include "DynamicTrack.h"

cDTParameter::cDTParameter(cDynamicTrack *aDynamicTrack)
    : DynamicTrack(aDynamicTrack),
      DataFieldInterface(0),
      DataFieldOffset(0) {
}

cDTStringParameter::cDTStringParameter(std::string aParameterName,
                                       cDynamicTrack &aDynamicTrack)
    : cDTParameter(&aDynamicTrack) {
    aDynamicTrack.AddParameter(aParameterName, this);
}

cDTStringParameter::~cDTStringParameter() {
    DynamicTrack->RemoveParameter(this);
}


