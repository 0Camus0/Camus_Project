#ifndef CA_TEXTURE_H
#define CA_TEXTURE_H

#include <fstream>
#include <string>

#include <config.h>

#define TEXTURE_NOT_LOADED	0xFFFF
#define TEXTURE_LOADED	0
#define TEXTURE_BAD_INDEX	(1<<31)

namespace hyperspace {
	namespace video {
		enum channelS_ {
			CH_ALPHA	 = (1 << 0),
			CH_RGB		 = (1 << 1),
			CH_RGBA		 = (1 << 2),
		};

		enum pixel_format_ {
			PFMT_SIGNED			= (1 << 3),		
			PFMT_UNSIGNED		= (1 << 4),		
			PFMT_NORMALIZED		= (1 << 5),		
			PFMT_UNNORMALIZED	= (1 << 6),		
			PFMT_BYTE			= (1 << 7),		
			PFMT_SHORT			= (1 << 8),		
			PFMT_INT			= (1 << 9),	
			PFMT_FLOAT			= (1 << 10)
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
	
		enum params_in {
			GENERATE_MIPMAPS				= (1 << 0),
			FILTER_NEAREST					= (1 << 1),
			FILTER_LINEAR					= (1 << 2),
			FILTER_NEAREST_MIPMAP_NEAREST	= (1 << 3),
			FILTER_NEAREST_MIPMAP_LINEAR	= (1 << 4),
			FILTER_LINEAR_MIPMAP_NEAREST	= (1 << 5),
			FILTER_LINEAR_MIPMAP_LINEAR		= (1 << 6),
			FILTER_ANISOTROPIC				= (1 << 7),
			WRAP_CLAMP						= (1 << 8),
			WRAP_REPEAT						= (1 << 9)
		};
		
		class Texture {
		public:
			Texture() :	
						size(0),						
						props(0),
						params(0),
						x(0),
						y(0),
						id(0),
						bounded(0),
						mipmaps(0)
						{
							
						}

			
			~Texture() {
			
			}

			virtual void	LoadAPITexture(unsigned char* buffer);
			virtual void	LoadAPITextureCompressed(unsigned char* buffer);

			virtual void	SetTextureParams(unsigned int &params, unsigned int &target);
			virtual void	GetFormatBpp(unsigned int &props, unsigned int &format, unsigned int &bpp);

			char			optname[128];
			unsigned int	size;
			unsigned int	props;
			unsigned int	params;
			unsigned short	x, y;
			unsigned short	id;
			unsigned char	bounded;
			unsigned char	mipmaps;
		};	
	}
}

#endif