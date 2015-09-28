#ifndef CA_WIN32_APP_H
#define CA_WIN32_APP_H

#include <Core/Core.h>
#include <Driver/BaseDriver.h>
#include <memory>

class Win32App : public RootApp {
	public:
		Win32App() : 
		  m_bAlive(true){}
		void InitGlobalVars();
		void OnCreateApplication();
		void OnDestroyApplication();
		void OnInterruptApplication();
		void OnResumeApplication();
		void UpdateApplication();

		bool						m_bAlive;
		std::unique_ptr<BaseDriver>	m_pVideoDriver;
};


#endif