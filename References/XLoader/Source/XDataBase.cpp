#include "../include/XDataBase.h"

bool	XDataBase::LoadXFile(const std::string	&FileName){
	ifstream	inFile(FileName.c_str(),ios::binary|ios::in);
	if(!inFile.good()){
		inFile.close();
		return false;
	}

	inFile.seekg( 0, std::ios::end );
	unsigned long Size = inFile.tellg();
	inFile.seekg( 0, std::ios::beg );
	 
	char *pData = new char[Size+1];
	pData[Size] = '\0';

	inFile.read(pData,Size);
	inFile.close();
		
	m_ActualStream.clear();
	m_ActualStream.str(std::string());

	m_ActualStream.write((const char*)pData,Size);

	m_ActualStream.seekg( 0, std::ios::beg );

	delete [] pData;
	pData = 0;

	return Parse(FileName);
}

bool	XDataBase::LoadXFromMemory(char* pData,const unsigned int &size){

	if(!pData||!size)
		return false;


	m_ActualStream.clear();
	m_ActualStream.str(std::string());

	m_ActualStream.write((const char*)pData,size);

	m_ActualStream.seekg( 0, std::ios::beg );

	return Parse("FromMemory");

}

unsigned short	XDataBase::GetTemplateType(std::string Line,std::string *retName){
	short ret = -1;
	for (int i = 0; i < xF::STD_X_REF ; i++){
		ret = Line.find(XstringTemplates[i]);
		if(ret!=-1){
			if(retName){
				short dif = Line.find_last_of(" ") - Line.find(" ",ret);
				*retName  = Line.substr(ret+XstringTemplates[i].size(),--dif);}
			return i;}}

	ret = Line.find("{");
	if((ret!=-1)&&(Line.find("}")!=-1)){
		if(retName!=xNULL){
			short dif = Line.find_last_of(" ") - Line.find(" ",ret);
			*retName  = Line.substr(++(++ret),--dif);}
		return xF::STD_X_REF;}

	return -1;
}

bool	XDataBase::Parse(std::string name){

	while(!m_Stack.empty()){
		m_Stack.pop();
	}

	xMesh	*tmp = new xMesh();
	XMeshDataBase.push_back(tmp);
	m_pActualMesh = XMeshDataBase.back();
	m_pActualMesh->FileName = name;

	std::string Line;
	while(!m_ActualStream.eof()){
		std::getline(m_ActualStream,Line);
		if(Line.find("{")!=-1){
			std::string rets;
			unsigned short Ret = GetTemplateType(Line,&rets);
			switch(Ret){
			case xF::STD_X_FRAME:{
#if	DEBUG_COUTS
				std::cout << "Found Frame : " << rets << std::endl;
#endif
				xBone tmp;
				tmp.Name = rets;
				tmp.Dad = 0;				
				m_pActualMesh->Skeleton.Bones.push_back(tmp);
			//	m_pActualMesh->SkeletonAnimated.Bones.push_back(tmp);
				ProcessFrameBlock(rets);
							 }break;

			case xF::STD_X_TICKSPERSECCOND:{
#if	DEBUG_COUTS
				std::cout << "Found Ticks : " << rets << std::endl;
#endif
				ProcessTicksPerSecond(&m_pActualMesh->Animation);
							 }break;

			case STD_X_ANIMATIONSET:{
#if	DEBUG_COUTS
				std::cout << "Found Animation Set : " << rets << std::endl;
#endif
				ProcessAnimationSet(&m_pActualMesh->Animation,rets);
									   }break;
			}//switch			
		}//if
	}


	for (unsigned int i = 0; i <  m_pActualMesh->Skeleton.Bones.size() ; i++){
		for (unsigned int j = 0; j <  m_pActualMesh->Skeleton.Bones[i].Sons.size() ; j++){
			for (unsigned int k = 0; k < m_pActualMesh->Skeleton.Bones[i].Sons.size() ; k++){
				if(m_pActualMesh->Skeleton.Bones[i].Sons[j]!=m_pActualMesh->Skeleton.Bones[i].Sons[k]){
					m_pActualMesh->Skeleton.Bones[m_pActualMesh->Skeleton.Bones[i].Sons[j]].Brothers.push_back(m_pActualMesh->Skeleton.Bones[i].Sons[k]);
				//	m_pActualMesh->SkeletonAnimated.Bones[m_pActualMesh->SkeletonAnimated.Bones[i].Sons[j]].Brothers.push_back(m_pActualMesh->SkeletonAnimated.Bones[i].Sons[k]);
					
				}
			}
		}
	}

	m_pActualMesh->Skeleton.NumBones = m_pActualMesh->Skeleton.Bones.size();
	m_pActualMesh->SkeletonAnimated.NumBones = m_pActualMesh->Skeleton.NumBones;
	m_pActualMesh->SkeletonAnimated.Bones = std::vector<xBone>(m_pActualMesh->Skeleton.Bones);

	for(unsigned int i = 0; i <  m_pActualMesh->Geometry.size(); i++){
		for(unsigned int j = 0; j < m_pActualMesh->Geometry[i].Info.SkinMeshHeader.NumBones; j++){
			for(unsigned int k = 0; k <  m_pActualMesh->Skeleton.Bones.size(); k++){
				if(m_pActualMesh->Geometry[i].Info.SkinWeights[j].NodeName.compare(m_pActualMesh->Skeleton.Bones[k].Name)==0){
					m_pActualMesh->Geometry[i].Info.SkinWeights[j].MatrixCombined = &m_pActualMesh->Skeleton.Bones[k].Combined;
					m_pActualMesh->Geometry[i].Info.SkinWeights[j].MatrixCombinedAnimation = &m_pActualMesh->SkeletonAnimated.Bones[k].Combined;
				}
			}
		}
	}
		

	m_pActualMesh->CreateBuffers();

	
	m_pActualMesh->m_AnimController = new AnimationController(&m_pActualMesh->Animation,&m_pActualMesh->Skeleton,&m_pActualMesh->SkeletonAnimated);
	

	return true;
}


