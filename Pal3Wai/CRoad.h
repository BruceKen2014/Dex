

/*
���ߣ�������
2012.12
��ɫ����ȥ�������������·�������
*/

#pragma  once

#include "CSoundObject.h"
 

/*����ʾ��
   1				  3
   |------------------|
   |				  |length
   |				  |
   |------------------| 2
   0(pos)  width
*/
//�����������Ӱ��UV
enum
{
	RD_EW, //����
	RD_SN, //�ϱ�
};
#define  CROAD_VERTEX   4
#define  TITLE_X        1  //��������ƽ������
#define  TITLE_Y        1  //��������ƽ������
class CRoad: public CSoundObject
{
protected:
	float m_width, m_length;   //��·������ĳ���
	float m_tileX, m_tileY;    //��������ƽ������
	int  m_dir;               //RD_EW �ϱ����� RD_SN ��������
public:
	CRoad(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, float width, float length, int id, int dir, float tileX, float tileY);
	~CRoad();

protected: 
	virtual bool  CreateVB();  //��������

public:
	virtual void  ShutDown();
	virtual void  Update();
	virtual void  Render();
	virtual bool  IsOn(D3DXVECTOR3 pos);
	virtual float GetHeight(D3DXVECTOR3 pos) ;
};