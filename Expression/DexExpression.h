
#ifndef _DEX_EXPRESSION_H
#define _DEX_EXPRESSION_H

#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

/*
�������ʽ
*/

#define MAX_EXPR_STRING 256

//* / % ��ͬ�����ȼ�
enum EExprOp
{
	OP_ADD     = '+',	    // +
	OP_MINUS   = '-',		// -
	OP_MUL     = '*',       // *
	OP_DIVIDE  = '/',	    // /
	OP_LEFT_B  = '(',		// (   
	OP_RIGHT_B = ')',		// )  
	OP_BIT_AND = '&',       // &
	OP_BIT_OR  = '|',       // |
	OP_COMMA   = ',',       // ,  ����
	OP_MO      = '%',
	OP_AND,                 // &&
	OP_OR,                  // ||
	
};
enum EExprType
{
	Expr_None,
	Expr_OP, 
	Expr_Char,
	Expr_Bool,	 
	Expr_Int,  
	Expr_Float,
	Expr_String,
	Expr_Fun,
};
struct stExpStackValue
{
	EExprType type;
	union
	{
		EExprOp op_value;
		char    c_value;
		bool    b_value;
		int     i_value;
		float   f_value;
		char    s_value[64];
		char    fun_value[64];
	};
	stExpStackValue operator + (const stExpStackValue& val) const
	{
		stExpStackValue ret = *this;
		switch(type)
		{
		case Expr_Bool:
			{
				ret.b_value = ret.b_value || val.b_value;
				break;
			}
		case Expr_Char:
			{
				ret.c_value += val.c_value;
				break;
			}
		case Expr_Int:
			{
				ret.i_value += val.i_value;
				break;
			}
		case Expr_Float:
			{
				ret.f_value += val.f_value;  
				break;
			}
		case Expr_String:
			{
				if(strlen(ret.s_value) + strlen(val.s_value) < MAX_EXPR_STRING)
				{
					strcat(ret.s_value, val.s_value);	
				}	
				else
				{
					//DexLog::getSingleton()->BeginLog();
					//DexLog::getSingleton()->Log(log_allert, "�ַ������ȹ���\n");
					//DexLog::getSingleton()->EndLog();
				}
				break;
			}
		} 
		return ret;
	}
	stExpStackValue operator - (const stExpStackValue& val) const
	{
		stExpStackValue ret = *this;
		switch(type)
		{
		case Expr_Bool:
			{
				//DexLog::getSingleton()->BeginLog();
				//DexLog::getSingleton()->Log(log_allert, "��������bool�޷�������\n");
				//DexLog::getSingleton()->EndLog();
				break;
			}
		case Expr_Char:
			{
				ret.c_value -= val.c_value;
				break;
			}
		case Expr_Int:
			{
				ret.i_value -= val.i_value;
				break;
			}
		case Expr_Float:
			{
				ret.f_value -= val.f_value;  
				break;
			}
		case Expr_String:
			{
				//DexLog::getSingleton()->BeginLog();
				//DexLog::getSingleton()->Log(log_allert, "��������string�޷���������\n");
				//DexLog::getSingleton()->EndLog();
				break;
			}
		} 
		return ret;
	}
	stExpStackValue operator * (const stExpStackValue& val) const
	{
		stExpStackValue ret = *this;
		switch(type)
		{
		case Expr_Bool:
			{
				//DexLog::getSingleton()->BeginLog();
				//DexLog::getSingleton()->Log(log_allert, "�ٲ�������bool�޷���ˣ���\n");
				//DexLog::getSingleton()->EndLog();
				break;
			}
		case Expr_Char:
			{
				//DexLog::getSingleton()->BeginLog();
				//DexLog::getSingleton()->Log(log_allert, "��������char�޷���ˣ�\n");
				//DexLog::getSingleton()->EndLog();
				break;
			}
		case Expr_Int:
			{
				ret.i_value *= val.i_value;
				break;
			}
		case Expr_Float:
			{
				ret.f_value *= val.f_value;  
				break;
			}
		case Expr_String:
			{
				//DexLog::getSingleton()->BeginLog();
				//DexLog::getSingleton()->Log(log_allert, "��������string�޷���ˣ�\n");
				//DexLog::getSingleton()->EndLog();
				break;
			}
		} 
		return ret;
	}
	stExpStackValue operator / (const stExpStackValue& val) const
	{
		stExpStackValue ret = *this;
		switch(type)
		{
		case Expr_Bool:
			{
				//cout<<"��������bool�޷���������"<<endl;
				break;
			}
		case Expr_Char:
			{
				//cout<<"��������char�޷���������"<<endl;
				break;
			}
		case Expr_Int:
			{
				ret.i_value /= val.i_value;
				break;
			}
		case Expr_Float:
			{
				ret.f_value /= val.f_value;  
				break;
			}
		case Expr_String:
			{
				//cout<<"��������string�޷���������"<<endl;
				break;
			}
		} 
		return ret;
	}
	stExpStackValue operator & (const stExpStackValue& val) const
	{
		stExpStackValue ret = *this;
		switch(type)
		{
		case Expr_Bool:
			{
				ret.b_value &= val.b_value;
				break;
			}
		case Expr_Char:
			{
				//cout<<"��������char�޷�����λ�룡"<<endl;
				break;
			}
		case Expr_Int:
			{
				ret.i_value &= val.i_value;
				break;
			}
		case Expr_Float:
			{
				//cout<<"��������float�޷�����λ�룡"<<endl; 
				break;
			}
		case Expr_String:
			{
				//cout<<"��������string�޷�����λ�룡"<<endl; 
				break;
			}
		} 
		return ret;
	}
	stExpStackValue operator | (const stExpStackValue& val) const
	{
		stExpStackValue ret = *this;
		switch(type)
		{
		case Expr_Bool:
			{
				ret.b_value |= val.b_value;
				break;
			}
		case Expr_Char:
			{
				//cout<<"��������char�޷�����λ��"<<endl;
				break;
			}
		case Expr_Int:
			{
				ret.i_value |= val.i_value;
				break;
			}
		case Expr_Float:
			{
				//cout<<"��������float�޷�����λ��"<<endl; 
				break;
			}
		case Expr_String:
			{
				//cout<<"��������string�޷�����λ��"<<endl; 
				break;
			}
		} 
		return ret;
	}

