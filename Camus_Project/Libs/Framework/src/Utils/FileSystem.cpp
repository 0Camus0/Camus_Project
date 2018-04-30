#include <Utils\FileSystem.h>
#include <Utils\Log.h>

#ifdef __APPLE__
#import <UIKit/UIKit.h>
#endif

#if defined(OS_ANDROID)
#include <android/native_activity.h>
#include <miniz.h>
#include <miniz_zip.h>
mz_zip_archive zip_archive;
#endif

#include <fstream>

namespace t1000 {
	namespace fs {

		Filesystem* Filesystem::instance() {
			static Filesystem inst;
			return &inst;
		}

		void Filesystem::InitFS() {
#ifdef OS_WIN32
			ResPath = "Resourcez/";
#elif defined(OS_ANDROID)
			//ResPath = std::string(internalDataPath) + "Resourcez/";
			//ResPath = "/storage/sdcard0/Resourcez/";
#elif defined(__APPLE__)
            const char* paths = [[[NSBundle mainBundle] resourcePath] UTF8String];
            ResPath = std::string(paths) + std::string("/Resourcez/");
#endif
		}

		void Filesystem::InitFS(std::string inter, std::string external,std::string apk) {
#ifdef OS_WIN32
			ResPath = "Resourcez/";
#elif defined(OS_ANDROID)
		    ResPath = "assets/";
			OpenAPK(apk);
#elif defined(__APPLE__)
			const char* paths = [[[NSBundle mainBundle] resourcePath] UTF8String];
			ResPath = std::string(paths) + std::string("/Resourcez/");
#endif			
		}

		void *Filesystem::GetFile(std::string path, int *size) {
			if (!bLoadedAPK) {
				LogPrintDebug("Trying to access unloaded apk, probable seg fault!");
				return 0;
			}

			unsigned int numfiles = mz_zip_reader_get_num_files(&zip_archive);


			for (unsigned int i = 0; i < numfiles; i++) {
				mz_zip_archive_file_stat file_stat;
				mz_zip_reader_file_stat(&zip_archive, i, &file_stat);
				std::string name = std::string(file_stat.m_filename);
				if (name == path) {
					size_t uncompressed_size = file_stat.m_uncomp_size;
					*size = uncompressed_size;
					void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncompressed_size, 0);
					if (!p) {
						LogPrintDebug("Failing when trying to load file [%s] from APK", file_stat.m_filename);
						return 0;
					}
					return p;
				}
			}

			LogPrintDebug("File [%s] from APK not Found", path.c_str());
			return 0;
		}

		void  Filesystem::ReleaseFile(void *p) {
			mz_free(p);
		}

		void Filesystem::OpenAPK(std::string apk_path){
			mz_bool status;

			memset(&zip_archive, 0, sizeof(zip_archive));

			status = mz_zip_reader_init_file(&zip_archive, apk_path.c_str(), 0);

			if (!status) {
				LogPrintDebug("Apk on location [%s] does not exist", apk_path.c_str());
				return;
			}
			else {
				LogPrintDebug("Apk on location [%s] does exist [GREAT!]", apk_path.c_str());
			}

			unsigned int numfiles = mz_zip_reader_get_num_files(&zip_archive);

			LogPrintDebug("Apk have [%d] files: ", numfiles);

			for (unsigned int i = 0; i < numfiles; i++) {
				mz_zip_archive_file_stat file_stat;
				mz_zip_reader_file_stat(&zip_archive, i, &file_stat);
				LogPrintDebug("Apk File [%d] with name[%s] and size[%d] ",i,file_stat.m_filename, file_stat.m_uncomp_size);
			}

			bLoadedAPK = true;
			
		}

		void Filesystem::CloseAPK() {
			mz_zip_reader_end(&zip_archive);
			bLoadedAPK = false;
		}

		std::string Filesystem::GetResourcesPath() {
			return ResPath;
		}
	}
}