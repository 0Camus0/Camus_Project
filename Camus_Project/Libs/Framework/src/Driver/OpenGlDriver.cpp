#include <Driver\OpenglDriver.h>
#include <Driver\DriverProperties.h>
#include <Utils\WindowProperties.h>
#include <Utils\Log.h>

#ifdef __APPLE__
extern EAGLContext *g_EAGLContext;
#endif
#ifndef USE_DEBUG
#define ReportEGLError(...) ((void)0)
#else
void ReportEGLError(const char* c_ptr){
#if !defined(__APPLE__)
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS){
		LogPrintError("%s failed (%d).\n", c_ptr, iErr);
	}
#endif
}
#endif

namespace hyperspace {
	namespace video {
		OpenGLDriver::OpenGLDriver() {
			bInited = false;
#ifndef __APPLE__
			eglWindow = 0;
			eglDisplay = 0;
			eglSurface = 0;
			eglContext = 0;
#endif
		}

		OpenGLDriver::~OpenGLDriver() {
		
		}

		void OpenGLDriver::SetWindow(void *window) {
#if USER_LOG_DEBUG_DRIVER_FLOW
			LogPrintDebug("OpenGLDriver::SetWindow");
#endif
#ifdef OS_ANDROID
			eglWindow = (ANativeWindow*)window;
#elif OS_WIN32
			eglWindow = GetActiveWindow();
#endif	
		}

		void	OpenGLDriver::InitDriver() {
#if USER_LOG_DEBUG_DRIVER_FLOW
			LogPrintDebug("OpenGLDriver::InitDriver");
#endif

#if defined(OS_WIN32) || defined(OS_ANDROID)
			if (eglWindow == 0) {
#if USER_LOG_DEBUG_DRIVER_FLOW
				LogPrintDebug("OpenGLDriver::InitDriver - No egl window yet");
#endif
				return;
			}

			if (bInited) {
#if USER_LOG_DEBUG_DRIVER_FLOW
				LogPrintDebug("OpenGLDriver::InitDriver - Driver already intied, reseting it.");
#endif
				ResetDriver();
				return;
			}
#endif
			auto &properties = GetDriverProperties();
            
 #if defined(OS_WIN32) || defined(OS_ANDROID)
			EGLint numConfigs, w, h;

#ifdef OS_WIN32
			HDC	hDC = GetDC(eglWindow);
			eglDisplay = eglGetDisplay(hDC);
#elif defined(OS_ANDROID)
			EGLint format;
			eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif

#if USER_LOG_DEBUG_DRIVER_CALLS
			ReportEGLError("eglGetDisplay");
#endif

			EGLint iMajorVersion, iMinorVersion;

			if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion)) {
#if USER_LOG_DEBUG_DRIVER_CALLS
				ReportEGLError("eglInitialize");
#endif
			}

			eglBindAPI(EGL_OPENGL_ES_API);		// To check later

#if USER_LOG_DEBUG_DRIVER_CALLS
			ReportEGLError("eglBindAPI");
#endif

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
#if USER_LOG_DEBUG_DRIVER_CALLS
			ReportEGLError("eglChooseConfig");
#endif

#ifdef OS_ANDROID
			eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format);
			ANativeWindow_setBuffersGeometry(eglWindow, 0, 0, format);
#if USER_LOG_DEBUG_DRIVER_CALLS
			ReportEGLError("eglGetConfigAttrib");
#endif
#endif
			eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
#if USER_LOG_DEBUG_DRIVER_CALLS
			ReportEGLError("eglCreateWindowSurface");
#endif

			EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
			eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

#if USER_LOG_DEBUG_DRIVER_CALLS
			ReportEGLError("eglCreateContext");
#endif

			if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
#if USER_LOG_DEBUG_DRIVER_CALLS
				LogPrintError("Failed to Make Current (eglMakeCurrent)");
#endif
				return;
			}

			eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &w);
			eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &h);

			GetWindowParameters().SetParametersFromDriver(w, h);

			

#endif
            properties.Version = std::string((const char*)glGetString(GL_VERSION));
