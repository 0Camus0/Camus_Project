#include <Config.h>

#ifndef T1000_GLDRIVER_H
#define T1000_GLDRIVER_H

#include <video/BaseDriver.h>
#include <video/gl/GL_Headers.h>

#ifdef OS_ANDROID
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>
#endif

#include <vector>

namespace t1000 {
  /* DEVICES */
  class GLDeviceContext : public DeviceContext {
  public:
    void* GetAPIObject() const override;
    void** GetAPIObjectReference() const override;

    void release() override;
    void SetPrimitiveTopology(T_TOPOLOGY::E topology) override;
    void DrawIndexed(unsigned vertexCount, unsigned startIndex, unsigned startVertex) override;
    int internalIBFormat;
    int internalTopology;
    int internalStride;
    unsigned int internalShaderProgram;
  private:
  };
  class GLDevice : public Device {
  public:
    void* GetAPIObject() const override;
    void** GetAPIObjectReference() const override;

    void release() override;
    Buffer* CreateBuffer(T_BUFFER_TYPE::E bufferType, BufferDesc desc, void* initialData = nullptr) override;
    ShaderBase* CreateShader(std::string src_vs, std::string src_fs, unsigned long long sig = T_NO_SIGNATURE) override;
    Texture* CreateTexture(std::string path) override;
    Texture* CreateTextureFromMemory(const unsigned char *buff, int w, int h, int channels, std::string name) override;
    Texture* CreateCubeMap(const unsigned char * buff, int w, int h) override;
    BaseRT* CreateRT(int nrt, int cf, int df, int w, int h, bool genMips = false) override; // TODO RT Desc
  private:
  };

  /* BUFFERS */
  class GLVertexBuffer : public VertexBuffer {
  public:
    void* GetAPIObject() const override;
    void** GetAPIObjectReference() const override;

    GLVertexBuffer() = default;
    GLVertexBuffer(GLVertexBuffer const& other) = default;
    GLVertexBuffer(GLVertexBuffer&& other) = default;

    void Set(const DeviceContext& deviceContext, const unsigned stride, const unsigned offset) override;
    void UpdateFromSystemCopy(const DeviceContext& deviceContext) override;
    void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) override;
    void release() override;
  private:
    friend Device;
    void Create(const Device& device, BufferDesc desc, void* initialData = nullptr) override;
    unsigned int APIID;
  };
  class GLIndexBuffer : public IndexBuffer {
  public:
    void* GetAPIObject() const override;
    void** GetAPIObjectReference() const override;

    void Set(const DeviceContext& deviceContext, const unsigned offset, T_IB_FORMAT::E format = T_IB_FORMAT::R32) override;
    void UpdateFromSystemCopy(const DeviceContext& deviceContext) override;
    void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) override;
    void release() override;
  private:
    friend Device;
    void Create(const Device& device, BufferDesc desc, void* initialData = nullptr) override;
    unsigned int APIID;
  };
  class GLConstantBuffer : public ConstantBuffer {
  public:
    void* GetAPIObject() const override;
    void** GetAPIObjectReference() const override;

    void Set(const DeviceContext& deviceContext) override;
    void UpdateFromSystemCopy(const DeviceContext& deviceContext) override;
    void UpdateFromBuffer(const DeviceContext& deviceContext, const void* buffer) override;
    void release() override;
  private:
    friend Device;
    void Create(const Device& device, BufferDesc desc, void* initialData = nullptr) override;
    unsigned int APIID;
  };



  class GLDriver : public BaseDriver {
  public:
	  GLDriver();
    void	InitDriver();
    void	CreateSurfaces();
    void	DestroySurfaces();
    void	Update();
    void	DestroyDriver();
    void	SetWindow(void *window);
    void	SetDimensions(int, int);
    void	SetBlendState(BLEND_STATES state) override;
    void	SetDepthStencilState(DEPTH_STENCIL_STATES state) override;
    void	SaveScreenshot(std::string path) override;
	void	SetCullFace(FACE_CULLING state) override;
	void	ResetDriver();
    void	PopRT();

    void	Clear();
    void	SwapBuffers();

#ifdef USING_GL_ES
	bool	EGLInit();
	void	EGLCreateSurfaces();
	void	EGLDestroySurfaces();
	void	EGLResetDriver();
#endif

    bool	CheckExtension(std::string s);

#ifndef __APPLE__

	#ifdef USING_GL_ES
		EGLDisplay			eglDisplay;
		EGLConfig			eglConfig;
		EGLSurface			eglSurface;
		EGLContext			eglContext;

		#ifdef OS_WIN32
			EGLNativeWindowType	eglWindow;
		#elif defined(OS_ANDROID)
			ANativeWindow		*eglWindow;
		#endif

	#endif

#endif
    GLint				CurrentFBO;
#if defined(USING_OPENGL) || defined(USING_OPENGL_ES30) || defined(USING_OPENGL_ES31)
    static GLenum		DrawBuffers[16];
#endif
    std::vector<std::string>	ExtensionsTok;
    std::string					Extensions;

	bool				bInited;
  };
}

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>


namespace patch
{
	template < typename T > std::string to_string(const T& n)
	{
		std::ostringstream stm;
		stm << n;
		return stm.str();
	}
}


#endif
