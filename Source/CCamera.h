/*
/*************************************************************
****DemoEx�����������
****���ߣ�������(BruceKen)
*************************************************************
*/


#ifndef CCAMERA_H
#define CCAMERA_H

#include "../Geometry/CFrustum.h"
#include "../DexBase/DexSceneObject.h"
#include "../DexMath/DexVector3.h"



class CCamera:public CDexSceneObject
{	
	Dex_DeclareClass(CCamera,0)
public:
	typedef enum
	{ 
		DIR_VIEW,       //view��
		DIR_UP,	    	//up��
		DIR_RIGHT,      //right��
		DIR_X,	   //X��
		DIR_Y,	   //Y��
		DIR_Z	   //Z��
	}	  
	MOVE_DIRECTION;
	float       m_fFocusDistance;		  //����:�����λ�õ��ӵ�ľ���
	DexVector3 m_pos, m_focusPoint;	  //�����λ�ú��ӵ�
	DexVector3 m_up, m_right, m_view;    //�����up�ᡢright�ᡢview��
	DexVector3 m_destPos;             //�ƶ��������Ŀ�ĵ�
	DexVector3 m_moveDir;             //�ƶ�����(����Ŀ�ĵص�ʱ���Զ�����)
	bool        m_isMoving;            //�Ƿ������ƶ�
	bool        m_moveFocus;           //�Ƿ��ƶ�����
	bool        m_handmove;            //�����Զ��ƶ�ʱ�Ƿ�ر��ֶ��ƶ�
	float       m_moveVel;             //�ƶ��ٶ�


	//ͶӰ�������
	float       m_fov;       //�۲��
	float       m_aspect;    //��߱�
	float       m_nearDistance;	//�������
	float       m_farDistance;	//��Զ����

	//maya�������͵�camera�������ӵ��Զ����ȷ���ƶ���
	float       m_fMayaFlag;//����camera

protected:
	CFrustum    m_frustum;


protected:
	void Rotation(D3DXMATRIX matrix);     //��ָ������ת,��RotationX�������������õ�
public:
	CCamera() ;
	virtual ~CCamera(){};
	void  SetCamera(float x, float y, float z,     //λ��
		           float xv, float yv, float zv,  //�ӵ�
		           float xu, float yu, float zu); //up��
	void  SetCamera(DexVector3 pos, DexVector3 focuspoint, DexVector3 upvector);
	virtual bool Update(int delta);

	//ͶӰ����
	void  SetProject(float fov, float aspect, float near_distance, float far_distance);	//���봫�������������ܵõ���ȷ��ƽ��ͷ��
	float GetFov();
	float GetAspect();
	float GetNearDis();
	float GetFarDis();		
    //ƽ��ͷ��
	void        CalFrustum();	//ֻ���õ���ʱ�����
	CFrustum    GetFrustum();

	DexVector3 GetPosition()       { return m_pos; };
	DexVector3 GetView();
	DexVector3 GetUpVector()       { return m_up; };
	DexVector3 GetRight()          { return m_right; };
	DexVector3 GetFocusPoint()     { return m_focusPoint; };
	DexVector3 GetHeadDir();  //ȡ�������ͷ������
	
	float       GetFocusDis();



	void SetPosition(const DexVector3& pos)  { m_pos = pos; };
	void SetPosition(float x, float y, float z); 
	void SetFocus(const DexVector3& focus)   { m_focusPoint = focus; };
	void SetFocus(float x, float y, float z); 
	//�ֶ��ƶ�����
	//��������������ƶ�
	//move_focus :�ƶ�λ��ʱ�Ƿ��ƶ��ӵ�
	void        MoveCamera(MOVE_DIRECTION direction, float amount, bool move_focus = false); 
	//����ָ�������ƶ������
	void        MoveCamera(DexVector3 vec, float amount, bool move_focus = false);
	void        MoveFocus(MOVE_DIRECTION direction, float amount);
	void        MoveFocus(DexVector3 vec, float amount);

	//�Զ��ƶ�����
	virtual void  EnableMove(bool move);	    //����ر��Զ��ƶ�   
	virtual bool  GetIsMoving();
	virtual void  SetHandMove(bool hand_move);  //�Զ��ƶ�ʱ�Ƿ�����ֶ��ƶ�
	virtual bool  GetHandMove();
	virtual void  SetDestPos(DexVector3 pos);
	virtual void  SetDestPos(float x, float y, float z);
	virtual DexVector3 GetDestPos();
	virtual void  SetMoveVel(float vel);
	virtual float GetMoveVel();
	virtual void  SetMoveFocus(bool move_focus);
	virtual bool  GetMoveFocus();

	
	void RotateSelf(MOVE_DIRECTION direction,float angle);    //���������������ת
	void RotateUpY(float angle)	  ;                           //��ͨ�����������(0,1,0)��ת
	void RotateX(float angle);			   //��X����ת
	void RotateY(float angle);
	void RotateZ(float angle);
	void RotateFocus( float angle);  //���ӵ���ת

};

#endif