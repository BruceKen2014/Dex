/*
DexEgine
Matrix
By Bruce
2015.11.21
*/
#ifndef _DEX_MATRIX_H
#define _DEX_MATRIX_H

#include <d3dx9math.h>
#include "../DexBase/DexType.h"
#include "DexVector3.h"
#include "DexMath.h"
/*
单位阵
 1 0 0 0
 0 1 0 0
 0 0 1 0
 0 0 0 1

 S: Scale
 P: Position

 Sx   0   0   0
 0   Sy   0   0
 0    0  Sz   0
 Px   Py  Pz  0


 单位阵绕X轴旋转a弧度后的矩阵
 1 0 0 0
 0 1 0 0
 0 0 1 0
 0 0 0 1
 1      0       0      0
 0    cos(a)  sin(a)   0
 0   -sin(a)  cos(a)   0
 0      0       0      1
 公式：_12 = old_12*cos(a) - old_13*sin(a);
       _13 = old_12*sin(a) + old_13*cos(a);

	   _22 = old_22*cos(a) - old_23*sin(a);
	   _23 = old_22*sin(a) + old_23*cos(a);

	   _32 = old_32*cos(a) - old_33*sin(a);
	   _33 = old_32*sin(a) + old_33*cos(a);

	   _42 = old_42*cos(a) - old_43*sin(a);
	   _43 = old_42*sin(a) + old_43*cos(a);
 单位阵绕Y轴旋转a弧度后的矩阵
 1 0 0 0
 0 1 0 0
 0 0 1 0
 0 0 0 1
 cos(a)     0     -sin(a)   0
   0        1       0       0
 sin(a)     0      cos(a)   0
   0        0       0       1
 公式：_11 =  old_11*cos(a) + old_13*sin(a);
	   _13 = -old_11*sin(a) + old_13*cos(a);

	   _21 =  old_21*cos(a) + old_23*sin(a);
	   _23 = -old_21*sin(a) + old_23*cos(a);

	   _31 =  old_31*cos(a) + old_33*sin(a);
	   _33 = -old_31*sin(a) + old_33*cos(a);

	   _41 =  old_41*cos(a) + old_43*sin(a);
	   _43 = -old_41*sin(a) + old_43*cos(a);
 单位阵绕Z轴旋转a弧度后的矩阵
 1 0 0 0
 0 1 0 0
 0 0 1 0
 0 0 0 1
  cos(a)     sin(a)     0     0
 -sin(a)     cos(a)     1     0
    0          0        1     0
    0          0        0     1
 公式：_11 =  old_11*cos(a) - old_12*sin(a);
	   _12 =  old_11*sin(a) + old_12*cos(a);

	   _21 =  old_21*cos(a) - old_22*sin(a);
	   _22 =  old_21*sin(a) + old_22*cos(a);

	   _31 =  old_31*cos(a) - old_32*sin(a);
	   _32 =  old_31*sin(a) + old_32*cos(a);

	   _41 =  old_41*cos(a) - old_42*sin(a);
	   _42 =  old_41*sin(a) + old_42*cos(a);
*/
template<typename T>
class DexMatrix4x4T
{

public:
	union
	{
		T m_m[4][4];
		T m[16];
		struct  
		{
			T    _11, _12, _13, _14;
			T    _21, _22, _23, _24;
			T    _31, _32, _33, _34;
			T    _41, _42, _43, _44; //41 42 43 为pos: x y z
		};
	};
public:
	DexMatrix4x4T();
	DexMatrix4x4T(const DexMatrix4x4T<T> &matrix);
	DexMatrix4x4T(T* p_value);
public:
	DexMatrix4x4T<T> Identity();
	bool			 IsIdentity()	const	;
	DexMatrix4x4T<T> GetTranspose() const; //得到矩阵的转置矩阵
	DexMatrix4x4T<T> MakeTranspose(); //转置本矩阵
	DexMatrix4x4T<T> Invert();   //变为自己的逆矩阵
	DexMatrix4x4T<T> GetInvert() const; //得到本矩阵的逆矩阵
	void			 Reset();
	void			 Set(T* pValue); //一个16维数组

