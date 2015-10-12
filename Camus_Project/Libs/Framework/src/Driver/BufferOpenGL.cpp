#include <Driver/BufferOpenGL.h>

#include <cstring>

namespace hyperspace {

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

void CVertexBuffer::DestroyBuffer(){

}
	
void CVertexBuffer::UpdateBuffer(void *data, std::size_t size, bool discardOld) {


}
	
void CVertexBuffer::UpdateSubBuffer(void *data, std::size_t size, unsigned int offset) {

}

	// IBO

CIndexBuffer::CIndexBuffer() : CBuffer_() {

}

void CIndexBuffer::CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS) {

}

void CIndexBuffer::DestroyBuffer() {

}

void CIndexBuffer::UpdateBuffer(void *data, std::size_t size, bool discardOld) {


}

void CIndexBuffer::UpdateSubBuffer(void *data, std::size_t size, unsigned int offset) {

}


}