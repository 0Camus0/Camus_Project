
#include <WindowManager/Win32App.h>
#include <Utils/WindowProperties.h>

// SDL
#include <SDL/SDL.h>
// Windows 
#include <windows.h>
#include <mmsystem.h>

#include <Driver/OpenGLDriver.h>

#ifdef OS_WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

#include <algorithm>
#include <iostream>
#include <cmath>


float clip(float n, float lower, float upper) {
	return std::max(
		lower,
		std::min(n, upper));
}

extern bool g_bAppRunning;


void Win32App::InitGlobalVars() {
	hyperspace::GetWindowParameters().GatherProperties();
}

void Win32App::OnCreateApplication() {

	SDL_Init(SDL_INIT_VIDEO);
	SDL_WM_SetCaption("Camus Framework",0);

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

	pVideoDriver->Update();

	static float ang = 0.0f;

	float R = 0.0f, G = 0.0f, B = 0.0f;

	ang += 0.1f;

	R = (clip(std::sin(ang), 0.0f, 1.0f))*0.5f + 0.5f;
	G = (clip(std::cos(ang + .70f), 0.0f, 1.0f))*0.5f + 0.5f;
	B = (clip(std::tan(ang + 1.44f), 0.0f, 1.0f))*0.5f + 0.5f;

	pVideoDriver->Clear(hyperspace::video::draw_bits_::COLOR_BIT, R, G, B, 1.0f);
	pVideoDriver->SwapBuffers();
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
