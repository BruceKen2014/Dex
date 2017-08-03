
#ifndef _DEX_EXPRESSION_H
#define _DEX_EXPRESSION_H

#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

/*
解析表达式
*/

#define MAX_EXPR_STRING 256

//* / % 在同级优先级
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
	OP_COMMA   = ',',       // ,  逗号
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
					//DexLog::getSingleton()->Log(log_allert, "字符串长度过长\n");
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
				//DexLog::getSingleton()->Log(log_allert, "操作错误！bool无法互减！\n");
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
				//DexLog::getSingleton()->Log(log_allert, "操作错误！string无法互减！！\n");
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
				//DexLog::getSingleton()->Log(log_allert, "操操作错误！bool无法相乘！！\n");
				//DexLog::getSingleton()->EndLog();
				break;
			}
		case Expr_Char:
			{
				//DexLog::getSingleton()->BeginLog();
				//DexLog::getSingleton()->Log(log_allert, "操作错误！char无法相乘！\n");
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
				//DexLog::getSingleton()->Log(log_allert, "操作错误！string无法相乘！\n");
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
				//cout<<"操作错误！bool无法作除法！"<<endl;
				break;
			}
		case Expr_Char:
			{
				//cout<<"操作错误！char无法作除法！"<<endl;
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
				//cout<<"操作错误！string无法作除法！"<<endl;
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
				//cout<<"操作错误！char无法作按位与！"<<endl;
				break;
			}
		case Expr_Int:
			{
				ret.i_value &= val.i_value;
				break;
			}
		case Expr_Float:
			{
				//cout<<"操作错误！float无法作按位与！"<<endl; 
				break;
			}
		case Expr_String:
			{
				//cout<<"操作错误！string无法作按位与！"<<endl; 
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
				//cout<<"操作错误！char无法作按位或！"<<endl;
				break;
			}
		case Expr_Int:
			{
				ret.i_value |= val.i_value;
				break;
			}
		case Expr_Float:
			{
				//cout<<"操作错误！float无法作按位或！"<<endl; 
				break;
			}
		case Expr_String:
			{
				//cout<<"操作错误！string无法作按位或！"<<endl; 
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
				//cout<<"操作错误！bool无法作取模操作！"<<endl;
				break;
			}
		case Expr_Char:
			{
				//cout<<"操作错误！char无法作取模操作！"<<endl;
				break;
			}
		case Expr_Int:
			{
				ret.i_value %= val.i_value;
				break;
			}
		case Expr_Float:
			{
				//cout<<"操作错误！float无法作取模操作！"<<endl; 
				break;
			}
		case Expr_String:
			{
				//cout<<"操作错误！string无法作取模操作！"<<endl; 
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
extern bool Bigger(EExprOp op1, EExprOp op2); //op1是否比op2优先级高
//字符串分割为中缀表达式
extern void StrToExpr(const char* str, vector<stExpStackValue>& value_stack);
//中缀转换为后缀表达式
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

//计算后缀表达式的值
extern void ExcuteExp(const char* str);


#endif