
#include "DexIni.h"
#include "DexMem.h"
#include "DexCommonFunction.h"
#include "DexLog.h"
#include "DexDefine.h"


void DexIni::stIniFieldData::AddData(DString key, DString value)
{
	//DEX_ENSURE(data.find(key) == data.end()); insert会进行存在检查，这句是不必要的
	data.insert(make_pair(key, value));
}
DexIni::DexIni()
{
	m_pCurrField = DexNull;
}

DexIni::~DexIni()
{
	clearData();
}

bool DexIni::readIniFile(DString filename)
{
	DexMem mem;
	DEX_ENSURE_B(mem.IniFromFile(filename.c_str()));
	char lineData[1024];
	stIniFieldData* pFieldData = DexNull;
	DUInt32 lineIndex = 0;
	while (!mem.End())
	{
		++lineIndex;
		mem.ReadLine(lineData);
		auto firstCharIndex = 0;
		if (dexstrlen(lineData) == 0)
			//空行
			continue;
		DexCommonFunction::DeleteStrSpace(lineData);
		if (lineData[firstCharIndex] == '/')
			//注释行
			continue;
		if (lineData[firstCharIndex] == '[')
		{
			++firstCharIndex;
			char* flag = dexstrchr(&lineData[firstCharIndex], ']');
			*flag = '\0';
			DString field = DString(&lineData[firstCharIndex]);
			DexCommonFunction::DeleteStrSpace(field);
			InitMap::iterator ite = m_data.find(field);
			if (ite != m_data.end())
			{//已有field，目前先删除，也就是保留最后一个field
				pFieldData = &ite->second;
				pFieldData->data.clear();
			}
			else
				pFieldData = _addField(field);
		}
		else
		{//读field的数据
			if (pFieldData == DexNull)
			{//没有field的无效数据
				DexLog::getSingleton()->LogLine(log_allert, "invalid ini file, line:%d", lineIndex);
			}
			else
			{
				DVector<DString> splitData;
				DexCommonFunction::SplitStr(lineData, '=', splitData);
				if (splitData.size() != 2)
				{
					DexLog::getSingleton()->LogLine(log_allert, "ini file data format error! line:%d", lineIndex);
				}
				else
					pFieldData->AddData(splitData[0],splitData[1]);
			}
		}
	}
	return true;
}

DString DexIni::getString(DString field, DString key) const
{
	auto ite = m_data.find(field);
	DEX_ENSURE_S(ite != m_data.end());
	auto ite2 = ite->second.data.find(key);
	DEX_ENSURE_S(ite2 != ite->second.data.end());
	return (ite2->second);
}

DInt32 DexIni::getInt(DString field, DString key) const
{
	auto ite = m_data.find(field);
	DEX_ENSURE_I(ite != m_data.end());
	auto ite2 = ite->second.data.find(key);
	DEX_ENSURE_I(ite2 != ite->second.data.end());
	return DexCommonFunction::str_to_int(ite2->second);
}

DFloat32 DexIni::getFloat(DString field, DString key) const
{
	auto ite = m_data.find(field);
	DEX_ENSURE_F(ite != m_data.end());
	auto ite2 = ite->second.data.find(key);
	DEX_ENSURE_F(ite2 != ite->second.data.end());
	return DexCommonFunction::str_to_float(ite2->second);
}

void DexIni::clearData()
{
	for (auto ite : m_data)
		ite.second.data.clear();
	m_data.clear();
}

void DexIni::writeData(DString field, DString key, DString value)
{
	if (m_pCurrField != DexNull && m_pCurrField->name == field)
	{
		m_pCurrField->AddData(key, value);
	}
	else
	{
		m_pCurrField = _findField(field);
		if (m_pCurrField == DexNull)
			m_pCurrField = _addField(field);
		m_pCurrField->AddData(key, value);
	}
}

void DexIni::writeData(DString field, DString key, DInt32 value)
{
	if (m_pCurrField != DexNull && m_pCurrField->name == field)
	{
		m_pCurrField->AddData(key, DexCommonFunction::int_to_str(value));
	}
	else
	{
		m_pCurrField = _findField(field);
		if (m_pCurrField == DexNull)
			m_pCurrField = _addField(field);
		m_pCurrField->AddData(key, DexCommonFunction::int_to_str(value));
	}
}

void DexIni::writeData(DString field, DString key, DFloat32 value)
{
	if (m_pCurrField != DexNull && m_pCurrField->name == field)
	{
		m_pCurrField->AddData(key, DexCommonFunction::float_to_str(value));
	}
	else
	{
		m_pCurrField = _findField(field);
		if (m_pCurrField == DexNull)
			m_pCurrField = _addField(field);
		m_pCurrField->AddData(key, DexCommonFunction::float_to_str(value));
	}
}

void DexIni::saveFile(DString filename)
{
	DexMem mem;
	mem.SetMemoryFlag(true, 1024 * 32, DexNull);
	mem.BeginWrite();
	for (auto ite : m_data)
	{
		mem.WriteLine("[" + ite.first + "]");
		for (auto ite2 : ite.second.data)
			mem.WriteLine("    " + ite2.first + "=" + ite2.second);
	}
	mem.SaveToFile(filename.c_str());
}

DexIni::stIniFieldData* DexIni::_findField(DString field)
{
	InitMap::iterator ite = m_data.find(field);
	DEX_ENSURE_P(ite != m_data.end());
	return &(ite->second);
}

DexIni::stIniFieldData* DexIni::_addField(DString field)
{
	stIniFieldData fieldData;
	fieldData.name = field;
	std::pair<InitMap::iterator, bool> ret = m_data.insert(make_pair(field, fieldData));
	return &(ret.first->second);

}