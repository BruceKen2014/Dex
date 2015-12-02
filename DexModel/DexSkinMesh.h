


#include <d3dx9math.h>
#include <vector>
#include <map>
#include "../DexBase/DexMaterial.h"
#include "../DexBase/DexVertex.h"
#include "../DexMath/DexVector2.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexQuaternion.h"
#include "../DexMath/DexMatrix.h"

#include "DexModelBase.h"
class Joint;
class Joint
{
public:
	int16	   id;
	DexMatrix4x4 world_matrix; //最终世界matrix
	DexMatrix4x4 frame_matrix; //每一帧在父坐标系下的matrix
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
	friend class DexSkinMesh;
protected:
	DexVector3 meshPosition; //mesh空间坐标
	DexVector3 worldPosition; //世界坐标
	DexVector3 normalPosition;
	DexVector3 worldNormal; 
	stVertex3* vertexData;
	DexVector2 uv;
	std::vector<Joint*>  Joints;
	std::vector<DexVector3>  Joints_localPos; //mesh 空间中相对于各个骨骼的坐标
	std::vector<DexVector3>  Joints_localNormalPoint; //mesh 空间中相对于各个骨骼的normal坐标
	std::vector<float>  JointWeigth;

public:
	MeshVertex()
	{
		Joints.clear();
		JointWeigth.clear();
		vertexData = NULL;
		normalPosition.Set(0.0f, 1.0f, 0.0f);
	}
	~MeshVertex(){}
public:
	//calculate position and normal
	//return position
	DexVector3 Compute();
	void BindJoint(Joint* joint, float weight);
};
class CDexTex;
/*
一个模型的skinmesh可能包含多个mesh，比如一个拿着砍刀的熊猫，熊猫本身是一个mesh，有着对应的材质和贴图，
而砍刀是另一个mesh，有着另外一套材质贴图,但是公用一份顶点数据,每个mesh有着不同的indice
这种情况是就一个整体的skinmesh来说的，对于一个可以持有不同的砍刀的熊猫来说，熊猫和砍刀应该不属于同一个
skinmesh，只不过把砍刀这个独立的skinmesh绑定到了熊猫这个skinmesh的某个骨骼上
*/
//根节点ID
#define _SKIN_MESH_ROOT_JOINT_ID	0x0FFFFFFF
class DexSkinMesh: public DexModelBase
{
	Dex_DeclareClass(DexSkinMesh, 0)
	friend class DexModelMs3dLoader;
	class DexMesh
	{		/*目前渲染时，将indices统一一次送给directx渲染，这样的话渲染三角形和渲染线段必须用不同的indice,
			有点浪费空间，如果保存的是三角形信息，三角形里面保存的是顶点索引，这样就不用保存两份indice，但是
			这样渲染模型必须一个一个三角形渲染，效率会有问题。现在省空间的做法是，如果选择线段渲染，那么由三角形
			的indice生成一份线段indice，不选择线段渲染时将线段indice删除掉
			*/
	public:
		int8	    id;
		int8		textureId;
		int8		materialId;
		int32		indices_count;
		int32*		indices;
		int32		indices_count_line;
		int32*      indices_line;

	public:
		DexMesh();
		~DexMesh();
	public:
		void DestroyLineIndices();
		int32* CreateLineIndices();
	};
protected:
	Joint*		root_joint;
	int16		AnimateStartTime;
	int16		AnimateNowTime;
	int16		AnimateEndTime;
	int16		AnimateMaxTime;
	stVertex3*	vertexs;
	bool		m_bHaveAnimation; 
	bool		m_bAnimate;
	DexMatrix4x4 bindMatrix; //一个skinmesh被绑定在场景中的一个节点上，这是该节点的matrix
	std::vector<DexMesh*> vec_Meshs;
	std::vector<CDexTex*> vec_texture;
	std::vector<DexMaterial> vec_material;
	std::vector<Joint*>		 joints;
	std::vector<MeshVertex*> mesh_vertexs;
public:
	DexSkinMesh();
	DexSkinMesh(int16 maxAniTime);
	~DexSkinMesh();
public:
	void SetSceneNodeMatrix(const DexMatrix4x4& matrix);
	void SetMaxAniTime(int16 time);
	void SetAnimateEnable(bool animate);
	bool SetAnimateTime(int16 start, int16 end);
	void CalculateVertex();
	virtual bool Update(int32 delta);
	virtual bool Render();
	Joint* FindJoint(int32 jointId);
	DexMesh* FindMesh(int8 meshId);
	DexMesh* AddMesh(int8 meshId);
	bool AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	//某一帧，某个节点相对父亲节点，只有位移的话，调用这个接口
	//pos是相对于父joint的位移
	bool AddJointKeyFrameTrans(int32 jointId, int32 time, const DexVector3& pos);
	//某一帧，某个节点相对父亲节点，只有缩放的话，调用这个接口
	//scale是相对于父joint的缩放
	bool AddJointKeyFrameScale(int32 jointId, int32 time, const DexVector3& scale);
	//某一帧，某个节点自身只有旋转的话，调用这个接口，节点没必要绕别的点旋转，任何绕别的点旋转的操作都可以转换到父亲节点自身的旋转
	//axis:绕自身的哪个轴旋转
	//radian:旋转的弧度
	bool AddJointKeyFrameRotation(int32 jointId, int32 time, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrameRotation(int32 jointId, int32 time, const DexQuaternion& qua);

	bool SetTexture(int8 meshId, int8 textureId);
	bool AddTexture(CDexTex* tex);
	bool AddTexture(const char* filename);
	bool AddMaterial(const DexMaterial& material);

	void AddVertex(const DexVector3& pos);
	bool SetVertexNormal(int32 vertexIndex, float x, float y, float z);
	bool SetVertexNormal(int32 vertexIndex, const DexVector3& normal);
	bool SetVertexUv(int32 vertexIndex, float32 u, float v);
	bool SetVertexUv(int32 vertexIndex, const DexVector2& uv);
	void AddVertex(const DexVector3& pos, const DexVector3& normal, const DexVector2& uv, int jointid1 = -1, float weight1 = 1.0f, int jointid2 = -1, float weight2 = 0.0f);
	
	void SetIndices(int8 meshId, void* indics, int32 count);
protected:
	virtual bool CheckAnimate();
};