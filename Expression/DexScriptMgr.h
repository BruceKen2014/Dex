

#ifndef _DEX_SCIPTMGR_H
#define _DEX_SCIPTMGR_H

#include <vector>
#include <string>

using namespace std;
//脚本管理器
class CDexScriptMgr
{
protected:
	vector<string>  m_scriptVec; //脚本字符串
	size_t          m_iScriptIndex; //当前执行到了第几行
	int m_iWaitTime;   //脚本暂停时间 ms
	bool m_bGoing;     //是否正在执行 脚本提供暂停函数 同时提供继续函数	也可有主程序调用继续函数

public:
	CDexScriptMgr();
	~CDexScriptMgr();

public:
	bool   LoadScriptFile(const char* filename); //加载脚本文件
	void   Run(int delta);  //管理器运行

	void   AddCommand(string str);    //程序硬性添加命令 用于测试
	void   ExeCommand(string command);
	void   setWaitTime(int wait_time);
	int    getWaitTime();
	void   Pause();
	void   KeepGoing();
};

#endif