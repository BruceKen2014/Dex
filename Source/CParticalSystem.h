

/*************************************************************
****DemoEx引擎粒子系统CPariticalSystem.h
****简介：CPariticalSystem粒子系统基类
          可由此派生出各种特殊的粒子特效
****@作者：阚正杰(BruceKen)
****@日期:2012-10-14 
*************************************************************/

#ifndef _CPARTICALSYSTEM_H
#define _CPARTICALSYSTEM_H


#include <iostream>
#include <vector>
#include <math.h>
#include "../DexBase/typedefine.h"
//#include <d3d9.h>
//#include <d3dx9.h>

#define D3DFVF_PARTICAL (D3DFVF_XYZ | D3DFVF_DIFFUSE ) 

#define PARTICAL_VELOCITY  0.5f    //默认的粒子速度(三个方向上的合速度)
#define PARTICAL_SIZE      0.5f    //默认的粒子大小
#define PARTICAL_COLOR     DexColor(1.0f, 1.0f, 1.0f)	 //默认的粒子颜色

typedef enum
{
	PS_NOTYPE,         //系统基类
	PS_FIREWORK = 1,   //烟花粒子系统
	PS_RAIN     = 2,   //雨雪粒子系统
	PS_FIRE     = 3,   //火焰粒子系统	
	PS_WATERFALL= 4,   //瀑布粒子系统
}PS_TYPE;	  //粒子系统类型


struct CPartical
{
	//这里的数据先后必须和顶点格式的先后顺序相同，否则渲染可能不正确
	float m_posX, m_posY, m_posZ;    //粒子位置
	DexColor m_color;                   //粒子颜色 
	float m_size;                    //粒子大小
	float m_velocityX, m_velocityY, m_velocityZ;  //粒子速度
	float m_deltaX, m_deltaY, m_deltaZ;           //加速度
public:
	CPartical()
	{
		m_posX = m_posY = m_posZ; 
		m_color = D3DCOLOR_XRGB(255,255,255);
		m_size = PARTICAL_SIZE; 
		m_velocityX = m_velocityY = m_velocityZ = 0;
		m_deltaX = m_deltaY = m_deltaZ = 0;
	}
};
	 
///////////////////////////////////////////////////////////
//粒子系统基类
class CParticalSystem      
{
protected:
	PS_TYPE	          m_type;              //系统类型 如烟花、雨雪等
	int               m_id;                //系统ID
	DWORD             m_FVF;               //粒子系统中粒子的顶点类型
	LPDIRECT3DDEVICE9 m_D3DDevice;         //设备指针	
	LPDIRECT3DTEXTURE9  m_texture;         //纹理
	LPDIRECT3DVERTEXBUFFER9 m_particalBuffer;  //粒子缓存	
	float             m_size;              //系统中的粒子大小  
	float             m_velocity;          //粒子速度
	float    m_posX, m_posY, m_posZ;       //系统位置
	int         m_particalCount;           //粒子数量
	DexColor         m_color;             //系统中的粒子颜色
	bool              m_isUniqueColor;     //系统是否是单色调(否则是缤纷彩色)
	bool              m_render;            //是否开启渲染

public:
	CParticalSystem();
	virtual ~CParticalSystem(){};
    virtual void ShutDown() = 0;
	virtual bool Check() = 0; 
public:
	virtual bool Initial() = 0;
	virtual void Render() = 0;
	virtual void UpdatePartical() = 0;

	virtual bool LoadTexture(char* filename);  //加载纹理
	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture) = 0;
	virtual void SetParticalVelocity(float velocity) = 0; //设置粒子速度，暂不能实时改变渲染的粒子速度
	virtual void SetParticalSize(float size) = 0;     //设置粒子大小，可实时改变粒子大小
	virtual void SetColor(DexColor color = PARTICAL_COLOR) = 0;	//设置粒子颜色       
	virtual void SetIsUniqueColor(bool isbool);       //设置是否单色调
	virtual void SetRender(bool render);              //是否渲染
	
	virtual void SetPosition(D3DXVECTOR3 pos);
	virtual bool GetIsUniqueColor() const;
	virtual bool GetRender() const;
	//注：					 
	//渲染期间可调用SetIsUniqueColor 和	 SetColor两个方法动态改变粒子颜色
	//比如原来是彩色 现在想变为绿色 则调用语句为
	//SetIsUniqueColor(true)
	//SetColor(color)
	//反之同理
	//SetIsUniqueColor(false)
	//SetColor()
};

