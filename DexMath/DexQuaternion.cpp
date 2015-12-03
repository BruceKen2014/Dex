
#include <math.h>
#include "DexMath.h"
#include "DexQuaternion.h"

DexQuaternion::DexQuaternion()
{
	x = y = z = w = 0.0f;
}

DexQuaternion::DexQuaternion(float32 _x, float32 _y, float32 _z, float32 _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

DexQuaternion::DexQuaternion(const DexVector3& vec, float32 radian)
{
	Set(vec, radian);
}

DexQuaternion DexQuaternion::Set(const DexVector3& vector, float32 radian)
{
	float32 _sin = 0.0f;
	radian *= 0.5f;
	DexVector3 temp(vector);
	temp.Normalize();
	_sin = sinf(radian);
	x = temp.x * _sin;
	y = temp.y * _sin;
	z = temp.z * _sin;
	w = cosf(radian);
	return *this;
}

DexQuaternion DexQuaternion::Set(float32 _x, float32 _y, float32 _z, float32 _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	return *this;
}

DexQuaternion DexQuaternion::Set(float32 pitch, float32 yaw, float32 roll)
{
	return *this;
}
DexQuaternion DexQuaternion::Normalize()
{
	float32 mag2 = x*x + y*y + z*z + w*w;
	if (DexMath::Equal(mag2, 0.0f))
	{
		mag2 = DexMath::Sqrt(mag2);
		x /= mag2;
		y /= mag2;
		z /= mag2;
		w /= mag2;
	}
	return *this;
}

DexMatrix4x4 DexQuaternion::GetMatrix() const
{
	
	float32 x2 = x*x;
	float32 y2 = y*y;
	float32 z2 = z*z;
	float32 xy = x*y;
	float32 xz = x*z;
	float32 yz = y*z;
	float32 wx = w*x;
	float32 wy = w*y;
	float32 wz = w*z;
	float32 value[16];
	value[0] = 1.0f - 2.0f * (y2 + z2);
	value[1] = 2.0f * (xy - wz);
	value[2] = 2.0f * (xz + wy);
	value[3] = 0.0f;
	value[4] = 2.0f * (xy + wz);
	value[5] = 1.0f - 2.0f * (x2 + z2);
	value[6] = 2.0f * (yz - wx);
	value[7] = 0.0f;
	value[8] = 2.0f * (xz - wy);
	value[9] = 2.0f * (yz + wx);
	value[10] = 1.0f - 2.0f * (x2 + y2);
	value[11] = 0.0f;
	value[12] = 0.0f;
	value[13] = 0.0f;
	value[14] = 0.0f;
	value[15] = 1.0f;
	DexMatrix4x4 ret(value);
	return ret;
}

DexQuaternion DexQuaternion::GetConjugate() const
{
	return DexQuaternion(-x, -y, -z, w);
}

DexQuaternion DexQuaternion::MakeConjugate()
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
	return *this;
}

DexVector3 DexQuaternion::Rotate(const DexVector3& vector) const
{
	DexVector3 temp(vector);
	temp.Normalize();

	DexQuaternion vecQuat(temp.x, temp.y, temp.z, 0.0f);
	DexQuaternion resQuat = vecQuat * GetConjugate();
	resQuat = *this * resQuat;
	return DexVector3(resQuat.x, resQuat.y, resQuat.z);
}
/*
四元数目前只直接用于旋转向量，先得到向量，记录两点距离
然后得到新的向量，从centerPoint倒退对应的距离，即得到新的点
*/
DexVector3 DexQuaternion::Rotate(const DexVector3& point, const DexVector3 centerPoint) const
{
	DexVector3 vector = centerPoint - point;
	float length = vector.Length();
	vector.Normalize();
	vector = Rotate(vector);
	vector.Normalize();
	return centerPoint - vector*length;
}

inline DexQuaternion& DexQuaternion::operator=(const DexQuaternion& quaternion)
{
	x = quaternion.x;
	y = quaternion.y;
	z = quaternion.z;
	w = quaternion.w;
	return *this;
}

