#include <Utils/xFile/XDataBase.h>
#include <Utils/FileSystem.h>
#include <Utils/Time.h>
#include <Utils/Log.h>


const char* xTemplatesc_Str[] = {
	"template ",
	"KeyValuePair ",
	"Frame ",
	"FrameTransformMatrix ",
	"ObjectMatrixComment ",
	"Mesh ",
	"MeshNormals ",
	"MeshTextureCoords ",
	"DeclData ",
	"XSkinMeshHeader ",
	"SkinWeights ",
	"MeshMaterialList ",
	"Material ",
	"EffectInstance ",
	"EffectParamDWord ",
	"EffectParamFloats ",
	"EffectParamString ",
	"TextureFilename ",
	"AnimTicksPerSecond ",
	"AnimationSet ",
	"Animation ",
	"AnimationKey ",
	"AnimationOptions "
	};

namespace xF {
#if !USE_STRING_STREAM
	void		XDataBase::advance_to_next_open_brace() {
		do{
			index++;
		}while (pData[index] != '{');
	}

	void XDataBase::advance_to_next_close_brace() {
		do {
			index++;
		} while (pData[index] != '}');
		index++;
	}
#endif

	bool	XDataBase::LoadXFile(const std::string	&FileName) {
		PROFILING_SCOPE("LoadXFile")

		std::string Path = fs::Filesystem::instance()->GetResourcesPath();
		Path += "Models/";
		Path += FileName;

		ifstream	inFile(Path.c_str(), ios::binary | ios::in);
		if (!inFile.good()) {
			inFile.close();
			return false;
		}

		inFile.seekg(0, std::ios::end);
		unsigned int Size = static_cast<unsigned int>(inFile.tellg());
		inFile.seekg(0, std::ios::beg);

		
		pData = new char[Size + 1];
		pData[Size] = '\0';

		inFile.read(pData, Size);
		inFile.close();

#if USE_STRING_STREAM
		m_ActualStream.clear();
		m_ActualStream.str(std::string());
		m_ActualStream.write((const char*)pData, Size);
		m_ActualStream.seekg(0, std::ios::beg);		
#endif
		index = 0;
		Parse(Path);

		delete [] pData;
		pData = 0;

		return true;
	}

	bool	XDataBase::LoadXFromMemory(char* pData, const unsigned int &size) {

		if (!pData || !size)
			return false;


		m_ActualStream.clear();
		m_ActualStream.str(std::string());

		m_ActualStream.write((const char*)pData, size);

		m_ActualStream.seekg(0, std::ios::beg);

		return Parse("FromMemory");

	}
#if !USE_STRING_STREAM
	unsigned int	XDataBase::GetxTemplateTypeChar(std::string &retName) {
		PROFILING_SCOPE("GetTemplateType");
/*
		unsigned int current_index = index;
		unsigned int ret = STD_NOT;
		while (pData[current_index] != '\n') {
			current_index--;
		}

		std::string Line = std::string(&pData[current_index + 1], index - current_index);
		for (current_index = 0; current_index < xF::STD_X_REF; current_index++) {
			ret = Line.find(xTemplatesStr[current_index]);
			if (ret != -1) {
					std::size_t dif = Line.find_last_of(" ") - Line.find(" ", ret);
					retName = Line.substr(ret + xTemplatesStr[current_index].size(), --dif);
#if DEBUG_COUTS
					LogPrintDebug("Template type found [%s] name [%s]", xTemplatesStr[current_index].c_str(), retName.c_str());
#endif
				return current_index;
			}
		}

		ret = Line.find("{");
		if ((ret != -1) && (Line.find("}") != -1)) {
				std::size_t dif = Line.find_last_of(" ") - Line.find(" ", ret);
				retName = Line.substr(++(++ret), --dif);
#if DEBUG_COUTS
				LogPrintDebug("Template type [%s] not recognized", retName.c_str());
#endif
		}

		avance_to_next_close_brace();

		return STD_NOT;
		*/

		unsigned int current_index = index;
		unsigned int ret = STD_NOT;
		while (pData[current_index] != '\n') {
			current_index--;
		}
		int size = index - current_index;
		char *tmpLine = new char[size+1];
		memcpy(tmpLine, &pData[current_index + 1],size);
		tmpLine[size] = '\0';
		for (unsigned int i = 0; i < STD_X_REF; i++) {
			if (strstr(tmpLine, xTemplatesc_Str[i])) {
				current_index = index;
				while (pData[current_index] != ' ') {
					current_index--;
				}
				
				while (pData[current_index] == ' ') {
					current_index--;
				}	
				ret = current_index+1;
				while (pData[current_index] != ' ') {
					current_index--;
				}
				current_index++;
				
				if (current_index < (index-size))
					current_index = (index - size) + 1;

				retName = std::string(&pData[current_index], ret - current_index);
				ret = i;
#if DEBUG_COUTS
				LogPrintDebug("Template type found [%s] name [%s]", xTemplatesc_Str[i], retName.c_str());
#endif
				break;
			}
		}

		if (retName.find("mesh") != -1) {
			LogPrintDebug("MESH");
		}


		if (retName.find("Dealer") != -1) {
			LogPrintDebug("Dealer");
		}

		delete[] tmpLine;
	

		return ret;
		/*
		while (pData[current_index] != ' ') {
   current_index--;
  }
  while (pData[current_index] == ' ') {
   current_index--;
  }
  while (pData[current_index] != ' ') {
   current_index--;
  }
		*/

	}
#endif

	unsigned int	XDataBase::GetxTemplateType(std::string Line, std::string *retName) {
		std::size_t ret = -1;
		for (int i = 0; i < xF::STD_X_REF; i++) {
			ret = Line.find(xTemplatesStr[i]);
			if (ret != -1) {
				if (retName) {
					std::size_t dif = Line.find_last_of(" ") - Line.find(" ", ret);
					*retName = Line.substr(ret + xTemplatesStr[i].size(), --dif);
				}
				return i;
			}
		}

		ret = Line.find("{");
		if ((ret != -1) && (Line.find("}") != -1)) {
			if (retName != xNULL) {
				std::size_t dif = Line.find_last_of(" ") - Line.find(" ", ret);
				*retName = Line.substr(++(++ret), --dif);
			}
			return xF::STD_X_REF;
		}

		return STD_NOT;
	}

