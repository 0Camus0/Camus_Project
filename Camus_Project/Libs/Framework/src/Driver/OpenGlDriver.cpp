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
OpenGLDriver::OpenGLDriver() {
	bInited = false;
	eglWindow = 0;
	eglDisplay = 0;
	eglSurface = 0;
	eglContext = 0;
}

void OpenGLDriver::SetWindow(void *window) {
	LogPrintDebug("OpenGLDriver::SetWindow");
#ifdef OS_ANDROID
	eglWindow = (ANativeWindow*)window;
#else
	eglWindow = GetActiveWindow();
#endif	
}

void	OpenGLDriver::InitDriver() {
	LogPrintDebug("OpenGLDriver::InitDriver");

	if (eglWindow == 0) {
		LogPrintDebug("OpenGLDriver::InitDriver - No egl window yet");
		return;
	}

	if (bInited) {
		LogPrintDebug("OpenGLDriver::InitDriver - Driver already intied, reseting it.");
		ResetDriver();
		return;
	}

	auto properties = GetDriverProperties();
	EGLint numConfigs,w,h;

	#ifdef OS_WIN32
		HDC	hDC = GetDC(eglWindow);
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
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
		ReportEGLError("eglCreateWindowSurface");

		EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
		eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
		ReportEGLError("eglCreateContext");

		if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
			LogPrintError("Failed to Make Current (eglMakeCurrent)");
			return;
		}

		eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &w);
		eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &h);

		GetWindowParameters().SetParametersFromDriver(w, h);

		LogPrintDebug("Driver successfuly inited version: %s" , glGetString(GL_VERSION));

		bInited = true;
		
}

void OpenGLDriver::ResetDriver() {
	EGLint w, h;
#ifdef OS_ANDROID
	EGLint format;
	eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format);
	ANativeWindow_setBuffersGeometry(eglWindow, 0, 0, format);
	ReportEGLError("eglGetConfigAttrib");
#endif
	CreateSurfaces();

	if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
		LogPrintError("Failed to Make Current (eglMakeCurrent)");
		return;
	}

	eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &w);
	eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &h);

	GetWindowParameters().SetParametersFromDriver(w, h);

	LogPrintDebug("Driver successfuly restarted.");
}

void	OpenGLDriver::CreateSurfaces() {
	LogPrintDebug("OpenGLDriver::CreateSurfaces");
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
	ReportEGLError("eglCreateWindowSurface");
}

void	OpenGLDriver::DestroySurfaces() {
	LogPrintDebug("OpenGLDriver::DestroySurfaces");
	eglDestroySurface(eglDisplay, eglSurface);
	eglMakeCurrent(eglDisplay, 0, 0, eglContext);
}
  
#ifdef OS_WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

#include <algorithm>
#include <iostream>
#include <cmath>

float clip(float n, float lower, float upper) {
	return std::max(
		lower, 
		std::min(n, upper));
}

void	OpenGLDriver::Update() {

	
	static float ang = 0.0f;

	float R = 0.0f, G = 0.0f, B = 0.0f;

	ang += 0.1f;

	R = (clip(std::sin(ang), 0.0f, 1.0f))*0.5f + 0.5f;
	G = (clip(std::cos(ang + .70f), 0.0f, 1.0f))*0.5f + 0.5f;
	B = (clip(std::tan(ang + 1.44f), 0.0f, 1.0f))*0.5f + 0.5f;

	glClearColor(R,G,B,1.0f);

	glClear(GL_COLOR_BUFFER_BIT);
}

void	OpenGLDriver::SwapBuffers() {
	eglSwapBuffers(eglDisplay, eglSurface);
}

void	OpenGLDriver::DestroyDriver(){

}