	stExpStackValue operator % (const stExpStackValue& val) const
	{
		stExpStackValue ret = *this;
		switch(type)
		{
		case Expr_Bool:
			{
				//cout<<"��������bool�޷���ȡģ������"<<endl;
				break;
			}
		case Expr_Char:
			{
				//cout<<"��������char�޷���ȡģ������"<<endl;
				break;
			}
		case Expr_Int:
			{
				ret.i_value %= val.i_value;
				break;
			}
		case Expr_Float:
			{
				//cout<<"��������float�޷���ȡģ������"<<endl; 
				break;
			}
		case Expr_String:
			{
				//cout<<"��������string�޷���ȡģ������"<<endl; 
				break;
			}
		} 
		return ret;
	}
};

extern bool IsDoubleCharOpType(char* c);
extern bool IsSingleCharOpType(char c);
extern bool IsNumber( char c);
extern bool IsFloat(char* number_str);
extern bool Bigger(EExprOp op1, EExprOp op2); //op1�Ƿ��op2���ȼ���
//�ַ����ָ�Ϊ��׺���ʽ
extern void StrToExpr(const char* str, vector<stExpStackValue>& value_stack);
//��׺ת��Ϊ��׺���ʽ
extern void MiddleToHou(const vector<stExpStackValue>& middle, vector<stExpStackValue>& hou);
extern stExpStackValue StackValueOp(EExprOp op, stExpStackValue first_value, stExpStackValue second_value);
  
class CExprDo
{
public:
	CExprDo(){};
	virtual ~CExprDo(){}; 
	virtual void Handle(stExpStackValue value1[], int valueCount, stExpStackValue& retValue) = 0;
};
extern map<string, CExprDo*> g_ExprMap;

#define ADDEXPRFUN(name)	class CExprDo_##name :public CExprDo \
{						\
public:		\
	CExprDo_##name() { string m_name = #name; g_ExprMap[m_name] = this ;}; \
	virtual void Handle(stExpStackValue value1[], int valueCount, stExpStackValue& retValue) ;	 \
};	

//�����׺���ʽ��ֵ
extern void ExcuteExp(const char* str);


#endif