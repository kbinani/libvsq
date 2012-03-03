#ifndef __EventWriteOption_h__
#define __EventWriteOption_h__

#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

class EventWriteOption
{
public:
    bool length;
    bool note;
    bool dynamics;
    bool pmBendDepth;
    bool pmBendLength;
    bool pmbPortamentoUse;
    bool demDecGainRate;
    bool demAccent;
    bool preUtterance;
    bool voiceOverlap;

    EventWriteOption()
    {
        length = true;
        note = true;
        dynamics = true;
        pmBendDepth = true;
        pmBendLength = true;
        pmbPortamentoUse = true;
        demDecGainRate = true;
        demAccent = true;
        preUtterance = false;
        voiceOverlap = false;
    }
};

VSQ_END_NAMESPACE

#endif
