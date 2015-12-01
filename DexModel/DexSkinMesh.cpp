


#include "../Source/CTexture.h"
#include "../state/DexGameEngine.h"
#include "DexSkinMesh.h"

Joint::Joint()
{
	frames.clear();
	children.clear();
}
Joint::~Joint()
{
}
void Joint::AddChild(Joint* child)
{
	children.push_back(child);
	child->father = this;
}
//先按照时间从小到大插入
void Joint::AddKeyFrame(int32 time, const DexMatrix4x4& matrix)
{
	frames.push_back(matrix_key(time, matrix));
}
void Joint::Render()
{
	DexGameEngine::getEngine()->RenderCube(world_matrix);
	if (father != NULL)
	{
		DexGameEngine::getEngine()->Render3DLine(father->world_matrix.GetPosition(), world_matrix.GetPosition(), DexColor(1.0f, 1.0f, 1.0f), DexColor(1.0f, 1.0f, 1.0f));
	}
	for (size_t i = 0; i < children.size(); ++i)
	{
		children[i]->Render();
	}
}
void Joint::Update(int time)
{
	ComputeWorldMatrix(time);
	for (size_t i = 0; i < children.size(); ++i)
	{
		children[i]->Update(time);
	}
}
void Joint::ComputeWorldMatrix(int32 time)
{
	float time1;
	float time2;
	DexMatrix4x4 matrix1;
	DexMatrix4x4 matrix2;
	for (size_t i = 0; i < frames.size(); ++i)
	{
		if (time > frames[i].time)
		{
			time1 = frames[i].time;
			matrix1 = frames[i].matrix;
			continue;
		}
		else
		{
			time2 = frames[i].time;
			matrix2 = frames[i].matrix;
			break;
		}
	}
	if (father != NULL)
	{
		frame_matrix = matrix1 + (matrix2 - matrix1) * ((float32)(time - time1)) / ((float32)(time2 - time1));
		world_matrix = frame_matrix * father->world_matrix;
	}
	else
		world_matrix = frame_matrix;
}

DexVector3 Joint::GetPosInMeshSpace()const
{
	DexVector3 ret = frame_matrix.GetPosition();
	if (father != NULL)
	{
		ret += father->GetPosInMeshSpace();
	}
	return ret;
}

DexVector3 MeshVertex::Compute()
{
	worldPosition.Set(0.0f, 0.0f, 0.0f);
	worldNormal.Set(0.0f, 0.0f, 0.0f);
	if (Joints.size() != 0)
	{//顶点已绑定骨骼
		
		for (size_t i = 0; i < Joints.size(); ++i)
		{
			worldPosition += Joints_localPos[i] * Joints[i]->world_matrix* JointWeigth[i];
			//now I havent find a method use matrix and normal to calculate world normal
			//just use a temp point instead
			worldNormal += Joints_localNormalPoint[i] * Joints[i]->world_matrix* JointWeigth[i];
		}
	}
	else
	{
		worldPosition = meshPosition;
		worldNormal = normalPosition;
	}
	vertexData->SetPos(worldPosition);
	worldNormal = worldNormal - worldPosition;
	worldNormal.Normalize();
	vertexData->SetNormal(worldNormal);
	return worldPosition;
}

void MeshVertex::BindJoint(Joint* joint, float weight)
{
	if (joint == NULL)	return;
	DexVector3 pos = meshPosition;
	DexVector3 norPos = normalPosition;
	DexVector3 joint_mesh_pos = joint->GetPosInMeshSpace();
	pos -= joint_mesh_pos;
	norPos -= joint_mesh_pos;
	Joints_localPos.push_back(pos);
	Joints_localNormalPoint.push_back(norPos);
	JointWeigth.push_back(weight);
	Joints.push_back(joint);
}
DexSkinMesh::DexMesh::DexMesh()
{
	id = 0;
	textureId = -1;
	materialId = -1;
	indices_count = 0;
	indices = NULL;
	indices_count_line = 0;
	indices_line = NULL;
}
DexSkinMesh::DexMesh::~DexMesh()
{
	_SafeFree(indices);
	_SafeFree(indices_line);
}

