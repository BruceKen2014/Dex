
/*************************************************************
****DemoEx����ռ伸��ֱ����CLine
****���ߣ�������(BruceKen)
****���ڣ�2012-10-29
*************************************************************/

#ifndef _CLINE_H
#define _CLINE_H


#include "../DexBase/typedefine.h"
#include "CRay.h"

//����ֱ�ߵĹ�ϵ
enum 
{
	PARALLEL,    //ƽ��
	CROSS,       //����
	DIFFFACE,    //different face,����
};  


//3D�ռ�ֱ����
class CLine
{
//���öԳ�ʽ(����ʽ)����

protected:
	D3DXVECTOR3 m_vector;    //ֱ������
	D3DXVECTOR3 m_point;	 //ֱ���ϵ�ĳһ��

public:
	CLine();
	CLine(D3DXVECTOR3 _vector, D3DXVECTOR3 _point);
	CLine(D3DXVECTOR3 point1, D3DXVECTOR3 point2, int flag);
	//��������
	CLine(const stRay& ray);
	virtual ~CLine();

public:
	const D3DXVECTOR3 GetVector() const;
	const D3DXVECTOR3 GetPoint() const;
	void SetVector(D3DXVECTOR3 _vector);
	void SetPoint(D3DXVECTOR3 _point);

	//�������Ĺ�ϵ
	bool  IsOnLine(float x, float y, float z);       //�ж�ĳ���Ƿ���ֱ����
	float CalDistance(float x, float y , float z);   //����ĳ�㵽��ֱ�ߵľ���
	float CalDistance(D3DXVECTOR3 _point);
	D3DXVECTOR3 GetIncidence(D3DXVECTOR3 _point);     //ȡ�õ㵽ֱ�ߵ�Ͷ���

	//������ֱ�ߵĹ�ϵ
	int   GetRelation(CLine line);	         //ȡ�ú���һ��ֱ�ߵĹ�ϵ

	/*���·����ݲ�ʵ�֣�������չ*/
	float CalDistance(CLine line);	         //��������ֱ�ߵľ���

	CLine& operator=(const CLine& _line); 

	//bool  
};

#endif