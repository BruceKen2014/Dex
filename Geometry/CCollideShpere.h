

/*************************************************************
****Dex����ռ���ײ��
****���ߣ�������(BruceKen)
****���ڣ�2012-12-25
*************************************************************/
#pragma  once

#include "CBall.h"


/*Ϊ�˵�����Щ����Ĳ����������ײ���������,ȡ����Щ����
  �μ���ײ��⣬Ȼ���½�һ���ʺϵ�collide��������������ײ���
  */
class CCollideSphere:public stBall
{
protected:
	LPDIRECT3DDEVICE9 m_device;
#ifdef DEX_DEBGU   //����ģʽ����Ҫ��ʾ��ײ��,�������ĳ��򲢲���Ҫ������ʱȡ��DEX_DEBGU�꣬������û����m_mesh��Ҳ�ܽ�ʡ�ڴ�
	               //ǰ���Ǳ������еط���û��m_mesh�������ˣ��������ִ���
	LPD3DXMESH m_mesh;
#endif
	bool       m_render;

public:
	CCollideSphere(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 center, float radius, bool render);
	~CCollideSphere();

public:
	virtual void Shutdown();
	virtual void Render();

	virtual void SetRender(bool render);
	virtual bool GetRender();
};