#include "../Include/XFileParser.h"

#include "../../Framework/Include/GlobalHeader.h"

Parser::Parser()
{
	pBoneRoot = STDNULL;
	OutsideMaterials = STDNULL;

	NumOutsideMaterials = 0;
}

stdWORD Parser::GetTemplateType(stdSTRING Line,stdSTRING *retName)
{
	short ret = Line.find(XstringTemplates[STD_X_TEMPLATE]);
	if(ret!=-1){
		if(retName!=STDNULL){
			short dif = Line.find_last_of(" ") - Line.find(" ",ret);
			*retName  = Line.substr(ret+XstringTemplates[STD_X_TEMPLATE].size(),--dif);}
		return STD_X_TEMPLATE;}

 	for (int i = 0; i < MAXNUMTEMPLATES ; i++){
		ret = Line.find(XstringTemplates[i]);
		if(ret!=-1){
			if(retName!=STDNULL){
				short dif = Line.find_last_of(" ") - Line.find(" ",ret);
				*retName  = Line.substr(ret+XstringTemplates[i].size(),--dif);}
			return i;}}

	ret = Line.find("{");
	if((ret!=-1)&&(Line.find("}")!=-1)){
		if(retName!=STDNULL){
		short dif = Line.find_last_of(" ") - Line.find(" ",ret);
		*retName  = Line.substr(++(++ret),--dif);}
		return STD_X_REF;}

	return -1;
}

void Parser::ProcessEffectString(xEffectDefault *pDefault)
{
        pDefault->Type = STDX_STRINGS;

	In >> ctmp >> pDefault->NameParam;

	pDefault->NameParam = pDefault->NameParam.substr(0,pDefault->NameParam.size()-2);

//	std::cout << pDefault->NameParam<< std::endl;

	In >> ctmp >> pDefault->CaseString;

	pDefault->CaseString = pDefault->CaseString.substr(0,pDefault->CaseString.size()-2);

//	std::cout << pDefault->CaseString << std::endl;

	////system("pause");

	GetNextEndBracket();
}

void Parser::ProcessEffectFloats(xEffectDefault *pDefault)
{
        pDefault->Type = STDX_FLOATS;

	In >> ctmp >> pDefault->NameParam;

	pDefault->NameParam = pDefault->NameParam.substr(0,pDefault->NameParam.size()-2);

	//std::cout << "NameParam: " << pDefault->NameParam << std::endl;

	In >> pDefault->NumElements >> ctmp;

	//std::cout << "Num Elements: " << pDefault->NumElements << std::endl;

	if(pDefault->NumElements>0)
		pDefault->CaseFloat = new stdFLOAT[pDefault->NumElements];

	for (std_uint i = 0; i < pDefault->NumElements ; i++){
		In >> pDefault->CaseFloat[i] >> ctmp;
	//	std::cout << "Val[" << i << "] = " << pDefault->CaseFloat[i] << std::endl;
	}
	
//	//system("pause");

	GetNextEndBracket();
}

void Parser::ProcessEffectDwords(xEffectDefault *pDefault)
{
        pDefault->Type = STDX_DWORDS;

	In >> ctmp >> pDefault->NameParam;

	pDefault->NameParam = pDefault->NameParam.substr(0,pDefault->NameParam.size()-2);

	//std::cout << pDefault->NameParam<< std::endl;

	In >> pDefault->CaseDWORD >> ctmp;

//	std::cout << pDefault->CaseDWORD << std::endl;
	
	////system("pause");

	GetNextEndBracket();
}

void Parser::ProcessEffectInstance(xEffectInstance *pEffect)
{
	In >> ctmp >> pEffect->ShaderFileName;

	pEffect->ShaderFileName = pEffect->ShaderFileName.substr(0,pEffect->ShaderFileName.size()-2);

//	std::cout << pEffect->ShaderFileName << std::endl;

	std::streampos PosStream = In.tellg();

	pEffect->NumDefaults = 0;

	stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(In,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			std_uint T = GetTemplateType(Line,&rets);
			if(T == STD_X_EFFECT_PARAMS_DWORD || T == STD_X_EFFECT_PARAMS_FLOAT || T == STD_X_EFFECT_PARAMS_STRING ){
			pEffect->NumDefaults++;
			SkipBlock();
			}
		}
	}

	In.seekg(PosStream);
	Line = "";

	//std::cout << pEffect->NumDefaults << std::endl;
	////system("pause");

	if(pEffect->NumDefaults>0)
		pEffect->pDefaults = new xEffectDefault[pEffect->NumDefaults];

	pEffect->NumProcess = 0;

	while(Line.find("}")==-1){
		std::getline(In,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case STD_X_EFFECT_PARAMS_DWORD:{
		//		std::cout << "Found DWORD " << std::endl;	
				ProcessEffectDwords(&pEffect->pDefaults[pEffect->NumProcess++]);
									   }break;

			case STD_X_EFFECT_PARAMS_FLOAT:{
		//		std::cout << "Found FLOAT " << std::endl;	
				ProcessEffectFloats(&pEffect->pDefaults[pEffect->NumProcess++]);
									   }break;

			case STD_X_EFFECT_PARAMS_STRING:{
		//		std::cout << "Found STRING " << std::endl;	
				ProcessEffectString(&pEffect->pDefaults[pEffect->NumProcess++]);
									   }break;
			}
		}
	}

}

