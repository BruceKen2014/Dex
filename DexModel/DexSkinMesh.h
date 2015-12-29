
/* DexEngine&SkinMesh
   by Bruce
*/


#include <d3dx9math.h>
#include <map>
#include <string>
#include "../DexBase/DexMaterial.h"
#include "../DexBase/DexVertex.h"
#include "../DexBase/DexSTLVector.h"
#include "../DexMath/DexVector2.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexQuaternion.h"
#include "../DexMath/DexMatrix.h"

#include "DexModelBase.h"

//渲染关节点
#define SKINMESH_RENDER_JOINT		  1<<0		
//渲染关节到关节连线
#define SKINMESH_RENDER_JOINT2JOINT   1<<1
//渲染MESH
#define SKINMESH_RENDER_MESH		  1<<2
//渲染顶点到绑定关节的连线
#define SKINMESH_RENDER_VERTEX2JOINT  1<<3 
//渲染顶点法线
#define SKINMESH_RENDER_VERTEXNORMAL  1<<4
//渲染所有
#define SKINMESH_RENDER_ALL_FLAGS  (SKINMESH_RENDER_JOINT | \
	SKINMESH_RENDER_JOINT2JOINT | SKINMESH_RENDER_MESH | SKINMESH_RENDER_VERTEX2JOINT|\
	SKINMESH_RENDER_VERTEXNORMAL)

typedef enum
{
	SkinMeshAnimateType_OneTime,//只播放一次
	SkinMeshAnimateType_Loop,//循环播放
	SkinMeshAnimateType_OneTime_Back, //反向播放一次
	SkinMeshAnimateType_Loop_Back,//反向循环播放
	SkinMeshAnimateType_Total
}SkinMeshAnimateType;
typedef enum
{
	SkinMeshModelType_UnKnown, //unknown
	SkinMeshModelType_Dex,	   //dex引擎格式，目前尚未实现
	SkinMeshModelType_MS3D,    //.ms3d
	SkinMeshModelType_OBJ,	   //.obj
	SkinMeshModelType_X,       //.X
	SkinMeshModelType_Total
}SkinMeshModelType;
class CDexTex;
//根节点ID
#define _SKIN_MESH_ROOT_JOINT_ID	0x0FFF
class DexSkinMesh: public DexModelBase
{
	Dex_DeclareClass(DexSkinMesh, 0)
	friend class DexModelMs3dLoader;
	friend class DexModelObjLoader;
	class Joint
	{
	public:
		typedef struct _matrix_key
		{//<毫秒，矩阵>
			int32 time;
			DexMatrix4x4 matrix;
			_matrix_key() :time(0){}
			_matrix_key(int32 t, const DexMatrix4x4& m) :time(t), matrix(m){}
		}matrix_key;
	public:
		std::string	   str_name;
		int16		   id;
		bool		   m_bAnimate;
		Joint*		   m_pFather;
		DexMatrix4x4   world_matrix; //最终世界matrix
		DexMatrix4x4   frame_matrix; //每一帧在父坐标系下的matrix

		//初始化之后，运行过程中不再改变
		DexMatrix4x4   meshMatrix;	//mesh空间矩阵
		DexMatrix4x4   localMeshMatrixInvert; //关节在模型空间的逆矩阵，保存一次即可
		Vector<matrix_key> m_vecKeyFrames;
		Vector<Joint*> m_vecChildren;

	public:
		Joint();
		~Joint();
		void AddChild(Joint* child);
		//先按照时间从小到大插入
		void AddKeyFrame(int32 time, const DexMatrix4x4& matrix);
		void Render();
		void Render2Joint();
		void Update(int time);
		void ComputeWorldMatrix(int32 time);
	};
	class DexMesh;
	class MeshVertex
	{
		friend class DexSkinMesh;
		friend class DexModelObjLoader;
		friend class DexModelMs3dLoader;
	protected:
		DexVector3 meshPosition; //mesh空间坐标
		DexVector3 worldPosition;
		Vector<DexVector3> m_vecWorldNormal;
		Vector<DexVector3> m_vecNormalPosition; //skinmesh 空间中相对于各个骨骼的normal坐标
		Vector<stVertex3*>  m_vecVertexData;
		Vector<Joint*>		m_vecJoints;
		VectorFloat32   m_vecJointWeigth;
		std::map<DexMesh*, VectorInt32> m_mapMeshVertex; //对应于某个mesh里面的顶点索引