	bool	XDataBase::Parse(std::string name) {
		PROFILING_SCOPE("Parse")

		while (!m_Stack.empty()) {
			m_Stack.pop();
		}

		xMesh	*tmp = new xMesh();
		XMeshDataBase.push_back(tmp);
		m_pActualMesh = XMeshDataBase.back();
		m_pActualMesh->FileName = name;

#if USE_STRING_STREAM
		std::string Line;
		while (!m_ActualStream.eof()) {
		std::getline(m_ActualStream, Line);
		if (Line.find("{") != -1) {
			std::string rets;
			unsigned short Ret = GetxTemplateType(Line, &rets);
			
#else
		while(pData[index]!='\0'){
		advance_to_next_open_brace();{
			std::string rets;
			unsigned int Ret = GetxTemplateTypeChar(rets);
#endif
			switch (Ret) {
			
				case xF::STD_X_FRAME: {
#if	DEBUG_COUTS
					LogPrintDebug("Found Frame [%s]", rets.c_str());
#endif
					xBone tmp;
					tmp.Name = rets;
					tmp.Dad = 0;
					m_pActualMesh->Skeleton.Bones.push_back(tmp);
					ProcessFrameBlock(rets);
				}break;

				case xF::STD_X_TICKSPERSECCOND: {
#if	DEBUG_COUTS
					std::cout << "Found Ticks : " << rets << std::endl;
#endif
					ProcessTicksPerSecond(&m_pActualMesh->Animation);
				}break;

				case STD_X_ANIMATIONSET: {
#if	DEBUG_COUTS
					std::cout << "Found Animation Set : " << rets << std::endl;
#endif
					ProcessAnimationSet(&m_pActualMesh->Animation, rets);
				}break;
#if !USE_STRING_STREAM
				default: {
					advance_to_next_close_brace();
				}break;
#endif
				}//switch			
			}//if
		}


		for (unsigned int i = 0; i < m_pActualMesh->Skeleton.Bones.size(); i++) {
			for (unsigned int j = 0; j < m_pActualMesh->Skeleton.Bones[i].Sons.size(); j++) {
				for (unsigned int k = 0; k < m_pActualMesh->Skeleton.Bones[i].Sons.size(); k++) {
					if (m_pActualMesh->Skeleton.Bones[i].Sons[j] != m_pActualMesh->Skeleton.Bones[i].Sons[k]) {
						m_pActualMesh->Skeleton.Bones[m_pActualMesh->Skeleton.Bones[i].Sons[j]].Brothers.push_back(m_pActualMesh->Skeleton.Bones[i].Sons[k]);
						//	m_pActualMesh->SkeletonAnimated.Bones[m_pActualMesh->SkeletonAnimated.Bones[i].Sons[j]].Brothers.push_back(m_pActualMesh->SkeletonAnimated.Bones[i].Sons[k]);

					}
				}
			}
		}

		m_pActualMesh->Skeleton.NumBones = m_pActualMesh->Skeleton.Bones.size();
		m_pActualMesh->SkeletonAnimated.NumBones = m_pActualMesh->Skeleton.NumBones;
		m_pActualMesh->SkeletonAnimated.Bones = std::vector<xBone>(m_pActualMesh->Skeleton.Bones);

		for (unsigned int i = 0; i < m_pActualMesh->Geometry.size(); i++) {
			for (unsigned int j = 0; j < m_pActualMesh->Geometry[i].Info.SkinMeshHeader.NumBones; j++) {
				for (unsigned int k = 0; k < m_pActualMesh->Skeleton.Bones.size(); k++) {
					if (m_pActualMesh->Geometry[i].Info.SkinWeights[j].NodeName.compare(m_pActualMesh->Skeleton.Bones[k].Name) == 0) {
						m_pActualMesh->Geometry[i].Info.SkinWeights[j].MatrixCombined = &m_pActualMesh->Skeleton.Bones[k].Combined;
						m_pActualMesh->Geometry[i].Info.SkinWeights[j].MatrixCombinedAnimation = &m_pActualMesh->SkeletonAnimated.Bones[k].Combined;
					}
				}
			}
		}


		m_pActualMesh->CreateBuffers();


		m_pActualMesh->m_AnimController = new AnimationController(&m_pActualMesh->Animation, &m_pActualMesh->Skeleton, &m_pActualMesh->SkeletonAnimated);


		return true;
	}


	void XDataBase::ProcessFrameBlock(std::string &actual) {
		PROFILING_SCOPE("ProcessFrameBlock")

		m_Stack.push(actual);

#if USE_STRING_STREAM
		std::string Line;
		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				std::string rets;
				unsigned short Ret = GetxTemplateType(Line, &rets);
#else
		while(pData[index]!='}'){
				advance_to_next_open_brace(); {
				std::string rets;
				unsigned int Ret = GetxTemplateTypeChar(rets);
#endif
				switch (Ret)
				{
				case xF::STD_X_FRAME: {

					xBone tmp;
					tmp.Name = rets;
					for (unsigned int i = 0; i < m_pActualMesh->Skeleton.Bones.size(); i++) {
						if (m_pActualMesh->Skeleton.Bones[i].Name.compare(m_Stack.top()) == 0) {
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
				case xF::STD_X_MESH: {
#if	DEBUG_COUTS
					LogPrintDebug("Found x Mesh [%s]", rets.c_str());
#endif
					ProcessMeshBlock(rets);

				}break;

				case xF::STD_X_OBJ_CMMTX: {
					//	ProcessMatrix(&m_pActualMesh->Skeleton.Bones.back().Bone);
#if USE_STRING_STREAM
					GetNextEndBracket();
#else
					advance_to_next_close_brace();
#endif

				}break;

				case xF::STD_X_FRAME_TRANSFORM_MTX: {
					//	std::streampos PosStream = m_ActualStream.tellg();
					ProcessMatrix(&m_pActualMesh->Skeleton.Bones.back().Bone);
					//	m_ActualStream.seekg(PosStream);
					//	ProcessMatrix(&m_pActualMesh->SkeletonAnimated.Bones.back().Bone);

					//system("pause");
#if USE_STRING_STREAM
					GetNextEndBracket();
#else
					advance_to_next_close_brace();
#endif

				}break;

				}



			}


		}

		if (m_Stack.size() > 0)
			m_Stack.pop();
	}

	void XDataBase::ProcessMeshBlock(std::string actual) {
		/*PROFILING_SCOPE("ProcessMeshBlock")*/
		xF::xMeshGeometry	tmp;
		{
			PROFILING_SCOPE("ProcessMeshBlock")
			

			tmp.Name = actual;
			tmp.RelativeMatrix = m_pActualMesh->Skeleton.Bones.back().Bone;

#if USE_STRING_STREAM
			m_ActualStream >> tmp.NumVertices >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			tmp.Positions.reserve(tmp.NumVertices);
#elif USE_VECTOR_ARRAY_MODE
			tmp.Positions = std::vector<XVECTOR3>(tmp.NumVertices);
#endif

			float x = 0.0f, y = 0.0f, z = 0.0f;
			for (std_uint i = 0; i < tmp.NumVertices; i++) {
				m_ActualStream >> x >> c_temp >> y >> c_temp >> z >> c_temp >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
				tmp.Positions.push_back(XVECTOR3(x, y, z));
#elif USE_VECTOR_ARRAY_MODE
				tmp.Positions[i].x = x;
				tmp.Positions[i].y = y;
				tmp.Positions[i].z = z;
#endif
			}

				m_ActualStream >> tmp.NumTriangles >> c_temp;

#if USE_VECTOR_RESERVE_AND_PUSH	
				tmp.Triangles.reserve(3 * tmp.NumTriangles);
#elif USE_VECTOR_ARRAY_MODE
				tmp.Triangles = std::vector<xWORD>(3 * tmp.NumTriangles);
#endif
				unsigned int counter = 0;
				unsigned short a, b, c;
				for (unsigned int i = 0; i < tmp.NumTriangles; i++) {
					m_ActualStream >> c_temp >> c_temp >> a >> c_temp >> b >> c_temp >> c >> c_temp >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH	
					tmp.Triangles.push_back(a);
					tmp.Triangles.push_back(b);
					tmp.Triangles.push_back(c);
#elif USE_VECTOR_ARRAY_MODE
					tmp.Triangles[counter++] = a;
					tmp.Triangles[counter++] = b;
					tmp.Triangles[counter++] = c;
#endif
				}

#else
			int current_index = index;
			int token = 0;
			while (pData[current_index] != ';') {
				current_index++;
				if (pData[current_index] == ' ')
					token = current_index;
			}
			char cNumVerts[10];
			cNumVerts[7] = '\0';
			memcpy(cNumVerts, &pData[token + 1], current_index - token);
			tmp.NumVertices = static_cast<xDWORD>(atof(cNumVerts));

			current_index++;
#if USE_VECTOR_RESERVE_AND_PUSH
			tmp.Positions.reserve(tmp.NumVertices);
#elif USE_VECTOR_ARRAY_MODE
			tmp.Positions = std::vector<XVECTOR3>(tmp.NumVertices);
#endif

			char cVertComponent[15];
			cVertComponent[14] = '\0';
			int cont = 0;
			for (unsigned int i = 0; i < tmp.NumVertices; i++) {
				cont = 0;
				while (pData[current_index] != ',') {

					if (pData[current_index] == ' ')
						token = current_index;

					if (pData[current_index] == ';') {
						memcpy(cVertComponent, &pData[token + 1], current_index - token);
						tmp.Positions[i].v[cont++] = static_cast<float>(atof(cVertComponent));
						token = current_index;
						if (cont == 4)
							break;
					}
					current_index++;
				}
				current_index++;
			}

			current_index++;



			while (pData[current_index] != ';') {
				current_index++;
				if (pData[current_index] == ' ')
					token = current_index;
			}

			memcpy(cNumVerts, &pData[token + 1], current_index - token);
			tmp.NumTriangles = static_cast<xDWORD>(atoi(cNumVerts));


			while (pData[current_index] != ' ') {
				current_index++;
			}

			std::string pstr = std::string(&pData[current_index], 5);
			LogPrintDebug("actual [%s]", pstr.c_str());

#if USE_VECTOR_RESERVE_AND_PUSH	
			tmp.Triangles.reserve(3 * tmp.NumTriangles);
#elif USE_VECTOR_ARRAY_MODE
			tmp.Triangles = std::vector<xWORD>(3 * tmp.NumTriangles);
#endif
			char cTriang[8];
			cTriang[7] = '\0';
			cont = 0;
			int delim = 0;
			for (unsigned int i = 0; i < tmp.NumTriangles; i++) {
				delim = 0;
				while (delim < 5) {
					if (pData[current_index] == ',' || pData[current_index] == ';') {
						if (delim != 0 && delim != 4) {
							memcpy(cTriang, &pData[token + 1], current_index - token);
							tmp.Triangles[cont++] = static_cast<unsigned short>(atoi(cTriang));
						}
						token = current_index;

						delim++;
					}
					current_index++;
				}
			}
			current_index++;
			/*
			for (unsigned int i = 0; i < tmp.NumVertices; i++) {
				LogPrintDebug("[%f;%f;%f;,]", tmp.Positions[i].x, tmp.Positions[i].y, tmp.Positions[i].z);
			}
			*/
			/*
			for (unsigned int i = 0; i < tmp.Triangles.size(); i++) {
				LogPrintDebug("[%d]", tmp.Triangles[i]);
			}
			*/
			/*
				std::string pstr = std::string(&pData[current_index], 5);
			LogPrintDebug("actual [%s]", pstr.c_str());
			*/

#endif

		}
		system("pause");

		xSTRING Line;
		//std::getline(m_ActualStream,Line);
		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				xSTRING rets;
				switch (GetxTemplateType(Line, &rets))
				{
				case xF::STD_X_MESH_NORMALS: {
#if	DEBUG_COUTS
					std::cout << "Found Mesh Normals: " << std::endl;
#endif
					ProcessNormalsBlock(&tmp);
					//GetNextEndBracket();
				}break;

				case xF::STD_X_MESH_TEXCOORD: {
#if	DEBUG_COUTS
					std::cout << "Found Mesh Texture Coordinates: " << std::endl;
#endif
					ProcessTexCoordinatesBlock(&tmp);
					//GetNextEndBracket();
				}break;

				case xF::STD_X_DECLDATA: {
#if	DEBUG_COUTS
					std::cout << "Found Mesh decl data: " << std::endl;
#endif
					ProcessDeclDataBlock(&tmp);
					//	GetNextEndBracket();
				}break;

				case xF::STD_X_SKIN_HEADER: {
#if	DEBUG_COUTS
					std::cout << "Found Mesh skin header: " << rets << std::endl;
#endif
					ProcessSkinHeader(&tmp);
					//	GetNextEndBracket();
				}break;

				case xF::STD_X_SKIN_WEIGHTS: {
#if	DEBUG_COUTS
					std::cout << "Found Mesh skin weight: " << rets << std::endl;
#endif
					ProcessSkinWeights(&tmp);
					//	GetNextEndBracket();
				}break;

				case xF::STD_X_MATERIALS_LIST: {
#if	DEBUG_COUTS
					std::cout << "Found Mesh material list: " << rets << std::endl;
#endif
					ProcessMaterialBlock(&tmp);
					//	GetNextEndBracket();
				}break;
				}
			}
		}

		m_pActualMesh->Geometry.push_back(tmp);
	}

	void XDataBase::ProcessTicksPerSecond(xF::xAnimationInfo* pAnimation) {
		PROFILING_SCOPE("ProcessTicksPerSecond")
		std::string Temp;
		m_ActualStream >> Temp;
		Temp = Temp.substr(0, Temp.size() - 1);
		std::stringstream toInt(Temp);
		toInt >> pAnimation->TicksPerSecond;
		pAnimation->isAnimInfo = true;
	}

	void XDataBase::ProcessAnimationSet(xF::xAnimationInfo* pAnimation, const std::string name) {
		PROFILING_SCOPE("ProcessAnimationSet")
		xF::xAnimationSet tmp;
		pAnimation->Animations.push_back(tmp);
		xF::xAnimationSet* pActualAnimationSet = &pAnimation->Animations.back();

		pActualAnimationSet->Name = name;

		xSTRING Line;
		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				xSTRING rets;
				switch (GetxTemplateType(Line, &rets)) {
				case xF::STD_X_ANIMATION: {
#if DEBUG_COUTS
					std::cout << "Found Animation for: " << rets << std::endl;
#endif
					ProcessAnimation(pActualAnimationSet);
				}break;
				}
			}
		}

	}

	void XDataBase::ProcessAnimation(xF::xAnimationSet* out) {
		PROFILING_SCOPE("ProcessAnimation")
		xF::xSTRING Line;
		while ((Line.find("{") == -1) && (Line.find("}") == -1)) {
			std::getline(m_ActualStream, Line);
		}
		unsigned int FBracket = Line.find("{");	unsigned int LBracket = Line.find("}");	unsigned int dif = LBracket - FBracket;
		Line = Line.substr(++FBracket, --dif);	std::string::iterator end_pos = std::remove(Line.begin(), Line.end(), ' ');
		Line.erase(end_pos, Line.end());

		xF::xAnimationBone	tmp;
		out->BonesRef.push_back(tmp);
		xF::xAnimationBone* pCurrentAnimBone = &out->BonesRef.back();
		pCurrentAnimBone->BoneName = Line;

		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				xSTRING rets;
				switch (GetxTemplateType(Line, &rets)) {
				case xF::STD_X_ANIMATION_KEY: {
#if DEBUG_COUTS
					cout << "Found Animation Key: " << rets << endl;
#endif
					char type;
					m_ActualStream >> type >> c_temp;

					switch (type) {
					case '0': {
#if DEBUG_COUTS
						cout << "Type Rotation" << endl;
#endif
						ProcessAnimationKey_Rotation(pCurrentAnimBone);
					}break;
					case '1': {
#if DEBUG_COUTS
						cout << "Type Scale" << endl;
#endif
						ProcessAnimationKey_Scale(pCurrentAnimBone);
					}break;
					case '2': {
#if DEBUG_COUTS
						cout << "Type Position" << endl;
#endif
						ProcessAnimationKey_Position(pCurrentAnimBone);
					}break;
					} // switch(type){


				}break; // case STD_X_ANIMATION_KEY:{
				case xF::STD_X_ANIMATION_OPTIONS: {
#if DEBUG_COUTS
					cout << "Found Animation Options: " << rets << endl;
#endif
					GetNextEndBracket();
				}break;
				} // switch(GetTemplateType(Line,&rets)){
			}
		}

	}

	void XDataBase::ProcessAnimationKey_Rotation(xF::xAnimationBone* out) {
		PROFILING_SCOPE("ProcessAnimationKey_Rotation")
		int size_vec = 0;
		m_ActualStream >> size_vec >> c_temp;

#if USE_VECTOR_RESERVE_AND_PUSH	
		out->RotationKeys.reserve(size_vec);
#elif USE_VECTOR_ARRAY_MODE
		out->RotationKeys = std::vector<xRotationKey>(size_vec);
#endif
		xRotationKey tmp;
		for (int i = 0; i < size_vec; i++) {		
			m_ActualStream >> tmp.t.i_atTime >> c_temp >> c_temp >> c_temp >> tmp.Rot.w >> c_temp >> tmp.Rot.x >> c_temp >> tmp.Rot.y >> c_temp >> tmp.Rot.z >> c_temp >> c_temp >> c_temp;
#if DEBUG_COUTS
			cout << "time: " << tmp.t.i_atTime << " w: " << tmp.Rot.w << " x: " << tmp.Rot.x << " y: " << tmp.Rot.y << " z: " << tmp.Rot.z << endl;
#endif
#if USE_VECTOR_RESERVE_AND_PUSH	
			out->RotationKeys.push_back(tmp);
#elif USE_VECTOR_ARRAY_MODE
			out->RotationKeys[i].t.i_atTime = tmp.t.i_atTime;
			out->RotationKeys[i].Rot.x = tmp.Rot.x;
			out->RotationKeys[i].Rot.y = tmp.Rot.y;
			out->RotationKeys[i].Rot.z = tmp.Rot.z;
			out->RotationKeys[i].Rot.w = tmp.Rot.w;
#endif
		}

		m_ActualStream >> c_temp;

	}

	void XDataBase::ProcessAnimationKey_Scale(xF::xAnimationBone* out) {
		PROFILING_SCOPE("ProcessAnimationKey_Scale")
		int size_vec = 0;
		m_ActualStream >> size_vec >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH	
		out->ScaleKeys.reserve(size_vec);
#elif USE_VECTOR_ARRAY_MODE
		out->ScaleKeys = std::vector<xScaleKey>(size_vec);
#endif
		xScaleKey tmp;
		for (int i = 0; i < size_vec; i++) {
			m_ActualStream >> tmp.t.i_atTime >> c_temp >> c_temp >> c_temp >> tmp.Scale.x >> c_temp >> tmp.Scale.y >> c_temp >> tmp.Scale.z >> c_temp >> c_temp >> c_temp;
#if DEBUG_COUTS		
			cout << "time: " << tmp.t.i_atTime << " x: " << tmp.Scale.x << " y: " << tmp.Scale.y << " z: " << tmp.Scale.z << endl;
#endif
#if USE_VECTOR_RESERVE_AND_PUSH	
			out->ScaleKeys.push_back(tmp);
#elif USE_VECTOR_ARRAY_MODE
			out->ScaleKeys[i].t.i_atTime = tmp.t.i_atTime;
			out->ScaleKeys[i].Scale.x = tmp.Scale.x;
			out->ScaleKeys[i].Scale.y = tmp.Scale.y;
			out->ScaleKeys[i].Scale.z = tmp.Scale.z;
#endif
		}

		m_ActualStream >> c_temp;
	}

	void XDataBase::ProcessAnimationKey_Position(xF::xAnimationBone* out) {
		PROFILING_SCOPE("ProcessAnimationKey_Position")
		int size_vec = 0;
		m_ActualStream >> size_vec >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH	
		out->PositionKeys.reserve(size_vec);
#elif USE_VECTOR_ARRAY_MODE
		out->PositionKeys = std::vector<xPositionKey>(size_vec);
#endif
		xPositionKey tmp;
		for (int i = 0; i < size_vec; i++) {		
			m_ActualStream >> tmp.t.i_atTime >> c_temp >> c_temp >> c_temp >> tmp.Position.x >> c_temp >> tmp.Position.y >> c_temp >> tmp.Position.z >> c_temp >> c_temp >> c_temp;
#if DEBUG_COUTS
			cout << "time: " << tmp.t.i_atTime << " x: " << tmp.Position.x << " y: " << tmp.Position.y << " z: " << tmp.Position.z << endl;
#endif

#if USE_VECTOR_RESERVE_AND_PUSH	
			out->PositionKeys.push_back(tmp);
#elif USE_VECTOR_ARRAY_MODE
			out->PositionKeys[i].t.i_atTime = tmp.t.i_atTime;
			out->PositionKeys[i].Position.x = tmp.Position.x;
			out->PositionKeys[i].Position.y = tmp.Position.y;
			out->PositionKeys[i].Position.z = tmp.Position.z;
#endif
		}

		m_ActualStream >> c_temp;
	}

	void XDataBase::ProcessSkinHeader(xF::xMeshGeometry* pGeometry) {
		PROFILING_SCOPE("ProcessSkinHeader")
		m_ActualStream >> pGeometry->Info.SkinMeshHeader.MaxNumWeightPerVertex >> c_temp;
		m_ActualStream >> pGeometry->Info.SkinMeshHeader.MaxNumWeightPerFace >> c_temp;
		m_ActualStream >> pGeometry->Info.SkinMeshHeader.NumBones >> c_temp;

		if (pGeometry->Info.SkinMeshHeader.MaxNumWeightPerVertex == 4) {
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_SKININDEXES0;
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_SKINWEIGHTS0;
		}
		else if (pGeometry->Info.SkinMeshHeader.MaxNumWeightPerVertex == 4) {
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_SKININDEXES0;
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_SKINWEIGHTS0;
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_SKININDEXES1;
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_SKINWEIGHTS1;
		}
#if USE_VECTOR_RESERVE_AND_PUSH	
		pGeometry->Info.SkinWeights.reserve(pGeometry->Info.SkinMeshHeader.NumBones);
		for (std::size_t i = 0; i < pGeometry->Info.SkinMeshHeader.NumBones; i++) {
			xSkinWeights tmp;
			pGeometry->Info.SkinWeights.push_back(tmp);
		}
#elif USE_VECTOR_ARRAY_MODE
		pGeometry->Info.SkinWeights = std::vector<xSkinWeights>(pGeometry->Info.SkinMeshHeader.NumBones);
#endif
	
		pGeometry->Info.SkinMeshHeader.NumBonesProcess = 0;

		GetNextEndBracket();
	}

	void  XDataBase::ProcessSkinWeights(xF::xMeshGeometry* pGeometry) {
		PROFILING_SCOPE("ProcessSkinWeights")
		xF::xSkinWeights	*pSkin = &pGeometry->Info.SkinWeights[pGeometry->Info.SkinMeshHeader.NumBonesProcess];

		m_ActualStream >> c_temp >> pSkin->NodeName;

		pSkin->NodeName = pSkin->NodeName.substr(0, pSkin->NodeName.size() - 2);

		xDWORD	NumWeights;
		m_ActualStream >> NumWeights >> c_temp;

#if USE_VECTOR_RESERVE_AND_PUSH
		pSkin->VertexIndices.reserve(NumWeights);
		pSkin->Weights.reserve(NumWeights);
#elif USE_VECTOR_ARRAY_MODE
		pSkin->VertexIndices = std::vector<xDWORD>(NumWeights);
		pSkin->Weights = std::vector<xFLOAT>(NumWeights);
#endif
		xDWORD tmp_d;
		for (unsigned int i = 0; i < NumWeights; i++) {
			m_ActualStream >> tmp_d >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			pSkin->VertexIndices.push_back(tmp_d);
#elif USE_VECTOR_ARRAY_MODE
			pSkin->VertexIndices[i] = tmp_d;
#endif
		}

		xFLOAT tmp_f;
		for (unsigned int i = 0; i < NumWeights; i++) {
			m_ActualStream >> tmp_f >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			pSkin->Weights.push_back(tmp_f);
#elif USE_VECTOR_ARRAY_MODE
			pSkin->Weights[i] = tmp_f;
#endif
		}

		for (int i = 0; i < 16; i++) {
			m_ActualStream >> pSkin->MatrixOffset.mat[i] >> c_temp;
		}

		pGeometry->Info.SkinMeshHeader.NumBonesProcess++;

		GetNextEndBracket();
	}

	void XDataBase::ProcessMaterialBlock(xF::xMeshGeometry *pGeometry) {
		PROFILING_SCOPE("ProcessMaterialBlock")
		xDWORD	NumMaterials = 0;
		xDWORD	NumFaceIndices = 0;

		m_ActualStream >> NumMaterials >> c_temp;

#if USE_VECTOR_RESERVE_AND_PUSH
		pGeometry->MaterialList.Materials.reserve(NumMaterials);
		xMaterial tmp_m;
		for (unsigned int i = 0; i < NumMaterials; i++) {
			pGeometry->MaterialList.Materials.push_back(tmp_m);
		}
#elif USE_VECTOR_ARRAY_MODE
		pGeometry->MaterialList.Materials = std::vector<xMaterial>(NumMaterials);
#endif

		m_ActualStream >> NumFaceIndices >> c_temp;


#if USE_VECTOR_RESERVE_AND_PUSH
		pGeometry->MaterialList.FaceIndices.reserve(NumFaceIndices);
#elif USE_VECTOR_ARRAY_MODE
		pGeometry->MaterialList.FaceIndices = std::vector<xDWORD>(NumFaceIndices);
#endif
		xDWORD tmp_d;
		for (unsigned int i = 0; i < NumFaceIndices; i++) {
			
			m_ActualStream >> tmp_d >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			pGeometry->MaterialList.FaceIndices.push_back(tmp_d);
#elif USE_VECTOR_ARRAY_MODE
			pGeometry->MaterialList.FaceIndices[i] = tmp_d;
#endif
		}

		xF::xSTRING Line;
		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				xSTRING rets;
				switch (GetxTemplateType(Line, &rets))
				{
				case xF::STD_X_MATERIAL: {
#if DEBUG_COUTS
					std::cout << "Found Material : " << rets << " num: " << pGeometry->MaterialList.NumMatProcess << std::endl;
#endif
					pGeometry->MaterialList.Materials[pGeometry->MaterialList.NumMatProcess].Name = rets;
					ProcessMaterial(&pGeometry->MaterialList.Materials[pGeometry->MaterialList.NumMatProcess]);
					pGeometry->MaterialList.NumMatProcess++;
				}break;

				case xF::STD_X_REF: {
#if DEBUG_COUTS
					std::cout << "Found Reference : " << rets << std::endl;
#endif
					Line = "";
				}break;

				}
			}
		}
	}

	void XDataBase::ProcessMaterial(xMaterial* out) {
		PROFILING_SCOPE("ProcessMaterial")
		m_ActualStream >> out->FaceColor.r >> c_temp >> out->FaceColor.g >> c_temp >> out->FaceColor.b >> c_temp >> out->FaceColor.a >> c_temp >> c_temp;

		m_ActualStream >> out->Power >> c_temp;

		m_ActualStream >> out->Specular.r >> c_temp >> out->Specular.g >> c_temp >> out->Specular.b >> c_temp >> c_temp;

		m_ActualStream >> out->Emissive.r >> c_temp >> out->Emissive.g >> c_temp >> out->Emissive.b >> c_temp >> c_temp;


		xF::xSTRING Line;
		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				xSTRING rets;
				switch (GetxTemplateType(Line, &rets))
				{
				case xF::STD_X_EFFECT_INSTANCE: {
#if DEBUG_COUTS
					std::cout << "Found Effect : " << rets << std::endl;
#endif
					out->bEffects = true;
					ProcessEffectInstance(&out->EffectInstance);
				}break;

				case xF::STD_X_TEXTURE: {
#if DEBUG_COUTS
					std::cout << "Material Found Texture : " << rets << std::endl;
#endif
					GetNextEndBracket();
				}break;
				}
			}
		}

	}

	void XDataBase::ProcessEffectInstance(xF::xEffectInstance *out) {
		PROFILING_SCOPE("ProcessEffectInstance")

		m_ActualStream >> c_temp >> out->ShaderFileName;

		out->ShaderFileName = out->ShaderFileName.substr(0, out->ShaderFileName.size() - 2);


		std::streampos PosStream = m_ActualStream.tellg();

		out->NumDefaults = 0;

		xF::xSTRING Line;
		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				xF::xSTRING rets;
				std_uint T = GetxTemplateType(Line, &rets);
				if (T == xF::STD_X_EFFECT_PARAMS_DWORD || T == xF::STD_X_EFFECT_PARAMS_FLOAT || T == xF::STD_X_EFFECT_PARAMS_STRING) {
					out->NumDefaults++;
					GetNextEndBracket();
				}
			}
		}

		m_ActualStream.seekg(PosStream);
		Line = "";
