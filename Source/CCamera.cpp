/*
Demo Name:  Camera Movements
Author:  Allen Sherrod
Chapter:  Ch 15
*/


#include"CCamera.h"
#include<stdio.h>
#include "..\DexBase\CInput.h"
#include "..\DexMath\DexMatrix.h"
#include "..\DexMath\DexQuaternion.h"
//#include<d3dx9math.h>


//=====================CCamera protected 方法==========================
void CCamera::Rotation(D3DXMATRIX matrix)
{
	//D3DXVECTOR3 up, view, right, pos, focus;
	//memcpy(&up, &m_up, sizeof(D3DXVECTOR3));
	//memcpy(&view, &m_view, sizeof(D3DXVECTOR3));
	//memcpy(&right, &m_right, sizeof(D3DXVECTOR3));
	//memcpy(&pos, &m_pos, sizeof(D3DXVECTOR3));
	//memcpy(&focus, &m_focusPoint, sizeof(D3DXVECTOR3));
	//D3DXVec3TransformNormal(&up, &up, &matrix);
	//D3DXVec3TransformNormal(&view, &view, &matrix);
	//D3DXVec3TransformNormal(&right, &right, &matrix);
	//D3DXVec3TransformCoord(&pos, &pos, &matrix);
	//D3DXVec3TransformCoord(&focus, &focus, &matrix);

	//memcpy(&m_up, &up, sizeof(D3DXVECTOR3));
	//memcpy(&m_view, &view, sizeof(D3DXVECTOR3));
	//memcpy(&m_right, &right, sizeof(D3DXVECTOR3));
	//memcpy(&m_pos, &pos, sizeof(D3DXVECTOR3));
	//memcpy(&m_focusPoint, &focus, sizeof(D3DXVECTOR3));
	DexMatrix4x4 dexMatrix((float32*)&matrix);
	DexMatrix4x4::TransformNormal(m_up, dexMatrix);
	DexMatrix4x4::TransformNormal(m_view, dexMatrix);
	DexMatrix4x4::TransformNormal(m_right, dexMatrix);
	DexMatrix4x4::TransformCoord(m_pos, dexMatrix);
	DexMatrix4x4::TransformCoord(m_focusPoint, dexMatrix);
}

//=====================CCamera public 方法=============================

CCamera::CCamera()
{
	m_destPos = DexVector3(0, 0, 0);
	m_moveDir = DexVector3(0, 0, 0);
	m_isMoving = false;
	m_moveFocus = false;
	m_handmove = false;  //默认自动移动时关闭手动移动功能 
	m_moveVel = 0.0f;
	m_fMayaFlag = 30.0f;
}
void CCamera::SetCamera(float x, float y, float z,
						float xv, float yv, float zv,
						float xu, float yu, float zu)
{
	m_pos.x = x; m_pos.y = y; m_pos.z = z;
	m_destPos = m_pos;
	m_focusPoint.x = xv; m_focusPoint.y = yv; m_focusPoint.z = zv;
	m_up.x = xu; m_up.y = yu; m_up.z = zu;
	m_view = m_focusPoint - m_pos;
	m_view.Normalize();
	m_right = m_up.Cross(m_view);
	//单位化
	m_right.Normalize();

	m_fFocusDistance = (m_focusPoint - m_pos).Length(); 


	//char ch[64];
	//sprintf(ch, "焦距为%3.1f",m_fFocusDistance);
	//MessageBox(NULL, ch, TEXT(""), MB_OK);
}

DexVector3 CCamera::GetView()
{
	m_view = m_focusPoint - m_pos;
	m_view.Normalize();
	return m_view;
}

float CCamera::GetFocusDis()
{
	m_fFocusDistance = (m_pos - m_focusPoint).Length();
	return m_fFocusDistance;
}

