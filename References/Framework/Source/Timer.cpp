
#include "../Include/Timer.h"


Timer::Timer()
{
	#ifdef WIN32
		QueryPerformanceFrequency( (LARGE_INTEGER *)&TicksPerSecond );
		currentTime = LastTime = LastFPSUpdate = 0;
	#else

	#endif

	Fps = NumFrames = 0;
	tmpRunning = 0.0f;
	RunningTime = TimeElapsed =  0.0f;
	TimerStopped = true;
}

void Timer::Start()
{
	if ( !TimerStopped )
	{
		return;
	}

	#ifdef WIN32
		QueryPerformanceCounter( (LARGE_INTEGER *)&LastTime );
	#else
		gettimeofday(&LastTime, NULL);
	#endif

	TimerStopped = false;
}

void Timer::Stop()
{

	if ( TimerStopped )
	{
		return;
	}

	#ifdef WIN32

		INT64 stopTime = 0;
		QueryPerformanceCounter( (LARGE_INTEGER *)&stopTime );
		RunningTime += (float)(stopTime - LastTime) / (float)TicksPerSecond;

	#else
		gettimeofday(&stopTime, NULL);
		RunningTime += stopTime.tv_sec - LastTime.tv_sec + (stopTime.tv_usec - LastTime.tv_usec) * 0.000001f;
	#endif

		TimerStopped = true;

}

void Timer::Update()
{


	if ( TimerStopped )
	{
		return;
	}

	#ifdef WIN32
		QueryPerformanceCounter( (LARGE_INTEGER *)&currentTime );
		TimeElapsed = (float)(currentTime - LastTime) / (float)TicksPerSecond;
	#else
		gettimeofday(&currentTime, 0);
		TimeElapsed	= currentTime.tv_sec - LastTime.tv_sec + (currentTime.tv_usec - LastTime.tv_usec) * 0.000001f;
	#endif



	RunningTime += TimeElapsed;
	tmpRunning += TimeElapsed;

	NumFrames++;

	if(tmpRunning > 1.0f)
	{
		Fps = NumFrames;
	//	printf("fps: %d\n",Fps);
		NumFrames = 0;
		tmpRunning = 0.0f;
	}


	LastTime = currentTime;
}
