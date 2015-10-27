/*=============================================================================
Framework.h	: Framework for OGLES
Revision history:
* Created by Camus
=============================================================================*/

#ifndef D3DFRAMEWORK_H
#define D3DFRAMEWORK_H


#include "Core.h"
#include "OpenglDrv.h"
#include "Timer.h"
#include "EventManager.h"


class OGLFramework : public FrameworkBase
{
public:
	~OGLFramework(){ Release(); }
	OGLFramework( Base* pGameApp );
	virtual bool Init(char* title, int width, int height, bool windowed);
	virtual void SetGameApp(Base* pApp);
	virtual void Run();
	virtual void Release();
	virtual void Close();


	int	GetWidth(){ return WindowWidth; }
	int GetHeight(){ return WindowHeight; }

	bool			Windowed;

private:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnUpdate();
	virtual void OnRender();

        SDL_Event       evento;
        void ProcessEvent(SDL_Event *event);

	#ifdef _WIN32


		HWND            TheHwnd;
		HINSTANCE       ThehInstance;
		WINDOWPLACEMENT Wp;



	#else

		SDL_Surface *screen;
		SDL_GLES_Context *context;

	#endif


	void Pause( bool rendering, bool timer );


	bool            Active;
	bool            RenderingPaused;
	bool            TimerPaused;
	bool            Closing;
	bool			quit;

	int             WindowWidth;
	int             WindowHeight;
	int             FullscreenWidth;
	int             FullscreenHeight;
	int             RenderingPauseCount;
	int             TimerPauseCount;



	Base*		   ObjGameApp;
	Timer		   *timer;


};

#endif