void Parser::ProcessMaterial(xMaterial *pMaterial)
{
	xMaterial *tmpMaterial = pMaterial;

 	In >> tmpMaterial->FaceColor.r >> ctmp >> tmpMaterial->FaceColor.g >> ctmp >> tmpMaterial->FaceColor.b >> ctmp >> tmpMaterial->FaceColor.a >> ctmp >> ctmp;

 //	std::cout << tmpMaterial->FaceColor.r << " " << tmpMaterial->FaceColor.g << " "  << tmpMaterial->FaceColor.b << " " << tmpMaterial->FaceColor.a << std::endl;

	In >> tmpMaterial->Power >> ctmp;

//	std::cout << tmpMaterial->Power << std::endl;

	In >> tmpMaterial->Specular.r >> ctmp >> tmpMaterial->Specular.g >> ctmp >> tmpMaterial->Specular.b >> ctmp >> ctmp;

//	std::cout << tmpMaterial->Specular.r << " " << tmpMaterial->Specular.g << " "  << tmpMaterial->Specular.b << " " << std::endl;

	In >> tmpMaterial->Emissive.r >> ctmp >> tmpMaterial->Emissive.g >> ctmp >> tmpMaterial->Emissive.b >> ctmp >> ctmp;

//	std::cout << tmpMaterial->Emissive.r << " " << tmpMaterial->Emissive.g << " "  << tmpMaterial->Emissive.b << " " << std::endl;

	stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(In,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case STD_X_EFFECT_INSTANCE:{
			//	std::cout << "Found Effect : " << rets << std::endl;	
				tmpMaterial->bEffects = true;
				ProcessEffectInstance(&tmpMaterial->EffectInstance);
			//	//system("pause");
								}break;

			case STD_X_TEXTURE:{
			//	std::cout << "MATFound Texture : " << rets << std::endl;	
				SkipBlock();
			//	//system("pause");
								 }break;
			}
		}
	}

	//GetNextEndBracket();

	//system("pause");
}

void Parser::ProcessMaterialList(LPSTDMESHDATA *pMesh)
{
	LPSTDMESHDATA tmpMesh = *pMesh;

	In >> tmpMesh->MaterialList.NumMaterials >> ctmp;

//	std::cout << tmpMesh->MaterialList.NumMaterials << std::endl;

	if(tmpMesh->MaterialList.NumMaterials > 0)
		tmpMesh->MaterialList.Materials = new xMaterial[tmpMesh->MaterialList.NumMaterials];

	In >> tmpMesh->MaterialList.NumFaceIndices >> ctmp;

//	std::cout << tmpMesh->MaterialList.NumFaceIndices << std::endl;

	if(tmpMesh->MaterialList.NumFaceIndices > 0)
		tmpMesh->MaterialList.FaceIndices = new stdDWORD[tmpMesh->MaterialList.NumFaceIndices];

	for (std_uint i = 0; i < tmpMesh->MaterialList.NumFaceIndices; i++){
		In >> tmpMesh->MaterialList.FaceIndices[i] >> ctmp;
	//	std::cout << tmpMesh->MaterialList.FaceIndices[i] << std::endl;
	}

	stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(In,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case STD_X_MATERIAL:{
				std::cout << "Found Material : " << rets << std::endl;	
				tmpMesh->MaterialList.Materials[tmpMesh->MaterialList.NumMatProcess].Name = rets;
				ProcessMaterial(&tmpMesh->MaterialList.Materials[tmpMesh->MaterialList.NumMatProcess++]);
				//system("pause");
									}break;

			case STD_X_REF:{
				std::cout << "Found Reference : " << rets << std::endl;
				Line = "";
				//system("pause");
							   }break;

			}
		}
	}

	//system("pause");

}

