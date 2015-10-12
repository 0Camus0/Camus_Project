#ifndef CA_BASEDRIVER_H
#define CA_BASEDRIVER_H

namespace hyperspace {
	class BaseDriver {
	public:
		virtual	void	InitDriver() = 0;
		virtual void	CreateSurfaces() = 0;
		virtual void	DestroySurfaces() = 0;
		virtual void	Update() = 0;
		virtual void	SwapBuffers() = 0;
		virtual void	DestroyDriver() = 0;
		virtual void	SetWindow(void *window) = 0;
	};
}

#endif