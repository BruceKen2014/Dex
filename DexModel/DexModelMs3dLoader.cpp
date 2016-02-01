

#include <fstream>
#include <vector>
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexLog.h"
#include "../DexMath/DexMatrix.h"
#include "DexModelMs3dLoader.h"
#include "DexSkinMesh.h"
#include "../DexMath/DexMath2.h"

/*
MS3D STRUCTURES
*/

// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, 1 )  
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#endif

// File header
struct MS3DHeader
{
	char m_ID[10]; //�̶��ı�־ MS3D000000
	int32 m_version; //��ʽ�汾��
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	int8 m_flags;		// SELECTED | SELECTED2 | HIDDEN����ʾʱ���ã����ڱ༭ʱʹ��
	float m_vertex[3];	// ��������
	char m_boneID;		// �����ؽڵ�������-1��ʾ����û�б�ָ�����ؽ�
	int8 m_refCount;	// ��ʾʱ���ã����ڱ༭ʱʹ��
} PACK_STRUCT;

// vertex weights in 1.8.x
struct MS3DVertexWeights
{
	char boneIds[3];
	uint8 weights[3];
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	int16 m_flags;					// SELECTED | SELECTED2 | HIDDEN����ʾʱ���ã����ڱ༭ʱʹ��
	int16 m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	int8 m_smoothingGroup;		// ƽ���飬��Χ1-32
	int8 m_groupIndex;			// ������������
} PACK_STRUCT;
struct MS3DGroup {
	int8         flags;              // SELECTED | HIDDEN����ʾʱ���ã����ڱ༭ʱʹ��
	char         name[32];           // 
	int16        triangles;          // �����ε���Ŀ
	int16*       triangleIndices;    // �������������ε�����
	char         materialIndex;      // ���ʵ�������-1��ʾû�в���
}PACK_STRUCT;
// Material information
struct MS3DMaterial
{
	char m_name[32];		
	float m_ambient[4];
	float m_diffuse[4];
	float m_specular[4];
	float m_emissive[4];
	float m_shininess;	// 0.0f - 128.0f ֵԽ��Խ�⻬����Դ���������γɵ�����ԽС
	float m_transparency;	// 0.0f - 1.0f
	int8 m_mode;	// 0, 1, 2 is unused now
	char m_texture[128];
	char m_alphamap[128];
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	float m_time;
	float m_parameter[3];
} PACK_STRUCT;
//	Joint information
struct MS3DJoint
{	
	int8 m_flags;			// SELECTED | DIRTY
	char m_name[32];
	char m_parentName[32];	// ��һ���ؽڵ�����
	float m_rotation[3];	 // ��ת��������ǻ��ȶ����ǽǶȡ���תʱ����x����תrotation[0]������y��תrotation[1]�������z��תrotation[2]
	float m_translation[3];
	int16 m_numRotationKeyframes;	// �ؼ�֡����ת���ƶ�ʹ�ò�ͬ�Ĺؼ�֡���ؼ�֡�����ݰ�����֡��ʱ�̡�֡��ֵ��
	int16 m_numTranslationKeyframes;
} PACK_STRUCT;



// Default alignment
#ifdef _MSC_VER
#	pragma pack(pop)
#endif

DexModelMs3dLoader::DexModelMs3dLoader()
{

}
DexModelMs3dLoader::~DexModelMs3dLoader()
{

}

