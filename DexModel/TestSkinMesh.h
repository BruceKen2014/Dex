


#include <d3dx9math.h>
#include <vector>
#include <map>
#include "../DexBase/DexColor.h"
#include "../DexBase/DexVertex.h"
#include "../DexMath/DexVector2.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexMatrix.h"
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
	~Joint();
	void AddChild(Joint* child);
	//先按照时间从小到大插入
	void AddKeyFrame(int32 time, const DexMatrix4x4& matrix);
	void Render();
	void Update(int time);
	void ComputeWorldMatrix(int32 time);
	DexVector3 GetPosInMeshSpace()const;  //取得该joint在mesh空间的坐标
};
class MeshVertex
{
public:
	DexMatrix4x4 mesh_matrix; //mesh空间坐标
	DexMatrix4x4 world_matrix; //世界坐标
	std::vector<Joint*>  Joints;
	std::vector<DexMatrix4x4>  Joints_localMatrix; //mesh 空间中相对于各个骨骼的坐标
	std::vector<float>  JointWeigth;
	stVertex1* vertexData;
	DexVector2 uv;
public:
	MeshVertex()
	{
		Joints.clear();
		JointWeigth.clear();
		Joints_localMatrix.clear();
		vertexData = NULL;
	}
	~MeshVertex(){}
public:
	DexMatrix4x4 ComputeWorldMatrix();
	void BindJoint(Joint* joint, float weight);
};
class CDexTex;
class TestSkinMesh
{
public:
	Joint*		root_joint;
	int16		AnimateStartTime;
	int16		AnimateNowTime;
	int16		AnimateEndTime;
	int16		AnimateMaxTime;
	int32		indices_count;
	int32*		indices;
	CDexTex*	texture;
	stVertex1*	vertexs;
	std::vector<Joint*>		 joints;
	std::vector<MeshVertex*> mesh_vertexs;
	D3DMATERIAL9 material;
	

	
public:
	TestSkinMesh(int16 maxAniTime);
	~TestSkinMesh();
public:
	bool SetAnimateTime(int16 start, int16 end);
	void CalculateVertex();
	void Update(int32 delta);
	void Render();
	Joint* FindJoint(int32 jointId);
	bool AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix);
	bool SetTexture(CDexTex* tex);
	bool SetTexture(const char* filename);
	void AddVertex(const DexVector3& pos, const DexVector2& uv, int jointid1 = -1, float weight1 = 1.0f, int jointid2 = -1, float weight2 = 0.0f);
	void AddVertex(const DexMatrix4x4& mesh_matrix, int jointid1 = -1, float weight1 = 1.0f, int jointid2 = -1, float weight2 = 0.0f);
	void SetIndices(void* indics, int32 count);
};