/*=============================================================================
OpenglDrv.h   : Loads EGL.
Revision history:
* Created by Camus
=============================================================================*/

#ifndef OGLDRV_H
#define OGLDRV_H

#include "GlobalHeader.h"

class OglDrv
{
public:



	#ifdef _WIN32
			void	InitOgl(HWND hWnd, bool window = true);
	#else
			void	InitOgl(Display* dpy, Window &win, bool window = true);
	#endif



			static OglDrv* Instance();

	void ReadProperties();

	bool	       ReportError(char* Report);
	void			RecreateSurface();

	EGLDisplay			eglDisplay;
	EGLConfig			eglConfig;
	EGLSurface			eglSurface;
	EGLContext			eglContext;

	int		ResolutionW;
	int		ResolutionH;
	bool	Windowed;
	bool	HighQShadows;
	bool	HighQReflect;
	bool	Readfile;

#ifdef _WIN32
	EGLNativeWindowType	eglWindow;
#else

#endif

private:
	OglDrv(){}





};



#endif
