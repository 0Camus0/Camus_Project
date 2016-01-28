#ifdef USE_DEBUG
#include <Utils/Log.h>
#include <iostream>
#include <sstream>
#include <stdarg.h> 
#include <iosfwd>
#include <unistd.h>
#include <string>

#if defined(__APPLE__)
#include <asl.h>
#endif

#endif

#if USE_SHOW_MEM_USAGE

#if defined(OS_ANDROID)
#include <stdlib.h>
#elif defined(OS_WIN32)
#include <windows.h>
#include <Psapi.h>
#elif defined(__APPLE__)
#include <mach/mach.h>
#include <stdlib.h>
#endif

int	CurrentMemoryUssage() {
#if defined(OS_ANDROID)
	 struct mallinfo info = mallinfo();
	 return (int)info.uordblks;
#elif defined(OS_WIN32)
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	return (int)virtualMemUsedByMe;
#elif defined(__APPLE__)
    struct task_basic_info info;
    mach_msg_type_number_t size = sizeof(info);
    kern_return_t kerr = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
    return (kerr == KERN_SUCCESS) ? static_cast<int>(info.resident_size) : 0; // size in bytes
#endif
}
#endif

#if USE_SHOW_THREADS_IDS || USE_COLORED_CONSOLE || defined(OS_WIN32)
static char Log_Buffer[LOG_BUFFER_SIZE];	// 1 Kb should be enough
#endif

#if defined(OS_ANDROID) && defined(USE_DEBUG) && USE_SHOW_THREADS_IDS


void	LogPrintInfo(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);

	std::stringstream ss;
	std::stringstream extra_info_debug;

#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)gettid() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	ss << extra_info_debug.str() << std::string(Log_Buffer);

	__android_log_print(ANDROID_LOG_INFO, TAG_LOGGING, "%s", ss.str().c_str());
}

void	LogPrintDebug(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);
	std::stringstream ss;
	std::stringstream extra_info_debug;

#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)gettid() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	ss << extra_info_debug.str() << std::string(Log_Buffer);

	__android_log_print(ANDROID_LOG_DEBUG, TAG_LOGGING, "%s", ss.str().c_str());
}

void	LogPrintError(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);
	std::stringstream ss;
	std::stringstream extra_info_debug;

#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)gettid() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	ss << extra_info_debug.str() << std::string(Log_Buffer);

	__android_log_print(ANDROID_LOG_ERROR, TAG_LOGGING, "%s", ss.str().c_str());
}

void	LogPrintWarning(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);
	std::stringstream ss;
	std::stringstream extra_info_debug;

#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)gettid() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	ss << extra_info_debug.str() << std::string(Log_Buffer);

	__android_log_print(ANDROID_LOG_WARN, TAG_LOGGING, "%s", ss.str().c_str());
}

#endif


#if defined(OS_WIN32) && defined(USE_DEBUG)
#include <windows.h>

void	LogPrintInfo(const char* format,...) {
#if USE_COLORED_CONSOLE
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	WORD wOldColorAttrs;
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
#endif
	va_list arg;
	va_start(arg, format);
	vsprintf_s(Log_Buffer, format, arg);
	va_end(arg);
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, 8 | FOREGROUND_INTENSITY);
#endif

	std::stringstream extra_info_debug;
#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)GetCurrentThreadId() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	std::cout << extra_info_debug.str() << Log_Buffer << std::endl;

#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, wOldColorAttrs);
#endif
}

void	LogPrintDebug(const char* format, ...){
#if USE_COLORED_CONSOLE
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	WORD wOldColorAttrs;
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
#endif
	va_list arg;
	va_start(arg, format);
	vsprintf_s(Log_Buffer, format, arg);
	va_end(arg);
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, 2 | FOREGROUND_INTENSITY);
#endif
	std::stringstream extra_info_debug;
#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)GetCurrentThreadId() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	std::cout << extra_info_debug.str() << Log_Buffer << std::endl;
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, wOldColorAttrs);
#endif
}

void	LogPrintError(const char* format, ...) {
#if USE_COLORED_CONSOLE
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	WORD wOldColorAttrs;
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
#endif
	va_list arg;
	va_start(arg, format);
	vsprintf_s(Log_Buffer, format, arg);
	va_end(arg);
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, 4 | FOREGROUND_INTENSITY);
#endif
	std::stringstream extra_info_debug;
#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)GetCurrentThreadId() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	std::cout << extra_info_debug.str() << Log_Buffer << std::endl;
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, wOldColorAttrs);
#endif
}

