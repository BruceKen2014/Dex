
/* DexEngine&SkinMesh
   by Bruce
*/

#ifndef _DEX_MODEL_DEXSKINMESH_H
#define _DEX_MODEL_DEXSKINMESH_H


#include <d3dx9math.h>
#include <map>
#include <string>
#include "../DexBase/DexMaterial.h"
#include "../DexBase/DexVertex.h"
#include "../DexBase/DexDVector.h"
#include "../DexBase/DexString.h"
#include "../DexMath/DexVector2.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexQuaternion.h"
#include "../DexMath/DexMatrix.h"

#include "DexModelBase.h"
#include "DexShaderHlslSkinMesh.h"
#include "DexShaderHlslSkinMeshVertexToJoint.h"
#include "DexShaderHlslSkinMeshVertexNormal.h"

//skinmesh vertex can bind to 4 joints
#define  SKINMESH_VERTEX_JOINT_COUNT  4
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

class IDexVertexDecl;
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
	SkinMeshVertexBlend_VertexColor,
	SkinMeshVertexBlend_TexColor,
	SkinMeshVertexBlend_Blend,
	SkinMeshVertexBlend_Count
}SkinMeshVertexBlendType;
typedef enum
{
	SkinMeshModelType_UnKnown, //unknown
	SkinMeshModelType_Dex,	   //dex引擎格式，目前尚未实现
	SkinMeshModelType_MS3D,    //.ms3d
	SkinMeshModelType_OBJ,	   //.obj
	SkinMeshModelType_X,       //.X
	SkinMeshModelType_Total
}SkinMeshModelType;
class DexTexture;
//根节点ID
#define _SKIN_MESH_ROOT_JOINT_ID	0x0FFF
class DexSkinMesh: public DexModelBase
{
	Dex_DeclareClass(DexSkinMesh, DexModelBase,0)
	friend class DexModelMs3dLoader;
	friend class DexModelObjLoader;
	friend class DexModelDaeLoader;
	friend class DexModelSkinMeshLoader;
	friend class DexModelAniFileTextLoader;
	friend class DexShaderHlslSkinMesh;
	friend class DexShaderHlslSkinMeshVertexToJoint;
	friend class DexShaderHlslSkinMeshVertexNormal;
	class Joint
	{
	public:
		typedef struct _matrix_key
		{//<毫秒，矩阵>
			DInt32 time;
			DexMatrix4x4 matrix;
			_matrix_key() :time(0){}
			_matrix_key(DInt32 t, const DexMatrix4x4& m) :time(t), matrix(m){}
		}matrix_key;
	public:
		std::string	   str_name;
		DInt16		   id;
		bool		   m_bAnimate;
		Joint*		   m_pFather;
		DexMatrix4x4   world_matrix; //最终世界matrix
		DexMatrix4x4   frame_matrix; //每一帧在父坐标系下的matrix

		//初始化之后，运行过程中不再改变
		DexMatrix4x4   meshMatrix;	//mesh空间矩阵
		DexMatrix4x4   localMeshMatrixInvert; //关节在模型空间的逆矩阵，保存一次即可
		DVector<matrix_key> m_vecKeyFrames;
		DVector<Joint*> m_vecChildren;

