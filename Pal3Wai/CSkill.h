

#pragma once 

#include "../Source/CParticalSystem.h"
#include "../DexBase/CIni.h"
#include "../sound/CSoundSystem.h"
#include "CEnemy.h"

extern bool g_OpenFog;
enum
{
	SKILL_FLAG,
	SKILL_ATTACK,
	SKILL_ADDBLOOD,
};
//法术基类
class CSkill
{
protected:
	LPDIRECT3DDEVICE9 m_device;	 
	CEnemy*           m_enmy;   //作用于哪个敌人的指针
	char              m_sndFile[64];
	int               m_id;     //ID
	int               m_type;   //类型
	int               m_num;    //攻击值或者加血量
	float	m_posX, m_posY, m_posZ;    //位置
	
	bool m_isComplete;  //法术是否完成

public:
	CSkill(LPDIRECT3DDEVICE9 device);
	virtual ~CSkill() {};

public:
	virtual void ShutDown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void SetEnmy(CEnemy* enmy) ;
	virtual CEnemy* GetEnmy();
	virtual void SetPosition(D3DXVECTOR3 pos)  { m_posX = pos.x;m_posY = pos.y;m_posZ = pos.z;};

	virtual bool GetIsComplete() { return m_isComplete;};
	virtual int  GetId()		 { return m_id;};
	virtual int  GetType()       { return m_type;};
	virtual int  GetNum();
};

//法术加血
class CSkillAddBlood:public CSkill
{
protected:
	//pos属性为立方体下面中心
	char                m_file[64];
	CPartical*          m_particalList;            //粒子链表
	DWORD               m_FVF;               //粒子系统中粒子的顶点类型
	LPDIRECT3DTEXTURE9  m_texture;           //纹理
	LPDIRECT3DVERTEXBUFFER9 m_particalBuffer;  //粒子缓存	
	float               m_size;              //系统中的粒子大小  
	float               m_velocity;          //粒子速度
	float               m_width, m_height, m_depth;
	int                 m_particalCount;           //粒子数量
	int                 m_time;             //系统生存时间
	int                 m_currTime;         //当前已经过的时间
	DexColor           m_color;             //系统中的粒子颜色
	//bool              m_isUniqueColor;     //系统是否是单色调(否则是缤纷彩色)
	//bool              m_render;            //是否开启渲染

public:
	CSkillAddBlood(LPDIRECT3DDEVICE9 device);
	virtual ~CSkillAddBlood() ;

public:
	virtual void ShutDown() ;
	virtual void Update() ;
	virtual void Render() ;

protected:
	virtual bool CreatePartical();
public:
	virtual void LoadIni(char* filename, char*filed);
	virtual bool LoadTexture(char* filename);  //加载纹理
	virtual bool Initial();
	void         RePlay();  //时间到了之后重头播放
	//virtual void SetTexture(LPDIRECT3DTEXTURE9 texture) = 0;
	//virtual void SetParticalVelocity(float velocity) = 0; //设置粒子速度，暂不能实时改变渲染的粒子速度
	//virtual void SetParticalSize(float size) = 0;     //设置粒子大小，可实时改变粒子大小
	//virtual void SetColor(D3DXCOLOR color = PARTICAL_COLOR) = 0;	//设置粒子颜色       
	//virtual void SetIsUniqueColor(bool isbool);       //设置是否单色调
	//virtual void SetRender(bool render);              //是否渲染
	//virtual bool GetIsUniqueColor() const;
	//virtual bool GetRender() const;
};