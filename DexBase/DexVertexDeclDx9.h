

#ifndef _DEX_VERTEX_DECL_DX_9_H
#define _DEX_VERTEX_DECL_DX_9_H
#include <d3d9.h>
#include "IDexVertexDecl.h"

class DexVertexDeclDx9 :public IDexVertexDecl
{
public:
	IDirect3DVertexDeclaration9* m_pDecl;
public:
	DexVertexDeclDx9(IDirect3DDevice9* device, D3DVERTEXELEMENT9* element);
	virtual ~DexVertexDeclDx9();
};
#endif