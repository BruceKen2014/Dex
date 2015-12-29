
#include <fstream>
#include <vector>
#include <map>
#include "../DexBase/typedefine.h"
#include "../DexBase/DexDefine.h"
#include "../DexBase/DexLog.h"
#include "../Source/CCommandScript.h"
#include "../DexMath/DexMatrix.h"
#include "DexSkinMesh.h"
#include "DexModelObjLoader.h"

DexModelObjLoader::DexModelObjLoader()
{

}
DexModelObjLoader::~DexModelObjLoader()
{

}

DexModelBase* DexModelObjLoader::LoadModel(const char* filename)
{
	getLog()->BeginLog();
	int64 Time = getTime()->GetTotalMillSeconds();
	getLog()->Log(log_ok, "load obj model %s...\n", filename);
	//TODO 把model加入object工程，采用query机制
	DexSkinMesh* skinMesh = new DexSkinMesh();
	skinMesh->SetMeshType(SkinMeshModelType_OBJ);
	skinMesh->SetAnimateType(SkinMeshAnimateType_Total);
	int8* pBuffer = NULL;
	int8* pBufferCurr = NULL;
	int8* pBufferEnd = NULL;
	uint32 fileSize = 0;
	FILE* pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		getLog()->Log(log_error, "		load obj model %s failed!\n");
		getLog()->EndLog();
		delete skinMesh;
		return NULL;
	}
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	pBuffer = new int8[fileSize];
	fread(pBuffer, fileSize, 1, pFile);
	fclose(pFile);
	pBufferCurr = pBuffer;
	pBufferEnd = pBuffer + fileSize;
	DexVector3 tempV3;
	DexVector2 tempV2;
	int8 tempArry[32];
	Vector<DexVector3> vecPos;
	Vector<DexVector2> vecUv;
	Vector<DexVector3> vecNormal;
	Vector<ObjMaterial> vecMaterials;
	map<stIndex, int32> mapVertexInfo2;
	DexMaterial			tempMaterial;
	DexSkinMesh::DexMesh* pMesh = NULL;
	while (pBufferCurr != pBufferEnd)
	{
		switch (pBufferCurr[0])
		{
		case 'm':
			pBufferCurr += sizeof(int8)* 7;//'mtllib' and space
			getToken(pBufferCurr, tempArry, pBufferEnd);
			readMaterial((const char*)tempArry, vecMaterials);
			break;
		case 'g':
			pBufferCurr += sizeof(int8)* 2;//'g' and space
			getToken(pBufferCurr, tempArry, pBufferEnd);
			mapVertexInfo2.clear();
			pMesh = skinMesh->AddMesh((const char*)tempArry);
			break;
		case 'u':
			pBufferCurr += sizeof(int8)* 7;//'usemtl' and space
			getToken(pBufferCurr, tempArry, pBufferEnd);
			for (size_t i = 0; i < vecMaterials.size(); ++i)
			{
				if (strcmp((const char*)vecMaterials[i].name, (const char*)tempArry) == 0)
				{
					tempMaterial.ambient.Set(vecMaterials[i].m_ambient[0], vecMaterials[i].m_ambient[1], vecMaterials[i].m_ambient[2], 1.0f);
					tempMaterial.diffuse.Set(vecMaterials[i].m_diffuse[0], vecMaterials[i].m_diffuse[1], vecMaterials[i].m_diffuse[2], 1.0f);
					tempMaterial.specular.Set(vecMaterials[i].m_specular[0], vecMaterials[i].m_specular[1], vecMaterials[i].m_specular[2], 1.0f);
					tempMaterial.emissive.Set(vecMaterials[i].m_emissive[0], vecMaterials[i].m_emissive[1], vecMaterials[i].m_emissive[2], 1.0f);
					tempMaterial.power = vecMaterials[i].m_shininess;
					pMesh->m_iMaterialId = skinMesh->m_vecMaterials.size();
					if (skinMesh->AddTexture((const char*)vecMaterials[i].m_texture))
						//与material不同，texture有可能加载失败，所以需加安全判断
						pMesh->m_iTextureId = skinMesh->m_vecTextures.size() - 1;
					skinMesh->AddMaterial(tempMaterial);
					break;
				}
			}
			break;
		case 'v':
			switch (pBufferCurr[1])
			{
			case ' ':
			{//vertex
						pBufferCurr += sizeof(int8)* 2;//'v' and space
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV3.x = atof((const char*)tempArry);
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV3.y = atof((const char*)tempArry);
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV3.z = atof((const char*)tempArry);
						vecPos.push_back(tempV3);
						skinMesh->AddVertex(tempV3, NULL, NULL, 0);
						break;
			}
			case 't':
			{//uv
						pBufferCurr += sizeof(int8)* 3;//'v' and 't' and space
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV2.x = atof((const char*)tempArry);
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV2.y = atof((const char*)tempArry);
						vecUv.push_back(tempV2);
						break;
			}
			case 'n':
			{//normal
						pBufferCurr += sizeof(int8)* 3;//'v' and 'n' and space
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV3.x = atof((const char*)tempArry);
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV3.y = atof((const char*)tempArry);
						getToken(pBufferCurr, tempArry, pBufferEnd); tempV3.z = atof((const char*)tempArry);
						vecNormal.push_back(tempV3);
						break;
			}
			default:
				break;
			}
			break;
		case 'f':
		{
					/*
					f	1 2 3			三角形顶点索引为1 2 3 ，不用UV 和normal
					f	1/2	4/5	7/8		三角形顶点索引为1 4 7，UV索引为2 5 8
					f	1/2/3 6/7/8 4/5/6	三角形顶点索引为1 6 5=4，UV索引为 2 7 5，normal索引为3 8 6
					*/
					pBufferCurr += sizeof(int8)* 2;//'f' and space
					int32 index[3] = { 0, 0, 0 };
					int8  index_curr = 0;
					int8  indexBuff[32];
					int8  indexBuff_index = 0;
					DexVector3 temp_pos;
					DexVector3 temp_normal;
					float fTempU = 0.0f;
					float fTempV = 0.0f;
					for (int p = 1; p <= 3; ++p)
					{
						index_curr = 0;
						indexBuff_index = 0;
						index[0] = index[1] = index[2] = 0;
						getToken(pBufferCurr, tempArry, pBufferEnd);
						for (int i = 0; i < sizeof(tempArry); ++i)
						{
							if (tempArry[i] == '\0')
							{//一组index解析完毕
								indexBuff[indexBuff_index] = '\0';
								index[index_curr++] = atoi((const char*)indexBuff);
								break;
							}
							if (tempArry[i] == '/')
							{//解析完一组index当中的一个index
								indexBuff[indexBuff_index] = '\0';
								index[index_curr++] = atoi((const char*)indexBuff);
								indexBuff_index = 0;
								continue;
							}
							indexBuff[indexBuff_index++] = tempArry[i];
						}
						//三角形必然有顶点索引
						int32 iPosIndex = index[0] - 1;
						temp_pos = vecPos[iPosIndex];
						int32 iUvIndex = -1;
						int32 iNormalIndex = -1;
						if (index[1] != 0)
						{//用到了uv索引
							iUvIndex = index[1] - 1;
							fTempU = vecUv[iUvIndex].x;
							fTempV = vecUv[iUvIndex].y;
						}
						else
						{
							fTempU = fTempV = 0.0f;
						}
						if (index[2] != 0)
						{//用到了normal索引
							iNormalIndex = index[2] - 1;
							temp_normal = vecNormal[iNormalIndex];
						}
						else
						{
							temp_normal.Set(0.0f, 1.0f, 0.0f);
						}
						temp_normal.Normalize();
						stVertex3 newVertex(temp_pos, temp_normal, fTempU, fTempV);
						stIndex tempIndex(iPosIndex, iNormalIndex, iUvIndex);
						map<stIndex, int32>::iterator ite = mapVertexInfo2.find(tempIndex);
						if (ite != mapVertexInfo2.end())
						{//已经在当前mesh中找到了同样的顶点，则直接添加顶点indice即可
							pMesh->AddVertexIndice(ite->second);
						}
						else
						{
							mapVertexInfo2.insert(std::make_pair(tempIndex, pMesh->m_vecVertexs.size()));
							pMesh->AddVertexIndice(pMesh->m_vecVertexs.size());
							DexSkinMesh::MeshVertex* tempMeshVertex = skinMesh->m_vecMeshVertexs[iPosIndex];
							std::map<DexSkinMesh::DexMesh*, std::vector<int32>>::iterator vertexIte = tempMeshVertex->m_mapMeshVertex.find(pMesh);
							if (vertexIte != tempMeshVertex->m_mapMeshVertex.end())
							{
								vertexIte->second.push_back(pMesh->m_vecVertexs.size());
							}
							else
							{
								VectorInt32 vec;
								vec.push_back(pMesh->m_vecVertexs.size());
								tempMeshVertex->m_mapMeshVertex.insert(std::make_pair(pMesh, vec));
							}
							DexVector3 tempNormalPos = tempMeshVertex->meshPosition + temp_normal * 0.1f;
							(tempMeshVertex->m_vecNormalPosition).push_back(tempNormalPos);
							tempMeshVertex->m_vecWorldNormal.push_back(DexVector3(0.0f, 0.0f, 0.0f));
							pMesh->m_vecVertexs.push_back(newVertex);
						}
					}
					break;
		}
		default:
			moveNextLine(pBufferCurr, pBufferEnd);
			break;
		}
	}

	delete[] pBuffer;
	Time = getTime()->GetTotalMillSeconds() - Time;
	getLog()->Log(log_ok, "load obj model %s ok, use time %d ms\n", filename, Time);
	getLog()->EndLog();
	return skinMesh;
}

