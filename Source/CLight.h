/*************************************************************
****DemoEx引擎灯光封装类CLight
****作者：阚正杰(BruceKen)
****日期：2012-9-
*************************************************************/


#ifndef _CLIGHT_H
#define _CLIGHT_H

#include "../DexBase/typedefine.h"

class CLight
{
protected:
	D3DLIGHT9  m_light; 
	int32	   m_id;      //灯光ID序号
public:
	CLight() ;   
	CLight(D3DLIGHTTYPE _type, D3DCOLORVALUE diffuse, D3DCOLORVALUE specular);   //无论哪种灯光，这些属性都是共同的 
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

	virtual void SetPostion(float x, float y, float z);  //------------点光源---------聚光灯------------------------
	virtual void SetPosition(D3DXVECTOR3 position);
	virtual void SetDirection(D3DXVECTOR3 direction);	 //---------------------------聚光灯---------方向光---------
	virtual void SetRange(float range);				     //------------点光源---------聚光灯------------------------
	virtual void SetFallof(float fallof);                //---------------------------聚光灯------------------------
	virtual void SetThpi(float theta, float phi);
	virtual void SetTheta(float theta);                  //---------------------------聚光灯光源内部锥体弧度角------
	virtual void SetPhi(float phi);		                 //---------------------------聚光灯光源外部锥体弧度角------
};


#endif