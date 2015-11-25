


#include <d3dx9math.h>
#include <vector>
#include <map>
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexMatrix.h"
#include "../state/DexGameEngine.h"
class Joint;
class Joint
{
public:
	int16	   id;
	DexMatrix4x4 world_matrix; //最终世界matrix
	DexMatrix4x4 father_matrix; //在父坐标系下的matrix
	typedef struct _matrix_key
	{//<毫秒，矩阵>
		int32 time;
		DexMatrix4x4 matrix;
		_matrix_key(int32 t, const DexMatrix4x4& m) :time(t), matrix(m){}
	}matrix_key;
	std::vector<matrix_key> frames;	
	Joint*	   father;
	std::vector<Joint*> children;

public:
	Joint();
	void AddChild(Joint* child);
	//先按照时间从小到大插入
	void AddKeyFrame(int32 time, const DexMatrix4x4& matrix);
	void Render();
	void Update(int time);
	void ComputeWorldMatrix(int32 time);
};
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
		Joints.clear();
		JointWeigth.clear();
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
class TestSkinMesh
{
public:
	Joint*	root_joint;

	std::vector<Joint*>	joints;
	std::vector<MeshVertex*> vertexs;
	int32 AnimateNowTime;
	int32 AnimateMaxTime;

public:
	TestSkinMesh(int32 maxAniTime);
	void Update(int32 delta);
	void Render();
	Joint* FindJoint(int32 jointId);
	bool AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix);
	void AddVertex(const DexMatrix4x4& mesh_matrix, int jointid1 = -1, float weight1 = 1.0f, int jointid2 = -1, float weight2 = 0.0f);
};