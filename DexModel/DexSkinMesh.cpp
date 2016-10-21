

#include "DexBase/DexLog.h"
#include "DexBase/DexRenderDirectx9.h"
#include "DexBase/DexVertexDeclDx9.h"
#include "DexMath/DexVector4.h"
#include "../Source/CTexture.h"
#include "../state/DexGameEngine.h"
#include "../DexBase/DexTextureManager.h"
#include "../DexMath/DexMath2.h"
#include "DexSkinMesh.h"
#include "../DexBase/DexTexture.h"

DexSkinMesh::Joint::Joint() :m_bAnimate(true)
{
	id = 0;
	str_name = "default";
	m_pFather = NULL;
	m_vecKeyFrames.clear();
	m_vecChildren.clear();
}
DexSkinMesh::Joint::~Joint()
{
}
void DexSkinMesh::Joint::AddChild(Joint* child)
{
	m_vecChildren.push_back(child);
	child->m_pFather = this;
}
//先按照时间从小到大插入
void DexSkinMesh::Joint::AddKeyFrame(int32 time, const DexMatrix4x4& matrix)
{
	m_vecKeyFrames.push_back(matrix_key(time, matrix));
}
void DexSkinMesh::Joint::Render()
{
	DexGameEngine::getEngine()->RenderCube(world_matrix);
	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->Render();
	}

}
void DexSkinMesh::Joint::Render2Joint()
{
	if (m_pFather != NULL)
	{
		DexGameEngine::getEngine()->Render3DLine(m_pFather->world_matrix.GetPosition(), world_matrix.GetPosition(), DexColor(0.0f, 1.0f, 1.0f), DexColor(0.0f, 1.0f, 1.0f));
	}
	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->Render2Joint();
	}
}
void DexSkinMesh::Joint::Update(int time)
{
	ComputeWorldMatrix(time);
	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->Update(time);
	}
}
void DexSkinMesh::Joint::ComputeWorldMatrix(int32 time)
{
	if (m_bAnimate)
	{
		if (m_vecKeyFrames.size() > 0)
		{
			if (m_vecKeyFrames.size() == 1)
			{
				frame_matrix = m_vecKeyFrames[0].matrix;
			}
			else
			{
				float time1;
				float time2;
				DexMatrix4x4 matrix1;
				DexMatrix4x4 matrix2;
				bool  bJustEqual = false;
				for (size_t i = 0; i < m_vecKeyFrames.size(); ++i)
				{
					if (time == m_vecKeyFrames[i].time)
					{
						matrix1 = m_vecKeyFrames[i].matrix;
						bJustEqual = true;
						break;
					}
					if (time > m_vecKeyFrames[i].time)
					{
						time1 = m_vecKeyFrames[i].time;
						matrix1 = m_vecKeyFrames[i].matrix;
						continue;
					}
					else
					{
						time2 = m_vecKeyFrames[i].time;
						matrix2 = m_vecKeyFrames[i].matrix;
						break;
					}
				}
				if (bJustEqual)
					frame_matrix = matrix1;
				else
					frame_matrix = matrix1 + (matrix2 - matrix1) * ((float32)(time - time1)) / ((float32)(time2 - time1));
			}
		}
	}
	if (m_pFather != NULL)
	{
		world_matrix = frame_matrix * m_pFather->world_matrix;
	}
	else
		world_matrix = frame_matrix;
}
DexSkinMesh::DexMesh::DexMesh()
{
	id = 0;
	memset(name, 0, sizeof(name));
	m_iTextureId = -1;
	m_iMaterialId = -1;
	
}
DexSkinMesh::DexMesh::~DexMesh()
{
	m_vecIndices.clear();
	m_vecLineIndices.clear();
	m_vecVertexsBuffer.clear();
}

void DexSkinMesh::DexMesh::DestroyLineIndices()
{
	m_vecLineIndices.clear();
}
int32* DexSkinMesh::DexMesh::CreateLineIndices()
{
	m_vecLineIndices.clear();
	int lineCount = m_vecIndices.size() * 2;
	int32 line_indice_index = 0;
	for (int i = 0; i + 2 < m_vecIndices.size();)
	{//3个顶点为一个三角形
		m_vecLineIndices.push_back(m_vecIndices[i]);
		m_vecLineIndices.push_back(m_vecIndices[i + 1]);
		m_vecLineIndices.push_back(m_vecIndices[i]);
		m_vecLineIndices.push_back(m_vecIndices[i + 2]);
		m_vecLineIndices.push_back(m_vecIndices[i + 1]);
		m_vecLineIndices.push_back(m_vecIndices[i + 2]);
		i = i + 3;
	}
	DEX_ENSURE_P(m_vecLineIndices.size() != 0);
	return (int32*)&m_vecLineIndices[0];
}

