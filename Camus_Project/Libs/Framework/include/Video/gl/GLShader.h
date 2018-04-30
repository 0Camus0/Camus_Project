#include <Config.h>

#ifndef T1000_GLSHADER_H
#define T1000_GLSHADER_H

#include <video/BaseDriver.h>
#include <video/gl/GL_Headers.h>
#include <video/gl/GLSLParser.h>
#include <utils/Utils.h>

#include <vector>

namespace t1000 {
  class GLShader : public ShaderBase {
  public:
	  GLShader() {}

	  virtual ~GLShader() {}

	  bool CreateShaderAPI(std::string src_vs, std::string src_fs, unsigned long long sig) override;
    void  Set(const DeviceContext& deviceContext) override;
    void DestroyAPIShader() override;
	  unsigned int ShaderProg;


    std::vector<InputElement> locs;
    std::vector<InputElement> internalUniformsLocs;
    int vertexWidth;

	shader::GLSL_Parser m_parser;
    GLuint vshader_id;
    GLuint fshader_id;
  };
}


#endif
