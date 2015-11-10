#ifndef CA_CONFIG_H
#define CA_CONFIG_H

// Logging
#if OS_WIN32
#define USE_COLORED_CONSOLE 1
#define USE_PAUSE_EVENTS	0
#else
#define USE_COLORED_CONSOLE 0
#endif

#define USE_SHOW_THREADS_IDS 1
#define USE_SHOW_MEM_USAGE 1
#define TAG_LOGGING "DEBUG"

// Shaders
#define USE_LOG_DEBUG_SHADERS_CONTENT 0
#define USE_LOG_DEBUG_TECHNIQUES 0

// Textures
#define USE_LOG_DEBUG_TEX_LOADING 0

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

#endif