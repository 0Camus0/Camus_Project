#include <Utils/WindowProperties.h>

WindowParameters& GetWindowParameters(){
	static WindowParameters	_windowparameter;
	return _windowparameter;
}

void	WindowParameters::GatherProperties() {
#if defined(OS_WIN32)

#elif defined(OS_ANDROID)

#endif
}