void XDataBase::ProcessFrameBlock(std::string actual){
	m_Stack.push(actual);

	std::string Line;
	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);
		if(Line.find("{")!=-1){
			std::string rets;
			unsigned short Ret = GetTemplateType(Line,&rets);
			switch(Ret)
			{
			case xF::STD_X_FRAME:{
				
				xBone tmp;
				tmp.Name = rets;
				for (unsigned int i = 0; i < m_pActualMesh->Skeleton.Bones.size() ; i++)	{
					if(m_pActualMesh->Skeleton.Bones[i].Name.compare(m_Stack.top())==0){
						tmp.Dad = i;
						m_pActualMesh->Skeleton.Bones[i].Sons.push_back(m_pActualMesh->Skeleton.Bones.size());
					//	m_pActualMesh->SkeletonAnimated.Bones[i].Sons.push_back(m_pActualMesh->SkeletonAnimated.Bones.size());
						
						break;
					}
				}
				m_pActualMesh->Skeleton.Bones.push_back(tmp);
			//	m_pActualMesh->SkeletonAnimated.Bones.push_back(tmp);
				
				
				ProcessFrameBlock(rets);
				
							 }break;
			case xF::STD_X_MESH:{	
#if	DEBUG_COUTS
				std::cout << "Found Mesh name: " << rets <<std::endl;	
#endif
				ProcessMeshBlock(rets);

							}break;

			case xF::STD_X_OBJ_CMMTX:{
			//	ProcessMatrix(&m_pActualMesh->Skeleton.Bones.back().Bone);
				GetNextEndBracket();

							}break;

			case xF::STD_X_FRAME_TRANSFORM_MTX:{
			//	std::streampos PosStream = m_ActualStream.tellg();
				ProcessMatrix(&m_pActualMesh->Skeleton.Bones.back().Bone);
			//	m_ActualStream.seekg(PosStream);
			//	ProcessMatrix(&m_pActualMesh->SkeletonAnimated.Bones.back().Bone);
				
				GetNextEndBracket();

							}break;

			}

			
			
		}

		
	}

	if(m_Stack.size()>0)
		m_Stack.pop();
}