DexModelBase* DexModelMs3dLoader::LoadModel(const char* filename)
{
	getLog()->BeginLog();
	int64 Time = getTime()->GetTotalMillSeconds();
	getLog()->Log(log_ok, "load ms3d %s...\n", filename);
	//TODO ��model����object���̣�����query����
	DexSkinMesh* skinMesh = new DexSkinMesh();
	skinMesh->SetMeshType(SkinMeshModelType_MS3D);
	ifstream inputFile(filename, ios::in | ios::binary | ios::_Nocreate);
	if (inputFile.fail())
	{
		getLog()->Log(log_error, "		load ms3d %s failed!\n");
		getLog()->EndLog();
		delete skinMesh;
		return NULL;
	}
	inputFile.seekg(0, ios::end);
	long fileSize = inputFile.tellg();
	inputFile.seekg(0, ios::beg);

	int8 *pBuffer = new int8[fileSize];
	inputFile.read((char*)pBuffer, fileSize);
	inputFile.close();

	const int8 *pPtr = pBuffer;
	const int8 *EndPtr = pBuffer + fileSize;
	MS3DHeader *pHeader = (MS3DHeader*)pPtr;
	pPtr += sizeof(MS3DHeader);
	if (strncmp(pHeader->m_ID, "MS3D000000", 10) != 0)
	{
		getLog()->Log(log_error, "		load ms3d %s failed! Id error!\n");
		getLog()->EndLog();
		delete skinMesh;
		delete[] pBuffer;
		return NULL;
	}// "Not a valid Milkshape3D model file."

	if (pHeader->m_version < 3 || pHeader->m_version > 4)
	{
		getLog()->Log(log_error, "		load ms3d %s failed! version error!\n");
		getLog()->EndLog();
		delete skinMesh;
		return NULL;
	}// "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

	uint32 nVertices = *(int16*)pPtr;  //�������
	pPtr += sizeof(int16);

	MS3DVertex *pVertex = (MS3DVertex*)pPtr;

	MS3DVertex pVertex22[100];
	memcpy(pVertex22, pVertex, sizeof(MS3DVertex)* nVertices);
	pPtr += sizeof(MS3DVertex)* nVertices;
	int nTriangles = *(int16*)pPtr; //�����θ���
	pPtr += sizeof(int16);
	MS3DTriangle* Triangles = (MS3DTriangle*)pPtr;
	pPtr += sizeof(MS3DTriangle)*nTriangles;
	int nGroups = *(int16*)pPtr; //mesh����
	pPtr += sizeof(int16);
	for (int i = 0; i < nGroups; i++)
	{
		pPtr += sizeof(byte);	// flags
		pPtr += 32;				// name
		int16 iTriangles = *(int16*)pPtr; //���mesh�ж��ٸ�������
		pPtr += sizeof(int16);
		int16 *pTriangleIndices = (int16*)pPtr; //����������
		pPtr += sizeof(int16)* iTriangles;
		int32 indice_index = 0;
		DexSkinMesh::DexMesh* mesh = skinMesh->AddMesh(i); //�½�mesh
		DexVector3 tempPos;
		DexVector3 tempNormal;
		float tempU, tempV;
		DexSkinMesh::stMeshVertex tempVertex;
		for (int j = 0; j < iTriangles; j++)
		{//�������mesh���õ������������εĶ�������
			int triangleIndex = pTriangleIndices[j];
			//��������ε����������Ƿ���ڵ�ǰmesh�в��ҵ�����Ҫƥ��pos normal uv
			for (int point = 0; point < 3; ++point)
			{
				uint32 iPosIndex = Triangles[triangleIndex].m_vertexIndices[point];
				tempPos.Set(pVertex[iPosIndex].m_vertex);
				tempPos.z *= -1;
				tempNormal.Set(Triangles[triangleIndex].m_vertexNormals[point]);
				tempNormal.z *= -1;
				tempU = Triangles[triangleIndex].m_s[point]; tempV = Triangles[triangleIndex].m_t[point];
				bool find = false;
				for (size_t index = 0; index < mesh->m_vecVertexsBuffer.size(); ++index)
				{
					if (mesh->m_vecVertexsBuffer[index].pos == tempPos
						&& mesh->m_vecVertexsBuffer[index].normal == tempNormal
						&& DexMath::Equal(mesh->m_vecVertexsBuffer[index].uv.x, tempU)
						&& DexMath::Equal(mesh->m_vecVertexsBuffer[index].uv.y, tempV))
					{//�ҵ��Ѿ����ڵ�vertex
						mesh->AddVertexIndice(index);
						find = true;
						break;
					}
				}
				if (!find)
				{//δ�ҵ��Ѵ��ڵģ����½�����
					tempVertex.pos = (tempPos);
					tempVertex.normal = (tempNormal);
					tempVertex.uv = DexVector2(tempU, tempV);
					tempVertex.color = DEXCOLOR_WHITE;
					tempVertex.JointIndex[0] = pVertex[iPosIndex].m_boneID +1;//��0����root
					tempVertex.JointWeights[0] = 1.0f;
					mesh->AddVertexIndice(mesh->m_vecVertexsBuffer.size());
					mesh->m_vecVertexsBuffer.push_back(tempVertex);
				}
			}
		}
		char materialIndex = *(char*)pPtr;
		pPtr += sizeof(char);
		mesh->m_iMaterialId = materialIndex;
		mesh->m_iTextureId = materialIndex;
	}

	int nMaterials = *(int16*)pPtr;
	pPtr += sizeof(int16);
	for (int i = 0; i < nMaterials; i++)
	{
		MS3DMaterial *pMaterial = (MS3DMaterial*)pPtr;
		DexMaterial material;
		material.diffuse.Set(pMaterial->m_diffuse[0], pMaterial->m_diffuse[1], pMaterial->m_diffuse[2],
			pMaterial->m_diffuse[3]);
		material.ambient.Set(pMaterial->m_ambient[0], pMaterial->m_ambient[1], pMaterial->m_ambient[2],
			pMaterial->m_ambient[3]);
		material.specular.Set(pMaterial->m_specular[0], pMaterial->m_specular[1], pMaterial->m_specular[2],
			pMaterial->m_specular[3]);
		material.emissive.Set(pMaterial->m_emissive[0], pMaterial->m_emissive[1], pMaterial->m_emissive[2],
			pMaterial->m_emissive[3]);
		material.power = pMaterial->m_shininess;
		//���м���materialĿǰ��û֧��
		char* filename = (char*)malloc(strlen(pMaterial->m_texture) + 1);
		strcpy(filename, pMaterial->m_texture);
		skinMesh->AddMaterial(material);
		skinMesh->AddTexture(filename);
		pPtr += sizeof(MS3DMaterial);
		_SafeFree(filename);
	}
	float32 framePerSecond = *(float32*)pPtr;
	pPtr += sizeof(float32);

	float32 currentTime = *(float32*)pPtr;
	pPtr += sizeof(float32);

	float32 totalFrames = *(float32*)pPtr;
	pPtr += sizeof(float32);

	int16 jointCount = *(int16*)pPtr;
	pPtr += sizeof(int16);

	DexMatrix4x4 baseMatrix;
	DexMatrix4x4 matrix;
	DexMatrix4x4 matrixRotateX;
	DexMatrix4x4 matrixRotateY;
	DexMatrix4x4 matrixRotateZ;

	float32 AniTime = 0.0f;
	float32 minAniTime = 0;
	for (int i = 1; i <= jointCount; ++i)
	{
		baseMatrix.Identity(); matrixRotateX.Identity(); matrixRotateY.Identity(); matrixRotateZ.Identity();
		MS3DJoint *ms3dJoint = (MS3DJoint*)pPtr;
		baseMatrix.RotateX(-ms3dJoint->m_rotation[0]);
		baseMatrix.RotateY(-ms3dJoint->m_rotation[1]);
		baseMatrix.RotateZ(ms3dJoint->m_rotation[2]);

		baseMatrix.Translate(ms3dJoint->m_translation[0], ms3dJoint->m_translation[1], -ms3dJoint->m_translation[2]);

		DexSkinMesh::Joint* newJoint = skinMesh->FindJoint(i);
		if (newJoint == NULL)
			newJoint = skinMesh->AddJoint(i);
		skinMesh->SetJointInfo(newJoint->id, string(ms3dJoint->m_name), string(ms3dJoint->m_parentName), baseMatrix);
		int rotationFrames = ms3dJoint->m_numRotationKeyframes;
		int positionFrames = ms3dJoint->m_numTranslationKeyframes;
		pPtr += sizeof(MS3DJoint);
		std::vector<MS3DKeyframe> vec_rotation;
		std::vector<MS3DKeyframe> vec_position;
		for (int j = 0; j < rotationFrames; ++j)
		{
			MS3DKeyframe* key = (MS3DKeyframe*)pPtr;
			vec_rotation.push_back(*key);
			if (key->m_time > AniTime)
				AniTime = key->m_time;
			pPtr += sizeof(MS3DKeyframe);
		}
		for (int j = 0; j < positionFrames; ++j)
		{
			MS3DKeyframe* key = (MS3DKeyframe*)pPtr;
			vec_position.push_back(*key);
			if (key->m_time > AniTime)
				AniTime = key->m_time;
			pPtr += sizeof(MS3DKeyframe);
		}
		//�ҵ��б任����һ֡��Ϊ��������Ч��ʼʱ��
		if (vec_rotation.size() != 0)
		{
			minAniTime = vec_rotation[0].m_time;
		}
		if (vec_position.size() != 0 && vec_position[0].m_time < minAniTime)
		{
			minAniTime = vec_position[0].m_time;
		}
		for (size_t k = 0; k < vec_rotation.size(); ++k)
		{
			float32 time = vec_rotation[k].m_time;
			DexVector3 trans;
			for (size_t t = 0; t < vec_position.size(); ++t)
			{
				if (DexMath::Equal(time, vec_position[t].m_time))
				{//��һ֡����ƽ��
					trans.Set(vec_position[t].m_parameter[0],
						vec_position[t].m_parameter[1], -vec_position[t].m_parameter[2]);
					break;
				}
			}
			matrix.Identity();

			matrix.RotateX(-vec_rotation[k].m_parameter[0]);
			matrix.RotateY(-vec_rotation[k].m_parameter[1]);
			matrix.RotateZ(vec_rotation[k].m_parameter[2]);
			matrix.Translate(trans);
			skinMesh->AddJointKeyFrame(newJoint->str_name, time * 1000, matrix *  baseMatrix);
		}
		for (size_t k = 0; k < vec_position.size(); ++k)
		{
			float32 time = vec_position[k].m_time;
			DexVector3 trans(vec_position[k].m_parameter[0],
				vec_position[k].m_parameter[1], -vec_position[k].m_parameter[2]);
			bool _continue;
			for (size_t t = 0; t < vec_rotation.size(); ++t)
			{
				if (DexMath::Equal(time, vec_rotation[t].m_time))
				{//��������ת��һ֡���Ѿ��������
					_continue = true;
					break;
				}
			}
			if (_continue)
				continue;
			matrix.Identity();
			matrix.Translate(trans);
			skinMesh->AddJointKeyFrame(newJoint->str_name, time * 1000, matrix * baseMatrix);
		}
	}
	//map<int16, Vector<JointVertex>>::iterator ite = joint_Vertexs.find(-1);
	//if (ite != joint_Vertexs.end())
	//{//����û�а󶨵Ķ���
	//	for (int v = 0; v < ite->second.size(); ++v)
	//	{
	//		skinMesh->AddVertexJointInfo(ite->second[v].vertexIndex, skinMesh->m_pRootJoint->id, 1.0f);
	//	}
	//}
	if (pPtr < EndPtr && pHeader->m_version == 4)
	{
		int32 subVersion = *(int32*)pPtr; // comment subVersion, always 1
		pPtr += sizeof(subVersion);
		for (int i = 0; i < 4; ++i)
		{
			uint32 numComment = *(uint32*)pPtr;
			pPtr += sizeof(numComment);
			for (uint32 j = 0; j < numComment; ++j)
			{
				// according to scorpiomidget this field does
				// not exist for model comments. So avoid to
				// read it
				if (i != 3)
				{
					pPtr += sizeof(int32); //index
				}
				int32 commentLength = *(int32*)pPtr;
				pPtr += sizeof(commentLength);
				pPtr += commentLength;
			}
		}
		if (pPtr < EndPtr)
		{
			subVersion = *(int32*)pPtr; //vertex subversion
			pPtr += sizeof(subVersion);
			// read vertex weights, ignoring data 'extra' from 1.8.2
			int16 offset = subVersion == 1 ? 6 : 10;
			for (int index = 0; index < nVertices; ++index)
			{
				MS3DVertexWeights* vertexEx = (MS3DVertexWeights*)pPtr;
				for (int boneIndex = 0; boneIndex < 3; ++boneIndex)
				{
					//if (vertexEx->boneIds[boneIndex] != -1)
				}
				pPtr += offset;
			}
		}
		if (pPtr < EndPtr)
		{
			subVersion = *(int32*)pPtr; //joint subversion
			pPtr += sizeof(subVersion);
			// skip joint colors
			pPtr += 3 * sizeof(float32)*jointCount;
		}
		if (pPtr < EndPtr)
		{
			subVersion = *(int32*)pPtr; //model subversion
			pPtr += sizeof(subVersion);
		}
	}
	delete[] pBuffer;
	skinMesh->SetMaxAniTime(AniTime * 1000);
	skinMesh->SetAnimateTime(minAniTime * 1000, AniTime * 1000);
	//skinMesh->CalculateVertex();

	Time = getTime()->GetTotalMillSeconds() - Time;
	getLog()->Log(log_ok, "load ms3d %s ok, use time %d ms\n", filename, Time);
	getLog()->EndLog();
	return skinMesh;
}