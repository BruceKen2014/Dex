
#include "DexModelSkinMeshTextLoader.h"
#include "..\DexBase\DexLog.h"
#include "..\DexBase\DexCommonFunction.h"

#define getFloat DexCommonFunction::str_to_float(vecStr[index++])
#define getInt   DexCommonFunction::str_to_int(vecStr[index++])
#define getColor DexCommonFunction::str_to_float(vecStr[index++]) * 255
DexModelSkinMeshTextLoader::DexModelSkinMeshTextLoader()
{

}

DexModelSkinMeshTextLoader::~DexModelSkinMeshTextLoader()
{

}

bool DexModelSkinMeshTextLoader::SupportType(const char* fileType)
{
	return dexstricmp(fileType, ".dexmodeltext") == 0;
}

DexModelBase* DexModelSkinMeshTextLoader::LoadModel(const char* filename, DInt32 flag)
{
	DexMem mem;
	DEX_ENSURE_P(mem.IniFromFile(filename));
	DInt64 Time = getTime()->GetTotalMillSeconds();
	DexLog::getSingleton()->LogLine(log_ok, "load dex text model %s...", filename);
	DexSkinMesh* pDexSkinMesh = new DexSkinMesh;
	const DInt16  iMaxLineByte = 2048;
	char tempLineData[iMaxLineByte];

	DVector<DexSkinMesh::stMeshVertex> vecVertexs;
	while (!mem.End())
	{
		mem.ReadLine(tempLineData);
		if (tempLineData[0] == '[')
		{
			char Type = tempLineData[1];
			if (Type == 'm')
			{
				if (tempLineData[2] == 'a')
					//materialInfo
					ReadMaterials(&mem, pDexSkinMesh, tempLineData);
				else if (tempLineData[2] == 'e')
					//meshInfo
					ReadMeshes(&mem, pDexSkinMesh, vecVertexs, tempLineData);
			}
			else if (Type == 't')
				ReadTextures(&mem, pDexSkinMesh, tempLineData);
			else if (Type == 'j')
				ReadJoints(&mem, pDexSkinMesh, tempLineData);
			else if (Type == 'v')
				ReadVertex(&mem, vecVertexs, tempLineData);
			else if (Type == 'r')
				ReadRenderFlag(&mem, pDexSkinMesh, tempLineData);
			else if (Type == 'l')
				ReadLightFlag(&mem, pDexSkinMesh, tempLineData);
		}
		else
			continue;
	}
	Time = getTime()->GetTotalMillSeconds() - Time;
	DexLog::getSingleton()->LogLine(log_ok, "load dex text model %s ok, use time %d ms", filename, Time);
	return pDexSkinMesh;
}
bool DexModelSkinMeshTextLoader::SaveModel(DexSkinMesh* pSkinMesh, const char* filename, DInt32 flag)
{
	return true;
}

void DexModelSkinMeshTextLoader::ReadMaterials(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer)
{
	//name diffuse ambient emissive specular power
	pMem->ReadLine(pBuffer); //read material count
	DInt32 materialCount = DexCommonFunction::str_to_int(pBuffer);
	for (int m = 0; m < materialCount; ++m)
	{
		pMem->ReadLine(pBuffer);
		DexMaterial material;
		DVector<DString> vecStr;
		DUDInt32 index = 0;
		DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
		strcpy_s(material.name, vecStr[index++].c_str());
		DexColor color;
		color.r = getColor; color.g = getColor; color.b = getColor; color.a = getColor;
		material.diffuse = color;
		color.r = getColor; color.g = getColor; color.b = getColor; color.a = getColor;
		material.ambient = color;
		color.r = getColor; color.g = getColor; color.b = getColor; color.a = getColor;
		material.emissive = color;
		color.r = getColor; color.g = getColor; color.b = getColor; color.a = getColor;
		material.specular = color;
		material.power = DexCommonFunction::str_to_float(vecStr[index++]);
		pSkinMesh->AddMaterial(material);
	}

}

void DexModelSkinMeshTextLoader::ReadTextures(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer)
{
	pMem->ReadLine(pBuffer); //read texture count
	DInt32 textureCount = DexCommonFunction::str_to_int(pBuffer);
	for (DInt32 t = 0; t < textureCount; ++t)
	{
		pMem->ReadLine(pBuffer);
		pSkinMesh->AddTexture(pBuffer);
	}
}

void DexModelSkinMeshTextLoader::ReadJoints(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer)
{
	//jointName fatherId Id offset scale axis radian
	pMem->ReadLine(pBuffer); //read material count
	DInt32 jointCount = DexCommonFunction::str_to_int(pBuffer);
	
	for (DInt32 j = 0; j < jointCount; ++j)
	{
		pMem->ReadLine(pBuffer);
		DUDInt32 index = 1;
		DVector<DString> vecStr;
		DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
		DInt32 fatherId = getInt;
		DInt32 jointId = getInt;
		DexVector3 translation; translation.x = getFloat; translation.y = getFloat; translation.z = getFloat;
		DexVector3 scale; scale.x = getFloat; scale.y = getFloat; scale.z = getFloat;
		DexVector3 axis; axis.x = getFloat; axis.y = getFloat; axis.z = getFloat;
		DFloat32 axisRadian = _getRadian(getFloat);
		DexQuaternion quatenion(axis, axisRadian);

		DexMatrix4x4 rotationMatrix = quatenion.GetMatrix();
		DexMatrix4x4 scaleMatrix; scaleMatrix.Scale(scale);
		DexMatrix4x4 TransMatrix; TransMatrix.Translate(translation);
		DexMatrix4x4 matrix = scaleMatrix * rotationMatrix * TransMatrix;

		DexSkinMesh::Joint* joint = pSkinMesh->AddJoint(jointId,fatherId,  matrix);
		joint->str_name = vecStr[0];
	}
}

