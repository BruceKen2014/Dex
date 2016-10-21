
#include "../DexBase/DexTexture.h"
#include "DexModelSkinMeshLoader.h"
#include "..\state\DexGameEngine.h"
#include "DexSkinMesh.h"
#include "DexSkinMeshFF.h"
#include "..\DexBase\DexLog.h"
DexModelSkinMeshLoader::DexModelSkinMeshLoader()
{

}

DexModelSkinMeshLoader::~DexModelSkinMeshLoader()
{

}

bool DexModelSkinMeshLoader::SupportType(const char* fileType)
{
	return dexstricmp(fileType, ".dexmodel") == 0;
}

DexModelBase* DexModelSkinMeshLoader::LoadModel(const char* filename, int32 flag)
{
	getLog()->LogLine(log_ok, "load dex model %s...\n", filename);
	int64 Time = getTime()->GetTotalMillSeconds();
	DexMem& mem = DexGameEngine::getEngine()->getMem();
	mem.IniFromFile(filename);
	DexSkinMesh* pNewSkinMesh = DexNull;
	if (flag == 1)
		pNewSkinMesh = new DexSkinMeshFF;
	else
		pNewSkinMesh = new DexSkinMesh;
	mem.BeginRead();
	//ambient color
	mem >> pNewSkinMesh->m_ambientColor;
	//render flag
	mem >> pNewSkinMesh->m_iRenderFlag;
	mem >> pNewSkinMesh->m_iLightFlag;
	//animate time
	mem >> pNewSkinMesh->m_iAnimateMaxTime;
	mem >> pNewSkinMesh->m_iAnimateStartTime;
	mem >> pNewSkinMesh->m_iAnimateEndTime;
	//mesh
	ReadMesh(pNewSkinMesh, mem);
	//joint
	mem >> pNewSkinMesh->m_fJointScale;
	ReadJoint(pNewSkinMesh, mem);
	mem << pNewSkinMesh->m_iAnimateMaxTime;
	mem << pNewSkinMesh->m_iAnimateStartTime;
	mem << pNewSkinMesh->m_iAnimateEndTime;
	//material
	ReadMaterial(pNewSkinMesh, mem);

	if (pNewSkinMesh == nullptr)
		getLog()->LogLine(log_error, "load dex model can not find mesh !");
	else
	{
		Time = getTime()->GetTotalMillSeconds() - Time;
		getLog()->LogLine(log_ok, "load dex model %s ok, use time %d ms", filename, Time);
	}
	return pNewSkinMesh;
}

bool DexModelSkinMeshLoader::SaveModel(DexSkinMesh* pSkinMesh, const char* filename, int32 flag)
{
	DexMem& mem = DexGameEngine::getEngine()->getMem();
	mem.BeginWrite();
	//ambient color
	mem << pSkinMesh->m_ambientColor;
	//render flag
	mem << pSkinMesh->m_iRenderFlag;
	mem << pSkinMesh->m_iLightFlag;
	//animate time
	mem << pSkinMesh->m_iAnimateMaxTime;
	mem << pSkinMesh->m_iAnimateStartTime;
	mem << pSkinMesh->m_iAnimateEndTime;
	//mesh
	WriteMesh(pSkinMesh, mem);
	//joint
	mem << pSkinMesh->m_fJointScale;
	mem << pSkinMesh->m_vecJoints.size();
	WriteJoint(pSkinMesh->m_pRootJoint, mem);
	mem << pSkinMesh->m_iAnimateMaxTime;
	mem << pSkinMesh->m_iAnimateStartTime;
	mem << pSkinMesh->m_iAnimateEndTime;
	//material
	WriteMaterial(pSkinMesh, mem);
	mem.SaveToFile(filename);

	return true;
}