void	LogPrintWarning(const char* format, ...) {
#if USE_COLORED_CONSOLE
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	WORD wOldColorAttrs;
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
#endif
	va_list arg;
	va_start(arg, format);
	vsprintf_s(Log_Buffer, format, arg);
	va_end(arg);
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, 14 | FOREGROUND_INTENSITY);
#endif
	std::stringstream extra_info_debug;
#if USE_SHOW_THREADS_IDS
	extra_info_debug << "tid[" << (int)GetCurrentThreadId() << "] ";
#endif

#if USE_SHOW_MEM_USAGE
	int bytes = CurrentMemoryUssage();
	int mbytes = bytes / 1048576;
	extra_info_debug << "mem[" << mbytes << " MB] ";
#endif

	std::cout << extra_info_debug.str() << Log_Buffer << std::endl;
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, wOldColorAttrs);
#endif
}
#endif

#if defined(__APPLE__) && defined(USE_DEBUG)

void	LogPrintInfo(const char* format,...) {
    std::stringstream ss;
    std::stringstream extra_info_debug;
    va_list arg;
    va_start(arg, format);
    vsprintf(Log_Buffer, format, arg);
    va_end(arg);
    
    extra_info_debug << "[INFO] ";

#if USE_SHOW_THREADS_IDS
    mach_port_t tid = pthread_mach_thread_np(pthread_self());
    extra_info_debug << "tid[" << (int)tid << "] ";
#endif
    
#if USE_SHOW_MEM_USAGE
    int bytes = CurrentMemoryUssage();
    int mbytes = bytes / 1048576;
    extra_info_debug << "mem[" << mbytes << " MB] ";
#endif
    
    ss << extra_info_debug.str() << std::string(Log_Buffer);
    std::cout << extra_info_debug.str() << Log_Buffer << std::endl;


}

void	LogPrintDebug(const char* format, ...){
    std::stringstream ss;
    std::stringstream extra_info_debug;
    va_list arg;
    va_start(arg, format);
    vsprintf(Log_Buffer, format, arg);
    va_end(arg);
   
    extra_info_debug << "[DEBUG] ";
    
#if USE_SHOW_THREADS_IDS
    mach_port_t tid = pthread_mach_thread_np(pthread_self());
    extra_info_debug << "tid[" << (int)tid << "] ";
#endif
    
#if USE_SHOW_MEM_USAGE
    int bytes = CurrentMemoryUssage();
    int mbytes = bytes / 1048576;
    extra_info_debug << "mem[" << mbytes << " MB] ";
#endif
    
    ss << extra_info_debug.str() << std::string(Log_Buffer);
    std::cout << extra_info_debug.str() << Log_Buffer << std::endl;
}

void	LogPrintError(const char* format, ...) {
    std::stringstream ss;
    std::stringstream extra_info_debug;
    va_list arg;
    va_start(arg, format);
    vsprintf(Log_Buffer, format, arg);
    va_end(arg);
    
    extra_info_debug << "[ERROR] ";
    
#if USE_COLORED_CONSOLE
    SetConsoleTextAttribute(h, 4 | FOREGROUND_INTENSITY);
#endif
  
#if USE_SHOW_THREADS_IDS
    mach_port_t tid = pthread_mach_thread_np(pthread_self());
    extra_info_debug << "tid[" << (int)tid << "] ";
#endif
    
#if USE_SHOW_MEM_USAGE
    int bytes = CurrentMemoryUssage();
    int mbytes = bytes / 1048576;
    extra_info_debug << "mem[" << mbytes << " MB] ";
#endif
    
    ss << extra_info_debug.str() << std::string(Log_Buffer);
    std::cout << extra_info_debug.str() << Log_Buffer << std::endl;
}

void	LogPrintWarning(const char* format, ...) {

    std::stringstream ss;
    std::stringstream extra_info_debug;
    va_list arg;
    va_start(arg, format);
    vsprintf(Log_Buffer, format, arg);
    va_end(arg);

    extra_info_debug << "[WARNING] ";
    
#if USE_SHOW_THREADS_IDS
    mach_port_t tid = pthread_mach_thread_np(pthread_self());
    extra_info_debug << "tid[" << (int)tid << "] ";
#endif
    
#if USE_SHOW_MEM_USAGE
    int bytes = CurrentMemoryUssage();
    int mbytes = bytes / 1048576;
    extra_info_debug << "mem[" << mbytes << " MB] ";
#endif
    
    ss << extra_info_debug.str() << std::string(Log_Buffer);
    std::cout << extra_info_debug.str() << Log_Buffer << std::endl;
    
}


#endif