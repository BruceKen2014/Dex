

#include "..\DexBase\DexDefine.h"
#include "DexModelMwsLoader.h"
#include "..\DexBase\DexLog.h"
#include "..\DexBase\DexDMap.h"
#include "DexSkinMesh.h"

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

DexMaterial& DexModelMwsLoader::stMwsMaterial::stMaterialLayerData::getDexMaterial(DexMaterial& material)
{
	return material;
}
void DexModelMwsLoader::stMwsTree::buildMatrix()
{
	matrix.Identity();
	DexMatrix4x4 scaleMatrix;
	scaleMatrix.Scale(DexVector3(scale[0], scale[1], scale[2]));
	matrix = matrix * scaleMatrix;

	DexQuaternion quaternion(DexVector3(1, 0, 0), _getRadian(rotation[0]));
	matrix = matrix * quaternion.GetMatrix();
	quaternion.Set(DexVector3(0, 1, 0), -_getRadian(rotation[1]));
	matrix = matrix * quaternion.GetMatrix();
	quaternion.Set(DexVector3(0, 0, 1), _getRadian(rotation[2]));
	matrix = matrix * quaternion.GetMatrix();

	DexMatrix4x4 translate;
	translate.Translate(translation[0], translation[1], translation[2]);
	matrix *= translate;



	if (pFather != nullptr)
		matrix = matrix * pFather->matrix;
	for (DUDInt32 i = 0; i < children.size(); ++i)
		children[i]->buildMatrix();
}

bool DexModelMwsLoader::stMwsTree::isShape()
{
	return eTreeType == _stMwsTree::TYPE_GROUP ||
		eTreeType == _stMwsTree::TYPE_BILLBOARD_Y ||
		eTreeType == _stMwsTree::TYPE_BILLBOARD_XYZ ||
		eTreeType == _stMwsTree::TYPE_BILLBOARD2_Y ||
		eTreeType == _stMwsTree::TYPE_BILLBOARD2_XYZ;
}

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
	pRootTree = nullptr;
}

DexModelMwsLoader::~DexModelMwsLoader()
{
	freeModelData();
}

bool DexModelMwsLoader::readModelData(const char* filename)
{
	FILE* pFile = fopen(filename, "rb");
	if (pFile == NULL)
	{
		DexLog::getSingleton()->LogLine(log_error, "		open file: %s failed!", filename);
		return false;
	}
	const DInt16  iMaxLineByte = 2048;
	char tempLineData[iMaxLineByte];
	char LineData[iMaxLineByte * 2];
	char* pTempData = nullptr;
	DUInt8 iReadingType = ReadingNone;
	bool  bReadingCount = false;
	DUInt16 iDataIndex = 0;

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
				if (tempLineData[2] == 'E')
					iReadingType = ReadingTexture;
				else
					iReadingType = ReadingTree;
				break;
			case 'M':
				iReadingType = ReadingMaterial;
				break;
			case 'U':
				iReadingType = ReadingUV;
				break;
			case 'F':
				if (tempLineData[2] == 'A')
					iReadingType = ReadingFace;
				else
					iReadingType = ReadingNone;
				break;
			case 'G':
				iReadingType = ReadingGroup;
				break;
			default:
				//[POLYGON]、[FF12ATTRIBUTE]
				iReadingType = ReadingNone;
				break;
			}
			continue;
		case '\r':
		case '\n':
			continue;
		case '\"':
			if (tempLineData[1] == 'S' || tempLineData[1] == 'O')
			{
				bReadingCount = true;
				memset(LineData, 0, sizeof(LineData));
				continue;
			}
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
			{
				DUInt16 length = strlen(tempLineData);
				if (tempLineData[length - 3] == '\\')
				{//最后一个是\n 倒数第二个是\r
					tempLineData[length - 3] = '\0';
					strcat(LineData, tempLineData);
				}
				else
				{
					if (tempLineData[0] == '\"')
						//只有一行数据
						ReadGroup(tempLineData, pGroupData[iDataIndex++]);
					else//多行数据的最后一行
					{
						strcat(LineData, tempLineData);
						ReadGroup(LineData, pGroupData[iDataIndex++]);
						memset(LineData,0,sizeof(LineData));
					}
				}
			}
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
	fclose(pFile);
	return true;
}

