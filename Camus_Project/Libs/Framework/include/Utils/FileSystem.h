#ifndef CA_FILESYS_H
#define CA_FILESYS_H

#include <string>

namespace t1000 {
	namespace fs {
		class Filesystem {
		public:
			void	InitFS();
			std::string GetResourcesPath();

			static Filesystem* instance();

		private:
			Filesystem() {
			
			}



			std::string ResPath;

		};
	}
}

#endif