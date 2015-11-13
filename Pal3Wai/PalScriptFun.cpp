


#include "PalScriptFun.h"
#include "../Expression/DexScriptMgr.h"
#include "../Source/CCamera.h"



extern CCamera g_camera;
extern CDexScriptMgr* g_pScriptMgr;
void CExprDo_MoveCamera::Handle(stExpStackValue value1[], int valueCount, stExpStackValue& retValue)
{
	float xPos = value1[0].f_value;
	float yPos = value1[1].f_value;
	float zPos = value1[2].f_value;
	float vel  = value1[3].f_value;

	g_camera.EnableMove(true);
	g_camera.SetMoveVel(vel);
	g_camera.SetDestPos(xPos, yPos, zPos);
}
void CExprDo_Wait::Handle(stExpStackValue value1[], int valueCount, stExpStackValue& retValue)
{
	DEX_ENSURE(g_pScriptMgr != NULL);
	g_pScriptMgr->setWaitTime(value1[0].i_value);
}


void InitScriptExpression()
{
	DECLARE_COMMAND(MoveCamera);
	DECLARE_COMMAND(Wait);
}