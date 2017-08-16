
#include "PalCommand.h"
#include "PalStateBattleMain.h"
void CExprDo_ShowBackImage::Handle(stExpStackValue value1[], int valueCount, stExpStackValue& retValue)
{
	int show = value1[0].i_value;
	pTestGlobal->SetBackImageVisible(show);
}

void InitPalCommand()
{
	DECLARE_COMMAND(ShowBackImage);
}