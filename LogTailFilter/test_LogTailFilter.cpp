//=====================================================================
// Copyright (C) 2015 Sierra Wireless Inc. All rights reserved.
//
// File:                test_LogTailFilter.cpp
//
// Subsystem:
//
// Summary:             test case for CLogTailFilter class.
//
// Notes:               
//
//
// Author(s):   ...
//
//=====================================================================
// Version   Date         Author  Change    Description
//---------------------------------------------------------------------
// 0.0.0     12-Aug-2015  B.L.    00000     Created file.
// 0.0.1     17-Aug-2015  B.L.    00000     Modified input argument format.
//=============================================================================
#include "stdafx.h"
#include "LogTailFilter.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

void PrintElement( string & text )
{
    cout << text << endl;
}

int main(int argc, char * argv[])
{
    CLogTailFilter log_tail_filter;

    if( argc < 3 )
    {
		cerr << "UTILITY_ERROR: Usage: <input file path> <bypass_on/bypass_off> ( <register_N> <emitt_N> )*" << endl;
        return -1;
    }

    string strInputFile = string( argv[1] );
    string strBypassOn = "";
    strBypassOn = string( argv[2] );
    bool bBypassOn = ( 0 == strcmp( "bypass_on", strBypassOn.c_str() ) )? true : false;

    int iCountPairRegisterEmitt = ( argc - 3 )/2;
    int iCountRemainderRegisterEmitt = ( argc - 3 )%2;
    if ( 1 == iCountRemainderRegisterEmitt )
    {
        cerr << "UTILITY_ERROR: Usage: <input file path> <bypass_on/bypass_off> ( <register_N> <emitt_N> )*" << endl;
        return -1;
    }

	for( int i = 0; i < iCountPairRegisterEmitt; i++ )
    {
        string strRegisterMatch = string( argv[ i*2 + 3 ] );
        string strEmit = string( argv[ i*2 + 4 ] );
        log_tail_filter.RegisterMatchAndEmit( strRegisterMatch, strEmit );
	}
    
    log_tail_filter.ResetLogState();
    if( !log_tail_filter.SetFilePath( strInputFile ) )
    {
        cerr << "UTILITY_ERROR: Failed to set input file." << endl;
        return -1;
    }
        
    if( !log_tail_filter.OpenFile() )
    {
        cerr << "UTILITY_ERROR: Failed to open input file." << endl;
        return -1;
    }

    log_tail_filter.SetBypassMatchAndEmit( bBypassOn );
    log_tail_filter.MakeThread();
    log_tail_filter.RunThread();
    log_tail_filter.SignalRun();
        
    vector< string > AllEmitted;

    while(1)
    {
        log_tail_filter.GetAllEmitted( AllEmitted );
        log_tail_filter.ClearAllEmitted();
        if( !AllEmitted.empty() )
        {
            for_each( AllEmitted.begin(), AllEmitted.end(), &PrintElement );
        }
        Sleep(100);
    }

    return 0;
}

