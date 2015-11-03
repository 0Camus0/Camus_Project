#ifndef CA_FILESYS_H
#define CA_FILESYS_H

#include <Utils/MemoryTracker.h>
#include <string>

namespace hyperspace {
	namespace fs {
		class Filesystem {
		public:
			void	InitFS();
			std::string GetResourcesPath();

			static Filesystem* instance();

		private:
			Filesystem() {
				MemAppendHeap(Filesystem);
			}



			std::string ResPath;

		};
	}
}

#endif