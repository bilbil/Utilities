#include "stdafx.h"

#include "LogTailFilterImpl.h"
#include "ThreadWrapper.h"
#include "ThreadState.h"

#include <vector>
using namespace std;

// static member initialization
HANDLE CLogTailFilterImpl::m_hMutex = CreateMutex( NULL, FALSE, NULL );

CLogTailFilterImpl::CLogTailFilterImpl()
{
    m_bProcessTaskRun = false;
    m_bProcessTaskTerminate = false;
}

CLogTailFilterImpl::~CLogTailFilterImpl()
{
    Terminate();
}

bool CLogTailFilterImpl::SignalReset()
{
    Lock();
    m_bProcessTaskRun = false;
    m_bProcessTaskTerminate = false;
    Unlock();
    return true;
}

bool CLogTailFilterImpl::SignalRun()
{
    Lock();
    m_bProcessTaskRun = true;
    Unlock();
    return true;
}

bool CLogTailFilterImpl::SignalPause()
{
    Lock();
    m_bProcessTaskRun = false;
    Unlock();
    return true;
}

DWORD CLogTailFilterImpl::ProcessTask( LPVOID lpParam )
{
    CLogTailFilterImpl * Data  = ( CLogTailFilterImpl * ) lpParam;

    vector< string > filter_emitted;

    while( !Data->m_bProcessTaskRun ){}; //block until flagged to run

    while( !Data->m_bProcessTaskTerminate )
    {
        while( !Data->m_bProcessTaskRun ){}; //check for pause

        //process input log file and filter text
        int update_position;
        int length_total;
        Data->UpdatePositionAndLengthTotal( update_position, length_total );
        string line;
        if( Data->GetLineFromFile( line ) )
        {
            Data->ProcessInputText( line , filter_emitted );
        }
        Sleep(10);
    }

    Data->cbTerminated();

    return 0;
}

bool CLogTailFilterImpl::Terminate()
{
    m_bProcessTaskTerminate = true;
    return true;
}

void CLogTailFilterImpl::Lock()
{
    WaitForSingleObject( m_hMutex, INFINITE);
}

void CLogTailFilterImpl::Unlock()
{
    ReleaseMutex(m_hMutex);
}
