#include <utils/Utils.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <Utils/FileSystem.h>
#include <Utils/Log.h>

#ifdef USING_GL_COMMON
void CheckGLError(){
    GLenum errCode;
    if ((errCode = glGetError()) !=
    GL_NO_ERROR)
    {
		LogPrintDebug("\nGL ERROR[%d]\n",errCode);
    exit(errCode);
    }
}

void CheckFBStatus(){
GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
switch(status)
{
  case GL_FRAMEBUFFER_COMPLETE:
  //  printf("Framebuffer complete.");
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
	  LogPrintDebug("[ERROR] Framebuffer incomplete: Attachment is NOT complete.");
    break;

  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
	  LogPrintDebug("[ERROR] Framebuffer incomplete: No image is attached to FBO.");
   break;
#ifndef USING_OPENGL
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
	  LogPrintDebug("[ERROR] Framebuffer incomplete: Attached images have different dimensions.");
    break;
#endif
  case GL_FRAMEBUFFER_UNSUPPORTED:
	  LogPrintDebug("[ERROR] Unsupported by FBO implementation.");
    break;

   default:
	   LogPrintDebug("[ERROR] Unknown error.");
    break;
}
}

void checkcompilederrors(GLuint shader, GLenum type) {
	GLint bShaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &bShaderCompiled);
	if (!bShaderCompiled) {
		int i32InfoLogLength, i32CharsWritten;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

		char* pszInfoLog = new char[i32InfoLogLength];
		glGetShaderInfoLog(shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
		char* pszMsg = new char[i32InfoLogLength + 256];
		if (type == GL_FRAGMENT_SHADER) {
			sprintf(pszMsg, "Failed to compile pixel shader: %s", pszInfoLog);
		}
		else if (type == GL_VERTEX_SHADER) {
			sprintf(pszMsg, "Failed to compile vertex shader: %s", pszInfoLog);
		}
		else {
			sprintf(pszMsg, "Failed to compile wtf shader: %s", pszInfoLog);
		}
		LogPrintDebug("%s", pszMsg);
		delete[] pszMsg;
		delete[] pszInfoLog;
	}
}

GLuint createShader(GLenum type, char* pSource) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const char**)&pSource, NULL);
	glCompileShader(shader);
	checkcompilederrors(shader, type);
	return shader;
}
#else
void checkcompilederrors(unsigned int shader, unsigned int type) {

}
unsigned int createShader(unsigned int type, char* pSource) {
	return 0;
}
#endif

char *file2string(const char *path) {
	//FILE *fd;
	//long len, r;

	std::string OS_Path = t1000::fs::Filesystem::instance()->GetResourcesPath();
	OS_Path += "Shaders/";
	std::string FilePath = OS_Path + std::string(path);

	int Size = 0;
	char *pData = (char*)t1000::fs::Filesystem::instance()->GetFile(FilePath, &Size);

	char *str = new char[Size + 1];
	str[Size] = '\0';
	memcpy(str, pData, Size);
	t1000::fs::Filesystem::instance()->ReleaseFile(pData);


	return str;

	/*
	if (!(fd = fopen(FilePath.c_str(), "r"))) {
		LogPrintDebug("[file2string] File [%s] does not exist\n", FilePath.c_str());
		return NULL;
	}
	fseek(fd, 0, SEEK_END);
	len = ftell(fd);
	fseek(fd, 0, SEEK_SET);
	char *str = (char*)malloc( 1 + len * sizeof(char));
	r = fread(str, sizeof(char), len, fd);
	str[r] = '\0';
	fclose(fd);

	*/

	return str;
}

std::string RemovePath(std::string p) {
	std::string path = p;
	int firstSlash = path.find_last_of("\\") + 1;
	int Length = path.size() - firstSlash;
	path = path.substr(firstSlash, Length);
	return path;
}
