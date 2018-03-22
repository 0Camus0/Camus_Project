#ifndef CA_GL_BUFFER_H
#define CA_GL_BUFFER_H

#include <Driver\Buffer.h>
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
namespace hyperspace {
	namespace video {
		class VertexBufferGL : public CBuffer_ {
		public:
			VertexBufferGL();
			void CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS = 0);
			void DestroyBuffer();
			void UpdateBuffer(void *data, std::size_t size, unsigned int offset);
		};

		class IndexBufferGL : public CBuffer_ {
		public:
			IndexBufferGL();
			void CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS = 0);
			void DestroyBuffer();
			void UpdateBuffer(void *data, std::size_t size, unsigned int offset);
		};
	}
}

#endif