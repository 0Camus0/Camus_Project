#include "../Include/GlobalHeader.h"
#include "../Include/EventManager.h"

#include "../Include/OpenglDrv.h"


EventManager* EventManager::Instance()
{
	static EventManager instance;
	return &instance;
}

void EventManager::ReadEvents()
{
		SDL_ShowCursor(SDL_DISABLE);
                while(SDL_PollEvent( &evento )){

					if(evento.type==SDL_VIDEORESIZE)
					{
						SDL_SetVideoMode(evento.resize.w,evento.resize.h,32,SDL_HWSURFACE | SDL_RESIZABLE);

						OglDrv::Instance()->RecreateSurface();

						
						
						//WindowWidth = event->resize.w;
						//WindowHeight = event->resize.h;
					}

					if(evento.type==SDL_KEYDOWN){
							PressedKeys[evento.key.keysym.sym] = true;
					}

					if(evento.type==SDL_KEYUP){
						PressedKeys[evento.key.keysym.sym] = false;
					}
       }

		int x=0,y=0;
		for(int i = 1; i < 3 ; i++){
			if(SDL_GetMouseState(&x,&y)&SDL_BUTTON(i)){		
				PressedButs[i-1] = !(LockButs[i-1]);
			}else{
				PressedButs[i-1] = false;
				LockButs[i-1] = false;
			}
		}

#ifdef _WIN32

		XPosActual  = x;
		YPosActual  = y;

		Xdiff = XPosActual - XSize/2;
		Ydiff = YPosActual - YSize/2;

		//Xdiff = XPosActual - XPosUltima;
		//Ydiff = YPosActual - YPosUltima;

		XPosUltima = XPosActual;
		YPosUltima = YPosActual;

		SDL_WarpMouse(XSize/2,YSize/2);

#else
		static int cont = 0;
		if(PressedButs[0])
			cont++;
		else
			cont=0;

		XPosActual  = x;
		YPosActual  = y;

		if(cont==1){
			XPosUltima = XPosActual;
			YPosUltima = YPosActual;
		}

		Xdiff = XPosActual - XPosUltima;
		Ydiff = YPosActual - YPosUltima;

		XPosUltima = XPosActual;
		YPosUltima = YPosActual;
#endif
		if(!PressedButs[0]){
			Xdiff = Ydiff = 0;
		}	
}


void EventManager::LockKey(int i)
{
	PressedKeys[i] = false;
//	KeyLock[i] = true;
//	cout << "bloqueado index: " << i << endl;
}

void EventManager::Init()
{
	int range;

	#ifdef _WIN32
		range = 320;
	#else
		range = 100;
	#endif

	for (int i = 0; i < 320 ; i++)
	{
		KeyLock[i] = false;
		PressedKeys[i] = false;
	}

	for(int i = 1; i < 3 ; i++){
			PressedButs[i-1] = false;
			LockButs[i-1] = false;
	}

	XPosActual = YPosActual = Xdiff = Ydiff = XPosUltima = YPosUltima = 0;
}

void EventManager::SetScreenSize(int x,int y)
{
	XSize = x;
	YSize = y;
}
