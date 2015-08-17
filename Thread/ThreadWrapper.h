#pragma once

#include <windows.h>

#include "ThreadState.h"

typedef DWORD (CALLBACK * tPtrThreadFunc)( LPVOID );

template < typename Base >
class CThreadWrapper : public Base {
public:
    CThreadWrapper( tPtrThreadFunc ThreadFunc )
    {
        m_ThreadFunc = ThreadFunc;
        m_ThreadState = CThreadState::UNINNIT;
    }
    ~CThreadWrapper()
    {
        Base::Terminate();
        DWORD result = WaitForSingleObject( m_hThread, INFINITE );
    }
    bool MakeThread()
    {
        if( CThreadState::UNINNIT == m_ThreadState || CThreadState::TERMINATED == m_ThreadState )
        {
            m_hThread = CreateThread( 
                NULL,                                  // default security attributes
                0,                                     // use default stack size  
                m_ThreadFunc,                         // function for threading
                (void*) this,                          // pass instance as the argument to thread function 
                CREATE_SUSPENDED,                      // thread starting state
                &m_dwThreadId);                        // returns the thread identifier 

            if (m_hThread == NULL) 
            {
                ExitProcess(3);
            }
            m_ThreadState = CThreadState::INNIT;
            return true;
        }
        else
        {
            return false;
        }
    }
    bool RunThread()
    {
        if( !m_hThread || CThreadState::TERMINATED == m_ThreadState)
        {
            return false;
        }
	    
        if( CThreadState::PAUSED == m_ThreadState || CThreadState::INNIT == m_ThreadState )
        {
            m_ThreadState = CThreadState::RUNNING;
            ResumeThread( m_hThread );
        }
        return true;
    }
    bool SignalThreadEnd() /// should be called by the terminating thread to signal end of thread
    {
        m_ThreadState = CThreadState::TERMINATED;
        m_hThread = 0;
        return true;
    }
    bool SetThreadFunc( tPtrThreadFunc ThreadFunc )
    {
        m_ThreadFunc = ThreadFunc;
        return true;
    }
    bool ForceTerminate()
    {
        if( m_hThread )
        {
            int iRet = TerminateThread( m_hThread, -1 );
            if( iRet != 0 )
            {
                m_ThreadState = CThreadState::TERMINATED;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    CThreadState::State GetThreadState()
    {
        return m_ThreadState;
    }
protected:
    HANDLE m_hThread;   /// handle to thread object
    DWORD m_dwThreadId; /// handle to thread id object
    CThreadState::State m_ThreadState; /// indicator for thread status
    static tPtrThreadFunc m_ThreadFunc; /// threading function pointer
private:
    static DWORD CALLBACK DefaultThreadFunc( LPVOID lpParam ); // /default empty threading function
};

template < typename Base >
DWORD CThreadWrapper< Base >::DefaultThreadFunc( LPVOID lpParam )
{
    return 0;
}

template < typename Base >
tPtrThreadFunc CThreadWrapper< Base >::m_ThreadFunc = &CThreadWrapper< Base >::DefaultThreadFunc;
