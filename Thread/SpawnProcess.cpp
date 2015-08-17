#include "stdafx.h"

#include "SpawnProcess.h"
#include "ThreadState.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

/// static member initialization
HANDLE CSpawnProcess::m_hMutex = CreateMutex( NULL, FALSE, NULL );

CSpawnProcess::CSpawnProcess()
{
    m_bProcessTaskRun = false;
    m_bProcessTaskTerminate = false;
    m_strProcessName = "";
    m_strProcessArg = "";
    m_strOutput = "";
    m_iRetVal = -1;
}

CSpawnProcess::~CSpawnProcess()
{
    Terminate();
}

bool CSpawnProcess::SignalReset()
{
    Lock();
    m_bProcessTaskRun = false;
    m_bProcessTaskTerminate = false;
    m_strProcessName = "";
    m_strProcessArg = "";
    m_strOutput = "";
    m_iRetVal = -1;
    Unlock();
    return true;
}

void CSpawnProcess::SetProcessName( string strProcessName )
{
    m_strProcessName = strProcessName;
}

void CSpawnProcess::SetProcessArgument( string strProcessArg )
{
    m_strProcessArg = strProcessArg;
}

bool CSpawnProcess::SignalRun()
{
    Lock();
    m_bProcessTaskRun = true;
    Unlock();
    return true;
}

bool CSpawnProcess::SignalPause()
{
    Lock();
    m_bProcessTaskRun = false;
    Unlock();
    return true;
}

void CSpawnProcess::Lock()
{
    WaitForSingleObject( m_hMutex, INFINITE);
}

void CSpawnProcess::Unlock()
{
    ReleaseMutex(m_hMutex);
}

DWORD CSpawnProcess::ProcessTask( LPVOID lpParam )
{
    CSpawnProcess * Data  = ( CSpawnProcess * ) lpParam;

    while( !Data->m_bProcessTaskRun ){};

    string strCmd = Data->m_strProcessName + " ";
    strCmd += Data->m_strProcessArg;

    FILE* pipe = _popen( strCmd.c_str(), "r" );
    if ( !pipe )
    {
        Data->m_iRetVal = -1;
        Data->cbTerminated();
        return -1;
    }
    if( Data->m_bProcessTaskTerminate )
    {
        Data->cbTerminated();
        return -1;
    }
    Data->m_strOutput = "";
    char buffer[2048];  //get stdout from pipe
    while(!feof(pipe))
    {
		if( Data->m_bProcessTaskTerminate )
		{
			Data->cbTerminated();
			return -1;
		}
        if( NULL != fgets(buffer, 2048, pipe) )
        {
            Data->m_strOutput += buffer;
        }
    }
    Data->Lock();
    Data->m_iRetVal = _pclose( pipe ); //get return code and close process
    Data->Unlock();

    Data->cbTerminated();
	
	Data->m_strOutput.erase(std::remove(Data->m_strOutput.begin(), Data->m_strOutput.end(), '\n'), Data->m_strOutput.end());
	Data->m_strOutput.erase(std::remove(Data->m_strOutput.begin(), Data->m_strOutput.end(), '\r'), Data->m_strOutput.end());

    return 0;
}

void CSpawnProcess::GetOutput( string & strOutput )
{
    strOutput = m_strOutput;
}
void CSpawnProcess::GetRetVal( int & iRetVal )
{
    iRetVal = m_iRetVal;
}

bool CSpawnProcess::Terminate()
{
    Lock();
    m_bProcessTaskTerminate = true;
    Unlock();
    return true;
}
