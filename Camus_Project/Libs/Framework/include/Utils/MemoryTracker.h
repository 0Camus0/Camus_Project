#ifndef CA_MEM_TRAKCER_H
#define CA_MEM_TRAKCER_H

#include <config.h>

#if USE_MEMORY_TRACKER

#include <stdio.h>

template<typename T>
void	AppendHeapAlloc(T size) {
	*GetHeapAlloc() += static_cast<int>(size);
}

int		GetTotalMemoryUsedBytes();
int		GetTotalMemoryUsedKb();
int		GetTotalMemoryUsedMB();
int*	GetHeapAlloc();

#define MemAppendHeap(x) AppendHeapAlloc(sizeof(x))
#define GetTotalMem() GetTotalMemoryUsedMB()
#define GetStackMem() TEX_STACK_ALLOCATED_SIZE
#define GetHeapMem() *GetHeapAlloc()
#else
#define MemAppendHeap(A) ((void)0)
#define GetTotalMem() 0
#define GetStackMem() 0
#define GetHeapMem() 0
#endif





#endif