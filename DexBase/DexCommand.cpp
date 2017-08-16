

#include "DexCommand.h"
#include "../state/DexGameEngine.h"

void CExprDo_SetClearColor::Handle(stExpStackValue value1[], int valueCount, stExpStackValue& retValue)
{
	float r = value1[0].f_value;
	float g = value1[1].f_value;
	float b = value1[2].f_value;

	DexGameEngine::getEngine()->SetClearColor(r, g, b);

}