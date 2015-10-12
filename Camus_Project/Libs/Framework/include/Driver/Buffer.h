#ifndef CA_BUFFER_H
#define CA_BUFFER_H

#include <cstddef>

namespace hyperspace {

	struct CBuffer_ {
		enum FLAGS_ {
			READ_BUFFER			= 1,
			WRITE_BUFFER		= 2,
			STATIC_BUFFER		= 4,
			DYNAMIC_BUFFER		= 8,
			STREAM_BUFFER		= 16,
			PERSISTENT_BUFFER	= 32,
			SAVE_COPY_ON_RAM	= 64,
		};
		CBuffer_() : 
			id(0),
			offset(0),
			flags(0),
			size(0)
		{
			buffer = 0;
		}
		virtual void CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS = 0) = 0;
		virtual void DestroyBuffer() = 0;
		virtual void UpdateBuffer(void *data, std::size_t size, bool discardOld = true) = 0;
		virtual void UpdateSubBuffer(void *data, std::size_t size, unsigned int offset) = 0;

		unsigned int	id;
		unsigned int	offset;
		unsigned int	flags;

		std::size_t		 size;
		char			*buffer;
	};
}

#endif