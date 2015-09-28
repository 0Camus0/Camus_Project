#ifndef CA_OPENGL_DRIVER_H
#define CA_OPENGL_DRIVER_H

#include <Driver/BaseDriver.h>

// Headers for EGL and OGLES
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#ifdef OS_ANDROID
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#endif

class OpenGLDriver : public BaseDriver {
public:
	void	InitDriver();
	void	CreateSurfaces();
	void	DestroySurfaces();
	void	Update();
	void	SwapBuffers();
	void	DestroyDriver();

	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;

#ifdef OS_WIN32
	EGLNativeWindowType	eglWindow;
#elif defined(OS_ANDROID)
	ANativeWindow		*eglWindow;
#endif
};


#endif