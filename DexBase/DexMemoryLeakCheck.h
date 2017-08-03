
#ifndef _DEX_MEMORYLEAKCHECK_H
#define _DEX_MEMORYLEAKCHECK_H

#ifdef _DEBUG
#	define _CRTDBG_MAP_ALLOC //为了能够捕捉malloc的泄露，定义这个宏以后malloc会成为_malloc_dbg
#	define DEBUG_CLIENTBLOCK new(_CLIENT_BLOCK, __FILE__, __LINE__)
#	define dexnew DEBUG_CLIENTBLOCK
#include <crtdbg.h>
#endif
/*
测试一段代码是否有内存泄露
DexCheckMemLeak::getDexMemoryLeakCheck()->BeginCheck();
allocate code
free code
DexCheckMemLeak::getDexMemoryLeakCheck()->EndCheck();
如果有的话，output将会输出一些信息

中间的申请释放代码最好是小的单元，如果模块中有大量的内存操作，目前不能输出泄露的所在文件行数，定位比较麻烦
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
		_CrtMemCheckpoint(&enterpoint);//记录入口内存情况
	}
	bool EndCheck()
	{
		_CrtMemState exitpoint;
		_CrtMemCheckpoint(&exitpoint);//记录退出内存情况
		_CrtMemState diff;
		if (_CrtMemDifference(&diff, &enterpoint, &exitpoint))
		{//如果两块内存不一样，则打印
			_CrtMemDumpStatistics(&diff);
			return true;
		}
		return false;
	}
	//allocateIndex是在内存泄露发生时第多少次申请内存，程序会在此断点
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