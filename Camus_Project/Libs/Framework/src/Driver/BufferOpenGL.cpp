#include <Driver/BufferOpenGL.h>

#include <cstring>

namespace hyperspace {
	namespace video {
		// VBO
		CVertexBuffer::CVertexBuffer() : CBuffer_() {

		}

		void CVertexBuffer::CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS) {
			int gl_flags = 0;

			if (FLAGS & FLAGS_::DYNAMIC_BUFFER)
				gl_flags |= GL_DYNAMIC_DRAW;

			if (FLAGS & FLAGS_::STATIC_BUFFER)
				gl_flags |= GL_STATIC_DRAW;

			if (FLAGS & FLAGS_::STREAM_BUFFER)
				gl_flags |= GL_STREAM_DRAW;

			this->offset = offset;
			this->flags = FLAGS;
			this->size = size;

			glGenBuffers(1, &id);
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferData(GL_ARRAY_BUFFER, size, data, gl_flags);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glFlush();

			if (FLAGS & FLAGS_::SAVE_COPY_ON_RAM) {
				buffer = new char[size + 1];
				buffer[size] = '\0';
				std::memcpy(buffer, data, size);
			}
		}

		void CVertexBuffer::DestroyBuffer() {
			glDeleteBuffers(1, &id);
			if (flags & FLAGS_::SAVE_COPY_ON_RAM) {
				delete[] buffer;
				buffer = 0;
			}
			id = 0;
			offset = 0;
			flags = 0;
			size = 0;
		}

		void CVertexBuffer::UpdateBuffer(void *data, std::size_t size, unsigned int offset) {
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if (flags & FLAGS_::UPDATE_SAVED_COPY && flags & FLAGS_::SAVE_COPY_ON_RAM) {
				std::memcpy(buffer + offset, data, size);
			}
		}


		// IBO

		CIndexBuffer::CIndexBuffer() : CBuffer_() {

		}

		void CIndexBuffer::CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS) {
			int gl_flags = 0;

			if (FLAGS & FLAGS_::DYNAMIC_BUFFER)
				gl_flags |= GL_DYNAMIC_DRAW;

			if (FLAGS & FLAGS_::STATIC_BUFFER)
				gl_flags |= GL_STATIC_DRAW;

			if (FLAGS & FLAGS_::STREAM_BUFFER)
				gl_flags |= GL_STREAM_DRAW;

			this->offset = offset;
			this->flags = FLAGS;
			this->size = size;

			glGenBuffers(1, &id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, gl_flags);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glFlush();

			if (FLAGS & FLAGS_::SAVE_COPY_ON_RAM) {
				buffer = new char[size + 1];
				buffer[size] = '\0';
				std::memcpy(buffer, data, size);
			}
		}

		void CIndexBuffer::DestroyBuffer() {
			glDeleteBuffers(1, &id);
			if (flags & FLAGS_::SAVE_COPY_ON_RAM) {
				delete[] buffer;
				buffer = 0;
			}
			id = 0;
			offset = 0;
			flags = 0;
			size = 0;
		}

		void CIndexBuffer::UpdateBuffer(void *data, std::size_t size, unsigned int offset) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if (flags & FLAGS_::UPDATE_SAVED_COPY && flags & FLAGS_::SAVE_COPY_ON_RAM) {
				std::memcpy(buffer + offset, data, size);
			}
		}

	}
}