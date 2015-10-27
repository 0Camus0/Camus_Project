/*=============================================================================
Core.h	: App and Framework Base classes
Revision history:
* Created by Camus
=============================================================================*/

#ifndef CORE_H
#define CORE_H

class Base
{
public:
	virtual ~Base(){}
	virtual void Init() = 0;
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate(float globalTime) = 0;
	virtual void OnInput(float globalTime) = 0;
	virtual void OnRender() = 0;
	virtual void Release() = 0;
};


class FrameworkBase
{
public:
	virtual bool Init(char* title, int width, int height, bool windowed) = 0;
	virtual void SetGameApp(Base* pApp) = 0;
	virtual void Run() = 0;
	virtual void Release() = 0;
	virtual void Close() = 0;
private:
	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;


};

#endif
