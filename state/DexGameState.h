
#ifndef _DEXGAMESTATE_H
#define _DEXGAMESTATE_H

#include "../DexBase/CDexObject.h"
class DexGlobal
{

};
class CDexScene;
class DexGameState: public CDexObject
{
	Dex_DeclareClass(DexGameState,1)
protected:
    /*��state��Ӧ����Դ�Ƿ��Ѿ����룬���л�state��ʱ��Ҫ��state��Դ�ͷŵ����ô˱�����־�Ƿ���������Դ,
	*/
	bool m_bApply; 
	CDexScene* m_pScene;
public:
	DexGameState();
	virtual ~DexGameState();
public:		  
	//virtual void OnEvent(stEvent event);
	virtual bool ApplyRes() { return true;};     //�л�����stateǰ������Դ
	virtual void ReleaseRes()  {};   //�ͷ���Դ
	
	virtual bool Update(int delta) = 0;
	virtual void BeginRender(){};
	virtual void Render() = 0;
	virtual void EndRender() {};
	virtual void OnEnterState() {};
	virtual void OnLeaveState() {};
	virtual void MouseWheelFront() ;
	virtual void MouseWheelBack();
	virtual void  MouseMove(int xPos, int yPos);
	virtual void  MouseLDown(int xPos, int yPos);
	virtual void  MouseLUp(int xPos, int yPos);
	virtual void  MouseRDown(int xPos, int yPos);
	virtual void  MouseRUp(int xPos, int yPos);
	virtual void  KeyUp();
	virtual void  KeyDown();
	CDexScene* getStateScene(){ return m_pScene;} ;
	void SetApply(bool apply) { m_bApply = apply;}
	bool NeedApply()		  { return !m_bApply;}
};

class DexGameStateLoading: public DexGameState
{
	Dex_DeclareClass(DexGameStateLoading,1)
public:
	DexGameStateLoading();
	virtual ~DexGameStateLoading();
public:		  
	virtual bool ApplyRes();     //�л�����stateǰ������Դ
	virtual void ReleaseRes();   //�ͷ���Դ

	virtual bool Update(int delta);
	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();
};

#endif