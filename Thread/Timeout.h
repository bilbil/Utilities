#pragma once

#include <time.h>
#include <windows.h>
#include <vector>
#include <string>

class CGuiLauncher;
class CSysProcessor;

class CTimeout
{
public:
    CTimeout();
    ~CTimeout();
    bool SetTimeout( int iDuration_ms );
    bool Reset();
    bool Start();
    bool Pause();
    static HANDLE m_hMutex; // lock for non-threadsafe access
    bool IsTimeout();  /// return true if time duration had passed
    /// task processing thread
    static DWORD CALLBACK ProcessTask( LPVOID lpParam );     /// processing thread
    bool Terminate(); ///end ProcessTask thread
    void Lock();
    void Unlock();
private:
    int m_iDuration_ms;
    bool m_bProcessTaskRun;
    bool m_bProcessTaskTerminate;
    bool m_bIsTimeout;
};
