
#pragma  once

#include "CSoundObject.h"

/*
���ߣ�������
2012.12
��ɫ����ȥ��������������ӣ���Ҫ�����߶ȼ���ã�����ֱ��������Ⱦ
*/

/*
�������¶˵��󶥵���Ϊ֧��,Ҳ�����������ӳ����淽�����¶˶����Ϊpos���ڵ�
*/
#define  LADDER_COUNT   4
#define  L_TITLE_X        10  //��������ƽ������
#define  L_TITLE_Y        10  //��������ƽ������
class CLadder:public CSoundObject
{
protected:
	float m_width, m_length, m_height;   //�����
	int   m_dir;  //���ӳ���
	stVertex1 m_point[4];  //�����ĸ����㣬road���������Ա��Ϊ�߶ȼ��Ҫ�򵥵ö࣬��������ҪƵ��ʹ���ĸ��������߶ȼ��
public:
	CLadder(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, float width, float length, float height, int dir, int id);
	~CLadder();

protected: 
	virtual bool  CreateVB();  //��������

public:
	virtual void  ShutDown();
	virtual void  Update();
	virtual void  Render();
	virtual bool  IsOn(D3DXVECTOR3 pos);
	virtual float GetHeight(D3DXVECTOR3 pos);   //ǰ�����ж��Ƿ��������ϣ���������Զ�ڱ𴦣����߶�����������ʱ��ͬ
};