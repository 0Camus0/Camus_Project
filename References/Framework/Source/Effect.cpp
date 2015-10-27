#include "../Include/Effect.h"
#include "../Include/TextureContainer.h"
#include "../Include/StaticMesh.h"

bool	STDTechnique::CompileTechnique(std::string name,std::string vert,std::string pixel){

	Name = name;

	char *vsSourceP = file2string(vert.c_str());
	char *fsSourceP = file2string(pixel.c_str());

	Program = glCreateProgram();

	bool	Ret = false;

	VertexIndex = createShader(GL_VERTEX_SHADER, vsSourceP);
	FragmentIndex = createShader(GL_FRAGMENT_SHADER, fsSourceP);

	glAttachShader(Program, VertexIndex);
	glAttachShader(Program, FragmentIndex);

	glLinkProgram(Program);


	Pos0           = glGetAttribLocation(Program, "myVertex");
	TexCoord0      = glGetAttribLocation(Program, "myUV");
	TexCoord1      = glGetAttribLocation(Program, "UV1");
	TexCoord2      = glGetAttribLocation(Program, "UV2");
	TexCoord3      = glGetAttribLocation(Program, "UV3");
	Normal0        = glGetAttribLocation(Program, "myNormal");
	Tangent0       = glGetAttribLocation(Program, "myTanget");
	Binormal0      = glGetAttribLocation(Program, "myBinormal");
	VertexWeights0 = glGetAttribLocation(Program, "Weights");
	VertexIndex0   = glGetAttribLocation(Program, "Indexes");

	Wloc		       =  glGetUniformLocation(Program, "World");
	WVPloc			   =  glGetUniformLocation(Program, "WorldViewProjection");
	WVPLightloc		   =  glGetUniformLocation(Program, "LightWorldViewProjection");
	UsePCFloc		   =  glGetUniformLocation(Program, "PCF");
	ResShadowMaploc	   =  glGetUniformLocation(Program,"ResShadowMap");
	Camloc			   =  glGetUniformLocation(Program, "CamPos"); 
	Light0loc		   =  glGetUniformLocation(Program, "myLightDirection");
	Light1loc		   =  glGetUniformLocation(Program, "myLightDirection1");
	Light2loc		   =  glGetUniformLocation(Program, "Light2Pos");
	DiffSamplerloc     =  glGetUniformLocation(Program, "sampler2d");
	NormalSamplerloc   =  glGetUniformLocation(Program, "normal2d");
	SpecularSamplerloc =  glGetUniformLocation(Program, "specular2d");
	GlossSamplerloc    =  glGetUniformLocation(Program, "gloss2d");
	ReflectMapSamplerloc = glGetUniformLocation(Program, "reflect2d"); 
	ShadowMapSamplerloc = glGetUniformLocation(Program,"shadow2d");

	return true;
}

void	STDTechnique::ApplyRenderStates(bool enable){
	if(enable){
		for (std_uint i = 0; i < vCaps.size(); i++){
			glEnable(vCaps[i]);
			switch(vCaps[i]){
				case GL_CULL_FACE:{
					glCullFace(CullMode);
					glFrontFace(FaceMode);
								  }break;

				case GL_DEPTH_TEST:{

								   }break;
			}
		}
	}else{
		for (std_uint i = 0; i < vCaps.size(); i++)
			glDisable(vCaps[i]);
	}
}

void	STDTechnique::UseTechnique(){
	 glUseProgram(Program);
}

void	STDTechnique::CleanTechnique(){
	ApplyRenderStates(false);
}

void	_STDEffect::AddTechnique(std::string name,std::string vert,std::string pixel){

	STDTechnique Tech;
	Tech.CompileTechnique(name,vert,pixel);
	Techniques.insert( pair<std::string,STDTechnique>(name,Tech) );
	vTechniques.push_back(Tech);
}

void	_STDEffect::SetTechnique(std::string Name){
	Actual = Techniques.find(Name);
	glUseProgram(Actual->second.Program);
}

void	_STDEffect::SetWorldMatrix(STDMATRIX& w){
	glUniformMatrix4fv( Actual->second.Wloc, 1, GL_FALSE, w);
}

void	_STDEffect::SetWVPMatrix(STDMATRIX& wvp){
	glUniformMatrix4fv( Actual->second.WVPloc, 1, GL_FALSE, wvp);
}

void _STDEffect::SetWVPLightMatrix(STDMATRIX& wvp){
	glUniformMatrix4fv( Actual->second.WVPLightloc, 1, GL_FALSE, wvp);
}

void	_STDEffect::SetCamPos(STDVECTOR3& campos){
	glUniform3fv( Actual->second.Camloc, 1, campos );
}

void	_STDEffect::SetLightPos(STDVECTOR3& lightpos,std_uint lightindex){
	switch(lightindex){
		case 0:{
			glUniform3fv( Actual->second.Light0loc, 1, lightpos );
			   }break;
		case 1:{
			glUniform3fv( Actual->second.Light1loc, 1, lightpos );
			   }break;
		case 2:{
			glUniform3fv( Actual->second.Light2loc, 1, lightpos );
			   }break;
	}
}

