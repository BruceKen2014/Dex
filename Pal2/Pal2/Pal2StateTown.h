
#ifndef _PAL2GAMESTATE_TOWN_H
#define _PAL2GAMESTATE_TOWN_H
#include "state/DexGameState.h"
#include <vector>
class CModel;
class CModelAlpha;
class CModelXAni;
class CCollideSphere;
class CCollideBox;
class CSoundObject;
class CParticalSystem;
class DexGameStateTown: public DexGameState
{
public:
	DexGameStateTown(int id);
	virtual ~DexGameStateTown();
public:
	std::vector<CModel*>  g_modelVector;   //���������е�ģ������
	std::vector<CModelAlpha*>  g_alphaModelVector;   //����alpha���Ե�ģ��(�������ӵ�)��Z���򽫸�����Щ������������ľ����������
	std::vector<CModelXAni*>  g_aniModelVector;	    //���ж�̬���Ե�����
	std::vector<CCollideSphere*>  g_collideSphere; //�����ײ��
	std::vector<CCollideBox*>  g_collideBox; //�����ײ������
	std::vector<CSoundObject*> g_sndObject;  //��������(���桢��·���š�����)
	std::vector<CParticalSystem*> g_partical;  //����ϵͳ����ѩ������
public:		  
	//virtual void OnEvent(stEvent event);
	virtual bool ApplyRes() ;     //�л�����stateǰ������Դ
	virtual void ReleaseRes();   //�ͷ���Դ

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