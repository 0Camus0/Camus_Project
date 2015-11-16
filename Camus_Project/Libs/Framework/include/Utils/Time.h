#ifndef CA_CTIME_BASE_H

#define CA_CTIME_BASE_H

#include <config.h>

#ifdef OS_WIN32
#include <windows.h>
#elif defined(OS_ANDROID)
#include <time.h>
#endif

#include <string>

#if USE_PROFILING
#define PROFILING_SCOPE(a) TimeEvent t(a);
#else
#define PROFILING_SCOPE(a)
#endif

namespace hyperspace {

	struct tevent_ {
		std::string		name;
#ifdef OS_WIN32
		LARGE_INTEGER	duration;
#elif defined(OS_ANDROID)
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