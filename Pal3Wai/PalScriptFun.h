

#pragma once
#include "../Expression/DexExpression.h"



//声明命令表达式
ADDEXPRFUN(MoveCamera)
ADDEXPRFUN(Wait)

//实例化命令变量
#define DECLARE_COMMAND(command)\
	CExprDo_##command * command	 = new CExprDo_##command

extern void InitScriptExpression();