void DexSkinMesh::DexMesh::CreateNormalBuffer()
{
	stMeshVertex temp;
	for (size_t i = 0; i < m_vecVertexsBuffer.size(); ++i)
	{
		temp = m_vecVertexsBuffer[i];
		temp.color = DEXCOLOR_YELLOW;
		m_vecDebugNormalIndices.push_back(m_vecDebugNormalBuffer.size());
		m_vecDebugNormalBuffer.push_back(temp);
		temp.pos = temp.pos + temp.normal * 1.0f;
		m_vecDebugNormalIndices.push_back(m_vecDebugNormalBuffer.size());
		m_vecDebugNormalBuffer.push_back(temp);
	}
}

void DexSkinMesh::DexMesh::DestroyNormalBuffer()
{
	m_vecDebugNormalBuffer.clear();
	m_vecDebugNormalIndices.clear();
}
uint32 DexSkinMesh::DexMesh::GetNormalBufferCount() const
{
	return m_vecDebugNormalBuffer.size();
}

void* DexSkinMesh::DexMesh::GetNormalBuffer()
{
	DEX_ENSURE_P(m_vecDebugNormalBuffer.size() != 0);
	return (void*)&m_vecDebugNormalBuffer[0];
}
uint32 DexSkinMesh::DexMesh::GetNormalIndiceCount() const
{
	return m_vecDebugNormalIndices.size();
}

void* DexSkinMesh::DexMesh::GetNormalIndiceBuffer()
{
	DEX_ENSURE_P(m_vecDebugNormalIndices.size() != 0);
	return (void*)&m_vecDebugNormalIndices[0];
}

void DexSkinMesh::DexMesh::CreateVertexToJointBuffer(const DVector<Joint*>& vecJoints)
{
	uint32 iVertexSize = m_vecVertexsBuffer.size();
	for (size_t i = 0; i < iVertexSize; ++i)
	{
		stMeshVertex temp;
		temp = m_vecVertexsBuffer[i];
		temp.color = DEXCOLOR_GREEN;
		m_vecDebugVertexToJointBuffer.push_back(temp);
	}
	//m_vecDebugVertexToJointBuffer.resize(iVertexSize);
	//memcpy(&m_vecDebugVertexToJointBuffer[0], &m_vecVertexsBuffer[0], iVertexSize * sizeof(stMeshVertex));
	for (size_t i = 0; i < vecJoints.size(); ++i)
	{
		stMeshVertex temp;
		temp.color = DEXCOLOR_GREEN;
		temp.pos = vecJoints[i]->meshMatrix.GetPosition();
		temp.JointIndex[0] = i;
		temp.JointWeights[0] = 1.0f;
		m_vecDebugVertexToJointBuffer.push_back(temp);
	}
	for (size_t i = 0; i < iVertexSize; ++i)
	{
		for (size_t j = 0; j < SKINMESH_VERTEX_JOINT_COUNT; ++j)
		{
			if (!DexMath::Equal(m_vecVertexsBuffer[i].JointWeights[j], 0.0f))
			{
				m_vecDebugVertexToJointIndices.push_back(i);
				m_vecDebugVertexToJointIndices.push_back(iVertexSize + m_vecVertexsBuffer[i].JointIndex[j]);
			}
		}
	}
}

void DexSkinMesh::DexMesh::DestroyVertexToJointlBuffer()
{
	m_vecDebugVertexToJointBuffer.clear();
	m_vecDebugVertexToJointIndices.clear();
}
uint32 DexSkinMesh::DexMesh::GetVertexToJointBufferCount() const
{
	return m_vecDebugVertexToJointBuffer.size();
}

void* DexSkinMesh::DexMesh::GetVertexToJointBuffer()
{
	DEX_ENSURE_P(m_vecDebugVertexToJointBuffer.size() != 0);
	return (void*)&m_vecDebugVertexToJointBuffer[0];
}
uint32 DexSkinMesh::DexMesh::GetVertexToJointIndiceCount() const
{
	return m_vecDebugVertexToJointIndices.size();
}

void* DexSkinMesh::DexMesh::GetVertexToJointIndiceBuffer()
{
	DEX_ENSURE_P(m_vecDebugVertexToJointIndices.size() != 0);
	return (void*)&m_vecDebugVertexToJointIndices[0];
}

uint32 DexSkinMesh::DexMesh::GetVertexCount() const
{
	return m_vecVertexsBuffer.size();
}

void* DexSkinMesh::DexMesh::GetVertexBuffer()
{
	DEX_ENSURE_P(m_vecVertexsBuffer.size() != 0);
	return (void*)&m_vecVertexsBuffer[0];
}
uint32 DexSkinMesh::DexMesh::GetIndiceCount() const
{
	return m_vecIndices.size();
}

void* DexSkinMesh::DexMesh::GetIndiceBuffer()
{
	DEX_ENSURE_P(m_vecIndices.size() != 0);
	return (void*)&m_vecIndices[0];
}
uint32 DexSkinMesh::DexMesh::GetLineIndiceCount() const
{
	return m_vecLineIndices.size();
}

