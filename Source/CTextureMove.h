

/*�����ƶ����ʵ�ͼƬ
By Bruce Ken
*/
#pragma  once

#include "../DexBase/typedefine.h"
#include "CTexture.h"


//�ƶ�ͼƬ��alpha��˸����
enum
{
	CTEXTUREMOVE_NONE, //alpha���仯
	CTEXTUREMOVE_ZTH,  //ZERO TO WHOLE  ��ȫ����ȫ��
    CTEXTUREMOVE_HTZ, //whole to zero ��ȫ�ֵ�ȫ��
	CTEXTUREMOVE_SPLASH, //��������
};
class CTextureMove: public CTextureEx
{
protected:
	LPDIRECT3DDEVICE9 m_device;	
	TSHORT  m_startX, m_startY;	 //�ƶ���ʼλ��
	TSHORT  m_endX, m_endY;		 //�ƶ�ĩ��λ��
	float  m_currX, m_currY;
	float   m_speed;    //�ƶ��ٶ�
	bool    m_complete;  //�Ƿ��ƶ�����
	int     m_alphaFlag; //alpha��������

public:
	CTextureMove(LPDIRECT3DDEVICE9 device);
	virtual~CTextureMove();

public:
	virtual void ShutDown();

	virtual void Update();
	virtual void Render();

	virtual void SetStartPos(TSHORT x, TSHORT y);
	virtual void SetEndPos(TSHORT x, TSHORT y);
	virtual void SetSpeed(float speed);
	virtual void SetAlphaFlag(int alphaflag);

	virtual void Reset(); //���õ���ʼ״̬

	virtual bool GetComplete();
};