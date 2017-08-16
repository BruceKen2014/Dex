/*
DexEngine:单例类
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
SINGLETON_DECLARE 与 SINGLETON_IMPLEMENT成对使用
一般情况下，如果一个单例类A没有子类，那么声明时使用SINGLETON_DECLARE(A),实现时使用SINGLETON_IMPLEMENT(A,A)即可
如果单例类A有子类，比如A=platform B=platformWin,那么就需要对单例进行继承，如下所示:
SINGLETON_INHERIT只需要在单例的子类中添加，并且单独出现，不需要添加SINGLETON_IMPLEMENT进行实现，因为单例的控制由父类控制，和子类无关。
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