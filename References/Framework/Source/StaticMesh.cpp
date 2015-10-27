#include "../Include/StaticMesh.h"

using namespace stdEngine;

STD_Static_Mesh::STD_Static_Mesh(){

}

bool STD_Static_Mesh::LoadMesh(string Path){

	FileParser.LoadFile(Path);

	std_uint NumVerts = 0, NumFaces = 0, NumMats = 0;
	for (int i = 0; i < FileParser.vMeshes.size(); i++)	{
		NumVerts+=FileParser.vMeshes[i]->Vertex.NumVertices;
		NumFaces+=FileParser.vMeshes[i]->Vertex.NumFaces;
		NumMats+=FileParser.vMeshes[i]->MaterialList.NumMaterials;
	}
	NumVertices = NumVerts;
	NumTris = NumFaces;
	NumSubsets = NumMats;
	NumMeshes = FileParser.vMeshes.size();

	Meshes = new STD_SubsetInfo[FileParser.vMeshes.size()];
	Subsets = new STD_SubsetInfo[NumSubsets];
	STD_TBNVertex  *Geometry = new STD_TBNVertex[NumVerts];
	unsigned short *Indices = new unsigned short[NumFaces*3];
	

	std_uint vertexIndex = 0 , trisIndex = 0, subsetIndex = 0, lastVertex = 0, lastIndex = 0;
	for (int i = 0; i < FileParser.vMeshes.size(); i++)	{
		for (int j = 0; j < FileParser.vMeshes[i]->Vertex.NumVertices; j++){
			Geometry[vertexIndex].Pos = FileParser.vMeshes[i]->Vertex.Vertices[j];
			Geometry[vertexIndex].TC = FileParser.vMeshes[i]->TexCoords.TexCoordinates[j];
			Geometry[vertexIndex].Normal = FileParser.vMeshes[i]->Normals.Vertices[j];
			Geometry[vertexIndex].Tangent = FileParser.vMeshes[i]->Tangent.Vertices[j];
			Geometry[vertexIndex].Binormal = FileParser.vMeshes[i]->BiNormal.Vertices[j];
			vertexIndex++;
		}

		std_uint SubCounter = 0;
		unsigned short *IndicesMesh = new unsigned short[FileParser.vMeshes[i]->Vertex.NumFaces*3];
		for (int j = 0; j < FileParser.vMeshes[i]->Vertex.NumFaces; j++){
			for (int k = 0; k < 3 ; k++){
				Indices[trisIndex] = lastVertex + FileParser.vMeshes[i]->Vertex.Faces[j].Indices[k];
				IndicesMesh[SubCounter] = Indices[trisIndex];
				trisIndex++;
				SubCounter++;
			}
		}

		STD_SubsetInfo *SubSubsets = new STD_SubsetInfo[FileParser.vMeshes[i]->MaterialList.NumMaterials];
		
		for (int j = 0;j < FileParser.vMeshes[i]->MaterialList.NumMaterials;j++)
			SubSubsets[j].Id = 0;

		for (int j = 0; j < FileParser.vMeshes[i]->MaterialList.NumFaceIndices; j++){
			SubSubsets[FileParser.vMeshes[i]->MaterialList.FaceIndices[j]].Id++;
		}	

		for (int j = 0;j < FileParser.vMeshes[i]->MaterialList.NumMaterials;j++){
			unsigned short *IndicesSubsets = new unsigned short[SubSubsets[j].Id*3];
			std_uint contador = 0;
			for (int k = 0; k < FileParser.vMeshes[i]->MaterialList.NumFaceIndices; k++){
				if(j==FileParser.vMeshes[i]->MaterialList.FaceIndices[k]){
					for(int p=0;p<3;p++){
					IndicesSubsets[contador] = lastVertex + FileParser.vMeshes[i]->Vertex.Faces[k].Indices[p];
					contador++;
					}
				}				
			}
			
			Subsets[subsetIndex+j].NumTris = SubSubsets[j].Id;
			Subsets[subsetIndex+j].MeshBelong = i;
			GetEffectInstance(&Subsets[subsetIndex+j].Material,&FileParser.vMeshes[i]->MaterialList.Materials[j].EffectInstance);
			glGenBuffers(1, &Subsets[subsetIndex+j].Id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Subsets[subsetIndex+j].Id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, contador*sizeof(unsigned short), IndicesSubsets, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			delete IndicesSubsets;
		}
			
		glGenBuffers(1, &Meshes[i].Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Meshes[i].Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, SubCounter*sizeof(unsigned short), IndicesMesh, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		delete [] IndicesMesh;
		delete [] SubSubsets;

		

		Meshes[i].WorldMatrix = FileParser.vMeshes[i]->Relative;
		Meshes[i].VertexStart = lastVertex;
		Meshes[i].NumVertex = FileParser.vMeshes[i]->Vertex.NumVertices;
		Meshes[i].TriStart = lastIndex;
		Meshes[i].NumTris = FileParser.vMeshes[i]->Vertex.NumFaces;
		

		lastVertex = vertexIndex;
		lastIndex = trisIndex;
		subsetIndex+=FileParser.vMeshes[i]->MaterialList.NumMaterials;
	}

	glGenBuffers(1, &IndexBuffObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumTris*3*sizeof(unsigned short), Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1,&VertexBuffObject);
	glBindBuffer(GL_ARRAY_BUFFER,VertexBuffObject);
	glBufferData(GL_ARRAY_BUFFER,(NumVertices*sizeof(STD_TBNVertex)),&Geometry[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

STD_SubsetInfo* STD_Static_Mesh::GetSubsetInfo(std_uint index){


}

void STD_Static_Mesh::GetEffectInstance(MaterialInfo* mat,xEffectInstance* effect){

	
	for (UINT j = 0; j < effect->NumDefaults; j++)
	{
		string Name = effect->pDefaults[j].NameParam;
		string Path;

		//  Maps
		if(Name == "diffuseMap"){
			Path = effect->pDefaults[j].CaseString;
			mat->DiffuseId = TXCONTAINER->LoadTexture(Path,true,false);
			}

		if(Name == "normalMap"){
			Path = effect->pDefaults[j].CaseString;
			mat->NormalId = TXCONTAINER->LoadTexture(Path,true,false);
		}
	}

}

void STD_Static_Mesh::Release(){
	glDeleteBuffers(1, &IndexBuffObject);
	glDeleteBuffers(1, &VertexBuffObject);

	for (int i = 0; i < NumMeshes; i++){
		glDeleteBuffers(1,&Meshes[i].Id);
	}

	for (int i = 0; i < NumSubsets; i++){
		glDeleteBuffers(1,&Subsets[i].Id);
	}

	FileParser.FreeAll();
}