	DexMatrix4x4T<T>&  operator= ( const DexMatrix4x4T<T>& matrix );
	DexMatrix4x4T<T>&  operator= ( const D3DXMATRIX& matrix );
	DexMatrix4x4T<T>   operator+ ( const DexMatrix4x4T<T>& matrix ) const;
	DexMatrix4x4T<T>&  operator+=( const DexMatrix4x4T<T>& matrix ) ;
	DexMatrix4x4T<T>   operator- ( const DexMatrix4x4T<T>& matrix ) const;
	DexMatrix4x4T<T>&  operator-=( const DexMatrix4x4T<T>& matrix ) ;
	DexMatrix4x4T<T>   operator* ( const DexMatrix4x4T<T>& matrix ) const;
	DexMatrix4x4T<T>&  operator*= ( const DexMatrix4x4T<T>& matrix );
	DexMatrix4x4T<T>   operator* ( const T& value ) const;
	DexMatrix4x4T<T>&  operator*=( const T& value );
	DexMatrix4x4T<T>   operator/ ( const T& value ) const;
	DexMatrix4x4T<T>&  operator/=( const T& value);
	bool  operator== ( const DexMatrix4x4T<T>& matrix ) const;
	bool  operator!= ( const DexMatrix4x4T<T>& matrix ) const;

	DexMatrix4x4T<T>  Scale(float32 x, float32 y, float32 z);
	DexMatrix4x4T<T>  Scale(const DexVector3& scale);
	DexMatrix4x4T<T>  Translate(float32 x, float32 y, float32 z);
	DexMatrix4x4T<T>  Translate(const DexVector3& trans);
	DexMatrix4x4T<T>  SetPosition(float32 x, float32 y, float32 z);
	DexMatrix4x4T<T>  SetPosition(const DexVector3& pos);

	DexVector3		  GetPosition() const;
	//void MulVec4(CVector4* vec);

