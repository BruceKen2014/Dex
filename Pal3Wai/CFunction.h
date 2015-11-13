/*************************************************************
ע�����ļ�������֮ǰд��2D����Demo����
*************************************************************/

#pragma once



/*===================================================================
============================���ù��ܺ�������=========================
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


//��Ϣ��ʾ
//void  _Message(char * message, ...);

//���ϵͳʱ��
void  _GetTime(_TIME * t);

//-----------------ģ�庯��ֱ����.h�ļ���ʵ��---------------------
//�ͷ�һ���ڴ�ָ��
template <typename T>
inline void _FreeP(T *t)
{
	if(t)
	{
		free(t);
		t = NULL;
	}
};
//�ͷŶ���ָ��
template <typename T>
inline void _deleteP(T *t)
{
	if(t)
	{
		delete t;
		t = NULL;
	}
}
//�ͷŶ�������ָ��
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
//�ͷ�Object
template <typename T>
inline void _FreeObject(T t)
{
	if(t != NULL)
	{
		DeleteObject(t);
		t = NULL;
	}
};

