#include "../Include/GlobalHeader.h"

#include "../Include/OpenglDrv.h"

#include <fstream>
using namespace std;

OglDrv* OglDrv::Instance()
{
	static OglDrv instance;
	return &instance;
}

#ifdef _WIN32

/*
int		ResolutionW;
int		ResolutionH;
bool	Windowed;
bool	HighQShadows;
bool	HighQReflect;
*/
void OglDrv::ReadProperties(){
	ifstream in;
	in.open("Config.txt");

	if(!in.good()){
		in.close();
		ResolutionW = 1280;
		ResolutionH = 720;
		Windowed = true;
		HighQShadows = true;
		HighQReflect = true;
		return;
	}

	string tmp;
	in >> tmp >> ResolutionW;
	in >> tmp >> ResolutionH;
	in >> tmp >> Windowed;
	in >> tmp >> HighQShadows;
	in >> tmp >> HighQReflect;

	if(ResolutionW<=0){
		ResolutionW = 1280;
		ResolutionH = 720;
		Windowed = true;
		HighQShadows = true;
		HighQReflect = true;
	}

	if(ResolutionH<=0){
		ResolutionW = 1280;
		ResolutionH = 720;
		Windowed = true;
		HighQShadows = true;
		HighQReflect = true;
	}
	in.close();
}

void OglDrv::InitOgl(HWND hWnd, bool window)
{
	eglWindow = hWnd;

	HDC	hDC	= GetDC(hWnd);

	eglDisplay = eglGetDisplay(hDC);

	EGLint iMajorVersion, iMinorVersion;

	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		ReportError("eglInitialize() failed.");
	//	goto cleanup;
	}

	eglBindAPI(EGL_OPENGL_ES_API);
	if (!ReportError("eglBindAPI"))
	{
		//goto cleanup;
	}

	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,				0,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,
		EGL_DEPTH_SIZE,			EGL_DONT_CARE,
		EGL_NONE
	};

	int iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		ReportError("eglChooseConfig() failed.");
	//	goto cleanup;
	}

	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);
	if (!ReportError("eglCreateWindowSurface"))
	{
	//	goto cleanup;
	}

	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);
	if (!ReportError( "eglCreateContext"))
	{
	//	goto cleanup;
	}

	
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!ReportError( "eglMakeCurrent"))
	{
	//	goto cleanup;
	}






// cleanup:
//
// //	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
// //	eglTerminate(eglDisplay);
}

void OglDrv::RecreateSurface(){

	
}

#else

void OglDrv::InitOgl(Display* dpy, Window &win, bool window)
{

	eglDisplay = eglGetDisplay((EGLNativeDisplayType)dpy);

	if(eglDisplay == EGL_NO_DISPLAY) {
		printf("Got no EGL display\n");

	}

	if(!eglInitialize(eglDisplay, NULL, NULL)) {
		printf("Unable to initialize EGL\n");

	}

	EGLint attr[] = {
		EGL_BUFFER_SIZE, 16,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_TRUE,
		EGL_DEPTH_SIZE,			16,
		EGL_NONE
	};

	EGLConfig ecfg;
	EGLint num_config;
	if(!eglChooseConfig(eglDisplay, attr, &ecfg, 1, &num_config)) {
		printf("Failed to choose config (%x)\n", eglGetError());

	}
	if(num_config != 1) {
		printf("Didn't get exactly one config, but %d\n", num_config);

	}

        eglSurface = eglCreateWindowSurface(eglDisplay, ecfg, win, NULL);
	if(eglSurface == EGL_NO_SURFACE) {
		printf("Unable to create EGL surface (%x)\n", eglGetError());

	}

	EGLint ctxattr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, ecfg, EGL_NO_CONTEXT, ctxattr);
	if(eglContext == EGL_NO_CONTEXT) {
		printf("Unable to create EGL context (%x)\n", eglGetError());

	}

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

}

#endif


bool OglDrv::ReportError(char* Report)
{
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		char pszStr[256];
		sprintf(pszStr, "%s failed (%d).\n", Report, iErr);

		#ifdef _WIN32
		MessageBox(0, pszStr, "Error", MB_OK|MB_ICONEXCLAMATION);
		#else
		printf("%s",pszStr);
		#endif

		return false;
	}

	return true;
}
