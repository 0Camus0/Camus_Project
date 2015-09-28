#ifndef CA_CORE_H
#define CA_CORE_H


class RootApp {
	public:
		virtual void InitGlobalVars() = 0;
		virtual void OnCreateApplication() = 0;
		virtual void OnDestroyApplication() = 0;
		virtual void OnInterruptApplication() = 0;
		virtual void OnResumeApplication() = 0;
		virtual void UpdateApplication() = 0;
};


#endif