#ifndef CA_TEXTURE_H
#define CA_TEXTURE_H


#include <fstream>
#include <string>

#define	TEXTURE_BUDGET_SIZE_MB		256
#define TEXTURE_BUDGET_SIZE_BYTES	1024*1024*TEXTURE_BUDGET_SIZE_MB

#define MAX_TEXURE_LIMIT	100

#define NOT_FOUND_TEXTURE	0xFFFF

namespace hyperspace {
	namespace texture {

		enum channelS_ {
			ALPHA		= 0x1,
			LUMINANCE	= 0x2,
			RGB			= 0x4,
			RGBA		= 0x8,
			DEPTH		= 0x10
		};

		enum pixel_format_ {
			INTEGER_8	= 0x20,
			UINTEGER_8	= 0x40,
			FLOAT_8		= 0x80, 
			INTEGER_16	= 0x100,
			UINTEGER_16 = 0x200,
			INTEGER_32	= 0x400,
			UINTEGER_32	= 0x800,
			FLOAT_32	= 0x1000,
		};

		enum file_format {
			BMP	= 0x2000,
			PNG	= 0x4000,
			DDS	= 0x8000,
			TGA = 0x10000
		};

		enum compress_format {
			RAW  = 0x20000,
			ETC1 = 0x40000,
			ETC2 = 0x80000,
			DXT1 = 0x100000,
			DXT5 = 0x200000,
			PVR  = 0x400000
 		};
	
		enum props_ {
			MIPMAPS		= 0x800000,
			CUBE_MAP	= 0x1000000,
			ALPHA		= 0x2000000,
			COMPRESSED	= 0x4000000
		};

		class Texture {
		public:
			Texture(){}

			unsigned short	id;
			unsigned int	size;
			unsigned int	offset;
			unsigned int	props;
			unsigned char	x, y;
		

		};

		class TextureManager {
		public:
			unsigned short	LoadTexture(std::string filename);


			unsigned short	ProcessHeader(const std::ifstream &stream);

			static Texture			textures[MAX_TEXURE_LIMIT];
			static unsigned char	tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
			static unsigned char	tex_names_pool[MAX_TEXURE_LIMIT][32];
		};
	}
}

#endif