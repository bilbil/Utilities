#include "stdafx.h"

#include "LogTail.h"

#include <string>
#include <cassert>

#include <windows.h>

using namespace std;

CLogTail log_tail;

void fnExitCleanUp()
{
    log_tail.CloseFile(); //closes file
}

int main(int argc, char * argv[])
{
    atexit ( fnExitCleanUp );

    if( argc < 2 )
    {
        cerr << "UTILITY_ERROR: Require input file path as argument." << endl;
        return -1;
    }

    string strInputFile = string( argv[1] );
        
    log_tail.ResetLogState();
    if( !log_tail.SetFilePath( strInputFile ) )
    {
        cerr << "UTILITY_ERROR: Failed to set input file." << endl;
		return -1;
    }
        
    if( !log_tail.OpenFile() )
    {
        cerr << "UTILITY_ERROR: Failed to open input file." << endl;
		return -1;
    }

    int position;
    int length;

    while(1)
    {
        if( log_tail.UpdatePositionAndLengthTotal( position, length ) )
        {
            //cout << "Updated position: " << position << ", length: " << length << endl;
        }
        string line;
        if( log_tail.GetLineFromFile( line ) )
        {
            cout << line << endl;
        }
        Sleep(5);
    }

    return 0;
}

