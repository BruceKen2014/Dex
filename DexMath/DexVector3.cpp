

#include "DexVector3.h"

DexVector3::DexVector3()
{
	x = y = z = 0.0f;
}

DexVector3::DexVector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

DexVector3::~DexVector3()
{

}
//static
void DexVector3::Normalize(DexVector3& vector3)
{
	float length = vector3.Length();
	if(length < 0.00001f)
	{
		return;
	}
	vector3.x = vector3.x / length;
	vector3.y = vector3.y / length;
	vector3.z = vector3.z / length;
}

DexVector3 operator* (float _value, const DexVector3& vector3)
{
	DexVector3 ret;
	ret.x = vector3.x *  _value;
	ret.y = vector3.y *  _value;
	ret.z = vector3.z *  _value;
	return ret;
}