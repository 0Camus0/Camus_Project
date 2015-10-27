/*=============================================================================
XDataBaseLoader.h  	: .X File Parser v2
Revision history:
* Created by Camus
=============================================================================*/
#ifndef XDATABASE_H
#define XDATABASE_H

#define DEBUG_COUTS 0

#include "../Include/xDefs.h"

#include "../Include/xOGLESMesh.h"

using namespace xF;

#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

class XDataBase
{
public:
	XDataBase(){}
	~XDataBase(){
		for (unsigned int i = 0; i < XMeshDataBase.size(); i++){
			delete XMeshDataBase[i];
			
		}
		XMeshDataBase.clear();
	}
	bool	LoadXFile(const std::string	&FileName);
	bool	LoadXFromMemory(char* pData,const unsigned int &size);

	std::vector<xMesh*>	XMeshDataBase;

private:
	bool			Parse(std::string);
	unsigned short	GetTemplateType(std::string Line,std::string *retName=0);

	void		ProcessFrameBlock(std::string actual);
	void		ProcessMeshBlock(std::string actual);
	void		ProcessNormalsBlock(xF::xMeshGeometry *pGeometry);
	void		ProcessTexCoordinatesBlock(xF::xMeshGeometry *pGeometry);
	void		ProcessDeclDataBlock(xF::xMeshGeometry *pGeometry);
	void		ProcessMaterialBlock(xF::xMeshGeometry *pGeometry);
	void		ProcessSkinHeader(xF::xMeshGeometry* pGeometry);
	void		ProcessSkinWeights(xF::xMeshGeometry* pGeometry);

	void		ProcessTicksPerSecond(xF::xAnimationInfo* pAnimation);
	void		ProcessAnimationSet(xF::xAnimationInfo* pAnimation,const std::string name);

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

	std::stringstream		 m_ActualStream;
	std::stack<std::string>	 m_Stack;
	xMesh					*m_pActualMesh;


	// ...
	char					c_temp;

};


#endif