#pragma once

class CThreadState
{
public:
	typedef enum State
	{
		UNINNIT,
		INNIT,
		RUNNING,
		PAUSED,
		TERMINATED,
	} State;
};
