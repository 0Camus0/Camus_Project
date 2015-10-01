#ifdef USE_DEBUG
#include <Utils/Log.h>
#include <iostream>
#include <sstream>
#include <stdarg.h> 
#include <iosfwd>
#endif

#if USE_SHOW_THREADS_IDS || USE_COLORED_CONSOLE || defined(OS_WIN32)
static char Log_Buffer[1024];	// 1 Kb should be enough
#endif

#if defined(OS_ANDROID) && defined(USE_DEBUG) && USE_SHOW_THREADS_IDS
#include <unistd.h>
#include <string>

std::string	GetId() {
	std::string thread_id = "unknown";
	pid_t tid = gettid();
	std::stringstream ss;
	ss << tid;
	thread_id = ss.str();
	return thread_id;
}

void	LogPrintInfo(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);
	std::string Final = "THREAD[";
	Final += GetId();
	Final += "] - ";
	Final += std::string(Log_Buffer);
	__android_log_print(ANDROID_LOG_INFO, TAG_LOGGING, "%s", Final.c_str());
}

void	LogPrintDebug(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);
	std::string Final = "THREAD[";
	Final += GetId();
	Final += "] - ";
	Final += std::string(Log_Buffer);
	__android_log_print(ANDROID_LOG_DEBUG, TAG_LOGGING, "%s", Final.c_str());
}

void	LogPrintError(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);
	std::string Final = "THREAD[";
	Final += GetId();
	Final += "] - ";
	Final += std::string(Log_Buffer);
	__android_log_print(ANDROID_LOG_ERROR, TAG_LOGGING, "%s", Final.c_str());
}

void	LogPrintWarning(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vsprintf(Log_Buffer, format, arg);
	va_end(arg);
	std::string Final = "THREAD[";
	Final += GetId();
	Final += "] - ";
	Final += std::string(Log_Buffer);
	__android_log_print(ANDROID_LOG_WARN, TAG_LOGGING, "%s", Final.c_str());
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
#if USE_SHOW_THREADS_IDS
	std::cout << "THREAD[" << (int)GetCurrentThreadId() << "] - " << Log_Buffer << std::endl;
#else
	std::cout << Log_Buffer << std::endl;
#endif
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
#if USE_SHOW_THREADS_IDS
	std::cout << "THREAD[" << (int)GetCurrentThreadId() << "] - " << Log_Buffer << std::endl;
#else
	std::cout << Log_Buffer << std::endl;
#endif
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
#if USE_SHOW_THREADS_IDS
	std::cout << "THREAD[" << (int)GetCurrentThreadId() << "] - " << Log_Buffer << std::endl;
#else
	std::cout << Log_Buffer << std::endl;
#endif
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
#if USE_SHOW_THREADS_IDS
	std::cout << "THREAD[" << (int)GetCurrentThreadId() << "] - " << Log_Buffer << std::endl;
#else
	std::cout << Log_Buffer << std::endl;
#endif
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, wOldColorAttrs);
#endif
}
#endif