void DexSkinMesh::DexMesh::DestroyLineIndices()
{
	_SafeFree(indices_line);
}
int32* DexSkinMesh::DexMesh::CreateLineIndices()
{
	_SafeFree(indices_line);
	indices_count_line = indices_count * 2;
	indices_line = (int32*)malloc(sizeof(int32)* indices_count_line);
	int32 line_indice_index = 0;
	for (int i = 0; i+2 < indices_count;)
	{//3个顶点为一个三角形
		indices_line[line_indice_index++] = indices[i];
		indices_line[line_indice_index++] = indices[i+1];

		indices_line[line_indice_index++] = indices[i];
		indices_line[line_indice_index++] = indices[i+2];

		indices_line[line_indice_index++] = indices[i + 1];
		indices_line[line_indice_index++] = indices[i + 2];
		i = i + 3;
	}
	return indices_line;
}

DexSkinMesh::DexSkinMesh()
{
	AnimateStartTime = 0;
	AnimateNowTime = 0;
	AnimateEndTime = 0;
	AnimateMaxTime = 0;
	root_joint = NULL;
	vertexs = NULL;
	m_bHaveAnimation = false;
	m_bAnimate = false;
	m_bLightFlag = true;

	root_joint = new Joint;
	root_joint->father = NULL;
	root_joint->id = _SKIN_MESH_ROOT_JOINT_ID;
	root_joint->frame_matrix.Identity();
	joints.push_back(root_joint);
}
DexSkinMesh::DexSkinMesh(int16 maxAniTime)
{
	AnimateStartTime = 0;
	AnimateNowTime = 0;
	AnimateEndTime = maxAniTime;
	AnimateMaxTime = maxAniTime;
	root_joint = NULL;
	vertexs = NULL;
	m_bHaveAnimation = true;
	m_bAnimate = true;
	root_joint = new Joint;
	root_joint->father = NULL;
	root_joint->id = _SKIN_MESH_ROOT_JOINT_ID;
	root_joint->frame_matrix.Identity();
	joints.push_back(root_joint);
}
DexSkinMesh::~DexSkinMesh()
{
	_SafeFree(vertexs);
	_SafeClearVector(joints);
	_SafeClearVector(mesh_vertexs);
	_SafeClearVector(vec_Meshs);
	vec_material.clear();
	for (int i = 0; i < vec_texture.size(); ++i)
	{
		_SafeReduceRef(vec_texture[i]);
	}
	vec_texture.clear();
}

void DexSkinMesh::SetSceneNodeMatrix(const DexMatrix4x4& matrix)
{
	bindMatrix = matrix;
	root_joint->frame_matrix = bindMatrix; 
	root_joint->world_matrix = bindMatrix;
}

void DexSkinMesh::SetMaxAniTime(int16 time)
{
	AnimateMaxTime = time;
	m_bHaveAnimation = true;
	m_bAnimate = true;
}
bool DexSkinMesh::SetAnimateTime(int16 start, int16 end)
{
	bool ret = false;
	if (start >= 0 && start <= end && end <= AnimateMaxTime)
	{
		AnimateStartTime = start;
		AnimateEndTime = end;
		ret = true;
	}
	return ret;
}

void DexSkinMesh::SetAnimateEnable(bool animate)
{
	m_bAnimate = animate;
}

void DexSkinMesh::CalculateVertex()
{
	//如果是一个没有骨骼的静态模型
	//所有的顶点绑定到原点的根节点上
	_SafeFree(vertexs);
	vertexs = new stVertex3[mesh_vertexs.size()];
	bool n_root = joints.size() == 1;
	for (size_t i = 0; i < mesh_vertexs.size(); ++i)
	{
		vertexs[i].m_u = mesh_vertexs[i]->uv.x;
		vertexs[i].m_v = mesh_vertexs[i]->uv.y;
		vertexs[i].color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 1.0f));
		vertexs[i].SetPos(mesh_vertexs[i]->worldPosition);
		vertexs[i].SetNormal(mesh_vertexs[i]->worldNormal);
		mesh_vertexs[i]->vertexData = &vertexs[i];
		if (n_root)
		{
			mesh_vertexs[i]->BindJoint(root_joint, 1.0f);
		}
	}
}

