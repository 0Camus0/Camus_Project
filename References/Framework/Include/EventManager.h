/*=============================================================================
EventManager.h  : Event manager.
Revision history:
* Created by Camus
=============================================================================*/

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include "GlobalHeader.h"




class EventManager
{
public:


	static EventManager* Instance();



	void Init();
	void SetScreenSize(int x,int y);

	

	void LockKey(int i);

	int	 GetXDiff(){return -Xdiff;}
	int	 GetYDiff(){return -Ydiff;}

	bool* GetPressedKeys(){ return PressedKeys; }

	
	SDL_Event       evento;
	bool			PressedKeys[320];
	bool			KeyLock[320];
	bool			PressedButs[3];
	bool			LockButs[3];
	SDL_Event*	GetEvent(){ return &evento; }
	void ReadEvents();
	

	int		XPosActual;
	int		YPosActual;
	int		XPosUltima;
	int		YPosUltima;

	int		Xdiff;
	int		Ydiff;

	int		XSize;
	int		YSize;



private:
	EventManager(){}



};

#endif
