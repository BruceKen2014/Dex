/*************************************************************
****DemoEx����ƹ��װ��CLight
****���ߣ�������(BruceKen)
****���ڣ�2012-9-
*************************************************************/


#ifndef _CLIGHT_H
#define _CLIGHT_H

#include "../DexBase/typedefine.h"

class CLight
{
protected:
	D3DLIGHT9  m_light; 
	int32	   m_id;      //�ƹ�ID���
public:
	CLight() ;   
	CLight(D3DLIGHTTYPE _type, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular);   //�������ֵƹ⣬��Щ���Զ��ǹ�ͬ�� 
	virtual ~CLight() ;
	virtual void ShutDown();

public:
	virtual D3DLIGHT9   GetLight() const;
	virtual D3DXVECTOR3 GetPosition(); 
	virtual D3DXVECTOR3 GetDirection();
	virtual float       GetRange();
	virtual float       GetFallof();
	virtual float       GetTheta();
	virtual float       Getphi();
	virtual void        SetId( int32 _id);
	virtual int32       GetId() const;

	virtual void SetPostion(float x, float y, float z);  //------------���Դ---------�۹��------------------------
	virtual void SetPosition(D3DXVECTOR3 position);
	virtual void SetDirection(D3DXVECTOR3 direction);	 //---------------------------�۹��---------�����---------
	virtual void SetRange(float range);				     //------------���Դ---------�۹��------------------------
	virtual void SetFallof(float fallof);                //---------------------------�۹��------------------------
	virtual void SetThpi(float theta, float phi);
	virtual void SetTheta(float theta);                  //---------------------------�۹�ƹ�Դ�ڲ�׶�廡�Ƚ�------
	virtual void SetPhi(float phi);		                 //---------------------------�۹�ƹ�Դ�ⲿ׶�廡�Ƚ�------
};


#endif