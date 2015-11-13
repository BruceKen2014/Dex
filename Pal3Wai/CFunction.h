/*************************************************************
注：本文件来自于之前写的2D引擎Demo引擎
*************************************************************/

#pragma once



/*===================================================================
============================公用功能函数定义=========================
====================================================================*/
//template <typename T>
//struct _Node
//{
//	T data;
//	_Node * next;
//
//	_Node(){ data = NULL; next = NULL;};
//};
typedef  struct
{
	int   m_year;
	short m_month;
	short m_day;
	short m_hour;
	short m_min;
	short m_second;
	short m_week;
} _TIME;


//消息提示
//void  _Message(char * message, ...);

//获得系统时间
void  _GetTime(_TIME * t);

//-----------------模板函数直接在.h文件中实现---------------------
//释放一般内存指针
template <typename T>
inline void _FreeP(T *t)
{
	if(t)
	{
		free(t);
		t = NULL;
	}
};
//释放对象指针
template <typename T>
inline void _deleteP(T *t)
{
	if(t)
	{
		delete t;
		t = NULL;
	}
}
//释放对象数组指针
template <typename T>
inline void _FreeArray(T *t)
{
	if(t != NULL)
	{
		delete[] t;
		t = NULL;
	}
}
//----------------------------------------------------------------
//释放Object
template <typename T>
inline void _FreeObject(T t)
{
	if(t != NULL)
	{
		DeleteObject(t);
		t = NULL;
	}
};

