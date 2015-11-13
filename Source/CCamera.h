/*
/*************************************************************
****DemoEx引擎摄像机类
****作者：阚正杰(BruceKen)
*************************************************************
*/


#ifndef CCAMERA_H
#define CCAMERA_H

#include "../Geometry/CFrustum.h"
#include "../DexBase/DexSceneObject.h"




class CCamera:public CDexSceneObject
{	
	Dex_DeclareClass(CCamera,0)
public:
	typedef enum
	{ 
		DIR_VIEW,       //view轴
		DIR_UP,	    	//up轴
		DIR_RIGHT,      //right轴
		DIR_X,	   //X轴
		DIR_Y,	   //Y轴
		DIR_Z	   //Z轴
	}	  
	MOVE_DIRECTION;
	float       m_fFocusDistance;		  //焦距:摄像机位置到视点的距离
	D3DXVECTOR3 m_pos, m_focusPoint;	  //摄像机位置和视点
	D3DXVECTOR3 m_up, m_right, m_view;    //摄像机up轴、right轴、view轴
	D3DXVECTOR3 m_destPos;             //移动摄像机的目的地
	D3DXVECTOR3 m_moveDir;             //移动方向(设置目的地的时候自动计算)
	bool        m_isMoving;            //是否正在移动
	bool        m_moveFocus;           //是否移动焦点
	bool        m_handmove;            //开启自动移动时是否关闭手动移动
	float       m_moveVel;             //移动速度


	//投影矩阵参数
	float       m_fov;       //观察角
	float       m_aspect;    //宽高比
	float       m_nearDistance;	//最近距离
	float       m_farDistance;	//最远距离

protected:
	CFrustum    m_frustum;


protected:
	void Rotation(D3DXMATRIX matrix);     //绕指定轴旋转,在RotationX等三个方法中用到
public:
	CCamera() ;
	virtual ~CCamera(){};
	void  SetCamera(float x, float y, float z,     //位置
		           float xv, float yv, float zv,  //视点
		           float xu, float yu, float zu); //up轴
	void  SetCamera(D3DXVECTOR3 pos, D3DXVECTOR3 focuspoint, D3DXVECTOR3 upvector);
	virtual bool Update(int delta);

	//投影矩阵
	void  SetProject(float fov, float aspect, float near_distance, float far_distance);	//必须传参数，这样才能得到正确的平截头体
	float GetFov();
	float GetAspect();
	float GetNearDis();
	float GetFarDis();		
    //平截头体
	void        CalFrustum();	//只在用到的时候计算
	CFrustum    GetFrustum();

	D3DXVECTOR3 GetPosition()       { return m_pos;};	
	D3DXVECTOR3 GetView();
	D3DXVECTOR3 GetUpVector()       { return m_up;};
	D3DXVECTOR3 GetRight()          { return m_right;};
	D3DXVECTOR3 GetFocusPoint()     { return m_focusPoint;} ;
	
	float       GetFocusDis();



	void SetPosition(const D3DXVECTOR3& pos)  { m_pos = pos;};  
	void SetPosition(float x, float y, float z); 
	void SetFocus(const D3DXVECTOR3& focus)   { m_focusPoint = focus;};
	void SetFocus(float x, float y, float z); 
	//手动移动方法
	//绕自身的三个轴移动
	//move_focus :移动位置时是否移动视点
	void        MoveCamera(MOVE_DIRECTION direction, float amount, bool move_focus = false); 
	//沿着指定向量移动摄像机
	void        MoveCamera(D3DXVECTOR3 vec, float amount, bool move_focus = false);
	void        MoveFocus(MOVE_DIRECTION direction, float amount);
	void        MoveFocus(D3DXVECTOR3 vec, float amount);

	//自动移动方法
	virtual void  EnableMove(bool move);	    //激活、关闭自动移动   
	virtual bool  GetIsMoving();
	virtual void  SetHandMove(bool hand_move);  //自动移动时是否可以手动移动
	virtual bool  GetHandMove();
	virtual void  SetDestPos(D3DXVECTOR3 pos);   
	virtual void  SetDestPos(float x, float y, float z);
	virtual D3DXVECTOR3 GetDestPos();
	virtual void  SetMoveVel(float vel);
	virtual float GetMoveVel();
	virtual void  SetMoveFocus(bool move_focus);
	virtual bool  GetMoveFocus();

	
	void RotateSelf(MOVE_DIRECTION direction,float angle);    //绕自身的三个轴旋转
	void RotateUpY(float angle)	  ;                           //绕通过自身的向量(0,1,0)旋转
	void RotateX(float angle);			   //绕X轴旋转
	void RotateY(float angle);
	void RotateZ(float angle);
	void RotateFocus( float angle);  //绕视点旋转

};

#endif