#include <Config.h>

#ifdef OS_ANDROID
#include "AndroidFramework.cpp"
#elif defined(WIN32)
#include "Win32Framework.cpp"
#endif