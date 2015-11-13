

#ifndef _DEX_SCIPTMGR_H
#define _DEX_SCIPTMGR_H

#include <vector>
#include <string>

using namespace std;
//�ű�������
class CDexScriptMgr
{
protected:
	vector<string>  m_scriptVec; //�ű��ַ���
	size_t          m_iScriptIndex; //��ǰִ�е��˵ڼ���
	int m_iWaitTime;   //�ű���ͣʱ�� ms
	bool m_bGoing;     //�Ƿ�����ִ�� �ű��ṩ��ͣ���� ͬʱ�ṩ��������	Ҳ������������ü�������

public:
	CDexScriptMgr();
	~CDexScriptMgr();

public:
	bool   LoadScriptFile(const char* filename); //���ؽű��ļ�
	void   Run(int delta);  //����������

	void   AddCommand(string str);    //����Ӳ��������� ���ڲ���
	void   ExeCommand(string command);
	void   setWaitTime(int wait_time);
	int    getWaitTime();
	void   Pause();
	void   KeepGoing();
};

#endif