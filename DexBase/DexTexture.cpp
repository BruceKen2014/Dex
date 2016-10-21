


#include "DexTexture.h"
#include "DexLog.h"
#include "..\state\DexGameEngine.h"

DexTexture::DexTexture()
{
	m_pData = DexNull;
	m_width = 0;
	m_height = 0;
}

DexTexture::~DexTexture()
{

}

int DexTexture::GetWidth() const
{
	return m_width;
}
int DexTexture::GetHeight() const
{
	return m_height;
}
int8* DexTexture::GetData() 
{
	return m_pData; 
}
DString DexTexture::Name() const
{
	return name;
}

DString DexTexture::FileName()
{
	std::vector<std::string> out;
	SplitStr(name, '/', out);
	if (out.size() == 0)
		return "";
	return out[out.size() - 1];
}
