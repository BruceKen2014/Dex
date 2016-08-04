

#include "DexSkinMeshFF.h"
#include "..\state\DexGameEngine.h"

DexSkinMeshFF::DexSkinMeshFF()
{
	m_mwsRootJoint = nullptr;
}
DexSkinMeshFF::~DexSkinMeshFF()
{
	_SafeClearVector(m_vecMwsJoints);
}

DexSkinMesh::Joint* DexSkinMeshFF::FindMwsJoint(int32 index)
{
	for (size_t i = 0; i < m_vecMwsJoints.size(); ++i)
		if (m_vecMwsJoints[i]->id == index)
			return m_vecMwsJoints[i];
	return nullptr;
}

DexSkinMesh::Joint* DexSkinMeshFF::FindMwsJoint(DString name)
{
	for (size_t i = 0; i < m_vecMwsJoints.size(); ++i)
		if (m_vecMwsJoints[i]->str_name == name)
			return m_vecMwsJoints[i];
	return nullptr;
}

DexSkinMesh::Joint* DexSkinMeshFF::AddMwsJoint(DString name, int32 index, DexSkinMesh::Joint* pFather, DexMatrix4x4 father_matrix)
{
	Joint* pJoint = new Joint;
	pJoint->str_name = name;
	pJoint->id = index;
	if (pFather != nullptr)
		pFather->AddChild(pJoint);
	else
	{//mws root joint
		m_mwsRootJoint = pJoint;
	}
	pJoint->frame_matrix = father_matrix;//设置默认第一帧的矩阵
	if (pFather != nullptr)
		pJoint->meshMatrix = pJoint->frame_matrix * pFather->world_matrix;
	else
		pJoint->meshMatrix = pJoint->frame_matrix;
	pJoint->world_matrix = pJoint->meshMatrix;
	pJoint->localMeshMatrixInvert = pJoint->meshMatrix.GetInvert();
	m_vecMwsJoints.push_back(pJoint);
	
	stFFJoint ffJoint;
	ffJoint.pDaeJoint = FindJoint(name);
	if (ffJoint.pDaeJoint != nullptr)
		ffJoint.pDaeJoint->localMeshMatrixInvert = pJoint->localMeshMatrixInvert;
	ffJoint.pMwsJoint = pJoint;
	m_mapJoints[name] = ffJoint;

	return pJoint;
}

bool DexSkinMeshFF::Update(int32 delta)
{
	delta = m_fAnimateRatio * delta;
	DEX_ENSURE_B(DexModelBase::Update(delta));
	switch (GetAnimateType())
	{
	case SkinMeshAnimateType_OneTime:
	{
		UpdateOneTime(delta);
		break;
	}
	case SkinMeshAnimateType_Loop:
	{
		UpdateLoop(delta);
		break;
	}
	case SkinMeshAnimateType_OneTime_Back:
	{
		UpdateOneTimeBack(delta);
		break;
	}
	case SkinMeshAnimateType_Loop_Back:
	{
		UpdateLoopBack(delta);
		break;
	}
	default:
		UpdateOneTime(delta);
	}
	D3DXMATRIX matWorld, matView, matProj;
	DexMatrix4x4 worldMatrix;
	memcpy(&matWorld, &worldMatrix, sizeof(DexMatrix4x4));
	DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_PROJECTION, &matProj);
	matWVP = matWorld * matView * matProj;

	
	if (m_mwsRootJoint != nullptr)
		m_mwsRootJoint->Update(m_iAnimateNowTime);
	for (DMap<DString, stFFJoint>::iterator ite = m_mapJoints.begin(); ite != m_mapJoints.end(); ++ite)
	{
		if (ite->second.pDaeJoint != nullptr)
			ite->second.pDaeJoint->world_matrix = ite->second.pMwsJoint->world_matrix *bindMatrix;
	}

	memset(jointsMatrix, 0, sizeof(D3DXMATRIX)* sGetMaxJointCount());
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		jointsMatrix[i] = *(D3DXMATRIX*)&(m_vecJoints[i]->localMeshMatrixInvert) * (*(D3DXMATRIX*)&(m_vecJoints[i]->world_matrix));
	}
	return true;
}

bool DexSkinMeshFF::Render()
{
	DEX_ENSURE_B(DexSkinMesh::Render());

	DexMatrix4x4 jointScaleMatrix;
	jointScaleMatrix.Scale(DexVector3(m_fJointScale, m_fJointScale, m_fJointScale));
	//for (size_t i = 0; i < m_vecMwsJoints.size(); ++i)
		//DexGameEngine::getEngine()->RenderCube(jointScaleMatrix * m_vecMwsJoints[i]->world_matrix);

	//if (m_mwsRootJoint != nullptr)
		//m_mwsRootJoint->Render2Joint();

	return true;
}