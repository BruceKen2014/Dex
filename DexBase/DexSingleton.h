/*
DexEngine:������
*/
#ifndef _DEX_SINGLETON_H
#define _DEX_SINGLETON_H

#define SINGLETON_DECLARE(cls)\
public:\
	static cls* getSingleton();\
protected:\
	cls(); \
	virtual ~cls();\
	cls(const cls& value) = delete; \
	cls& operator = (const cls& value) = delete; 

#define SINGLETON_IMPLEMENT(cls,realCls)\
	cls* cls::getSingleton()\
{\
	static cls* instance = new realCls(); \
	return instance; \
}
#define SINGLETON_INHERIT(cls,realCls)\
	friend class realCls;\
protected:\
    cls(); \
    virtual ~cls();\
	cls(const cls& value) = delete; \
	cls& operator = (const cls& value) = delete; 


/*
SINGLETON_DECLARE �� SINGLETON_IMPLEMENT�ɶ�ʹ��
һ������£����һ��������Aû�����࣬��ô����ʱʹ��SINGLETON_DECLARE(A),ʵ��ʱʹ��SINGLETON_IMPLEMENT(A,A)����
���������A�����࣬����A=platform B=platformWin,��ô����Ҫ�Ե������м̳У�������ʾ:
SINGLETON_INHERITֻ��Ҫ�ڵ�������������ӣ����ҵ������֣�����Ҫ���SINGLETON_IMPLEMENT����ʵ�֣���Ϊ�����Ŀ����ɸ�����ƣ��������޹ء�
//A.h
class A
{SINGLETON_DECLARE(A)};
//A.cpp
SINGLETON_IMPLEMENT(A,B)

//B.h
class B:public A
{SINGLETON_INHERIT(B,A)};
*/
#endif