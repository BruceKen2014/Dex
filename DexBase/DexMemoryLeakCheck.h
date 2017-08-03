
#ifndef _DEX_MEMORYLEAKCHECK_H
#define _DEX_MEMORYLEAKCHECK_H

#ifdef _DEBUG
#	define _CRTDBG_MAP_ALLOC //Ϊ���ܹ���׽malloc��й¶������������Ժ�malloc���Ϊ_malloc_dbg
#	define DEBUG_CLIENTBLOCK new(_CLIENT_BLOCK, __FILE__, __LINE__)
#	define dexnew DEBUG_CLIENTBLOCK
#include <crtdbg.h>
#endif
/*
����һ�δ����Ƿ����ڴ�й¶
DexCheckMemLeak::getDexMemoryLeakCheck()->BeginCheck();
allocate code
free code
DexCheckMemLeak::getDexMemoryLeakCheck()->EndCheck();
����еĻ���output�������һЩ��Ϣ

�м�������ͷŴ��������С�ĵ�Ԫ�����ģ�����д������ڴ������Ŀǰ�������й¶�������ļ���������λ�Ƚ��鷳
*/
class DexCheckMemLeak
{
private:
	_CrtMemState enterpoint;
	DexCheckMemLeak(){};
public:
	static DexCheckMemLeak* getDexMemoryLeakCheck()
	{
		static DexCheckMemLeak* DCML= new DexCheckMemLeak();
		return DCML;
	}
public:

	void BeginCheck()
	{
		_CrtMemCheckpoint(&enterpoint);//��¼����ڴ����
	}
	bool EndCheck()
	{
		_CrtMemState exitpoint;
		_CrtMemCheckpoint(&exitpoint);//��¼�˳��ڴ����
		_CrtMemState diff;
		if (_CrtMemDifference(&diff, &enterpoint, &exitpoint))
		{//��������ڴ治һ�������ӡ
			_CrtMemDumpStatistics(&diff);
			return true;
		}
		return false;
	}
	//allocateIndex�����ڴ�й¶����ʱ�ڶ��ٴ������ڴ棬������ڴ˶ϵ�
	void SetBreakAlloc(int allocateIndex)
	{
		_CrtSetBreakAlloc(allocateIndex);
	}
	void CheckMemoryLeakEnd()
	{
		_CrtDumpMemoryLeaks();
	}
};

#endif