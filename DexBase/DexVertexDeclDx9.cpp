

#include "DexVertexDeclDx9.h"

DexVertexDeclDx9::DexVertexDeclDx9(IDirect3DDevice9* device, D3DVERTEXELEMENT9* element) :m_pDecl(NULL)
{
	if (device == NULL || element == NULL)
		return;
	device->CreateVertexDeclaration(element, &m_pDecl);
}

DexVertexDeclDx9::~DexVertexDeclDx9()
{

}