
#include "../state/DexGameEngine.h"
#include "DexRenderDirectx9.h"
#include "../DexModel/DexShaderHlslSkinMesh.h"
#include "../DexModel/DexShaderHlslSkinMeshVertexToJoint.h"
#include "../DexModel/DexShaderHlslSkinMeshVertexNormal.h"


DexRenderDirectX9::DexRenderDirectX9()
{
}

DexRenderDirectX9::~DexRenderDirectX9()
{
}

bool DexRenderDirectX9::InitShader()
{
	IDexShader* shader = nullptr;
	shader = new DexShaderHlslSkinMesh;
	m_mapShaders[DexShaderHlslSkinMesh::getClassType()] = shader;
	shader = new DexShaderHlslSkinMeshVertexToJoint;
	m_mapShaders[DexShaderHlslSkinMeshVertexToJoint::getClassType()] = shader;
	shader = new DexShaderHlslSkinMeshVertexNormal;
	m_mapShaders[DexShaderHlslSkinMeshVertexNormal::getClassType()] = shader;
	return true;

}
void DexRenderDirectX9::DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 stridesize)
{
	D3DPRIMITIVETYPE d3d_primitive_type = D3DPT_LINELIST;
	switch (type)
	{
	case DexPT_POINTLIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_POINTLIST; break; }
	case DexPT_LINELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINELIST; break; }
	case DexPT_LINESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINESTRIP; break; }
	case DexPT_TRIANGLELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST; break; }
	case DexPT_TRIANGLESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP; break; }
	case DexPT_TRIANGLEFAN:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLEFAN; break; }
	default:
		break;
	}
	//DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertexs, sizeof(stVertex0));
	//这里的D3DFMT_INDEX32，如果indices是int32整型数组的话，设置D3DFMT_INDEX32,如果是int16类型的整型数组的话，那么设置D3DFMT_INDEX16
	DexGameEngine::getEngine()->GetDevice()->DrawIndexedPrimitiveUP(d3d_primitive_type, 0, vertexCount, primitiveCount, indices, D3DFMT_INDEX32, vertexs, stridesize);
}

void DexRenderDirectX9::DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 fvf, int32 stridesize)
{
	D3DPRIMITIVETYPE d3d_primitive_type = D3DPT_LINELIST;
	switch (type)
	{
	case DexPT_POINTLIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_POINTLIST; break; }
	case DexPT_LINELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINELIST; break; }
	case DexPT_LINESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_LINESTRIP; break; }
	case DexPT_TRIANGLELIST:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST; break; }
	case DexPT_TRIANGLESTRIP:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLESTRIP; break; }
	case DexPT_TRIANGLEFAN:{d3d_primitive_type = D3DPRIMITIVETYPE::D3DPT_TRIANGLEFAN; break; }
	default:
		break;
	}
	DexGameEngine::getEngine()->GetDevice()->SetFVF((DWORD)fvf);
	//DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertexs, sizeof(stVertex0));
	//这里的D3DFMT_INDEX32，如果indices是int32整型数组的话，设置D3DFMT_INDEX32,如果是int16类型的整型数组的话，那么设置D3DFMT_INDEX16
	DexGameEngine::getEngine()->GetDevice()->DrawIndexedPrimitiveUP(d3d_primitive_type, 0, vertexCount, primitiveCount, indices, D3DFMT_INDEX32, vertexs, stridesize);
}