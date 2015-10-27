/*=============================================================================
XFileParser.h  	: .X File Parser
Revision history:
* Created by Camus
=============================================================================*/
#ifndef XFPARSER_H
#define XFPARSER_H

#include "XDefs_legacy.h"
#include <fstream>
#include <vector>

using namespace stdEngine;

class Parser
{
public:
	Parser();
	stdEngine::XReturnType LoadFile(stdEngine::stdSTRING FileName);
	stdEngine::stdWORD		GetTemplateType(stdEngine::stdSTRING Line,stdEngine::stdSTRING *retName=0);
	void		ProcessBlocks();
	void		ProcessFrameBlock(LPSTDBONE pBone=0,stdEngine::stdSTRING bName = "");
	void		ProcessMesh(LPSTDBONE *pBone=0,stdEngine::stdSTRING bName = "");
	void		ProcessNormals(LPSTDMESHDATA *pMesh);
	void		ProcessTexCoords(LPSTDMESHDATA *pMesh);
	void		ProcessDeclData(LPSTDMESHDATA *pMesh);
	void		ProcessSkinHeader(LPSTDMESHDATA *pMesh);
	void		ProcessSkinWeights(LPSTDMESHDATA *pMesh);
	void		ProcessMaterialList(LPSTDMESHDATA *pMesh);
	void		ProcessMaterial(stdEngine::xMaterial *pMaterial);
	void		ProcessEffectInstance(stdEngine::xEffectInstance *pEffect);

	void		ProcessEffectString(stdEngine::xEffectDefault *pDefault);
	void		ProcessEffectFloats(stdEngine::xEffectDefault *pDefault);
	void		ProcessEffectDwords(stdEngine::xEffectDefault *pDefault);


	void		ProcessMatrix(STDMATRIX* out);
	void		GetDeclDataToMesh();
	

	void		SkipBlock();
	void		GetNextEndBracket();

	void		ShowDebugAllInfo();
	void		CheckRecursive(LPSTDBONE pBone);

	void		FreeAll();

// private:
	std::ifstream	In;
	std::ofstream   sample;

	char			 ctmp;
	int				itemp;

	std_uint		NumOutsideMaterials;
	stdEngine::xMaterial		*OutsideMaterials;

	LPSTDBONE					pBoneRoot;
	LPSTDMESHDATA				pMeshRoot;
	std::vector<LPSTDMESHDATA>	vMeshes;

};

#endif