bool DexSkinMesh::Update(int32 delta)
{
	DEX_ENSURE_B(DexModelBase::Update(delta));
	if (CheckAnimate())
	{
		AnimateNowTime += delta;
		if (AnimateNowTime > AnimateEndTime)
		{
			AnimateNowTime = AnimateEndTime;
		}
		if (root_joint != NULL)
		{
			root_joint->Update(AnimateNowTime);
		}
		if (AnimateNowTime >= AnimateEndTime)
		{
			AnimateNowTime = AnimateStartTime;
		}
	}
	for (size_t i = 0; i < mesh_vertexs.size(); ++i)
	{
		if (mesh_vertexs[i] != NULL)
		{
			mesh_vertexs[i]->Compute();
		}
	}
	return true;
}
bool DexSkinMesh::Render()
{
	DEX_ENSURE_B(DexModelBase::Render());
	if (root_joint != NULL)
	{
		root_joint->Render();
	}
	for (size_t i = 0; i < mesh_vertexs.size(); ++i)
	{
		DexGameEngine::getEngine()->Render3DLine(mesh_vertexs[i]->worldPosition, mesh_vertexs[i]->worldNormal, DexColor(1.0f, 0.0f, 1.0f), 10.0f, DexColor(1.0f, 0.0f, 1.0f));
	}
	bool enabelLight = DexGameEngine::getEngine()->GetLightEnable();
	DexGameEngine::getEngine()->SetLightEnable(enabelLight && m_bLightFlag);
	for (size_t i = 0; i < vec_Meshs.size(); ++i)
	{
		if (vec_Meshs[i] == NULL)
			continue;
		if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_LINE)
		{
			DexGameEngine::getEngine()->SetLightEnable(false);
			if (vec_Meshs[i]->indices_line == NULL)
			{
				vec_Meshs[i]->CreateLineIndices();
			}
			DexGameEngine::getEngine()->DrawPrimitive(DexPT_LINELIST, vertexs, mesh_vertexs.size(), vec_Meshs[i]->indices_line,
				vec_Meshs[i]->indices_count_line / 2, FVF_XYZ_COLOR_T1_N, sizeof(stVertex3));
		}
		else if (DexGameEngine::getEngine()->GetRenderMode() == DexRenderMode_TRIANGLE)
		{
			if (vec_Meshs[i]->indices_line != NULL)
			{
				vec_Meshs[i]->DestroyLineIndices();
			}
			DexGameEngine::getEngine()->SetMaterial(vec_material[vec_Meshs[i]->materialId]);
			DexGameEngine::getEngine()->SetTexture(0, vec_texture[vec_Meshs[i]->textureId]);
			DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, vertexs, mesh_vertexs.size(), vec_Meshs[i]->indices,
				vec_Meshs[i]->indices_count / 3, FVF_XYZ_COLOR_T1_N, sizeof(stVertex3));
		}

	}
	DexGameEngine::getEngine()->SetLightEnable(enabelLight);
	DexGameEngine::getEngine()->SetTexture(0, NULL);
	return true;
}
Joint* DexSkinMesh::FindJoint(int32 jointId)
{
	if (jointId == -1)
		return NULL;
	Joint* ret = NULL;
	for (size_t i = 0; i < joints.size(); ++i)
	{
		if (joints[i] != NULL && joints[i]->id == jointId)
		{
			ret = joints[i];
			break;
		}
	}
	return ret;
}
DexSkinMesh::DexMesh* DexSkinMesh::FindMesh(int8 meshId)
{
	DexMesh* ret = NULL;
	for (size_t i = 0; i < vec_Meshs.size(); ++i)
	{
		if (vec_Meshs[i] != NULL && vec_Meshs[i]->id == meshId)
		{
			ret = vec_Meshs[i];
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
	vec_Meshs.push_back(mesh);
	return mesh;
}
bool DexSkinMesh::AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix)
{
	bool ret = false;
	Joint* joint = new Joint;
	joint->id = id;
	joint->frame_matrix = father_matrix;
	for (size_t i = 0; i < joints.size(); ++i)
	{
		if (joints[i]->id == father_id)
		{
			joints[i]->AddChild(joint);
			ret = true;
			break;
		}
	}
	joints.push_back(joint);
	if (!ret)
	{//没有找到已经存在的joint,添加到root_joint中去
		root_joint->AddChild(joint);
	}
	return ret;
}
bool DexSkinMesh::AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix)
{
	Joint* joint = FindJoint(jointid);
	DEX_ENSURE_B(joint);
	joint->AddKeyFrame(time, matrix);
	return true;
}