#if USER_LOG_DEBUG_DRIVER_FLOW
            LogPrintDebug("Driver successfuly inited version: %s", properties.Version.c_str());
#endif
            properties.SetExtensions(std::string((const char*)glGetString(GL_EXTENSIONS)));
#if USER_LOG_DEBUG_DRIVER_FLOW
            properties.ListExtensions();
#endif
            
			bInited = true;

		}

		void OpenGLDriver::ResetDriver() {
#if defined(OS_WIN32) || defined(OS_ANDROID)
			EGLint w, h;
#ifdef OS_ANDROID
			EGLint format;
			eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &format);
			ANativeWindow_setBuffersGeometry(eglWindow, 0, 0, format);
#if USER_LOG_DEBUG_DRIVER_CALLS
			ReportEGLError("eglGetConfigAttrib");
#endif
#endif
			CreateSurfaces();

			if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
#if USER_LOG_DEBUG_DRIVER_CALLS
				LogPrintError("Failed to Make Current (eglMakeCurrent)");
#endif
				return;
			}

			eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &w);
			eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &h);

			GetWindowParameters().SetParametersFromDriver(w, h);
#if USER_LOG_DEBUG_DRIVER_FLOW
			LogPrintDebug("Driver successfuly restarted.");
#endif
#endif
		}

		void	OpenGLDriver::CreateSurfaces() {
#if USER_LOG_DEBUG_DRIVER_FLOW
			LogPrintDebug("OpenGLDriver::CreateSurfaces");
#endif
#if defined(OS_WIN32) || defined(OS_ANDROID)
			eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
			ReportEGLError("eglCreateWindowSurface");
#endif
		}

		void	OpenGLDriver::DestroySurfaces() {
#if USER_LOG_DEBUG_DRIVER_FLOW
			LogPrintDebug("OpenGLDriver::DestroySurfaces");
#endif
 #if defined(OS_WIN32) || defined(OS_ANDROID)
			eglDestroySurface(eglDisplay, eglSurface);
			eglMakeCurrent(eglDisplay, 0, 0, eglContext);
#endif
		}

		
		void	OpenGLDriver::Clear(draw_bits_ mask, int r, int g, int b, int a, float depth) {

			static GLint glmask;

			glmask = 0;
			if (mask&draw_bits_::COLOR_BIT)
				glmask |= GL_COLOR_BUFFER_BIT;

			if (mask&draw_bits_::DEPTH_BIT)
				glmask |= GL_DEPTH_BUFFER_BIT;

			if (mask&draw_bits_::STENCIL_BIT)
				glmask |= GL_STENCIL_BUFFER_BIT;

			float r_ = static_cast<float>(r) / 255.0f;
			float g_ = static_cast<float>(g) / 255.0f;
			float b_ = static_cast<float>(b) / 255.0f;
			float a_ = static_cast<float>(a) / 255.0f;
			glClearColor(r_, g_, b_, a_);
			glClearDepthf(depth);
			glClear(glmask);

		}

		void	OpenGLDriver::Clear(draw_bits_ mask, float r, float g, float b, float a, float depth) {

			static GLint glmask;

			glmask = 0;
			if (mask&draw_bits_::COLOR_BIT)
				glmask |= GL_COLOR_BUFFER_BIT;

			if (mask&draw_bits_::DEPTH_BIT)
				glmask |= GL_DEPTH_BUFFER_BIT;

			if (mask&draw_bits_::STENCIL_BIT)
				glmask |= GL_STENCIL_BUFFER_BIT;

			glClearColor(r, g, b, a);
			glClearDepthf(depth);
			glClear(glmask);

		}

		void	OpenGLDriver::Update() {
	
		}

		void	OpenGLDriver::SwapBuffers() {
#if defined(OS_WIN32) || defined(OS_ANDROID)
			eglSwapBuffers(eglDisplay, eglSurface);
#elif defined(__APPLE__)
           [g_EAGLContext presentRenderbuffer:GL_RENDERBUFFER];
#endif
		}

		void	OpenGLDriver::DestroyDriver() {

		}
	}
}