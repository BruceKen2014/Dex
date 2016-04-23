/*************************************************************
****DemoEx����ռ���������CBall
****���ߣ�������(BruceKen)
****���ڣ�2012-12-25
*************************************************************/

#ifndef _CBOX_H
#define _CBOX_H

#include "CBall.h"
#include "CPlane.h"

/*ʾ��ͼ
			7					 max
            ---------------------- 6
           /|				    /|
          / |				   / |
         /  |				  /	 | height
      4 /--------------------/5	 |
        |	|3	   *����	 |	 |
        |	|----------------|---| 2
        |  /            	 |	/
        | /0			     | / depth
        |/-------------------|/1	
   small(pos)   width
   */

class CBox
{
protected:
	float m_posX, m_posY, m_posZ;
	float m_maxX, m_maxY, m_maxZ;
	float m_width, m_height, m_depth;
	float m_centerX, m_centerY, m_centerZ;

protected:
	DexVector3 m_point0;	 //���ｫ��Щ������Ϊ��Ա����Ϊ������ײ���ʱÿ�ζ�Ҫ���Ѷ�������ͷſ�����
	DexVector3 m_point1;	     //��������������ʡ�����ڴ棬����������CPU����
	DexVector3 m_point2;
	DexVector3 m_point3;
	DexVector3 m_point4;
	DexVector3 m_point5;
	DexVector3 m_point6;
	DexVector3 m_point7;
public:
	CBox();
	~CBox();

public:
	virtual bool Initialize(const DexVector3& pos, const DexVector3& _size);
	virtual bool Initialize(const DexVector3& center, const DexVector3& _size, bool flagcenter);
	virtual bool IsPointIn(DexVector3 _point) const; //���Ƿ�������������(���������ϵ����)
	
	//�ж��Ƿ������ײ������ײ��ȡ����ײ��(��Բ�ĵ�Ͷ���)
	virtual bool CheckCollide(stBall& ball, DexVector3& inci_point = DexVector3(0.0f, 0.0f, 0.0f)) const;

	virtual bool CheckCollide(const CBox* box) const;

	virtual DexVector3 GetCenter();
	void setPosition(const DexVector3& pos);
	void setCenter(const DexVector3& pos); //��center�ƶ���pos��
	
	DexVector3 getSize();

	DexVector3 getPoint(int index) const;//ȡ�ö�Ӧindex�Ķ˵�
};

#endif