void XDataBase::ProcessMeshBlock(std::string actual){
	xF::xMeshGeometry	tmp;
	m_pActualMesh->Geometry.push_back(tmp);
	xF::xMeshGeometry *pActualGeometry = &m_pActualMesh->Geometry.back();

	pActualGeometry->Name = actual;
	pActualGeometry->RelativeMatrix = m_pActualMesh->Skeleton.Bones.back().Bone;

	m_ActualStream >> pActualGeometry->NumVertices >> c_temp;
	pActualGeometry->Positions.NumVertices = pActualGeometry->NumVertices;
	pActualGeometry->Positions.Vertices = new XVECTOR3[pActualGeometry->Positions.NumVertices];

	for (std_uint i = 0; i < pActualGeometry->Positions.NumVertices ; i++){
		m_ActualStream >> pActualGeometry->Positions.Vertices[i].x >> c_temp >> pActualGeometry->Positions.Vertices[i].y >> c_temp >> pActualGeometry->Positions.Vertices[i].z >> c_temp >> c_temp;
		pActualGeometry->Positions.Vertices[i].w = 1.0f;
	}

	m_ActualStream >> pActualGeometry->NumTriangles >> c_temp;

	pActualGeometry->Triangles.NumVertexIndices = 3*pActualGeometry->NumTriangles;
	pActualGeometry->Triangles.Indices = new stdWORD[pActualGeometry->Triangles.NumVertexIndices];
	unsigned int counter = 0;
	for (unsigned int i = 0; i < pActualGeometry->NumTriangles ; i++){
		unsigned short a,b,c;
		m_ActualStream >> c_temp >> c_temp >> a >> c_temp >> b >> c_temp >> c >> c_temp >> c_temp;
		pActualGeometry->Triangles.Indices[counter++] = a;
		pActualGeometry->Triangles.Indices[counter++] = b;
		pActualGeometry->Triangles.Indices[counter++] = c;
	}


	stdSTRING Line;
	//std::getline(m_ActualStream,Line);
	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case xF::STD_X_MESH_NORMALS:{
#if	DEBUG_COUTS
				std::cout << "Found Mesh Normals: " << std::endl;	
#endif
				ProcessNormalsBlock(pActualGeometry);
				//GetNextEndBracket();
									}break;

			case xF::STD_X_MESH_TEXCOORD:{
#if	DEBUG_COUTS
				std::cout << "Found Mesh Texture Coordinates: " << std::endl;	
#endif
				ProcessTexCoordinatesBlock(pActualGeometry);
				//GetNextEndBracket();
										}break;

			case xF::STD_X_DECLDATA:{
#if	DEBUG_COUTS
				std::cout << "Found Mesh decl data: " << std::endl;	
#endif
				ProcessDeclDataBlock(pActualGeometry);
			//	GetNextEndBracket();
								}break;

			case xF::STD_X_SKIN_HEADER:{
#if	DEBUG_COUTS
				std::cout << "Found Mesh skin header: " << rets  << std::endl;	
#endif
				ProcessSkinHeader(pActualGeometry);
			//	GetNextEndBracket();
								   }break;

			case xF::STD_X_SKIN_WEIGHTS:{
#if	DEBUG_COUTS
				std::cout << "Found Mesh skin weight: " << rets << std::endl;
#endif
				ProcessSkinWeights(pActualGeometry);
			//	GetNextEndBracket();
									}break;

			case xF::STD_X_MATERIALS_LIST:{
#if	DEBUG_COUTS
				std::cout << "Found Mesh material list: " << rets << std::endl;
#endif
				ProcessMaterialBlock(pActualGeometry);
			//	GetNextEndBracket();
									  }break;
			}
		}
	}

	
}

void XDataBase::ProcessTicksPerSecond(xF::xAnimationInfo* pAnimation){
	std::string Temp;
	m_ActualStream >> Temp;
	Temp = Temp.substr(0,Temp.size()-1);
	std::stringstream toInt(Temp);
	toInt >> pAnimation->TicksPerSecond;
	pAnimation->isAnimInfo = true;
}

