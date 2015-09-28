#ifndef CA_BASEDRIVER_H
#define CA_BASEDRIVER_H


class BaseDriver {
public:
	virtual	void	InitDriver()	= 0;
	virtual void	CreateSurfaces()  = 0;
	virtual void	DestroySurfaces() = 0;
	virtual void	SwapBuffers()	  = 0;
	virtual void	DestroyDriver() = 0;
};

#endif