	public:
		MeshVertex()
		{
			m_vecWorldNormal.clear();
			m_vecNormalPosition.clear();
			m_vecVertexData.clear();
			m_vecJoints.clear();
			m_vecJointWeigth.clear();
		}
		~MeshVertex(){}
	};
	class DexMesh
	{		/*目前渲染时，将indices统一一次送给directx渲染，这样的话渲染三角形和渲染线段必须用不同的indice,
			有点浪费空间，如果保存的是三角形信息，三角形里面保存的是顶点索引，这样就不用保存两份indice，但是
			这样渲染模型必须一个一个三角形渲染，效率会有问题。现在省空间的做法是，如果选择线段渲染，那么由三角形
			的indice生成一份线段indice，不选择线段渲染时将线段indice删除掉
			*/
	public:
		char		name[64];
		int8	    id;
		int8		m_iTextureId;
		int8		m_iMaterialId;
		Vector<stVertex3>	m_vecVertexs; //用于向device传输要渲染的顶点信息
		VectorInt32		m_vecIndices;//vertex indice
		VectorInt32		m_vecLineIndices;//line vertex indice
	public:
		DexMesh();
		~DexMesh();
	public:
		void   DestroyLineIndices();
		int32* CreateLineIndices();
		uint32 GetVertexCount()	const;
		void*  GetVertexBuffer();
		uint32 GetIndiceCount() const;
		void*  GetIndiceBuffer();
		uint32 GetLineIndiceCount() const;
		void*  GetLineIndiceBuffer();
		//返回新插入顶点的index
		uint32 AddVertex(const DexVector3& pos, const DexVector3& normal, float u, float v);
		void   AddVertexIndice(const int32& index);
	};
protected:
	Joint*		m_pRootJoint;
	int16		m_iAnimateStartTime;
	int16		m_iAnimateNowTime;
	int16		m_iAnimateEndTime;
	int16		m_iAnimateMaxTime;
	int16		m_iRenderFlag;
	bool		m_bHaveAnimation;
	bool		m_bAnimate;
	float32		m_fAnimateRatio; //动画速率，默认1.0f
	SkinMeshAnimateType m_eAniType;
	SkinMeshModelType m_eMeshType;