void* DexSkinMesh::DexMesh::GetLineIndiceBuffer()
{
	DEX_ENSURE_P(m_vecLineIndices.size() != 0);
	return (void*)&m_vecLineIndices[0];
}
uint32 DexSkinMesh::DexMesh::AddVertex(const DexVector3& pos, const DexVector3& normal, float u, float v)
{
	uint32 ret = 0;
	DexVector3 Normal(normal);
	Normal.Normalize();
	bool find = false;
	for (size_t i = 0; i < m_vecVertexsBuffer.size(); ++i)
	{
		if (m_vecVertexsBuffer[i].pos == pos &&
			m_vecVertexsBuffer[i].normal == Normal &&
			DexMath::Equal(m_vecVertexsBuffer[i].uv.x, u) &&
			DexMath::Equal(m_vecVertexsBuffer[i].uv.y, v))
		{
			ret = i;
			find = true;
			break;
		}
	}
	if (find)
	{
		return ret;
	}
	ret = m_vecVertexsBuffer.size();
	stMeshVertex vertex;
	vertex.pos = pos;
	vertex.normal = normal;
	vertex.uv.x = u;
	vertex.uv.y = v;
	m_vecVertexsBuffer.push_back(vertex);
	return ret;
}

void DexSkinMesh::DexMesh::AddVertexIndice(const int32& index)
{
	m_vecIndices.push_back(index);
}

DexSkinMesh::DexSkinMesh()
{
	Init();
}
DexSkinMesh::DexSkinMesh(int16 maxAniTime)

{
	Init();
	m_iAnimateEndTime = maxAniTime;
	m_iAnimateMaxTime = maxAniTime;
}

void DexSkinMesh::Init()
{
	m_iRenderFlag = 0;
	m_fAnimateRatio = 1.0f;
	m_eAniType = SkinMeshAnimateType_Loop;
	m_eMeshType = SkinMeshModelType_UnKnown;
	m_iAnimateStartTime = 0;
	m_iAnimateNowTime = 0;
	m_iAnimateEndTime = 0;
	m_iAnimateMaxTime = 0;
	m_pRootJoint = NULL;
	m_bHaveAnimation = true;
	m_bAnimate = true;
	m_iLightFlag = 0;

	m_pRootJoint = new Joint;
	m_pRootJoint->m_pFather = NULL;
	m_pRootJoint->str_name = "root_joint";
	m_pRootJoint->id = _SKIN_MESH_ROOT_JOINT_ID;
	m_pRootJoint->frame_matrix.Identity();
	m_pRootJoint->meshMatrix.Identity();
	m_pRootJoint->localMeshMatrixInvert.Identity();
	m_pRootJoint->world_matrix.Identity();
	m_vecJoints.push_back(m_pRootJoint);
	SetRenderFlag(SKINMESH_RENDER_MESH);
	jointsMatrix = new D3DXMATRIX[sGetMaxJointCount()];
	m_fJointScale = 1.0f;
	InitShader();

	iHideMeshIndex = -1;

}
void DexSkinMesh::InitShader()
{
	D3DXMatrixIdentity(&matWVP);
	memset(jointsMatrix, 0, sizeof(D3DXMATRIX)* sGetMaxJointCount());
	if (DexGameEngine::getEngine()->GetRender() != DexNull)
	{
		m_pShaderForMesh = DexGameEngine::getEngine()->GetRender()->GetShader(DexShaderHlslSkinMesh::getClassType());
		m_pShaderForVertexToJoint = DexGameEngine::getEngine()->GetRender()->GetShader(DexShaderHlslSkinMeshVertexToJoint::getClassType());
		m_pShaderForVertexNormal = DexGameEngine::getEngine()->GetRender()->GetShader(DexShaderHlslSkinMeshVertexNormal::getClassType());
	}
	SetAmbientColor(DexColor(0.3f, 0.0f, 0.0f, 1.0f));
}

void DexSkinMesh::SetAmbientColor(const DexColor& color)
{
	m_ambientColor = color;
}

void DexSkinMesh::SetDirectionLight(const stDexDirectionLight& light)
{
	m_directionLight = light;
}

void DexSkinMesh::ClearPointLight()
{
	m_vecPointLight.clear();
}

void DexSkinMesh::AddPointLight(const stDexPointLight& light)
{
	m_vecPointLight.push_back(light);
}

DexSkinMesh::~DexSkinMesh()
{
	_SafeClearVector(m_vecJoints);
	_SafeClearVector(m_vecMeshs);
	_SafeDeleteArr(jointsMatrix);
	m_vecMaterials.clear();
	for (size_t i = 0; i < m_vecTextures.size(); ++i)
	{
		DexGameEngine::getEngine()->GetTextureManager()->ReleaseTexture(m_vecTextures[i]);
	}
	m_vecTextures.clear();
}

uint16 DexSkinMesh::sGetMaxJointCount()
{
	return 60;
}
void DexSkinMesh::SetSceneNodeMatrix(const DexMatrix4x4& matrix)
{
	bindMatrix = matrix;
	m_pRootJoint->frame_matrix = bindMatrix; 
	m_pRootJoint->world_matrix = bindMatrix;
}

