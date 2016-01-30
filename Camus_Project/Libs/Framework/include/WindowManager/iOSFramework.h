#ifndef CA_IOSFRAMEWORK_H
#define CA_IOSFRAMEWORK_H



#include <Core/Core.h>
#include <Driver/BaseDriver.h>


#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface GLView : UIView {
@public
   
    
}
@end


@interface AppDelegate : NSObject <UIApplicationDelegate> {

}

- (void) timecheck;

@property (nonatomic, retain) IBOutlet UIWindow *window;


@end

@interface GLViewController : UIViewController {
@public
    
    
}
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
