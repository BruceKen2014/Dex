
#include "DexTextureDx9.h"
#include "..\state\DexGameEngine.h"
#include "DexDeviceDirectx9.h"

DexTextureDx9::DexTextureDx9()
{

}

DexTextureDx9::~DexTextureDx9()
{

}

bool DexTextureDx9::LoadTexture(DString filename, const DexColor& transcolor /*= DEXCOLOR_BLACK*/)
{
	name = filename;
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 d3d9Tex;
	LPDIRECT3DDEVICE9 d3d9Device = ((DexDeviceDirectx9*)(DexGameEngine::getEngine()->GetDDevice()))->GetD3D9Device();
	
	DexMem mem;
	mem.IniFromFile(filename.c_str());
	if (D3DXCreateTextureFromFileInMemoryEx(d3d9Device, mem.GetData(),mem.GetLength(),D3DX_DEFAULT_NONPOW2/*使用此参数才不会放大，使用0或者D3DX_DEFAULT图片会被放大一倍*/
		, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE, D3DX_FILTER_TRIANGLE,
		getD3DColor(transcolor), &info, NULL, &d3d9Tex) != D3D_OK)
	{
		//_Message("加载纹理%s失败!",filename);
		return false;
	}
	m_pData = (DInt8*)d3d9Tex;
	m_width = info.Width;
	m_height = info.Height;
	return true;
}

void DexTextureDx9::ShutDown()
{
	LPDIRECT3DTEXTURE9 d3d9Tex = (LPDIRECT3DTEXTURE9)m_pData;
	_SafeRelease(d3d9Tex);
}
