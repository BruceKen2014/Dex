

#include "DexBase/DexLog.h"
#include "DexBase/DexRenderDirectx9.h"
#include "DexBase/DexVertexDeclDx9.h"
#include "DexMath/DexVector4.h"
#include "../Source/CTexture.h"
#include "../state/DexGameEngine.h"
#include "../DexMath/DexMath2.h"
#include "DexSkinMesh.h"
const int JointNumber = 40;
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
				for (size_t i = 0; i < m_vecKeyFrames.size(); ++i)
				{
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

void DexSkinMesh::DexMesh::CreateVertexToJointBuffer(const Vector<Joint*>& vecJoints)
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

DexSkinMesh::DexSkinMesh() :m_iRenderFlag(0), m_eAniType(SkinMeshAnimateType_Loop),
m_fAnimateRatio(1.0f),
m_eMeshType(SkinMeshModelType_UnKnown)
{
	m_iAnimateStartTime = 0;
	m_iAnimateNowTime = 0;
	m_iAnimateEndTime = 0;
	m_iAnimateMaxTime = 0;
	m_pRootJoint = NULL;
	m_bHaveAnimation = false;
	m_bAnimate = false;
	m_bLightFlag = true;

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
	jointsMatrix = new D3DXMATRIX[JointNumber];
	InitShader();
}
DexSkinMesh::DexSkinMesh(int16 maxAniTime) :m_iRenderFlag(0), m_eAniType(SkinMeshAnimateType_Loop), m_fAnimateRatio(1.0f),
m_eMeshType(SkinMeshModelType_UnKnown)
{
	m_iAnimateStartTime = 0;
	m_iAnimateNowTime = 0;
	m_iAnimateEndTime = maxAniTime;
	m_iAnimateMaxTime = maxAniTime;
	m_pRootJoint = NULL;
	m_bHaveAnimation = true;
	m_bAnimate = true;

	m_pRootJoint = new Joint;
	m_pRootJoint->m_pFather = NULL;
	m_pRootJoint->id = _SKIN_MESH_ROOT_JOINT_ID;
	m_pRootJoint->frame_matrix.Identity();
	m_vecJoints.push_back(m_pRootJoint);
	SetRenderFlag(SKINMESH_RENDER_MESH);
	jointsMatrix = new D3DXMATRIX[JointNumber];
	InitShader();
}
void DexSkinMesh::InitShader()
{
	D3DXMatrixIdentity(&matWVP);
	memset(jointsMatrix, 0, sizeof(D3DXMATRIX)* JointNumber);
	shader = new DexShaderHlslSkinMesh;
	shader->SetTarget(this);
	//test fx effect
	LPD3DXBUFFER	pbuff = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(DexGameEngine::getEngine()->GetDevice(), "shader\\effect.fx", NULL, NULL, D3DXSHADER_DEBUG, NULL,
		&pFxEffect, &pbuff);
	if (pbuff)
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, (char*)(pbuff->GetBufferPointer()));
		pbuff->Release();
	}
	if (FAILED(hr))
	{
		getLog()->BeginLog();
		getLog()->Log(log_error, "D3DXCreateEffectFromFile 调用失败！");
		getLog()->EndLog();
	}
	else
	{
		WVPMatrixHandle = pFxEffect->GetParameterByName(0, "WVPMarix");
		Tex0Handle = pFxEffect->GetParameterByName(0, "Tex0");
		TechHandle = pFxEffect->GetTechniqueByName("T0");
		VertexBlendFlag = pFxEffect->GetParameterByName(0, "VertexBlendFlag");
		m_bLightEffect = pFxEffect->GetParameterByName(0, "g_bLightEffect"); 
		m_bLightEnable = pFxEffect->GetParameterByName(0, "g_bLightEnable");
		m_biPointLightCount = pFxEffect->GetParameterByName(0, "g_iLightCountPoint");
		m_bPointData = pFxEffect->GetParameterByName(0, "g_LightPoints"); 
		m_ambientColor = pFxEffect->GetParameterByName(0, "g_ambientColor");
		m_material = pFxEffect->GetParameterByName(0, "g_material");
		JointMatrixHandle = pFxEffect->GetParameterByName(0, "JointsMatrix");
		//JointInvertMatrixHandle = pFxEffect->GetParameterByName(0, "JointsMatrixInvert");
	}
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 36, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		//{ 0, 60, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		//{ 0, 64, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
		D3DDECL_END()
	};
	m_pDecl = new DexVertexDeclDx9(DexGameEngine::getEngine()->GetDevice(), decl);
}
DexSkinMesh::~DexSkinMesh()
{
	_SafeClearVector(m_vecJoints);
	_SafeClearVector(m_vecMeshs);
	_SafeDelete(m_pDecl);
	_SafeDeleteArr(jointsMatrix);
	m_vecMaterials.clear();
	for (size_t i = 0; i < m_vecTextures.size(); ++i)
	{
		_SafeReduceRef(m_vecTextures[i]);
	}
	m_vecTextures.clear();
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
	pFxEffect->SetMatrix(WVPMatrixHandle, &matWVP);
	
	memset(jointsMatrix, 0, sizeof(D3DXMATRIX)* JointNumber);
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		jointsMatrix[i] = *(D3DXMATRIX*)&(m_vecJoints[i]->localMeshMatrixInvert) * (*(D3DXMATRIX*)&(m_vecJoints[i]->world_matrix));
	}
	pFxEffect->SetMatrixArray(JointMatrixHandle, jointsMatrix, JointNumber);
	return true;
}
bool DexSkinMesh::Render()
{
	DEX_ENSURE_B(DexModelBase::Render());
#ifdef _DEBUG
	if (GetRenderFlag(SKINMESH_RENDER_JOINT))
	{
		m_pRootJoint->Render();
	}
	if (GetRenderFlag(SKINMESH_RENDER_JOINT2JOINT))
	{
		m_pRootJoint->Render2Joint();
	}
	if (GetRenderFlag(SKINMESH_RENDER_VERTEX2JOINT))
	{//画出顶点到所绑定关节的线，用于调试
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
	Joint* joint = new Joint;
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
	joint->frame_matrix = father_matrix;
	for (size_t i = 0; i < m_vecJoints.size(); ++i)
	{
		if (m_vecJoints[i]->str_name == father_name)
		{
			m_vecJoints[i]->AddChild(joint);
			ret = true;
			break;
		}
	}
	m_vecJoints.push_back(joint);
	if (!ret)
	{//没有找到已经存在的joint,添加到root_joint中去
		m_pRootJoint->AddChild(joint);
	}
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
	if (m_iAnimateNowTime > m_iAnimateEndTime)
	{
		m_iAnimateNowTime = m_iAnimateEndTime;
	}
	if (m_pRootJoint != NULL)
	{
		m_pRootJoint->Update(m_iAnimateNowTime);
	}
	if (m_iAnimateNowTime >= m_iAnimateEndTime)
	{
		m_iAnimateNowTime = m_iAnimateStartTime;
	}
	m_iAnimateNowTime += delta;
	return true;
}

bool DexSkinMesh::UpdateOneTimeBack(int32 delta)
{
	if (m_pRootJoint != NULL)
	{
		m_pRootJoint->Update(m_iAnimateNowTime);
	}
	if (m_iAnimateNowTime < m_iAnimateStartTime)
		return false;
	m_iAnimateNowTime -= delta;
	return true;
}

bool DexSkinMesh::UpdateLoopBack(int32 delta)
{
	if (m_iAnimateNowTime < m_iAnimateStartTime)
	{
		m_iAnimateNowTime = m_iAnimateStartTime;
	}
	if (m_pRootJoint != NULL)
	{
		m_pRootJoint->Update(m_iAnimateNowTime);
	}
	if (m_iAnimateNowTime <= m_iAnimateStartTime)
	{
		m_iAnimateNowTime = m_iAnimateEndTime;
	}
	m_iAnimateNowTime -= delta;
	return true;
}


bool DexSkinMesh::SetTexture(int8 meshId, int8 textureId)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE_B(mesh);
	mesh->m_iTextureId = textureId;
	return true;
}

