#include <Utils\FileSystem.h>
#include <Utils\Log.h>

#ifdef __APPLE__
#import <UIKit/UIKit.h>
#endif

#include <fstream>

namespace hyperspace {
	namespace fs {

		Filesystem* Filesystem::instance() {
			static Filesystem inst;
			return &inst;
		}

		void Filesystem::InitFS() {
#ifdef OS_WIN32
			ResPath = "Resourcez/";
#elif defined(OS_ANDROID)
			ResPath = "/storage/sdcard0/Resourcez/";
#elif defined(__APPLE__)
            const char* paths = [[[NSBundle mainBundle] resourcePath] UTF8String];
            ResPath = std::string(paths) + std::string("/Resourcez/");
#endif
		}

		std::string Filesystem::GetResourcesPath() {
			return ResPath;
		}
	}
}