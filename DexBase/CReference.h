
/*
Dex�������ü����ļ�
���ߣ�BruceKen
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
	virtual void ShutDown();   //�����������ã�ֻ�����������ͷ��Լ�ʱ����
public:	
	void AddRef();
	unsigned int GetRefCount() { return m_count;}; //�������˶��ٴ�
	void ReduceRef();
};

template<typename T> //�����T������CRefCount��������

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