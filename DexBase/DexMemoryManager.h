

#ifndef _DEX_MEMORY_MANAGER_H
#define _DEX_MEMORY_MANAGER_H

#include "DexType.h"
class DexMemoryManager
{
public:
	static DexMemoryManager* sGetMemoryManager();
public:
	char* allocateMemory(DUInt32 size);
	void  deallocateMemory(void* ptr);
private:
	DexMemoryManager();
	~DexMemoryManager();
};
#endif