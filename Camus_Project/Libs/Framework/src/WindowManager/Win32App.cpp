
#include <WindowManager/Win32App.h>
#include <Utils/WindowProperties.h>

// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

#include <Driver/OpenGLDriver.h>

extern bool g_bAppRunning;


void Win32App::InitGlobalVars() {
	GetWindowParameters().GatherProperties();
}

void Win32App::OnCreateApplication() {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Camus Framework",0);

	int flags = SDL_HWSURFACE;
	auto parameters = GetWindowParameters();
	
	if (parameters.Properties & WindowParameters::FULL_SCREEN)
		flags |= SDL_FULLSCREEN;

	if (parameters.Properties & WindowParameters::BORDERLESS)
		flags |= SDL_NOFRAME;

	if (parameters.Properties & WindowParameters::RESIZEABLE)
		flags |= SDL_RESIZABLE;

	SDL_SetVideoMode(parameters.Width, parameters.Height, 32, flags);

	ShowCursor((parameters.Properties & WindowParameters::SHOW_CURSOR) ? TRUE : FALSE);

	m_pVideoDriver = std::unique_ptr<BaseDriver>(new OpenGLDriver);

	m_pVideoDriver->InitDriver();

}

void Win32App::OnDestroyApplication() {
		
	SDL_Quit();

}

void Win32App::OnInterruptApplication() {

}

void Win32App::ResetApplication() {

}

void Win32App::OnResumeApplication() {

}

void Win32App::UpdateApplication() {

	ProcessInput();	

	m_pVideoDriver->Update();

	m_pVideoDriver->SwapBuffers();
}

void Win32App::ProcessInput() {
	SDL_Event       evento;

	while (SDL_PollEvent(&evento)) {
		if (evento.type == SDL_KEYDOWN) {
			if (evento.key.keysym.sym == SDLK_q) {
				g_bAppRunning = false;
			}
		}

	}
}