	DexMatrix4x4 bindMatrix; //一个skinmesh被绑定在场景中的一个节点上，这是该节点的matrix
	Vector<DexMesh*>	 m_vecMeshs;		//mesh
	Vector<Joint*>		 m_vecJoints;		//joint
	Vector<MeshVertex*> m_vecMeshVertexs;	//vertex
	Vector<CDexTex*>	 m_vecTextures;		//texture
	Vector<DexMaterial> m_vecMaterials;	//material

public:
	DexSkinMesh();
	DexSkinMesh(int16 maxAniTime);
	~DexSkinMesh();
public:
//skin mesh
	//改变这个skinmesh的矩阵
	void SetSceneNodeMatrix(const DexMatrix4x4& matrix);
	virtual bool Update(int32 delta);
	virtual bool Render();
	//读取索引skinmesh数据后，务必调用此接口
	void CalculateVertex();
//model type
	void SetMeshType(SkinMeshModelType type);
	SkinMeshModelType GetMeshType() const;
//animation
	void SetMaxAniTime(int16 time);
	void SetAnimateEnable(bool animate);
	bool GetAnimateEnable()const;
	bool SetAnimateTime(int16 start, int16 end);
	SkinMeshAnimateType GetAnimateType()	const;
	void SetAnimateType(SkinMeshAnimateType type);
	void ReStartAnimation(); //如果是一次性的动画的话，重新播放当前动画
	int16 GetAnimateNowTime()const;
	int16 GetAnimateEndTime()const;
	int16 GetAnimateStartTime()const;
	float32 GetAnimateRatio() const;
	void  SetAnimateRatio(float32 ratio);
//mesh
	DexMesh* FindMesh(int8 meshId);
	DexMesh* FindMesh(const char* meshName);
	DexMesh* AddMesh(int8 meshId);
	DexMesh* AddMesh(const char* meshName);
	//设置对应mesh的顶点信息
	void SetMeshVertexs(int8 meshId, void* vertexs, int32 count);
	void SetMeshIndices(int8 meshId, void* indics, int32 count);
//joint by id
	Joint* AddJoint(int id); //只单纯添加一个关节，之后调用setjointinfo赋值相关数据
	Joint* SetJointInfo(int id, string name, string father_name, const DexMatrix4x4& father_matrix);
	//一次性添加骨骼及其数据
	Joint* AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(int32 jointId);
//joint by name
	//一些模型文件joint只有name没有id,则调用以下接口
	Joint* AddJoint(string name);
	Joint* SetJointInfo(string name, int father_id, const DexMatrix4x4& father_matrix);
	//这里传入的是在父节点中的相对矩阵，传入要计算根据此计算mesh空间矩阵
	Joint* AddJoint(string name, string father_name, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(string name);
//joint frame by id
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	//某一帧，某个节点相对父亲节点，只有位移的话，调用这个接口,pos是相对于父joint的位移
	bool AddJointKeyFrameTrans(int32 jointId, int32 time, const DexVector3& pos);
	//某一帧，某个节点相对父亲节点，只有缩放的话，调用这个接口,scale是相对于父joint的缩放
	bool AddJointKeyFrameScale(int32 jointId, int32 time, const DexVector3& scale);
	//某一帧，某个节点自身只有旋转的话，调用这个接口，节点没必要绕别的点旋转，任何绕别的点旋转的操作都可以转换到父亲节点自身的旋转
	//axis:绕自身的哪个轴旋转
	//radian:旋转的弧度
	bool AddJointKeyFrameRotation(int32 jointId, int32 time, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrameRotation(int32 jointId, int32 time, const DexQuaternion& qua);
//joint frame by name
	bool AddJointKeyFrame(string name, int32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(string name, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrame(string name, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	bool AddJointKeyFrameTrans(string name, int32 time, const DexVector3& pos);
	bool AddJointKeyFrameScale(string name, int32 time, const DexVector3& scale);
	bool AddJointKeyFrameRotation(string name, int32 time, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrameRotation(string name, int32 time, const DexQuaternion& qua);

//texture
	bool SetTexture(int8 meshId, int8 textureId);
	bool AddTexture(CDexTex* tex);
	bool AddTexture(const char* filename);
//material
	bool AddMaterial(const DexMaterial& material);

//vertex
	void AddVertex(const DexVector3& pos);
	//这里添加的顶点属于skinmesh，如果JointId=NULL,则绑定到根节点上
	void AddVertex(const DexVector3& pos, int16* JointId, float* weight, int16 jointCount);
	void AddVertexJointInfo(int32 vertexIndex, int16 jointId, float32 weight);//向一个已存在的顶点添加joint信息
	void AddJointInfo(MeshVertex* vertex, Joint* pJoint, float32 fWeight);
	//这里添加的顶点属于mesh，pos应当是已经在skinmesh中的，
	//如果pos不在skinmesh中，则向skinmesh添加pos顶点
	void AddVertex(int8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv);
	//add triangle, for mesh,3个index是对应于mesh已经存在的顶点索引
	void AddMeshTriangle(int8 meshId, int32 index0, int32 index1, int32 index2);
//render flag
	bool GetRenderFlag(int16 flag);
	void SetRenderFlag(int16 flag);
protected:
//joint key frame
	bool AddJointKeyFrame(Joint* joint, int32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(Joint* joint, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrame(Joint* joint, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	bool AddJointKeyFrameTrans(Joint* joint, int32 time, const DexVector3& pos);
	bool AddJointKeyFrameScale(Joint* joint, int32 time, const DexVector3& scale);
	bool AddJointKeyFrameRotation(Joint* joint, int32 time, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrameRotation(Joint* joint, int32 time, const DexQuaternion& qua);
//animation
	//return value:当前关节是否有动画，根据返回值来决定是否更新顶点
	void UpdateVertex();
	bool UpdateOneTime(int32 delta);
	bool UpdateLoop(int32 delta);
	bool UpdateOneTimeBack(int32 delta);
	bool UpdateLoopBack(int32 delta);
protected:
	//calculate position and normal
	//return position
	DexVector3 ComputeVertex(MeshVertex* vertex);
	//判断一个pos的顶点是否在skinmesh中
	bool	   FindVertex(const DexVector3& pos);
	virtual bool IsStaticModel();

};