/*************************************************************
****DemoEx引擎模型基类
*************************************************************/

#ifndef  _CMODEL_H
#define  _CMODEL_H

#include "../DexBase/typedefine.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/CDexTime.h"
#include "../Geometry/CBall.h"

#define   BALL_SCALE   0.7 //碰撞圆球缩放比

class CModel
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	D3DXMATRIX		  m_Matrix;
	float             m_posX, m_posY, m_posZ;       //物体位置
	float             m_desPosX, m_desPosY, m_desPosZ ;//要移动到的目标位置
	float             m_scaleX, m_scaleY, m_scaleZ;	//物体的缩放
	float             m_rotateX, m_rotateY, m_rotateZ;  //旋转的角度
	float             m_vel;                              //总速度
	float             m_VelX, m_VelY, m_VelZ;             //分速度
	D3DXVECTOR3       m_iniDir;                     //初始朝向（用以计算两次的旋转角度）
	D3DXVECTOR3		  m_lastDir;                    //上次朝向(用以判断是否需要旋转)
	D3DXVECTOR3       m_direction;                  //当前朝向
	int               m_id;

	
	stBall            m_iniBall;    //原始球,从X文件加载进来时的信息
	stBall            m_referBall;  //参照球，经过伸缩变换后的信息
	stBall            m_ball;       //实时球,根据参照球求出

	//以下用作调试用
#ifdef DEX_DEBGU
	LPD3DXMESH        m_sphere;	       //包围球的渲染对象
#endif

	bool              m_synDir;                     //是否开启朝向与速度同步，即速度改变时，朝向是否也需要跟着变
	bool              m_renderSphere;  //是否渲染包围球
	bool              m_moving;        //是否正在移动
	bool              m_collide;       //是否参加碰撞检测
	bool              m_cullNone;      //是否开启双面渲染(对于竹子等带有alpha属性的对象需要开启)

public:
	CModel();
	virtual ~CModel(){};


protected:
	virtual void CalCulateMatrix();  //坐标、缩放、旋转改变时调用此函数
public:
	virtual void ShutDown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void RenderShpere();
	virtual bool CheckPick(stRay ray );    //检测鼠标是否放在了该物体上,输入：鼠标点击射线

public:

	D3DXMATRIX GetFinalMatrix() { return m_Matrix;};
	virtual void SetCullNone(bool b);  //是否开启双面渲染
	virtual bool GetCullNone(); 
	//位置
	virtual void SetPositionX(float xPos) ;
	virtual void SetPositionY(float yPos) ;
	virtual void SetPositionZ(float zPos) ;
	virtual void SetPosition(float xPos, float yPos, float zPos);
	virtual void SetPosition(D3DXVECTOR3 pos);

	virtual void Move(D3DXVECTOR3 vec, float length);
	//缩放
	virtual void SetScaleX(float xScale) ;
	virtual void SetScaleY(float yScale) ;
	virtual void SetScaleZ(float zScale) ;
	virtual void SetScale(float xScale, float yScale, float zScale);
	virtual void SetScale(float scale);//设置整体缩放 必须在SetPosition之前调用
        	void GetScale(float& xScale, float& yScale, float&zScale);
	//旋转
	virtual void SetRotateX(float xRotate) ;
	virtual void SetRotateY(float yRotate) ;
	virtual void SetRotateZ(float zRotate) ;
	virtual void SetRotate(float xRotate, float yRotate, float zRotate);
	       float GetRotateY();

	//速度
	virtual void SetVeclX(float velX);
	virtual void SetVeclY(float velY) ;
	virtual void SetVeclZ(float velZ) ;
	virtual void SetVel(float velX, float velY, float velZ) ;
	virtual void SetVel(D3DXVECTOR3 vel);  //设置三个方向的分速度
	virtual void SetVel(float vel);
	//朝向
	virtual void IniDirection(D3DXVECTOR3 dir) ; //这里由于事先不清楚模型的朝向，需要先传入一个初始的朝向
	virtual void IniDirection(float x, float y, float z) ; //这里由于事先不清楚模型的朝向，需要先传入一个初始的朝向
	virtual void SetDirection(D3DXVECTOR3 dir);
	virtual void SetDirection(float x, float y, float z);

											   
	virtual void SetRenderSphere(bool render) {  m_renderSphere = render;};	 //是否渲染包围球
	virtual void SetSysDir(bool sysdir)       {  m_synDir = sysdir;};        //是否开启速度朝向同步
	virtual void SetMoving(bool moving)		  {  m_moving = moving;};		 //是否开启移动状态

	//根据特定的模型设置碰撞球缩放程度
	virtual void SetBallScale(float scale);
	virtual D3DXVECTOR3 GetBallTop();//取得包围球的顶点

	virtual void SetCollide(bool collide);
	virtual bool GetCollide();

	virtual void SetId(int _id);
	virtual int  GetId();

	
	
	virtual bool GetRenderShpere()            {  return m_renderSphere;};	
	virtual bool GetSysDir()                  {  return m_synDir;};
	virtual bool GetMoving()                  {  return m_moving;};

	virtual stBall GetBall();    //取得包围球信息
	virtual D3DXVECTOR3 GetPosition() const;
	virtual D3DXVECTOR3 GetDirection();
	virtual float GetVelX() ;
	virtual float GetVelY() ;
	virtual float GetVelZ() ;
	virtual D3DXVECTOR3 GetVel();


	virtual void  FaceTo(D3DXVECTOR3 pos);	   //将模型转向某个点
	virtual void  FaceTo(float x, float y, float z);
};

#endif