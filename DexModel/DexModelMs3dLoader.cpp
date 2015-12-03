

#include <fstream>
#include <vector>
#include "../DexBase/DexDefine.h"
#include "../DexMath/DexMatrix.h"
#include "DexModelMs3dLoader.h"
#include "DexSkinMesh.h"

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
	//TODO ��model����object���̣�����query����
	DexSkinMesh* skinMesh = new DexSkinMesh();
	ifstream inputFile(filename, ios::in | ios::binary | ios::_Nocreate);
	if (inputFile.fail())
	{
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
	MS3DHeader *pHeader = (MS3DHeader*)pPtr;
	pPtr += sizeof(MS3DHeader);
	if (strncmp(pHeader->m_ID, "MS3D000000", 10) != 0)
	{
		delete skinMesh;
		delete[] pBuffer;
		return NULL;
	}// "Not a valid Milkshape3D model file."

	if (pHeader->m_version < 3 || pHeader->m_version > 4)
	{
		delete skinMesh;
		return NULL;
	}// "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

	int nVertices = *(int16*)pPtr;  //�������
	pPtr += sizeof(int16);

	int i;
	for (i = 0; i < nVertices; i++)
	{
		//Ŀǰ��δ֧��ms3d boneid
		MS3DVertex *pVertex = (MS3DVertex*)pPtr;
		DexVector3 pos(pVertex->m_vertex);
		skinMesh->AddVertex(pos, pVertex->m_boneID, 1.0f);
		pPtr += sizeof(MS3DVertex);
	}
	int nTriangles = *(int16*)pPtr; //�����θ���
	struct Triangle
	{
		float m_vertexNormals[3][3];
		float m_s[3], m_t[3];
		int m_vertexIndices[3];
	};
	Triangle* Triangles = new Triangle[nTriangles];
	pPtr += sizeof(int16);
	for (i = 0; i < nTriangles; i++)
	{
		MS3DTriangle *pTriangle = (MS3DTriangle*)pPtr;
		int vertexIndices[3] = { pTriangle->m_vertexIndices[0], pTriangle->m_vertexIndices[1], pTriangle->m_vertexIndices[2] };
		float t[3] = { 1.0f - pTriangle->m_t[0], 1.0f - pTriangle->m_t[1], 1.0f - pTriangle->m_t[2] };
		
		//�������ǵ�skinmesh���ԣ�����ֻ��Ҫindice����������ֻ��¼indice
		//memcpy(Triangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof(float)* 3 * 3);
		//memcpy(Triangles[i].m_s, pTriangle->m_s, sizeof(float)* 3);
		//memcpy(Triangles[i].m_t, t, sizeof(float)* 3);
		memcpy(Triangles[i].m_vertexIndices, vertexIndices, sizeof(int)* 3);
		//������ܻ��һЩ���ö�������ظ�����
		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[0], pTriangle->m_vertexNormals[0]);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[0], pTriangle->m_s[0], t[0]);

		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[1], pTriangle->m_vertexNormals[1]);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[1], pTriangle->m_s[1], t[1]);

		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[2], pTriangle->m_vertexNormals[2]);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[2], pTriangle->m_s[2], t[2]);
		pPtr += sizeof(MS3DTriangle);
	}

	int nGroups = *(int16*)pPtr; //mesh����
	pPtr += sizeof(int16);
	for (i = 0; i < nGroups; i++)
	{
		pPtr += sizeof(byte);	// flags
		pPtr += 32;				// name
		int16 nTriangles = *(int16*)pPtr; //���mesh�ж��ٸ�������
		pPtr += sizeof(int16);
		int *pTriangleIndices = new int[nTriangles]; //����������

		int32* indices = new int32[nTriangles * 3];
		int32 indice_index = 0;
		DexSkinMesh::DexMesh* mesh =  skinMesh->AddMesh(i); //�½�mesh
		for (int j = 0; j < nTriangles; j++)
		{//�������mesh���õ������������εĶ�������
			int triangleIndex = *(int16*)pPtr;
			pPtr += sizeof(int16);
			indices[indice_index++] = Triangles[triangleIndex].m_vertexIndices[0];
			indices[indice_index++] = Triangles[triangleIndex].m_vertexIndices[1];
			indices[indice_index++] = Triangles[triangleIndex].m_vertexIndices[2];
		}
		skinMesh->SetIndices(i, indices, nTriangles * 3);
		char materialIndex = *(char*)pPtr;
		pPtr += sizeof(char);
		mesh->materialId = materialIndex;
		mesh->textureId = materialIndex;
		delete[] indices;
		delete[] pTriangleIndices;
	}

	int nMaterials = *(int16*)pPtr;
	pPtr += sizeof(int16);
	for (i = 0; i < nMaterials; i++)
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
		char* filename = (char*) malloc(strlen(pMaterial->m_texture) + 1);
		strcpy(filename, pMaterial->m_texture);
		skinMesh->AddMaterial(material);
		skinMesh->AddTexture(filename);
		pPtr += sizeof(MS3DMaterial);
		_SafeFree(filename);
	}
	float32 fps = *(float32*)pPtr;
	pPtr += sizeof(float32);

	float32 currentTime = *(float32*)pPtr;
	pPtr += sizeof(float32);

	float32 totalFrames = *(float32*)pPtr;
	pPtr += sizeof(float32);

	int16 jointCount = *(int16*)pPtr;
	pPtr += sizeof(int16);

	DexMatrix4x4 matrix;
	DexMatrix4x4 matrixRotateX;
	DexMatrix4x4 matrixRotateY;
	DexMatrix4x4 matrixRotateZ;
	
	float32 AniTime = 0.0f;
	for (int i = 0; i < jointCount; ++i)
	{
		matrix.Identity(); matrixRotateX.Identity(); matrixRotateY.Identity(); matrixRotateZ.Identity();
		MS3DJoint *ms3dJoint = (MS3DJoint*)pPtr;
		matrixRotateX.RotateX(ms3dJoint->m_rotation[0]);
		matrixRotateY.RotateY(ms3dJoint->m_rotation[1]);
		matrixRotateZ.RotateZ(ms3dJoint->m_rotation[2]);
		matrix = matrixRotateX * matrixRotateY * matrixRotateZ;
		DexSkinMesh::Joint* newJoint = skinMesh->AddJoint(ms3dJoint->m_name, ms3dJoint->m_parentName, matrix);
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
		for (size_t k = 0; k < vec_rotation.size(); ++k)
		{
			float32 time = vec_rotation[k].m_time;
			DexVector3 trans;
			for (size_t t = 0; t < vec_position.size(); ++t)
			{
				if (DexMath::Equal(time, vec_position[t].m_time))
				{//��һ֡����ƽ��
					trans.Set(vec_position[t].m_parameter);
					break;
				}
			}
			matrix.Identity(); matrixRotateX.Identity(); matrixRotateY.Identity(); matrixRotateZ.Identity();
			matrixRotateX.RotateX(vec_rotation[k].m_parameter[0]);
			matrixRotateY.RotateY(vec_rotation[k].m_parameter[1]);
			matrixRotateZ.RotateZ(vec_rotation[k].m_parameter[2]);
			matrix = matrixRotateX * matrixRotateY * matrixRotateZ;
			matrix.Translate(trans);
			skinMesh->AddJointKeyFrame(newJoint->str_name, time * 1000, matrix);
		}
		for (size_t k = 0; k < vec_position.size(); ++k)
		{
			float32 time = vec_position[k].m_time;
			DexVector3 trans(vec_position[k].m_parameter);
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
			skinMesh->AddJointKeyFrame(newJoint->str_name, time * 1000, matrix);
		}
	}
	delete[] Triangles;
	delete[] pBuffer;
	skinMesh->SetMaxAniTime(AniTime * 1000);
	skinMesh->CalculateVertex();
	return skinMesh;
}