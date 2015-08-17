#pragma once

#include "LogTail.h"
#include "FilterText.h"

#include <windows.h>

class CLogTailFilterImpl : public CLogTail, public CFilterText
{
public:
    CLogTailFilterImpl();
    ~CLogTailFilterImpl();
    bool SignalReset();
    bool SignalRun();
    bool SignalPause();
    static DWORD CALLBACK ProcessTask( LPVOID lpParam ); /// task processing thread
    bool Terminate(); ///end ProcessTask thread
    void Lock();
    void Unlock();
    static HANDLE m_hMutex; /// lock for non-threadsafe access
    virtual bool cbTerminated(){ return false; } /// called when thread exits normally
private:
    bool m_bProcessTaskRun;
    bool m_bProcessTaskTerminate;
};
