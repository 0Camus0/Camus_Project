#ifndef CA_LOG_H
#define CA_LOG_H

#include <config.h>

#ifdef USE_DEBUG
#ifdef OS_WIN32
	void	LogPrintInfo(const char* format, ...);
	void	LogPrintDebug(const char* format, ...);
	void	LogPrintError(const char* format, ...);
	void	LogPrintWarning(const char* format, ...);
#elif OS_ANDROID
	#include <android/log.h>
	#if USE_SHOW_THREADS_IDS
		void	LogPrintInfo(const char* format, ...);
		void	LogPrintDebug(const char* format, ...);
		void	LogPrintError(const char* format, ...);
		void	LogPrintWarning(const char* format, ...);
	#else
		#define  LogPrintInfo(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG_LOGGING, __VA_ARGS__))
		#define  LogPrintDebug(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG_LOGGING, __VA_ARGS__))
		#define  LogPrintError(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG_LOGGING, __VA_ARGS__))
		#define  LogPrintWarning(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG_LOGGING, __VA_ARGS__))
	#endif
#endif
#else
	#define  LogPrintInfo(...) ((void)0)
	#define  LogPrintDebug(...) ((void)0)
	#define  LogPrintError(...) ((void)0)
	#define  LogPrintWarning(...) ((void)0)
#endif

#endif