void XDataBase::ProcessAnimationSet(xF::xAnimationInfo* pAnimation,const std::string name){
	xF::xAnimationSet tmp;
	pAnimation->Animations.push_back(tmp);
	xF::xAnimationSet* pActualAnimationSet = &pAnimation->Animations.back();

	pActualAnimationSet->Name = name;

	stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);	
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets)){
				case xF::STD_X_ANIMATION:{
#if DEBUG_COUTS
					std::cout << "Found Animation for: " << rets  << std::endl;	
#endif
					ProcessAnimation(pActualAnimationSet);
					}break;
			}
		}
	}

}

void XDataBase::ProcessAnimation(xF::xAnimationSet* out){
	xF::stdSTRING Line;
	while((Line.find("{")==-1)&&(Line.find("}")==-1)){
		std::getline(m_ActualStream,Line);
	}
	unsigned int FBracket = Line.find("{");	unsigned int LBracket = Line.find("}");	unsigned int dif = LBracket - FBracket;
	Line = Line.substr(++FBracket,--dif);	std::string::iterator end_pos = std::remove(Line.begin(), Line.end(), ' ');
	Line.erase(end_pos, Line.end());

	xF::xAnimationBone	tmp;
	out->BonesRef.push_back(tmp);
	xF::xAnimationBone* pCurrentAnimBone = &out->BonesRef.back();
	pCurrentAnimBone->BoneName = Line;

	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);	
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets)){
				case xF::STD_X_ANIMATION_KEY:{
#if DEBUG_COUTS
					cout << "Found Animation Key: " << rets << endl; 
#endif
					char type;
					m_ActualStream >> type >> c_temp;

						switch(type){
							case '0':{
#if DEBUG_COUTS
								cout << "Type Rotation"<< endl;
#endif
								ProcessAnimationKey_Rotation(pCurrentAnimBone);
									 }break;
							case '1':{
#if DEBUG_COUTS
								cout << "Type Scale"<< endl;
#endif
								ProcessAnimationKey_Scale(pCurrentAnimBone);
									 }break;
							case '2':{
#if DEBUG_COUTS
								cout << "Type Position"<< endl;
#endif
								ProcessAnimationKey_Position(pCurrentAnimBone);
									 }break;
						} // switch(type){


									 }break; // case STD_X_ANIMATION_KEY:{
				case xF::STD_X_ANIMATION_OPTIONS:{
#if DEBUG_COUTS
					cout << "Found Animation Options: " << rets << endl; 
#endif
							GetNextEndBracket();
										 }break;
			} // switch(GetTemplateType(Line,&rets)){
		}
	}

}

void XDataBase::ProcessAnimationKey_Rotation(xF::xAnimationBone* out){
	int size_vec = 0;
	m_ActualStream >> size_vec >> c_temp;

	out->RotationKeys.reserve(size_vec);

	for (int i = 0; i<size_vec;i++){
		xRotationKey tmp;
		m_ActualStream >> tmp.t.i_atTime >> c_temp >> c_temp >> c_temp >> tmp.Rot.w >> c_temp >> tmp.Rot.x >> c_temp >> tmp.Rot.y >> c_temp >> tmp.Rot.z >> c_temp >> c_temp >> c_temp;
#if DEBUG_COUTS
		cout << "time: " << tmp.t.i_atTime << " w: " << tmp.Rot.w << " x: " << tmp.Rot.x << " y: " << tmp.Rot.y << " z: " << tmp.Rot.z << endl;
#endif
		out->RotationKeys.push_back(tmp);
	}

	m_ActualStream >> c_temp;
	
}

void XDataBase::ProcessAnimationKey_Scale(xF::xAnimationBone* out){
	int size_vec = 0;
	m_ActualStream >> size_vec >> c_temp;

	out->ScaleKeys.reserve(size_vec);

	for (int i = 0; i<size_vec;i++){
		xScaleKey tmp;
		m_ActualStream >> tmp.t.i_atTime >> c_temp >> c_temp >> c_temp >> tmp.Scale.x >> c_temp >> tmp.Scale.y >> c_temp >> tmp.Scale.z >> c_temp >> c_temp >> c_temp;
#if DEBUG_COUTS		
		cout << "time: " << tmp.t.i_atTime << " x: " << tmp.Scale.x << " y: " << tmp.Scale.y << " z: " << tmp.Scale.z << endl;
#endif
		out->ScaleKeys.push_back(tmp);
	}

	m_ActualStream >> c_temp;
}