void CCamera::CalFrustum()
{
	m_view.Normalize();
	DexVector3 near_point = m_pos + m_view * m_nearDistance;
	DexVector3 far_point = m_pos + m_view* m_farDistance;
	m_frustum.SetPlane(FT_FRONT, m_view * -1.0f, far_point);
	m_frustum.SetPlane(FT_BACK, m_view, near_point);
	m_right = m_up.Cross(m_view);
	float nnh = m_nearDistance * tan(m_fov/2);
	DexVector3 top = m_view*m_nearDistance + m_up* nnh;
	DexVector3 vec = m_right.Cross(top);
	vec.Normalize();
	m_frustum.SetPlane(FT_TOP, vec, m_pos);
	DexVector3 bottom = m_view*m_nearDistance - m_up*nnh;
	vec = bottom.Cross(m_right);
	vec.Normalize();
	m_frustum.SetPlane(FT_BOTTOM, vec, m_pos);
	float nnw = nnh*m_aspect;
	DexVector3 right = m_view*m_nearDistance + m_right*nnw;
	vec = right.Cross(m_up);
	vec.Normalize();
	m_frustum.SetPlane(FT_RIGHT, vec, m_pos);
	DexVector3 left = m_view*m_nearDistance - m_right*nnw;
	vec = m_up.Cross(left);
	vec.Normalize();
	m_frustum.SetPlane(FT_LEFT, vec, m_pos);
}

CFrustum CCamera::GetFrustum()
{
	return m_frustum;
}

DexVector3 CCamera::GetHeadDir()
{
	DexVector3 ret = m_view.Cross(m_right);
	ret.Normalize();
	return ret;
}

void CCamera::SetCamera(DexVector3 pos, DexVector3 focuspoint, DexVector3 upvector)
{
	SetCamera(pos.x, pos.y, pos.z, focuspoint.x, focuspoint.y, focuspoint.z, upvector.x, upvector.y, upvector.z);
}

