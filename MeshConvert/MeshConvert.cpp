
/*
本工具用于模型的转换，对于ms3d、obj、dae模型，引擎本身提供模型之间的数据转换功能，本工具用于接收用户参数，执行相关的转换操作
^.^Bruce
*/

#pragma comment(lib, "../bin/Dex.lib")
#include "stdafx.h"
#include <iostream>
#include "../state/DexGameEngine.h"
#include "../DexModel/DexSkinMesh.h"
#include "../DexBase/DexCommonFunction.h"
#include "../DexBase/DexStreamFile.h"
#include "../DexBase/DexMd5Generator.h"
using namespace std;
#define FILENAME_MAX_LENGTH   128
int main(int argc, char* argv[])
{
	//char* resModelFile = "D:/Dex/ParticleEditer/bin/model/dae/c1004/mws/c1004.dae";// argv[1];
	//DexSkinMesh* pSkinMesh = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel(resModelFile, 1);
	DexStreamFile::sCreateStreamFile();
	DexMd5Generator::sCreateDexMd5Generator();
	if (argc == 1)
	{//click exe to run
		DexMem mem;
		mem.IniFromFile("bin/ConvertList.txt");
		char lineData[2048];
		while (!mem.End())
		{
			mem.ReadLine(lineData);
			
			DVector<DString> outStr;
			SplitStr(DString(lineData), ' ', outStr);
			const char* resModelFile = outStr[0].c_str();
			int flag = dexatoi(outStr[1].c_str());
			DVector<DString> outStr2;
			SplitStr(DString(resModelFile), '.', outStr2);
			outStr2[0].resize(128);
			const char* outModelFile = dexstrcat((char*)outStr2[0].c_str(), ".dexmodel");
			
			DexSkinMesh* pSkinMesh = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel(resModelFile, flag);
			if (pSkinMesh == DexNull)
				cout << "error: can not find model file: " << resModelFile << endl;
			else
			{
				cout << "converting..." << endl;
				//set render order info
				char RenderOrderInfoFile[FILENAME_MAX_LENGTH];
				char* ptr1 = (char*)resModelFile;
				char* ptr2 = RenderOrderInfoFile;
				while (*ptr1 != '.')
				{
					*ptr2 = *ptr1;
					ptr1++; ptr2++;
				}
				strcpy(ptr2, "_RenderOrder.txt");
				DexMem mem;
				bool orderFileExist = mem.IniFromFile(RenderOrderInfoFile);
				if (orderFileExist)
				{
					cout << "reset render order" << endl;
					char lineInfo[128];
					mem.ReadLine(lineInfo);
					DVector<DString> tempVectorString;
					SplitStr(lineInfo, ',', tempVectorString);
					DVector<DInt32> tempVectorInt;
					for (size_t i = 0; i < tempVectorString.size(); ++i)
						tempVectorInt.push_back(DexCommonFunction::str_to_int(tempVectorString[i]));
					pSkinMesh->SetOrderInfo(tempVectorInt);
				}
				DexGameEngine::getEngine()->SaveModel(pSkinMesh, outModelFile, flag);
				cout << "convert complete ! model path:" << outModelFile << endl;
				delete pSkinMesh;
			}
			
		}
	}
	if (argc == 3)
	{//exe by command line
		//argv[0]: exe file
		//argv[1]: model file path:like D:/Dex/ParticleEditer/bin/model/dae/c1004/mws/c1004.dae
		//argv[2]: flag
		//for example : MeshConvert F:\desktop\游戏编程\Github_Dex\Dex\ParticleEditer\bin\model\dae\monster\m1002\m1002.xml 1
		cout << "argc:0 " << argv[0] << endl;
		cout << "argc:1 " << argv[1] << endl;
		cout << "argc:2 " << argv[2] << endl;
		char* resModelFile = argv[1];
		DVector<DString> outStr;
		SplitStr(argv[1], '.', outStr);
		outStr[0].reserve(100);
		char* outModelFile = dexstrcat((char*)outStr[0].c_str(), ".dexmodel");
		int flag =  dexatoi(argv[2]);
		cout << "parsing model..." << endl;
		DexSkinMesh* pSkinMesh = (DexSkinMesh*)DexGameEngine::getEngine()->CreateModel(resModelFile, flag);
		if (pSkinMesh == DexNull)
			cout << "error: can not find model file: " << resModelFile << endl;
		else
		{
			cout << "converting..." << endl;
			//set render order info
			char RenderOrderInfoFile[FILENAME_MAX_LENGTH];
			char* ptr1 = resModelFile;
			char* ptr2 = RenderOrderInfoFile;
			while (*ptr1 != '.')
			{
				*ptr2 = *ptr1;
				ptr1++; ptr2++;
			}
			strcpy(ptr2, "_RenderOrder.txt");
			DexMem mem;
			bool orderFileExist = mem.IniFromFile(RenderOrderInfoFile);
			if (orderFileExist)
			{
				cout << "reset render order" << endl;
				char lineInfo[128];
				mem.ReadLine(lineInfo);
				DVector<DString> tempVectorString;
				SplitStr(lineInfo, ',', tempVectorString);
				DVector<DInt32> tempVectorInt;
				for (size_t i = 0; i < tempVectorString.size(); ++i)
					tempVectorInt.push_back(DexCommonFunction::str_to_int(tempVectorString[i]));
				pSkinMesh->SetOrderInfo(tempVectorInt);
			}
			DexGameEngine::getEngine()->SaveModel(pSkinMesh, outModelFile, flag);
			cout << "convert complete ! model path:" << outModelFile << endl;
			delete pSkinMesh;
		}
	}
	DexGameEngine::getEngine()->ShutDownEngine();
	return 0;
}

