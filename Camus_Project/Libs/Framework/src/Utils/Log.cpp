
#if defined(OS_WIN32) && defined(USE_DEBUG)
#include <Utils/Log.h>
#include <iostream>
#include <stdarg.h> 
#include <windows.h>
#include <iosfwd>

static char Log_Buffer[1024];	// 1 Kb should be enough


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
	std::cout << Log_Buffer << std::endl;
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
	std::cout << Log_Buffer << std::endl;
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
	std::cout << Log_Buffer << std::endl;
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
	std::cout << Log_Buffer << std::endl;
#if USE_COLORED_CONSOLE
	SetConsoleTextAttribute(h, wOldColorAttrs);
#endif
}
#endif