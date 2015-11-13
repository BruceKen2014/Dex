
/*
Dex引擎数字类
*/
#include "../DexBase/typedefine.h"
#include "../Source/CTexture.h"
#include "../DexBase/CIni.h"

#pragma  once

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
//数字基类
class CNumber
{
	//LPDIRECT3DTEXTURE9 m_texture;  //数字图片纹理
protected:
	LPDIRECT3DDEVICE9 m_device;
	int  m_number ; //要显示的数字
	CTexture* m_texture;
	std::vector<int>  m_numberVec; //如果是纹理，则存储数字转换后的各位数的数组
	TSHORT m_startPosX, m_startPosY; //数字位置
	int   m_NumberWidth;  //每个纹理字符的宽度
	int   m_offSet; //数字之间的间隔(纹理数字默认是一个字符宽度)
	bool  m_OpenSign; //是否开启符号显示
public:
	CNumber(LPDIRECT3DDEVICE9 device);
	virtual ~CNumber();	 
	virtual void ShutDown();
public:	  
	virtual void Update();
	virtual void Render();
	virtual bool LoadTexture(char* filename);
	virtual void SetNumber(int number);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetOpenSign(bool openSign);
	

};

//动态数字	

class CNumberAni: public CNumber
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
	CNumberAni(LPDIRECT3DDEVICE9 device);
	virtual ~CNumberAni();
public:
	virtual void Update() ;	 
	virtual void Render();

	virtual bool LoadIni(char* filename, char*filed);

	virtual void SetSpeed(int speed);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetEndPos(TSHORT endX, TSHORT endY);
	virtual void ResetAlpha();  //重置alpha
	virtual bool GetComplete(); //数字是否显示完成
	
};
