

#pragma once
#include "../Expression/DexExpression.h"



//����������ʽ
ADDEXPRFUN(MoveCamera)
ADDEXPRFUN(Wait)

//ʵ�����������
#define DECLARE_COMMAND(command)\
	CExprDo_##command * command	 = new CExprDo_##command

extern void InitScriptExpression();