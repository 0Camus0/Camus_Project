#ifndef CA_OPENGL_DRIVER_H
#define CA_OPENGL_DRIVER_H

#include <Driver/BaseDriver.h>

// Headers for EGL and OGLES

#ifdef __APPLE__
#include <OpenGLES/EAGL.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#ifdef OS_ANDROID
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#endif

namespace hyperspace {
	namespace video {
		class OpenGLDriver : public BaseDriver {
		public:
			OpenGLDriver();
			~OpenGLDriver();
			void	InitDriver();
			void	CreateSurfaces();
			void	DestroySurfaces();
			void	Update();
			
			void	DestroyDriver();
			void	SetWindow(void *window);

		
			void	Clear(draw_bits_ mask, int r, int g, int b, int a,float depth = 1.0f);
			void	Clear(draw_bits_ mask, float r, float g, float b, float a, float depth = 1.0f);
			void	SwapBuffers();

			void	ResetDriver();
#ifdef __APPLE__
            EAGLContext                *eglContext;
#else
			EGLDisplay			eglDisplay;
			EGLConfig			eglConfig;
			EGLSurface			eglSurface;
			EGLContext			eglContext;
#endif
            
#ifdef OS_WIN32
			EGLNativeWindowType	eglWindow;
#elif defined(OS_ANDROID)
			ANativeWindow		*eglWindow;
#endif

			bool				bInited;
		};
	}
}

#endif