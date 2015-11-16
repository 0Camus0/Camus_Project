
#ifndef XDATABASE_H
#define XDATABASE_H

#define DEBUG_COUTS 0
#define DEBUG_GET_TEMPLATE 0

#define DEBUG_GET_BRACE 0
#define DEBUG_MATRICES 0
#define DEBUG_VERTICES 0
#define DEBUG_INDICES 0
#define DEBUG_NORMALS 0
#define DEBUG_TEXCOORDS 0
#define DEBUG_WEIGHTS 0
#define DEBUG_MATERIAL_INDICES 0
#define DEBUG_MATERIAL_COLORS 0
#define DEBUG_NUMBER_MATERIALS 0
#define DEBUG_ANIMATION 0
#define DEBUG_TICKS_PER_SECOND 0
#define DEBUG_ANIMATION_KEYS 0
#define DEBUG_END_BLOCKS 0

#define DEBUG_EFFECT_INSTANCE 0
#define DEBUG_EFFECT_DWORDS 0
#define DEBUG_EFFECT_FLOATS 0
#define DEBUG_EFFECT_STRING 0


#define PROFILE_MATRICES 0
#define PROFILE_BRACES 0
#define PROFILE_SKIN_WEIGHTS 0
#define PROFILE_FRAME_BONE 0
#define PROFILE_ANIMATION_SET 0
#define PROFILE_ANIMATION 0
#define PROFILE_ANIMATION_ROTATION 0
#define PROFILE_ANIMATION_SCALE 0
#define PROFILE_ANIMATION_POSITION 0
#define PROFILE_TICKS 0

#define USE_PREFETCH_ANIMATION_SETS_NUM 0
#define USE_VECTOR_RESERVE_AND_PUSH 0
#define USE_VECTOR_ARRAY_MODE 1

#define USE_STRING_STREAM 0


#include <Utils/xFile/xDefs.h>
#include <Utils/xFile/xMesh.h>


using namespace hyperspace;

#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

namespace xF {

	class XDataBase
	{
	public:
		XDataBase() {}
		~XDataBase() {
			for (unsigned int i = 0; i < XMeshDataBase.size(); i++) {
				delete XMeshDataBase[i];

			}
			XMeshDataBase.clear();
		}
		bool	LoadXFile(const std::string	&FileName);
		bool	LoadXFromMemory(char* pData, const unsigned int &size);

		std::vector<xMesh*>	XMeshDataBase;

	private:
		bool			Parse(std::string);
		unsigned int	GetxTemplateType(std::string Line, std::string *retName = 0);

		void		ProcessFrameBlock(std::string &actual);
		void		ProcessMeshBlock(std::string actual);
		void		ProcessNormalsBlock(xF::xMeshGeometry *pGeometry);
		void		ProcessTexCoordinatesBlock(xF::xMeshGeometry *pGeometry);
		void		ProcessDeclDataBlock(xF::xMeshGeometry *pGeometry);
		void		ProcessMaterialBlock(xF::xMeshGeometry *pGeometry);
		void		ProcessSkinHeader(xF::xMeshGeometry* pGeometry);
		void		ProcessSkinWeights(xF::xMeshGeometry* pGeometry);

		void		ProcessTicksPerSecond(xF::xAnimationInfo* pAnimation);
		void		ProcessAnimationSet(xF::xAnimationInfo* pAnimation, const std::string name);

		void		GetNextEndBracket();

		void		ProcessAnimation(xF::xAnimationSet* out);
		void		ProcessAnimationKey_Rotation(xF::xAnimationBone* out);
		void		ProcessAnimationKey_Scale(xF::xAnimationBone* out);
		void		ProcessAnimationKey_Position(xF::xAnimationBone* out);

		void		ProcessMatrix(XMATRIX44* out);
		void		ProcessMaterial(xF::xMaterial* out);
		void		ProcessEffectInstance(xF::xEffectInstance *out);
		void		ProcessEffectString(xF::xEffectDefault *out);
		void		ProcessEffectFloats(xF::xEffectDefault *out);
		void		ProcessEffectDwords(xF::xEffectDefault *out);

#if !USE_STRING_STREAM
		void			advance_to_next_open_brace();
		void			advance_to_next_close_brace();
		void			advance_to_next_space();
		unsigned int	GetxTemplateTypeChar(std::string &retName);


		void		PrintNextCharsAndPause();

		void			advance_to_next_close_brace_pre();
		unsigned int	prefetch_animation_num();
#endif

		std::stringstream		 m_ActualStream;
		std::stack<std::string>	 m_Stack;
		xMesh					*m_pActualMesh;
		char					 *pData;
		unsigned int			 index;

		// ...
		char					c_temp;

	};
}


#endif