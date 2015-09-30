#include <Driver/OpenglDriver.h>
#include <Driver/DriverProperties.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>

#ifndef USE_DEBUG
#define ReportEGLError(...) ((void)0)
#else
void ReportEGLError(const char* c_ptr){
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS){
		LogPrintError("%s failed (%d).\n", c_ptr, iErr);
	}
}
#endif

void	OpenGLDriver::InitDriver() {

	auto properties = GetDriverProperties();
	EGLint numConfigs,w,h;

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

		ReportEGLError("eglGetDisplay");

		EGLint iMajorVersion, iMinorVersion;

		if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion)) {
			ReportEGLError("eglInitialize");
		}
		
		eglBindAPI(EGL_OPENGL_ES_API);		// To check later

		ReportEGLError("eglBindAPI");

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

		ReportEGLError("eglChooseConfig");

#ifdef OS_ANDROID
		eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format);
		ANativeWindow_setBuffersGeometry(eglWindow, 0, 0, format);
		ReportEGLError("eglGetConfigAttrib");
#endif
		EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
		ReportEGLError("eglCreateWindowSurface");

		eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
		ReportEGLError("eglCreateContext");

		if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
			LogPrintError("Failed to Make Current (eglMakeCurrent)");
			return;
		}

		eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &w);
		eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &h);

		GetWindowParameters().SetParametersFromDriver(w, h);
		
}

void	OpenGLDriver::CreateSurfaces() {

}

void	OpenGLDriver::DestroySurfaces() {

}

void	OpenGLDriver::Update() {

	glClearColor(0.6f, 0.85f, 0.91f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
}

void	OpenGLDriver::SwapBuffers() {
	eglSwapBuffers(eglDisplay, eglSurface);
}

void	OpenGLDriver::DestroyDriver(){

}