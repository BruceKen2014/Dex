

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




TestSkinMesh::TestSkinMesh(int32 maxAniTime)
{
	AnimateNowTime = 0;
	AnimateMaxTime = maxAniTime;
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
void TestSkinMesh::AddVertex(const DexMatrix4x4& mesh_matrix, int jointid1 , float weight1 , int jointid2 , float weight2 )
{
	MeshVertex* vertex = new MeshVertex;
	vertex->mesh_matrix = mesh_matrix;
	Joint* joint1 = FindJoint(jointid1);
	Joint* joint2 = FindJoint(jointid2);
	if (jointid1 != NULL)
	{
		vertex->Joints.push_back(joint1);
		vertex->JointWeigth.push_back(weight1);
	}
	if (joint2 != NULL)
	{
		vertex->Joints.push_back(joint2);
		vertex->JointWeigth.push_back(weight2);
	}
	vertexs.push_back(vertex);
}