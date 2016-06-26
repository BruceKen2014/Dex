

#include "..\DexBase\DexDefine.h"
#include "DexModelMwsLoader.h"
#include "..\DexBase\DexLog.h"

#define ReadingNone			0
#define ReadingVertex		1
#define ReadingNormal		2
#define ReadingColor		3
#define ReadingUV			4
#define ReadingTexture		5
#define ReadingMaterial		6
#define ReadingFace			7
#define ReadingGroup		8
#define ReadingTree			9
DexModelMwsLoader::DexModelMwsLoader()
{
	iMwsVersion = 0;
	iVertexCount = 0;
	iNormalCount = 0;
	iUvCount = 0;
	iColorCount = 0;
	iTextureCount = 0;
	iMaterialCount = 0;
	iFaceCount = 0;
	iGroupCount = 0;
	iTreeCount = 0;

	pVertexData = nullptr;
	pNormalData = nullptr;
	pUvData = nullptr;
	pColorData = nullptr;
	pTextureData = nullptr;
	pMaterialData = nullptr;
	pFaceData = nullptr;
	pGroupData = nullptr;
	pTreeData = nullptr;
}

DexModelMwsLoader::~DexModelMwsLoader()
{
	freeModelData();
}

void DexModelMwsLoader::freeModelData()
{
	_SafeDeleteArr(pVertexData);
	_SafeDeleteArr(pNormalData);
	_SafeDeleteArr(pUvData);
	_SafeDeleteArr(pColorData);
	_SafeDeleteArr(pTextureData);
	_SafeDeleteArr(pMaterialData);
	_SafeDeleteArr(pFaceData);
	_SafeDeleteArr(pGroupData);
	_SafeDeleteArr(pTreeData);
}

int DexModelMwsLoader::getNextInt(const char*& ptr, char splitChar[2])
{
	char tempData[32];
	uint16 iIndex = 0;
	while (*ptr != splitChar[0] && *ptr != splitChar[1])
		tempData[iIndex++] = *ptr++;
	tempData[iIndex] = '\0';
	return atoi(tempData);
}

float DexModelMwsLoader::getNextFloat(const char*& ptr, char splitChar[2])
{
	char tempData[32];
	uint16 iIndex = 0;
	while (*ptr != splitChar[0] && *ptr != splitChar[1])
		tempData[iIndex++] = *ptr++;
	ptr++;//skip split char
	tempData[iIndex] = '\0';
	return atof(tempData);
}

void DexModelMwsLoader::ReadVertex(const char* data, stMwsVertex& vertex)
{
	char splitChar[2] = { ',', ';' };
	const char* ptr = data;
	vertex.pos.x = getNextFloat(ptr, splitChar);
	vertex.pos.y = getNextFloat(ptr, splitChar);
	vertex.pos.z = getNextFloat(ptr, splitChar);
}
void DexModelMwsLoader::ReadNormal(const char* data, stMwsNormal& normal)
{
	char splitChar[2] = { ',', '\r' };
	const char* ptr = data;
	normal.normal.x = getNextFloat(ptr, splitChar);
	normal.normal.y = getNextFloat(ptr, splitChar);
	splitChar[0] = '\n';
	normal.normal.z = getNextFloat(ptr, splitChar);
}	
void DexModelMwsLoader::ReadUv(const char* data, stMwsUv& uv)
{
	char splitChar[2] = { ',', '\r' };
	const char* ptr = data;
	uv.uv.x = getNextFloat(ptr, splitChar);
	splitChar[0] = '\n';
	uv.uv.y = getNextFloat(ptr, splitChar);
}
void DexModelMwsLoader::ReadColor(const char* data, stMwsColor& color)
{
	char splitChar[2] = { ',', '\r' };
	const char* ptr = data;
	color.color.x = getNextFloat(ptr, splitChar);
	color.color.y = getNextFloat(ptr, splitChar);
	color.color.z = getNextFloat(ptr, splitChar);
	splitChar[0] = '\n';
	color.color.w = getNextFloat(ptr, splitChar);
}
void DexModelMwsLoader::ReadTexture(const char* data, stMwsTexture& texture)
{
	uint16 nameSize = strlen(data) - 4 - 1;//tm2" and the first "
	memcpy(texture.sName, &data[1], nameSize);
	strcat(texture.sName, "tga\0");
	//memcpy(&texture.sName[nameSize], "tga\0", 5);
}
void DexModelMwsLoader::ReadMaterial(const char* data, stMwsMaterial& material)
{}
void DexModelMwsLoader::ReadFace(const char* data, stMwsFace& face)
{}
void DexModelMwsLoader::ReadGroup(const char* data, stMwsGroup& group)
{}
void DexModelMwsLoader::ReadTree(const char* data, stMwsTree& tree)
{}