void DexSkinMesh::SetMaxAniTime(int16 time)
{
	m_iAnimateMaxTime = time;
	m_iAnimateStartTime = 0;
	m_iAnimateEndTime = time;
	m_bHaveAnimation = true;
	m_bAnimate = true;
}
bool DexSkinMesh::SetAnimateTime(int16 start, int16 end)
{
	bool ret = false;
	if (start >= 0 && start <= end && end <= m_iAnimateMaxTime)
	{
		m_iAnimateNowTime = start;
		m_iAnimateStartTime = start;
		m_iAnimateEndTime = end;
		ret = true;
	}
	return ret;
}

SkinMeshAnimateType DexSkinMesh::GetAnimateType()	const
{
	return m_eAniType;
}

void DexSkinMesh::SetAnimateType(SkinMeshAnimateType type)
{
	m_eAniType = type;
}

void DexSkinMesh::ReStartAnimation()
{
	if (m_eAniType == SkinMeshAnimateType_OneTime)
	{
		m_iAnimateNowTime = m_iAnimateStartTime;
	}
	else if (m_eAniType == SkinMeshAnimateType_OneTime_Back)
	{
		m_iAnimateNowTime = m_iAnimateEndTime;
	}
}
int16 DexSkinMesh::GetAnimateNowTime() const
{
	return m_iAnimateNowTime;
}

int16 DexSkinMesh::GetAnimateEndTime()const
{
	return m_iAnimateEndTime;
}

int16 DexSkinMesh::GetAnimateStartTime() const
{
	return m_iAnimateStartTime;
}

float32 DexSkinMesh::GetAnimateRatio() const
{
	return m_fAnimateRatio;
}

void DexSkinMesh::SetAnimateRatio(float32 ratio)
{
	m_fAnimateRatio = ratio;
}

void DexSkinMesh::SetAnimateEnable(bool animate)
{
	m_bAnimate = animate;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i] != NULL)
		{
			m_vecJoints[i]->m_bAnimate = m_bAnimate;
		}
	}
}

bool DexSkinMesh::GetAnimateEnable() const
{
	return m_bAnimate;
}

void DexSkinMesh::CalculateVertex()
{
}

void DexSkinMesh::SetMeshType(SkinMeshModelType type)
{
	m_eMeshType = type;
}

SkinMeshModelType DexSkinMesh::GetMeshType() const
{
	return m_eMeshType;
}

bool DexSkinMesh::Update(int32 delta)
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
	memset(jointsMatrix, 0, sizeof(D3DXMATRIX)* sGetMaxJointCount());
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		jointsMatrix[i] = *(D3DXMATRIX*)&(m_vecJoints[i]->localMeshMatrixInvert) * (*(D3DXMATRIX*)&(m_vecJoints[i]->world_matrix));
	}
	return true;
}
bool DexSkinMesh::Render()
{
	DEX_ENSURE_B(DexModelBase::Render());
#ifdef _DEBUG
	if (GetRenderFlag(SKINMESH_RENDER_JOINT))
	{
		DexMatrix4x4 jointScaleMatrix;
		jointScaleMatrix.Scale(DexVector3(m_fJointScale, m_fJointScale, m_fJointScale));
		for (size_t i = 0; i < m_vecJoints.size(); ++i)
			DexGameEngine::getEngine()->RenderCube(jointScaleMatrix * m_vecJoints[i]->world_matrix);
	}
	if (GetRenderFlag(SKINMESH_RENDER_JOINT2JOINT))
	{
		m_pRootJoint->Render2Joint();
	}
#endif
	if (GetRenderFlag(SKINMESH_RENDER_MESH))
	{//shader render
		RenderMesh();
	}
	if (GetRenderFlag(SKINMESH_RENDER_VERTEXNORMAL))
	{//render vertex normal, just use for debug
		RenderVertexNormal();
	}
	if (GetRenderFlag(SKINMESH_RENDER_VERTEX2JOINT))
	{//render vertex to joint
		RenderVertexToJoint();
	}
	return true;
}

DexSkinMesh::DexMesh* DexSkinMesh::FindMesh(int8 meshId)
{
	DexMesh* ret = NULL;
	for (size_t i = 0; i < m_vecMeshs.size(); ++i)
	{
		if (m_vecMeshs[i] != NULL && m_vecMeshs[i]->id == meshId)
		{
			ret = m_vecMeshs[i];
			break;
		}
	}
	return ret;
}

DexSkinMesh::DexMesh* DexSkinMesh::FindMesh(const char*  meshName)
{
	DexMesh* ret = NULL;
	for (size_t i = 0; i < m_vecMeshs.size(); ++i)
	{
		if (m_vecMeshs[i] != NULL && strcmp(m_vecMeshs[i]->name, meshName) == 0)
		{
			ret = m_vecMeshs[i];
			break;
		}
	}
	return ret;
}

