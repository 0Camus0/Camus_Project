#include <Config.h>

#ifdef OS_WIN32
#include<config.h>
#include <WindowManager\Win32Framework.h>
#include <Utils\WindowProperties.h>
#include <Utils\FileSystem.h>
// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

#if defined(USING_OPENGL_ES)
#include <Driver\OpenGLDriver.h>
#else
#include <Driver\Direct3D11Driver.h>
#endif
// #ifdef OS_WIN32
// #ifdef min
// #undef min
// #endif
// #ifdef max
// #undef max
// #endif
// #endif
// 
// #include <algorithm>
// #include <iostream>
// #include <cmath>
// 
// 
// float clip(float n, float lower, float upper) {
// 	return std::max(
// 		lower,
// 		std::min(n, upper));
// }

double	g_Frequency;

extern bool g_bAppRunning;


void Win32Framework::InitGlobalVars() {
	t1000::GetWindowParameters().GatherProperties();
	LARGE_INTEGER y;
	QueryPerformanceFrequency(&y);
	g_Frequency = double(y.QuadPart)/ 1000000.0;
}

void Win32Framework::OnCreateApplication() {

	t1000::fs::Filesystem::instance()->InitFS();

	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Camus Framework", 0);

	int flags = SDL_HWSURFACE;
	auto parameters = t1000::GetWindowParameters();

	int width = parameters.WindowedWidth;
	int height = parameters.WindowedHeight;

	if (parameters.Properties & t1000::WindowParameters::FULL_SCREEN) {
		flags |= SDL_FULLSCREEN;
		width = parameters.FullScreenSelectedWidth;
		height = parameters.FullScreenSelectedHeight;
	}

	if (parameters.Properties & t1000::WindowParameters::BORDERLESS)
		flags |= SDL_NOFRAME;

	if (parameters.Properties & t1000::WindowParameters::RESIZEABLE)
		flags |= SDL_RESIZABLE;

	SDL_SetVideoMode(width, height, 32, flags);

	ShowCursor((parameters.Properties & t1000::WindowParameters::SHOW_CURSOR) ? TRUE : FALSE);

#if defined(USING_OPENGL_ES)
	pVideoDriver = new t1000::video::OpenGLDriver();
#elif defined(USING_D3D11)
	pVideoDriver = new t1000::video::Direct3D11Driver();
#endif
	pVideoDriver->SetWindow(0);
	pVideoDriver->InitDriver();

	pBaseApp->CreateAssets();

}

void Win32Framework::OnDestroyApplication() {

	pBaseApp->DestroyAssets();

	SDL_Quit();

}

void Win32Framework::OnInterruptApplication() {
	pBaseApp->OnPause();
}

void Win32Framework::ResetApplication() {

}

void Win32Framework::OnResumeApplication() {
	pBaseApp->OnResume();
}

void Win32Framework::UpdateApplication() {

	ProcessInput();
	
	if (pBaseApp->bPaused)
		return;

	pVideoDriver->Update();

	pBaseApp->OnUpdate();

	pBaseApp->OnDraw();
}

void Win32Framework::ProcessInput() {
	SDL_Event       evento;

	while (SDL_PollEvent(&evento)) {

		if(pBaseApp->bPaused && evento.type == SDL_KEYDOWN)
			continue;

		switch (evento.type) {
		case SDL_KEYDOWN: {
			if (evento.key.keysym.sym == SDLK_q) {
				g_bAppRunning = false;
			}
		}break;
#if USE_PAUSE_EVENTS
		case SDL_ACTIVEEVENT: {


			if (((int)evento.active.state == 2 || (int)evento.active.state == 6) && (int)evento.active.gain == 0) {
			pBaseApp->OnPause();
			}

			if ((int)evento.active.state == 6 && (int)evento.active.gain == 1) {
				pBaseApp->OnResume();
			}
						
			}break;
#endif
		}
	

	}

	pBaseApp->OnInput();
}

#endif