void XDataBase::ProcessAnimationKey_Position(xF::xAnimationBone* out){
	int size_vec = 0;
	m_ActualStream >> size_vec >> c_temp;

	out->PositionKeys.reserve(size_vec);

	for (int i = 0; i<size_vec;i++){
		xPositionKey tmp;
		m_ActualStream >> tmp.t.i_atTime >> c_temp >> c_temp >> c_temp >> tmp.Position.x >> c_temp >> tmp.Position.y >> c_temp >> tmp.Position.z >> c_temp >> c_temp >> c_temp;
#if DEBUG_COUTS
		cout << "time: " << tmp.t.i_atTime << " x: " << tmp.Position.x << " y: " << tmp.Position.y << " z: " << tmp.Position.z << endl;
#endif
		out->PositionKeys.push_back(tmp);
	}

	m_ActualStream >> c_temp;
}

void XDataBase::ProcessSkinHeader(xF::xMeshGeometry* pGeometry){
	m_ActualStream >> pGeometry->Info.SkinMeshHeader.MaxNumWeightPerVertex >> c_temp;
	m_ActualStream >> pGeometry->Info.SkinMeshHeader.MaxNumWeightPerFace >> c_temp;
	m_ActualStream >> pGeometry->Info.SkinMeshHeader.NumBones >> c_temp;

	if(pGeometry->Info.SkinMeshHeader.MaxNumWeightPerVertex==4){
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_SKININDEXES0;
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_SKINWEIGHTS0;
	}else if(pGeometry->Info.SkinMeshHeader.MaxNumWeightPerVertex==4){
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_SKININDEXES0;
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_SKINWEIGHTS0;
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_SKININDEXES1;
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_SKINWEIGHTS1;
	}

	pGeometry->Info.SkinWeights = new xSkinWeights[pGeometry->Info.SkinMeshHeader.NumBones];

	pGeometry->Info.SkinMeshHeader.NumBonesProcess = 0;

	GetNextEndBracket();
}

void  XDataBase::ProcessSkinWeights(xF::xMeshGeometry* pGeometry){
	
	xF::xSkinWeights	*pSkin = &pGeometry->Info.SkinWeights[pGeometry->Info.SkinMeshHeader.NumBonesProcess];

	m_ActualStream >> c_temp >> pSkin->NodeName;

	pSkin->NodeName = pSkin->NodeName.substr(0,pSkin->NodeName.size()-2);

	m_ActualStream >> pSkin->NumWeights >> c_temp;

	pSkin->VertexIndices = new xF::stdDWORD[pSkin->NumWeights];
	pSkin->Weights = new xF::stdFLOAT[pSkin->NumWeights];

	for (unsigned int i = 0; i < pSkin->NumWeights ; i++){
		m_ActualStream >> pSkin->VertexIndices[i] >> c_temp;
	}

	for (unsigned int i = 0; i < pSkin->NumWeights ; i++){
		m_ActualStream >> pSkin->Weights[i] >> c_temp;
	}
	
	for (int i = 0; i < 16 ; i++){
		m_ActualStream >> pSkin->MatrixOffset.mat[i] >> c_temp;
	}

	pGeometry->Info.SkinMeshHeader.NumBonesProcess++;

	GetNextEndBracket();
}

void XDataBase::ProcessMaterialBlock(xF::xMeshGeometry *pGeometry){

	

	m_ActualStream >> pGeometry->Materials.NumMaterials >> c_temp;
	
	pGeometry->Materials.Materials = new xF::xMaterial[pGeometry->Materials.NumMaterials];

	m_ActualStream >> pGeometry->Materials.NumFaceIndices >> c_temp;

	
	pGeometry->Materials.FaceIndices = new xF::stdDWORD[pGeometry->Materials.NumFaceIndices];

	for (unsigned int i = 0; i <  pGeometry->Materials.NumFaceIndices; i++){
		m_ActualStream >> pGeometry->Materials.FaceIndices[i] >> c_temp;
	}

	xF::stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case xF::STD_X_MATERIAL:{
#if DEBUG_COUTS
				std::cout << "Found Material : " << rets  << " num: " << pGeometry->Materials.NumMatProcess << std::endl;	
#endif
				pGeometry->Materials.Materials[pGeometry->Materials.NumMatProcess].Name = rets;
				ProcessMaterial(&pGeometry->Materials.Materials[pGeometry->Materials.NumMatProcess]);
				pGeometry->Materials.NumMatProcess++;
								}break;

			case xF::STD_X_REF:{
#if DEBUG_COUTS
				std::cout << "Found Reference : " << rets << std::endl;
#endif
				Line = "";
						   }break;

			}
		}
	}
}

