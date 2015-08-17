#pragma once

#include <iostream>
#include <fstream>
#include <string>

class CLogTail
{
public:
                     CLogTail();
                     ~CLogTail();
                void ResetLogState();                                                      //resets states of the file reader
                bool SetFilePath( std::string path );                                      //sets input file path
                bool OpenFile();                                                           //opens input file
                bool CloseFile();                                                          //closes input file if it is open
                bool GetLineFromFile( std::string & line );                                //retrieves the next newest line from file if possible
                bool UpdatePositionAndLengthTotal( int & position, int & length_total );   //updates the position and total length of text in file
private:
       std::ifstream m_InputFile;
         std::string m_strFilePath;
                 int m_iPosition;
                 int m_iLengthTotal;
};
