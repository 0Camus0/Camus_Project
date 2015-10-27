#include "../Include/GlobalHeader.h"
#include "../Include/Framework.h"


using namespace std;


OGLFramework::OGLFramework( Base* pGameApp  )
{
	ObjGameApp = pGameApp;
	Active = true;
	RenderingPaused = false;
	TimerPaused = false;
	Closing = false;
	timer = NULL;

	#ifdef _WIN32

	#else

		display = NULL;

	#endif

}

void OGLFramework::SetGameApp(Base* pGameApp )
{
	ObjGameApp = NULL;
	ObjGameApp = pGameApp;
}

void OGLFramework::Release()
{
	Closing = true;

	OnDestroy();
}

bool OGLFramework::Init( char* title, int width, int height, bool windowed )
{
	WindowWidth = width;
	WindowHeight = height;

	Active = true;

	Windowed = windowed;

	#ifdef _WIN32

		HINSTANCE hInstance = GetModuleHandle(NULL);

		ThehInstance = hInstance;

		SDL_Init(SDL_INIT_VIDEO);

		SDL_WM_SetCaption(title,"Texturas/chile.ico");
	//	SDL_WM_SetIcon()
	
		if(windowed)
			SDL_SetVideoMode(width,height,32,SDL_HWSURFACE | SDL_RESIZABLE|SDL_NOFRAME);
		else
			SDL_SetVideoMode(width,height,32,SDL_HWSURFACE|SDL_FULLSCREEN);
		

		

		OglDrv::Instance()->InitOgl(GetActiveWindow());
	#else

	int res;
	res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	

	res = SDL_GLES_Init(SDL_GLES_VERSION_2_0);
	

        screen = SDL_SetVideoMode(0, 0, 16, SDL_SWSURFACE|SDL_FULLSCREEN);


	SDL_WM_SetCaption("SDLgles v2 test", "SDLgles v2 test");
	SDL_ShowCursor(SDL_DISABLE);

        SDL_GLES_SetAttribute(SDL_GLES_BUFFER_SIZE,16);
        SDL_GLES_SetAttribute(SDL_GLES_DEPTH_SIZE,16);

	context = SDL_GLES_CreateContext();


	res = SDL_GLES_MakeCurrent(context);



// 		display = XOpenDisplay(NULL);
// 
// 
// 
// 		Window root = DefaultRootWindow(display);
// 
// 
//                 XSetWindowAttributes swa;
// 
// 		swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask |ButtonPressMask | ButtonReleaseMask ;
// 
// 
// 
//                 win = XCreateWindow(display, root, 0, 0, width,height, 0, CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &swa);
// 
// 
// 		Atom wm_state = XInternAtom(display, "_NET_WM_STATE", True);
//                 Atom fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", True);
// 	/*	Atom mh = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
// 		Atom mv = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);*/
// 
// 
//                 XChangeProperty(display, win, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&fullscreen, 1);
// 	/*	XChangeProperty(display, win, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&mh, 1);
// 		XChangeProperty(display, win, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&mv, 1);*/
// 
// 		int one = 1;
// 		Atom non_composited = XInternAtom(display, "_HILDON_NON_COMPOSITED_WINDOW", False);
// 		XChangeProperty(display, win, non_composited, XA_INTEGER, 32, PropModeReplace, (unsigned char*)&one, 1);
// 
// 
// 
// 
// 		XMapWindow(display, win);
// 
// 		XStoreName(display, win, title);
// 
// 
// 
// 		OglDrv::Instance()->InitOgl(display,win);
// 
// 		Window fw;
// 		int state;
// 		XGetInputFocus(display,&fw,&state);
// 
// 
// 		XSelectInput(display,fw,ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask );




	#endif


	FullscreenWidth = width;
	FullscreenHeight = height;


	OnCreate();
	Pause( false, false );

	timer = new Timer();
	timer->Start();

	EventManager::Instance()->Init();
	EventManager::Instance()->SetScreenSize(FullscreenWidth,FullscreenHeight);

	return true;
}

void OGLFramework::Run()
{
	while ( !Closing )
	{


	EventManager::Instance()->ReadEvents();
	ProcessEvent( EventManager::Instance()->GetEvent() );

		if ( Active )
		{
			OnUpdate();
			OnRender();
		}else
		{
			break;
		}
	}

	OnDestroy();
}


void OGLFramework::ProcessEvent(SDL_Event *event)
{
/*	if(event->type==SDL_KEYDOWN) {
		if(event->key.keysym.sym == SDLK_q ){ SDL_Quit(); exit(0);}}
	if(event->type==SDL_QUIT) {  SDL_Quit(); exit(0);}
	if(event->type==SDL_VIDEORESIZE)
	{
		

		WindowWidth = event->resize.w;
		WindowHeight = event->resize.h;
	}*/
}

void OGLFramework::OnCreate()
{
	if ( ObjGameApp != NULL )
	{
		ObjGameApp->OnCreate();

		if(Windowed)
			SDL_SetVideoMode(WindowWidth,WindowHeight,32,SDL_HWSURFACE | SDL_RESIZABLE);
	}
}

void OGLFramework::OnDestroy()
{

		ObjGameApp->OnDestroy();

		#ifdef _WIN32

		#else
		eglDestroyContext(OglDrv::Instance()->eglDisplay, OglDrv::Instance()->eglContext);
		eglDestroySurface(OglDrv::Instance()->eglDisplay, OglDrv::Instance()->eglSurface);
		eglTerminate(OglDrv::Instance()->eglDisplay);
		XDestroyWindow(display, win);
		XCloseDisplay(display);
		#endif

		SDL_Quit();
}

void OGLFramework::Close(){
	Closing = true;
}

void OGLFramework::OnUpdate()
{
	timer->Update();

	if ( ObjGameApp != NULL )
	{
			float elapsedTime = timer->GetElapsedTime();
			ObjGameApp->OnInput(elapsedTime);
			ObjGameApp->OnUpdate(elapsedTime);
	}
}

void OGLFramework::OnRender()
{
	if ( !Active  )
	{
		return;
	}

	if ( ObjGameApp != NULL )
	{
		ObjGameApp->OnRender();
	}

}


void OGLFramework::Pause( bool rendering, bool timerd )
{
	RenderingPauseCount += rendering ? 1 : -1;
	RenderingPauseCount = (RenderingPauseCount < 0) ? 0 : RenderingPauseCount;

	TimerPauseCount += timerd ? 1 : -1;
	TimerPauseCount = (TimerPauseCount < 0) ? 0 : TimerPauseCount;

	RenderingPaused = (RenderingPauseCount > 0);
	TimerPaused = (TimerPauseCount > 0);

	if(timer)
	{
		if ( TimerPaused  )
		{
			timer->Stop();
		}
		else
		{
			timer->Start();
		}
	}


}
