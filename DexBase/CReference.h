
/*
Dex引擎引用计数文件
作者：BruceKen
*/

#pragma  once

#define _ADDREF(p) if((p) != NULL)  p->AddRef()

#define _REDUCEREF(p) if((p) != NULL)	   \
				   {				   \
				   (p)->ReduceRef(); \
				   if((p)->GetRefCount() == 0)\
					   {						\
					   delete (p);			\
					   (p) = NULL; 			\
					   }					    \
				   }

#define Class(c)  class c:public CRefCount

class CRefCount
{
protected:
	unsigned int m_count;

public:
	CRefCount();
	virtual ~CRefCount();

protected:
	virtual void ShutDown();   //不允许外界调用，只能由自身在释放自己时调用
public:	
	void AddRef();
	unsigned int GetRefCount() { return m_count;}; //被引用了多少次
	void ReduceRef();
};

template<typename T> //这里的T必须由CRefCount派生而来

class CPointRef
{
private:
	T* m_member;

public:
	CPointRef(T* _in)
	{
		m_member = _in;
	}
	CPointRef(const CPointRef<T>& _in)
	{
		m_member = _in.m_member;
	}
	~CPointRef()
	{
		m_member->ReduceRef();
	}
public:
	T* Get() {   return m_member;};
	T* operator->()const{    return m_member;    }
	CPointRef<T>& operator=(T* _in)
	{
		if(_in != m_member)
		{
			if(m_member)
				m_member->ReduceRef();
		    m_member = _in;
			m_member->AddRef();
		}
		return *this;
	}
	CPointRef<T>& operator=(const CPointRef<T>& _in)
	{
		if(_in != this && m_member != _in.m_member)
		{
			if(m_member)
				m_member->ReduceRef();
			m_member = _in;
			m_member->AddRef();
		}
		return *this;
	}
};