#pragma once

#include "LogTail.h"
#include "FilterText.h"
#include "ThreadWrapper.h"
#include "LogTailFilterImpl.h"

class CLogTailFilter : public CThreadWrapper< CLogTailFilterImpl >
{
public:
    CLogTailFilter() : CThreadWrapper( CThreadWrapper< CLogTailFilterImpl >::ProcessTask ){}
    bool cbTerminate()
    {
        SignalThreadEnd();
        return true;
    }
};
