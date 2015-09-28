#ifndef CA_APP_MANAGER_H
#define CA_APP_MANAGER_H

#include <Core/Core.h>
#include <memory>
#include <thread>

class AppManager {
public:
	std::unique_ptr<RootApp>	pApp;

	void	CreateApp();
	void	MainAppThread();
	void	Join();

	std::thread	_thread;


};

AppManager&	GetAppManager();

#endif