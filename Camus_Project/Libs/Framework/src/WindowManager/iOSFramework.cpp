
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

void iOSFramework::InitGlobalVars(){
    LogPrintDebug("iOSFramework - InitGlobalVars");
}

void iOSFramework::OnCreateApplication(){
   LogPrintDebug("iOSFramework - OnCreateApplication");
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
   LogPrintDebug("iOSFramework - UpdateApplication");
    
    sleep(1);
}

void iOSFramework::ProcessInput(){
   LogPrintDebug("iOSFramework - ProcessInput");
}

void iOSFramework::ResetApplication(){
   LogPrintDebug("iOSFramework - ResetApplication");
}

#include <Utils/Log.h>

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    LogPrintDebug("AppDelegate didFinishLaunchingWithOptions");
    
    Resume();
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
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
    LogPrintDebug("AppDelegate applicationDidBecomeActive");
}

- (void)applicationWillTerminate:(UIApplication *)application {
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    LogPrintDebug("AppDelegate applicationWillTerminate");
}

@end

#endif