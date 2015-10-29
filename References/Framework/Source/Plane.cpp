#include "../Include/Plane.h"

void Plane::Create(string modelPath,string texturePath){


	MeshP.LoadFile(modelPath.c_str());


	Indices = new unsigned short[MeshP.vMeshes[0]->Vertex.NumFaces*3];

	int conts = 0;
	for (int i = 0; i < MeshP.vMeshes[0]->Vertex.NumFaces ; i++)
	{
		for (int j = 0; j < 3 ; j++)
		{
			Indices[conts] = MeshP.vMeshes[0]->Vertex.Faces[i].Indices[j];
			conts++;
		}
	}


	char *vsSourceP = file2string("Shaders/SkyVert.vert");
	char *fsSourceP = file2string("Shaders/SkyPixel.frag");


	ShaderProgramP = glCreateProgram();

	VertexShaderP = createShader(GL_VERTEX_SHADER, vsSourceP);
	FragShaderP = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(ShaderProgramP, VertexShaderP);
	glAttachShader(ShaderProgramP, FragShaderP);

	glLinkProgram(ShaderProgramP);
	glUseProgram(ShaderProgramP);

	VertexLoc = glGetAttribLocation(ShaderProgramP, "myVertex");

	UVLoc = glGetAttribLocation(ShaderProgramP, "myUV");

	isampler2DP = glGetUniformLocation(ShaderProgramP, "sampler2d");

	TextureP.Id = TXCONTAINER->LoadTexture(texturePath,true,false); 

	glBindTexture(GL_TEXTURE_2D,GETTEXTURE(TextureP.Id).Id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	STDMATRIX scl;
	STDMATRIX rot;
	MatRotationY(&rot,Deg2Rad(180.0f));
	MatScaling(&scl,2.5f,2.5f,2.5f);
	WorldP=scl*rot;
}

void Plane::Render(Cam* cam,bool invert){

	STDMATRIX scl;
	STDMATRIX rot;
	STDMATRIX tras;
	MatRotationY(&rot,Deg2Rad(180.0f));
	if(invert){
		MatScaling(&scl,2.5f,-2.5f,2.5f);
	}else{
		MatScaling(&scl,2.5f,2.5f,2.5f);
	}

	MatTranslation(&tras,cam->GetPosition()->x,cam->GetPosition()->y,cam->GetPosition()->z);
	WorldP=scl*rot*tras;

	glUseProgram(ShaderProgramP);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,  GETTEXTURE(TextureP.Id).Id );

	int cacatelas = glGetUniformLocation(ShaderProgramP, "World");
	glUniformMatrix4fv( cacatelas, 1, GL_FALSE, WorldP);

	int i32Location = glGetUniformLocation(ShaderProgramP, "WorldViewProjection");
	glUniformMatrix4fv( i32Location, 1, GL_FALSE, ((WorldP) * ( *cam->GetViewMatrix() ) * (*cam->GetProjectionMatrix())) );

	glEnableVertexAttribArray(VertexLoc);
	glVertexAttribPointer(VertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(STDVECTOR3), &MeshP.vMeshes[0]->Vertex.Vertices[0]);

	glEnableVertexAttribArray(UVLoc);
	glVertexAttribPointer(UVLoc, 2, GL_FLOAT, GL_FALSE, sizeof(STDVECTOR2), &MeshP.vMeshes[0]->TexCoords.TexCoordinates[0] );
	glUniform1i(isampler2DP,0);

	glDrawElements(GL_TRIANGLES, MeshP.vMeshes[0]->Vertex.NumFaces*3 , GL_UNSIGNED_SHORT,  Indices);

	glDisableVertexAttribArray(VertexLoc);
	glDisableVertexAttribArray(UVLoc);
}

void Plane::Release(){


}