void	_STDEffect::SetFloat(std::string Name,float &f){
	glUniform1f(glGetUniformLocation(Actual->second.Program,Name.c_str()),f);
}

void	_STDEffect::SetVector3(std::string Name,STDVECTOR3 &v3){
	glUniform3fv(glGetUniformLocation(Actual->second.Program,Name.c_str()),1,v3);
}

void	_STDEffect::SetVector2(std::string Name,STDVECTOR2 &v2){
	glUniform2fv(glGetUniformLocation(Actual->second.Program,Name.c_str()),1,v2);
}

void	_STDEffect::SetMatrix(std::string Name,STDMATRIX &mat){
	glUniformMatrix4fv(glGetUniformLocation(Actual->second.Program,Name.c_str()),1,GL_FALSE,mat);
}

void	_STDEffect::SetBool(std::string Name,bool &b){
	glUniform1i(glGetUniformLocation(Actual->second.Program,Name.c_str()),(int)b);
}

void	_STDEffect::SetInt(std::string Name,int &i){
	glUniform1i(glGetUniformLocation(Actual->second.Program,Name.c_str()),i);
}

void	_STDEffect::SetDifusseTexture(std_uint id){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, GETTEXTURE(id).Id );
	glUniform1i(Actual->second.DiffSamplerloc,0);
}

void	_STDEffect::SetNormalTexture(std_uint id){
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, GETTEXTURE(id).Id );
	glUniform1i(Actual->second.NormalSamplerloc,1);
}

void	_STDEffect::SetSpecularTexture(std_uint id){
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, GETTEXTURE(id).Id );
	glUniform1i(Actual->second.SpecularSamplerloc,2);
}

void	_STDEffect::SetOtherTexture(std_uint id,std_uint sampler,std_uint samplernumber){
	glBindTexture(GL_TEXTURE_2D, GETTEXTURE(id).Id );
	glUniform1i(sampler,samplernumber);
}

void	_STDEffect::EnalbeSTD_TBNVertexLocs(){
	glEnableVertexAttribArray(Actual->second.Pos0);
	glEnableVertexAttribArray(Actual->second.TexCoord0);
	glEnableVertexAttribArray(Actual->second.Normal0);
	glEnableVertexAttribArray(Actual->second.Tangent0);
	glEnableVertexAttribArray(Actual->second.Binormal0);

	glVertexAttribPointer(Actual->second.Pos0, 3, GL_FLOAT, GL_FALSE, sizeof(STD_TBNVertex), (void*)0);
	glVertexAttribPointer(Actual->second.TexCoord0, 2, GL_FLOAT, GL_FALSE, sizeof(STD_TBNVertex),(void*)12 );
	glVertexAttribPointer(Actual->second.Normal0, 3, GL_FLOAT, GL_FALSE, sizeof(STD_TBNVertex),(void*)20  );
	glVertexAttribPointer(Actual->second.Tangent0, 3, GL_FLOAT, GL_FALSE, sizeof(STD_TBNVertex), (void*)32  );
	glVertexAttribPointer(Actual->second.Binormal0, 3, GL_FLOAT, GL_FALSE, sizeof(STD_TBNVertex), (void*)44  );
}

