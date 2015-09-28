/**************************************************************************
 * Name         : egltypes.h
 *
 * Copyright    : 2005-6 by Imagination Technologies Limited. All rights reserved.
 *              : No part of this software, either material or conceptual 
 *              : may be copied or distributed, transmitted, transcribed,
 *              : stored in a retrieval system or translated into any 
 *              : human or computer language in any form by any means,
 *              : electronic, mechanical, manual or other-wise, or 
 *              : disclosed to third parties without the express written
 *              : permission of Imagination Technologies Limited, Unit 8, HomePark
 *              : Industrial Estate, King's Langley, Hertfordshire,
 *              : WD4 8LZ, U.K.
 *
 * Platform     : ANSI
 *
 * $Date: 2007/08/07 14:43:30 $ $Revision: 1.4 $
 * $Log: eglplatform.h,v $
 * Revision 1.4  2007/08/07 14:43:30  grk
 * [INTERNAL BUILDS OGLES2] Makefile changes to enable content file compilation.
 *
 * Revision 1.3  2007/08/02 14:34:36  ptj
 * *** empty log message ***
 *
 * Revision 1.2  2007/08/02 10:45:10  ptj
 * Moved egltypes.h -> eglplatform.h
 *
 * Revision 1.2  2006/11/02 10:09:18  ptj
 * [INTERNAL BUILDS OGLES2 INCLUDE] egltypes.h:  Something added new line at every line (so there were 2 new line chars at the end of each line). corrected.
 *
 * Revision 1.1  2006/08/16 11:23:28  Jakub.Lamik
 * Initial revision
 * Revision 1.3  2006/06/29 16:13:55  benji.bowman
 * Move to EGL 1.3
 * Revision 1.2  2006/04/03 09:39:47  benji.bowman
 * Fixes for calling convention
 * Revision 1.1  2006/02/14 17:34:50  stuart.smith
 * Initial revision
 *
 * Revision 1.1  2005/10/10 10:50:35  benji.bowman
 * Initial revision
 **************************************************************************/
#ifndef _egltypes_h_
#define _egltypes_h_

#if defined(_WIN32) || defined(__VC32__)             /* Win32 */
#   if defined (_DLL_EXPORTS)
#       define EGLAPI __declspec(dllexport)
#   else
#       define EGLAPI __declspec(dllimport)
#   endif
#elif defined (__ARMCC_VERSION)                      /* ADS */
#   define EGLAPI
#elif defined (__SYMBIAN32__) && defined (__GCC32__) /* Symbian GCC */
#   define EGLAPI __declspec(dllexport)
#elif defined (__GNUC__)                             /* GCC dependencies (kludge) */
#   define EGLAPI
#elif defined (_UITRON_)
#	define EGLAPI
#endif

#if !defined (EGLAPI)
#   error Unsupported platform!
#endif

#define EGLAPIENTRY

#if defined __linux__
	#include <sys/types.h>
	#if defined(SUPPORT_X11)
		#include <X11/Xlib.h>
		typedef Display*	EGLNativeDisplayType;
		typedef Window		EGLNativeWindowType;
		typedef Pixmap		EGLNativePixmapType;
	#else
		typedef int		EGLNativeDisplayType;
		typedef void*	EGLNativeWindowType;
		typedef void*	EGLNativePixmapType;
	#endif
#elif defined(UNDER_CE) || defined(_WIN32)
	typedef int int32_t;
	#undef UNREFERENCED_PARAMETER
	#include <windows.h>
	typedef HDC		EGLNativeDisplayType;
	typedef HWND	EGLNativeWindowType;
	typedef void*	EGLNativePixmapType;
#elif defined(__SYMBIAN32__)
	#include <e32def.h>
	typedef TInt EGLNativeDisplayType;
	/*
		Declare these as void although they points to classes - we can't
		include	a C++ header file as the EGL files are all written in C.
	*/
	#define EGLNativeWindowType void* /* Is really an RWindow* */
	#define EGLNativePixmapType void* /* Is really a CFbsBitmap* */
#elif defined(_UITRON_)
	typedef int int32_t;	
	typedef int		EGLNativeDisplayType;
	typedef void*	EGLNativeWindowType;
	typedef void*	EGLNativePixmapType;
#else
	typedef int		EGLNativeDisplayType;
	typedef void*	EGLNativeWindowType;
	typedef void*	EGLNativePixmapType;
#endif

#endif /* _egltypes_h_ */