bool DexModelObjLoader::readMaterial(const char* filename, Vector<ObjMaterial>& vecMaterial)
{
	int8* pBuffer = NULL;
	int8* pBufferCurr = NULL;
	int8* pBufferEnd = NULL;
	uint32 fileSize = 0;
	FILE* pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	pBuffer = new int8[fileSize];
	fread(pBuffer, fileSize, 1, pFile);
	fclose(pFile);
	pBufferCurr = pBuffer;
	pBufferEnd = pBuffer + fileSize;

	int8	tempArr[64];
	ObjMaterial tempMaterial;
	bool	readingMaterial = false;
	while (pBufferCurr != pBufferEnd)
	{
		switch (pBufferCurr[0])
		{
		case 'n':
			//newmtl
			if (readingMaterial)
			{//上次正在读取material，更新到vecMaterial,
				vecMaterial.push_back(tempMaterial);
			}
			pBufferCurr += sizeof(int8)* 7; //'newmtl' and space
			getToken(pBufferCurr, tempMaterial.name, pBufferEnd);
			readingMaterial = true;
			break;
		case 'i':
			pBufferCurr += sizeof(int8)* 6; //'illum' and space
			getToken(pBufferCurr, tempArr, pBufferEnd);
			tempMaterial.m_Illumination = atoi((const char*)tempArr);
			break;
		case 'm':
			//map texture 目前支持一种texture
			pBufferCurr += sizeof(int8)* 7;//'map_Kd' and space
			getToken(pBufferCurr, tempMaterial.m_texture, pBufferEnd);
			break;
		case 'N':
			switch (pBufferCurr[1])
			{
			case 's':
				//shiness
				pBufferCurr += sizeof(int8)* 3;//	'Ns' and space
				getToken(pBufferCurr, tempArr, pBufferEnd);
				tempMaterial.m_shininess = atof((const char*)tempArr);
				break;
			case 'i':
				//refraction index
				pBufferCurr += sizeof(int8)* 3;//	'Ni' and space
				getToken(pBufferCurr, tempArr, pBufferEnd);
				//暂时不作处理
				break;
			default:
				break;
			}
			break;
		case 'K':
			switch (pBufferCurr[1])
			{
			case 'd':
				//diffuse
				pBufferCurr += sizeof(int8)* 3; //'Kd' and space
				//r g b
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_diffuse[0] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_diffuse[1] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_diffuse[2] = atof((const char*)tempArr);
				break;
			case 'a':
				//ambient
				pBufferCurr += sizeof(int8)* 3; //'Ka' and space
				//r g b
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_ambient[0] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_ambient[1] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_ambient[2] = atof((const char*)tempArr);
				break;
			case 's':
				//specular
				pBufferCurr += sizeof(int8)* 3; //'Ks' and space
				//r g b
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_specular[0] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_specular[1] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_specular[2] = atof((const char*)tempArr);
				break;
			case 'e':
				//emissive
				pBufferCurr += sizeof(int8)* 3; //'Ke' and space
				//r g b
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_emissive[0] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_emissive[1] = atof((const char*)tempArr);
				getToken(pBufferCurr, tempArr, pBufferEnd); tempMaterial.m_emissive[2] = atof((const char*)tempArr);
				break;
			default:
				break;
			}
			break;
		default:
			//其他空行和注释行等
			moveNextLine(pBufferCurr, pBufferEnd);
			break;
		}
	}
	//文件结束时将最后一次正在读的material添加到vec中
	vecMaterial.push_back(tempMaterial);
	delete[] pBuffer;
	return true;
}
void DexModelObjLoader::getToken(int8*& pBuffer, int8* pOut, int8* pEnd)
{
	while (pBuffer <= pEnd)
	{
		if (*pBuffer == '\r')
		{
			pBuffer++;
		}
		else if (*pBuffer == ' ' ||
				 *pBuffer == '\n')
		{
			pBuffer++;
			break;
		}
		else
		{
			*pOut++ = *pBuffer++;
		}
	}
	*pOut = '\0';
}

void DexModelObjLoader::moveNextLine(int8*& pBuffer, int8* pEnd)
{
	while (pBuffer <= pEnd)
	{
		if (*pBuffer++ == '\n')
		{
			break;
		}
	}
}