bool DexModelMwsLoader::SupportType(const char* fileType)
{
	return dexstricmp(fileType, ".mws") == 0;
}
DexModelBase* DexModelMwsLoader::LoadModel(const char* filename, int32 flag)
{
	FILE* pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		getLog()->LogLine(log_error, "		open file: %s failed!", filename);
		return nullptr;
	}
	const int16  iMaxLineByte = 512;
	char tempLineData[iMaxLineByte];
	char* pTempData = nullptr;
	uint8 iReadingType = ReadingNone;
	bool  bReadingCount = false;
	uint16 iDataIndex = 0;
	while (!feof(pFile))
	{
		fgets(tempLineData, iMaxLineByte, pFile);
		switch (tempLineData[0])
		{
		case '@':
			pTempData = (char*)&(tempLineData[4]);
			iMwsVersion = atoi(pTempData);
			break;
		case '[':
			switch (tempLineData[1])
			{
			case 'V':
				iReadingType = ReadingVertex;
				break;
			case 'N':
				iReadingType = ReadingNormal;
				break;
			case 'C':
				iReadingType = ReadingColor;
				break;
			case 'T':
			{
				if (tempLineData[2] == 'E')
					iReadingType = ReadingTexture;
				else
					iReadingType = ReadingTree;
			}
				break;
			case 'M':
				iReadingType = ReadingMaterial;
				break;
			case 'U':
				iReadingType = ReadingUV;
				break;
			case 'F':
				iReadingType = ReadingFace;
				break;
			case 'G':
				iReadingType = ReadingGroup;
				break;
			default:
				//[POLYGON]¡¢[FF12ATTRIBUTE]
				break;
			}
			continue;
		case '\r':
		case '\n':
			continue;
		case '\"':
			bReadingCount = true;
			continue;
		default:
			break;
		}
		if (bReadingCount)
		{
			switch (iReadingType)
			{
			case ReadingVertex:
				iVertexCount = atoi(tempLineData);
				pVertexData = new stMwsVertex[iVertexCount];
				break;
			case ReadingNormal:
				iNormalCount = atoi(tempLineData);
				pNormalData = new stMwsNormal[iNormalCount];
				break;
			case ReadingColor:
				iColorCount = atoi(tempLineData);
				pColorData = new stMwsColor[iColorCount];
				break;
			case ReadingUV:
				iUvCount = atoi(tempLineData);
				pUvData = new stMwsUv[iUvCount];
				break;
			case ReadingTexture:
				iTextureCount = atoi(tempLineData);
				pTextureData = new stMwsTexture[iTextureCount];
				break;
			case ReadingMaterial:
				iMaterialCount = atoi(tempLineData);
				pMaterialData = new stMwsMaterial[iMaterialCount];
				break;
			case ReadingFace:
				iFaceCount = atoi(tempLineData);
				pFaceData = new stMwsFace[iFaceCount];
				break;
			case ReadingGroup:
				iGroupCount = atoi(tempLineData);
				pGroupData = new stMwsGroup[iGroupCount];
				break;
			case ReadingTree:
				iTreeCount = atoi(tempLineData);
				pTreeData = new stMwsTree[iTreeCount];
				break;
			default:
				break;
			}
			iDataIndex = 0;
			bReadingCount = false;
		}
		else
		{
			switch (iReadingType)
			{
			case ReadingVertex:
				ReadVertex(tempLineData, pVertexData[iDataIndex++]);
				break;
			case ReadingNormal:
				ReadNormal(tempLineData, pNormalData[iDataIndex++]);
				break;
			case ReadingUV:
				ReadUv(tempLineData, pUvData[iDataIndex++]);
				break;
			case ReadingColor:
				ReadColor(tempLineData, pColorData[iDataIndex++]);
				break;
			case ReadingTexture:
				ReadTexture(tempLineData, pTextureData[iDataIndex++]);
				break;
			case ReadingMaterial:
				ReadMaterial(tempLineData, pMaterialData[iDataIndex++]);
				break;
			case ReadingGroup:
				ReadGroup(tempLineData, pGroupData[iDataIndex++]);
				break;
			case ReadingTree:
				ReadTree(tempLineData, pTreeData[iDataIndex++]);
				break;
			case ReadingFace:
				ReadFace(tempLineData, pFaceData[iDataIndex++]);
				break;
			default:
				break;
			}
		}

	}
	freeModelData();
	return nullptr;
}