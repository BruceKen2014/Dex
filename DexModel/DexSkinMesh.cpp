

#include "DexBase/DexLog.h"
#include "../Source/CTexture.h"
#include "../state/DexGameEngine.h"
#include "../DexMath/DexMath2.h"
#include "DexSkinMesh.h"

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
	m_iTextureId = -1;
	m_iMaterialId = -1;
}
DexSkinMesh::DexMesh::~DexMesh()
{
	m_vecVertexs.clear();
	m_vecIndices.clear();
	m_vecLineIndices.clear();
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

uint32 DexSkinMesh::DexMesh::GetVertexCount() const
{
	return m_vecVertexs.size();
}

void* DexSkinMesh::DexMesh::GetVertexBuffer()
{
	DEX_ENSURE_P(m_vecVertexs.size() != 0);
	return (void*)&m_vecVertexs[0];
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
	for (size_t i = 0; i < m_vecVertexs.size(); ++i)
	{
		if (m_vecVertexs[i].m_pos == pos &&
			m_vecVertexs[i].m_normal == Normal &&
			DexMath::Equal(m_vecVertexs[i].m_u,u) &&
			DexMath::Equal(m_vecVertexs[i].m_v,v))
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
	ret = m_vecVertexs.size();
	stVertex3 vertex(pos, Normal, u, v);
	m_vecVertexs.push_back(vertex);
	return ret;
}

void DexSkinMesh::DexMesh::AddVertexIndice(const int32& index)
{
	m_vecIndices.push_back(index);
}

DexSkinMesh::DexSkinMesh() :m_iRenderFlag(0), m_eAniType(SkinMeshAnimateType_Loop), m_fAnimateRatio(1.0f),
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
	m_pRootJoint->id = _SKIN_MESH_ROOT_JOINT_ID;
	m_pRootJoint->frame_matrix.Identity();
	m_vecJoints.push_back(m_pRootJoint);
	SetRenderFlag(SKINMESH_RENDER_MESH);
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
}
DexSkinMesh::~DexSkinMesh()
{
	_SafeClearVector(m_vecJoints);
	_SafeClearVector(m_vecMeshVertexs);
	_SafeClearVector(m_vecMeshs);
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
	//如果是一个没有骨骼的静态模型
	//所有的顶点绑定到原点的根节点上
	bool bStatic = m_vecJoints.size() == 1;
	for (size_t i = 0; i < m_vecMeshVertexs.size(); ++i)
	{
		MeshVertex* tempMeshVertex = m_vecMeshVertexs[i];
		if (bStatic)
		{
			AddJointInfo(m_vecMeshVertexs[i], _SKIN_MESH_ROOT_JOINT_ID, 1.0f);
		}
		for (size_t iMeshIndex = 0; iMeshIndex < m_vecMeshs.size(); ++iMeshIndex)
		{
			DexMesh* tempMesh = m_vecMeshs[iMeshIndex];
			for (int iMeshVertexIndex = 0; iMeshVertexIndex < tempMesh->GetVertexCount();
				++iMeshVertexIndex)
			{
				if (tempMesh->m_vecVertexs[iMeshVertexIndex].m_pos == tempMeshVertex->meshPosition)
				{
					(tempMeshVertex->m_vecVertexData).push_back(&(tempMesh->m_vecVertexs[iMeshVertexIndex]));
					DexVector3 tempNormal = tempMesh->m_vecVertexs[iMeshVertexIndex].m_normal;
					tempNormal.Normalize();
					DexVector3 tempNormalPos = tempMeshVertex->meshPosition + tempNormal * 10.0f;
					(tempMeshVertex->m_vecNormalPosition).push_back(tempNormalPos);
					tempMeshVertex->m_vecWorldNormal.push_back(DexVector3(0.0f, 0.0f, 0.0f));
				}
			}
		}
	}
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
	UpdateVertex();
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
	for (size_t i = 0; i < m_vecMeshVertexs.size(); ++i)
	{
		if (GetRenderFlag(SKINMESH_RENDER_VERTEXNORMAL))
		{//绘制顶点法线，调试用
			for (size_t normalIndex = 0; normalIndex < m_vecMeshVertexs[i]->m_vecWorldNormal.size(); ++normalIndex)
			{
				DexGameEngine::getEngine()->Render3DLine(m_vecMeshVertexs[i]->worldPosition, m_vecMeshVertexs[i]->m_vecWorldNormal[normalIndex], DexColor(1.0f, 0.0f, 1.0f), 10.0f, DexColor(1.0f, 0.0f, 1.0f));
			}
		}
		if (GetRenderFlag(SKINMESH_RENDER_VERTEX2JOINT))
		{//画出顶点到所绑定关节的线，用于调试
			for (size_t k = 0; k < m_vecMeshVertexs[i]->m_vecJointId.size(); ++k)
			{
				int16 bindJointId = m_vecMeshVertexs[i]->m_vecJointId[k];
				for (size_t j = 0; j < m_vecJoints.size(); ++j)
				{
					if (m_vecJoints[j]->id == bindJointId)
					{
						DexGameEngine::getEngine()->Render3DLine(m_vecMeshVertexs[i]->worldPosition, m_vecJoints[j]->world_matrix.GetPosition(), DEXCOLOR_GREEN, DEXCOLOR_GREEN);
					}
				}
			}
		}
	}
#endif
	if (GetRenderFlag(SKINMESH_RENDER_MESH))
	{
		DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		bool enabelLight = DexGameEngine::getEngine()->GetLightEnable();
		DexGameEngine::getEngine()->SetLightEnable(enabelLight && m_bLightFlag);
		for (size_t i = 0; i < m_vecMeshs.size(); ++i)
		{
			if (m_vecMeshs[i] == NULL)
				continue;
			if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_LINE)
			{
				DexGameEngine::getEngine()->SetLightEnable(false);
				if (m_vecMeshs[i]->GetLineIndiceCount() == 0)
				{
					m_vecMeshs[i]->CreateLineIndices();
				}
				DexGameEngine::getEngine()->SetTexture(0, NULL);
				DexGameEngine::getEngine()->DrawPrimitive(DexPT_LINELIST, m_vecMeshs[i]->GetVertexBuffer(), m_vecMeshs[i]->GetVertexCount(),
					m_vecMeshs[i]->GetLineIndiceBuffer(),m_vecMeshs[i]->GetLineIndiceCount() / 2, FVF_XYZ_COLOR_T1_N, sizeof(stVertex3));
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
					DexGameEngine::getEngine()->SetTexture(0, m_vecTextures[m_vecMeshs[i]->m_iTextureId]);
				DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, m_vecMeshs[i]->GetVertexBuffer(), m_vecMeshs[i]->GetVertexCount(), 
					m_vecMeshs[i]->GetIndiceBuffer(),m_vecMeshs[i]->GetIndiceCount() / 3, FVF_XYZ_COLOR_T1_N, sizeof(stVertex3));
			}

		}
		DexGameEngine::getEngine()->SetLightEnable(enabelLight);
		DexGameEngine::getEngine()->SetTexture(0, NULL);
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

DexSkinMesh::DexMesh* DexSkinMesh::AddMesh(int8 meshId)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE_P(mesh == NULL);
	mesh = new DexMesh;
	mesh->id = meshId;
	m_vecMeshs.push_back(mesh);
	return mesh;
}