void Parser::ProcessSkinWeights(LPSTDMESHDATA *pMesh)
{
	std::string tmpStr;
	
	In >> ctmp >> tmpStr;

	tmpStr = tmpStr.substr(0,tmpStr.size()-2);

	(*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NodeName = tmpStr;

//	std::cout << (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NodeName << std::endl;

	In >> (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NumWeights >> ctmp;

//	std::cout << "Num Weights: " << (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NumWeights << std::endl;

//	//system("pause");

	if((*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NumWeights>0){
		(*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].Weights	= new stdFLOAT[(*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NumWeights];
		(*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].VertexIndices = new stdDWORD[(*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NumWeights];
	}

	for (std_uint i = 0; i < (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NumWeights; i++){
		In >> (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].VertexIndices[i] >> ctmp;
	//	std::cout << (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].VertexIndices[i] << std::endl;
	}

	for (std_uint i = 0; i < (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].NumWeights; i++){
		In >> (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].Weights[i] >> ctmp;
	//	std::cout << (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].Weights[i] << std::endl;
	}

	for (std_uint i = 0; i < 16 ; i++){
		In >> (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].MatrixOffset.mat[i] >> ctmp;
	//	std::cout << (*pMesh)->SkinInfo.SkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess].MatrixOffset.mat[i] << " ";
	}

	In >> ctmp;

//	//system("pause");

	GetNextEndBracket();

	(*pMesh)->SkinInfo.SkinMeshHeader.NumBonesProcess++;
}

void Parser::ProcessSkinHeader(LPSTDMESHDATA *pMesh)
{
	In >> (*pMesh)->SkinInfo.SkinMeshHeader.MaxNumWeightPerVertex >> ctmp;
	In >> (*pMesh)->SkinInfo.SkinMeshHeader.MaxNumWeightPerFace >> ctmp;
	In >> (*pMesh)->SkinInfo.SkinMeshHeader.NumBones >> ctmp;

	if((*pMesh)->SkinInfo.SkinMeshHeader.NumBones > 0)
		(*pMesh)->SkinInfo.SkinWeights = new xSkinWeights[(*pMesh)->SkinInfo.SkinMeshHeader.NumBones];

// 	std::cout << (*pMesh)->SkinInfo.SkinMeshHeader.MaxNumWeightPerVertex << std::endl;
// 	std::cout << (*pMesh)->SkinInfo.SkinMeshHeader.MaxNumWeightPerFace << std::endl;
// 	std::cout << (*pMesh)->SkinInfo.SkinMeshHeader.NumBones << std::endl;
 
// 	//system("pause");

	GetNextEndBracket();
}

void Parser::ProcessDeclData(LPSTDMESHDATA *pMesh)
{
	In >>  (*pMesh)->DeclData.NumElements >> ctmp;

//	std::cout << "Num Vertex Elements: " << (*pMesh)->DeclData.NumElements << std::endl;

//	system("pause");

	if((*pMesh)->DeclData.NumElements>0)
		(*pMesh)->DeclData.Elements = new xVertexElement[(*pMesh)->DeclData.NumElements];

	for (std_uint i = 0; i < (*pMesh)->DeclData.NumElements; i++){
		
		In >> (*pMesh)->DeclData.Elements[i].Type   >> ctmp 
		   >> (*pMesh)->DeclData.Elements[i].Method >> ctmp 
		   >> (*pMesh)->DeclData.Elements[i].Usage  >> ctmp 
		   >> (*pMesh)->DeclData.Elements[i].UsageIndex >> ctmp >> ctmp;

   		std::cout << (*pMesh)->DeclData.Elements[i].Type   << " " << 
   					 (*pMesh)->DeclData.Elements[i].Method << " " << 
   					 (*pMesh)->DeclData.Elements[i].Usage  << " " << 
   					 (*pMesh)->DeclData.Elements[i].UsageIndex << " " << std::endl;
 
 	//	system("pause");
	}

	In >>  (*pMesh)->DeclData.NumValues >> ctmp;

 	std::cout << "Num Values: " << (*pMesh)->DeclData.NumValues << std::endl;
// 
	//system("pause");

	if((*pMesh)->DeclData.NumValues>0)
		(*pMesh)->DeclData.Values = new stdDWORD[(*pMesh)->DeclData.NumValues];

	

	for (std_uint i = 0; i < (*pMesh)->DeclData.NumValues; i++){
		In >> (*pMesh)->DeclData.Values[i] >> ctmp;
		/*stdDWORD val = *(&(*pMesh)->DeclData.Values[i]);
		union 		{
			stdDWORD i;
			float f;
		}u;
		u.i = val; */
		//std::cout << *(float*)(&(*pMesh)->DeclData.Values[i]) << std::endl;
		//std::cout << u.f << std::endl;
		//system("pause");
	}

//	system("pause");

	GetNextEndBracket();
}

void Parser::ProcessTexCoords(LPSTDMESHDATA *pMesh)
{
	In >> (*pMesh)->TexCoords.NumTexcoords >> ctmp;

//	std::cout << "Num TexCoords: " << (*pMesh)->TexCoords.NumTexcoords << std::endl;

	//LOGD("ANTES NEW STDVEC2");
	if((*pMesh)->TexCoords.NumTexcoords > 0)
		(*pMesh)->TexCoords.TexCoordinates = new STDVECTOR2[(*pMesh)->TexCoords.NumTexcoords];

	//	//system("pause");

	//LOGD("ASIGNANDO");
	for (std_uint i = 0; i < (*pMesh)->TexCoords.NumTexcoords ; i++){
		In >> (*pMesh)->TexCoords.TexCoordinates[i].x >> ctmp >> (*pMesh)->TexCoords.TexCoordinates[i].y >> ctmp >> ctmp;
	//	std::cout << (*pMesh)->TexCoords.TexCoordinates[i].x << "  " << (*pMesh)->TexCoords.TexCoordinates[i].y << std::endl;
	}

//	//system("pause");

	GetNextEndBracket();
}

void Parser::ProcessNormals(LPSTDMESHDATA *pMesh)
{
	In >> (*pMesh)->Normals.NumVertices >> ctmp;

//	std::cout << "Num Normalss: " << (*pMesh)->Normals.NumVertices << std::endl;

	if((*pMesh)->Normals.NumVertices > 0)
		(*pMesh)->Normals.Vertices = new STDVECTOR3[(*pMesh)->Normals.NumVertices];

//	//system("pause");

	for (std_uint i = 0; i < (*pMesh)->Normals.NumVertices ; i++){
		In >> (*pMesh)->Normals.Vertices[i].x >> ctmp >> (*pMesh)->Normals.Vertices[i].y >> ctmp >> (*pMesh)->Normals.Vertices[i].z >> ctmp >> ctmp;
	//	std::cout << (*pMesh)->Normals.Vertices[i].x << "  " << (*pMesh)->Normals.Vertices[i].y << "  " << (*pMesh)->Normals.Vertices[i].z << std::endl;
 	}

	In >> (*pMesh)->Normals.NumFaces >> ctmp;

//	std::cout << "num faces: " << (*pMesh)->Normals.NumFaces << std::endl;

	if((*pMesh)->Normals.NumFaces > 0)
		(*pMesh)->Normals.Faces = new xMeshFace[(*pMesh)->Normals.NumFaces];

//	//system("pause");

	for (std_uint i = 0; i < (*pMesh)->Normals.NumFaces ; i++){
		In >> (*pMesh)->Normals.Faces[i].NumVertexIndices >> ctmp;

	//	std::cout << "num indexes per face: " << (*pMesh)->Normals.Faces[i].NumVertexIndices << " : ";

		if((*pMesh)->Normals.Faces[i].NumVertexIndices>0)
			(*pMesh)->Normals.Faces[i].Indices = new stdWORD[(*pMesh)->Normals.Faces[i].NumVertexIndices];

		for (std_uint j = 0; j < (*pMesh)->Normals.Faces[i].NumVertexIndices ; j++){
			In >> (*pMesh)->Normals.Faces[i].Indices[j] >> ctmp;
		//	std::cout << (*pMesh)->Normals.Faces[i].Indices[j] << " ";
		}

	//	std::cout << std::endl;
	
		In >> ctmp;
	}

//	//system("pause");

	GetNextEndBracket();

}

void Parser::ProcessMesh(LPSTDBONE *pBone,stdSTRING bName)
{
	LPSTDMESHDATA ptmpMesh = new STDMESHDATA;
	ptmpMesh->Name = bName;
	ptmpMesh->pNextMesh = STDNULL;

	ptmpMesh->Relative = (*pBone)->TransformationMatrix;
	vMeshes.push_back(ptmpMesh);

	In >> ptmpMesh->Vertex.NumVertices >> ctmp;

//	std::cout << "num vertices: " << ptmpMesh->Vertex.NumVertices << std::endl;

	if(ptmpMesh->Vertex.NumVertices > 0)
		ptmpMesh->Vertex.Vertices = new STDVECTOR3[ptmpMesh->Vertex.NumVertices];

	////system("pause");

	for (std_uint i = 0; i < ptmpMesh->Vertex.NumVertices ; i++){
		In >> ptmpMesh->Vertex.Vertices[i].x >> ctmp >> ptmpMesh->Vertex.Vertices[i].y >> ctmp >> ptmpMesh->Vertex.Vertices[i].z >> ctmp >> ctmp;
	//	std::cout << ptmpMesh->Vertex.Vertices[i].x << "  " << ptmpMesh->Vertex.Vertices[i].y << "  " << ptmpMesh->Vertex.Vertices[i].z << std::endl;
 	}

	In >> ptmpMesh->Vertex.NumFaces >> ctmp;

//	std::cout << "num faces: " << ptmpMesh->Vertex.NumFaces << std::endl;

	if(ptmpMesh->Vertex.NumFaces > 0)
		ptmpMesh->Vertex.Faces = new xMeshFace[ptmpMesh->Vertex.NumFaces];

	////system("pause");

	for (std_uint i = 0; i < ptmpMesh->Vertex.NumFaces ; i++){
		In >> ptmpMesh->Vertex.Faces[i].NumVertexIndices >> ctmp;

	//	std::cout << "num indexes per face: " << ptmpMesh->Vertex.Faces[i].NumVertexIndices << " : ";

		if(ptmpMesh->Vertex.Faces[i].NumVertexIndices>0)
			ptmpMesh->Vertex.Faces[i].Indices = new stdWORD[ptmpMesh->Vertex.Faces[i].NumVertexIndices];

		for (std_uint j = 0; j < ptmpMesh->Vertex.Faces[i].NumVertexIndices ; j++){
			In >> ptmpMesh->Vertex.Faces[i].Indices[j] >> ctmp;
			//std::cout << ptmpMesh->Vertex.Faces[i].Indices[j] << " ";
		}

	//	std::cout << std::endl;
	
		In >> ctmp;
	}


 	stdSTRING Line;
 	while(Line.find("}")==-1){
 		std::getline(In,Line);
  		if(Line.find("{")!=-1){
  			stdSTRING rets;
  			switch(GetTemplateType(Line,&rets))
  			{
				case STD_X_MESH_NORMALS:{
				//	LOGD("ENCONTRO NORMALS");
				//	std::cout << "Found Mesh Normals: " << std::endl;	
					ProcessNormals(&ptmpMesh);
				//	//system("pause");
										}break;

				case STD_X_MESH_TEXCOORD:{
				//	LOGD("TEXCOORDS");
				//	std::cout << "Found Mesh Texture Coordinates: " << std::endl;	
					ProcessTexCoords(&ptmpMesh);
				//	//system("pause");
										}break;

				case STD_X_DECLDATA:{
				//	LOGD("TANGENTS/BINORMALS");
				//	std::cout << "Found Mesh decl data: " << std::endl;	
					ProcessDeclData(&ptmpMesh);
				//	//system("pause");
										}break;

				case STD_X_SKIN_HEADER:{
				//	LOGD("SKIN");
				//	std::cout << "Found Mesh skin header: " << std::endl;	
					ProcessSkinHeader(&ptmpMesh);
				//	//system("pause");
										}break;

				case STD_X_SKIN_WEIGHTS:{
				//	std::cout << "Found Mesh skin weight: " << rets << std::endl;
					ProcessSkinWeights(&ptmpMesh);
				//	//system("pause");
									   }break;

				case STD_X_MATERIALS_LIST:{
				//	LOGD("MATERIAL");
				//	std::cout << "Found Mesh material list: " << rets << std::endl;
					ProcessMaterialList(&ptmpMesh);
				//	//system("pause");
										}break;
  			}
  		}
 	}
	
	ptmpMesh = STDNULL;
}

void Parser::ProcessFrameBlock(LPSTDBONE pBone,stdSTRING bName)
{
	LPSTDBONE ptmpBone = STDNULL;
		
 	if(pBone!=STDNULL && pBone->pBoneParent==STDNULL){
		LPSTDBONE ptmpRoot = pBoneRoot;
 		ptmpBone = new STDBONE;
 		ptmpBone->Name = bName;
 		ptmpBone->pBoneParent = STDNULL;
 		ptmpBone->pBoneFirstChild = STDNULL;
 		ptmpBone->pBoneNextBrother = STDNULL;
	
 		while(ptmpRoot->pBoneNextBrother!=STDNULL)
			ptmpRoot = ptmpRoot->pBoneNextBrother;

 		ptmpRoot->pBoneNextBrother = ptmpBone;
 		pBone = ptmpRoot->pBoneNextBrother;
 		ptmpBone = STDNULL;
		ptmpRoot = STDNULL;
 	}
 
 	if(pBoneRoot==STDNULL){
 		ptmpBone = new STDBONE;
 		ptmpBone->Name = bName;
 		ptmpBone->pBoneParent = STDNULL;
 		ptmpBone->pBoneFirstChild = STDNULL;
 		ptmpBone->pBoneNextBrother = STDNULL;
 		pBoneRoot = ptmpBone;
 		pBone = pBoneRoot;
 		ptmpBone = STDNULL;
 	}
	
	stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(In,Line);
 		if(Line.find("{")!=-1){
 			stdSTRING rets;
  			switch(GetTemplateType(Line,&rets))
  			{
  			case STD_X_FRAME:{
					std::cout << "Found Frame : " << rets << std::endl;	
				//LOGD("ENCONTRO FRAME");
				if(bName.compare(pBone->Name)==0){
 					ptmpBone = new STDBONE;
 					ptmpBone->Name = rets;
 					ptmpBone->pBoneParent = pBone;
 					ptmpBone->pBoneFirstChild = STDNULL;
 					ptmpBone->pBoneNextBrother = STDNULL;
 					pBone->pBoneFirstChild = ptmpBone;	
 					pBone = pBone->pBoneFirstChild;
 					ptmpBone = STDNULL;
				//	std::cout << "CREATE SON: " << rets <<std::endl;	
				}else{
					ptmpBone = new STDBONE;
					ptmpBone->Name = rets;
					ptmpBone->pBoneParent = pBone->pBoneParent;
					ptmpBone->pBoneFirstChild = STDNULL;
					ptmpBone->pBoneNextBrother = STDNULL;
					pBone->pBoneNextBrother = ptmpBone;
					pBone = pBone->pBoneNextBrother;
					ptmpBone = STDNULL;
				//	std::cout << "CREATE BROTHER: " << rets <<std::endl;
				}						
					ProcessFrameBlock(pBone,rets);

							 }break;
			case STD_X_MESH:{
			//	LOGD("ENCONTRO MESH");
			//		std::cout << "Found Mesh name: " << rets <<std::endl;	
					ProcessMesh(&pBone,rets);
							}break;

			case STD_X_FRAME_TRANSFORM_MTX:{
					std::cout << "Found TransformMatrix: " << std::endl;	
					ProcessMatrix(&pBone->TransformationMatrix);
				//	//system("pause");
							}break;

			case STD_X_OBJ_CMMTX:{
			//		std::cout << "Found ObjectMatrix: " << std::endl;	
					ProcessMatrix(&pBone->ObjectCustomMatrix);
				//	//system("pause");
								}break;
 
  			}
 		}
	}
}

void Parser::GetDeclDataToMesh()
{
	for (int l = 0; l < vMeshes.size() ; l++)
	{
		std::string str = "NUM ELEMENTS " + double2string(vMeshes[l]->DeclData.NumElements);
		//LOGD(str.c_str());
		str = "NUM VALUES " + double2string(vMeshes[l]->DeclData.NumValues);
	//	LOGD(str.c_str());

		if(vMeshes[l]->DeclData.NumElements<=0){
		//	LOGD("SKIP: NO TANGENTS on MESH &d",l);
			continue;
		}

		std_uint order[3];
		std_uint cont = 0;
		for (std_uint i = 0; i < vMeshes[l]->DeclData.NumElements; i++){

			switch(vMeshes[l]->DeclData.Elements[i].Usage)
			{
			case 5:{ // texcoord
				order[0] = cont;
				cont++;
				   }break;

			case 6:{ // tangent
				vMeshes[l]->Tangent.NumVertices = vMeshes[l]->Vertex.NumVertices;
				vMeshes[l]->Tangent.Vertices = new STDVECTOR3[vMeshes[l]->Vertex.NumVertices];
				order[1] = cont;
				cont++;
				   }break;

			case 7:{ // binormal
				vMeshes[l]->BiNormal.NumVertices = vMeshes[l]->Vertex.NumVertices;
				vMeshes[l]->BiNormal.Vertices = new STDVECTOR3[vMeshes[l]->Vertex.NumVertices];
				order[2] = cont;
				cont++;
				   }break;
			}
		}

		std_uint advance = 0 ;
		advance+=3;
		for(std_uint i = 0; i < vMeshes[l]->Vertex.NumVertices; i++){
			vMeshes[l]->Tangent.Vertices[i].x = *(float*)(&vMeshes[l]->DeclData.Values[(advance++)]);
			vMeshes[l]->Tangent.Vertices[i].y = *(float*)(&vMeshes[l]->DeclData.Values[(advance++)]);
			vMeshes[l]->Tangent.Vertices[i].z = *(float*)(&vMeshes[l]->DeclData.Values[(advance++)]);
			advance+=3;
		}


		advance = 0;
		for(std_uint i = 0; i < vMeshes[l]->Vertex.NumVertices; i++){
			vMeshes[l]->BiNormal.Vertices[i].x = *(float*)(&vMeshes[l]->DeclData.Values[(advance++)]);
			vMeshes[l]->BiNormal.Vertices[i].y = *(float*)(&vMeshes[l]->DeclData.Values[(advance++)]);
			vMeshes[l]->BiNormal.Vertices[i].z = *(float*)(&vMeshes[l]->DeclData.Values[(advance++)]);
			advance+=3;
		}
		
	}

	/*LPSTDMESHDATA ptmpMeshRoot = pMeshRoot;

	while(ptmpMeshRoot!=STDNULL){

	std::cout << "num val: " << ptmpMeshRoot->DeclData.NumElements << std::endl;
	
	if(ptmpMeshRoot->DeclData.NumValues<=0)
		return;

	std_uint order[3];
	std_uint cont = 0;
	for (std_uint i = 0; i < ptmpMeshRoot->DeclData.NumElements; i++){

		    switch(ptmpMeshRoot->DeclData.Elements[i].Usage)
			{
			case 5:{ // texcoord
					order[0] = cont;
					cont++;
				   }break;

			case 6:{ // tangent
					ptmpMeshRoot->Tangent.NumVertices = ptmpMeshRoot->Vertex.NumVertices;
					ptmpMeshRoot->Tangent.Vertices = new STDVECTOR3[ptmpMeshRoot->Vertex.NumVertices];
					order[1] = cont;
					cont++;
				   }break;

			case 7:{ // binormal
					ptmpMeshRoot->BiNormal.NumVertices = ptmpMeshRoot->Vertex.NumVertices;
					ptmpMeshRoot->BiNormal.Vertices = new STDVECTOR3[ptmpMeshRoot->Vertex.NumVertices];
					order[2] = cont;
					cont++;
				   }break;
			}
	}

	std_uint advance = 0 ;
	advance+=3;
	for(std_uint i = 0; i < ptmpMeshRoot->Vertex.NumVertices; i++){
		ptmpMeshRoot->Tangent.Vertices[i].x = *(float*)(&ptmpMeshRoot->DeclData.Values[(advance++)]);
		ptmpMeshRoot->Tangent.Vertices[i].y = *(float*)(&ptmpMeshRoot->DeclData.Values[(advance++)]);
		ptmpMeshRoot->Tangent.Vertices[i].z = *(float*)(&ptmpMeshRoot->DeclData.Values[(advance++)]);
		advance+=3;
		if(i==0){
			std::cout << "Tan: " << ptmpMeshRoot->Tangent.Vertices[i].x <<  " " << ptmpMeshRoot->Tangent.Vertices[i].y << " " << ptmpMeshRoot->Tangent.Vertices[i].z << std::endl;
		//	system("pause");
		}
	}
	

	advance = 0;
	for(std_uint i = 0; i < ptmpMeshRoot->Vertex.NumVertices; i++){
		ptmpMeshRoot->BiNormal.Vertices[i].x = *(float*)(&ptmpMeshRoot->DeclData.Values[(advance++)]);
		ptmpMeshRoot->BiNormal.Vertices[i].y = *(float*)(&ptmpMeshRoot->DeclData.Values[(advance++)]);
		ptmpMeshRoot->BiNormal.Vertices[i].z = *(float*)(&ptmpMeshRoot->DeclData.Values[(advance++)]);
		advance+=3;
		if(i==0){
			std::cout << "Bin: " << ptmpMeshRoot->BiNormal.Vertices[i].x <<  " " << ptmpMeshRoot->BiNormal.Vertices[i].y << " " << ptmpMeshRoot->BiNormal.Vertices[i].z << std::endl;
		//	system("pause");
		}
	}



	if(ptmpMeshRoot->pNextMesh)
		ptmpMeshRoot = ptmpMeshRoot->pNextMesh;
	}*/
}

void Parser::ProcessBlocks()
{
  

	stdSTRING Line;
	std::getline(In,Line);

	while(!In.eof()){
		std::getline(In,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case STD_X_FRAME:{
			//	LOGD("ENCONTRO FRAME");
                                std::cout << "Found Frame : " << rets << std::endl;
				ProcessFrameBlock(pBoneRoot,rets);
							 }break;

			case STD_X_MESH:{

							}break;

			case STD_X_MATERIAL:{
				std::cout << "Found Outside Material : " << rets << std::endl;

// 				if(NumOutsideMaterials==0){
// 					OutsideMaterials = new xMaterial[1];
// 				}else{
// 
// 					xMaterial *tmpMat = new xMaterial[NumOutsideMaterials];
// 
// 			//		copy()
// 					for (std_uint i = 0; i < NumOutsideMaterials ; i++)
// 						 tmpMat[i] = OutsideMaterials[i];
// 
// 					delete [] OutsideMaterials;
// 
// 					OutsideMaterials = new xMaterial[NumOutsideMaterials+1];
// 
// 					for (std_uint i = 0; i < NumOutsideMaterials ; i++)
// 						*OutsideMaterials[i] = *tmpMat[i];
// 
// 					delete [] tmpMat;
// 					tmpMat = STDNULL;
// 				}
// 
// 				
// 
// 				ProcessMaterial(&OutsideMaterials[NumOutsideMaterials++]);

				
							}break;

			case STD_X_ANIMATIONSET:{

									}break;
			}
		} 
	}

	std::cout << "\n\nDebug Time\n\n";

//	LOGD("GetDeclDataToMesh");
	GetDeclDataToMesh();


//	for (std_uint i = 0; i < NumOutsideMaterials ; i++)
//		std::cout << OutsideMaterials[0].Name << std::endl;
	
	
  //     ShowDebugAllInfo();
	
}

void Parser::ProcessMatrix(STDMATRIX* out)
{
	char tmp;
	stdSTRING Line;

	for (int i = 0; i < 16 ; i++){
		In >> out->mat[i] >> tmp;
		//std::cout << out->mat[i] << " " << std::endl;
	}
	//system("pause");

	std::getline(In,Line);
	std::getline(In,Line);
}

void Parser::SkipBlock()
{
	stdSTRING Line;
	std::getline(In,Line);

	while(Line.find("}")==-1){
		std::getline(In,Line);		
		//std::cout << Line << std::endl;
	}
}

void Parser::GetNextEndBracket()
{
	stdSTRING Line;
	std::getline(In,Line);
	while(Line.find("}")==-1){
		std::getline(In,Line);		
	}
}

void Parser::ShowDebugAllInfo()
{
	//	std::cout << std::endl << "BONE NAME:  " << pBoneRoot->AsociatedMesh->Name << " Num Verts: " << pBoneRoot->AsociatedMesh->Vertex.NumVertices << std::endl << std::endl;

	// 	LPSTDBONE ptmpRoot = pBoneRoot;
	// 
	// 	while(ptmpRoot!=STDNULL){
	// 		std::cout << " BONE NAME:  " << ptmpRoot->Name << std::endl << std::endl;
	// 
	// 		ptmpRoot = ptmpRoot->pBoneNextBrother;
	// 	}
	// 
	// 
	
/*
 	LPSTDMESHDATA ptmpMeshRoot = pMeshRoot;
 
 	while(ptmpMeshRoot!=STDNULL){
 		std::cout << " MESH NAME:  " << ptmpMeshRoot->Name << std::endl << std::endl;
 
 		//system("pause");
 
 		std::cout << " NUM VERTS:  " << ptmpMeshRoot->Vertex.NumVertices << std::endl << std::endl;
 
 		//system("pause");
 
 		for (std_uint i = 0; i < ptmpMeshRoot->Vertex.NumVertices ; i++)
 			std::cout << ptmpMeshRoot->Vertex.Vertices[i].x << "  " << ptmpMeshRoot->Vertex.Vertices[i].y << "  " << ptmpMeshRoot->Vertex.Vertices[i].z << std::endl;
 
 		std::cout << " NUM NORMALS:  " << ptmpMeshRoot->Normals.NumVertices << std::endl << std::endl;
 
 		//system("pause");
 
 		for (std_uint i = 0; i < ptmpMeshRoot->Normals.NumVertices ; i++)
 			std::cout << ptmpMeshRoot->Normals.Vertices[i].x << "  " << ptmpMeshRoot->Normals.Vertices[i].y << "  " << ptmpMeshRoot->Normals.Vertices[i].z << std::endl;
 
 		std::cout << " NUM TEXCOORDS:  " << ptmpMeshRoot->TexCoords.NumTexcoords << std::endl << std::endl;
 
 		//system("pause");
 
 		for (std_uint i = 0; i < ptmpMeshRoot->TexCoords.NumTexcoords ; i++)
 			std::cout << ptmpMeshRoot->TexCoords.TexCoordinates[i].x << "  " << ptmpMeshRoot->TexCoords.TexCoordinates[i].y << std::endl;
 
 		std::cout << "NUM VALUES: " << ptmpMeshRoot->DeclData.NumValues << std::endl;
 
 		//system("pause");
 
 		for (std_uint i = 0; i < ptmpMeshRoot->DeclData.NumValues; i++)
 			std::cout << ptmpMeshRoot->DeclData.Values[i] << std::endl;
 
 		std::cout << "MESH SKIN HEADER: " << std::endl;
 
 		std::cout << "MaxNumWeightPerVertex: " << ptmpMeshRoot->SkinInfo.SkinMeshHeader.MaxNumWeightPerVertex << std::endl;
 	 	std::cout << "MaxNumWeightPerFace: " << ptmpMeshRoot->SkinInfo.SkinMeshHeader.MaxNumWeightPerFace << std::endl;
 	 	std::cout << "NumBones: " << ptmpMeshRoot->SkinInfo.SkinMeshHeader.NumBones << std::endl;
 
 		//system("pause");
 
 		for (std_uint k = 0; k < ptmpMeshRoot->SkinInfo.SkinMeshHeader.NumBones; k++){
 
 			std::cout << "Bone: " << ptmpMeshRoot->SkinInfo.SkinWeights[k].NodeName << std::endl;
 
 			std::cout << "Num Weights: " << ptmpMeshRoot->SkinInfo.SkinWeights[k].NumWeights << std::endl;
 
 			for (std_uint i = 0; i < ptmpMeshRoot->SkinInfo.SkinWeights[k].NumWeights; i++)
 				std::cout << ptmpMeshRoot->SkinInfo.SkinWeights[k].VertexIndices[i] << std::endl;
 
 			for (std_uint i = 0; i < ptmpMeshRoot->SkinInfo.SkinWeights[k].NumWeights; i++)
 				std::cout << ptmpMeshRoot->SkinInfo.SkinWeights[k].Weights[i] << std::endl;
 
 			for (std_uint i = 0; i < 16 ; i++)
 				std::cout << ptmpMeshRoot->SkinInfo.SkinWeights[k].MatrixOffset.mat[i] << " ";
 
 			//system("pause");
 		}
 
 		//system("pause");
 
 		std::cout << "MATERIALS: " << std::endl;		
 
 		std::cout << "Num Dif Materials: " << ptmpMeshRoot->MaterialList.NumMaterials << std::endl;
 
 		std::cout << "Faces: " << std::endl;	
 
 		//system("pause");
 
 		for (std_uint i = 0; i < ptmpMeshRoot->MaterialList.NumFaceIndices; i++)
 			std::cout << ptmpMeshRoot->MaterialList.FaceIndices[i] << std::endl;
 
 		std::cout << "Materials: " << std::endl;
 
 
 		for (std_uint j = 0; j < ptmpMeshRoot->MaterialList.NumMaterials; j++){
 
 			std::cout << ptmpMeshRoot->MaterialList.Materials[j].FaceColor.r << " " << ptmpMeshRoot->MaterialList.Materials[j].FaceColor.g << " "  << ptmpMeshRoot->MaterialList.Materials[j].FaceColor.b << " " << ptmpMeshRoot->MaterialList.Materials[j].FaceColor.a << std::endl;
 			std::cout << ptmpMeshRoot->MaterialList.Materials[j].Power << std::endl;
 			std::cout << ptmpMeshRoot->MaterialList.Materials[j].Specular.r << " " << ptmpMeshRoot->MaterialList.Materials[j].Specular.g << " "  << ptmpMeshRoot->MaterialList.Materials[j].Specular.b << " " << std::endl;
 			std::cout << ptmpMeshRoot->MaterialList.Materials[j].Emissive.r << " " << ptmpMeshRoot->MaterialList.Materials[j].Emissive.g << " "  << ptmpMeshRoot->MaterialList.Materials[j].Emissive.b << " " << std::endl;
 
 			//system("pause");
 
 
 			if(ptmpMeshRoot->MaterialList.Materials[j].bEffects){
 
 				std::cout << "Effectx: " << std::endl;
 				for (std_uint i = 0; i < ptmpMeshRoot->MaterialList.Materials[j].EffectInstance.NumDefaults; i++)
 					std::cout << ptmpMeshRoot->MaterialList.Materials[j].EffectInstance.pDefaults[i].NameParam << std::endl;
 			}
 
 			//system("pause");
 		}
 
 		ptmpMeshRoot = ptmpMeshRoot->pNextMesh;
 		//system("pause");
 	}

	//	CheckRecursive(pBoneRoot);
	*/
}

void Parser::CheckRecursive(LPSTDBONE pBone)
{
//	std::cout << " BONE NAME:  " << pBone->Name << std::endl;

	std::cout << "RecBONE NAME:  " << pBone->Name << std::endl;

	for (int i = 0; i < 16 ; i++)
		std::cout << pBone->TransformationMatrix.mat[i] << " , " ;

	std::cout << std::endl;

	//system("pause");


	if( pBone->pBoneFirstChild != STDNULL )
	{
		std::cout << " TO NEXT CHILD " << std::endl;
		CheckRecursive( pBone->pBoneFirstChild );
	}

	if( pBone->pBoneNextBrother != STDNULL )
	{
		std::cout << " TO NEXT SIBLING " << std::endl;
		CheckRecursive( pBone->pBoneNextBrother );
	}

	
	
}

#include <stdlib.h>

XReturnType Parser::LoadFile(stdSTRING FileName)
{
	In.open(FileName.c_str(),std::ifstream::in);

	stdSTRING cadena = "archivo a abrir: " + FileName;
	

	
        if(!In.good()){ In.close(); std::cout << "ARCHIVO NO ENCONTRADO" << std::endl;;
                return STD_FILE_NOT_FOUND;}

        if(In.bad()){ In.close(); std::cout << "ARCHIVO INVALIDO " << std::endl;;
                return STD_INVALID_FILE;}


	stdSTRING Line;
	std::getline(In,Line);


    /*if(Line != "xof 0303txt 0032") { In.close();
                return STD_UNKNOWN_FORMAT; }*/

	ctmp = 0;
	itemp = 0;

     //sample.open("output.txt");

	//LOGD("por checar blocks");

	ProcessBlocks();




	//system("pause");
	In.close();



        return STD_OK;

}

void Parser::FreeAll(){

	for (int i = 0; i < vMeshes.size();  i++)
	{
		LPSTDMESHDATA ptmpMeshRoot = vMeshes[i];

		if(ptmpMeshRoot->Vertex.Vertices){
			delete [] ptmpMeshRoot->Vertex.Vertices;
			ptmpMeshRoot->Vertex.Vertices = STDNULL;
		}

		if(ptmpMeshRoot->Normals.Vertices){
			delete [] ptmpMeshRoot->Normals.Vertices;
			ptmpMeshRoot->Normals.Vertices = STDNULL;
		}

		if(ptmpMeshRoot->Tangent.Vertices){
			delete [] ptmpMeshRoot->Tangent.Vertices;
			ptmpMeshRoot->Tangent.Vertices = STDNULL;
		}

		if(ptmpMeshRoot->BiNormal.Vertices){
			delete [] ptmpMeshRoot->BiNormal.Vertices;
			ptmpMeshRoot->BiNormal.Vertices = STDNULL;
		}

		if(ptmpMeshRoot->Vertex.Faces->Indices){
			delete [] ptmpMeshRoot->Vertex.Faces->Indices;
			ptmpMeshRoot->Vertex.Faces->Indices = STDNULL;
		}

		if(ptmpMeshRoot->Vertex.Faces){
			delete [] ptmpMeshRoot->Vertex.Faces;
			ptmpMeshRoot->Vertex.Faces = STDNULL;
		}

		if(ptmpMeshRoot->Normals.Faces->Indices){
			delete [] ptmpMeshRoot->Normals.Faces->Indices;
			ptmpMeshRoot->Normals.Faces->Indices = STDNULL;
		}
		if(ptmpMeshRoot->Normals.Faces){
			delete [] ptmpMeshRoot->Normals.Faces;
			ptmpMeshRoot->Normals.Faces = STDNULL;
		}

		if(ptmpMeshRoot->Tangent.Faces){
			delete [] ptmpMeshRoot->Tangent.Faces;
			ptmpMeshRoot->Tangent.Faces = STDNULL;
		}
		if(ptmpMeshRoot->BiNormal.Faces){
			delete [] ptmpMeshRoot->BiNormal.Faces;
			ptmpMeshRoot->BiNormal.Faces = STDNULL;
		}

		if(ptmpMeshRoot->DeclData.Elements){
			delete [] ptmpMeshRoot->DeclData.Elements;
			ptmpMeshRoot->DeclData.Elements = STDNULL;
		}
		if(ptmpMeshRoot->DeclData.Values){
			delete [] ptmpMeshRoot->DeclData.Values;
			ptmpMeshRoot->DeclData.Values = STDNULL;
		}

		if(ptmpMeshRoot->MaterialList.FaceIndices){
			delete [] ptmpMeshRoot->MaterialList.FaceIndices;
			ptmpMeshRoot->MaterialList.FaceIndices = STDNULL;
		}
		if(ptmpMeshRoot->MaterialList.Materials->EffectInstance.pDefaults){
			delete [] ptmpMeshRoot->MaterialList.Materials->EffectInstance.pDefaults;
			ptmpMeshRoot->MaterialList.Materials->EffectInstance.pDefaults = STDNULL;
		}
		if(ptmpMeshRoot->MaterialList.Materials){
			delete [] ptmpMeshRoot->MaterialList.Materials;
			ptmpMeshRoot->MaterialList.Materials = STDNULL;
		}

		if(ptmpMeshRoot->SkinInfo.SkinWeights){
			delete [] ptmpMeshRoot->SkinInfo.SkinWeights;
			ptmpMeshRoot->SkinInfo.SkinWeights = STDNULL;
		}
		if(ptmpMeshRoot->TexCoords.TexCoordinates){
			delete [] ptmpMeshRoot->TexCoords.TexCoordinates;
			ptmpMeshRoot->TexCoords.TexCoordinates = STDNULL;
		}

		ptmpMeshRoot = STDNULL;
	}

	for (int i = 0; i < vMeshes.size();  i++)
	{
		delete vMeshes[i];
		vMeshes[i] = STDNULL;
	}

	vMeshes.clear();
}