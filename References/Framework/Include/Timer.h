/*=============================================================================
Timer.h : Simple timer class.	
Revision history:
* Created by Camus
=============================================================================*/

#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include "GlobalHeader.h"


class Timer
{
public:
	Timer();
	void Start();
	void Stop();
	void Update();


	bool IsStopped() { return TimerStopped; }

	float GetRunningTime()
	{
		#ifdef WIN32
		return RunningTime;
		#else
		return RunningTime/1000.0;
		#endif
	}

	float GetElapsedTime() { return TimerStopped ? 0.0f : TimeElapsed; }

	void ResetRunningTime() { RunningTime=0; }

private:

	#ifdef WIN32
		INT64   TicksPerSecond;
		INT64   currentTime;
		INT64   LastTime;
		INT64   LastFPSUpdate;
		INT64   FPSUpdateInterval;
	#else
		timeval currentTime;
		timeval stopTime;
		timeval LastTime;
		timeval LastFPSUpdate;
		timeval FPSUpdateInterval;
	#endif

	UINT  NumFrames;
	float RunningTime;
	float TimeElapsed;
	float tmpRunning;
	int	  Fps;
	bool TimerStopped;
};

#endif // TIME_H_INCLUDED