void DexSkinMesh::SetMeshVertexs(int8 meshId, void* vertexs, int32 count)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE(mesh);
	mesh->m_vecVertexs.clear();
	mesh->m_vecVertexs.resize(count);
	memcpy(&mesh->m_vecVertexs[0], vertexs, sizeof(stVertex3)*count);
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

void DexSkinMesh::UpdateVertex()
{
	for (size_t i = 0; i < m_vecMeshVertexs.size(); ++i)
	{
		if (m_vecMeshVertexs[i] != NULL)
		{
			ComputeVertex(m_vecMeshVertexs[i]);
		}
	}
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

void DexSkinMesh::AddVertex(const DexVector3& pos, int16* JointId, float* weight, int16 jointCount)
{
	MeshVertex* vertex = new MeshVertex;
	vertex->meshPosition = pos;
	vertex->worldPosition = pos;
	if (JointId == NULL)
	{
		AddJointInfo(vertex, m_pRootJoint->id, 1.0f);
	}
	else
	{
		for (int i = 0; i < jointCount; ++i)
		{
			AddJointInfo(vertex, JointId[i], weight[i]);
			Joint* joint = FindJoint(JointId[i]);
			if (joint == NULL)
			{
				AddJoint(JointId[i]);
			}
		}
	}
	//if (m_vecMeshVertexs.size() >= m_vecMeshVertexs.capacity())
	//	m_vecMeshVertexs.reserve(m_vecMeshVertexs.size() * 2);
	m_vecMeshVertexs.push_back(vertex);
}

void DexSkinMesh::AddMeshTriangle(int8 meshId, int32 index0, int32 index1, int32 index2)
{
	DexMesh* pMesh = FindMesh(meshId);
	DEX_ENSURE(pMesh != NULL);
	pMesh->m_vecIndices.push_back(index0);
	pMesh->m_vecIndices.push_back(index1);
	pMesh->m_vecIndices.push_back(index2);
}
void DexSkinMesh::AddVertexJointInfo(int32 vertexIndex, int16 jointId, float32 weight)
{
	DEX_ENSURE(vertexIndex < m_vecMeshVertexs.size()
		&& m_vecMeshVertexs[vertexIndex] != NULL);
	AddJointInfo(m_vecMeshVertexs[vertexIndex], jointId, weight);
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

bool DexSkinMesh::GetRenderFlag(int16 flag)
{
	return m_iRenderFlag & flag;
}

void DexSkinMesh::SetRenderFlag(int16 flag)
{
	m_iRenderFlag |= flag;
}

bool DexSkinMesh::IsStaticModel()
{
	return !m_bHaveAnimation;
}

DexVector3 DexSkinMesh::ComputeVertex(MeshVertex* vertex)
{
	vertex->worldPosition.Set(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < vertex->m_vecWorldNormal.size(); ++i)
	{
		vertex->m_vecWorldNormal[i].Set(0.0f, 0.0f, 0.0f);
	}
	if (vertex->m_vecJointId.size() != 0)
	{//顶点已绑定骨骼
		Joint* joint = NULL;
		for (size_t i = 0; i < vertex->m_vecJointId.size(); ++i)
		{
			joint = FindJoint(vertex->m_vecJointId[i]);
			vertex->worldPosition += vertex->meshPosition * joint->localMeshMatrixInvert * joint->world_matrix* vertex->m_vecJointWeigth[i];
			//now I havent find a method use matrix and normal to calculate world normal
			//just use a temp point instead
			for (int j = 0; j < vertex->m_vecWorldNormal.size(); ++j)
			{
				vertex->m_vecWorldNormal[j] += vertex->m_vecNormalPosition[j] * joint->localMeshMatrixInvert * joint->world_matrix* vertex->m_vecJointWeigth[i];
			}
		}
	}
	else
	{//this should not happen
		vertex->worldPosition = vertex->meshPosition;
		for (int i = 0; i < vertex->m_vecVertexData.size(); ++i)
		{
			vertex->m_vecWorldNormal[i] = vertex->m_vecNormalPosition[i];
		}
	}
	for (std::map<int8, std::vector<int32>>::iterator ite = vertex->m_mapMeshVertex.begin();
		ite != vertex->m_mapMeshVertex.end(); ++ite)
	{
		DexMesh* pMesh = FindMesh(ite->first);
		for (int k = 0; k < ite->second.size(); ++k)
		{
			pMesh->m_vecVertexs[ite->second[k]].SetPos(vertex->worldPosition);
			vertex->m_vecWorldNormal[k] = vertex->m_vecWorldNormal[k] - vertex->worldPosition;
			vertex->m_vecWorldNormal[k].Normalize();
			pMesh->m_vecVertexs[ite->second[k]].SetNormal(vertex->m_vecWorldNormal[k]);
		}
	}
	//for (int i = 0; i < vertex->m_vecVertexData.size(); ++i)
	//{
	//	vertex->m_vecVertexData[i]->SetPos(vertex->worldPosition);
	//	vertex->m_vecWorldNormal[i] = vertex->m_vecWorldNormal[i] - vertex->worldPosition;
	//	vertex->m_vecWorldNormal[i].Normalize();
	//	vertex->m_vecVertexData[i]->SetNormal(vertex->m_vecWorldNormal[i]);
	//}
	return vertex->worldPosition;
}

void DexSkinMesh::AddJointInfo(MeshVertex* vertex, int16 jointId, float32 weight)
{
	//repeat check
	for (size_t i = 0; i < vertex->m_vecJointId.size(); ++i)
	{
		if (vertex->m_vecJointId[i] == jointId)
			//alread exist
			return;
	}
	vertex->m_vecJointId.push_back(jointId);
	vertex->m_vecJointWeigth.push_back(weight);
}

bool DexSkinMesh::FindVertex(const DexVector3& pos)
{
	for (size_t i = 0; i < m_vecMeshVertexs.size(); ++i)
	{
		if (m_vecMeshVertexs[i]->meshPosition == pos)
		{
			return true;
		}
	}
	return false;
}