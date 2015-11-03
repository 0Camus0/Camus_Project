#include <Utils/MemoryTracker.h>

#if USE_MEMORY_TRACKER

int gHeapAllocUsed = 0;

int*	GetHeapAlloc() {
	return &gHeapAllocUsed;
}

int		GetTotalMemoryUsedBytes() {
	return TEX_STACK_ALLOCATED_SIZE + gHeapAllocUsed;
}

int		GetTotalMemoryUsedKb() {
	return (TEX_STACK_ALLOCATED_SIZE + gHeapAllocUsed) / 1024;
}

int		GetTotalMemoryUsedMB() {
	return (TEX_STACK_ALLOCATED_SIZE + gHeapAllocUsed) / (1024 * 1024);
}
#endif