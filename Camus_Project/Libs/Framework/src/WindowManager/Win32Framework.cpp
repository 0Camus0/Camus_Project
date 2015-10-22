
#include <WindowManager/Win32Framework.h>
#include <Utils/WindowProperties.h>

// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

#include <Driver/OpenGLDriver.h>

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

extern bool g_bAppRunning;


void Win32Framework::InitGlobalVars() {
	hyperspace::GetWindowParameters().GatherProperties();
}

void Win32Framework::OnCreateApplication() {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Camus Framework", 0);

	int flags = SDL_HWSURFACE;
	auto parameters = hyperspace::GetWindowParameters();

	if (parameters.Properties & hyperspace::WindowParameters::FULL_SCREEN)
		flags |= SDL_FULLSCREEN;

	if (parameters.Properties & hyperspace::WindowParameters::BORDERLESS)
		flags |= SDL_NOFRAME;

	if (parameters.Properties & hyperspace::WindowParameters::RESIZEABLE)
		flags |= SDL_RESIZABLE;

	SDL_SetVideoMode(parameters.Width, parameters.Height, 32, flags);

	ShowCursor((parameters.Properties & hyperspace::WindowParameters::SHOW_CURSOR) ? TRUE : FALSE);

	pVideoDriver = new hyperspace::video::OpenGLDriver();
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

	pBaseApp->OnUpdate(0);

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

		case SDL_ACTIVEEVENT: {


			if (((int)evento.active.state == 2 || (int)evento.active.state == 6) && (int)evento.active.gain == 0) {
				pBaseApp->OnPause();
			}

			if ((int)evento.active.state == 6 && (int)evento.active.gain == 1) {
				pBaseApp->OnResume();
			}
						
			}break;
		}
	

	}

	pBaseApp->OnInput();
}