#include <SDL/SDL.h>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#else

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <SDL_gles.h>

#endif

#ifndef NULL
typedef 0 NULL;

#endif

typedef unsigned int UINT;


// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <malloc.h>
#include <memory.h>
#include <math.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>

// C++ Headers

#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <string>
using namespace std;

// Macros
#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }
#define SAFE_DELETE_ARRAY(x) if( x ) { delete [] (x); (x) = NULL; }


GLuint createShader(GLenum type, char* pSource);
void checkcompilederrors(GLuint shader,GLenum type);
void printShaderInfoLog(GLuint shader);
char *file2string(const char *path);

double string2double(const string &s);
string double2string(const double &d);
string double2stringlowP(const double &d);
