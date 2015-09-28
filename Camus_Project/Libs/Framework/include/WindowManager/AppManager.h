#ifndef CA_APP_MANAGER_H
#define CA_APP_MANAGER_H

#include <Core/Core.h>

class AppManager {
public:
	RootApp*	pApp;

	void CreateApp();

private:
	AppManager& getinstance();

};

#endif