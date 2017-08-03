



#ifndef _DEX_INI_H
#define _DEX_INI_H

#include "DexType.h"
#include "DexString.h"
#include "DexDVector.h"
#include "DexDMap.h"
class DexIni
{
public:
	DexIni();
	~DexIni();
public:
	//read ini file
	bool readIniFile(DString filename);
	DString getString(DString field, DString key) const;
	DInt32   getInt(DString field, DString key) const;
	DFloat32 getFloat(DString field, DString key) const;
	void clearData();

	//write ini file
	void writeData(DString field, DString key, DString value);
	void writeData(DString field, DString key, DInt32 value);
	void writeData(DString field, DString key, DFloat32 value);
	void saveFile(DString filename);
private:
	typedef struct _stIniFieldData
	{
		DString    name;
		DMapStrStr data;
		void AddData(DString key, DString value);
	}stIniFieldData;
	stIniFieldData* _findField(DString field);
	stIniFieldData* _addField(DString field);
private:

	typedef DMap<DString, stIniFieldData> InitMap;
	InitMap m_data;
	stIniFieldData* m_pCurrField;
};
#endif