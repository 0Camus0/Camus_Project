#ifndef CA_IOSFRAMEWORK_H
#define CA_IOSFRAMEWORK_H



#include <Core/Core.h>
#include <Driver/BaseDriver.h>


#import <UIKit/UIKit.h>

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;


@end

class iOSFramework : public hyperspace::RootFramework {
public:
    iOSFramework(hyperspace::AppBase *pBaseApp) : RootFramework(pBaseApp) {
        
    }
    void InitGlobalVars();
    void OnCreateApplication();
    void OnDestroyApplication();
    void OnInterruptApplication();
    void OnResumeApplication();
    void UpdateApplication();
    void ProcessInput();
    void ResetApplication();
    ~iOSFramework() {
        
    }
};


#endif 
