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

void WindowParameters::SetParametersFromDriver(short _Width, short _Height) {
	if (Width != _Width || Height != _Height)
		Properties |= MODIFIED_BY_DEVICE;

	Width = _Width;
	Height = _Height;
}
