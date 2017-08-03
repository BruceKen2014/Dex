
#include "DexSkinMeshFF.h"
#include "DexModelAniFileFF12ActLoader.h"


DexModelAniFileFF12ActLoader::DexModelAniFileFF12ActLoader()
{

}

DexModelAniFileFF12ActLoader::~DexModelAniFileFF12ActLoader()
{

}

bool DexModelAniFileFF12ActLoader::SupportType(const char* fileType)
{
	return dexstricmp(fileType, ".act") == 0;
}

bool DexModelAniFileFF12ActLoader::LoadAnimationFile(DexModelBase* pModel, const char* filename)
{
	DEX_ENSURE_B(pModel != nullptr && pModel->getType() == DexSkinMeshFF::getClassType());
	DexSkinMeshFF* pSkinMeshFF = (DexSkinMeshFF*)pModel;

	DexMem mem;
	DEX_ENSURE_B(mem.IniFromFile(filename));
	//将之前的动画信息清除
	for (size_t i = 0; i < pSkinMeshFF->m_vecMwsJoints.size(); ++i)
		pSkinMeshFF->m_vecMwsJoints[i]->m_vecKeyFrames.clear();

	DexSkinMesh::Joint* pJoint = nullptr;
	const DInt16  iMaxLineByte = 256;
	const DInt16  iMaxJointNameByte = 64;
	const DInt16  iFps = 1000 / 30; //每秒60帧
	DInt16        iFrames = 0;//该动画有多少帧
	char tempData[iMaxLineByte];
	char jointName[iMaxJointNameByte];
	DexVector3 oldTranslate;
	DexVector3 oldRotation;
	DVector<DString> values;
	while (!mem.End())
	{
		mem.ReadLine(tempData);
		if (pJoint != nullptr)
		{
			values.clear();
			SplitStr(tempData, ',', values);
			if (strlen(tempData) == 0)
			{
				pJoint = nullptr;
				continue;
			}
			DexMatrix4x4 matrix;
			DexQuaternion quatenion;

			DexVector3 translate;
			if (values[1].length() == 1)
				translate.x = oldTranslate.x;
			else
				translate.x = atof(values[1].c_str());
			if (values[2].length() == 1)
				translate.y = oldTranslate.y;
			else
				translate.y = atof(values[2].c_str());
			if (values[3].length() == 1)
				translate.z = oldTranslate.z;
			else
				translate.z = atof(values[3].c_str());

			DexVector3 rotation;
			if (values[4].length() == 1)
				rotation.x = oldRotation.x;
			else
				rotation.x = -_getRadian(atof(values[4].c_str()));
			if (values[5].length() == 1)
				rotation.y = oldRotation.y;
			else
				rotation.y = -_getRadian(atof(values[5].c_str()));
			if (values[6].length() == 1)
				rotation.z = oldRotation.z;
			else
				rotation.z = -_getRadian(atof(values[6].c_str()));

			quatenion.Set(DexVector3(1, 0, 0), rotation.x);
			matrix = matrix * quatenion.GetMatrix();
			quatenion.Set(DexVector3(0, 1, 0), rotation.y);
			matrix = matrix * quatenion.GetMatrix();
			quatenion.Set(DexVector3(0, 0, 1), rotation.z);
			matrix = matrix * quatenion.GetMatrix();

			matrix.Translate(translate);
			matrix.ConvertHandMatrix();

			pJoint->AddKeyFrame((atoi(values[0].c_str()) - 1) * iFps, matrix);
			oldTranslate = translate;
			oldRotation = rotation;
		}
		else
		{
			switch (tempData[0])
			{
			case '[':
				for (DInt16 i = 1; i < iMaxJointNameByte; ++i)
				{
					if (tempData[i] == ']')
					{
						jointName[i - 1] = 0;
						break;
					}
					else
						jointName[i - 1] = tempData[i];
				}
				//if (strcmp(jointName, "n_hara") == 0)
				pJoint = pSkinMeshFF->FindMwsJoint(jointName);
				break;
			case '$':
				//like $timeLength=300
				iFrames = atoi(&tempData[12]);
				break;
			default:
				break;
			}
		}

	}
	pSkinMeshFF->SetMaxAniTime((iFrames - 1) * iFps);
	return true;
}