void DexModelSkinMeshLoader::WriteMesh(DexSkinMesh* pSkinMesh, DexMem& mem)
{
	mem << pSkinMesh->m_vecMeshs.size();
	for (size_t i = 0; i < pSkinMesh->m_vecMeshs.size(); ++i)
	{
		DexSkinMesh::DexMesh* pMesh = pSkinMesh->m_vecMeshs[i];
		mem.Write(pMesh->name, sizeof(pMesh->name));
		mem << pMesh->id;
		mem << pMesh->m_iTextureId;
		mem << pMesh->m_iMaterialId;
		//vertex indice
		int temp = pMesh->m_vecIndices.size();
		mem << temp;
		if (temp != 0)
			mem.Write(&pMesh->m_vecIndices[0], temp * sizeof(pMesh->m_vecIndices[0]));
		//vertex buffer
		temp = pMesh->m_vecVertexsBuffer.size();
		mem << temp;
		if (temp != 0)
			mem.Write(&pMesh->m_vecVertexsBuffer[0], temp * sizeof(pMesh->m_vecVertexsBuffer[0]));
		/*
		以下是调试数据，如果需要，可以由程序动态生成
		//line indice
		temp = pMesh->m_vecLineIndices.size();
		mem << temp;
		if (temp != 0)
			mem.Write(&pMesh->m_vecLineIndices[0], temp * sizeof(pMesh->m_vecLineIndices[0]));
		//debug normal
		temp = pMesh->m_vecDebugNormalIndices.size();
		mem << temp;
		if (temp != 0)
			mem.Write(&pMesh->m_vecDebugNormalIndices[0], temp * sizeof(pMesh->m_vecDebugNormalIndices[0]));
		//debug normal buffer
		temp = pMesh->m_vecDebugNormalBuffer.size();
		mem << temp;
		if (temp != 0)
			mem.Write(&pMesh->m_vecDebugNormalBuffer[0], temp * sizeof(pMesh->m_vecDebugNormalBuffer[0]));
		//debug vertex joint
		temp = pMesh->m_vecDebugVertexToJointIndices.size();
		mem << temp;
		if (temp != 0)
			mem.Write(&pMesh->m_vecDebugVertexToJointIndices[0], temp * sizeof(pMesh->m_vecDebugVertexToJointIndices[0]));
		//debug vertex joint buffer
		temp = pMesh->m_vecDebugVertexToJointBuffer.size();
		mem << temp;
		if (temp != 0)
			mem.Write(&pMesh->m_vecDebugVertexToJointBuffer[0], temp * sizeof(pMesh->m_vecDebugVertexToJointBuffer[0]));
		*/
	}
}
void DexModelSkinMeshLoader::ReadMesh(DexSkinMesh* pSkinMesh, DexMem& mem)
{
	int meshCount = 0;
	mem >> meshCount;
	pSkinMesh->m_vecMeshs.resize(meshCount);
	for (int i = 0; i < meshCount; ++i)
	{
		DexSkinMesh::DexMesh* pMesh = new DexSkinMesh::DexMesh;
		pSkinMesh->m_vecMeshs[i] = pMesh;
		mem.Read(pMesh->name, sizeof(pMesh->name));
		mem >> pMesh->id;
		mem >> pMesh->m_iTextureId;
		mem >> pMesh->m_iMaterialId;
		int temp = 0;
		//vertex indice
		mem >> temp;
		pMesh->m_vecIndices.resize(temp);
		if (temp != 0)
			mem.Read(&pMesh->m_vecIndices[0], sizeof(pMesh->m_vecIndices[0]) * temp);
		//vertex buffer
		mem >> temp;
		pMesh->m_vecVertexsBuffer.resize(temp);
		if (temp != 0)
			mem.Read(&pMesh->m_vecVertexsBuffer[0], sizeof(pMesh->m_vecVertexsBuffer[0]) * temp);
		/*
		//line indice
		mem >> temp;
		pMesh->m_vecLineIndices.resize(temp);
		if (temp != 0)
			mem.Read(&pMesh->m_vecLineIndices[0], sizeof(pMesh->m_vecLineIndices[0]) * temp);
		//debug normal
		mem >> temp;
		pMesh->m_vecDebugNormalIndices.resize(temp);
		if (temp != 0)
			mem.Read(&pMesh->m_vecDebugNormalIndices[0], sizeof(pMesh->m_vecDebugNormalIndices[0]) * temp);
		//debug normal buffer
		mem >> temp;
		pMesh->m_vecDebugNormalBuffer.resize(temp);
		if (temp != 0)
			mem.Read(&pMesh->m_vecDebugNormalBuffer[0], sizeof(pMesh->m_vecDebugNormalBuffer[0]) * temp);
		//debugg vertex to joint
		mem >> temp;
		pMesh->m_vecDebugVertexToJointIndices.resize(temp);
		if (temp != 0)
			mem.Read(&pMesh->m_vecDebugVertexToJointIndices[0], sizeof(pMesh->m_vecDebugVertexToJointIndices[0]) * temp);
		//debugg vertex to joint buffer
		mem >> temp;
		pMesh->m_vecDebugVertexToJointBuffer.resize(temp);
		if (temp != 0)
			mem.Read(&pMesh->m_vecDebugVertexToJointBuffer[0], sizeof(pMesh->m_vecDebugVertexToJointBuffer[0]) * temp);
			*/
	}
}

