


/*战斗进度条类
By BuceKen
*/

#pragma once


#include "../Source/CTexture.h"
#include "CEnemy.h"
//战斗头像类
class CFightHead
{
protected:
	LPDIRECT3DDEVICE9  m_device;
	CTextureEx*  m_texture;  //头像
	CEnemy*     m_enmyPoint;  //指向外部战斗敌人的指针，通过外部传入，
	float  m_speed;  //速度
	TSHORT  m_offsetX, m_offsetY;	  //移动起点
	float m_posX, m_posY;  //当前位置
	bool   m_player ;  //是否是主角

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
	virtual void  SetIsPlayer(bool b);  //是否是主角头像
	virtual bool  GetIsPlayer();
	virtual TSHORT GetPosX();  //得到当前X位置
	virtual TSHORT GetPosY();
	virtual TSHORT GetDownCenterX(); //取得头像下边沿中间坐标
	virtual TSHORT GetDownCenterY(); //取得头像下边沿中间坐标
	virtual TSHORT GetOffSetX();

	virtual bool LoadTexture(char* filename);
};

class CFightLabel
{
protected:
	LPDIRECT3DDEVICE9  m_device;
	CFightHead*        m_figher;        //达到战斗条件的头像指针，由m_head传入
	CFightHead*        m_selectHead;        //玩家选择攻击的头像指针，由外部玩家选择传入
	CTextureEx*        m_pointHead;     //玩家选择某个敌人，在该头像下面显示剑图标,自行加载，自行删除
	std::vector<CFightHead*>  m_heads;  //进度条中的头像,外部负责添加，内部负责删除
	CTextureEx*  m_back;
	TSHORT      m_length;   //进度条长度
	TSHORT      m_posX, m_posY;  //坐标
	TSHORT      m_offsetX, m_offsetY;//头像起始偏移量
	TSHORT      m_offsetXM, m_offsetYM;  //主角头像起始偏移量
	TSHORT      m_endOffsetX, m_endOffsetY;  //头像移动到末尾的偏移量

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
	virtual bool ThereIsComplete();  //是否有头像到达战斗状态
	virtual void ResetFighteHead();  //将到尽头的头像放回起点
	virtual CFightHead* GetFighterHead();  //若有头像到达战斗状态，取得该敌人的指针
	
};