DexSkinMesh::DexMesh* DexSkinMesh::FindMesh(uint8 iMaterialIndex, bool bAdd)
{
	DexMesh* pRet = nullptr;
	for (size_t i = 0; i < m_vecMeshs.size(); ++i)
	{
		if (m_vecMeshs[i] != nullptr && m_vecMeshs[i]->m_iMaterialId == iMaterialIndex)
		{
			pRet = m_vecMeshs[i];
			break;
		}
	}
	if (pRet == nullptr && bAdd)
	{
		pRet = new DexMesh;
		pRet->id = iMaterialIndex;
		m_vecMeshs.push_back(pRet);
	}
	return pRet;
}
DexSkinMesh::DexMesh* DexSkinMesh::AddMesh(int8 meshId)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE_P(mesh == NULL);
	mesh = new DexMesh;
	mesh->id = meshId;
	m_vecMeshs.push_back(mesh);
	return mesh;
}

DexSkinMesh::DexMesh* DexSkinMesh::AddMesh(const char* meshName)
{
	DexMesh* mesh = FindMesh(meshName);
	DEX_ENSURE_P(mesh == NULL);
	mesh = new DexMesh;
	mesh->id = m_vecMeshs.size();
	memcpy(mesh->name, meshName, strlen(meshName));
	m_vecMeshs.push_back(mesh);
	return mesh;
}

void DexSkinMesh::SetMeshVertexs(int8 meshId, void* vertexs, int32 count)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE(mesh);
	mesh->m_vecVertexsBuffer.clear();
	mesh->m_vecVertexsBuffer.resize(count);
	memcpy(&mesh->m_vecVertexsBuffer[0], vertexs, sizeof(stMeshVertex)*count);
	//for (uint32 i = 0; i < count; ++i)
	//	mesh->m_vecVertexs.push_back(((stVertex3*)vertexs)[i]);
}
void DexSkinMesh::SetMeshIndices(int8 meshId, void* indics, int32 count)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE(mesh);
	mesh->m_vecIndices.clear();
	mesh->m_vecIndices.resize(count);
	memcpy(&mesh->m_vecIndices[0], indics, sizeof(int32)*count);
	//for (uint32 i = 0; i < count; ++i)
	//	mesh->m_vecIndices.push_back(((int32*)indics)[i]);
}

DexSkinMesh::Joint* DexSkinMesh::AddJoint(int id)
{
	Joint* joint = new Joint;
	joint->id = id;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i]->id == id)
		{//重复
			delete joint;
			return NULL;
		}
	}
	m_vecJoints.push_back(joint);
	return joint;
}

DexSkinMesh::Joint* DexSkinMesh::SetJointInfo(int id, string name, string father_name, const DexMatrix4x4& father_matrix)
{
	Joint* joint = FindJoint(id);
	Joint* father = NULL;
	joint->str_name = name;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i]->str_name == father_name)
		{
			m_vecJoints[i]->AddChild(joint);
			father = m_vecJoints[i];
			break;
		}
	}
	if (!father)
	{//没有找到已经存在的joint,添加到root_joint中去
		m_pRootJoint->AddChild(joint);
		father = m_pRootJoint;
	}
	joint->meshMatrix = father_matrix * father->meshMatrix;
	joint->localMeshMatrixInvert = joint->meshMatrix.GetInvert();
	joint->frame_matrix = father_matrix;//设置默认第一帧的矩阵
	joint->world_matrix = joint->meshMatrix;
	return joint;
}

DexSkinMesh::Joint* DexSkinMesh::AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix)
{
	Joint* joint = FindJoint(id);
	DEX_ENSURE_P(joint == DexNull);
	joint = new Joint;
	joint->id = id;
	Joint* father = NULL;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i]->id == father_id)
		{
			m_vecJoints[i]->AddChild(joint);
			father = m_vecJoints[i];
			break;
		}
	}
	m_vecJoints.push_back(joint);
	if (!father)
	{//没有找到已经存在的joint,添加到root_joint中去
		m_pRootJoint->AddChild(joint);
		father = m_pRootJoint;
	}
	joint->meshMatrix = father_matrix * father->meshMatrix;
	joint->localMeshMatrixInvert = joint->meshMatrix.GetInvert();
	joint->frame_matrix = father_matrix;//设置默认第一帧的矩阵
	joint->world_matrix = joint->meshMatrix;
	return joint;
}

DexSkinMesh::Joint* DexSkinMesh::FindJoint(int32 jointId)
{
	if (jointId == -1)
		return NULL;
	Joint* ret = NULL;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i] != NULL && m_vecJoints[i]->id == jointId)
		{
			ret = m_vecJoints[i];
			break;
		}
	}
	return ret;
}

DexSkinMesh::Joint* DexSkinMesh::AddJoint(string name)
{
	Joint* joint = new Joint;
	joint->str_name = name;
	joint->id = m_vecJoints.size();
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i]->id == joint->id)
		{//重复
			delete joint;
			return NULL;
		}
	}
	m_vecJoints.push_back(joint);
	return joint;
}

