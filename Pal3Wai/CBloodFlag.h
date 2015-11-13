
/*
Ѫ���࣬��������������
*/
#pragma  once

#include "../Source/CTexture.h"
#include "../Source/CMyFont.h"
#include "../DexBase/CIni.h"

class CBloodLabel
{
protected: 
	LPDIRECT3DDEVICE9 m_device;
	CTextureEx*   m_texture;
	CMyFont*      m_font;  //����ָ��,�ⲿ���룬�ⲿɾ��
	int m_number;
	int m_currNum;
	TSHORT m_posXF;	  //����λ��
	TSHORT m_posYF;
	TSHORT m_posX;	  //Ѫ��λ��
	TSHORT m_posY;

public:
	CBloodLabel(LPDIRECT3DDEVICE9 device);
	virtual ~CBloodLabel();

public:
	virtual void ShutDown();
	virtual bool LoadIni(char* filename, char*filed);
	virtual void Update();
	virtual void Render();

	virtual void SetNumber(int number);
	virtual void SetCurrNum(int number);
	virtual int  GetCurrNum()  { return m_currNum;};
	virtual void AddNum(int number);  //��Ѫ
	virtual void ReduceNum(int number); //��Ѫ����
	virtual void SetFont(CMyFont* font);
};
