#include "stdafx.h"
#include "FilterText.h"

#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include <windows.h>

using namespace std;

CFilterText::CFilterText()
{
    m_bBypassMatchAndEmit = false;
    m_MapMatchAndEmit.clear();
    m_Emitted.clear();
	m_hMutex = CreateMutex( NULL, FALSE, NULL );
}

bool CFilterText::RegisterMatchAndEmit( string match, string emit )
{
    bool bRet = true;

    Lock();
    m_MapMatchAndEmit.insert( std::pair< string, string >( match, emit ) );
    Unlock();

    return bRet;
}

bool CFilterText::SetBypassMatchAndEmit( bool bSetBypassOn )
{
    bool bRet = true;

    if( bSetBypassOn )
    {
        m_bBypassMatchAndEmit = true;
    }
    else
    {
        m_bBypassMatchAndEmit = false;
    }

    return bRet;
}

bool CFilterText::ProcessInputText( string & input, vector< string > & emitted )
{
    bool bRet = false;

    emitted.clear();
    
    if( m_bBypassMatchAndEmit )
    {
        emitted.push_back( input );
		Lock();
        m_Emitted.insert( m_Emitted.end(), emitted.begin(), emitted.end() );
		Unlock();
        return true;
    }

    for( map< string, string >::iterator it = m_MapMatchAndEmit.begin(), itend = m_MapMatchAndEmit.end(); it != itend; it++ )
    {
        if( string::npos != input.find( it->first ) )
        {
            emitted.push_back( it->second );
            bRet = true;
        }
    }

    //accumulate emitted
    Lock();
    m_Emitted.insert( m_Emitted.end(), emitted.begin(), emitted.end() );
    Unlock();

    return bRet;
}

bool CFilterText::GetAllEmitted( vector< string > & emitted )
{
    Lock();
    emitted = m_Emitted;
    Unlock();
    return true;
}

bool CFilterText::ClearAllEmitted()
{
    Lock();
    m_Emitted.clear();
	Unlock();
    return true;
}

bool CFilterText::ClearRegistered()
{
    m_MapMatchAndEmit.clear();
    return true;
}

void CFilterText::Lock()
{
    WaitForSingleObject( m_hMutex, INFINITE);
}

void CFilterText::Unlock()
{
    ReleaseMutex(m_hMutex);
}
