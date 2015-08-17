#pragma once

#include <string>
#include <map>
#include <vector>

#include <windows.h>

class CFilterText
{
public:
    CFilterText();
    bool RegisterMatchAndEmit( std::string match, std::string emit );
    bool SetBypassMatchAndEmit( bool bSetBypassOn );
    bool ProcessInputText( std::string & input, std::vector< std::string > & emitted );
    bool GetAllEmitted( std::vector< std::string > & emitted );
    bool ClearAllEmitted();
    bool ClearRegistered();
private:
    std::map< std::string, std::string > m_MapMatchAndEmit;
    std::vector< std::string > m_Emitted;
    bool m_bBypassMatchAndEmit;
    HANDLE m_hMutex; /// lock for non-threadsafe access
    void Lock();
    void Unlock();
};
