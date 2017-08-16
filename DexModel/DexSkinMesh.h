
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
//��Ⱦ�ؽڵ�
#define SKINMESH_RENDER_JOINT		  1<<0		
//��Ⱦ�ؽڵ��ؽ�����
#define SKINMESH_RENDER_JOINT2JOINT   1<<1
//��ȾMESH
#define SKINMESH_RENDER_MESH		  1<<2
//��Ⱦ���㵽�󶨹ؽڵ�����
#define SKINMESH_RENDER_VERTEX2JOINT  1<<3 
//��Ⱦ���㷨��
#define SKINMESH_RENDER_VERTEXNORMAL  1<<4
//��Ⱦ����
#define SKINMESH_RENDER_ALL_FLAGS  (SKINMESH_RENDER_JOINT | \
	SKINMESH_RENDER_JOINT2JOINT | SKINMESH_RENDER_MESH | SKINMESH_RENDER_VERTEX2JOINT|\
	SKINMESH_RENDER_VERTEXNORMAL)

class IDexVertexDecl;
typedef enum
{
	SkinMeshAnimateType_OneTime,//ֻ����һ��
	SkinMeshAnimateType_Loop,//ѭ������
	SkinMeshAnimateType_OneTime_Back, //���򲥷�һ��
	SkinMeshAnimateType_Loop_Back,//����ѭ������
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
	SkinMeshModelType_Dex,	   //dex�����ʽ��Ŀǰ��δʵ��
	SkinMeshModelType_MS3D,    //.ms3d
	SkinMeshModelType_OBJ,	   //.obj
	SkinMeshModelType_X,       //.X
	SkinMeshModelType_Total
}SkinMeshModelType;
class DexTexture;
//���ڵ�ID
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
		{//<���룬����>
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
		DexMatrix4x4   world_matrix; //��������matrix
		DexMatrix4x4   frame_matrix; //ÿһ֡�ڸ�����ϵ�µ�matrix

		//��ʼ��֮�����й����в��ٸı�
		DexMatrix4x4   meshMatrix;	//mesh�ռ����
		DexMatrix4x4   localMeshMatrixInvert; //�ؽ���ģ�Ϳռ������󣬱���һ�μ���
		DVector<matrix_key> m_vecKeyFrames;
		DVector<Joint*> m_vecChildren;

	public:
		Joint();
		~Joint();
		void AddChild(Joint* child);
		//�Ȱ���ʱ���С�������
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
	{		/*Ŀǰ��Ⱦʱ����indicesͳһһ���͸�directx��Ⱦ�������Ļ���Ⱦ�����κ���Ⱦ�߶α����ò�ͬ��indice,
			�е��˷ѿռ䣬������������������Ϣ�����������汣����Ƕ��������������Ͳ��ñ�������indice������
			������Ⱦģ�ͱ���һ��һ����������Ⱦ��Ч�ʻ������⡣����ʡ�ռ�������ǣ����ѡ���߶���Ⱦ����ô��������
			��indice����һ���߶�indice����ѡ���߶���Ⱦʱ���߶�indiceɾ����
			*/
	public:
		char		name[64];
		DInt8	    id;
		DInt8		m_iTextureId;
		DInt8		m_iMaterialId;

		////������device����Ҫ��Ⱦ�Ķ�����Ϣ
		VectorDInt32		m_vecIndices;//vertex indice
		DVector<stMeshVertex> m_vecVertexsBuffer;
		VectorDInt32		m_vecLineIndices;//line vertex indice,�õ�buffer����m_vecVertexsBuffer

		//����debug ����
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
		//����debug normal
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

		//�����²��붥���index
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
	DFloat32		m_fAnimateRatio; //�������ʣ�Ĭ��1.0f
	SkinMeshAnimateType m_eAniType;
	SkinMeshModelType m_eMeshType;

	DexMatrix4x4 bindMatrix; //һ��skinmesh�����ڳ����е�һ���ڵ��ϣ����Ǹýڵ��matrix
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
	//ģ����Ӱ��ĵƹ⣬��ʱ�ȷ�zhe��
	DexColor	 m_ambientColor;
	DVector<stDexPointLight>  m_vecPointLight;
	stDexDirectionLight m_directionLight;
	DFloat32		m_fJointScale; //�������ţ����ڵ�����Ⱦ����
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
	//�ı����skinmesh�ľ���
	void SetSceneNodeMatrix(const DexMatrix4x4& matrix);
	virtual bool Update(DInt32 delta);
	virtual bool Render();
	//��ȡ����skinmesh���ݺ���ص��ô˽ӿ�
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
	void ReStartAnimation(); //�����һ���ԵĶ����Ļ������²��ŵ�ǰ����
	DInt16 GetAnimateNowTime()const;
	DInt16 GetAnimateEndTime()const;
	DInt16 GetAnimateStartTime()const;
	DFloat32 GetAnimateRatio() const;
	void  SetAnimateRatio(DFloat32 ratio);
//mesh
	DexMesh* FindMesh(DInt8 meshId);
	DexMesh* FindMesh(const char* meshName);
	//���ݴ����iMaterialIndexѰ��mesh�����û�У�bAdd��ʾ�Ƿ����
	DexMesh* FindMesh(DUInt8 iMaterialIndex, bool bAdd);
	DexMesh* AddMesh(DInt8 meshId);
	DexMesh* AddMesh(const char* meshName);
	//���ö�Ӧmesh�Ķ�����Ϣ
	void SetMeshVertexs(DInt8 meshId, void* vertexs, DInt32 count);
	void SetMeshIndices(DInt8 meshId, void* indics, DInt32 count);
//joint by id
	Joint* AddJoint(int id); //ֻ�������һ���ؽڣ�֮�����setjointinfo��ֵ�������
	Joint* SetJointInfo(int id, string name, string father_name, const DexMatrix4x4& father_matrix);
	//һ������ӹ�����������
	Joint* AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(DInt32 jointId);
//joint by name
	//һЩģ���ļ�jointֻ��nameû��id,��������½ӿ�
	Joint* AddJoint(string name);
	Joint* SetJointInfo(string name, int father_id, const DexMatrix4x4& father_matrix);
	//���ﴫ������ڸ��ڵ��е���Ծ��󣬴���Ҫ������ݴ˼���mesh�ռ����
	Joint* AddJoint(string name, string father_name, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(string name);
	DUInt32 FindJointIndex(DString name);
//joint frame by id
	bool AddJointKeyFrame(DInt32 jointid, DInt32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(DInt32 jointid, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, DFloat32 radian);
	bool AddJointKeyFrame(DInt32 jointid, DInt32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	//ĳһ֡��ĳ���ڵ���Ը��׽ڵ㣬ֻ��λ�ƵĻ�����������ӿ�,pos������ڸ�joint��λ��
	bool AddJointKeyFrameTrans(DInt32 jointId, DInt32 time, const DexVector3& pos);
	//ĳһ֡��ĳ���ڵ���Ը��׽ڵ㣬ֻ�����ŵĻ�����������ӿ�,scale������ڸ�joint������
	bool AddJointKeyFrameScale(DInt32 jointId, DInt32 time, const DexVector3& scale);
	//ĳһ֡��ĳ���ڵ�����ֻ����ת�Ļ�����������ӿڣ��ڵ�û��Ҫ�Ʊ�ĵ���ת���κ��Ʊ�ĵ���ת�Ĳ���������ת�������׽ڵ��������ת
	//axis:��������ĸ�����ת
	//radian:��ת�Ļ���
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
	//���material,��������ӵ�material��index
	DInt32 AddMaterial(const DexMaterial& material);
	DInt32 FindMaterial(DString materialName);

//vertex
	void AddVertex(const DexVector3& pos);
	//������ӵĶ�������skinmesh�����JointId=NULL,��󶨵����ڵ���
	void AddVertex(const DexVector3& pos, DInt16* JointId, float* weight, DInt16 jointCount);
	//ֱ����mesh��Ӷ���
	void AddVertex(DInt8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv);
	void AddVertex(DInt8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv, DInt16* JointId, float* weight, DUInt16 jointCount);
	//add triangle, for mesh,3��index�Ƕ�Ӧ��mesh�Ѿ����ڵĶ�������
	void AddMeshTriangle(DInt8 meshId, DInt32 index0, DInt32 index1, DInt32 index2);
//render flag
	bool GetRenderFlag(DInt16 flag);
	void SetRenderFlag(DInt16 flag);
	void SetJointScale(DFloat32 fScale);
//Render Order
	void SetOrderInfo(const DVector<DInt32>& vector);//������Ⱦ���
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
	//�ж�һ��pos�Ķ����Ƿ���skinmesh��
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