void CCamera::SetPosition(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

void CCamera::SetFocus(float x, float y, float z)
{
	m_focusPoint.x = x;
	m_focusPoint.y = y;
	m_focusPoint.z = z;
}

bool CCamera::Update(int delta)
{
	DEX_ENSURE_B(CDexSceneObject::Update(delta));
	if(m_isMoving)
	{
		m_pos += m_moveVel*m_moveDir;
		if(m_moveFocus)
		{
			m_focusPoint += m_moveVel*m_moveDir;
		}
		if((m_pos - m_destPos).Length() < 2*m_moveVel)
		{//已经到达目的地
			m_destPos = m_pos; //为了消除误差
			m_isMoving =  false;
		}
	}
	if (CInputSystem::GetInputSingleton().WheelFront())
	{
		MoveCamera(m_view, 4.0f, false);
	}
	if (CInputSystem::GetInputSingleton().WheelBack())
	{
		MoveCamera(m_view, -4.0f, false);
	}
	if (CInputSystem::GetInputSingleton().KeyDown(DIK_LALT))
	{
		if (CInputSystem::GetInputSingleton().LeftButtonDown())
		{
			float fRotate = CInputSystem::GetInputSingleton().MouseDeltaX();
			if (fRotate != 0)
			{
				fRotate /= 30.0f;

				DexMatrix4x4::Rotate(m_pos, m_focusPoint, DexVector3(0.0f, 1.0f, 0.0f), -fRotate);
				//DexQuaternion quatenion(DexVector3(0.0f, 1.0f, 0.0f), -fRotate);
				//DexMatrix4x4 matrix;
				//matrix.Translate(m_focusPoint);
				//matrix *= quatenion.GetMatrix();
				//DexMatrix4x4::TransformCoord(m_pos, matrix); //这个算法好像有Bug
				m_view = m_focusPoint - m_pos;
				m_view.Normalize();
				m_right = m_up.Cross(m_view);
				m_right.Normalize();
			}
			fRotate = CInputSystem::GetInputSingleton().MouseDeltaY();
			if (fRotate != 0)
			{
				fRotate /= 30.0f;
				DexMatrix4x4::Rotate(m_pos, m_focusPoint, m_right, -fRotate);
				//DexQuaternion quatenion(m_right, -fRotate);
			//	DexMatrix4x4 matrix;
				//matrix.Translate(m_focusPoint);
				//matrix *= quatenion.GetMatrix();
				//DexMatrix4x4::TransformCoord(m_pos, matrix);
				m_view = m_focusPoint - m_pos;
				m_view.Normalize();
				m_right = m_up.Cross(m_view);

				m_right.Normalize();
			}

		}
		if (CInputSystem::GetInputSingleton().MiddleButtonDown())
		{
			float fMove =  CInputSystem::GetInputSingleton().MouseDeltaY();
			if (fMove != 0)
			{
				DexVector3 headDir = GetHeadDir();
				m_focusPoint += headDir * fMove;
				m_pos += headDir * fMove;
			}
			fMove = CInputSystem::GetInputSingleton().MouseDeltaX();
			if (fMove != 0)
			{
				m_focusPoint += m_right * -fMove;
				m_pos += m_right * -fMove;
			}

		}
	}
	return true;
}

void CCamera::SetProject(float fov, float aspect, float near_distance, float far_distance)
{
	m_fov = fov;
	m_aspect = aspect;
	m_nearDistance = near_distance;
	m_farDistance = far_distance;
}

float CCamera::GetFov()
{
	return m_fov;
}

float CCamera::GetAspect()
{
	return m_aspect;
}

float CCamera::GetNearDis()
{
	return m_nearDistance;
}

float CCamera::GetFarDis()
{
	return m_farDistance;
}



void CCamera::MoveCamera(MOVE_DIRECTION direction, float amount, bool move_focus)
{	
	if(m_isMoving && !m_handmove)
		return;
	switch(direction)
	{
	case DIR_VIEW:	     //前后移动
		{
			m_pos += m_view * amount;
			break;
		}
	case DIR_UP:	    //上下移动
		{
			m_pos += m_up * amount;
		    break;
		}
	case DIR_RIGHT:	    //左右移动
		{
			m_pos += m_right * amount;
		    break;
		}
	case DIR_X:
		{
			m_pos.x += amount;
			break;
		}
	case DIR_Y:
		{
			m_pos.y += amount;
			break;
		}
	case DIR_Z:
		{
			m_pos.z += amount;
			break;
		}
	default:
		break;
	}
	if(move_focus)
		MoveFocus(direction, amount);
}


void CCamera::MoveCamera(DexVector3 vec, float amount, bool move_focus /* = false */)
{
	if(m_isMoving && !m_handmove)
		return;
	vec.Normalize();
	m_pos.x += vec.x * amount;
	m_pos.y += vec.y * amount;
	m_pos.z += vec.z * amount;
	if(move_focus)
		MoveFocus(vec, amount);
}
void CCamera::MoveFocus(MOVE_DIRECTION direction, float amount)
{
	if(m_isMoving && !m_handmove)
		return;
	switch(direction)
	{
	case DIR_VIEW:	     //前后移动
		m_focusPoint += m_view * amount;
		break;
	case DIR_UP:	    //上下移动
		m_focusPoint += m_up * amount;
		break;
	case DIR_RIGHT:	    //左右移动
		m_focusPoint += m_right * amount;
		break;
	case DIR_X:
		m_focusPoint.x += amount;
		break;
	case DIR_Y:
		m_focusPoint.y += amount;
		break;
	case DIR_Z:
		m_focusPoint.z += amount;
		break;
	default:
		break;
	}
}



void CCamera::MoveFocus(DexVector3 vec, float amount)
{
	if(m_isMoving && !m_handmove)
		return;
	vec.Normalize();
	m_focusPoint.x += vec.x * amount;
	m_focusPoint.y += vec.y * amount;
	m_focusPoint.z += vec.z * amount;
}

void CCamera::EnableMove(bool move)
{
	m_isMoving = move;
}

void CCamera::SetDestPos(DexVector3 pos)
{
	m_destPos = pos;
	m_moveDir = m_destPos - m_pos;
	m_moveDir.Normalize();
}

void CCamera::SetDestPos(float x, float y, float z)
{
	m_destPos.x = x;
	m_destPos.y = y;
	m_destPos.z = z;
	m_moveDir = m_destPos - m_pos;
	m_moveDir.Normalize();
}

DexVector3 CCamera::GetDestPos()
{
	return m_destPos;
}

bool CCamera::GetIsMoving()
{
	return m_isMoving;
}

void CCamera::SetHandMove(bool hand_move)
{
	m_handmove = hand_move;
}

bool CCamera::GetHandMove()
{
	return m_handmove;
}

void CCamera::SetMoveVel(float vel)
{
	m_moveVel = vel;
}

float CCamera::GetMoveVel()
{
	return m_moveVel;
}

void CCamera::SetMoveFocus(bool move_focus)
{
	m_moveFocus = move_focus;
}

bool CCamera::GetMoveFocus()
{
	return m_moveFocus;
}

void CCamera::RotateSelf(MOVE_DIRECTION direction,float angle)
{
	D3DXMATRIX R;
	DexVector3 temp = DexVector3(0, 1, 0);
	D3DXVECTOR3 view, right, up;
	memcpy(&view, &m_view, sizeof(D3DXVECTOR3));
	memcpy(&right, &m_right, sizeof(D3DXVECTOR3));
	memcpy(&up, &m_up, sizeof(D3DXVECTOR3));
	switch(direction)					   
	{
	case DIR_VIEW:	  /*绕view轴旋转*/
		D3DXMatrixRotationAxis(&R, &view, angle);
		//旋转right和up轴
		D3DXVec3TransformNormal(&right, &right, &R);
		D3DXVec3TransformNormal(&up, &up, &R);
		break;
	case DIR_UP:      /*绕up轴旋转*/
		D3DXMatrixRotationAxis(&R, &up, angle);
		//旋转right和view轴
		D3DXVec3TransformNormal(&right, &right, &R);
		D3DXVec3TransformNormal(&view, &view, &R);
		memcpy(&m_view, &view, sizeof(D3DXVECTOR3));
		m_focusPoint = m_pos + m_view * m_fFocusDistance;
		break;
	case DIR_RIGHT:	  /*绕right轴旋转*/
		D3DXMatrixRotationAxis(&R, &right, angle);
		//旋转up和view轴
		D3DXVec3TransformNormal(&up, &up, &R);
		D3DXVec3TransformNormal(&view, &view, &R);
		memcpy(&m_view, &view, sizeof(D3DXVECTOR3));
		m_focusPoint = m_pos + m_view * m_fFocusDistance;
		break;
	default:
		break;
	}
	memcpy(&m_view, &view, sizeof(D3DXVECTOR3));
	memcpy(&m_right, &right, sizeof(D3DXVECTOR3));
	memcpy(&m_up, &up, sizeof(D3DXVECTOR3));
}

void CCamera::RotateUpY(float angle)
{
	D3DXVECTOR3 UpX = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &UpX, angle);
	D3DXVECTOR3 view, right, up;
	memcpy(&view, &m_view, sizeof(D3DXVECTOR3));
	memcpy(&right, &m_right, sizeof(D3DXVECTOR3));
	memcpy(&up, &m_up, sizeof(D3DXVECTOR3));
	//旋转up、view、right轴
	D3DXVec3TransformNormal(&up, &up, &R);
	D3DXVec3TransformNormal(&right, &right, &R);
	D3DXVec3TransformNormal(&view, &view, &R);
	memcpy(&m_view, &view, sizeof(D3DXVECTOR3));
	memcpy(&m_right, &right, sizeof(D3DXVECTOR3));
	memcpy(&m_up, &up, sizeof(D3DXVECTOR3));

	m_focusPoint = m_pos + m_view * m_fFocusDistance;
}

