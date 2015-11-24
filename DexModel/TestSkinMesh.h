


#include <d3dx9math.h>
#include <vector>
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexMatrix.h"

class Joint;
class MeshVertex
{
public:
	DexMatrix4x4 mesh_matrix; //mesh空间坐标
	DexMatrix4x4 world_matrix; //世界坐标
	std::vector<Joint*>  Joints;
	std::vector<float>  JointWeigth;
public:
	MeshVertex()
	{
	}
public:
	DexMatrix4x4 ComputeWorldMatrix()
	{
		world_matrix.Reset();
		for (size_t i = 0; i < Joints.size(); ++i)
		{
			world_matrix += Joints[i]->world_matrix*mesh_matrix*JointWeigth[i];
		}
		return world_matrix;
	}
};
class Joint
{
public:
	int16	   id;
	DexMatrix4x4 world_matrix; //最终世界matrix
	DexMatrix4x4 father_matrix; //在父坐标系下的matrix
	Joint*	   father;
	std::vector<Joint*> children;

public:
	void AddChild(Joint* child)
	{
		children.push_back(child);
		child->father = this;
	}
	DexMatrix4x4 ComputeWorldMatrix()
	{
		world_matrix = father->world_matrix * father_matrix;
		return world_matrix;
	}
};
class TestSkinMesh
{
public:
	Joint*	root_joint;

	std::vector<Joint*>	joints;

	
	int32		vertex_count;
	MeshVertex* vertex;

public:
	bool AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix)
	{
		bool ret = false;
		if (father_id == -1)
		{//root joint
			Joint* root= new Joint;
			root->father = NULL;
			root->id = id;
			root->father_matrix = father_matrix;
			joints.push_back(root);
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
	void AddVertex(const DexMatrix4x4& mesh_matrix, int jointid1 = -1, int jointid2 = -1)
	{
	}
};