void DexModelMwsLoader::buildTree()
{
	for (DUInt16 i = 0; i < iTreeCount; ++i)
	{
		stMwsTree* treeThis = &pTreeData[i];
		if (treeThis->iParentIndex >= 0)
		{
			stMwsTree* treeParent = &pTreeData[treeThis->iParentIndex];
			treeThis->pFather = treeParent;
			treeParent->children.push_back(treeThis);
		}
	}
	pRootTree->buildMatrix();
	for (DUInt16 i = 0; i < iTreeCount; ++i)
	{
		pTreeData[i].matrix.ConvertHandMatrix();
	}
}

void DexModelMwsLoader::AddMesh(stMwsTree* pTree, DexSkinMesh* pSkinMesh)
{
	//暂时只考虑静态mws文件
	DEX_ENSURE(pTree != nullptr && pSkinMesh != nullptr);
	stMwsGroup* pGroup = &pGroupData[pTree->iGroupIndex];
	DUInt16 iVertexIndex = 0;
	DUInt16 iNormalIndex = 0;
	DUInt16 iColorIndex = 0;
	DUInt16 iUvIndex = 0;
	DMap<stIndex, DInt32> mapVertexInfo;
	stIndex index;
	DexSkinMesh::stMeshVertex newVertex;
	DexSkinMesh::DexMesh* pMesh = nullptr;
	for (DUInt16 f = 0; f < pGroup->iFaceCount; ++f)
	{
		DUInt16 iFaceIndex = pGroup->pFaceIndex[f];
		stMwsFace* pFace = &pFaceData[iFaceIndex];
		if (pMesh == nullptr || pMesh->m_iMaterialId != pFace->pMaterialIndex[0])
		{//drawcall合并，寻找属于这个材质的mesh，找不到则创建
			pMesh = pSkinMesh->FindMesh(pFace->pMaterialIndex[0], true);
			pMesh->m_iMaterialId = pFace->pMaterialIndex[0];
			pMesh->m_iTextureId = pMaterialData[pMesh->m_iMaterialId].pData[0].textureIndex;
			mapVertexInfo.clear();
		}
		if (pFace->iVertices == 3)
		{
			for (DUInt16 v = 0; v < pFace->iVertices; ++v)
			{//temp consider pFace->iTimes == 1
				iVertexIndex = pFace->pVertexIndex[v];
				stMwsVertex* pVertex = &pVertexData[iVertexIndex];
				newVertex.pos.x = pVertex->pos[0]; newVertex.pos.y = pVertex->pos[1]; newVertex.pos.z = -pVertex->pos[2];

				iNormalIndex = pFace->pNormalIndex[v];
				stMwsNormal* pNormal = &pNormalData[iNormalIndex];
				newVertex.normal.x = pNormal->normal[0]; newVertex.normal.y = pNormal->normal[1]; newVertex.normal.z = -pNormal->normal[2];

				iColorIndex = pFace->pColorIndex[v];
				stMwsColor* pColor = &pColorData[iColorIndex];
				newVertex.color.r = pColor->color[0] * 255; newVertex.color.g = pColor->color[1] * 255; newVertex.color.b = pColor->color[2] * 255; newVertex.color.a = pColor->color[3] * 255;

				iUvIndex = pFace->pUvIndex[v];
				stMwsUv* pUv = &pUvData[iUvIndex];
				newVertex.uv.x = pUv->uv[0]; 
				newVertex.uv.y = 1.0f - pUv->uv[1];

				index.m_iPosIndex = iVertexIndex;
				index.m_iNormalIndex = iNormalIndex;
				index.m_iColorIndex = iColorIndex;
				index.m_iUvIndex = iUvIndex;

				DMap<stIndex, DInt32>::iterator ite = mapVertexInfo.find(index);
				if (ite != mapVertexInfo.end())
					pMesh->AddVertexIndice(ite->second);
				else
				{
					DexMatrix4x4::TransformCoord(newVertex.pos, pTree->matrix);
					DexMatrix4x4::TransformNormal(newVertex.normal, pTree->matrix);
					mapVertexInfo.insert(std::make_pair(index, pMesh->m_vecVertexsBuffer.size()));
					pMesh->AddVertexIndice(pMesh->m_vecVertexsBuffer.size());
					pMesh->m_vecVertexsBuffer.push_back(newVertex);
				}
			}
		}
		else if (pFace->iVertices == 4)
		{
			int indices1[] = { 0, 1, 2 };
			int indices2[] = { 0, 2, 3 };
			for (DUInt16 n = 0; n < 2; ++n)
			{
				for (DUInt16 i = 0; i < 3; ++i)
				{//temp consider pFace->iTimes == 1
					DUInt16 v = 0; 
					if (n == 0)
						v = indices1[i];
					else
						v = indices2[i];
					iVertexIndex = pFace->pVertexIndex[v];
					stMwsVertex* pVertex = &pVertexData[iVertexIndex];
					newVertex.pos.x = pVertex->pos[0]; newVertex.pos.y = pVertex->pos[1]; newVertex.pos.z = -pVertex->pos[2];

					iNormalIndex = pFace->pNormalIndex[v];
					stMwsNormal* pNormal = &pNormalData[iNormalIndex];
					newVertex.normal.x = pNormal->normal[0]; newVertex.normal.y = pNormal->normal[1]; newVertex.normal.z = -pNormal->normal[2];

					iColorIndex = pFace->pColorIndex[v];
					stMwsColor* pColor = &pColorData[iColorIndex];
					newVertex.color.r = pColor->color[0] * 255; newVertex.color.g = pColor->color[1] * 255; newVertex.color.b = pColor->color[2] * 255; newVertex.color.a = pColor->color[3] * 255;

					iUvIndex = pFace->pUvIndex[v];
					stMwsUv* pUv = &pUvData[iUvIndex];
					newVertex.uv.x = pUv->uv[0];
					newVertex.uv.y = 1.0 - pUv->uv[1];

					index.m_iPosIndex = iVertexIndex;
					index.m_iNormalIndex = iNormalIndex;
					index.m_iColorIndex = iColorIndex;
					index.m_iUvIndex = iUvIndex;

					DMap<stIndex, DInt32>::iterator ite = mapVertexInfo.find(index);
					if (ite != mapVertexInfo.end())
						pMesh->AddVertexIndice(ite->second);
					else
					{
						DexMatrix4x4::TransformCoord(newVertex.pos, pTree->matrix);
						DexMatrix4x4::TransformNormal(newVertex.normal, pTree->matrix);
						mapVertexInfo.insert(std::make_pair(index, pMesh->m_vecVertexsBuffer.size()));
						pMesh->AddVertexIndice(pMesh->m_vecVertexsBuffer.size());
						pMesh->m_vecVertexsBuffer.push_back(newVertex);
					}
				}
			}

		}
		
	}
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

int DexModelMwsLoader::getNextInt(const char*& ptr, char splitChar[3])
{
	char tempData[32];
	DUInt16 iIndex = 0;
	while (*ptr != splitChar[0] && *ptr != splitChar[1] && *ptr != splitChar[2])
		tempData[iIndex++] = *ptr++;
	ptr++;//skip split char
	tempData[iIndex] = '\0';
	return atoi(tempData);
}

bool DexModelMwsLoader::getNextBool(const char*& ptr, char splitChar[3])
{
	return getNextInt(ptr, splitChar) == 1;
}

DFloat64 DexModelMwsLoader::getNextDouble(const char*& ptr, char splitChar[3])
{
	char tempData[32];
	DUInt16 iIndex = 0;
	while (*ptr != splitChar[0] && *ptr != splitChar[1] && *ptr != splitChar[2])
		tempData[iIndex++] = *ptr++;
	ptr++;//skip split char
	tempData[iIndex] = '\0';
	return atof(tempData);
}

float DexModelMwsLoader::getNextFloat(const char*& ptr, char splitChar[3])
{
	return getNextDouble(ptr, splitChar);
}

void DexModelMwsLoader::getNextString(const char*& ptr, DString& str)
{
	char tempCharArr[128];
	DUInt8 index = 0;
	while (*ptr != '\"')
		ptr++;
	ptr++;// skip the first "
	
	while (*ptr != '\"')
		tempCharArr[index++] = *ptr++;
	tempCharArr[index] = '\0';
	str = tempCharArr;
	ptr++;//skip the second "
	ptr++; //skip the split char
}
void DexModelMwsLoader::getNextString(const char*& ptr, char* str)
{
	DUInt8 index = 0;
	while (*ptr != '\"')
		ptr++;
	ptr++;// skip the first "
	while (*ptr != '\"')
		str[index++] = *ptr++;
	str[index] = '\0';
	ptr++;//skip the second "
	ptr++; //skip the split char
}
void DexModelMwsLoader::ReadVertex(const char* data, stMwsVertex& vertex)
{
	char splitChar[3] = { ',', ';','\r' };
	const char* ptr = data;
	vertex.pos[0] = getNextDouble(ptr, splitChar);
	vertex.pos[1] = getNextDouble(ptr, splitChar);
	vertex.pos[2] = getNextDouble(ptr, splitChar);
}
void DexModelMwsLoader::ReadNormal(const char* data, stMwsNormal& normal)
{
	char splitChar[3] = { ',', '\r', '\n'};
	const char* ptr = data;
	normal.normal[0] = getNextDouble(ptr, splitChar);
	normal.normal[1] = getNextDouble(ptr, splitChar);
	splitChar[0] = '\n';
	normal.normal[2] = getNextDouble(ptr, splitChar);
}	
void DexModelMwsLoader::ReadUv(const char* data, stMwsUv& uv)
{
	char splitChar[3] = { ',', '\r', '\n' };
	const char* ptr = data;
	uv.uv[0] = getNextDouble(ptr, splitChar);
	splitChar[0] = '\n';
	uv.uv[1] = getNextDouble(ptr, splitChar);
}
void DexModelMwsLoader::ReadColor(const char* data, stMwsColor& color)
{
	char splitChar[3] = { ',', '\r', '\n' };
	const char* ptr = data;
	color.color[0] = getNextDouble(ptr, splitChar);
	color.color[1] = getNextDouble(ptr, splitChar);
	color.color[2] = getNextDouble(ptr, splitChar);
	splitChar[0] = '\n';
	color.color[3] = getNextDouble(ptr, splitChar);
}
void DexModelMwsLoader::ReadTexture(const char* data, stMwsTexture& texture)
{
	DUInt16 nameSize = strlen(data)-4;//首尾两个引号和\r\n
	memcpy(texture.sName, &data[1], nameSize);
	texture.sName[nameSize - 3] = 't';
	texture.sName[nameSize - 2] = 'g';
	texture.sName[nameSize - 1] = 'a';
	texture.sName[nameSize] = '\0';
}
void DexModelMwsLoader::ReadMaterial(const char* data, stMwsMaterial& material)
{
	char splitChar[3] = { ',', '\r', '\n' };
	getNextString(data, material.sName); 
	material.iNumLayers = getNextInt(data, splitChar);
	material.pData = new stMwsMaterial::stMaterialLayerData[material.iNumLayers];
	for (DUInt16 i = 0; i < material.iNumLayers; ++i)
	{
		stMwsMaterial::stMaterialLayerData* mld = &material.pData[i];
		mld->textureIndex = getNextInt(data, splitChar);
		mld->wrap = getNextBool(data, splitChar);
		mld->wrapU = getNextBool(data, splitChar);
		mld->wrapV = getNextBool(data, splitChar);
		mld->culling = getNextBool(data, splitChar);
		mld->reflection = getNextBool(data, splitChar);
		mld->reflectionEye = getNextBool(data, splitChar);
		mld->reflectionUV = getNextBool(data, splitChar);
		getNextString(data, mld->reflectionTextureName);
		mld->rayDirection[0] = getNextDouble(data, splitChar);
		mld->rayDirection[1] = getNextDouble(data, splitChar);
		mld->rayDirection[2] = getNextDouble(data, splitChar);
		mld->fresnel = getNextInt(data, splitChar) == 1;
		mld->fresnelMode = getNextInt(data, splitChar) == 1;
		mld->fresnelEffectColor[0] = getNextDouble(data, splitChar);
		mld->fresnelEffectColor[1] = getNextDouble(data, splitChar);
		mld->fresnelEffectColor[2] = getNextDouble(data, splitChar);
		mld->fresnelEffectColor[3] = getNextDouble(data, splitChar);
		mld->fresnelBaseColor[0] = getNextDouble(data, splitChar);
		mld->fresnelBaseColor[1] = getNextDouble(data, splitChar);
		mld->fresnelBaseColor[2] = getNextDouble(data, splitChar);
		mld->fresnelBaseColor[3] = getNextDouble(data, splitChar);
		mld->fresnelIntensity = getNextDouble(data, splitChar);
		mld->alphaBlend = getNextBool(data, splitChar);
		mld->alphaBlendMode[0] = getNextInt(data, splitChar);
		mld->alphaBlendMode[1] = getNextInt(data, splitChar);
		mld->alphaBlendMode[2] = getNextInt(data, splitChar);
		mld->alphaBlendMode[3] = getNextInt(data, splitChar);
		mld->alphaBlendFix = getNextDouble(data, splitChar);
		mld->paletteNo = getNextInt(data, splitChar);
		mld->zSort = getNextBool(data, splitChar);
		mld->specular = getNextBool(data, splitChar);
		mld->specularColor[0] = getNextDouble(data, splitChar);
		mld->specularColor[1] = getNextDouble(data, splitChar);
		mld->specularColor[2] = getNextDouble(data, splitChar);
		mld->specularColor[3] = getNextDouble(data, splitChar);
		mld->specularIntensity = getNextDouble(data, splitChar);
		mld->color[0] = getNextDouble(data, splitChar);
		mld->color[1] = getNextDouble(data, splitChar);
		mld->color[2] = getNextDouble(data, splitChar);
		mld->color[3] = getNextDouble(data, splitChar);
	}
}
void DexModelMwsLoader::ReadFace(const char* data, stMwsFace& face)
{
	char splitChar[3] = { ',', '\r', '\n' };
	const char* ptr = data;
	face.iTimes = getNextInt(ptr, splitChar);
	face.iVertices = getNextInt(ptr, splitChar);

	face.pVertexIndex = new DUInt16[face.iVertices];
	face.pNormalIndex = new DUInt16[face.iTimes*face.iVertices];
	face.pColorIndex = new DUInt16[face.iTimes*face.iVertices];
	face.pUvIndex = new DUInt16[face.iTimes*face.iVertices];
	face.pMaterialIndex = new DUInt16[face.iTimes];

	for (DUInt16 i = 0; i < face.iVertices; ++i)
		face.pVertexIndex[i] = getNextInt(ptr, splitChar);
	for (DUInt16 t = 0; t < face.iTimes; ++t)
	{
		for (DUInt16 v = 0; v < face.iVertices; ++v)
		{
			face.pNormalIndex[t*face.iVertices + v] = getNextInt(ptr, splitChar);
			face.pColorIndex[t*face.iVertices + v] = getNextInt(ptr, splitChar);
			face.pUvIndex[t*face.iVertices + v] = getNextInt(ptr, splitChar);
		}
		face.pMaterialIndex[t] = getNextInt(ptr, splitChar);
	}

}
void DexModelMwsLoader::ReadGroup(const char* data, stMwsGroup& group)
{
	char splitChar[3] = { ',', '\r', '\n' };
	getNextString(data, group.sName);
	getNextString(data, group.sParentName);
	group.iFaceCount = getNextInt(data, splitChar);
	group.pFaceIndex = new DUInt16[group.iFaceCount];
	for (DUInt16 i = 0; i < group.iFaceCount; ++i)
	{
		group.pFaceIndex[i] = getNextInt(data, splitChar); 
	}
}
void DexModelMwsLoader::ReadTree(const char* data, stMwsTree& tree)
{
	char splitChar[3] = { ',', '\r', '\n' };
	getNextString(data, tree.sName);
	tree.iParentIndex = getNextInt(data, splitChar);
	DString type;
	getNextString(data, type);
	if (type == "transform")
		tree.eTreeType = _stMwsTree::TYPE_TRANSFORM;
	else if (type == "shape")
		tree.eTreeType = _stMwsTree::TYPE_GROUP;
	else if (type == "joint")
		tree.eTreeType = _stMwsTree::TYPE_JOINT;
	else if (type == "effector")
		tree.eTreeType = _stMwsTree::TYPE_EFFECTOR;
	else if (type == "chain")
		tree.eTreeType = _stMwsTree::TYPE_CHAIN;
	else if (type == "null_skeleton")
		tree.eTreeType = _stMwsTree::TYPE_NULL_SKELETON;
	else if (type == "lod_group")
		tree.eTreeType = _stMwsTree::TYPE_LOD_GROUP;
	else if (type == "billboard_y")
		tree.eTreeType = _stMwsTree::TYPE_BILLBOARD_Y;
	else if (type == "billboard_xyz")
		tree.eTreeType = _stMwsTree::TYPE_BILLBOARD_XYZ;
	else if (type == "billboard2_y")
		tree.eTreeType = _stMwsTree::TYPE_BILLBOARD2_Y;
	else if (type == "billboard2_xyz")
		tree.eTreeType = _stMwsTree::TYPE_BILLBOARD2_XYZ;
	else if (type == "lod_group")
		tree.eTreeType = _stMwsTree::TYPE_LOD_GROUP;
	else
	{
		//暂不读入
	}
	if (0)//tree.eTreeType == _stMwsTree::TYPE_TRANSFORM)
	{
		getNextDouble(data, splitChar);
		getNextDouble(data, splitChar);
		getNextDouble(data, splitChar);
		tree.translation[0] = 0;
		tree.translation[1] = 0;
		tree.translation[2] = 0;
	}
	else
	{
		tree.translation[0] = getNextDouble(data, splitChar);
		tree.translation[1] = getNextDouble(data, splitChar);
		tree.translation[2] = getNextDouble(data, splitChar);
	}

	tree.rotation[0] = getNextDouble(data, splitChar);
	tree.rotation[1] = getNextDouble(data, splitChar);
	tree.rotation[2] = getNextDouble(data, splitChar);
	tree.scale[0] = getNextDouble(data, splitChar);
	tree.scale[1] = getNextDouble(data, splitChar);
	tree.scale[2] = getNextDouble(data, splitChar);
	if (tree.isShape())
		tree.iGroupIndex = getNextInt(data, splitChar);
	else if (tree.eTreeType == _stMwsTree::TYPE_LOD_GROUP)
	{
		tree.iNumLods = getNextInt(data, splitChar);
		for (DUInt16 i = 0; i < tree.iNumLods; ++i)
			tree.lodThresold[i] = getNextDouble(data, splitChar);
	}
	if (tree.iParentIndex == -1)
		pRootTree = &tree;
}

bool DexModelMwsLoader::SupportType(const char* fileType)
{
	return dexstricmp(fileType, ".mws") == 0;
}
DexModelBase* DexModelMwsLoader::LoadModel(const char* filename, DInt32 flag)
{
	DexLog::getSingleton()->LogLine(log_ok, "load mws model %s...", filename);
	DInt64 Time = getTime()->GetTotalMillSeconds();
	bool ret = readModelData(filename);
	DEX_ENSURE_P(ret);
	buildTree();
	DexSkinMesh* pDexSkinMesh = new DexSkinMesh;
	
	//texture
	for (DUInt16 t = 0; t < iTextureCount; ++t)
		pDexSkinMesh->AddTexture(pTextureData[t].sName);
		
	//material
	for (DUInt16 i = 0; i < iMaterialCount; ++i)
	{
		DexMaterial material;
		pMaterialData[i].pData[0].getDexMaterial(material);
		pDexSkinMesh->AddMaterial(material);
	}
	for (DUInt16 i = 0; i < iTreeCount; ++i)
	{
		stMwsTree* pTree = &pTreeData[i];
		if (pTree->isShape())
		{
			DexMatrix4x4 matrix = pTree->matrix;
			stMwsGroup* pGroup = &pGroupData[pTree->iGroupIndex];
			AddMesh(pTree, pDexSkinMesh);
		}
	}

	freeModelData();
	Time = getTime()->GetTotalMillSeconds() - Time;
	DexLog::getSingleton()->LogLine(log_ok, "load mws model %s ok, use time %d ms", filename, Time);
	return pDexSkinMesh;
}

bool DexModelMwsLoader::SaveModel(DexSkinMesh* pSkinMesh, const char* filename, DInt32 flag)
{
	return true;
}