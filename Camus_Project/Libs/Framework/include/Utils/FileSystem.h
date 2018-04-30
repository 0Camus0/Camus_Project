#ifndef T1000_FILESYS_H
#define T1000_FILESYS_H

#include <string>

namespace t1000 {
	namespace fs {
		class Filesystem {
		public:			
			void	InitFS();
			void	InitFS(std::string inter,std::string external,std::string apk);
			std::string GetResourcesPath();

			void *GetFile(std::string path,int *size);
			void ReleaseFile(void *);

			static Filesystem* instance();

		private:
			Filesystem() {
				bLoadedAPK = false;
			}

			void	OpenAPK(std::string apk_path);
			void	CloseAPK();

			bool bLoadedAPK;

			std::string ResPath;

			std::string InternalPath;
			std::string ExternalPath;

		};
	}
}

#endif