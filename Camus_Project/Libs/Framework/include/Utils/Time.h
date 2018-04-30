#ifndef T1000_CTIME_BASE_H
#define T1000_CTIME_BASE_H

#include <config.h>

#ifdef OS_WIN32
#include <windows.h>
#elif defined(OS_ANDROID) || defined(OS_LINUX)
#include <time.h>
#elif defined(__APPLE__)
#include <sys/time.h>
#endif

#include <string>

#if USE_PROFILING
#define PROFILING_SCOPE(a) TimeEvent t(a);
#else
#define PROFILING_SCOPE(a)
#endif

namespace t1000 {

	struct tevent_ {
		std::string		name;
#ifdef OS_WIN32
		LARGE_INTEGER	duration;
#elif defined(OS_ANDROID) || defined(__APPLE__) || defined(OS_LINUX)
		timeval			duration;
#endif
	};


	class TimeEvent {
	public:
		
		TimeEvent(std::string name);

		~TimeEvent();

		tevent_	t;

	};

}
#endif