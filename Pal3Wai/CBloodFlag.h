
/*
血条类，包括神条、气条
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
	CMyFont*      m_font;  //字体指针,外部传入，外部删除
	int m_number;
	int m_currNum;
	TSHORT m_posXF;	  //数字位置
	TSHORT m_posYF;
	TSHORT m_posX;	  //血条位置
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
	virtual void AddNum(int number);  //加血
	virtual void ReduceNum(int number); //减血操作
	virtual void SetFont(CMyFont* font);
};