#if USE_VECTOR_RESERVE_AND_PUSH
		out->pDefaults.reserve(out->NumDefaults);
		for (std::size_t i = 0; i < out->NumDefaults; i++) {
			xEffectDefault tmp;
			out->pDefaults.push_back(tmp);
		}
#elif USE_VECTOR_ARRAY_MODE
		out->pDefaults = std::vector<xEffectDefault>(out->NumDefaults);
#endif

		out->NumProcess = 0;

		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
			if (Line.find("{") != -1) {
				xF::xSTRING rets;
				switch (GetxTemplateType(Line, &rets))
				{
				case xF::STD_X_EFFECT_PARAMS_DWORD: {
#if DEBUG_COUTS
					std::cout << "Found DWORD " << std::endl;
#endif
					ProcessEffectDwords(&out->pDefaults[out->NumProcess++]);
				}break;

				case xF::STD_X_EFFECT_PARAMS_FLOAT: {
#if DEBUG_COUTS
					std::cout << "Found FLOAT " << std::endl;
#endif
					ProcessEffectFloats(&out->pDefaults[out->NumProcess++]);
				}break;

				case xF::STD_X_EFFECT_PARAMS_STRING: {
#if DEBUG_COUTS
					std::cout << "Found STRING " << std::endl;
#endif
					ProcessEffectString(&out->pDefaults[out->NumProcess++]);
				}break;
				}
			}
		}

	}


	void XDataBase::ProcessEffectString(xF::xEffectDefault *out) {
	//	PROFILING_SCOPE("ProcessEffectString")

		out->Type = xF::STDX_STRINGS;

		m_ActualStream >> c_temp >> out->NameParam;

		out->NameParam = out->NameParam.substr(0, out->NameParam.size() - 2);

		m_ActualStream >> c_temp >> out->CaseString;

		out->CaseString = out->CaseString.substr(0, out->CaseString.size() - 2);

		GetNextEndBracket();
	}

	void XDataBase::ProcessEffectFloats(xF::xEffectDefault *out) {
	//	PROFILING_SCOPE("ProcessEffectFloats")

		out->Type = xF::STDX_FLOATS;

		m_ActualStream >> c_temp >> out->NameParam;

		out->NameParam = out->NameParam.substr(0, out->NameParam.size() - 2);

		xDWORD NumElements = 0;
		m_ActualStream >> NumElements >> c_temp;

#if USE_VECTOR_RESERVE_AND_PUSH
		if (NumElements > 0)
			out->CaseFloat.reserve(NumElements);
#elif USE_VECTOR_ARRAY_MODE
		if (NumElements > 0)
			out->CaseFloat = std::vector<xFLOAT>(NumElements);
#endif
		xFLOAT tmp_f;
		for (std_uint i = 0; i < NumElements; i++) {
			m_ActualStream >> tmp_f >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			out->CaseFloat.push_back(tmp_f);
#elif USE_VECTOR_ARRAY_MODE
			out->CaseFloat[i] = tmp_f;
#endif
		}


		GetNextEndBracket();
	}

	void XDataBase::ProcessEffectDwords(xEffectDefault *out) {
		//PROFILING_SCOPE("ProcessEffectDwords")
		out->Type = xF::STDX_DWORDS;

		m_ActualStream >> c_temp >> out->NameParam;

		out->NameParam = out->NameParam.substr(0, out->NameParam.size() - 2);

		m_ActualStream >> out->CaseDWORD >> c_temp;

		GetNextEndBracket();
	}

	void XDataBase::ProcessNormalsBlock(xF::xMeshGeometry *pGeometry) {
		xDWORD NumVertices = 0;
		m_ActualStream >> NumVertices >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
		pGeometry->Normals.reserve(NumVertices);
#elif USE_VECTOR_ARRAY_MODE
		pGeometry->Normals = std::vector<XVECTOR3>(NumVertices);
#endif
		float x = 0.0f, y = 0.0f, z = 0.0f;
		for (std_uint i = 0; i < NumVertices; i++) {
			m_ActualStream >> x >> c_temp >> y >> c_temp >> z >> c_temp >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			pGeometry->Normals.push_back(XVECTOR3(x, y, z));
#elif USE_VECTOR_ARRAY_MODE
			pGeometry->Normals[i].x = x;
			pGeometry->Normals[i].y = y;
			pGeometry->Normals[i].z = z;
#endif
		}
		pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_NORMAL;

		GetNextEndBracket();

	}

	void  XDataBase::ProcessTexCoordinatesBlock(xF::xMeshGeometry *pGeometry) {
		PROFILING_SCOPE("ProcessTexCoordinatesBlock")
		xDWORD NumTexcoords = 0;
		float x = 0.0f, y = 0.0f;

		m_ActualStream >> NumTexcoords >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
		pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].reserve(NumTexcoords);		