void CCamera::RotateX(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationX(&R,angle);
	Rotation(R);
}

void CCamera::RotateY(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationY(&R,angle);
	Rotation(R);
}

void CCamera::RotateZ(float angle)
{
	D3DXMATRIX R;
	D3DXMatrixRotationZ(&R,angle);
	Rotation(R);
}

void CCamera::RotateFocus(float angle)
{
	//float _amount = _GetDistance(m_pos, point);

	//D3DXMATRIX trans;
	//D3DXMatrixTranslation( &trans, point.x, point.y, point.z); 

	//SetCamera(point, m_focusPoint, m_up);    //先平移到改点
	//RotateY(angle);// 绕自身旋转
	//MoveCamera(DIR_VIEW, -_amount);	  //然后退回等距离

	//利用向量旋转的原理
	D3DXVECTOR3 v;
	memcpy(&v, &(m_pos - m_focusPoint), sizeof(D3DXVECTOR3));
	//建立对应fYaw的旋转矩阵
	D3DXMATRIX matYaw;
	D3DXVECTOR3 upY(0.0f,1.0f,0.0f);//Y轴
	D3DXMatrixRotationAxis(&matYaw, &upY, angle);

	D3DXVec3TransformNormal(&v, &v, &matYaw);   
	//D3DXVec3TransformCoord(&m_up,&m_up,&matYaw); 
	//D3DXVec3TransformNormal(&m_view,&m_view,&matYaw); 
	memcpy(&m_view, &v, sizeof(D3DXVECTOR3));
	m_view.Normalize();
	m_pos.x=m_focusPoint.x + v.x;
	m_pos.z=m_focusPoint.z + v.z;
	m_right = m_view.Cross(m_up);
	//单位化
	m_right.Normalize();
}