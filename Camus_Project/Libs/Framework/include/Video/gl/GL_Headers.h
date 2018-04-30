#include <Config.h>

#ifndef T1000_GL_HEADERS_H
#define T1000_GL_HEADERS_H


#ifdef OS_WINDOWS
	#if defined(USING_OPENGL_ES20)
		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
	#elif defined(USING_OPENGL_ES30)
		#include <EGL/egl.h>
		#include <GLES3/gl3.h>
	#elif defined(USING_OPENGL_ES31)
		#include <EGL/egl.h>
		#include <GLES3/gl31.h>
	#elif defined(USING_OPENGL)
		#include <GL/glew.h>
		#include <SDL/SDL.h>
	#else
		#include <GL/glew.h>
		#include <SDL/SDL.h>
	#endif
#elif defined(OS_LINUX)
	#if defined(USING_OPENGL_ES20)
		#include <EGL/egl.h>
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
	#elif defined(USING_OPENGL_ES30)
		#include <EGL/egl.h>
		#include <GLES3/gl3.h>
	#elif defined(USING_OPENGL_ES31)
		#include <EGL/egl.h>
		#include <GLES3/gl31.h>
	#elif defined(USING_OPENGL)
		#include <GL/glew.h>
	#else
		#include <GL/glew.h>
	#endif
#elif defined(OS_ANDROID)
	#include <EGL/egl.h>		
	#include <GLES3/gl3.h>
	#include <GLES2/gl2ext.h>
#endif


#endif