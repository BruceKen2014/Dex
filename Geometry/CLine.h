
/*************************************************************
****DemoEx����ռ伸��ֱ����CLine
****���ߣ�������(BruceKen)
****���ڣ�2012-10-29
*************************************************************/

#ifndef _CLINE_H
#define _CLINE_H

#include "../DexMath/DexVector3.h"
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
	DexVector3 m_vector;    //ֱ������
	DexVector3 m_point;	 //ֱ���ϵ�ĳһ��

public:
	CLine();
	CLine(DexVector3 _vector, DexVector3 _point);
	CLine(DexVector3 point1, DexVector3 point2, int flag);
	//��������
	CLine(const stRay& ray);
	virtual ~CLine();

public:
	const DexVector3 GetVector() const;
	const DexVector3 GetPoint() const;
	void SetVector(DexVector3 _vector);
	void SetPoint(DexVector3 _point);

	//�������Ĺ�ϵ
	bool  IsOnLine(float x, float y, float z);       //�ж�ĳ���Ƿ���ֱ����
	float CalDistance(float x, float y , float z);   //����ĳ�㵽��ֱ�ߵľ���
	float CalDistance(DexVector3 _point);
	DexVector3 GetIncidence(DexVector3 _point);     //ȡ�õ㵽ֱ�ߵ�Ͷ���

	//������ֱ�ߵĹ�ϵ
	int   GetRelation(CLine line);	         //ȡ�ú���һ��ֱ�ߵĹ�ϵ

	/*���·����ݲ�ʵ�֣�������չ*/
	float CalDistance(CLine line);	         //��������ֱ�ߵľ���

	CLine& operator=(const CLine& _line); 

	//bool  
};

#endif