void XDataBase::ProcessMaterial(xMaterial* out){

	m_ActualStream >> out->FaceColor.r >> c_temp >> out->FaceColor.g >> c_temp >> out->FaceColor.b >> c_temp >> out->FaceColor.a >> c_temp >> c_temp;

	m_ActualStream >> out->Power >> c_temp;

	m_ActualStream >> out->Specular.r >> c_temp >> out->Specular.g >> c_temp >> out->Specular.b >> c_temp >> c_temp;

	m_ActualStream >> out->Emissive.r >> c_temp >> out->Emissive.g >> c_temp >> out->Emissive.b >> c_temp >> c_temp;


	xF::stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);
		if(Line.find("{")!=-1){
			stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case xF::STD_X_EFFECT_INSTANCE:{
#if DEBUG_COUTS
				std::cout << "Found Effect : " << rets << std::endl;	
#endif
				out->bEffects = true;
				ProcessEffectInstance(&out->EffectInstance);
									   }break;

			case xF::STD_X_TEXTURE:{
#if DEBUG_COUTS
				std::cout << "Material Found Texture : " << rets << std::endl;	
#endif
				GetNextEndBracket();
							   }break;
			}
		}
	}

}

void XDataBase::ProcessEffectInstance(xF::xEffectInstance *out){
	

	m_ActualStream >> c_temp >> out->ShaderFileName;

	out->ShaderFileName = out->ShaderFileName.substr(0,out->ShaderFileName.size()-2);


	std::streampos PosStream = m_ActualStream.tellg();

	out->NumDefaults = 0;

	xF::stdSTRING Line;
	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);
		if(Line.find("{")!=-1){
			xF::stdSTRING rets;
			std_uint T = GetTemplateType(Line,&rets);
			if(T == xF::STD_X_EFFECT_PARAMS_DWORD || T == xF::STD_X_EFFECT_PARAMS_FLOAT || T == xF::STD_X_EFFECT_PARAMS_STRING ){
				out->NumDefaults++;
				GetNextEndBracket();
			}
		}
	}

	m_ActualStream.seekg(PosStream);
	Line = "";

	out->pDefaults = new xF::xEffectDefault[out->NumDefaults];

	out->NumProcess = 0;

	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);
		if(Line.find("{")!=-1){
			xF::stdSTRING rets;
			switch(GetTemplateType(Line,&rets))
			{
			case xF::STD_X_EFFECT_PARAMS_DWORD:{
#if DEBUG_COUTS
				std::cout << "Found DWORD " << std::endl;	
#endif
				ProcessEffectDwords(&out->pDefaults[out->NumProcess++]);
										   }break;

			case xF::STD_X_EFFECT_PARAMS_FLOAT:{
#if DEBUG_COUTS
				std::cout << "Found FLOAT " << std::endl;	
#endif
				ProcessEffectFloats(&out->pDefaults[out->NumProcess++]);
										   }break;

			case xF::STD_X_EFFECT_PARAMS_STRING:{
#if DEBUG_COUTS
				std::cout << "Found STRING " << std::endl;	
#endif
				ProcessEffectString(&out->pDefaults[out->NumProcess++]);
											}break;
			}
		}
	}

}


void XDataBase::ProcessEffectString(xF::xEffectDefault *out){
	

	out->Type = xF::STDX_STRINGS;

	m_ActualStream >> c_temp >> out->NameParam;

	out->NameParam = out->NameParam.substr(0,out->NameParam.size()-2);

	m_ActualStream >> c_temp >> out->CaseString;

	out->CaseString = out->CaseString.substr(0,out->CaseString.size()-2);

	GetNextEndBracket();
}

