


#include <d3dx9math.h>
#include <vector>
#include "../DexMath/DexVector3.h"

class Joint;
class MeshVertex
{
public:
	D3DXMATRIX mesh_matrix; //mesh空间坐标
	D3DXMATRIX world_matrix; //世界坐标
	std::vector<Joint*>  Joints;
	std::vector<float>  JointWeigth;
public:
	MeshVertex()
	{
		memset(mesh_matrix, 0, sizeof(mesh_matrix));
		memset(world_matrix, 0, sizeof(world_matrix));
	}
public:
	D3DXMATRIX ComputeWorldMatrix()
	{
		memset(world_matrix, 0, sizeof(world_matrix));
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
	D3DXMATRIX world_matrix; //最终世界matrix
	D3DXMATRIX father_matrix; //在父坐标系下的matrix
	Joint*	   father;
	std::vector<Joint*> children;

public:
	void AddChild(Joint* child)
	{
		children.push_back(child);
		child->father = this;
	}
	D3DXMATRIX ComputeWorldMatrix()
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
	bool AddJoint(int id, int father_id, const D3DXMATRIX& father_matrix)
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
	void AddVertex(const D3DXMATRIX& mesh_matrix, int jointid1 = -1, int jointid2 = -1)
	{
	}
};