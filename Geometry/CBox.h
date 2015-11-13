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
	D3DXVECTOR3 m_point0;	 //���ｫ��Щ������Ϊ��Ա����Ϊ������ײ���ʱÿ�ζ�Ҫ���Ѷ�������ͷſ�����
	D3DXVECTOR3 m_point1;	     //��������������ʡ�����ڴ棬����������CPU����
	D3DXVECTOR3 m_point2;
	D3DXVECTOR3 m_point3;
	D3DXVECTOR3 m_point4;
	D3DXVECTOR3 m_point5;
	D3DXVECTOR3 m_point6;
	D3DXVECTOR3 m_point7;
public:
	CBox();
	~CBox();

public:
	virtual bool Initialize(const D3DXVECTOR3& pos, const D3DXVECTOR3& _size);
	virtual bool Initialize(const D3DXVECTOR3& center, const D3DXVECTOR3& _size, bool flagcenter);
	virtual bool IsPointIn(D3DXVECTOR3 _point) const; //���Ƿ�������������(���������ϵ����)
	
	//�ж��Ƿ������ײ������ײ��ȡ����ײ��(��Բ�ĵ�Ͷ���)
	virtual bool CheckCollide(stBall& ball, D3DXVECTOR3& inci_point = D3DXVECTOR3(0.0f, 0.0f, 0.0f)) const; 

	virtual bool CheckCollide(const CBox* box) const;

	virtual D3DXVECTOR3 GetCenter();
	void setPosition(const D3DXVECTOR3& pos);
	void setCenter(const D3DXVECTOR3& pos); //��center�ƶ���pos��
	
	D3DXVECTOR3 getSize();

	D3DXVECTOR3 getPoint(int index) const ;//ȡ�ö�Ӧindex�Ķ˵�
};

#endif