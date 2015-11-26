
#include "../state/DexGameEngine.h"
#include "TestSkinMesh.h"

Joint::Joint()
{
	frames.clear();
	children.clear();
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
		DexGameEngine::getEngine()->Render3DLine(father->world_matrix.GetPosition(), world_matrix.GetPosition());
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
	for (int i = 0; i < frames.size(); ++i)
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
	father_matrix = matrix1 + (matrix2 - matrix1) * ((float32)(time - time1)) / ((float32)(time2 - time1));
	if (father != NULL)
		world_matrix = father_matrix * father->world_matrix;
	else
		world_matrix = father_matrix;
}

DexVector3 Joint::GetPosInMeshSpace()const
{
	DexVector3 ret = father_matrix.GetPosition();
	if (father != NULL)
	{
		ret += father->GetPosInMeshSpace();
	}
	return ret;
}



DexMatrix4x4 MeshVertex::ComputeWorldMatrix()
{
	world_matrix.Reset();
	for (size_t i = 0; i < Joints.size(); ++i)
	{
		world_matrix += Joints_localMatrix[i] * Joints[i]->world_matrix* JointWeigth[i];
	}
	vertexData->m_pos.x = world_matrix.GetPosition().x; 
	vertexData->m_pos.y = world_matrix.GetPosition().y;
	vertexData->m_pos.z = world_matrix.GetPosition().z;
	return world_matrix;
}

void MeshVertex::BindJoint(Joint* joint, float weight)
{
	if (joint == NULL)	return;
	DexMatrix4x4 matrix = mesh_matrix;
	DexVector3 joint_mesh_pos = joint->GetPosInMeshSpace();
	matrix.Translate(joint_mesh_pos.GetInvert());
	Joints_localMatrix.push_back(matrix);
	JointWeigth.push_back(weight);
	Joints.push_back(joint);
}

TestSkinMesh::TestSkinMesh(int32 maxAniTime)
{
	AnimateNowTime = 0;
	AnimateMaxTime = maxAniTime;
	vertexs = NULL;
	indices = NULL;
}

void TestSkinMesh::CalculateVertex()
{
	if (vertexs != NULL)
	{
		free(vertexs);
	}
	vertexs = new stVertex0[mesh_vertexs.size()];
	for (size_t i = 0; i < mesh_vertexs.size(); ++i)
	{
		vertexs[i].m_color = getD3DColor(DexColor(1.0f, 1.0f, 1.0f, 1.0f));
		vertexs[i].m_pos.x = mesh_vertexs[i]->mesh_matrix.GetPosition().x;
		vertexs[i].m_pos.y = mesh_vertexs[i]->mesh_matrix.GetPosition().y;
		vertexs[i].m_pos.z = mesh_vertexs[i]->mesh_matrix.GetPosition().z;
		mesh_vertexs[i]->vertexData = &vertexs[i];
	}
}

void TestSkinMesh::Update(int32 delta)
{
	AnimateNowTime += delta;
	if (AnimateNowTime > AnimateMaxTime)
	{
		AnimateNowTime = AnimateMaxTime;
	}
	if (root_joint != NULL)
	{
		root_joint->Update(AnimateNowTime);
	}
	for (int i = 0; i < mesh_vertexs.size(); ++i)
	{
		if (mesh_vertexs[i] != NULL)
		{
			mesh_vertexs[i]->ComputeWorldMatrix();
		}
	}
	if (AnimateNowTime >= AnimateMaxTime)
	{
		AnimateNowTime = 0;
	}
}
void TestSkinMesh::Render()
{
	if (root_joint != NULL)
	{
		root_joint->Render();
	}
	DexGameEngine::getEngine()->DrawPrimitive(DexPT_TRIANGLELIST, vertexs, mesh_vertexs.size(), indices, indices_count/3, sizeof(stVertex0));
}
Joint* TestSkinMesh::FindJoint(int32 jointId)
{
	if (jointId == -1)
		return NULL;
	Joint* ret = NULL;
	for (int i = 0; i < joints.size(); ++i)
	{
		if (joints[i] != NULL && joints[i]->id == jointId)
		{
			ret = joints[i];
			break;
		}
	}
	return ret;
}
bool TestSkinMesh::AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix)
{
	bool ret = false;
	if (father_id == -1)
	{//root joint
		root_joint = new Joint;
		root_joint->father = NULL;
		root_joint->id = id;
		root_joint->father_matrix = father_matrix;
		joints.push_back(root_joint);
		ret = true;
	}
	else
	{
		for (size_t i = 0; i < joints.size(); ++i)
		{
			if (joints[i]->id == father_id)
			{
				Joint* joint = new Joint;
				joints[i]->AddChild(joint);
				joint->id = id;
				joint->father_matrix = father_matrix;
				joints.push_back(joint);
				ret = true;
				break;
			}
		}
	}
	return ret;
}
bool TestSkinMesh::AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix)
{
	Joint* joint = FindJoint(jointid);
	if (joint == NULL)
		return false;
	joint->AddKeyFrame(time, matrix);
	return true;
}
void TestSkinMesh::AddVertex(const DexVector3& pos, int jointid1 /* = -1 */, float weight1 /* = 1.0f */, int jointid2 /* = -1 */, float weight2 /* = 0.0f */)
{
	MeshVertex* vertex = new MeshVertex;
	DexMatrix4x4 mesh_matrix;
	mesh_matrix.Identity(); mesh_matrix.Translate(pos);
	vertex->mesh_matrix = mesh_matrix;
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
void TestSkinMesh::AddVertex(const DexMatrix4x4& mesh_matrix, int jointid1 , float weight1 , int jointid2 , float weight2 )
{
	MeshVertex* vertex = new MeshVertex;
	vertex->mesh_matrix = mesh_matrix;
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

void TestSkinMesh::SetIndices(void* indics, int32 count)
{
	if (indices != NULL)
	{
		free(indices);
	}
	indices_count = count;
	indices = (int32*)malloc(sizeof(int32)*count);
	memcpy(indices, indics, sizeof(int32) * count);
}