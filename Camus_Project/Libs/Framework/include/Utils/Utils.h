#ifndef T800_UTILS_GL_H
#define T800_UTILS_GL_H

#include <Config.h>

#include <stdio.h>
#include <iostream>
#include <string>

#include <video/gl/GL_Headers.h>

#ifdef USING_GL_COMMON
void checkcompilederrors(GLuint shader, GLenum type);
void CheckGLError();
void CheckFBStatus();
GLuint createShader(GLenum type, char* pSource);
#else
void checkcompilederrors(unsigned int shader, unsigned int type);
unsigned int createShader(unsigned int type, char* pSource);
#endif

char *file2string(const char *path);
std::string RemovePath(std::string p);

#endif