DexSkinMesh::Joint* DexSkinMesh::SetJointInfo(string name, int father_id, const DexMatrix4x4& father_matrix)
{
	bool find_father = false;
	Joint* joint = FindJoint(name);
	joint->frame_matrix = father_matrix;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i]->id == father_id)
		{
			m_vecJoints[i]->AddChild(joint);
			find_father = true;
			break;
		}
	}
	if (!find_father)
	{//没有找到已经存在的joint,添加到root_joint中去
		m_pRootJoint->AddChild(joint);
	}
	return joint;
}

DexSkinMesh::Joint* DexSkinMesh::AddJoint(string name, string father_name, const DexMatrix4x4& father_matrix)
{
	bool ret = false;
	Joint* joint = new Joint;
	joint->str_name = name;
	joint->id = m_vecJoints.size();
	Joint* father = NULL;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i]->str_name == father_name)
		{
			m_vecJoints[i]->AddChild(joint);
			father = m_vecJoints[i];
			ret = true;
			break;
		}
	}
	m_vecJoints.push_back(joint);
	if (!ret)
	{//没有找到已经存在的joint,添加到root_joint中去
		m_pRootJoint->AddChild(joint);
		father = m_pRootJoint;
	}
	joint->meshMatrix = father_matrix * father->meshMatrix;
	joint->localMeshMatrixInvert = joint->meshMatrix.GetInvert();
	joint->frame_matrix = father_matrix;//设置默认第一帧的矩阵
	joint->world_matrix = joint->meshMatrix;
	return joint;
}

DexSkinMesh::Joint* DexSkinMesh::FindJoint(string name)
{
	Joint* ret = NULL;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i] != NULL && m_vecJoints[i]->str_name == name)
		{
			ret = m_vecJoints[i];
			break;
		}
	}
	return ret;
}

uint32 DexSkinMesh::FindJointIndex(DString name)
{
	uint32 ret = -1;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i] != NULL && m_vecJoints[i]->str_name == name)
		{
			ret = i;
			break;
		}
	}
	return ret;
}

