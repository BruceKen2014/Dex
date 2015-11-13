
#ifndef _PAL2GAMESTATE_BATTLE_H
#define _PAL2GAMESTATE_BATTLE_H

#include "state/DexGameState.h"
#include "PalPlayer.h"

class DexGameStateBattle: public DexGameState
{
public:
	DexGameStateBattle(int id);
	virtual ~DexGameStateBattle();




public:		  
	//virtual void OnEvent(stEvent event);
	virtual bool ApplyRes() ;  
	virtual void ReleaseRes();  

	virtual void Update(int delta);
	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();
	virtual void  MouseWheelFront();
	virtual void  MouseWheelBack();
	virtual void  MouseMove(int xPos, int yPos);
	virtual void  MouseLDown(int xPos, int yPos);
	virtual void  MouseLUp(int xPos, int yPos);
	virtual void  MouseRUp(int xPos, int yPos);
	virtual void  KeyUp();
	virtual void  KeyDown();

	void SetApply(bool apply) { m_bApply = apply;}
	bool NeedApply()		  { return m_bApply;}
	int  GetStateId()		  { return m_id;};
};

#endif