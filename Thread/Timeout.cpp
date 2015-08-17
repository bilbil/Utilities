#include "stdafx.h"

#include "Timeout.h"

#include <vector>
#include <string>
#include <iostream>
using namespace std;

/// static member initialization
HANDLE CTimeout::m_hMutex = CreateMutex( NULL, FALSE, NULL );

CTimeout::CTimeout()
{
    m_iDuration_ms = 1000;  // default duration
    m_bProcessTaskRun = false;
    m_bProcessTaskTerminate = false;
    m_bIsTimeout = false;
}

CTimeout::~CTimeout()
{
    Terminate();
}

bool CTimeout::SetTimeout( int iDuration_ms )
{
    if( 0 > iDuration_ms )
    {
        return false;
    }
    m_iDuration_ms = iDuration_ms;
    return true;
}

bool CTimeout::Reset()
{
    Lock();
    m_bProcessTaskRun = false;
    m_bProcessTaskTerminate = false;
    m_bIsTimeout = false;
    Unlock();
    return true;
}

bool CTimeout::Start()
{
    Lock();
    m_bProcessTaskRun = true;
    Unlock();
    return true;
}

bool CTimeout::Pause()
{
    Lock();
    m_bProcessTaskRun = false;
    Unlock();
    return true;
}

void CTimeout::Lock()
{
    WaitForSingleObject( m_hMutex, INFINITE);
}

void CTimeout::Unlock()
{
    ReleaseMutex(m_hMutex);
}

DWORD CTimeout::ProcessTask( LPVOID lpParam )
{
    CTimeout * Data  = ( CTimeout * ) lpParam;
    while(1)
    {
        if( Data->m_bProcessTaskTerminate )
        {
            return 0;
        }
        if( !Data->m_bProcessTaskRun )
        {
            continue;
        }
        // sleep for a set duration
        Sleep( Data->m_iDuration_ms );

        Data->Lock();
        Data->m_bIsTimeout = true;
        Data->Unlock();
    }
}

bool CTimeout::IsTimeout()
{
    if( m_bIsTimeout )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CTimeout::Terminate()
{
    Lock();
    m_bProcessTaskTerminate = true;
    Unlock();
    return true;
}
