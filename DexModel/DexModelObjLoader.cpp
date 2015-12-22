
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

struct stIndex
{
	int32 m_iPosIndex;
	int32 m_iNormalIndex;
	int32 m_iUvIndex;
	stIndex(int32 i1, int32 i2, int i3) :m_iPosIndex(i1), m_iNormalIndex(i2), m_iUvIndex(i3)
	{}
	bool operator == (const stIndex& index)	const
	{
		return m_iPosIndex == index.m_iPosIndex &&
			m_iNormalIndex == index.m_iNormalIndex &&
			m_iUvIndex == index.m_iUvIndex;
	}
	bool operator < (const stIndex& index)	const
	{
		if (m_iPosIndex < index.m_iPosIndex)
			return true;
		if (m_iPosIndex > index.m_iPosIndex)
			return false;
		if (m_iNormalIndex < index.m_iNormalIndex)
			return true;
		if (m_iNormalIndex > index.m_iNormalIndex)
			return false;
		if (m_iUvIndex < index.m_iUvIndex)
			return true;
		if (m_iUvIndex > index.m_iUvIndex)
			return false;
		return false;
	}
};
DexModelBase* DexModelObjLoader::LoadModel(const char* filename)
{
	getLog()->BeginLog();
	int64 Time = getTime()->GetTotalMillSeconds();
	getLog()->Log(log_ok, "load obj model %s...\n", filename);
	//TODO 把model加入object工程，采用query机制
	DexSkinMesh* skinMesh = new DexSkinMesh();
	skinMesh->SetMeshType(SkinMeshModelType_OBJ);
	DexSkinMesh::DexMesh* pMesh = skinMesh->AddMesh(1);
	int8 loadType = 0;
	if (loadType == 0)
	{
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
		std::vector<DexVector3> vec_pos;
		std::vector<DexVector2> vec_uv;
		std::vector<DexVector3> vec_normal;
		//pos normal uv index
		map<int32, map<int32, map<int32, int32>>> mapVertexInfo;
		map<stIndex, int32> mapVertexInfo2;
		while (pBufferCurr != pBufferEnd)
		{
			switch (pBufferCurr[0])
			{
			case 'v':
				switch (pBufferCurr[1])
				{
				case ' ':
				{//vertex
							pBufferCurr += sizeof(int8)* 2;//'v' and space
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV3.x = atof((const char*)tempArry);
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV3.y = atof((const char*)tempArry);
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV3.z = atof((const char*)tempArry);
							if (vec_pos.size() >= vec_pos.capacity())
							{
								vec_pos.reserve(vec_pos.size() * 2);
							}
							vec_pos.push_back(tempV3);
							skinMesh->AddVertex(tempV3, NULL, NULL, 0);
							break;
				}
				case 't':
				{//uv
							pBufferCurr += sizeof(int8)* 3;//'v' and 't' and space
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV2.x = atof((const char*)tempArry);
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV2.y = atof((const char*)tempArry);
							if (vec_uv.size() >= vec_uv.capacity())
							{
								vec_uv.reserve(vec_uv.size()*2);
							}
							vec_uv.push_back(tempV2);
							break;
				}
				case 'n':
				{//normal
							pBufferCurr += sizeof(int8)* 3;//'v' and 'n' and space
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV3.x = atof((const char*)tempArry);
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV3.y = atof((const char*)tempArry);
							GetToken(pBufferCurr, tempArry, pBufferEnd); tempV3.z = atof((const char*)tempArry);
							if (vec_normal.size() >= vec_normal.capacity())
							{
								vec_normal.reserve(vec_normal.size() * 2);
							}
							vec_normal.push_back(tempV3);
							break;
				}
				default:
					break;
				}
				break;
			case 'f':
			{
						//MoveNextLine(pBufferCurr, pBufferEnd);
						//break;
						pBufferCurr += sizeof(int8)* 2;//'f' and space
						int32 index[3] = { -1, -1, -1 };
						int8  index_curr = 0;
						int8  indexBuff[16];
						int8  indexBuff_index = 0;
						for (int p = 1; p <= 3; ++p)
						{
							index_curr = 0;
							indexBuff_index = 0;
							GetToken(pBufferCurr, tempArry, pBufferEnd);
							for (int i = 0; i < sizeof(tempArry); ++i)
							{
								if (tempArry[i] == '\0')
								{
									indexBuff[indexBuff_index] = '\0';
									index[index_curr++] = atoi((const char*)indexBuff);
									break;
								}
								if (tempArry[i] == '/')
								{
									indexBuff[indexBuff_index] = '\0';
									index[index_curr++] = atoi((const char*)indexBuff);
									indexBuff_index = 0;
									continue;
								}
								indexBuff[indexBuff_index++] = tempArry[i];
							}
							int32 iPosIndex = index[0] - 1;
							int32 iUvIndex = index[1] - 1;
							int32 iNormalIndex = index[2] - 1;
							stVertex3 newVertex(vec_pos[iPosIndex], vec_normal[iNormalIndex],
								vec_uv[iUvIndex].x, vec_uv[iUvIndex].y);
							stIndex tempIndex(iPosIndex, iNormalIndex, iUvIndex);
							map<stIndex, int32>::iterator ite = mapVertexInfo2.find(tempIndex);
							if (ite != mapVertexInfo2.end())
							{
								//realIndice[iRealIndiceCount++] = iRealVertexCount;
								pMesh->AddVertexIndice(ite->second);
								continue;
							}
							mapVertexInfo2.insert(std::make_pair(tempIndex, pMesh->m_vecVertexs.size()));
							pMesh->AddVertexIndice(pMesh->m_vecVertexs.size());
							

							MeshVertex* tempMeshVertex = skinMesh->m_vecMeshVertexs[iPosIndex];
							std::map<int8, std::vector<int32>>::iterator vertexIte = tempMeshVertex->m_mapMeshVertex.find(pMesh->id);
							if (vertexIte != tempMeshVertex->m_mapMeshVertex.end())
							{
								vertexIte->second.push_back(pMesh->m_vecVertexs.size());
							}
							else
							{
								tempMeshVertex->m_mapMeshVertex[pMesh->id].push_back(pMesh->m_vecVertexs.size());
							}
							DexVector3 tempNormalPos = tempMeshVertex->meshPosition + vec_normal[iNormalIndex] * 10.0f;
							(tempMeshVertex->m_vecNormalPosition).push_back(tempNormalPos);
							tempMeshVertex->m_vecWorldNormal.push_back(DexVector3(0.0f, 0.0f, 0.0f));
							pMesh->m_vecVertexs.push_back(newVertex);
						}
						
						break;
			}
			case '#':
			case ' ':
				MoveNextLine(pBufferCurr, pBufferEnd);
				break;
			default:
				MoveNextLine(pBufferCurr, pBufferEnd);
				break;
			}
		}
		delete[] pBuffer;
	}
	else if (loadType == 1)
	{
		CommandScript* pScript = getComandScript();
		if (!pScript->OpenScript(filename))
		{
			pScript->CloseScript();
			getLog()->Log(log_error, "		load obj model %s failed!\n");
			getLog()->EndLog();
			delete skinMesh;
			return NULL;
		}
		char lineContent[512];
		std::vector<DexVector3> vec_pos;
		std::vector<DexVector2> vec_uv;
		std::vector<DexVector3> vec_normal;
		std::vector<string> out;
		stVertex3  tempVertex;
		DexVector3 tempV3;
		DexVector2 tempV2;
		//pos normal uv index
		map<int32, map<int32, map<int32, int32>>> mapVertexInfo;
		while (pScript->GetCurrLine() < pScript->GetScriptLine())
		{
			pScript->GetTotalLine(lineContent);
			if (strlen(lineContent) == 0)//空行
			{
				pScript->MoveCurrLine();
				continue;
			}
			if (lineContent[0] == '#') //注释行
			{
				pScript->MoveCurrLine();
				continue;
			}
			out.clear();
			SplitStr(lineContent, ' ', out);
			if (out[0] == "v")
			{//vertex
				tempV3.x = atof(out[1].c_str()); tempV3.y = atof(out[2].c_str()); tempV3.z = atof(out[3].c_str());
				vec_pos.push_back(tempV3);
				skinMesh->AddVertex(tempV3, NULL, NULL, 0);
			}
			else if (out[0] == "vt")
			{//uv
				tempV2.x = atof(out[1].c_str()); tempV2.y = atof(out[2].c_str());
				vec_uv.push_back(tempV2);
			}
			else if (out[0] == "vn")
			{//normal
				tempV3.x = atof(out[1].c_str()); tempV3.y = atof(out[2].c_str()); tempV3.z = atof(out[3].c_str());
				vec_normal.push_back(tempV3);
			}
			else if (out[0] == "f")
			{//triangle
				std::vector<string> vec_triangle;
				int32 iPosIndex = 0;
				int32 iNormalIndex = 0;
				int32 iUvIndex = 0;
				for (int p = 1; p <= 3; ++p)
				{
					vec_triangle.clear();
					SplitStr(out[p], '/', vec_triangle);
					iPosIndex = atoi(vec_triangle[0].c_str()) - 1;
					iUvIndex = atoi(vec_triangle[1].c_str()) - 1;
					iNormalIndex = atoi(vec_triangle[2].c_str()) - 1;
					if (mapVertexInfo.find(iPosIndex) != mapVertexInfo.end())
					{
						if (mapVertexInfo[iPosIndex].find(iNormalIndex) != mapVertexInfo[iPosIndex].end())
						{
							if (mapVertexInfo[iPosIndex][iNormalIndex].find(iUvIndex)
								!= mapVertexInfo[iPosIndex][iNormalIndex].end())
							{
								pMesh->AddVertexIndice(mapVertexInfo[iPosIndex][iNormalIndex][iUvIndex]);
								continue;
							}
						}
					}
					mapVertexInfo[iPosIndex][iNormalIndex][iUvIndex] = pMesh->m_vecVertexs.size();
					pMesh->AddVertexIndice(pMesh->m_vecVertexs.size());
					pMesh->m_vecVertexs.push_back(stVertex3(vec_pos[iPosIndex], vec_normal[iNormalIndex],
						vec_uv[iUvIndex].x, vec_uv[iUvIndex].y));
					/*这里并未直接调用DexMesh的AddVertex函数，AddVertex函数里面遍历vector进行查找判断
					当vector size非常大的时候，非常耗费时间，这里用了map来代替判断功能，能够加快一定的速度
					对于有3500+个顶点的obj,可节省时间2+秒
					*/
					//uint32 index = pMesh->AddVertex(vec_pos[iPosIndex], vec_normal[iNormalIndex], vec_uv[iUvIndex].x, vec_uv[iUvIndex].y);
					//pMesh->AddVertexIndice(index);
				}
			}
			pScript->MoveCurrLine();
		}
		pScript->CloseScript();
	}
	//skinMesh->CalculateVertex();
	Time = getTime()->GetTotalMillSeconds() - Time;
	getLog()->Log(log_ok, "load obj model %s ok, use time %d ms\n", filename, Time);
	getLog()->EndLog();
	return skinMesh;
}

void DexModelObjLoader::GetToken(int8*& pBuffer, int8* pOut, int8* pEnd)
{
	while (pBuffer <= pEnd)
	{
		if (*pBuffer == ' ' ||
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

void DexModelObjLoader::MoveNextLine(int8*& pBuffer, int8* pEnd)
{
	while (pBuffer <= pEnd)
	{
		if (*pBuffer++ == '\n')
		{
			break;
		}
	}
}