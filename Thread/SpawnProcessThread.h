#include "SpawnProcess.h"
#include "ThreadState.h"
#include "ThreadWrapper.h"

class CSpawnProcessThread : public CThreadWrapper< CSpawnProcess > 
{
public:
CSpawnProcessThread() : CThreadWrapper( CThreadWrapper< CSpawnProcess >::ProcessTask )
    {
    }
    bool cbTerminated() // callback function to set the thread state to the terminated status
    {
        SignalThreadEnd(); 
        return true;
    }
};
