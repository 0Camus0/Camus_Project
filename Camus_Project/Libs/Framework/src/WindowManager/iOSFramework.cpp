
#ifdef __APPLE__

#include <WindowManager/iOSFramework.h>
#include <Utils/WindowProperties.h>
#include <Utils/Log.h>
#include <Driver/OpenGLDriver.h>
#include <Utils/FileSystem.h>

#include <unistd.h>

#ifdef USE_C11_THREADS
#include <thread>
#include <mutex>
#include <condition_variable>
extern std::mutex						g_mutex;
extern std::condition_variable			g_cond;
#else
#include <pthread.h>
#include <sched.h>
extern pthread_mutex_t					g_mutex;
extern pthread_cond_t					g_cond;
#endif
extern bool g_bAppRunning;
extern hyperspace::RootFramework			*pFramework;

EAGLContext *g_EAGLContext = nil;

GLView*     g_View = nil;

void Suspend() {
    LogPrintDebug("Suspend APP");
#ifdef USE_C11_THREADS
    g_mutex.lock();
    g_bAppRunning = false;
    g_mutex.unlock();
#else
    pthread_mutex_lock(&g_mutex);
    g_bAppRunning = false;
    pthread_mutex_unlock(&g_mutex);
#endif
}

void Resume() {
    LogPrintDebug("Resume APP");
#ifdef USE_C11_THREADS
    g_mutex.lock();
    g_bAppRunning = true;
    g_cond.notify_all();
    g_mutex.unlock();
#else
    pthread_mutex_lock(&g_mutex);
    g_bAppRunning = true;
    pthread_cond_broadcast(&g_cond);
    pthread_mutex_unlock(&g_mutex);
#endif
}

void CheckSuspend(iOSFramework* pApp) {
#ifdef USE_C11_THREADS
    std::unique_lock<std::mutex> locker(g_mutex);
    while (!g_bAppRunning) {
        pFramework->OnInterruptApplication();
        LogPrintDebug("CheckSuspend while waiting");
        g_cond.wait(locker);
    }
#else
    pthread_mutex_lock(&g_mutex);
    while (!g_bAppRunning) {
        LogPrintDebug("CheckSuspend while waiting");
        pthread_cond_wait(&g_cond, &g_mutex);
    }
    pthread_mutex_unlock(&g_mutex);
#endif
}

void iOSFramework::InitGlobalVars(){
    LogPrintDebug("iOSFramework - InitGlobalVars");
}

void iOSFramework::OnCreateApplication(){
   LogPrintDebug("iOSFramework - OnCreateApplication");
    
   hyperspace::fs::Filesystem::instance()->InitFS();
    
  
    
   pVideoDriver = new hyperspace::video::OpenGLDriver();


    if(!g_EAGLContext || ![EAGLContext setCurrentContext:g_EAGLContext]){
        LogPrintDebug("iOSFramework - OnCreateApplication - Can attach GL Context");
        exit( 1 );
    }
  
 
   pVideoDriver->InitDriver();
    
   pBaseApp->CreateAssets();
    
    

}

void iOSFramework::OnDestroyApplication(){
   LogPrintDebug("iOSFramework - OnDestroyApplication");
}

void iOSFramework::OnInterruptApplication(){
   LogPrintDebug("iOSFramework - OnInterruptApplication");
}

void iOSFramework::OnResumeApplication(){
   LogPrintDebug("iOSFramework - OnResumeApplication");
}

void iOSFramework::UpdateApplication(){
  // LogPrintDebug("iOSFramework - UpdateApplication");
    
    
    if (!pBaseApp->bPaused) {
        pVideoDriver->Update();
        pBaseApp->OnUpdate(0);
        pBaseApp->OnDraw();
    }
   // sleep(1);
    
    CheckSuspend(this);
}

void iOSFramework::ProcessInput(){
   LogPrintDebug("iOSFramework - ProcessInput");
}

void iOSFramework::ResetApplication(){
   LogPrintDebug("iOSFramework - ResetApplication");
}

#include <Utils/Log.h>

@implementation GLViewController

-(void)loadView
{
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    GLView*   View = [[GLView alloc] initWithFrame:screenBounds];
    
    self.view = View;
    
}


@end


@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}


- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.contentScaleFactor = 2.0;
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)super.layer;
        
        eaglLayer.opaque = YES;
        eaglLayer.contentsScale = 2.0f;
        self.multipleTouchEnabled = true;
        
        g_EAGLContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        if(!g_EAGLContext || ![EAGLContext setCurrentContext:g_EAGLContext]){
            return nil;
        }
        
        GLuint framebuffer,renderbuffer;
        glGenFramebuffersOES(1,&framebuffer);
        glGenRenderbuffersOES(1,&renderbuffer);
        
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderbuffer);
        
        [g_EAGLContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
        
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,GL_COLOR_ATTACHMENT0_OES,GL_RENDERBUFFER_OES,renderbuffer);
        
        CGSize o = [[[UIScreen mainScreen] currentMode] size];

        glViewport(0,0,o.width,o.height);
        
        
        
    }
    return self;
}


- (void)dealloc
{
    if([EAGLContext currentContext]==g_EAGLContext)
        [EAGLContext setCurrentContext:nil];
    
}


@end

@implementation AppDelegate


@synthesize window=_window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    LogPrintDebug("AppDelegate didFinishLaunchingWithOptions");

    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGSize o = [[[UIScreen mainScreen] currentMode] size];
    
    hyperspace::GetWindowParameters().SetParametersFromDriver(o.width,o.height);
    
    _window = [[UIWindow alloc] initWithFrame:screenBounds];
    g_View = [[GLView alloc] initWithFrame:screenBounds];
    [UIApplication sharedApplication].idleTimerDisabled = YES;
    
    UIViewController *m_ViewController = [UIViewController alloc];
    
    m_ViewController.view = g_View;
    
    [self.window addSubview:g_View];
    
    [self.window setRootViewController:m_ViewController];
    
    [self.window makeKeyAndVisible];
   
    
    Resume();
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
    Suspend();
    LogPrintDebug("AppDelegate applicationWillResignActive");
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    LogPrintDebug("AppDelegate applicationDidEnterBackground");
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    LogPrintDebug("AppDelegate applicationWillEnterForeground");
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
    Resume();
    LogPrintDebug("AppDelegate applicationDidBecomeActive");
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    LogPrintDebug("AppDelegate applicationWillTerminate");
}

- (void) timecheck{
    static double timed = 0.0;
    
    //  double dif = [[NSDate date] timeIntervalSince1970] - timed;
    
    //  printf("time main: %f\n",1.0f/dif);
    
    //if(!Rthread.alive)
    //      [self applicationWillTerminate];
    
    
    timed = [[NSDate date] timeIntervalSince1970];
    
}


@end

#endif