///////////////////////////////////////////////////////////
//烟花粒子系统
class CFireworks: public CParticalSystem	
{
//下一版本:将发射与爆炸速度分开
protected: 
	//原来的系统位置信息则为发射点
	CPartical * m_particalList;            //粒子链表
	D3DXVECTOR3 m_explodePoint;     //爆炸点
	D3DXVECTOR3 m_launchDirection;  //发射方向，如(0,1,0) 向Y轴正方向发射 
	float m_launchDistance;         //发射距离，即从发射点到爆炸点的距离
	float m_nowDistance;            //当前距离
	float m_explodeRadius;          //爆炸半径
public:
	CFireworks();
	CFireworks(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CFireworks();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	//只能在渲染前调用的方法
	
	
	//渲染期间依然可以调用的方法	
	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(DexColor color = PARTICAL_COLOR);
public:	
	//设置烟花系统特有参数
	virtual void SetFirework(D3DXVECTOR3 direction, float distance, float radius);  

};

///////////////////////////////////////////////////////////
//雨雪粒子系统
class CRainSystem: public CParticalSystem
{
//下一版本：可添加雨雪的方向属性，风力属性
protected:
	//原来的位置信息变为长方体的中心点
	CPartical * m_particalList;            //粒子链表
	float m_width, m_height, m_depth; //长方体的长、宽、高
public:
	CRainSystem();
	CRainSystem(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CRainSystem();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(DexColor color = PARTICAL_COLOR);

public:
	//设置雨雪系统特有参数
	virtual void SetRainPS(float width, float height, float _depth);
};

///////////////////////////////////////////////////////////
//火焰粒子系统
class CFireSystem: public CParticalSystem
{
	//下一版本
protected:
	struct CFirePartical
	{
		//这里的数据先后必须和顶点格式的先后顺序相同，否则渲染可能不正确
		float m_posX, m_posY, m_posZ;    //粒子位置
		ULONG m_color;                   //粒子颜色 
		float m_size;                    //粒子大小
		float m_velocityX, m_velocityY, m_velocityZ;  //粒子速度
		float m_deltaX, m_deltaY, m_deltaZ;           //加速度
		float m_lifetime;                //粒子的生存时间
		float m_currentime;              //粒子的当前寿命
	public:
		CFirePartical()
		{
			m_posX = m_posY = m_posZ; 
			m_color = D3DCOLOR_XRGB(255,255,255);
			m_size = PARTICAL_SIZE; 
			m_velocityX = m_velocityY = m_velocityZ = 0;
			m_deltaX = m_deltaY = m_deltaZ = 0;
			m_lifetime = m_currentime = 0;
		}
	};
	CFirePartical * m_particalList;
	//原来的位置信息变为火焰粒子产生的中心点
	//这里模拟原型为：在一个球体内随机产生火焰粒子，并且向上燃烧
	float   m_radius;	  //火焰产生的半径
	float   m_pow;        //火焰的最大能量，这决定了火焰粒子生存的时间
	float   m_wave;       //火焰的波动指数,这将影响火焰的波动力道,通过粒子的加速度指数完成效果
	short   m_r, m_g, m_b; //火焰颜色
	                       //原来的粒子颜色信息为白色，即火焰中心的颜色
public:
	CFireSystem();
	CFireSystem(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CFireSystem();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(DexColor color = PARTICAL_COLOR); //设置所有粒子为该颜色
	
	virtual void SetFireColor(short r, short g, short b);//不是统一设置粒子颜色，而是设置火焰的最终颜色

public:
	//设置火焰系统特有参数
	virtual void SetFirePS(float radius, float _pow, float wave, short r, short g, short b);
};





///////////////////////////////////////////////////////////
//瀑布粒子系统(等将瀑布的生成算法想好的时候再实现)
/*class CWaterfall:public	 CParticalSystem
{
	//原先的位置信息现在为瀑布水流的中心
	//这里模拟的原型为一条小河流到指定位置向下倾泻
protected:
	struct CWaterfallPartical
	{
		//这里的数据先后必须和顶点格式的先后顺序相同，否则渲染可能不正确
		float m_posX, m_posY, m_posZ;    //粒子位置
		ULONG m_color;                   //粒子颜色 
		float m_size;                    //粒子大小
		float m_velocityX, m_velocityY, m_velocityZ;  //粒子速度
		float m_deltaX, m_deltaY, m_deltaZ;           //加速度
		float m_lifetime;                //粒子的生存时间
		float m_currentime;              //粒子的当前寿命
		float m_weight;                  //粒子的重量
	public:
		CWaterfallPartical()
		{
			m_posX = m_posY = m_posZ; 
			m_color = D3DCOLOR_XRGB(255,255,255);
			m_size = PARTICAL_SIZE; 
			m_velocityX = m_velocityY = m_velocityZ = 0;
			m_deltaX = m_deltaY = m_deltaZ = 0;
			m_lifetime = m_currentime = 0;
			m_weight = 0;
		}
	};
protected:
	CWaterfallPartical * m_particalList;
	D3DXVECTOR3 m_direction;  //水流的方向
	float m_width, m_length;  //水流的长宽
	float m_height;           //瀑布的高度，这将影响粒子的生存时间
	float m_gravity;          //重力加速度
public:
	CWaterfall();
	CWaterfall(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CWaterfall();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(D3DXCOLOR color = PARTICAL_COLOR); //设置所有粒子为该颜色

public:
	//设置瀑布系统特有参数
	virtual void SetWaterfall(D3DXVECTOR3 direction, float width, float length, float height, float gravity);
};
*/
///////////////////////////////////////////////////////////
//粒子系统管理器
//从脚本中加载粒子系统参数
class CParticalController
{
protected:
	std::vector<CParticalSystem*> m_particalvector;   //粒子系统向量
};
#endif