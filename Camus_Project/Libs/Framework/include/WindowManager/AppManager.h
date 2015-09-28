#ifndef CA_APP_MANAGER_H
#define CA_APP_MANAGER_H

#include <Core/Core.h>
#include <memory>

class AppManager {
public:
	std::unique_ptr<RootApp>	pApp;

	void	CreateApp();
	void	MainAppThread();


};

AppManager&	GetAppManager();

#endif