inline bool DexQuaternion::operator==(const DexQuaternion& quaternion) const
{
	return DexMath::Equal(x, quaternion.x)
		&& DexMath::Equal(y, quaternion.y)
		&& DexMath::Equal(z, quaternion.z)
		&& DexMath::Equal(w, quaternion.w);
}
/*
q1 = x1i + y1j + z1k +w1
q2 = x2i + y2j + z2k +w2
q1 + q1 = (x1+x2)i + (y1+y2)j + (z1+z2)k + w1+w2
*/
inline DexQuaternion DexQuaternion::operator+(const DexQuaternion& quaternion) const
{
	DexQuaternion ret;
	ret.x = x + quaternion.x;
	ret.y = y + quaternion.y;
	ret.z = z + quaternion.z;
	ret.w = w + quaternion.w;
	return ret;
}

inline DexQuaternion& DexQuaternion::operator+=(const DexQuaternion& quaternion)
{
	x = x + quaternion.x;
	y = y + quaternion.y;
	z = z + quaternion.z;
	w = w + quaternion.w;
	return *this;
}


inline DexQuaternion DexQuaternion::operator-(const DexQuaternion& quaternion) const
{
	DexQuaternion ret;
	ret.x = x - quaternion.x;
	ret.y = y - quaternion.y;
	ret.z = z - quaternion.z;
	ret.w = w - quaternion.w;
	return ret;
}

inline DexQuaternion& DexQuaternion::operator-=(const DexQuaternion& quaternion)
{
	x = x - quaternion.x;
	y = y - quaternion.y;
	z = z - quaternion.z;
	w = w - quaternion.w;
	return *this;
}
/*
q1 = x1i + y1j + z1k + w1
q2 = x2i + y2j + z2k + w2
q1 * q2 满足分配率
其中i^2 = -1	
	j^2 = -1
	z^2 = -1
    i * j = k = - j * i
	j * k = i = - k * j
	k * i = j = - i * k
*/
inline DexQuaternion DexQuaternion::operator*(const DexQuaternion& quaternion) const
{
	 return DexQuaternion(w * quaternion.x + x * quaternion.w + y * quaternion.z - z * quaternion.y,
						  w * quaternion.y + y * quaternion.w + z * quaternion.x - x * quaternion.z,
						  w * quaternion.z + z * quaternion.w + x * quaternion.y - y * quaternion.x,
						  w * quaternion.x - x * quaternion.x - y * quaternion.y - z * quaternion.z);
}

inline DexQuaternion& DexQuaternion::operator*=(const DexQuaternion& quaternion)
{
	float32 _x = x;
	float32 _y = y;
	float32 _z = z;
	float32 _w = w;
	x = _w * quaternion.x + _x * quaternion.w + _y * quaternion.z - _z * quaternion.y;
	y = _w * quaternion.y + _y * quaternion.w + _z * quaternion.x - _x * quaternion.z;
	z = _w * quaternion.z + _z * quaternion.w + _x * quaternion.y - _y * quaternion.x;
	w = _w * quaternion.x - _x * quaternion.x - _y * quaternion.y - _z * quaternion.z;
	return *this;
}

inline DexQuaternion DexQuaternion::operator*(float32 value) const
{
	DexQuaternion ret;
	ret.x = x * value;
	ret.y = y * value;
	ret.z = z * value;
	ret.w = w * value;
	return ret;
}

inline DexQuaternion& DexQuaternion::operator*=(float32 value)
{
	x = x * value;
	y = y * value;
	z = z * value;
	w = w * value;
	return *this;
}

inline DexQuaternion DexQuaternion::operator/(float32 div) const
{
	DexQuaternion ret;
	if (DexMath::Equal(div, 0.0f))
	{
		return ret;
	}
	ret.x = x / div;
	ret.y = y / div;
	ret.z = z / div;
	ret.w = w / div;
	return ret;
}

inline DexQuaternion& DexQuaternion::operator/=(float32 div) 
{
	if (DexMath::Equal(div, 0.0f))
	{
		return *this;
	}
	x = x / div;
	y = y / div;
	z = z / div;
	w = w / div;
	return *this;
}

inline DexQuaternion operator * (float32 value, const DexQuaternion& quaternion)
{
	return DexQuaternion(value * quaternion.x,
		value * quaternion.y,
		value * quaternion.z,
		value * quaternion.z);
}