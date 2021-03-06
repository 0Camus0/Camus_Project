#ifndef T1000_CONFIG_H
#define T1000_CONFIG_H


#if defined(__linux__) && defined(__ANDROID__)
	#ifndef OS_ANDROID
	#define OS_ANDROID
	#endif
#elif defined(__linux__) && !defined(__ANDROID__)
	#ifndef OS_LINUX
	#define OS_LINUX
	#endif
#else
	#define OS_WIN32
#endif

// Logging
#ifdef OS_WIN32
#define USE_COLORED_CONSOLE 1
#define USE_PAUSE_EVENTS	0
#else
#define USE_COLORED_CONSOLE 0
#endif

#define USE_SHOW_THREADS_IDS 1
#define USE_SHOW_MEM_USAGE 1
#define TAG_LOGGING "DEBUG"

// Profiling
#define USE_PROFILING 1

// Shaders
#define USE_LOG_DEBUG_SHADERS_CONTENT 0
#define USE_LOG_DEBUG_TECHNIQUES 0

// Textures
#define USE_LOG_DEBUG_TEX_LOADING 0

// Window Properties
#define USE_LOG_WINDOW_PROPERTIES 1

// Drivers
#define USER_LOG_DEBUG_DRIVER_CALLS 1
#define USER_LOG_DEBUG_DRIVER_FLOW 1

// Memory
#define USE_MEMORY_TRACKER 1

#define LOG_BUFFER_SIZE	1024*1024

#define MAX_TEXURE_LIMIT	25

#define	TEXTURE_BUDGET_SIZE_MB		35
#define TEXTURE_BUDGET_SIZE_BYTES	1024*1024*TEXTURE_BUDGET_SIZE_MB

#define MAX_PATH_SIZE	512
#define PATH_ARRAY_SIZE MAX_PATH_SIZE*MAX_TEXURE_LIMIT


#define TEXTURE_CLASS_SIZE	160
#define TEX_ARRAY_SIZE TEXTURE_CLASS_SIZE*MAX_TEXURE_LIMIT

#define TEX_STACK_ALLOCATED_SIZE TEXTURE_BUDGET_SIZE_BYTES+PATH_ARRAY_SIZE+TEX_ARRAY_SIZE

// ---------------

#ifndef T_NO_SIGNATURE
#define T_NO_SIGNATURE -1
#endif

#ifndef FORCE_LOW_RES_TEXTURES
#define FORCE_LOW_RES_TEXTURES 0
#endif

#ifndef FORCED_FACTOR
#define FORCED_FACTOR 2
#endif

#ifndef DEBUG_DRIVER
#define DEBUG_DRIVER 0
#endif

#ifndef VDEBUG_NO_LIGHT
#define VDEBUG_NO_LIGHT 0
#endif

#ifndef VDEBUG_SIMPLE_COLOR
#define VDEBUG_SIMPLE_COLOR 0
#endif

#if VDEBUG_SIMPLE_COLOR && VDEBUG_NO_LIGHT
#undef VDEBUG_NO_LIGHT
#define VDEBUG_NO_LIGHT 0
#endif


#ifndef USE_LOAD_FROM_MEMORY
#define USE_LOAD_FROM_MEMORY 1
#endif

#define D3D11	1 
#define OGLES20 2
#define OGLES30 3
#define OGLES31 4
#define OGL 5

#define DRIVER_SELECTED OGLES30

#if defined(OS_LINUX) || defined(OS_ANDROID)
	#ifdef  DRIVER_SELECTED
	#undef  DRIVER_SELECTED
	#define DRIVER_SELECTED OGLES30
	#endif
#endif

#if DRIVER_SELECTED == D3D11
#define USING_D3D11
#elif DRIVER_SELECTED == OGLES20
#define USING_OPENGL_ES20
#elif DRIVER_SELECTED == OGLES30
#define USING_OPENGL_ES30
#elif DRIVER_SELECTED == OGLES31
#define USING_OPENGL_ES31
#elif DRIVER_SELECTED == OGL
#define USING_OPENGL
#else
#define USING_OPENGL // Default GL
#endif

#if defined(USING_OPENGL_ES20) || defined(USING_OPENGL_ES30)  || defined(USING_OPENGL_ES31) || defined(USING_OPENGL)
#define USING_GL_COMMON
#endif

#if defined(USING_GL_COMMON) && !defined(USING_OPENGL)
#define USING_GL_ES
#endif

#define SDL 1
#define FREEGLUT 2
#define WAYLAND_NATIVE 3
#define ANDROID_EGL 4

#define WINDOW_MANAGER WAYLAND_NATIVE

#if defined(OS_LINUX) 
	#ifdef  WINDOW_MANAGER
	#undef  WINDOW_MANAGER
	#define WINDOW_MANAGER WAYLAND_NATIVE
	#endif
#endif

#if defined(OS_ANDROID) 
	#ifdef  WINDOW_MANAGER
	#undef  WINDOW_MANAGER
	#define WINDOW_MANAGER ANDROID_EGL
	#endif
#endif

#if defined(OS_WINDOWS)
	#ifdef  WINDOW_MANAGER
	#undef  WINDOW_MANAGER
	#define WINDOW_MANAGER SDL 
	#endif
#endif

#if WINDOW_MANAGER == SDL
#define USING_SDL
#elif WINDOW_MANAGER == FREEGLUT
#define USING_FREEGLUT
#elif WINDOW_MANAGER == WAYLAND_NATIVE
#define USING_WAYLAND_NATIVE
#elif WINDOW_MANAGER == ANDROID_EGL
#define USING_ANDROID_EGL
#else
#define USING_SDL
#endif




#endif