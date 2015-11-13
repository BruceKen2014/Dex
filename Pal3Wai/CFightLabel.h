


/*ս����������
By BuceKen
*/

#pragma once


#include "../Source/CTexture.h"
#include "CEnemy.h"
//ս��ͷ����
class CFightHead
{
protected:
	LPDIRECT3DDEVICE9  m_device;
	CTextureEx*  m_texture;  //ͷ��
	CEnemy*     m_enmyPoint;  //ָ���ⲿս�����˵�ָ�룬ͨ���ⲿ���룬
	float  m_speed;  //�ٶ�
	TSHORT  m_offsetX, m_offsetY;	  //�ƶ����
	float m_posX, m_posY;  //��ǰλ��
	bool   m_player ;  //�Ƿ�������

public:
	CFightHead(LPDIRECT3DDEVICE9 device);
	virtual ~CFightHead();

public:
	virtual void ShutDown();
	virtual void Update();
	virtual void Render();

	virtual void  SetEnemy(CEnemy* enmy);
	virtual CEnemy* GetEnemy();
	virtual CTextureEx* GetTex();
	virtual void  SetPos(float x, float y);
	virtual void  SetOffset(TSHORT x, TSHORT y);
	virtual void  SetSpeed(float speed);
	virtual void  SetIsPlayer(bool b);  //�Ƿ�������ͷ��
	virtual bool  GetIsPlayer();
	virtual TSHORT GetPosX();  //�õ���ǰXλ��
	virtual TSHORT GetPosY();
	virtual TSHORT GetDownCenterX(); //ȡ��ͷ���±����м�����
	virtual TSHORT GetDownCenterY(); //ȡ��ͷ���±����м�����
	virtual TSHORT GetOffSetX();

	virtual bool LoadTexture(char* filename);
};

class CFightLabel
{
protected:
	LPDIRECT3DDEVICE9  m_device;
	CFightHead*        m_figher;        //�ﵽս��������ͷ��ָ�룬��m_head����
	CFightHead*        m_selectHead;        //���ѡ�񹥻���ͷ��ָ�룬���ⲿ���ѡ����
	CTextureEx*        m_pointHead;     //���ѡ��ĳ�����ˣ��ڸ�ͷ��������ʾ��ͼ��,���м��أ�����ɾ��
	std::vector<CFightHead*>  m_heads;  //�������е�ͷ��,�ⲿ������ӣ��ڲ�����ɾ��
	CTextureEx*  m_back;
	TSHORT      m_length;   //����������
	TSHORT      m_posX, m_posY;  //����
	TSHORT      m_offsetX, m_offsetY;//ͷ����ʼƫ����
	TSHORT      m_offsetXM, m_offsetYM;  //����ͷ����ʼƫ����
	TSHORT      m_endOffsetX, m_endOffsetY;  //ͷ���ƶ���ĩβ��ƫ����

public:
	CFightLabel(LPDIRECT3DDEVICE9 device);
	virtual ~CFightLabel();

public:
	virtual void ShutDown();
	virtual void Update();
	virtual void Render();
	virtual void AddHead(CFightHead* head);
	virtual void SetSelectHead(CFightHead *head);

	virtual void SetPos(TSHORT x, TSHORT y);
	virtual void SetOffset(TSHORT x, TSHORT y);
	virtual void SetOffsetM(TSHORT x, TSHORT y);
	virtual void SetEndOffset(TSHORT x, TSHORT y);

	virtual void SetLength(TSHORT length);

	virtual bool LoadBackTexture(char* filename);
	virtual bool LoadPointHeadTex(char* filename);
	virtual bool ThereIsComplete();  //�Ƿ���ͷ�񵽴�ս��״̬
	virtual void ResetFighteHead();  //������ͷ��ͷ��Ż����
	virtual CFightHead* GetFighterHead();  //����ͷ�񵽴�ս��״̬��ȡ�øõ��˵�ָ��
	
};