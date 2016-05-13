/*
DexEngine&quaternion
*/

#ifndef _DEX_QUATERNION_H
#define _DEX_QUATERNION_H
/*
http://www.wy182000.com/2012/07/17/quaternion��Ԫ������ת�Լ�yaw-pitch-roll-�ĺ���/
*/
#include "../DexBase/DexType.h"
#include "DexVector3.h"
#include "DexMatrix.h"
/*
q = xi + yj + zk +w
*/
class DexQuaternion
{
public:
	union 
	{
		float32 f[4];
		struct
		{
			float32 x, y, z, w;
		};
	};
public:
	DexQuaternion();
	DexQuaternion(const DexVector3& vec, float32 radian);
	DexQuaternion(float32 _x, float32 _y, float32 _z, float32 _w);
	DexQuaternion(float32 *pValue); //һ��4ά���� ǰ����������ת�� ���ĸ�������ת�Ƕ�
public:
	DexQuaternion   Set(const DexVector3& vector, float32 radian);
	DexQuaternion   Set(float32 pitch, float32 yaw, float32 roll);
	DexQuaternion   Set(float32 _x, float32 _y, float32 _z, float32 _w);
	DexQuaternion   Normalize();
	DexMatrix4x4    GetMatrix()const;
	DexQuaternion   GetConjugate() const;
	DexQuaternion   MakeConjugate();
	//��vector����Ӧ����Ԫ����ת
	DexVector3		Rotate(const DexVector3& vector)const;
	//��point����centerPoint��quaternion������ת
	DexVector3		Rotate(const DexVector3& point, const DexVector3 centerPoint) const;
public:
	DexQuaternion& operator = (const DexQuaternion& quaternion);
	bool		   operator ==(const DexQuaternion& quaternion) const;
	DexQuaternion  operator + (const DexQuaternion& quaternion) const;
	DexQuaternion& operator +=(const DexQuaternion& quaternion) ;
	DexQuaternion  operator - (const DexQuaternion& quaternion) const;
	DexQuaternion& operator -=(const DexQuaternion& quaternion);
	DexQuaternion  operator * (const DexQuaternion& quaternion) const;
	DexQuaternion& operator *=(const DexQuaternion& quaternion);
	DexQuaternion  operator * (float32 value) const;
	DexQuaternion& operator *=(float32 value) ;
	DexQuaternion  operator / (float32 div) const;
	DexQuaternion& operator /=(float32 div) ;
};
DexQuaternion  operator * (float32 value, const DexQuaternion& quaternion);
#endif 