	public:
		Joint();
		~Joint();
		void AddChild(Joint* child);
		//先按照时间从小到大插入
		void AddKeyFrame(DInt32 time, const DexMatrix4x4& matrix);
		void Render();
		void Render2Joint();
		void Update(int time);
		void ComputeWorldMatrix(DInt32 time);
	};
	typedef struct _stMeshVertex
	{
		DexVector3 pos;
		DexVector3 normal;
		DexColor   color;
		DexVector2 uv;
		DUInt8	   JointIndex[SKINMESH_VERTEX_JOINT_COUNT];
		DFloat32	   JointWeights[SKINMESH_VERTEX_JOINT_COUNT];
		_stMeshVertex() :color(DEXCOLOR_WHITE)
		{
			memset(JointIndex, 0, sizeof(JointIndex));
			memset(JointWeights, 0, sizeof(JointWeights));
			JointWeights[0] = 1.0f;
		}
	}stMeshVertex;
	class DexMesh
	{		/*目前渲染时，将indices统一一次送给directx渲染，这样的话渲染三角形和渲染线段必须用不同的indice,
			有点浪费空间，如果保存的是三角形信息，三角形里面保存的是顶点索引，这样就不用保存两份indice，但是
			这样渲染模型必须一个一个三角形渲染，效率会有问题。现在省空间的做法是，如果选择线段渲染，那么由三角形
			的indice生成一份线段indice，不选择线段渲染时将线段indice删除掉
			*/
	public:
		char		name[64];
		DInt8	    id;
		DInt8		m_iTextureId;
		DInt8		m_iMaterialId;

		////用于向device传输要渲染的顶点信息
		VectorDInt32		m_vecIndices;//vertex indice
		DVector<stMeshVertex> m_vecVertexsBuffer;
		VectorDInt32		m_vecLineIndices;//line vertex indice,用的buffer就是m_vecVertexsBuffer

		//用于debug 法线
		VectorDInt32		m_vecDebugNormalIndices;
		DVector<stMeshVertex> m_vecDebugNormalBuffer;

		//debug vertex to joint
		VectorDInt32		m_vecDebugVertexToJointIndices;
		DVector<stMeshVertex> m_vecDebugVertexToJointBuffer;
	public:
		DexMesh();
		virtual ~DexMesh();
	public:
		void   DestroyLineIndices();
		DInt32* CreateLineIndices();

		DUInt32 GetVertexCount()	const;
		void*  GetVertexBuffer();
		DUInt32 GetIndiceCount() const;
		void*  GetIndiceBuffer();

		DUInt32 GetLineIndiceCount() const;
		void*  GetLineIndiceBuffer();
	public:
		//用于debug normal
		void DestroyNormalBuffer();
		void CreateNormalBuffer();
		DUInt32 GetNormalBufferCount()	const;
		void*  GetNormalBuffer();
		DUInt32 GetNormalIndiceCount() const;
		void*  GetNormalIndiceBuffer();
	public:
		//debug vertex to joint
		void DestroyVertexToJointlBuffer();
		void CreateVertexToJointBuffer(const DVector<Joint*>& vecJoints);
		DUInt32 GetVertexToJointBufferCount()	const;
		void*  GetVertexToJointBuffer();
		DUInt32 GetVertexToJointIndiceCount() const;
		void*  GetVertexToJointIndiceBuffer();
	public:

		//返回新插入顶点的index
		DUInt32 AddVertex(const DexVector3& pos, const DexVector3& normal, float u, float v);
		void   AddVertexIndice(const DInt32& index);
	};
protected:
	Joint*		m_pRootJoint;
	DInt16		m_iAnimateStartTime;
	DInt16		m_iAnimateNowTime;
	DInt16		m_iAnimateEndTime;
	DInt16		m_iAnimateMaxTime;
	DInt16		m_iRenderFlag;
	bool		m_bHaveAnimation;
	bool		m_bAnimate;
	DFloat32		m_fAnimateRatio; //动画速率，默认1.0f
	SkinMeshAnimateType m_eAniType;
	SkinMeshModelType m_eMeshType;

	DexMatrix4x4 bindMatrix; //一个skinmesh被绑定在场景中的一个节点上，这是该节点的matrix
	DVector<DexMesh*>	 m_vecMeshs;		//mesh
	DVector<Joint*>		 m_vecJoints;		//joint
	DVector<DexTexture*>	 m_vecTextures;		//texture
	DVector<DexMaterial> m_vecMaterials;	//material
protected:
	D3DXMATRIX   matWVP;
	D3DXMATRIX* jointsMatrix;

