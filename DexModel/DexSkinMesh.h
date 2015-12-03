


#include <d3dx9math.h>
#include <vector>
#include <map>
#include <string>
#include "../DexBase/DexMaterial.h"
#include "../DexBase/DexVertex.h"
#include "../DexMath/DexVector2.h"
#include "../DexMath/DexVector3.h"
#include "../DexMath/DexQuaternion.h"
#include "../DexMath/DexMatrix.h"

#include "DexModelBase.h"

class MeshVertex
{
	friend class DexSkinMesh;
protected:
	DexVector3 meshPosition; //mesh�ռ�����
	DexVector3 worldPosition; //��������
	DexVector3 normalPosition;
	DexVector3 worldNormal; 
	stVertex3* vertexData;
	DexVector2 uv;
	std::vector<int16>  vec_JointId;
	std::vector<DexVector3>  Joints_localPos; //mesh �ռ�������ڸ�������������
	std::vector<DexVector3>  Joints_localNormalPoint; //mesh �ռ�������ڸ���������normal����
	std::vector<float>  JointWeigth;

public:
	MeshVertex()
	{
		vec_JointId.clear();
		JointWeigth.clear();
		vertexData = NULL;
		normalPosition.Set(0.0f, 1.0f, 0.0f);
	}
	~MeshVertex(){}
};
class CDexTex;
/*
һ��ģ�͵�skinmesh���ܰ������mesh������һ�����ſ�������è����è������һ��mesh�����Ŷ�Ӧ�Ĳ��ʺ���ͼ��
����������һ��mesh����������һ�ײ�����ͼ,���ǹ���һ�ݶ�������,ÿ��mesh���Ų�ͬ��indice
��������Ǿ�һ�������skinmesh��˵�ģ�����һ�����Գ��в�ͬ�Ŀ�������è��˵����è�Ϳ���Ӧ�ò�����ͬһ��
skinmesh��ֻ�����ѿ������������skinmesh�󶨵�����è���skinmesh��ĳ��������
*/
//���ڵ�ID
#define _SKIN_MESH_ROOT_JOINT_ID	0x0FFF
class DexSkinMesh: public DexModelBase
{
	Dex_DeclareClass(DexSkinMesh, 0)
	friend class DexModelMs3dLoader;
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
		Joint*		   father;
		DexMatrix4x4   world_matrix; //��������matrix
		DexMatrix4x4   frame_matrix; //ÿһ֡�ڸ�����ϵ�µ�matrix
		std::vector<matrix_key> frames;
		std::vector<Joint*> children;

	public:
		Joint();
		~Joint();
		void AddChild(Joint* child);
		//�Ȱ���ʱ���С�������
		void AddKeyFrame(int32 time, const DexMatrix4x4& matrix);
		void Render();
		void Update(int time);
		void ComputeWorldMatrix(int32 time);
		DexVector3 GetPosInMeshSpace()const;  //ȡ�ø�joint��mesh�ռ������
	};
	class DexMesh
	{		/*Ŀǰ��Ⱦʱ����indicesͳһһ���͸�directx��Ⱦ�������Ļ���Ⱦ�����κ���Ⱦ�߶α����ò�ͬ��indice,
			�е��˷ѿռ䣬������������������Ϣ�����������汣����Ƕ��������������Ͳ��ñ�������indice������
			������Ⱦģ�ͱ���һ��һ����������Ⱦ��Ч�ʻ������⡣����ʡ�ռ�������ǣ����ѡ���߶���Ⱦ����ô��������
			��indice����һ���߶�indice����ѡ���߶���Ⱦʱ���߶�indiceɾ����
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
	DexMatrix4x4 bindMatrix; //һ��skinmesh�����ڳ����е�һ���ڵ��ϣ����Ǹýڵ��matrix
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

//skin mesh
	//�ı����skinmesh�ľ���
	void SetSceneNodeMatrix(const DexMatrix4x4& matrix);
	virtual bool Update(int32 delta);
	virtual bool Render();
	//��ȡ����skinmesh���ݺ���ص��ô˽ӿ�
	void CalculateVertex();
//animation
	void SetMaxAniTime(int16 time);
	void SetAnimateEnable(bool animate);
	bool SetAnimateTime(int16 start, int16 end);
	
//mesh
	DexMesh* FindMesh(int8 meshId);
	DexMesh* AddMesh(int8 meshId);
	//���ö�Ӧmesh�Ķ���������Ϣ
	void SetIndices(int8 meshId, void* indics, int32 count);
//joint
	Joint* AddJoint(int id); //ֻ�������һ���ؽڣ�֮��ֵ�������
	Joint* AddJoint(int id, int father_id, const DexMatrix4x4& father_matrix);
	Joint* FindJoint(int32 jointId);
	//һЩģ���ļ�jointֻ��nameû��id,��������½ӿ�
	Joint* AddJoint(string name);
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
	//����һЩģ�����ݣ���ʼֻ����λ�ú͹���������Ϣ��֮���ٶ�ȡ����������ݺ͹������ݣ�����SetVertex��ؽӿ�
	void AddVertex(const DexVector3& pos, int16 JointId, float weight);
	//һ�������ö�������
	void AddVertex(const DexVector3& pos, const DexVector3& normal, const DexVector2& uv, int jointid1 = -1, float weight1 = 1.0f, int jointid2 = -1, float weight2 = 0.0f);
	bool SetVertexNormal(int32 vertexIndex, float x, float y, float z);
	bool SetVertexNormal(int32 vertexIndex, const DexVector3& normal);
	bool SetVertexUv(int32 vertexIndex, float32 u, float v);
	bool SetVertexUv(int32 vertexIndex, const DexVector2& uv);
protected:
	bool AddJointKeyFrame(Joint* joint, int32 time, const DexMatrix4x4& matrix);
	bool AddJointKeyFrame(Joint* joint, int32 time, const DexVector3& pos, const DexVector3& scale, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrame(Joint* joint, int32 time, const DexVector3& pos, const DexVector3& scale, const DexQuaternion& qua);
	bool AddJointKeyFrameTrans(Joint* joint, int32 time, const DexVector3& pos);
	bool AddJointKeyFrameScale(Joint* joint, int32 time, const DexVector3& scale);
	bool AddJointKeyFrameRotation(Joint* joint, int32 time, const DexVector3& axis, float32 radian);
	bool AddJointKeyFrameRotation(Joint* joint, int32 time, const DexQuaternion& qua);
protected:
	//calculate position and normal
	//return position
	DexVector3 ComputeVertex(MeshVertex* vertex);
	void	   AddJointInfo(MeshVertex* vertex, int16 jointId, float32 weight);
	bool	   BindVertexToJoint(MeshVertex* vertex);
	virtual bool CheckAnimate();

};