bool DexSkinMesh::AddTexture(CDexTex* tex)
{
	tex->AddRef();
	m_vecTextures.push_back(tex);
	return true;
}

bool DexSkinMesh::AddTexture(const char* filename)
{
	CDexTex* tex = new CDexTex();
	if (!tex->LoadTex(filename))
	{
		tex->ReduceRef();
		return false;
	}
	m_vecTextures.push_back(tex);
	return true;
}

bool DexSkinMesh::AddMaterial(const DexMaterial& material)
{
	m_vecMaterials.push_back(material);
	return true;
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
	DexGameEngine::getEngine()->GetRender()->SetShader(shader);
	DexGameEngine::getEngine()->GetRender()->RenderShader();
	return;
	//得找一下如何在shader中开启alphablend
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//这些属于固定管线的设置可以在shader中设置，所以这里不需要设置
	//D3DXMatrixIdentity(&DexGameEngine::getEngine()->g_worldMatrix);
	//DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &DexGameEngine::getEngine()->g_worldMatrix);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DexGameEngine::getEngine()->setDexVertexDecl(m_pDecl);
	int lighDSize = 0;
	struct tempPointLight
	{
		DexVector4  color;
		DexVector4  position;
		DexVector4  rangeAtte;
	};
	struct tempMaterial
	{
		DexVector4 diffuse;
		DexVector4 specular;
		DexVector4 ambient;
		DexVector4 emissive;
	};
	tempPointLight tLight;
	tLight.color = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	DexVector3 vec = DexGameEngine::getEngine()->getCamera()->GetPosition();
	tLight.position = DexVector4(vec.x, vec.y, vec.z, 1.0f);
	tLight.rangeAtte = DexVector4(500.0f, 0.0f, 0.0f, 0.0f);
	tempPointLight tLight2;
	tLight2.color = DexVector4(0.0f, 1.0f, 0.0f, 1.0f);
	tLight2.position = DexVector4(0.0f, 0.0f, -100.0f, 1.0f);
	tLight2.rangeAtte = DexVector4(1000.0f, 0.0f, 0.0f, 0.0f);
	tempPointLight arrLight[2] = { tLight, tLight2 };

	DexVector4 ambientColor(0.2f, 0.2f, 0.2f, 1.0f);

	tempMaterial material;
	material.diffuse = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	material.specular = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	material.ambient = DexVector4(1.0f, 1.0f, 1.0f, 1.0f);
	material.emissive = DexVector4(0.0f, 0.0f, 0.0f, 1.0f);
	pFxEffect->SetRawValue(m_material, &material, 0, sizeof(tempMaterial));
	pFxEffect->SetRawValue(m_bPointData, arrLight, 0, sizeof(tempPointLight) * 2);
	pFxEffect->SetFloatArray(m_ambientColor, (float*)&ambientColor, 4);
	pFxEffect->SetInt(m_biPointLightCount, 2);
	pFxEffect->SetBool(m_bLightEffect, true);
	pFxEffect->SetBool(m_bLightEnable, true);
	pFxEffect->SetTechnique(TechHandle);
	pFxEffect->SetInt(VertexBlendFlag, SkinMeshVertexBlend_Blend);
	uint32 pass = 0;
	pFxEffect->Begin(&pass, 0);
	for (size_t i = 0; i < m_vecMeshs.size(); ++i)
	{
		if (m_vecMeshs[i] == NULL)
			continue;
		if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_LINE)
		{
			if (m_vecMeshs[i]->GetLineIndiceCount() == 0)
			{
				m_vecMeshs[i]->CreateLineIndices();
			}
			pFxEffect->SetTexture(Tex0Handle, NULL);
			for (uint32 p = 0; p < pass; ++p)
			{
				pFxEffect->BeginPass(p);
				DexGameEngine::getEngine()->DrawPrimitive(DexPT_LINELIST, m_vecMeshs[i]->GetVertexBuffer(),
					m_vecMeshs[i]->GetVertexCount(), m_vecMeshs[i]->GetLineIndiceBuffer(),
					m_vecMeshs[i]->GetLineIndiceCount() / 2, sizeof(stMeshVertex));
				pFxEffect->EndPass();
			}
		}
		else if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_TRIANGLE)
		{
			if (m_vecMeshs[i]->GetLineIndiceCount() != 0)
			{
				m_vecMeshs[i]->DestroyLineIndices();
			}
			if (m_vecMeshs[i]->m_iMaterialId != -1)
				DexGameEngine::getEngine()->SetMaterial(m_vecMaterials[m_vecMeshs[i]->m_iMaterialId]);
			if (m_vecMeshs[i]->m_iTextureId != -1)
			{
				pFxEffect->SetTexture(Tex0Handle, m_vecTextures[m_vecMeshs[i]->m_iTextureId]->GetTexPt());
			}
			else
			{
				pFxEffect->SetTexture(0, NULL);
			}
			for (uint32 p = 0; p < pass; ++p)
			{
				pFxEffect->BeginPass(p);
				DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, m_vecMeshs[i]->GetVertexBuffer(),
					m_vecMeshs[i]->GetVertexCount(), m_vecMeshs[i]->GetIndiceBuffer(),
					m_vecMeshs[i]->GetIndiceCount() / 3, sizeof(stMeshVertex));
				pFxEffect->EndPass();
			}
		}
	}
	pFxEffect->End();
}

