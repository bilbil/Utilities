#pragma once

#include <time.h>
#include <windows.h>
#include <vector>
#include <string>
#include "ThreadState.h"

using namespace std;

class CSpawnProcess
{
public:
    CSpawnProcess();
    ~CSpawnProcess();
    bool SignalReset();
    bool SignalRun();
    bool SignalPause();
    static HANDLE m_hMutex; /// lock for non-threadsafe access
    static DWORD CALLBACK ProcessTask( LPVOID lpParam ); /// task processing thread
    bool Terminate(); ///end ProcessTask thread
    void Lock();
    void Unlock();
    void GetOutput( string & strOutput );
    void GetRetVal( int & iRetVal );
    void SetProcessName( string strProcessName );
    void SetProcessArgument( string strProcessArg );
    virtual bool cbTerminated(){ return false; } /// called when thread exits normally
protected:
    bool m_bProcessTaskRun;
    bool m_bProcessTaskTerminate;
    string m_strProcessName;
    string m_strProcessArg;
    string m_strOutput;
    int m_iRetVal;
};