bool DexSkinMesh::AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix)
{
	Joint* joint = FindJoint(jointid);
	bool ret = AddJointKeyFrame(joint, time, matrix);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian)
{
	Joint* joint = FindJoint(jointid);
	bool ret = AddJointKeyFrame(joint, time, pos, scale, axis, radian);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua)
{
	Joint* joint = FindJoint(jointid);
	bool ret = AddJointKeyFrame(joint, time, pos, scale, qua);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameTrans(int32 jointId, int32 time, const DexVector3& pos)
{
	Joint* joint = FindJoint(jointId);
	bool ret = AddJointKeyFrameTrans(joint, time, pos);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameScale(int32 jointId, int32 time, const DexVector3& scale)
{
	Joint* joint = FindJoint(jointId);
	bool ret = AddJointKeyFrameScale(joint, time, scale);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameRotation(int32 jointId, int32 time, const DexVector3& axis, float32 radian)
{
	Joint* joint = FindJoint(jointId);
	bool ret = AddJointKeyFrameRotation(joint, time, axis, radian);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameRotation(int32 jointId, int32 time, const DexQuaternion& qua)
{
	Joint* joint = FindJoint(jointId);
	bool ret = AddJointKeyFrameRotation(joint, time, qua);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrame(string name, int32 time, const DexMatrix4x4& matrix)
{
	Joint* joint = FindJoint(name);
	bool ret = AddJointKeyFrame(joint, time, matrix);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrame(string name, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian)
{
	Joint* joint = FindJoint(name);
	bool ret = AddJointKeyFrame(joint, time, pos, scale, axis, radian);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrame(string name, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua)
{
	Joint* joint = FindJoint(name);
	bool ret = AddJointKeyFrame(joint, time, pos, scale, qua);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameTrans(string name, int32 time, const DexVector3& pos)
{
	Joint* joint = FindJoint(name);
	bool ret = AddJointKeyFrameTrans(joint, time, pos);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameScale(string name, int32 time, const DexVector3& scale)
{
	Joint* joint = FindJoint(name);
	bool ret = AddJointKeyFrameScale(joint, time, scale);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameRotation(string name, int32 time, const DexVector3& axis, float32 radian)
{
	Joint* joint = FindJoint(name);
	bool ret = AddJointKeyFrameRotation(joint, time, axis, radian);
	return ret;
}

bool DexSkinMesh::AddJointKeyFrameRotation(string name, int32 time, const DexQuaternion& qua)
{
	Joint* joint = FindJoint(name);
	bool ret = AddJointKeyFrameRotation(joint, time, qua);
	return ret;
}


bool DexSkinMesh::AddJointKeyFrame(Joint* joint, int32 time, const DexMatrix4x4& matrix)
{
	DEX_ENSURE_B(joint);
	joint->AddKeyFrame(time, matrix);
	return true;
}

bool DexSkinMesh::AddJointKeyFrame(Joint* joint, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian)
{
	DEX_ENSURE_B(joint);
	DexQuaternion qua(axis, radian);
	DexMatrix4x4 rotationMatrix = qua.GetMatrix();
	DexMatrix4x4 scaleMatrix; scaleMatrix.Scale(scale);
	DexMatrix4x4 TransMatrix; TransMatrix.Translate(pos);
	DexMatrix4x4 matrix = scaleMatrix * rotationMatrix * TransMatrix;
	joint->AddKeyFrame(time, matrix);
	return true;
}

bool DexSkinMesh::AddJointKeyFrame(Joint* joint, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua)
{
	DEX_ENSURE_B(joint);
	DexMatrix4x4 rotationMatrix = qua.GetMatrix();
	DexMatrix4x4 scaleMatrix; scaleMatrix.Scale(scale);
	DexMatrix4x4 TransMatrix; TransMatrix.Translate(pos);
	DexMatrix4x4 matrix = scaleMatrix * rotationMatrix * TransMatrix;
	joint->AddKeyFrame(time, matrix);
	return true;
}

bool DexSkinMesh::AddJointKeyFrameTrans(Joint* joint, int32 time, const DexVector3& pos)
{
	DEX_ENSURE_B(joint);
	DexMatrix4x4 matrix;
	matrix.Translate(pos);
	joint->AddKeyFrame(time, matrix);
	return true;
}

bool DexSkinMesh::AddJointKeyFrameScale(Joint* joint, int32 time, const DexVector3& scale)
{
	DEX_ENSURE_B(joint);
	DexMatrix4x4 matrix;
	joint->AddKeyFrame(time, matrix.Scale(scale));
	return true;
}

bool DexSkinMesh::AddJointKeyFrameRotation(Joint* joint, int32 time, const DexVector3& axis, float32 radian)
{
	DEX_ENSURE_B(joint);
	DexQuaternion qua(axis, radian);
	DexMatrix4x4 matrix = qua.GetMatrix();
	joint->AddKeyFrame(time, matrix);
	return true;
}

bool DexSkinMesh::AddJointKeyFrameRotation(Joint* joint, int32 time, const DexQuaternion& qua)
{
	DEX_ENSURE_B(joint);
	DexMatrix4x4 matrix = qua.GetMatrix();
	joint->AddKeyFrame(time, matrix);
	return true;
}

bool DexSkinMesh::UpdateOneTime(int32 delta)
{
	bool before_end = m_iAnimateNowTime < m_iAnimateEndTime;
	if (m_pRootJoint != NULL)
	{
		m_pRootJoint->Update(m_iAnimateNowTime);
	}
	if (m_iAnimateNowTime > m_iAnimateEndTime)
		return false;
	m_iAnimateNowTime += delta;
	if (before_end && m_iAnimateNowTime > m_iAnimateEndTime)
	{//这是为了修复某些帧之间卡顿，导致这一帧更新之后，大于endtime,动画便不再更新从而无法停留在end位置的问题
		m_iAnimateNowTime = m_iAnimateEndTime;
	}
	return true;
}

bool DexSkinMesh::UpdateLoop(int32 delta)
{
	if (m_pRootJoint != NULL)
		m_pRootJoint->Update(m_iAnimateNowTime);
	m_iAnimateNowTime += delta;
	if (m_iAnimateNowTime > m_iAnimateEndTime)
		m_iAnimateNowTime = m_iAnimateStartTime;
	return true;
}

bool DexSkinMesh::UpdateOneTimeBack(int32 delta)
{
	if (m_pRootJoint != NULL)
		m_pRootJoint->Update(m_iAnimateNowTime);
	if (m_iAnimateNowTime < m_iAnimateStartTime)
		return false;
	m_iAnimateNowTime -= delta;
	return true;
}

bool DexSkinMesh::UpdateLoopBack(int32 delta)
{
	if (m_pRootJoint != NULL)
		m_pRootJoint->Update(m_iAnimateNowTime);
	m_iAnimateNowTime -= delta;
	if (m_iAnimateNowTime < m_iAnimateStartTime)
		m_iAnimateNowTime = m_iAnimateEndTime;
	return true;
}


bool DexSkinMesh::SetTexture(int8 meshId, int8 textureId)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE_B(mesh);
	mesh->m_iTextureId = textureId;
	return true;
}

bool DexSkinMesh::AddTexture(DexTexture* tex)
{
	tex->AddRef();
	m_vecTextures.push_back(tex);
	return true;
}

int32 DexSkinMesh::FindTexture(DString textureName)
{
	int32 index = -1;
	for (size_t i = 0; i < m_vecTextures.size(); ++i)
	{
		if (m_vecTextures[i] != DexNull &&
			m_vecTextures[i]->Name() == textureName)
		{
			index = i;
			break;
		}
	}
	return index;
}

int32 DexSkinMesh::AddTexture(const char* filename)
{
	int32 index = -1;
	for (size_t i = 0; i < m_vecTextures.size(); ++i)
	{
		if (m_vecTextures[i] != DexNull &&
			m_vecTextures[i]->Name() == filename)
		{
			index = i;
			m_vecTextures[i]->AddRef();
			break;
		}
	}
	if (index == -1)
	{
		DexTexture* tex = DexGameEngine::getEngine()->GetTextureManager()->FindTexure(filename);
		if (tex != DexNull)
			tex->AddRef();
		else
			tex = DexGameEngine::getEngine()->GetTextureManager()->CreateTexture(filename);
		m_vecTextures.push_back(tex);
		index = m_vecTextures.size()-1;
	}
	return index;
}

int32 DexSkinMesh::AddMaterial(const DexMaterial& material)
{
	int32 index = -1;
	for (size_t i = 0; i < m_vecMaterials.size(); ++i)
	{
		if (dexstrcmp(material.name, m_vecMaterials[i].name) == 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
	{
		m_vecMaterials.push_back(material);
		index = m_vecMaterials.size()-1;
	}
	return index;
}

int32 DexSkinMesh::FindMaterial(DString materialName)
{
	int32 index = -1;
	for (size_t i = 0; i < m_vecMaterials.size(); ++i)
	{
		if (dexstrcmp(materialName.c_str(), m_vecMaterials[i].name) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
}

void DexSkinMesh::AddVertex(const DexVector3& pos)
{
}
void DexSkinMesh::AddVertex(const DexVector3& pos, int16* JointId, float* weight, int16 jointCount)
{
}

void DexSkinMesh::AddMeshTriangle(int8 meshId, int32 index0, int32 index1, int32 index2)
{
	DexMesh* pMesh = FindMesh(meshId);
	DEX_ENSURE(pMesh != NULL);
	pMesh->m_vecIndices.push_back(index0);
	pMesh->m_vecIndices.push_back(index1);
	pMesh->m_vecIndices.push_back(index2);
}

void DexSkinMesh::AddVertex(int8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE(mesh);
	if (!FindVertex(pos))
	{//添加新顶点并绑到根节点上
		AddVertex(pos, NULL, NULL, 0);
	}
	mesh->AddVertex(pos, normal, uv.x, uv.y);
}

void DexSkinMesh::AddVertex(int8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv, int16* JointId, float* weight, uint16 jointCount)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE(mesh);
	stMeshVertex vertex;
	vertex.pos = pos;
	vertex.normal = normal;
	vertex.color = DEXCOLOR_WHITE;
	vertex.uv = uv;
	if (JointId == NULL)
	{//bind to root
		vertex.JointIndex[0] = 0;
		vertex.JointWeights[0] = 1.0f;
	}
	else
	{
		for (uint16 i = 0; i < jointCount; ++i)
		{
			vertex.JointIndex[i] = JointId[i];
			vertex.JointWeights[i] = weight[i];
		}
	}
	mesh->m_vecVertexsBuffer.push_back(vertex);
}

bool DexSkinMesh::GetRenderFlag(int16 flag)
{
	return m_iRenderFlag & flag;
}

void DexSkinMesh::SetRenderFlag(int16 flag)
{
	m_iRenderFlag |= flag;
	if (m_iRenderFlag & SKINMESH_RENDER_VERTEXNORMAL)
	{
		for (size_t i = 0; i < m_vecMeshs.size(); ++i)
		{
			m_vecMeshs[i]->CreateNormalBuffer();
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecMeshs.size(); ++i)
		{
			m_vecMeshs[i]->DestroyNormalBuffer();
		}
	}
	if (m_iRenderFlag & SKINMESH_RENDER_VERTEX2JOINT)
	{
		for (size_t i = 0; i < m_vecMeshs.size(); ++i)
		{
			m_vecMeshs[i]->CreateVertexToJointBuffer(m_vecJoints);
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecMeshs.size(); ++i)
		{
			m_vecMeshs[i]->DestroyVertexToJointlBuffer();
		}
	}
}

void DexSkinMesh::SetJointScale(float32 fScale)
{
	m_fJointScale = fScale;
}

bool DexSkinMesh::FindVertex(const DexVector3& pos)
{
	return false;
}

bool DexSkinMesh::IsStaticModel()
{
	return !m_bHaveAnimation;
}

void DexSkinMesh::RenderMesh()
{
	m_pShaderForMesh->SetTarget(this);
	DexGameEngine::getEngine()->GetRender()->SetShader(m_pShaderForMesh);
	DexGameEngine::getEngine()->GetRender()->RenderShader();
	m_pShaderForMesh->SetTarget(nullptr);
}

void DexSkinMesh::RenderVertexNormal()
{
	m_pShaderForVertexNormal->SetTarget(this);
	DexGameEngine::getEngine()->GetRender()->SetShader(m_pShaderForVertexNormal);
	DexGameEngine::getEngine()->GetRender()->RenderShader();
	m_pShaderForVertexNormal->SetTarget(nullptr);
}

void DexSkinMesh::RenderVertexToJoint()
{
	m_pShaderForVertexToJoint->SetTarget(this);
	DexGameEngine::getEngine()->GetRender()->SetShader(m_pShaderForVertexToJoint);
	DexGameEngine::getEngine()->GetRender()->RenderShader();
	m_pShaderForVertexToJoint->SetTarget(nullptr);
}