void _STDEffect::EnableVertexLocs(const xSubsetInfo &subset){
	unsigned int sizeVec3 = 0;
	unsigned short NumComponents = 0;
	unsigned short Stride = 0;
	if(subset.bAlignedVertex){
		sizeVec3 = 16;
		NumComponents = 4;
	}else{
		sizeVec3 = 12;
		NumComponents = 3;
	}
	if(subset.VertexAttrib&xMeshGeometry::HAS_POSITION){
		glEnableVertexAttribArray(Actual->second.Pos0);
		glVertexAttribPointer(Actual->second.Pos0, NumComponents, GL_FLOAT, GL_FALSE,subset.VertexSize, (void*)Stride);
		Stride+=sizeVec3;
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_NORMAL){
		glEnableVertexAttribArray(Actual->second.Normal0);
		glVertexAttribPointer(Actual->second.Normal0, NumComponents, GL_FLOAT, GL_FALSE,subset.VertexSize,(void*)Stride);
		Stride+=sizeVec3;
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TANGENT){
		glEnableVertexAttribArray(Actual->second.Tangent0);
		glVertexAttribPointer(Actual->second.Tangent0, NumComponents, GL_FLOAT, GL_FALSE,subset.VertexSize,(void*)Stride);
		Stride+=sizeVec3;
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_BINORMAL){
		glEnableVertexAttribArray(Actual->second.Binormal0);
		glVertexAttribPointer(Actual->second.Binormal0, NumComponents, GL_FLOAT, GL_FALSE,subset.VertexSize,(void*)Stride);
		Stride+=sizeVec3;
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD0){
		glEnableVertexAttribArray(Actual->second.TexCoord0);
		glVertexAttribPointer(Actual->second.TexCoord0, 2, GL_FLOAT, GL_FALSE,subset.VertexSize,(void*)Stride);
		Stride+=8;
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD1){
		glEnableVertexAttribArray(Actual->second.TexCoord1);
		glVertexAttribPointer(Actual->second.TexCoord1, 2, GL_FLOAT, GL_FALSE,subset.VertexSize,(void*)Stride);
		Stride+=8;
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD2){
		glEnableVertexAttribArray(Actual->second.TexCoord2);
		glVertexAttribPointer(Actual->second.TexCoord2, 2, GL_FLOAT, GL_FALSE,subset.VertexSize,(void*)Stride);
		Stride+=8;
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD3){
		glEnableVertexAttribArray(Actual->second.TexCoord3);
		glVertexAttribPointer(Actual->second.TexCoord3, 2, GL_FLOAT, GL_FALSE,subset.VertexSize,(void*)Stride);
		Stride+=8;
	}
}
void	_STDEffect::DisableVertexLocs(const xSubsetInfo &subset){
	if(subset.VertexAttrib&xMeshGeometry::HAS_POSITION){
		glDisableVertexAttribArray(Actual->second.Pos0);
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_NORMAL){
		glDisableVertexAttribArray(Actual->second.Normal0);
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TANGENT){
		glDisableVertexAttribArray(Actual->second.Tangent0);
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_BINORMAL){
		glDisableVertexAttribArray(Actual->second.Binormal0);
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD0){
		glDisableVertexAttribArray(Actual->second.Tangent0);
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD1){
		glDisableVertexAttribArray(Actual->second.TexCoord1);
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD2){
		glDisableVertexAttribArray(Actual->second.TexCoord2);
	}

	if(subset.VertexAttrib&xMeshGeometry::HAS_TEXCOORD3){
		glDisableVertexAttribArray(Actual->second.TexCoord3);
	}
}
void	_STDEffect::DisableSTD_TBNVertexLocs(){
	glDisableVertexAttribArray(Actual->second.Pos0);
	glDisableVertexAttribArray(Actual->second.TexCoord0);
	glDisableVertexAttribArray(Actual->second.Normal0);
	glDisableVertexAttribArray(Actual->second.Tangent0);
	glDisableVertexAttribArray(Actual->second.Binormal0);
}

void _STDEffect::EnableSTD_TBNVeretexToDepthLocs(){
	glEnableVertexAttribArray(Actual->second.Pos0);
	glVertexAttribPointer(Actual->second.Pos0, 3, GL_FLOAT, GL_FALSE, sizeof(STD_TBNVertex), (void*)0);
}
void _STDEffect::DisableSTD_TBNVeretexToDepthLocs(){
	glDisableVertexAttribArray(Actual->second.Pos0);
}

void printShaderInfoLog(GLuint shader) {
GLint length;
glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
if(length) {
char* buffer = new char[length];
glGetShaderInfoLog(shader, length, NULL, buffer);
printf("%s", buffer);
delete [] buffer;
GLint success;
glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
if(success != GL_TRUE) {
exit(1);
}
}
}

void checkcompilederrors(GLuint shader,GLenum type)
{
GLint bShaderCompiled;
glGetShaderiv(shader, GL_COMPILE_STATUS, &bShaderCompiled);
if (!bShaderCompiled)
{
// An error happened, first retrieve the length of the log message
int i32InfoLogLength, i32CharsWritten;
glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

// Allocate enough space for the message and retrieve it
char* pszInfoLog = new char[i32InfoLogLength];
glGetShaderInfoLog(shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

/*
Displays the message in a dialog box when the application quits
using the shell PVRShellSet function with first parameter prefExitMessage.
*/
	char* pszMsg = new char[i32InfoLogLength+256];
if(type == GL_FRAGMENT_SHADER )
{
sprintf(pszMsg, "Failed to compile pixel shader: %s", pszInfoLog);
}else if(type == GL_VERTEX_SHADER)
{
sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
}else
{
sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
}

printf("%s",pszMsg);
delete [] pszMsg;
delete [] pszInfoLog;

}
}

GLuint createShader(GLenum type, char* pSource) {
GLuint shader = glCreateShader(type);
glShaderSource(shader, 1, (const char**)&pSource, NULL);
glCompileShader(shader);
checkcompilederrors(shader,type);
return shader;
}

char *file2string(const char *path)
{
FILE *fd;
long len,r;
char *str;

if (!(fd = fopen(path, "r")))
{
fprintf(stderr, "Can't open file '%s'\n", path);
return NULL;
}

fseek(fd, 0, SEEK_END);
len = ftell(fd);
fseek(fd, 0, SEEK_SET);

str = (char*) malloc(len * sizeof(char));

r = fread(str, sizeof(char), len, fd);

str[r] = '\0';

fclose(fd);

return str;
}