#elif USE_VECTOR_ARRAY_MODE
		pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords] = std::vector<XVECTOR2>(NumTexcoords);
#endif
		for (std_uint i = 0; i < pGeometry->Normals.size(); i++) {
			m_ActualStream >> x >> c_temp >> y >> c_temp >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].push_back(XVECTOR2(x, y));
#elif USE_VECTOR_ARRAY_MODE
			pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords][i].x = x;
			pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords][i].y = y;
#endif
		}

		pGeometry->NumChannelsTexCoords++;

		if (pGeometry->NumChannelsTexCoords == 1) {
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD0;
		}
		else if (pGeometry->NumChannelsTexCoords == 2) {
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD1;
		}
		else if (pGeometry->NumChannelsTexCoords == 3) {
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD2;
		}
		else if (pGeometry->NumChannelsTexCoords == 4) {
			pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD3;
		}


		GetNextEndBracket();
	}

	void XDataBase::ProcessDeclDataBlock(xF::xMeshGeometry *pGeometry) {
		PROFILING_SCOPE("ProcessDeclDataBlock")

		xF::xDeclData	Data;
		xDWORD NumElements = 0;
		xDWORD NumValues = 0;
		m_ActualStream >> NumElements >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
		Data.Elements.reserve(NumElements);
#elif USE_VECTOR_ARRAY_MODE
		Data.Elements = std::vector<xVertexElement>(NumElements);
#endif
		xVertexElement tmp_xv;
		for (unsigned int i = 0; i < NumElements; i++) {
			
			m_ActualStream  >> tmp_xv.Type >> c_temp
				>> tmp_xv.Method >> c_temp
				>> tmp_xv.Usage >> c_temp
				>> tmp_xv.UsageIndex >> c_temp >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			Data.Elements.push_back(tmp_xv);
#elif USE_VECTOR_ARRAY_MODE
			Data.Elements[i].Type = tmp_xv.Type;
			Data.Elements[i].Method = tmp_xv.Method;
			Data.Elements[i].Usage = tmp_xv.Usage;
			Data.Elements[i].UsageIndex = tmp_xv.UsageIndex;
#endif
		}

		m_ActualStream >> NumValues >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
		Data.Values.reserve(NumValues);
#elif USE_VECTOR_ARRAY_MODE
		Data.Values = std::vector<xDWORD>(NumValues);
#endif
		xDWORD	tmp_d;
		for (unsigned int i = 0; i < NumValues; i++) {		
			m_ActualStream >> tmp_d >> c_temp;
#if USE_VECTOR_RESERVE_AND_PUSH
			Data.Values.push_back(tmp_d);
#elif USE_VECTOR_ARRAY_MODE
			Data.Values[i] = tmp_d;
#endif
		}

		int Offset = 0;
		xDWORD NumVertices = pGeometry->Positions.size();
#if USE_VECTOR_RESERVE_AND_PUSH
		float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
#endif
		for (unsigned int i = 0; i < NumElements; i++) {
			if (Data.Elements[i].Usage == xF::STDDECLUSAGE_TANGENT) {		
#if USE_VECTOR_RESERVE_AND_PUSH
				pGeometry->Tangents.reserve(NumVertices);
#elif USE_VECTOR_ARRAY_MODE
				pGeometry->Tangents = std::vector<XVECTOR3>(NumVertices);
#endif
				for (unsigned int j = 0; j < NumVertices; j++) {
#if USE_VECTOR_RESERVE_AND_PUSH
					x = *(float*)(&Data.Values[Offset++]);
					y = *(float*)(&Data.Values[Offset++]);
					z = *(float*)(&Data.Values[Offset++]);
					pGeometry->Tangents.push_back(XVECTOR3(x, y, z));
#elif USE_VECTOR_ARRAY_MODE
					pGeometry->Tangents[j].x = *(float*)(&Data.Values[Offset++]);
					pGeometry->Tangents[j].y = *(float*)(&Data.Values[Offset++]);
					pGeometry->Tangents[j].z = *(float*)(&Data.Values[Offset++]);
#endif
				}
				pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TANGENT;
			}
			else if (Data.Elements[i].Usage == xF::STDDECLUSAGE_BINORMAL) {
#if USE_VECTOR_RESERVE_AND_PUSH
				pGeometry->Binormals.reserve(NumVertices);
#elif USE_VECTOR_ARRAY_MODE
				pGeometry->Binormals = std::vector<XVECTOR3>(NumVertices);
#endif
				for (unsigned int j = 0; j < NumVertices; j++) {
#if USE_VECTOR_RESERVE_AND_PUSH
					x = *(float*)(&Data.Values[Offset++]);
					y = *(float*)(&Data.Values[Offset++]);
					z = *(float*)(&Data.Values[Offset++]);
					pGeometry->Binormals.push_back(XVECTOR3(x, y, z));
#elif USE_VECTOR_ARRAY_MODE
					pGeometry->Binormals[j].x = *(float*)(&Data.Values[Offset++]);
					pGeometry->Binormals[j].y = *(float*)(&Data.Values[Offset++]);
					pGeometry->Binormals[j].z = *(float*)(&Data.Values[Offset++]);
#endif
				}
				pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_BINORMAL;
			}
			else if (Data.Elements[i].Usage == xF::STDDECLUSAGE_TEXCOORD) {
#if USE_VECTOR_RESERVE_AND_PUSH
				float x = 0.0f, y = 0.0f;
				pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].reserve(NumVertices);
#elif USE_VECTOR_ARRAY_MODE
				pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords] = std::vector<XVECTOR2>(NumVertices);
#endif	
				for (unsigned int j = 0; j < NumVertices; j++) {
#if USE_VECTOR_RESERVE_AND_PUSH
					x = *(float*)(&Data.Values[Offset++]);
					y = *(float*)(&Data.Values[Offset++]);
					pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords].push_back(XVECTOR2(x, y));
#elif USE_VECTOR_ARRAY_MODE
					pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords][j].x = *(float*)(&Data.Values[Offset++]);
					pGeometry->TexCoordinates[pGeometry->NumChannelsTexCoords][j].y = *(float*)(&Data.Values[Offset++]);