void DexModelSkinMeshTextLoader::ReadVertex(DexMem* pMem, DVector<DexSkinMesh::stMeshVertex>& vertexs, char* pBuffer)
{
	pMem->ReadLine(pBuffer); //read vertex count
	DInt32 vertexCount = DexCommonFunction::str_to_int(pBuffer);
	for (DInt32 v = 0; v < vertexCount; ++v)
	{
		pMem->ReadLine(pBuffer);
		DUDInt32 index = 0;
		DVector<DString> vecStr;
		DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
		DexSkinMesh::stMeshVertex vertex;
		vertex.pos.x = getFloat; vertex.pos.y = getFloat; vertex.pos.z = getFloat;
		vertex.normal.x = getFloat; vertex.normal.y = getFloat; vertex.normal.z = getFloat; vertex.normal.Normalize();
		vertex.color.r = getColor; vertex.color.g = getColor; vertex.color.b = getColor; vertex.color.a = getColor;
		vertex.uv.x = getFloat; vertex.uv.y = getFloat;
		DInt32 jointCount = getInt;
		for (int j = 0; j < jointCount; ++j)
		{
			vertex.JointIndex[j] = getInt;
			vertex.JointWeights[j] = getFloat;
		}
		vertexs.push_back(vertex);
	}
}

void DexModelSkinMeshTextLoader::ReadMeshes(DexMem* pMem, DexSkinMesh* pSkinMesh, DVector<DexSkinMesh::stMeshVertex>& vertexs, char* pBuffer)
{
	pMem->ReadLine(pBuffer); //read mesh count
	DInt32 meshCount = DexCommonFunction::str_to_int(pBuffer);
	for (int m = 0; m < meshCount; ++m)
	{
		pMem->ReadLine(pBuffer);
		DUDInt32 index = 1;
		DVector<DString> vecStr;
		DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
		DInt32 meshId = getInt;
		DexSkinMesh::DexMesh* pMesh = pSkinMesh->AddMesh(meshId);
		strcpy_s(pMesh->name, vecStr[0].c_str());
		pMesh->m_iMaterialId = getInt;
		pMesh->m_iTextureId = getInt;
		DInt32 triangleCount = getInt;
		DInt32 vertexIndex = 0;
		DInt32 vertexIndice = 0;
		for (int t = 0; t < triangleCount; ++t)
		{
			for (int i = 0; i < 3; ++i)
			{
				vertexIndex = getInt;
				pMesh->m_vecVertexsBuffer.push_back(vertexs[vertexIndex]);
				pMesh->m_vecIndices.push_back(vertexIndice++);
			}
		}
	}
}

void DexModelSkinMeshTextLoader::ReadRenderFlag(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer)
{
	pMem->ReadLine(pBuffer); 
	DVector<DString> vecStr;
	DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
	DInt16 iRenderFlag = 0;
	for (size_t i = 0; i < vecStr.size(); ++i)
	{
		if (dexstricmp(vecStr[i].c_str(), "MESH") == 0)
			iRenderFlag |= SKINMESH_RENDER_MESH;
		else if (dexstricmp(vecStr[i].c_str(), "JOINT") == 0)
			iRenderFlag |= SKINMESH_RENDER_JOINT;
		else if (dexstricmp(vecStr[i].c_str(), "JOINT2JOINT") == 0)
			iRenderFlag |= SKINMESH_RENDER_JOINT2JOINT;
		else if (dexstricmp(vecStr[i].c_str(), "VERTEX2JOINT") == 0)
			iRenderFlag |= SKINMESH_RENDER_VERTEX2JOINT;
		else if (dexstricmp(vecStr[i].c_str(), "VERTEXNORMAL") == 0)
			iRenderFlag |= SKINMESH_RENDER_VERTEXNORMAL;
	}
	pSkinMesh->SetRenderFlag(iRenderFlag);
}

void DexModelSkinMeshTextLoader::ReadLightFlag(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer)
{
	pMem->ReadLine(pBuffer);
	DVector<DString> vecStr;
	DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
	DInt16 iLightFlag = 0;
	for (size_t i = 0; i < vecStr.size(); ++i)
	{
		if (dexstricmp(vecStr[i].c_str(), "ENABLE") == 0)
			iLightFlag |= DEXRENDER_LIGHT_ENABLE;
		else if (dexstricmp(vecStr[i].c_str(), "AMBIENT") == 0)
			iLightFlag |= DEXRENDER_LIGHT_AMBIENT;
		else if (dexstricmp(vecStr[i].c_str(), "POINT") == 0)
			iLightFlag |= DEXRENDER_LIGHT_POINT	;
		else if (dexstricmp(vecStr[i].c_str(), "DIRECTION") == 0)
			iLightFlag |= DEXRENDER_LIGHT_DIRECTION;
		else if (dexstricmp(vecStr[i].c_str(), "FOG") == 0)
			iLightFlag |= DEXRENDER_ENABLE_FOG;
	}
	pSkinMesh->SetLightFlag(iLightFlag);
}
#undef getFloat
#undef getInt
#undef getColor