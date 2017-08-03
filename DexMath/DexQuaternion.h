/*
DexEngine&quaternion
*/

#ifndef _DEX_QUATERNION_H
#define _DEX_QUATERNION_H
/*
http://www.wy182000.com/2012/07/17/quaternion四元数和旋转以及yaw-pitch-roll-的含义/
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
		DFloat32 f[4];
		struct
		{
			DFloat32 x, y, z, w;
		};
	};
public:
	DexQuaternion();
	DexQuaternion(const DexVector3& vec, DFloat32 radian);
	DexQuaternion(DFloat32 _x, DFloat32 _y, DFloat32 _z, DFloat32 _w);
	DexQuaternion(DFloat32 *pValue); //一个4维数组 前三个表明旋转轴 第四个表明旋转角度
public:
	DexQuaternion   Set(const DexVector3& vector, DFloat32 radian);
	DexQuaternion   Set(DFloat32 pitch, DFloat32 yaw, DFloat32 roll);
	DexQuaternion   Set(DFloat32 _x, DFloat32 _y, DFloat32 _z, DFloat32 _w);
	DexQuaternion   Normalize();
	DexMatrix4x4    GetMatrix()const;
	DexQuaternion   GetConjugate() const;
	DexQuaternion   MakeConjugate();
	//对vector向量应用四元数旋转
	DexVector3		Rotate(const DexVector3& vector)const;
	//点point绕着centerPoint以quaternion进行旋转
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
	DexQuaternion  operator * (DFloat32 value) const;
	DexQuaternion& operator *=(DFloat32 value) ;
	DexQuaternion  operator / (DFloat32 div) const;
	DexQuaternion& operator /=(DFloat32 div) ;
};
DexQuaternion  operator * (DFloat32 value, const DexQuaternion& quaternion);
#endif 