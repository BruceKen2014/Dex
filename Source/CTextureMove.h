

/*具有移动性质的图片
By Bruce Ken
*/
#pragma  once

#include "../DexBase/typedefine.h"
#include "CTexture.h"


//移动图片的alpha闪烁类型
enum
{
	CTEXTUREMOVE_NONE, //alpha不变化
	CTEXTUREMOVE_ZTH,  //ZERO TO WHOLE  由全隐到全现
    CTEXTUREMOVE_HTZ, //whole to zero 由全现到全隐
	CTEXTUREMOVE_SPLASH, //不断闪动
};
class CTextureMove: public CTextureEx
{
protected:
	LPDIRECT3DDEVICE9 m_device;	
	TSHORT  m_startX, m_startY;	 //移动起始位置
	TSHORT  m_endX, m_endY;		 //移动末端位置
	float  m_currX, m_currY;
	float   m_speed;    //移动速度
	bool    m_complete;  //是否移动结束
	int     m_alphaFlag; //alpha闪动类型

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

	virtual void Reset(); //重置到初始状态

	virtual bool GetComplete();
};