void XDataBase::ProcessEffectFloats(xF::xEffectDefault *out){
	out->Type = xF::STDX_FLOATS;

	m_ActualStream >> c_temp >> out->NameParam;

	out->NameParam = out->NameParam.substr(0,out->NameParam.size()-2);

	m_ActualStream >> out->NumElements >> c_temp;

	if(out->NumElements>0)
		out->CaseFloat = new xF::stdFLOAT[out->NumElements];

	for (std_uint i = 0; i < out->NumElements ; i++){
		m_ActualStream >> out->CaseFloat[i] >> c_temp;
	}


	GetNextEndBracket();
}

void XDataBase::ProcessEffectDwords(xEffectDefault *out){
	out->Type = xF::STDX_DWORDS;

	m_ActualStream >> c_temp >> out->NameParam;

	out->NameParam = out->NameParam.substr(0,out->NameParam.size()-2);

	m_ActualStream >> out->CaseDWORD >> c_temp;

	GetNextEndBracket();
}

void XDataBase::ProcessNormalsBlock(xF::xMeshGeometry *pGeometry){
	
	m_ActualStream >> pGeometry->Normals.NumVertices >> c_temp;
	pGeometry->Normals.Vertices = new XVECTOR3[pGeometry->Normals.NumVertices];
	for (std_uint i = 0; i < pGeometry->Normals.NumVertices ; i++){
		m_ActualStream >> pGeometry->Normals.Vertices[i].x >> c_temp >> pGeometry->Normals.Vertices[i].y >> c_temp >> pGeometry->Normals.Vertices[i].z >> c_temp >> c_temp;
		pGeometry->Normals.Vertices[i].w = 0.0f;
	}
	pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_NORMAL;

	GetNextEndBracket();

}

void  XDataBase::ProcessTexCoordinatesBlock(xF::xMeshGeometry *pGeometry){
	
	m_ActualStream >> pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].NumTexcoords >> c_temp;
	pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].TexCoordinates = new XVECTOR2[pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].NumTexcoords];

	for (std_uint i = 0; i < pGeometry->Normals.NumVertices ; i++){
		m_ActualStream >> pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].TexCoordinates[i].x >> c_temp >> pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].TexCoordinates[i].y >>  c_temp >> c_temp;
	}

	pGeometry->NumChannelsTexCoords++;

	if(pGeometry->NumChannelsTexCoords==1){
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD0;
	}else if(pGeometry->NumChannelsTexCoords==2){
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD1;
	}else if(pGeometry->NumChannelsTexCoords==3){
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD2;
	}else if(pGeometry->NumChannelsTexCoords==4){
		pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD3;
	}

	
	GetNextEndBracket();
}

