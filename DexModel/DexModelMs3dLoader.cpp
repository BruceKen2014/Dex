

#include <fstream>
#include <vector>
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexLog.h"
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
	char m_ID[10]; //固定的标志 MS3D000000
	int32 m_version; //格式版本号
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	int8 m_flags;		// SELECTED | SELECTED2 | HIDDEN。显示时无用，仅在编辑时使用
	float m_vertex[3];	// 顶点坐标
	char m_boneID;		// 所属关节的索引，-1表示顶点没有被指定到关节
	int8 m_refCount;	// 显示时无用，仅在编辑时使用
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
	int16 m_flags;					// SELECTED | SELECTED2 | HIDDEN。显示时无用，仅在编辑时使用
	int16 m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	int8 m_smoothingGroup;		// 平滑组，范围1-32
	int8 m_groupIndex;			// 所属的组索引
} PACK_STRUCT;
struct MS3DGroup {
	int8         flags;              // SELECTED | HIDDEN。显示时无用，仅在编辑时使用
	char         name[32];           // 
	int16        triangles;          // 三角形的数目
	int16*       triangleIndices;    // 组中所有三角形的索引
	char         materialIndex;      // 材质的索引，-1表示没有材质
}PACK_STRUCT;
// Material information
struct MS3DMaterial
{
	char m_name[32];		
	float m_ambient[4];
	float m_diffuse[4];
	float m_specular[4];
	float m_emissive[4];
	float m_shininess;	// 0.0f - 128.0f 值越大越光滑，光源照在其上形成的亮点越小
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
	char m_parentName[32];	// 上一级关节的名字
	float m_rotation[3];	 // 旋转，保存的是弧度而不是角度。旋转时先绕x轴旋转rotation[0]，再绕y旋转rotation[1]，最后绕z旋转rotation[2]
	float m_translation[3];
	int16 m_numRotationKeyframes;	// 关键帧。旋转和移动使用不同的关键帧。关键帧的数据包括了帧的时刻、帧的值。
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
	//TODO 把model加入object工程，采用query机制
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

	int nVertices = *(int16*)pPtr;  //顶点个数
	pPtr += sizeof(int16);

	int i;
	for (i = 0; i < nVertices; i++)
	{
		MS3DVertex *pVertex = (MS3DVertex*)pPtr;
		//ms3d模型空间为右手坐标系，DX为左右坐标系
		DexVector3 pos(pVertex->m_vertex[0], pVertex->m_vertex[1], -pVertex->m_vertex[2]);
		skinMesh->AddVertex(pos, pVertex->m_boneID, 1.0f);
		pPtr += sizeof(MS3DVertex);
	}
	int nTriangles = *(int16*)pPtr; //三角形个数
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
		
		//对于我们的skinmesh而言，后面只需要indice，所以这里只记录indice
		//memcpy(Triangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof(float)* 3 * 3);
		//memcpy(Triangles[i].m_s, pTriangle->m_s, sizeof(float)* 3);
		//memcpy(Triangles[i].m_t, t, sizeof(float)* 3);
		memcpy(Triangles[i].m_vertexIndices, vertexIndices, sizeof(int)* 3);
		//这里可能会对一些公用顶点进行重复设置
		DexVector3 normal;
		normal.Set(pTriangle->m_vertexNormals[0][0], pTriangle->m_vertexNormals[0][1], -pTriangle->m_vertexNormals[0][2]);
		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[0], normal);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[0], pTriangle->m_s[0], t[0]);
		normal.Set(pTriangle->m_vertexNormals[1][0], pTriangle->m_vertexNormals[0][1], -pTriangle->m_vertexNormals[1][2]);
		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[1], normal);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[1], pTriangle->m_s[1], t[1]);
		normal.Set(pTriangle->m_vertexNormals[2][0], pTriangle->m_vertexNormals[0][1], -pTriangle->m_vertexNormals[2][2]);
		skinMesh->SetVertexNormal(pTriangle->m_vertexIndices[2], normal);
		skinMesh->SetVertexUv(pTriangle->m_vertexIndices[2], pTriangle->m_s[2], t[2]);
		pPtr += sizeof(MS3DTriangle);
	}

	int nGroups = *(int16*)pPtr; //mesh数量
	pPtr += sizeof(int16);
	for (i = 0; i < nGroups; i++)
	{
		pPtr += sizeof(byte);	// flags
		pPtr += 32;				// name
		int16 nTriangles = *(int16*)pPtr; //这个mesh有多少个三角形
		pPtr += sizeof(int16);
		int *pTriangleIndices = new int[nTriangles]; //三角形索引

		int32* indices = new int32[nTriangles * 3];
		int32 indice_index = 0;
		DexSkinMesh::DexMesh* mesh =  skinMesh->AddMesh(i); //新建mesh
		for (int j = 0; j < nTriangles; j++)
		{//解析这个mesh所用到的所有三角形的顶点索引
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
		//还有几个material目前还没支持
		char* filename = (char*) malloc(strlen(pMaterial->m_texture) + 1);
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
	for (int i = 0; i < jointCount; ++i)
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
		//找到有变换的那一帧作为动作的有效开始时间
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
				{//这一帧还有平移
					trans.Set(vec_position[t].m_parameter[0],
						vec_position[t].m_parameter[1], -vec_position[t].m_parameter[2]);
					break;
				}
			}
			matrix.Identity();
			
			matrix.RotateX(-vec_rotation[k].m_parameter[0]);
			matrix.RotateY(-vec_rotation[k].m_parameter[1]);
			matrix.RotateZ( vec_rotation[k].m_parameter[2]);
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
				{//找了有旋转的一帧，已经被处理过
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
	if (pHeader->m_version == 4)
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
		}
	}
	delete[] Triangles;
	delete[] pBuffer;
	skinMesh->SetMaxAniTime(AniTime * 1000);
	skinMesh->SetAnimateTime(minAniTime * 1000, AniTime * 1000);
	skinMesh->CalculateVertex();

	Time = getTime()->GetTotalMillSeconds() - Time;
	getLog()->Log(log_ok, "load ms3d %s ok, use time %dms\n", filename, Time);
	getLog()->EndLog();
	return skinMesh;
}