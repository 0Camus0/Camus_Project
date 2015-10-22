#include <Utils/FileSystem.h>
namespace hyperspace {
	namespace fs {

		Filesystem* Filesystem::instance() {
			static Filesystem inst;
			return &inst;
		}

		void Filesystem::InitFS() {
#ifdef OS_WIN32
			ResPath = "Resources/";
#elif defined(OS_ANDROID)
			ResPath = "/storage/sdcard0/Resources/";
#endif
		}

		std::string Filesystem::GetResourcesPath() {
			return ResPath;
		}
	}
}