
#include <stdlib.h>
#include "DexMemoryManager.h"




DexMemoryManager* DexMemoryManager::sGetMemoryManager()
{
	static DexMemoryManager* gMemoryManager = DexNull;
	if (gMemoryManager == DexNull)
		gMemoryManager = new DexMemoryManager;
	return gMemoryManager;
}

DexMemoryManager::DexMemoryManager()
{

}

DexMemoryManager::~DexMemoryManager()
{

}

char* DexMemoryManager::allocateMemory(DUInt32 size)
{
	return (char*)malloc(sizeof(char)*size);
}

void DexMemoryManager::deallocateMemory(void* ptr)
{
	free(ptr);
}
