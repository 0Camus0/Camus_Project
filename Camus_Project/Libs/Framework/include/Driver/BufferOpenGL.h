#ifndef CA_GL_BUFFER_H
#define CA_GL_BUFFER_H

#include <Driver/Buffer.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace hyperspace {
	class CVertexBuffer : public hyperspace::CBuffer_ {
		public:
			CVertexBuffer();
			void CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS = 0);
			void DestroyBuffer();
			void UpdateBuffer(void *data, std::size_t size, bool discardOld = true);
			void UpdateSubBuffer(void *data, std::size_t size, unsigned int offset);
	};

	class CIndexBuffer : public hyperspace::CBuffer_ {
		public:
			CIndexBuffer();
			void CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS = 0);
			void DestroyBuffer();
			void UpdateBuffer(void *data, std::size_t size, bool discardOld = true);
			void UpdateSubBuffer(void *data, std::size_t size, unsigned int offset);
	};
}

#endif