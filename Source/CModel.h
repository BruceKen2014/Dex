/*************************************************************
****DemoEx����ģ�ͻ���
*************************************************************/

#ifndef  _CMODEL_H
#define  _CMODEL_H

#include "../DexBase/typedefine.h"
#include "../DexBase/DexLog.h"
#include "../DexBase/CDexTime.h"
#include "../Geometry/CBall.h"

#define   BALL_SCALE   0.7 //��ײԲ�����ű�

class CModel
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	D3DXMATRIX		  m_Matrix;
	float             m_posX, m_posY, m_posZ;       //����λ��
	float             m_desPosX, m_desPosY, m_desPosZ ;//Ҫ�ƶ�����Ŀ��λ��
	float             m_scaleX, m_scaleY, m_scaleZ;	//���������
	float             m_rotateX, m_rotateY, m_rotateZ;  //��ת�ĽǶ�
	float             m_vel;                              //���ٶ�
	float             m_VelX, m_VelY, m_VelZ;             //���ٶ�
	D3DXVECTOR3       m_iniDir;                     //��ʼ�������Լ������ε���ת�Ƕȣ�
	D3DXVECTOR3		  m_lastDir;                    //�ϴγ���(�����ж��Ƿ���Ҫ��ת)
	D3DXVECTOR3       m_direction;                  //��ǰ����
	int               m_id;

	
	stBall            m_iniBall;    //ԭʼ��,��X�ļ����ؽ���ʱ����Ϣ
	stBall            m_referBall;  //�����򣬾��������任�����Ϣ
	stBall            m_ball;       //ʵʱ��,���ݲ��������

	//��������������
#ifdef DEX_DEBGU
	LPD3DXMESH        m_sphere;	       //��Χ�����Ⱦ����
#endif

	bool              m_synDir;                     //�Ƿ����������ٶ�ͬ�������ٶȸı�ʱ�������Ƿ�Ҳ��Ҫ���ű�
	bool              m_renderSphere;  //�Ƿ���Ⱦ��Χ��
	bool              m_moving;        //�Ƿ������ƶ�
	bool              m_collide;       //�Ƿ�μ���ײ���
	bool              m_cullNone;      //�Ƿ���˫����Ⱦ(�������ӵȴ���alpha���ԵĶ�����Ҫ����)

public:
	CModel();
	virtual ~CModel(){};


protected:
	virtual void CalCulateMatrix();  //���ꡢ���š���ת�ı�ʱ���ô˺���
public:
	virtual void ShutDown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void RenderShpere();
	virtual bool CheckPick(stRay ray );    //�������Ƿ�����˸�������,���룺���������

public:

	D3DXMATRIX GetFinalMatrix() { return m_Matrix;};
	virtual void SetCullNone(bool b);  //�Ƿ���˫����Ⱦ
	virtual bool GetCullNone(); 
	//λ��
	virtual void SetPositionX(float xPos) ;
	virtual void SetPositionY(float yPos) ;
	virtual void SetPositionZ(float zPos) ;
	virtual void SetPosition(float xPos, float yPos, float zPos);
	virtual void SetPosition(D3DXVECTOR3 pos);

	virtual void Move(D3DXVECTOR3 vec, float length);
	//����
	virtual void SetScaleX(float xScale) ;
	virtual void SetScaleY(float yScale) ;
	virtual void SetScaleZ(float zScale) ;
	virtual void SetScale(float xScale, float yScale, float zScale);
	virtual void SetScale(float scale);//������������ ������SetPosition֮ǰ����
        	void GetScale(float& xScale, float& yScale, float&zScale);
	//��ת
	virtual void SetRotateX(float xRotate) ;
	virtual void SetRotateY(float yRotate) ;
	virtual void SetRotateZ(float zRotate) ;
	virtual void SetRotate(float xRotate, float yRotate, float zRotate);
	       float GetRotateY();

	//�ٶ�
	virtual void SetVeclX(float velX);
	virtual void SetVeclY(float velY) ;
	virtual void SetVeclZ(float velZ) ;
	virtual void SetVel(float velX, float velY, float velZ) ;
	virtual void SetVel(D3DXVECTOR3 vel);  //������������ķ��ٶ�
	virtual void SetVel(float vel);
	//����
	virtual void IniDirection(D3DXVECTOR3 dir) ; //�����������Ȳ����ģ�͵ĳ�����Ҫ�ȴ���һ����ʼ�ĳ���
	virtual void IniDirection(float x, float y, float z) ; //�����������Ȳ����ģ�͵ĳ�����Ҫ�ȴ���һ����ʼ�ĳ���
	virtual void SetDirection(D3DXVECTOR3 dir);
	virtual void SetDirection(float x, float y, float z);

											   
	virtual void SetRenderSphere(bool render) {  m_renderSphere = render;};	 //�Ƿ���Ⱦ��Χ��
	virtual void SetSysDir(bool sysdir)       {  m_synDir = sysdir;};        //�Ƿ����ٶȳ���ͬ��
	virtual void SetMoving(bool moving)		  {  m_moving = moving;};		 //�Ƿ����ƶ�״̬

	//�����ض���ģ��������ײ�����ų̶�
	virtual void SetBallScale(float scale);
	virtual D3DXVECTOR3 GetBallTop();//ȡ�ð�Χ��Ķ���

	virtual void SetCollide(bool collide);
	virtual bool GetCollide();

	virtual void SetId(int _id);
	virtual int  GetId();

	
	
	virtual bool GetRenderShpere()            {  return m_renderSphere;};	
	virtual bool GetSysDir()                  {  return m_synDir;};
	virtual bool GetMoving()                  {  return m_moving;};

	virtual stBall GetBall();    //ȡ�ð�Χ����Ϣ
	virtual D3DXVECTOR3 GetPosition() const;
	virtual D3DXVECTOR3 GetDirection();
	virtual float GetVelX() ;
	virtual float GetVelY() ;
	virtual float GetVelZ() ;
	virtual D3DXVECTOR3 GetVel();


	virtual void  FaceTo(D3DXVECTOR3 pos);	   //��ģ��ת��ĳ����
	virtual void  FaceTo(float x, float y, float z);
};

#endif