bool DexSkinMesh::SetTexture(int8 meshId, int8 textureId)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE_B(mesh);
	mesh->textureId = textureId;
	return true;
}

bool DexSkinMesh::AddTexture(CDexTex* tex)
{
	tex->AddRef();
	vec_texture.push_back(tex);
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
	vec_texture.push_back(tex);
	return true;
}

bool DexSkinMesh::AddMaterial(const DexMaterial& material)
{
	vec_material.push_back(material);
	return true;
}

void DexSkinMesh::AddVertex(const DexVector3& pos)
{
	MeshVertex* vertex = new MeshVertex;
	vertex->meshPosition = pos;
	vertex->worldPosition = pos;
	mesh_vertexs.push_back(vertex);
}

bool DexSkinMesh::SetVertexNormal(int32 vertexIndex, float x, float y, float z)
{
	DEX_ENSURE_B(vertexIndex < mesh_vertexs.size()
		&& mesh_vertexs[vertexIndex] != NULL);
	DexVector3 _normal(x,y,z);
	mesh_vertexs[vertexIndex]->normalPosition = mesh_vertexs[vertexIndex]->meshPosition + _normal * 10.0f;
	mesh_vertexs[vertexIndex]->worldNormal = _normal;
	return true;
}

bool DexSkinMesh::SetVertexNormal(int32 vertexIndex, const DexVector3& normal)
{
	DEX_ENSURE_B(vertexIndex < mesh_vertexs.size()
		&& mesh_vertexs[vertexIndex] != NULL);
	DexVector3 _normal = normal;
	mesh_vertexs[vertexIndex]->normalPosition = mesh_vertexs[vertexIndex]->meshPosition + _normal * 10.0f;
	mesh_vertexs[vertexIndex]->worldNormal = _normal;
	return true;
}

bool DexSkinMesh::SetVertexUv(int32 vertexIndex, float32 u, float v)
{
	DEX_ENSURE_B(vertexIndex < mesh_vertexs.size()
		&& mesh_vertexs[vertexIndex] != NULL);
	mesh_vertexs[vertexIndex]->uv.x = u;
	mesh_vertexs[vertexIndex]->uv.y = v;
	return true;
}

bool DexSkinMesh::SetVertexUv(int32 vertexIndex, const DexVector2& uv)
{
	DEX_ENSURE_B(vertexIndex < mesh_vertexs.size()
		&& mesh_vertexs[vertexIndex] != NULL);
	mesh_vertexs[vertexIndex]->uv = uv;
	return true;
}

void DexSkinMesh::AddVertex(const DexVector3& pos, const DexVector3& normal, const DexVector2& uv, int jointid1 /* = -1 */, float weight1 /* = 1.0f */, int jointid2 /* = -1 */, float weight2 /* = 0.0f */)
{
	DexVector3 _normal = normal;
	_normal.Normalize();
	MeshVertex* vertex = new MeshVertex;
	vertex->meshPosition = pos;
	vertex->worldPosition = pos;
	vertex->normalPosition = pos + _normal * 10.0f;
	vertex->worldNormal = _normal; 
	vertex->uv = uv;
	
	Joint* joint1 = FindJoint(jointid1);
	Joint* joint2 = FindJoint(jointid2);
	if (joint1 != NULL)
	{
		vertex->BindJoint(joint1, weight1);
	}
	if (joint2 != NULL)
	{
		vertex->BindJoint(joint2, weight2);
	}
	mesh_vertexs.push_back(vertex);
}

void DexSkinMesh::SetIndices(int8 meshId, void* indics, int32 count)
{
	DexMesh* mesh = FindMesh(meshId);
	DEX_ENSURE(mesh);
	if (mesh->indices != NULL)
	{
		free(mesh->indices);
	}
	mesh->indices_count = count;
	mesh->indices = (int32*)malloc(sizeof(int32)*count);
	memcpy(mesh->indices, indics, sizeof(int32)* count);
}

bool DexSkinMesh::CheckAnimate()
{
	return m_bHaveAnimation && m_bAnimate;
}