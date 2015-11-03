#ifndef CA_TEXTURE_H
#define CA_TEXTURE_H


#include <fstream>
#include <string>

#define	TEXTURE_BUDGET_SIZE_MB		256
#define TEXTURE_BUDGET_SIZE_BYTES	1024*1024*TEXTURE_BUDGET_SIZE_MB

#define MAX_TEXURE_LIMIT	100

#define TEXTURE_NOT_FOUND	0xFFFF
#define TEXTURE_FOUND	0

namespace hyperspace {
	namespace video {

		enum channelS_ {
			CH_ALPHA	 = 0x1,
			CH_LUMINANCE = 0x2,
			CH_RGB		 = 0x4,
			CH_RGBA		 = 0x8,
			CH_DEPTH	 = 0x10
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
			COMPRESSED	= 0x2000000
		};

		class Texture {
		public:
			Texture() :	size(0),
						offset(0),
						props(0),
						x(0),
						y(0),
						id(0),
						bounded(0)
						{}

			
			unsigned int	size;
			unsigned int	offset;
			unsigned int	props;
			unsigned short	x, y;
			unsigned short	id;
			unsigned short	bounded;
		

		};

		class TextureManager {
		public:
			unsigned short	LoadTexture(std::string filename);

			unsigned int	CheckFormat(std::ifstream &in);
			

			unsigned int	num_textures_loaded;

			static Texture			textures[MAX_TEXURE_LIMIT];
			static unsigned char	tex_mem_pool[TEXTURE_BUDGET_SIZE_BYTES];
			static unsigned char	tex_names_pool[MAX_TEXURE_LIMIT][32];
		};
	}
}

#endif