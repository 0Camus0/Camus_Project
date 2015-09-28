
#include <WindowManager/Win32App.h>
#include <Utils/WindowProperties.h>

// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

#include <Driver/OpenGLDriver.h>


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

}

void Win32App::OnDestroyApplication() {
		
	SDL_Quit();

}

void Win32App::OnInterruptApplication() {

}

void Win32App::OnResumeApplication() {

}

void Win32App::UpdateApplication() {

}