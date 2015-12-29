
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
	SkinMeshModelType_UnKnown, //unknown
	SkinMeshModelType_Dex,	   //dex�����ʽ��Ŀǰ��δʵ��
	SkinMeshModelType_MS3D,    //.ms3d
	SkinMeshModelType_OBJ,	   //.obj
	SkinMeshModelType_X,       //.X
	SkinMeshModelType_Total
}SkinMeshModelType;
class CDexTex;
//���ڵ�ID
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
		{//<���룬����>
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
		DexMatrix4x4   world_matrix; //��������matrix
		DexMatrix4x4   frame_matrix; //ÿһ֡�ڸ�����ϵ�µ�matrix

		//��ʼ��֮�����й����в��ٸı�
		DexMatrix4x4   meshMatrix;	//mesh�ռ����
		DexMatrix4x4   localMeshMatrixInvert; //�ؽ���ģ�Ϳռ������󣬱���һ�μ���
		Vector<matrix_key> m_vecKeyFrames;
		Vector<Joint*> m_vecChildren;

	public:
		Joint();
		~Joint();
		void AddChild(Joint* child);
		//�Ȱ���ʱ���С�������
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
		DexVector3 meshPosition; //mesh�ռ�����
		DexVector3 worldPosition;
		Vector<DexVector3> m_vecWorldNormal;
		Vector<DexVector3> m_vecNormalPosition; //skinmesh �ռ�������ڸ���������normal����
		Vector<stVertex3*>  m_vecVertexData;
		Vector<Joint*>		m_vecJoints;
		VectorFloat32   m_vecJointWeigth;
		std::map<DexMesh*, VectorInt32> m_mapMeshVertex; //��Ӧ��ĳ��mesh����Ķ�������

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
	{		/*Ŀǰ��Ⱦʱ����indicesͳһһ���͸�directx��Ⱦ�������Ļ���Ⱦ�����κ���Ⱦ�߶α����ò�ͬ��indice,
			�е��˷ѿռ䣬������������������Ϣ�����������汣����Ƕ��������������Ͳ��ñ�������indice������
			������Ⱦģ�ͱ���һ��һ����������Ⱦ��Ч�ʻ������⡣����ʡ�ռ�������ǣ����ѡ���߶���Ⱦ����ô��������
			��indice����һ���߶�indice����ѡ���߶���Ⱦʱ���߶�indiceɾ����
			*/
	public:
		char		name[64];
		int8	    id;
		int8		m_iTextureId;
		int8		m_iMaterialId;
		Vector<stVertex3>	m_vecVertexs; //������device����Ҫ��Ⱦ�Ķ�����Ϣ
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
		//�����²��붥���index
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
	float32		m_fAnimateRatio; //�������ʣ�Ĭ��1.0f
	SkinMeshAnimateType m_eAniType;
	SkinMeshModelType m_eMeshType;

	DexMatrix4x4 bindMatrix; //һ��skinmesh�����ڳ����е�һ���ڵ��ϣ����Ǹýڵ��matrix
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
	//�ı����skinmesh�ľ���
	void SetSceneNodeMatrix(const DexMatrix4x4& matrix);
	virtual bool Update(int32 delta);
	virtual bool Render();
	//��ȡ����skinmesh���ݺ���ص��ô˽ӿ�
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
	void ReStartAnimation(); //�����һ���ԵĶ����Ļ������²��ŵ�ǰ����
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
	//���ö�Ӧmesh�Ķ�����Ϣ
	void SetMeshVertexs(int8 meshId, void* vertexs, int32 count);
	void SetMeshIndices(int8 meshId, void* indics, int32 count);
//joint by id
	Joint* AddJoint(int id); //ֻ�������һ���ؽڣ�֮�����setjointinfo��ֵ�������
	Joint* SetJointInfo(int id, string name, string father_name, const DexMatrix4x4& father_matrix);
	//һ������ӹ�����������
	Joint* AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(int32 jointId);
//joint by name
	//һЩģ���ļ�jointֻ��nameû��id,��������½ӿ�
	Joint* AddJoint(string name);
	Joint* SetJointInfo(string name, int father_id, const DexMatrix4x4& father_matrix);
	//���ﴫ������ڸ��ڵ��е���Ծ��󣬴���Ҫ������ݴ˼���mesh�ռ����
	Joint* AddJoint(string name, string father_name, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(string name);
//joint frame by id
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrame(int32 jointid, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	//ĳһ֡��ĳ���ڵ���Ը��׽ڵ㣬ֻ��λ�ƵĻ�����������ӿ�,pos������ڸ�joint��λ��
	bool AddJointKeyFrameTrans(int32 jointId, int32 time, const DexVector3& pos);
	//ĳһ֡��ĳ���ڵ���Ը��׽ڵ㣬ֻ�����ŵĻ�����������ӿ�,scale������ڸ�joint������
	bool AddJointKeyFrameScale(int32 jointId, int32 time, const DexVector3& scale);
	//ĳһ֡��ĳ���ڵ�����ֻ����ת�Ļ�����������ӿڣ��ڵ�û��Ҫ�Ʊ�ĵ���ת���κ��Ʊ�ĵ���ת�Ĳ���������ת�������׽ڵ��������ת
	//axis:��������ĸ�����ת
	//radian:��ת�Ļ���
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
	//������ӵĶ�������skinmesh�����JointId=NULL,��󶨵����ڵ���
	void AddVertex(const DexVector3& pos, int16* JointId, float* weight, int16 jointCount);
	void AddVertexJointInfo(int32 vertexIndex, int16 jointId, float32 weight);//��һ���Ѵ��ڵĶ������joint��Ϣ
	void AddJointInfo(MeshVertex* vertex, Joint* pJoint, float32 fWeight);
	//������ӵĶ�������mesh��posӦ�����Ѿ���skinmesh�еģ�
	//���pos����skinmesh�У�����skinmesh���pos����
	void AddVertex(int8 meshId, const DexVector3& pos, const DexVector3& normal, const DexVector2& uv);
	//add triangle, for mesh,3��index�Ƕ�Ӧ��mesh�Ѿ����ڵĶ�������
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
	//return value:��ǰ�ؽ��Ƿ��ж��������ݷ���ֵ�������Ƿ���¶���
	void UpdateVertex();
	bool UpdateOneTime(int32 delta);
	bool UpdateLoop(int32 delta);
	bool UpdateOneTimeBack(int32 delta);
	bool UpdateLoopBack(int32 delta);
protected:
	//calculate position and normal
	//return position
	DexVector3 ComputeVertex(MeshVertex* vertex);
	//�ж�һ��pos�Ķ����Ƿ���skinmesh��
	bool	   FindVertex(const DexVector3& pos);
	virtual bool IsStaticModel();

};