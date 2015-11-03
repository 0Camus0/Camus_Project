#ifndef CA_BUFFER_H
#define CA_BUFFER_H

#include <Utils/MemoryTracker.h>
#include <cstddef>

namespace hyperspace {
	namespace video {
		struct CBuffer_ {
			enum FLAGS_ {
				READ_BUFFER = 1,
				WRITE_BUFFER = 2,
				STATIC_BUFFER = 4,
				DYNAMIC_BUFFER = 8,
				STREAM_BUFFER = 16,
				PERSISTENT_BUFFER = 32,
				SAVE_COPY_ON_RAM = 64,
				UPDATE_SAVED_COPY = 128,
				UNSIGNED_SHORT_INDICES = 256,
				UNSIGNED_INT_INDICES = 512
			};
			CBuffer_() :
				id(0),
				offset(0),
				flags(0),
				size(0)
			{
				MemAppendHeap(CBuffer_);
				buffer = 0;
			}
			virtual void CreateBuffer(void *data, std::size_t size, unsigned int offset, unsigned int FLAGS = 0) = 0;
			virtual void DestroyBuffer() = 0;
			virtual void UpdateBuffer(void *data, std::size_t size, unsigned int offset) = 0;


			unsigned int	id;
			unsigned int	offset;
			unsigned int	flags;

			std::size_t		 size;
			char			*buffer;
		};
	}
}

#endif