void DexSkinMesh::RenderVertexNormal()
{
	//得找一下如何在shader中开启alphablend
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DexGameEngine::getEngine()->setDexVertexDecl(m_pDecl);
	pFxEffect->SetTechnique(TechHandle);
	pFxEffect->SetInt(VertexBlendFlag, SkinMeshVertexBlend_VertexColor);
	uint32 pass = 0;
	pFxEffect->Begin(&pass, 0);
	for (size_t i = 0; i < m_vecMeshs.size(); ++i)
	{
		if (m_vecMeshs[i] == NULL)
			continue;
		//pFxEffect->SetTexture(Tex0Handle, NULL);
		for (uint32 p = 0; p < pass; ++p)
		{
			pFxEffect->BeginPass(p);
			DexGameEngine::getEngine()->DrawPrimitive(DexPT_LINELIST, m_vecMeshs[i]->GetNormalBuffer(),
				m_vecMeshs[i]->GetNormalBufferCount(), m_vecMeshs[i]->GetNormalIndiceBuffer(),
				m_vecMeshs[i]->GetNormalIndiceCount() / 2, sizeof(stMeshVertex));
			pFxEffect->EndPass();
		}
	}
	pFxEffect->End();
}

void DexSkinMesh::RenderVertexToJoint()
{
	//得找一下如何在shader中开启alphablend
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DexGameEngine::getEngine()->setDexVertexDecl(m_pDecl);
	pFxEffect->SetTechnique(TechHandle);
	pFxEffect->SetInt(VertexBlendFlag, SkinMeshVertexBlend_VertexColor);
	uint32 pass = 0;
	pFxEffect->Begin(&pass, 0);
	//pFxEffect->
	for (size_t i = 0; i < m_vecMeshs.size(); ++i)
	{
		if (m_vecMeshs[i] == NULL)
			continue;
		//pFxEffect->SetTexture(Tex0Handle, NULL);
		for (uint32 p = 0; p < pass; ++p)
		{
			pFxEffect->BeginPass(p);
			DexGameEngine::getEngine()->DrawPrimitive(DexPT_LINELIST, m_vecMeshs[i]->GetVertexToJointBuffer(),
				m_vecMeshs[i]->GetVertexToJointBufferCount(), m_vecMeshs[i]->GetVertexToJointIndiceBuffer(),
				m_vecMeshs[i]->GetVertexToJointIndiceCount() / 2, sizeof(stMeshVertex));
			pFxEffect->EndPass();
		}
	}
	pFxEffect->End();
}