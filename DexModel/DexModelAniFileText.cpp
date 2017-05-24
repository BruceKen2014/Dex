
#include "..\DexBase\DexString.h"
#include "..\DexBase\DexLog.h"
#include "DexSkinMesh.h"
#include "DexModelAniFileText.h"
#include "..\DexCommonFunction.h"

#define MAX_LINE_BYTE 2048
#define getFloat DexCommonFunction::str_to_float(vecStr[index++])
#define getInt   DexCommonFunction::str_to_int(vecStr[index++])
DexModelAniFileTextLoader::DexModelAniFileTextLoader()
{

}
DexModelAniFileTextLoader::~DexModelAniFileTextLoader()
{

}

bool DexModelAniFileTextLoader::SupportType(const char* fileType)
{
	return dexstricmp(fileType, ".dexact") == 0;
}

bool DexModelAniFileTextLoader::LoadAnimationFile(DexModelBase* pModel, const char* filename)
{
	DEX_ENSURE_B(pModel != DexNull);
	DexSkinMesh* pSkinMesh = (DexSkinMesh*)pModel;
	DexMem mem;
	DEX_ENSURE_B(mem.IniFromFile(filename));
	getLog()->LogLine(log_ok, "load dex model animation text file: %s...", filename);
	int64 Time = getTime()->GetTotalMillSeconds();

	const int16  iMaxLineByte = MAX_LINE_BYTE;
	char tempLineData[iMaxLineByte];
	while (!mem.End())
	{
		mem.ReadLine(tempLineData);
		if (tempLineData[0] == '[')
		{
			char Type = tempLineData[1];
			if (Type == 'a')
			{//animationTime
				ReadAnimationTime(&mem, pSkinMesh, tempLineData);
			}
			else if (Type == 'j')
			{//jointAnimationData
				ReadJointAnimation(&mem, pSkinMesh, tempLineData);
			}
		}
	}
	Time = getTime()->GetTotalMillSeconds() - Time;
	getLog()->LogLine(log_ok, "load dex model animation text file:%s ok, use time %d ms", filename, Time);
	return true;
}

void DexModelAniFileTextLoader::ReadAnimationTime(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer)
{
	pMem->ReadLine(pBuffer);
	int16 animationTime = DexCommonFunction::str_to_int(pBuffer);
	pSkinMesh->SetMaxAniTime(animationTime);
}

void DexModelAniFileTextLoader::ReadJointAnimation(DexMem* pMem, DexSkinMesh* pSkinMesh, char* pBuffer)
{
	pMem->ReadLine(pBuffer);
	DVector<DString> vecStr;
	DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
	DexSkinMesh::Joint* pJoint = pSkinMesh->FindJoint(vecStr[0]);
	uint16 iFrames= DexCommonFunction::str_to_int(vecStr[1]);
	for (uint16 f = 0; f < iFrames; ++f)
	{
		pMem->ReadLine(pBuffer);
		vecStr.clear();
		DexCommonFunction::SplitStr(pBuffer, ',', vecStr);
		uint16 index = 0;
		uint16 iFrameTime = getInt;
		DexVector3 translation; translation.x = getFloat; translation.y = getFloat; translation.z = getFloat;
		DexVector3 scale; scale.x = getFloat; scale.y = getFloat; scale.z = getFloat;
		DexVector3 axis; axis.x = getFloat; axis.y = getFloat; axis.z = getFloat;
		float32 radian = _getDegree(getFloat);
		pSkinMesh->AddJointKeyFrame(pJoint, iFrameTime, translation, scale, axis, radian);
	}
}
#undef getFloat
#undef getInt