#include <Driver/OpenglDriver.h>
#include <Driver/DriverProperties.h>

void	OpenGLDriver::InitDriver() {

	auto properties = GetDriverProperties();
	EGLint numConfigs;

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
		EGLint format;
		eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	#endif
		
		eglBindAPI(EGL_OPENGL_ES_API);		// To check later

		const EGLint attribs[] = {
			EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
	#ifdef OS_WIN32
			EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
	#endif
			EGL_BLUE_SIZE,		properties.BitsPerPixel,
			EGL_GREEN_SIZE,		properties.BitsPerPixel,
			EGL_RED_SIZE,		properties.BitsPerPixel,
			EGL_DEPTH_SIZE,		properties.BitsPerDepth,
			EGL_NONE
		};

		
		eglChooseConfig(eglDisplay, attribs, &eglConfig, 1, &numConfigs);

	#ifdef OS_ANDROID
		eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);
#endif
		EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
		eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
		eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
}

void	OpenGLDriver::CreateSurfaces() {

}

void	OpenGLDriver::DestroySurfaces() {

}

void	OpenGLDriver::SwapBuffers() {

}

void	OpenGLDriver::DestroyDriver(){

}