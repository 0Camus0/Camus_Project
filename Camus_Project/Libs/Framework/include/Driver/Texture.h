#ifndef CA_TEXTURE_H
#define CA_TEXTURE_H


#include <fstream>
#include <string>

#include <config.h>


#define TEXTURE_NOT_FOUND	0xFFFF
#define TEXTURE_FOUND	0

namespace hyperspace {
	namespace video {

		enum channelS_ {
			CH_ALPHA	 = (1 << 0),
			CH_RGB		 = (1 << 1),
			CH_RGBA		 = (1 << 2),
		};

		enum pixel_format_ {
			INTEGER_8	= (1 << 3),
			UINTEGER_8	= (1 << 4),
			FLOAT_8		= (1 << 5),
			INTEGER_16	= (1 << 6),
			UINTEGER_16 = (1 << 7),
			INTEGER_32	= (1 << 8),
			UINTEGER_32	= (1 << 9),
			FLOAT_32	= (1 << 10),
		};

		enum props_ {
			CUBE_MAP	= (1 << 11),
			COMPRESSED	= (1 << 12)
		};

		enum bpp_ {
			BPP_2 = (1 << 13),
			BPP_4 = (1 << 14),
			BPP_8 = (1 << 15)
		};

		enum file_format {
			BMP	= (1 << 16),
			PNG	= (1 << 17),
			DDS	= (1 << 18),
			TGA = (1 << 19),
			PVR = (1 << 20),
			KTX = (1 << 21)
		};

		enum compress_format {
			RAW		 = (1 << 22),
			ETC1	 = (1 << 23),
			ETC2	 = (1 << 24),
			DXT1	 = (1 << 25),
			DXT5	 = (1 << 26),
			PVRTC2   = (1 << 27),
			PVRTC4   = (1 << 28),
			PVRTCII2 = (1 << 29),
			PVRTCII4 = (1 << 30),
 		};
	
		
		class Texture {
		public:
			Texture() :	size(0),
						offset(0),
						props(0),
						x(0),
						y(0),
						id(0),
						bounded(0),
						mipmaps(0)
						{
							
						}

			
			~Texture() {
			
			}
			unsigned int	size;
			unsigned int	offset;
			unsigned int	props;
			unsigned short	x, y;
			unsigned short	id;
			unsigned char	bounded;
			unsigned char	mipmaps;
		};

		class TextureManager {
		public:
			TextureManager();
			~TextureManager() {
			
			}
			unsigned short	LoadTexture(std::string filename);

			unsigned int	CheckFormat(std::ifstream &in);
	
			unsigned int	LoadBufferUncompressed(std::string &Path,unsigned int format);

			unsigned int    LoadBufferCompressed(std::string &Path,unsigned int format);

			static unsigned int	num_textures_loaded;
			static unsigned int	current_index;

			static Texture			textures[MAX_TEXURE_LIMIT];
			static unsigned char	tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
			static			char	tex_paths_pool[MAX_TEXURE_LIMIT][MAX_PATH_SIZE];
		};
	}
}

#endif