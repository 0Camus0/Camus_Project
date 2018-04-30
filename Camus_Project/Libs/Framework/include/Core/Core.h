#ifndef T1000_CORE_H
#define T1000_CORE_H

#include <Config.h>

#include <video/BaseDriver.h>
#include <Utils/InputBase.h>
#include <utils/InputManager.h>
#include <utils/ResourceManager.h>
#include <scene/SceneProp.h>
namespace t1000 {

	class RootFramework;

	class AppBase {
	public:
		AppBase() : bInited(false), bPaused(false), pFramework(0) {}
    virtual void InitVars() = 0;
    virtual void CreateAssets() = 0;
    virtual void LoadAssets() = 0;
    virtual void DestroyAssets() = 0;
    virtual void OnUpdate() = 0;
		virtual void OnDraw() = 0;
		virtual void OnInput() = 0;

		virtual void OnPause() = 0;
		virtual void OnResume() = 0;

		virtual void OnReset() = 0;

    virtual void LoadScene(int id) = 0;
		void	SetParentFramework(RootFramework* pParentFramework) {
			pFramework = pParentFramework;
		}

		bool			bInited;
		bool			bPaused;
    RootFramework		*pFramework;
    InputManager		IManager;
    ResourceManager		resourceManager;
	};
  class SceneBase {
  public:
    SceneBase()  {}
    virtual ~SceneBase(){}
    virtual void OnUpdate(float _DtSecs) = 0;
    virtual void OnDraw() = 0;
    virtual void OnInput(InputManager* IManager) = 0;
    virtual void OnLoadScene() = 0;
    virtual void OnDestoryScene() = 0;
    virtual void InitVars() = 0;
    virtual void CreateAssets() = 0;
    virtual void DestroyAssets() = 0;
    SceneProps		SceneProp;
    RootFramework	*pFramework;
  };
  class RootFramework {
  public:
    RootFramework(AppBase *pApp) : pBaseApp(pApp) {}
	virtual ~RootFramework() {}
    virtual void InitGlobalVars() = 0;
    virtual void OnCreateApplication(ApplicationDesc desc) = 0;
    virtual void OnDestroyApplication() = 0;
    virtual void OnInterruptApplication() = 0;
    virtual void OnResumeApplication() = 0;
    virtual void UpdateApplication() = 0;
    virtual void ProcessInput() = 0;
    virtual void ResetApplication() = 0;
    virtual void ChangeAPI(T_GRAPHICS_API::E api) = 0;

    BaseDriver			*pVideoDriver;
    AppBase				*pBaseApp;
	input::EventManager	*pEventManager;
    bool m_inited;
    ApplicationDesc aplicationDescriptor;
  };
}

#endif