void DexModelSkinMeshLoader::WriteJoint(DexSkinMesh::Joint* pJoint, DexMem& mem)
{
	mem.Write((void*)pJoint->str_name.c_str(), 128);
	mem << pJoint->id;
	if (pJoint->m_pFather != DexNull)
		mem << pJoint->m_pFather->id;
	else
		mem << (int16)-1;
	mem << pJoint->frame_matrix;
	int32 keyFrames = pJoint->m_vecKeyFrames.size();
	mem << keyFrames;
	if (keyFrames != 0)
		mem.Write(&pJoint->m_vecKeyFrames[0], keyFrames * sizeof(pJoint->m_vecKeyFrames[0]));
	for (size_t i = 0; i < pJoint->m_vecChildren.size(); ++i)
	{
		WriteJoint(pJoint->m_vecChildren[i], mem);
	}

}

void DexModelSkinMeshLoader::ReadJoint(DexSkinMesh* pSkinMesh, DexMem& mem)
{
	int JointCount = 0;
	mem >> JointCount;
	char JointName[128];
	int16  JointId = 0;
	int16  JointFatherId = 0;
	int32  keyFrames = 0;
	DexMatrix4x4 JointFrameMatrix;
	for (int i = 0; i < JointCount; ++i)
	{
		mem.Read(JointName, 128);
		mem >> JointId;
		mem >> JointFatherId;
		mem >> JointFrameMatrix;
		mem >> keyFrames;
		DexSkinMesh::Joint* pJoint =  pSkinMesh->AddJoint(JointId, JointFatherId, JointFrameMatrix);
		if (pJoint != DexNull)
			pJoint->str_name = JointName;
		if (keyFrames != 0)
		{
			pJoint->m_vecKeyFrames.resize(keyFrames);
			mem.Read(&pJoint->m_vecKeyFrames[0], keyFrames * sizeof(pJoint->m_vecKeyFrames[0]));
		}
	}
}

void DexModelSkinMeshLoader::WriteMaterial(DexSkinMesh* pSkinMesh, DexMem& mem)
{
	int32 materialCount = pSkinMesh->m_vecMaterials.size();
	mem << materialCount;
	if (materialCount != 0)
		mem.Write(&pSkinMesh->m_vecMaterials[0], materialCount * sizeof(pSkinMesh->m_vecMaterials[0]));
	int32 textureCount = pSkinMesh->m_vecTextures.size();
	mem << textureCount;
	for (int i = 0; i < textureCount; ++i)
		mem.Write((void*)pSkinMesh->m_vecTextures[i]->Name().c_str(), 128);

}

void DexModelSkinMeshLoader::ReadMaterial(DexSkinMesh* pSkinMesh, DexMem& mem)
{
	int32 materialCount = 0;
	int32 textureCount = 0;
	mem >> materialCount;
	pSkinMesh->m_vecMaterials.resize(materialCount);
	if (materialCount != 0)
		mem.Read(&pSkinMesh->m_vecMaterials[0], materialCount * sizeof(pSkinMesh->m_vecMaterials[0]));

	mem >> textureCount;
	char textureName[128];
	for (int i = 0; i < textureCount; ++i)
	{
		mem.Read(textureName, 128);
		pSkinMesh->AddTexture(textureName);
	}
}




