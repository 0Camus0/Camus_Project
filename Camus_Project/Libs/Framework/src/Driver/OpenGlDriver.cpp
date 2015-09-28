#include <Driver/OpenglDriver.h>
#include <Driver/DriverProperties.h>

void	OpenGLDriver::InitDriver() {

	auto properties = GetDriverProperties();

	#ifdef OS_WIN32
		HWND hWnd = GetActiveWindow();
		eglWindow = hWnd;
	#elif defined(OS_ANDROID)
		//	eglWindow = engine->window;
	#endif

	
	#ifdef OS_WIN32
		HDC	hDC = GetDC(hWnd);
		eglDisplay = eglGetDisplay(hDC);
	#elif defined(OS_ANDROID)
		eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	#endif
		
		eglBindAPI(EGL_OPENGL_ES_API);		// To check later



}

void	OpenGLDriver::CreateSurfaces() {

}

void	OpenGLDriver::DestroySurfaces() {

}

void	OpenGLDriver::SwapBuffers() {

}

void	OpenGLDriver::DestroyDriver(){

}