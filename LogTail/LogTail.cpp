#include "stdafx.h"

#include "LogTail.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

CLogTail::CLogTail()
{
    ResetLogState();
}

CLogTail::~CLogTail()
{
    CloseFile();
}

//=============================================================================
// Summary:
//      - Reset parsing states.
//
// Notes:
//      - None.
//=============================================================================
void CLogTail::ResetLogState()
{
    m_iPosition = 0;
    m_iLengthTotal = 0;
    m_strFilePath = "";
}

//=============================================================================
// Summary:
//      - Set input file path.
//
// Notes:
//      - None.
//=============================================================================
bool CLogTail::SetFilePath( string path )
{
    bool bRet = true;
    
    m_strFilePath = path;

    return bRet;
}

//=============================================================================
// Summary:
//      - Opens file for reading.
//
// Notes:
//      - None.
//=============================================================================
bool CLogTail::OpenFile()
{
    bool bRet = true;
    
    CloseFile();

	m_InputFile.open( m_strFilePath.c_str(), ios_base::in );
    
    if( !m_InputFile.good() )
    {
        return false;
    }

    return bRet;
}

//=============================================================================
// Summary:
//      - Closes file if it's open.
//
// Notes:
//      - None.
//=============================================================================
bool CLogTail::CloseFile()
{
    bool bRet = true;
    
    if( m_InputFile.is_open() )
    {
        m_InputFile.close();
    }

    m_InputFile.clear();

    return bRet;
}

//=============================================================================
// Summary:
//      - Depending on the current position in the file, this get the next newest line or gets the remaining text if it's on the last line.
//      - Return:
//          - True: if returned line is valid
//          - False: return line is invalid
// Notes:
//      - None.
//=============================================================================
bool CLogTail::GetLineFromFile( string & line )
{
    bool bRet = true;
    
    if( !m_InputFile.is_open() || !m_InputFile.good() )
    {
        return false;
    }

    if( !m_InputFile.eof() )
    {
        getline( m_InputFile, line );
        if( !m_InputFile.eof() )
        {
            m_iPosition = m_InputFile.tellg();
        }
        else
        {
            int iPositionCurrent = m_InputFile.tellg();
            if( iPositionCurrent == m_iLengthTotal )
            {
                m_iPosition = m_InputFile.tellg();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    return bRet;
}

//=============================================================================
// Summary:
//      - Updates the current position within the file.
//      - Return:
//          - True: if returned position and total length of text are updated and valid
//          - False: if file is invalid or if text has not changed.
// Notes:
//      - None.
//=============================================================================
bool CLogTail::UpdatePositionAndLengthTotal( int & position, int & length_total )
{
    bool bRet = true;

    int iPositionOld = m_iPosition;
    int iLengthTotalOld = m_iLengthTotal;

    if( !m_InputFile.is_open() )
    {
        return false;
    }

    m_InputFile.clear();

    // get length of file
    m_InputFile.seekg( 0, m_InputFile.end );
    int iNewLength = m_InputFile.tellg();
    m_InputFile.seekg( 0, m_InputFile.beg );

    if( m_iLengthTotal > iNewLength  || m_iPosition > iNewLength )
    {
        //reset position to beginning
        m_InputFile.seekg( 0 );
        m_iPosition = m_InputFile.tellg();
    }
    else
    {
        m_InputFile.seekg( m_iPosition );
    }

    m_iLengthTotal = iNewLength;

    position = m_iPosition;
    length_total = m_iLengthTotal;

    if( !m_InputFile.good() )
    {
        return false;
    }

    if( iPositionOld == position && iLengthTotalOld == length_total )
    {
        return false;
    }

    return bRet;
}