	DexMatrix4x4T<T> RotateX(float32 radian);
	DexMatrix4x4T<T> RotateY(float32 radian);
	DexMatrix4x4T<T> RotateZ(float32 radian);
};
template<typename T>
inline DexMatrix4x4T<T>::DexMatrix4x4T()
{
	Identity();
}
template<typename T>
inline DexMatrix4x4T<T>::DexMatrix4x4T(const DexMatrix4x4T<T> &matrix)
{
	memcpy(m_m, matrix.m_m, sizeof(T)*16); //改为dexmemcpy
}
template<typename T>
inline DexMatrix4x4T<T>::DexMatrix4x4T(T* p_value) //一个16个T的数组
{
	memcpy(m_m, p_value, sizeof(T)*16); 
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::Identity()
{
	memset(m_m, 0, sizeof(T)*16);
	m_m[0][0] = m_m[1][1] = m_m[2][2] = m_m[3][3] = (T)1;
	return *this;
}
template<typename T>
inline bool DexMatrix4x4T<T>::IsIdentity()	const
{
	//先判断对角线，算法速度会快一点
	if( !DexMath::Equal(m_m[0][0], (T)1) ||
		!DexMath::Equal(m_m[1][1], (T)1) ||
		!DexMath::Equal(m_m[2][2], (T)1) ||
		!DexMath::Equal(m_m[3][3], (T)1) 
		)
		return false;
	for(int i = 0 ; i < 4; ++i)
	{
		for(int j = 0 ; j < 4 ; ++j)
		{
			if( i != j)
			{
				if(!DexMath::Equal(m_m[i][j], (T)0))
				{
					return false;
				}
			}
		}
	}
	return true;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::GetTranspose()	const
{
	DexMatrix4x4T<T> ret;
	//for (int line = 0; line < 4; ++line)
	//{
	//	for (int col = 0; col < 4; ++col)
	//	{
	//		ret.m_m[line][col] = m_m[col][line];
	//	}
	//}
	ret.m_m[0][0] = m_m[0][0]; ret.m_m[0][1] = m_m[1][0]; ret.m_m[0][2] = m_m[2][0]; ret.m_m[0][3] = m_m[3][0];
	ret.m_m[1][0] = m_m[0][1]; ret.m_m[1][1] = m_m[1][1]; ret.m_m[1][2] = m_m[2][1]; ret.m_m[1][3] = m_m[3][1];
	ret.m_m[2][0] = m_m[0][2]; ret.m_m[2][1] = m_m[1][2]; ret.m_m[2][2] = m_m[2][2]; ret.m_m[2][3] = m_m[3][2];
	ret.m_m[3][0] = m_m[0][3]; ret.m_m[3][1] = m_m[1][3]; ret.m_m[3][2] = m_m[2][3]; ret.m_m[3][3] = m_m[3][3];
	return ret;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::MakeTranspose()
{
	DexMatrix4x4T<T> ret;
	ret.m_m[0][0] = m_m[0][0]; ret.m_m[0][1] = m_m[1][0]; ret.m_m[0][2] = m_m[2][0]; ret.m_m[0][3] = m_m[3][0];
	ret.m_m[1][0] = m_m[0][1]; ret.m_m[1][1] = m_m[1][1]; ret.m_m[1][2] = m_m[2][1]; ret.m_m[1][3] = m_m[3][1];
	ret.m_m[2][0] = m_m[0][2]; ret.m_m[2][1] = m_m[1][2]; ret.m_m[2][2] = m_m[2][2]; ret.m_m[2][3] = m_m[3][2];
	ret.m_m[3][0] = m_m[0][3]; ret.m_m[3][1] = m_m[1][3]; ret.m_m[3][2] = m_m[2][3]; ret.m_m[3][3] = m_m[3][3];
	*this = ret;
	return ret;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::Invert()
{
	*this = GetInvert();
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::GetInvert() const
{
//矩阵A的逆矩阵A^-1 = A^*/|A|
//即A的伴随阵除以A的行列式
	if (IsIdentity())
		return *this;
#define determinant22(a11,a12,a21,a22)	(a11*a22 - a21*a12)
#define determinant33(a11,a12,a13,a21,a22,a23,a31,a32,a33)	\
	(a11 * determinant22(a22, a23, a32, a33) - a12*determinant22(a21, a23, a31, a33) + a13*determinant22(a21,a22,a31,a32))
	T d = _11 * determinant33(_22, _23, _24, _32, _33, _34, _42, _43, _44)
		- _12 * determinant33(_21, _23, _24, _31, _33, _34, _41, _43, _44)
		+ _13 * determinant33(_21, _22, _24, _31, _32, _34, _41, _42, _44)
		- _14 * determinant33(_21, _22, _23, _31, _32, _33, _41, _42, _43);
	if (DexMath::Equal(d, 0.0f))
		return *this;
	T A11 = determinant33(_22, _23, _24, _32, _33, _34, _42, _43, _44);
	T A12 =-determinant33(_21, _23, _24, _31, _33, _34, _41, _43, _44);
	T A13 = determinant33(_21, _22, _24, _31, _32, _34, _41, _42, _44);
	T A14 =-determinant33(_21, _22, _23, _31, _32, _33, _41, _42, _43);
	T A21 =-determinant33(_12, _13, _14, _32, _33, _34, _42, _43, _44);
	T A22 = determinant33(_11, _13, _14, _31, _33, _34, _41, _43, _44);
	T A23 =-determinant33(_11, _12, _14, _31, _32, _34, _41, _42, _44);
	T A24 = determinant33(_11, _12, _13, _31, _32, _33, _41, _42, _43);
	T A31 = determinant33(_12, _13, _14, _22, _23, _24, _42, _43, _44);
	T A32 =-determinant33(_11, _13, _14, _21, _23, _24, _41, _43, _44);
	T A33 = determinant33(_11, _12, _14, _21, _22, _24, _41, _42, _44);
	T A34 =-determinant33(_11, _12, _13, _21, _22, _23, _41, _42, _43);
	T A41 =-determinant33(_12, _13, _14, _22, _23, _24, _32, _33, _34);
	T A42 = determinant33(_11, _13, _14, _21, _23, _24, _31, _33, _34);
	T A43 =-determinant33(_11, _12, _14, _21, _22, _24, _31, _32, _34);
	T A44 = determinant33(_11, _12, _13, _21, _22, _23, _31, _32, _33);
	T A[16] = { A11,A21,A31,A41,A12,A22,A32,A42,A13,A23,A33,A43,A14,A24,A34,A44};
	DexMatrix4x4T<T> ret(A);
	ret /= d;
	return ret;
#undef determinant22
#undef determinant33
}
template<typename T>
inline void DexMatrix4x4T<T>::Reset()
{
	memset(m, 0, sizeof(T)*16);
}
template<typename T>
inline void DexMatrix4x4T<T>::Set(T* pValue)
{
	memcpy(m, pValue, sizeof(T) * 16);
}
template<typename T>
inline DexMatrix4x4T<T>& DexMatrix4x4T<T>::operator=( const DexMatrix4x4T &matrix )
{
	if(this == &matrix)
		return *this;
	memcpy(m_m, matrix.m_m, sizeof(T) * 16);
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T>& DexMatrix4x4T<T>::operator=( const D3DXMATRIX& matrix )
{
	memcpy(m_m, matrix.m, sizeof(T) * 16);
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::operator+( const DexMatrix4x4T<T>& matrix ) const
{
	DexMatrix4x4T<T> ret;
	ret.m[0]  = m[0]  + matrix.m[0]; 
	ret.m[1]  = m[1]  + matrix.m[1]; 
	ret.m[2]  = m[2]  + matrix.m[2]; 
	ret.m[3]  = m[3]  + matrix.m[3]; 
	ret.m[4]  = m[4]  + matrix.m[4]; 
	ret.m[5]  = m[5]  + matrix.m[5]; 
	ret.m[6]  = m[6]  + matrix.m[6]; 
	ret.m[7]  = m[7]  + matrix.m[7]; 
	ret.m[8]  = m[8]  + matrix.m[8]; 
	ret.m[9]  = m[9]  + matrix.m[9]; 
	ret.m[10] = m[10] + matrix.m[10]; 
	ret.m[11] = m[11] + matrix.m[11]; 
	ret.m[12] = m[12] + matrix.m[12]; 
	ret.m[13] = m[13] + matrix.m[13]; 
	ret.m[14] = m[14] + matrix.m[14]; 
	ret.m[15] = m[15] + matrix.m[15]; 
	return ret;
}
template<typename T>
inline DexMatrix4x4T<T>& DexMatrix4x4T<T>::operator+=( const DexMatrix4x4T<T>& matrix)
{
	m[0]  += matrix.m[0];
	m[1]  += matrix.m[1];
	m[2]  += matrix.m[2];
	m[3]  += matrix.m[3];
	m[4]  += matrix.m[4];
	m[5]  += matrix.m[5];
	m[6]  += matrix.m[6];
	m[7]  += matrix.m[7];
	m[8]  += matrix.m[8];
	m[9]  += matrix.m[9];
	m[10] += matrix.m[10];
	m[11] += matrix.m[11];
	m[12] += matrix.m[12]; 
	m[13] += matrix.m[13];
	m[14] += matrix.m[14];
	m[15] += matrix.m[15];
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::operator-( const DexMatrix4x4T<T>& matrix ) const
{
	DexMatrix4x4T<T> ret;
	ret.m[0]  = m[0]  - matrix.m[0]; 
	ret.m[1]  = m[1]  - matrix.m[1]; 
	ret.m[2]  = m[2]  - matrix.m[2]; 
	ret.m[3]  = m[3]  - matrix.m[3]; 
	ret.m[4]  = m[4]  - matrix.m[4]; 
	ret.m[5]  = m[5]  - matrix.m[5]; 
	ret.m[6]  = m[6]  - matrix.m[6]; 
	ret.m[7]  = m[7]  - matrix.m[7]; 
	ret.m[8]  = m[8]  - matrix.m[8]; 
	ret.m[9]  = m[9]  - matrix.m[9]; 
	ret.m[10] = m[10] - matrix.m[10]; 
	ret.m[11] = m[11] - matrix.m[11]; 
	ret.m[12] = m[12] - matrix.m[12]; 
	ret.m[13] = m[13] - matrix.m[13]; 
	ret.m[14] = m[14] - matrix.m[14]; 
	ret.m[15] = m[15] - matrix.m[15]; 
	return ret;
}
template<typename T>
inline DexMatrix4x4T<T>& DexMatrix4x4T<T>::operator-=( const DexMatrix4x4T<T>& matrix)
{
	m[0]  -= matrix.m[0];
	m[1]  -= matrix.m[1];
	m[2]  -= matrix.m[2];
	m[3]  -= matrix.m[3];
	m[4]  -= matrix.m[4];
	m[5]  -= matrix.m[5];
	m[6]  -= matrix.m[6];
	m[7]  -= matrix.m[7];
	m[8]  -= matrix.m[8];
	m[9]  -= matrix.m[9];
	m[10] -= matrix.m[10];
	m[11] -= matrix.m[11];
	m[12] -= matrix.m[12]; 
	m[13] -= matrix.m[13];
	m[14] -= matrix.m[14];
	m[15] -= matrix.m[15];
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::operator*( const DexMatrix4x4T<T> &matrix ) const
{
	if(IsIdentity())
	{//如果这个是单位阵，直接返回matrix，加快速度
		return matrix;
	}
	if(matrix.IsIdentity())
	{
		return *this;
	}
	DexMatrix4x4T<T> ret;
	//for(int i = 0 ; i < 4; i++)
	//{
	//	for( int j = 0 ; j < 4; j++)
	//	{
	//		for(int k = 0 ; k < 4; k++)
	//			ret.m_m[i][j] += m_m[i][k] * matrix.m_m[k][j];
	//	}
	//}
	//return ret;
	ret._11  = _11 * matrix._11  + _12 * matrix._21  + _13 * matrix._31 + _14 * matrix._41;
	ret._12  = _11 * matrix._12  + _12 * matrix._22  + _13 * matrix._32 + _14 * matrix._42;
	ret._13  = _11 * matrix._13  + _12 * matrix._23  + _13 * matrix._33 + _14 * matrix._43;
	ret._14  = _11 * matrix._14  + _12 * matrix._24  + _13 * matrix._34 + _14 * matrix._44;

	ret._21  = _21 * matrix._11  + _22 * matrix._21  + _23 * matrix._31 + _24 * matrix._41;
	ret._22  = _21 * matrix._12  + _22 * matrix._22  + _23 * matrix._32 + _24 * matrix._42;
	ret._23  = _21 * matrix._13  + _22 * matrix._23  + _23 * matrix._33 + _24 * matrix._43;
	ret._24  = _21 * matrix._14  + _22 * matrix._24  + _23 * matrix._34 + _24 * matrix._44;

	ret._31  = _31 * matrix._11  + _32 * matrix._21  + _33 * matrix._31 + _34 * matrix._41;
	ret._32  = _31 * matrix._12  + _32 * matrix._22  + _33 * matrix._32 + _34 * matrix._42;
	ret._33  = _31 * matrix._13  + _32 * matrix._23  + _33 * matrix._33 + _34 * matrix._43;
	ret._34  = _31 * matrix._14  + _32 * matrix._24  + _33 * matrix._34 + _34 * matrix._44;

	ret._41  = _41 * matrix._11  + _42 * matrix._21  + _43 * matrix._31 + _44 * matrix._41;
	ret._42  = _41 * matrix._12  + _42 * matrix._22  + _43 * matrix._32 + _44 * matrix._42;
	ret._43  = _41 * matrix._13  + _42 * matrix._23  + _43 * matrix._33 + _44 * matrix._43;
	ret._44  = _41 * matrix._14  + _42 * matrix._24  + _43 * matrix._34 + _44 * matrix._44;
	return ret;
}
template<typename T>
inline DexMatrix4x4T<T>& DexMatrix4x4T<T>::operator*=( const DexMatrix4x4T<T> &matrix )
{
	*this = *this * matrix;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::operator*( const T& value ) const
{
	DexMatrix4x4T<T> ret;
	ret.m[0]  = m[0]  * value; 
	ret.m[1]  = m[1]  * value; 
	ret.m[2]  = m[2]  * value; 
	ret.m[3]  = m[3]  * value; 
	ret.m[4]  = m[4]  * value; 
	ret.m[5]  = m[5]  * value; 
	ret.m[6]  = m[6]  * value; 
	ret.m[7]  = m[7]  * value; 
	ret.m[8]  = m[8]  * value; 
	ret.m[9]  = m[9]  * value; 
	ret.m[10] = m[10] * value; 
	ret.m[11] = m[11] * value; 
	ret.m[12] = m[12] * value; 
	ret.m[13] = m[13] * value; 
	ret.m[14] = m[14] * value; 
	ret.m[15] = m[15] * value; 
	return ret;
}
template<typename T>
inline DexMatrix4x4T<T>& DexMatrix4x4T<T>::operator*=( const T& value )
{
	m[0] *= value;
	m[1] *= value;
	m[2] *= value;
	m[3] *= value;
	m[4] *= value;
	m[5] *= value;
	m[6] *= value;
	m[7] *= value;
	m[8] *= value;
	m[9] *= value;
	m[10] *= value;
	m[11] *= value;
	m[12] *= value;
	m[13] *= value;
	m[14] *= value;
	m[15] *= value;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::operator/( const T& value ) const
{
	DexMatrix4x4T<T> ret;
	if(DexMath::Equal(value, 0.0f))
	{
		ret.Identity();
		return ret;
	}
	ret.m[0]  = m[0]  / value; 
	ret.m[1]  = m[1]  / value; 
	ret.m[2]  = m[2]  / value; 
	ret.m[3]  = m[3]  / value; 
	ret.m[4]  = m[4]  / value; 
	ret.m[5]  = m[5]  / value; 
	ret.m[6]  = m[6]  / value; 
	ret.m[7]  = m[7]  / value; 
	ret.m[8]  = m[8]  / value; 
	ret.m[9]  = m[9]  / value; 
	ret.m[10] = m[10] / value; 
	ret.m[11] = m[11] / value; 
	ret.m[12] = m[12] / value; 
	ret.m[13] = m[13] / value; 
	ret.m[14] = m[14] / value; 
	ret.m[15] = m[15] / value; 
	return ret;
}
template<typename T>
inline DexMatrix4x4T<T>& DexMatrix4x4T<T>::operator/=(const T& value)
{
	if (DexMath::Equal(value, 0.0f))
	{
		return *this;
	}
	m[0] = m[0] / value;
	m[1] = m[1] / value;
	m[2] = m[2] / value;
	m[3] = m[3] / value;
	m[4] = m[4] / value;
	m[5] = m[5] / value;
	m[6] = m[6] / value;
	m[7] = m[7] / value;
	m[8] = m[8] / value;
	m[9] = m[9] / value;
	m[10] = m[10] / value;
	m[11] = m[11] / value;
	m[12] = m[12] / value;
	m[13] = m[13] / value;
	m[14] = m[14] / value;
	m[15] = m[15] / value;
	return *this;
}
template<typename T>
inline bool DexMatrix4x4T<T>::operator == (const DexMatrix4x4T<T>& matrix) const
{
	for(int i = 0; i < 16;++i)
	{
		if(!DexMath::Equal(m[i], matrix.m[i]))
			return false;
	}
	return true;
}
template<typename T>
inline bool DexMatrix4x4T<T>::operator != (const DexMatrix4x4T<T>& matrix) const
{
	for(int i = 0; i < 16;++i)
	{
		if(!DexMath::Equal(m[i], matrix.m[i]))
			return true;
	}
	return false;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::Scale(float32 x, float32 y, float32 z)
{
	DexMatrix4x4T<T> ma;
	ma.Identity();
	ma._11 = x;
	ma._22 = y;
	ma._33 = z;
	(*this) = (*this) * ma;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::Scale(const DexVector3& scale)
{
	DexMatrix4x4T<T> ma;
	ma.Identity();
	ma._11 = scale.x;
	ma._22 = scale.y;
	ma._33 = scale.z;
	(*this) = (*this) * ma;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T>  DexMatrix4x4T<T>::Translate(float32 x, float32 y, float32 z)
{
	DexMatrix4x4T temp;
	temp.Identity();
	temp.SetPosition(x, y, z);
	*this = (*this) * temp;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T>  DexMatrix4x4T<T>::Translate(const DexVector3& trans)
{
	DexMatrix4x4T temp;
	temp.Identity();
	temp.SetPosition(trans);
	*this = (*this) * temp;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::SetPosition(float32 x, float32 y, float32 z)
{
	m_m[3][0] = x;
	m_m[3][1] = y;
	m_m[3][2] = z;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::SetPosition(const DexVector3& pos)
{
	m_m[3][0] = pos.x;
	m_m[3][1] = pos.y;
	m_m[3][2] = pos.z;
	return *this;
}
template<typename T>
inline DexVector3 DexMatrix4x4T<T>::GetPosition()	const
{
	DexVector3 ret;
	ret.x = m_m[3][0];
	ret.y = m_m[3][1];
	ret.z = m_m[3][2];
	return ret;
}
//template<typename T>
//inline void DexMatrix4x4T<T>::MulVec4(CVector4* vec)
//{
//}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::RotateX(float radian)
{
	//float32 old_12 = _12;
	//float32 old_13 = _13;
	//float32 a = radian;
	//_12 = old_12*cos(a) - old_13*sin(a);
	//_13 = old_12*sin(a) + old_13*cos(a);

	//float32 old_22 = _22;
	//float32 old_23 = _23;
	//_22 = old_22*cos(a) - old_23*sin(a);
	//_23 = old_22*sin(a) + old_23*cos(a);

	//float32 old_32 = _32;
	//float32 old_33 = _33;
	//_32 = old_32*cos(a) - old_33*sin(a);
	//_33 = old_32*sin(a) + old_33*cos(a);

	//float32 old_42 = _42;
	//float32 old_43 = _43;
	//_42 = old_42*cos(a) - old_43*sin(a);
	//_43 = old_42*sin(a) + old_43*cos(a);
	T value[16] =
	{
		1, 0, 0, 0,
		0,  DexMath::Cosf(radian), DexMath::Sinf(radian), 0,
		0, -DexMath::Sinf(radian), DexMath::Cosf(radian), 0,
		0, 0, 0, 1,
	};
	DexMatrix4x4T temp(value);
	*this = *this * temp;
	return *this;
}
template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::RotateY(float32 radian)
{
	//float32 old_11 = _11;
	//float32 old_13 = _13;
	//float32 a = radian;
	//_11 =  old_11*cos(a) + old_13*sin(a);
	//_13 = -old_11*sin(a) + old_13*cos(a);

	//float32 old_21 = _21;
	//float32 old_23 = _23;
	//_21 =  old_21*cos(a) + old_23*sin(a);
	//_23 = -old_21*sin(a) + old_23*cos(a);

	//float32 old_31 = _31;
	//float32 old_33 = _33;
	//_31 =  old_31*cos(a) + old_33*sin(a);
	//_33 = -old_31*sin(a) + old_33*cos(a);

	//float32 old_41 = _41;
	//float32 old_43 = _43;
	//_41 =  old_41*cos(a) + old_43*sin(a);
	//_43 = -old_41*sin(a) + old_43*cos(a);
	T value[16] =
	{
		DexMath::Cosf(radian), 0, -DexMath::Sinf(radian), 0,
		0, 1, 0, 0,
		DexMath::Sinf(radian), 0, DexMath::Cosf(radian), 0,
		0, 0, 0, 1,
	};
	DexMatrix4x4T temp(value);
	*this = *this * temp;
	return *this;
}

template<typename T>
inline DexMatrix4x4T<T> DexMatrix4x4T<T>::RotateZ(float32 radian)
{
	//float32 old_11 = _11;
	//float32 old_12 = _12;
	//float32 a = radian;
	//_11 =  old_11*cos(a) - old_12*sin(a);
	//_12 =  old_11*sin(a) + old_12*cos(a);

	//float32 old_21 = _21;
	//float32 old_22 = _22;
	//_21 =  old_21*cos(a) - old_22*sin(a);
	//_22 =  old_21*sin(a) + old_22*cos(a);

	//float32 old_31 = _31;
	//float32 old_32 = _32;
	//_31 =  old_31*cos(a) - old_32*sin(a);
	//_32 =  old_31*sin(a) + old_32*cos(a);

	//float32 old_41 = _41;
	//float32 old_42 = _42;
	//_41 =  old_41*cos(a) - old_42*sin(a);
	//_42 =  old_41*sin(a) + old_42*cos(a);
	T value[16] =
	{
		DexMath::Cosf(radian), DexMath::Sinf(radian), 0, 0,
		-DexMath::Sinf(radian), DexMath::Cosf(radian), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	DexMatrix4x4T temp(value);
	*this = *this * temp;
	return *this;
}
template<typename T1, typename T>
DexMatrix4x4T<T> operator* (T1 _value, const DexMatrix4x4T<T>& matrix)
{
	DexMatrix4x4T<T> ret;
	ret.m[0] = (T)(matrix.m[0] * _value);
	ret.m[1] = (T)(matrix.m[1] * _value);
	ret.m[2] = (T)(matrix.m[2] * _value);
	ret.m[3] = (T)(matrix.m[3] * _value);
	ret.m[4] = (T)(matrix.m[4] * _value);
	ret.m[5] = (T)(matrix.m[5] * _value);
	ret.m[6] = (T)(matrix.m[6] * _value);
	ret.m[7] = (T)(matrix.m[7] * _value);
	ret.m[8] = (T)(matrix.m[8] * _value);
	ret.m[9] = (T)(matrix.m[9] * _value);
	ret.m[10] = (T)(matrix.m[10] * _value);
	ret.m[11] = (T)(matrix.m[11] * _value);
	ret.m[12] = (T)(matrix.m[12] * _value);
	ret.m[13] = (T)(matrix.m[13] * _value);
	ret.m[14] = (T)(matrix.m[14] * _value);
	ret.m[15] = (T)(matrix.m[15] * _value);
	return ret;
}
template<typename T1, typename T>
DexVector3T<T1> operator* (const DexVector3T<T1>& vector, const DexMatrix4x4T<T>& matrix)
{
	DexVector3T<T1> ret;
	ret.x = (T1)(vector.x * matrix._11 + vector.y * matrix._21 + vector.z * matrix._31 + 1 * matrix._41);
	ret.y = (T1)(vector.x * matrix._12 + vector.y * matrix._22 + vector.z * matrix._32 + 1 * matrix._42);
	ret.z = (T1)(vector.x * matrix._13 + vector.y * matrix._23 + vector.z * matrix._33 + 1 * matrix._43);
	return ret;
}
typedef DexMatrix4x4T<float32>  DexMatrix4x4;

#endif