void XDataBase::ProcessDeclDataBlock(xF::xMeshGeometry *pGeometry){
	
	
	xF::xDeclData	Data;

	m_ActualStream >> Data.NumElements >> c_temp;

	Data.Elements = new xF::xVertexElement[Data.NumElements];

	for (unsigned int i = 0; i < Data.NumElements; i++){
		m_ActualStream >> Data.Elements[i].Type   >> c_temp 
		   >> Data.Elements[i].Method >> c_temp 
		   >> Data.Elements[i].Usage  >> c_temp 
		   >> Data.Elements[i].UsageIndex >> c_temp >> c_temp;
	}

	m_ActualStream >>  Data.NumValues >> c_temp;


	Data.Values = new xF::stdDWORD[Data.NumValues];

	for (unsigned int i = 0; i < Data.NumValues; i++){
		m_ActualStream >> Data.Values[i] >> c_temp;
	}

	int Offset = 0;
	for (unsigned int i = 0; i < Data.NumElements; i++){
		if(Data.Elements[i].Usage==xF::STDDECLUSAGE_TANGENT){
			pGeometry->Tangents.NumVertices = pGeometry->Positions.NumVertices;
			pGeometry->Tangents.Vertices = new XVECTOR3[pGeometry->Tangents.NumVertices];
			for (unsigned int j = 0; j < pGeometry->Tangents.NumVertices ; j++){
				pGeometry->Tangents.Vertices[j].x = *(float*)(&Data.Values[Offset++]);
				pGeometry->Tangents.Vertices[j].y = *(float*)(&Data.Values[Offset++]);
				pGeometry->Tangents.Vertices[j].z = *(float*)(&Data.Values[Offset++]);
				pGeometry->Tangents.Vertices[j].w = 0.0f;
			}
			pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TANGENT;
		}else if(Data.Elements[i].Usage==xF::STDDECLUSAGE_BINORMAL){
			pGeometry->Binormals.NumVertices = pGeometry->Positions.NumVertices;
			pGeometry->Binormals.Vertices = new XVECTOR3[pGeometry->Binormals.NumVertices];
			for (unsigned int j = 0; j < pGeometry->Binormals.NumVertices ; j++){
				pGeometry->Binormals.Vertices[j].x = *(float*)(&Data.Values[Offset++]);
				pGeometry->Binormals.Vertices[j].y = *(float*)(&Data.Values[Offset++]);
				pGeometry->Binormals.Vertices[j].z = *(float*)(&Data.Values[Offset++]);
				pGeometry->Binormals.Vertices[j].w = 0.0f;
			}
			pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_BINORMAL;
		}else if(Data.Elements[i].Usage==xF::STDDECLUSAGE_TEXCOORD){
			pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].NumTexcoords = pGeometry->Positions.NumVertices;
			pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].TexCoordinates = new XVECTOR2[pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].NumTexcoords];
			for (unsigned int j = 0; j < pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].NumTexcoords ; j++){
				pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].TexCoordinates[j].x = *(float*)(&Data.Values[Offset++]);
				pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].TexCoordinates[j].y = *(float*)(&Data.Values[Offset++]);
			}

			pGeometry->NumChannelsTexCoords++;

			if(pGeometry->NumChannelsTexCoords==1){
				pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD0;
			}else if(pGeometry->NumChannelsTexCoords==2){
				pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD1;
			}else if(pGeometry->NumChannelsTexCoords==3){
				pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD2;
			}else if(pGeometry->NumChannelsTexCoords==4){
				pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_TEXCOORD3;
			}

		}else if(Data.Elements[i].Usage==xF::STDDECLUSAGE_COLOR){
			pGeometry->VertexColors.NumVertices = pGeometry->Positions.NumVertices;
			pGeometry->VertexColors.Vertices = new XVECTOR3[pGeometry->VertexColors.NumVertices];
			for (unsigned int j = 0; j < pGeometry->Binormals.NumVertices ; j++){
				pGeometry->VertexColors.Vertices[j].x = *(float*)(&Data.Values[Offset++]);
				pGeometry->VertexColors.Vertices[j].y = *(float*)(&Data.Values[Offset++]);
				pGeometry->VertexColors.Vertices[j].z = *(float*)(&Data.Values[Offset++]);
				if(Data.Elements[i].Type==xF::STDDECLTYPE_FLOAT4){
					pGeometry->VertexColors.Vertices[j].w = *(float*)(&Data.Values[Offset++]);
				}else{
					pGeometry->VertexColors.Vertices[j].w = 0.0f;
				}
			}
			pGeometry->VertexAttributes|=xF::xMeshGeometry::HAS_VERTEXCOLOR;
		}
	}

	GetNextEndBracket();
}

void XDataBase::GetNextEndBracket(){
	std::string Line;
	std::getline(m_ActualStream,Line);
	while(Line.find("}")==-1){
		std::getline(m_ActualStream,Line);		
	}
}

void XDataBase::ProcessMatrix(XMATRIX44* out){
/*	XMATRIX44 Normal;
	char tmp;
	for (int i = 0; i < 16 ; i++){
		m_ActualStream >> Normal.mat[i] >> tmp;
	}

	for (int i = 0; i < 4 ; i++)
	{
		for (int j = 0; j < 4 ; j++)
		{
			out->m[i][j] = Normal.m[j][i];
		}
	}*/
 	char tmp;
 	for (int i = 0; i < 16 ; i++){
 		m_ActualStream >> out->mat[i] >> tmp;
 	}

}