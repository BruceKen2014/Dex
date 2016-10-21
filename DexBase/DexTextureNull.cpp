
#include "DexTextureNull.h"
#include "..\state\DexGameEngine.h"
#include "DexDeviceDirectx9.h"

DexTextureNull::DexTextureNull()
{

}

DexTextureNull::~DexTextureNull()
{

}

bool DexTextureNull::LoadTexture(DString filename, const DexColor& transcolor /*= DEXCOLOR_BLACK*/)
{
	name = filename;
	return false;
}

void DexTextureNull::ShutDown()
{
}
