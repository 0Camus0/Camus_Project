#ifndef CA_BASEDRIVER_H
#define CA_BASEDRIVER_H


class BaseDriver {
public:
	void	InitDriver()	= 0;

	void	CreateSurfaces()  = 0;
	void	DestroySurfaces() = 0;
	void	SwapBuffers()	  = 0;

	void	DestroyDriver() = 0;
};

#endif