#endif
				}

				pGeometry->NumChannelsTexCoords++;

				if (pGeometry->NumChannelsTexCoords == 1) {
					pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD0;
				}
				else if (pGeometry->NumChannelsTexCoords == 2) {
					pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD1;
				}
				else if (pGeometry->NumChannelsTexCoords == 3) {
					pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD2;
				}
				else if (pGeometry->NumChannelsTexCoords == 4) {
					pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_TEXCOORD3;
				}

			}
			else if (Data.Elements[i].Usage == xF::STDDECLUSAGE_COLOR) {
#if USE_VECTOR_RESERVE_AND_PUSH
				pGeometry->VertexColors.reserve(NumVertices);
#elif USE_VECTOR_ARRAY_MODE
				pGeometry->VertexColors = std::vector<XVECTOR3>(NumVertices);
#endif
				for (unsigned int j = 0; j < NumVertices; j++) {
#if USE_VECTOR_RESERVE_AND_PUSH
					x = *(float*)(&Data.Values[Offset++]);
					y = *(float*)(&Data.Values[Offset++]);
					z = *(float*)(&Data.Values[Offset++]);
					if (Data.Elements[i].Type == xF::STDDECLTYPE_FLOAT4) {
						w = *(float*)(&Data.Values[Offset++]);
					}
					else {
						w = 0.0f;
					}
					pGeometry->VertexColors.push_back(XVECTOR3(x, y, z, w));
#elif USE_VECTOR_ARRAY_MODE
					pGeometry->VertexColors[j].x = *(float*)(&Data.Values[Offset++]);
					pGeometry->VertexColors[j].y = *(float*)(&Data.Values[Offset++]);
					pGeometry->VertexColors[j].z = *(float*)(&Data.Values[Offset++]);
					if (Data.Elements[i].Type == xF::STDDECLTYPE_FLOAT4) {
						pGeometry->VertexColors[j].w = *(float*)(&Data.Values[Offset++]);
					}
					else {
						pGeometry->VertexColors[j].w = 0.0f;
					}
#endif
				}
				pGeometry->VertexAttributes |= xF::xMeshGeometry::HAS_VERTEXCOLOR;
			}
		}

		GetNextEndBracket();
	}

	void XDataBase::GetNextEndBracket() {
		std::string Line;
		std::getline(m_ActualStream, Line);
		while (Line.find("}") == -1) {
			std::getline(m_ActualStream, Line);
		}
	}

	void XDataBase::ProcessMatrix(XMATRIX44* out) {
#if PROFILE_MATRICES
		PROFILING_SCOPE("ProcessMatrix")
#endif

#if USE_STRING_STREAM
		char tmp;
		for (int i = 0; i < 16; i++) {
			m_ActualStream >> out->mat[i] >> tmp;
		}
#else
		char NumTemp[10];
		NumTemp[9] = '\0';

		int space = index;
		while (pData[index] != ',') {
			index++;
			if (pData[index] == ' ')
				space = index;
		}
		int firstcoma = index;
		int matCount = 0;
		index = firstcoma + 1;
		while (pData[index] != ' ') {
			if (pData[index] == ',' || pData[index] == ';') {
				memcpy(NumTemp, &pData[space + 1], 9);
				out->mat[matCount++] = static_cast<float>(atof(NumTemp));
				space = index;
				if(pData[index] == ';')
					break;
			}	
			index++;
		}
	#if DEBUG_MATRICES
			for (unsigned int i = 0; i < 16; i++) {
				LogPrintDebug("[%f]", out->mat[i]);
			}
	#endif
#endif

	}
}