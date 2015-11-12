#include <Utils/Time.h>
#include <Utils/Log.h>

extern double	g_Frequency;

namespace hyperspace {

	TimeEvent::TimeEvent(std::string name){
		t.name = name;
#ifdef OS_WIN32
		QueryPerformanceCounter(&t.duration);
#elif defined(OS_ANDROID)
		gettimeofday(&t.duration, NULL);
#endif	
	}


	TimeEvent::~TimeEvent() {
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		double time_ = double(end.QuadPart - t.duration.QuadPart)/g_Frequency;
		LogPrintInfo("[Profiling][%s] Time: [%f]",t.name.c_str(), time_);
	}
}