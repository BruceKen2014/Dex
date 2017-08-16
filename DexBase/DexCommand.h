
#ifndef _DEX_COMMAND_H
#define _DEX_COMMAND_H

#include "../Expression/DexExpression.h"
/*
按F1调出命令行
引擎添加一个新的命令步骤：
1 ADDEXPRFUN(command)
2 在cpp中实现
3 在DexGameEngine的InitCommand初始化一下
4 执行命令
例：调出命令行后，输入SetClearColor(1.0,0.0,0.0)
*/

//声明命令表达式
ADDEXPRFUN(SetClearColor)

#endif