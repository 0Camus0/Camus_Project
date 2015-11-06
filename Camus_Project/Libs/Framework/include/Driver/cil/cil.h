#ifndef CA_CIL_H
#define CA_CIL_H


#include <iostream>
#include <fstream>
#include <cstddef>
using namespace std;


#define CIL_PVRTC_2BPP_RGB_FMT		0
#define CIL_PVRTC_2BPP_RGBA_FMT		1
#define CIL_PVRTC_4BPP_RGB_FMT		2
#define CIL_PVRTC_4BPP_RGBA_FMT		3
#define CIL_PVRTCII_2BPP_RGB_FMT	4
#define CIL_PVRTCII_4BPP_RGB_FMT	5
#define CIL_ETC1_FMT				6
#define CIL_DXT1_FMT				7
#define CIL_DXT5_FMT				11
#define CIL_ETC2_FMT				23 //ETC2 RGBA not RGBA, for that we have ETC1

#define CIL_NOT_FOUND				0x0E2
#define CIL_CORRUPT					0x0E3
#define CIL_NO_MEMORY				0x0E4
#define CIL_PVR_V2_NOT_SUPPORTED	0x0E5
#define CIL_NOT_SUPPORTED_FILE		0x0E6
#define CIL_NO_ERROR				0

#define CIL_ALPHA		(1 << 0)
#define CIL_RGB			(1 << 1)
#define CIL_RGBA		(1 << 2)
#define CIL_INTEGER_8   (1 << 3)
#define CIL_UINTEGER_8  (1 << 4)
#define CIL_FLOAT_8		(1 << 5)
#define CIL_INTEGER_16  (1 << 6)
#define CIL_UINTEGER_16 (1 << 7)
#define CIL_INTEGER_32  (1 << 8)
#define CIL_UINTEGER_32 (1 << 9)
#define CIL_FLOAT_32    (1 << 10)
#define CIL_CUBE_MAP    (1 << 11)
#define CIL_COMPRESSED  (1 << 12)
#define CIL_BPP_2		(1 << 13)
#define CIL_BPP_4		(1 << 14)
#define CIL_BPP_8		(1 << 15)
#define CIL_BMP			(1 << 16)
#define CIL_PNG			(1 << 17)
#define CIL_DDS			(1 << 18)
#define CIL_TGA			(1 << 19)
#define CIL_PVR			(1 << 20)
#define CIL_KTX			(1 << 21)
#define CIL_RAW			(1 << 22)
#define CIL_ETC1		(1 << 23)
#define CIL_ETC2		(1 << 24)
#define CIL_DXT1		(1 << 25)
#define CIL_DXT5		(1 << 26)
#define CIL_PVRTC2		(1 << 27)
#define CIL_PVRTC4		(1 << 28)
#define CIL_PVRTCII2    (1 << 29)
#define CIL_PVRTCII4    (1 << 30)

struct pvr_v3_header {
	uint32_t		version;
	uint32_t		flags;
	uint32_t		pix_format_0;
	uint32_t		pix_format_1;
	uint32_t		color_space;
	uint32_t		channel_type;
	uint32_t		height;
	uint32_t		width;
	uint32_t		depth;
	uint32_t		surfaces;
	uint32_t		faces;
	uint32_t		mipmaps_c;
	uint32_t		metadata_size;
};




void checkformat(ifstream &in_,unsigned int &prop) {
	std::streampos begPos = in_.tellg();

	in_.seekg(begPos);
	char	dds[4];
	in_.read((char*)dds, 3);
	dds[3] = '\0';
	if (strcmp(dds, "DDS") == 0) {
		prop |= CIL_DDS;
		in_.seekg(begPos);
		return;
	}

	in_.seekg(begPos);
	unsigned char	ktx[5];
	in_.read((char*)ktx, 4);
	ktx[4] = '\0';
	if (strcmp((char*)ktx, "«KTX") == 0) {
		prop |= CIL_KTX;
		in_.seekg(begPos);
		return;
	}
	
	in_.seekg(begPos);
	char pvr[4];
	in_.read((char*)&pvr, 3);
	pvr[3] = '\0';
	if (strcmp((char*)pvr, "PVR") == 0) {
		prop |= CIL_PVR;
		in_.seekg(begPos);
		return;
	}

	prop = CIL_NOT_SUPPORTED_FILE;
}

unsigned char*	load_pvr(ifstream &in_, int *x, int *y, unsigned int &prop) {
	pvr_v3_header header;
	in_.seekg(0);
	in_.read((char*)&header, sizeof(pvr_v3_header));
	 
	if (header.version == 52) {
		prop = CIL_PVR_V2_NOT_SUPPORTED;
		return 0;
	}

	switch (header.pix_format_0){
		case  CIL_PVRTC_2BPP_RGB_FMT: {
			prop |= CIL_PVRTC2;
			prop |= CIL_RGB;
			prop |= CIL_BPP_2;
		}break;
		case  CIL_PVRTC_2BPP_RGBA_FMT: {
			prop |= CIL_PVRTC2;
			prop |= CIL_RGBA;
			prop |= CIL_BPP_2;
		}break;
		case  CIL_PVRTC_4BPP_RGB_FMT: {
			prop |= CIL_PVRTC4;
			prop |= CIL_RGB;
			prop |= CIL_BPP_4;
		}break;
		case  CIL_PVRTC_4BPP_RGBA_FMT: {
			prop |= CIL_PVRTCII2;
			prop |= CIL_RGBA;
			prop |= CIL_BPP_4;
		}break;
		case  CIL_PVRTCII_2BPP_RGB_FMT: {
			prop |= CIL_PVRTC2;
			prop |= CIL_RGB;
			prop |= CIL_BPP_2;
		}break;
		case  CIL_PVRTCII_4BPP_RGB_FMT: {
			prop |= CIL_PVRTCII4;
			prop |= CIL_RGBA;
			prop |= CIL_BPP_4;
		}break;
		case  CIL_ETC1_FMT: {
			prop |= CIL_ETC1;
			prop |= CIL_RGB;
			prop |= CIL_BPP_4;
		}break;
		case  CIL_DXT1_FMT: {
		
		}break;
		case  CIL_DXT5_FMT: {
		
		}break;
		case  CIL_ETC2_FMT: {
		
		}break;
	}


	cout << "version: " << header.version << endl
		<< "pixformat: " << header.pix_format_0 << endl
		<< "height: " << header.height << endl
		<< "width: " << header.width << endl
		<< "depth: " << header.depth << endl
		<< "surfaces: " << header.surfaces << endl
		<< "faces: " << header.faces << endl
		<< "mimaps: " << header.mipmaps_c << endl
		<< "size meta: " << header.metadata_size << endl;


	system("pause");
	return 0;
}


unsigned char*	cil_load(const char* filename,int *x, int *y, unsigned int *props) {
	*props = CIL_NOT_FOUND;
	ifstream in_(filename, ios::binary | ios::in);

	if (!in_.good()) {
		in_.close();
		*props = CIL_NOT_FOUND;
		return 0;
	}

	int x_ = 0, y_ = 0;
	unsigned int props_ = 0;
	checkformat(in_, props_);

	if (props_ == 0){
		return 0;
	}else if (props_&CIL_PVR) {
		unsigned char * buffer = load_pvr(in_, &x_, &y_, props_);
		*props = props_;
		*x = x_;
		*y = y_;
		return buffer;
	}

	return 0;
}

#endif