	IDexShader* m_pShaderForMesh;
	IDexShader* m_pShaderForVertexToJoint;
	IDexShader* m_pShaderForVertexNormal;
	//模型受影响的灯光，暂时先放zhe里
	DexColor	 m_ambientColor;
	DVector<stDexPointLight>  m_vecPointLight;
	stDexDirectionLight m_directionLight;
	DFloat32		m_fJointScale; //骨骼缩放，用于调试渲染骨骼
	void Init();
	void InitShader();
public:
	void SetAmbientColor(const DexColor& color);
	void SetDirectionLight(const stDexDirectionLight& light);
	void ClearPointLight();
	void AddPointLight(const stDexPointLight& light);
public:
	DexSkinMesh();
	DexSkinMesh(DInt16 maxAniTime);
	virtual ~DexSkinMesh();
#ifdef _DEBUG
	// for debug
	DInt32 iHideMeshIndex;
	DInt32 iOnlyShowIndex;
#endif
public:
	static DUInt16 sGetMaxJointCount();
public:
//skin mesh
	//改变这个skinmesh的矩阵
	void SetSceneNodeMatrix(const DexMatrix4x4& matrix);
	virtual bool Update(DInt32 delta);
	virtual bool Render();
	//读取索引skinmesh数据后，务必调用此接口
	void CalculateVertex();
//model type
	void SetMeshType(SkinMeshModelType type);
	SkinMeshModelType GetMeshType() const;
//animation
	void SetMaxAniTime(DInt16 time);
	void SetAnimateEnable(bool animate);
	bool GetAnimateEnable()const;
	bool SetAnimateTime(DInt16 start, DInt16 end);
	SkinMeshAnimateType GetAnimateType()	const;
	void SetAnimateType(SkinMeshAnimateType type);
	void ReStartAnimation(); //如果是一次性的动画的话，重新播放当前动画
	DInt16 GetAnimateNowTime()const;
	DInt16 GetAnimateEndTime()const;
	DInt16 GetAnimateStartTime()const;
	DFloat32 GetAnimateRatio() const;
	void  SetAnimateRatio(DFloat32 ratio);
//mesh
	DexMesh* FindMesh(DInt8 meshId);
	DexMesh* FindMesh(const char* meshName);
	//根据传入的iMaterialIndex寻找mesh，如果没有，bAdd表示是否添加
	DexMesh* FindMesh(DUInt8 iMaterialIndex, bool bAdd);
	DexMesh* AddMesh(DInt8 meshId);
	DexMesh* AddMesh(const char* meshName);
	//设置对应mesh的顶点信息
	void SetMeshVertexs(DInt8 meshId, void* vertexs, DInt32 count);
	void SetMeshIndices(DInt8 meshId, void* indics, DInt32 count);
//joint by id
	Joint* AddJoint(int id); //只单纯添加一个关节，之后调用setjointinfo赋值相关数据
	Joint* SetJointInfo(int id, string name, string father_name, const DexMatrix4x4& father_matrix);
	//一次性添加骨骼及其数据
	Joint* AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(DInt32 jointId);
//joint by name
	//一些模型文件joint只有name没有id,则调用以下接口
	Joint* AddJoint(string name);
	Joint* SetJointInfo(string name, int father_id, const DexMatrix4x4& father_matrix);
	//这里传入的是在父节点中的相对矩阵，传入要计算根据此计算mesh空间矩阵
	Joint* AddJoint(string name, string father_name, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(string name);
	DUInt32 FindJointIndex(DString name);
//joint frame by id
	bool AddJointKeyFrame(DInt32 jointid, DInt32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(DInt32 jointid, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, DFloat32 radian);
	bool AddJointKeyFrame(DInt32 jointid, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	//某一帧，某个节点相对父亲节点，只有位移的话，调用这个接口,pos是相对于父joint的位移
	bool AddJointKeyFrameTrans(DInt32 jointId, DInt32 time, const DexVector3& pos);
	//某一帧，某个节点相对父亲节点，只有缩放的话，调用这个接口,scale是相对于父joint的缩放
	bool AddJointKeyFrameScale(DInt32 jointId, DInt32 time, const DexVector3& scale);
	//某一帧，某个节点自身只有旋转的话，调用这个接口，节点没必要绕别的点旋转，任何绕别的点旋转的操作都可以转换到父亲节点自身的旋转
	//axis:绕自身的哪个轴旋转
	//radian:旋转的弧度
	bool AddJointKeyFrameRotation(DInt32 jointId, DInt32 time, const DexVector3& axis, DFloat32 radian);
	bool AddJointKeyFrameRotation(DInt32 jointId, DInt32 time, const DexQuaternion& qua);
//joint frame by name
	bool AddJointKeyFrame(string name, DInt32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(string name, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, DFloat32 radian);
	bool AddJointKeyFrame(string name, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	bool AddJointKeyFrameTrans(string name, DInt32 time, const DexVector3& pos);
	bool AddJointKeyFrameScale(string name, DInt32 time, const DexVector3& scale);
	bool AddJointKeyFrameRotation(string name, DInt32 time, const DexVector3& axis, DFloat32 radian);
	bool AddJointKeyFrameRotation(string name, DInt32 time, const DexQuaternion& qua);

//texture
	bool SetTexture(DInt8 meshId, DInt8 textureId);
	bool AddTexture(DexTexture* tex);
	DInt32 AddTexture(const char* filename);
	DInt32 FindTexture(DString textureName);
//material 
	//添加material,返回新添加的material的index
	DInt32 AddMaterial(const DexMaterial& material);
	DInt32 FindMaterial(DString materialName);

//vertex
	void AddVertex(const DexVector3& pos);
	//这里添加的顶点属于skinmesh，如果JointId=NULL,则绑定到根节点上
	void AddVertex(const DexVector3& pos, DInt16* JointId, float* weight, DInt16 jointCount);
	//直接向mesh添加顶点
	void AddVertex(DInt8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv);
	void AddVertex(DInt8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv, DInt16* JointId, float* weight, DUInt16 jointCount);
	//add triangle, for mesh,3个index是对应于mesh已经存在的顶点索引
	void AddMeshTriangle(DInt8 meshId, DInt32 index0, DInt32 index1, DInt32 index2);
//render flag
	bool GetRenderFlag(DInt16 flag);
	void SetRenderFlag(DInt16 flag);
	void SetJointScale(DFloat32 fScale);
//Render Order
	void SetOrderInfo(const DVector<DInt32>& vector);//重置渲染順序
protected:
//joint key frame
	bool AddJointKeyFrame(Joint* joint, DInt32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(Joint* joint, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, DFloat32 radian);
	bool AddJointKeyFrame(Joint* joint, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	bool AddJointKeyFrameTrans(Joint* joint, DInt32 time, const DexVector3& pos);
	bool AddJointKeyFrameScale(Joint* joint, DInt32 time, const DexVector3& scale);
	bool AddJointKeyFrameRotation(Joint* joint, DInt32 time, const DexVector3& axis, DFloat32 radian);
	bool AddJointKeyFrameRotation(Joint* joint, DInt32 time, const DexQuaternion& qua);

protected:
	//判断一个pos的顶点是否在skinmesh中
	bool	   FindVertex(const DexVector3& pos);
	//animation
	bool UpdateOneTime(DInt32 delta);
	bool UpdateLoop(DInt32 delta);
	bool UpdateOneTimeBack(DInt32 delta);
	bool UpdateLoopBack(DInt32 delta);
	virtual bool IsStaticModel();
protected:
	void RenderMesh();
	void RenderVertexNormal();
	void RenderVertexToJoint();
};

#endif