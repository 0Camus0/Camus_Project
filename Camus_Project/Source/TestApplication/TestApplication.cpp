// TestApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <WindowManager/AppManager.h>

#include <iostream>
#include <memory.h>

std::unique_ptr<AppManager> pAppManager;

int main()
{
	pAppManager = std::unique_ptr<AppManager>(new AppManager);

	pAppManager->CreateApp();

	std::cout << "Join..." << std::endl;

	pAppManager->Join();

	std::cout << "After Join, ready to close" << std::endl;


    return 0;
}

