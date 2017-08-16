#pragma  once
#include "DexBase/typedefine.h"
#include "DexBase/DexObject.h"
#include "Source/CTexture.h"
#include "DexBase/CIni.h"

#include "PalTypeDefine.h"

//数字基类
class PalNumber
{
public:
	enum
	{
		CN_POSITIVE, //正数
		CN_NEGATIVE, //负数
		CN_NOSIGN,   //无符号
	};
	enum
	{
		NUMBER_DEFAUT, //默认数字
		NUMBER_PIC,    //纹理数字
	};
	//LPDIRECT3DTEXTURE9 m_texture;  //数字图片纹理
protected:
	int  m_number ; //要显示的数字
	CTexture* m_texture;
	std::vector<int>  m_numberVec; //如果是纹理，则存储数字转换后的各位数的数组
	TSHORT m_startPosX, m_startPosY; //数字位置
	int   m_NumberWidth;  //每个纹理字符的宽度
	int   m_offSet; //数字之间的间隔(纹理数字默认是一个字符宽度)
	bool  m_OpenSign; //是否开启符号显示
public:
	PalNumber();
	virtual ~PalNumber();	 
	virtual void ShutDown();
public:	  
	virtual void Update();
	virtual void Render();
	virtual bool LoadTexture(const char* filename);
	virtual void SetNumber(int number);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetOpenSign(bool openSign);


};

//动态数字	

class PalNumberAni: public PalNumber
{
protected:
	int m_type;	 //数字类型
	TSHORT m_currPosX, m_currPosY;
	TSHORT m_endPosX, m_endPosY;
	TSHORT m_speed;  //移动速度	
	TSHORT m_alpha;  //当前alpha
	TSHORT m_alphaSpeed; //alpha减少速度
	bool   m_complete; //是否移动完成

public:
	PalNumberAni();
	virtual ~PalNumberAni();
public:
	virtual void Update() ;	 
	virtual void Render();

	virtual bool LoadIni(const char* filename, char*filed);

	virtual void SetSpeed(int speed);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetEndPos(TSHORT endX, TSHORT endY);
	virtual void ResetAlpha();  //重置alpha
	virtual bool GetComplete(); //数字是否显示完成

};


class PalNumberManager: public DexObject
{
protected:
	std::list<PalNumberAni*>  g_listNumbers;

public:
	PalNumberManager();
	~PalNumberManager();

public:
	PalNumberAni* createNumberAni(EPlayerType type, bool skill_good, EPalPlayerHpType hpType);
	bool Render();
	bool Update(int delta);

};