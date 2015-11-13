

/*************************************************************
****DemoEx����ռ伸��ƽ����CPlane
****���ߣ�������(BruceKen)
****���ڣ�2012-10-31
*************************************************************/

#ifndef _CPANEL_H
#define _CPANEL_H


#include "../DexBase/typedefine.h"
#include "CLine.h"
#include "CRay.h"

enum 
{
	CPANEL_ON,       //����ƽ����
	CPANEL_FRONT,    //����ƽ��ǰ��
	CPANEL_BACK,     //����ƽ�����
	CPANEL_CROSS,    //ֱ��(����)����ƽ��	ƽ���ཻ
	CPANEL_PARALLEL, //ֱ��(����)ƽ��ƽ�� ƽ��ƽ��ƽ��
	CPLANE_NOCROSS,  //������ƽ���޽���
};

class CPlane
{
//���ÿռ�ƽ���һ�㷽��
protected:			  
	float m_a, m_b, m_c, m_d;
public:
	CPlane();
	CPlane(float a, float b, float c, float d);	      //һ���ʼ��
	CPlane(D3DXVECTOR3 _normal, float d);			  //һ���ʼ������
	CPlane(D3DXVECTOR3 _normal, D3DXVECTOR3 point);   //�㷨ʽ��ʼ��
	CPlane(D3DXVECTOR3 point1, D3DXVECTOR3 point2, D3DXVECTOR3 point3);  //�����ʼ��
	~CPlane();

public:
	void  ShutDown();

	D3DXVECTOR3 GetNormal() const;                    //ȡ��ƽ�淨��
	void        SetNormal(D3DXVECTOR3 normal);
	void        Setm_d(float d);
	void        SetPoint(D3DXVECTOR3 point);

	//�������Ĺ�ϵ
	int   GetRelation(float x, float y, float z);     //����ƽ���ϡ�ǰ�桢����
	int   GetRelation(D3DXVECTOR3 point);			  
	float CalDistance(float x, float y, float z);	  //�㵽ƽ��ľ���
	float CalDistance(D3DXVECTOR3 point);
	D3DXVECTOR3 GetIncidence(D3DXVECTOR3 _point);  //ȡ��Ͷ���

	//������ֱ�ߵĹ�ϵ
	int   GetRelation(CLine line);                    //ƽ����ֱ��ƽ�л����ཻ
	float GetAngle(CLine line);						  //ȡ��ֱ����ƽ��ļн�
	float CalDistance(CLine line);                    //���ƽ��ֱ��ƽ�У���ֱ�ߵ�ƽ��ľ���
	D3DXVECTOR3 GetCrossPoint(CLine line);            //���ƽ��ֱ���ཻ��ȡ�ý���

	//���������ߵĹ�ϵ
	int   GetRelation(stRay ray);                                
	D3DXVECTOR3 GetCrossPoint(stRay ray);           

	//������ƽ��Ĺ�ϵ
	int   GetRelation(CPlane panel);
	float GetAngle(const CPlane& panel);	          //ȡ����ƽ��ļн�(�Զ���Ϊ��λ)

	/*�������������ݲ�ʵ�֣�������չ*/
	float CalDistance(CPlane panel);				  //��ƽ��ƽ�У�������ƽ�����
	CLine GetCrossLine(CPlane panel);                 //��ƽ���ཻ��ȡ���ֱཻ��

	
};

#endif