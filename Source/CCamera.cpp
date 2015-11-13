/*
Demo Name:  Camera Movements
Author:  Allen Sherrod
Chapter:  Ch 15
*/


#include"CCamera.h"
#include<stdio.h>
//#include<d3dx9math.h>


//=====================CCamera protected 方法==========================
void CCamera::Rotation(D3DXMATRIX matrix)
{
	D3DXVec3TransformNormal(&m_up,&m_up,&matrix);
	D3DXVec3TransformNormal(&m_view, &m_view, &matrix);
	D3DXVec3TransformNormal(&m_right, &m_right, &matrix);

	D3DXVec3TransformCoord(&m_pos, &m_pos, &matrix);
	D3DXVec3TransformCoord(&m_focusPoint, &m_focusPoint, &matrix);
}

//=====================CCamera public 方法=============================

CCamera::CCamera()
{
	m_destPos = D3DXVECTOR3(0,0,0);
	m_moveDir = D3DXVECTOR3(0,0,0);
	m_isMoving = false;
	m_moveFocus = false;
	m_handmove = false;  //默认自动移动时关闭手动移动功能 
	m_moveVel = 0.0f;
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

	D3DXVec3Normalize(&m_view, &m_view);
	D3DXVec3Cross(&m_right, &m_up,&m_view);
	//单位化
	D3DXVec3Normalize(&m_right, &m_right);

	m_fFocusDistance = D3DXVec3Length(&(m_focusPoint - m_pos)); 


	//char ch[64];
	//sprintf(ch, "焦距为%3.1f",m_fFocusDistance);
	//MessageBox(NULL, ch, TEXT(""), MB_OK);
}

D3DXVECTOR3 CCamera::GetView()
{
	m_view = m_focusPoint - m_pos;
	D3DXVec3Normalize(&m_view, &m_view);
	return m_view;
}

float CCamera::GetFocusDis()
{
	m_fFocusDistance = D3DXVec3Length(&(m_pos - m_focusPoint));
	return m_fFocusDistance;
}

void CCamera::CalFrustum()
{
	D3DXVec3Normalize(&m_view, &m_view);
	D3DXVECTOR3 near_point = m_pos + m_view * m_nearDistance;
	D3DXVECTOR3 far_point = m_pos + m_view* m_farDistance;
	m_frustum.SetPlane(FT_FRONT, -m_view, far_point);
	m_frustum.SetPlane(FT_BACK, m_view, near_point);
	D3DXVec3Cross(&m_right, &m_up, &m_view);
	float nnh = m_nearDistance * tan(m_fov/2);
	D3DXVECTOR3 top = m_view*m_nearDistance + m_up* nnh;
	D3DXVECTOR3 vec;
	D3DXVec3Cross(&vec, &m_right, &top);
	D3DXVec3Normalize(&vec, &vec);
	m_frustum.SetPlane(FT_TOP, vec, m_pos);
	D3DXVECTOR3 bottom = m_view*m_nearDistance - m_up*nnh;
	D3DXVec3Cross(&vec, &bottom, &m_right);
	D3DXVec3Normalize(&vec, &vec);
	m_frustum.SetPlane(FT_BOTTOM, vec, m_pos);
	float nnw = nnh*m_aspect;
	D3DXVECTOR3 right = m_view*m_nearDistance + m_right*nnw;
	D3DXVec3Cross(&vec, &right, &m_up);
	D3DXVec3Normalize(&vec, &vec);
	m_frustum.SetPlane(FT_RIGHT, vec, m_pos);
	D3DXVECTOR3 left = m_view*m_nearDistance - m_right*nnw;
	D3DXVec3Cross(&vec, &m_up, &left);
	D3DXVec3Normalize(&vec, &vec);
	m_frustum.SetPlane(FT_LEFT, vec, m_pos);
}

CFrustum CCamera::GetFrustum()
{
	return m_frustum;
}

void CCamera::SetCamera(D3DXVECTOR3 pos, D3DXVECTOR3 focuspoint, D3DXVECTOR3 upvector)
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
		if(D3DXVec3Length(&(m_pos - m_destPos)) < 2*m_moveVel)
		{//已经到达目的地
			m_destPos = m_pos; //为了消除误差
			m_isMoving =  false;
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


void CCamera::MoveCamera(D3DXVECTOR3 vec, float amount, bool move_focus /* = false */)
{
	if(m_isMoving && !m_handmove)
		return;
	D3DXVec3Normalize(&vec, &vec);
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



void CCamera::MoveFocus(D3DXVECTOR3 vec, float amount)
{
	if(m_isMoving && !m_handmove)
		return;
	D3DXVec3Normalize(&vec, &vec);
	m_focusPoint.x += vec.x * amount;
	m_focusPoint.y += vec.y * amount;
	m_focusPoint.z += vec.z * amount;
}

void CCamera::EnableMove(bool move)
{
	m_isMoving = move;
}

void CCamera::SetDestPos(D3DXVECTOR3 pos)
{
	m_destPos = pos;
	m_moveDir = m_destPos - m_pos;
	D3DXVec3Normalize(&m_moveDir, &m_moveDir);
}

void CCamera::SetDestPos(float x, float y, float z)
{
	m_destPos.x = x;
	m_destPos.y = y;
	m_destPos.z = z;
	m_moveDir = m_destPos - m_pos;
	D3DXVec3Normalize(&m_moveDir, &m_moveDir);
}

D3DXVECTOR3 CCamera::GetDestPos()
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
	D3DXVECTOR3 temp = D3DXVECTOR3(0,1,0);

	switch(direction)					   
	{
	case DIR_VIEW:	  /*绕view轴旋转*/
		D3DXMatrixRotationAxis(&R, &m_view, angle);
		//旋转right和up轴
		D3DXVec3TransformNormal(&m_right,&m_right,&R);
		D3DXVec3TransformNormal(&m_up, &m_up, &R);
		break;
	case DIR_UP:      /*绕up轴旋转*/
		D3DXMatrixRotationAxis(&R, &m_up, angle);
		//旋转right和view轴
		D3DXVec3TransformNormal(&m_right,&m_right,&R);
		D3DXVec3TransformNormal(&m_view, &m_view, &R);
		m_focusPoint = m_pos + m_view * m_fFocusDistance;
		break;
	case DIR_RIGHT:	  /*绕right轴旋转*/
		D3DXMatrixRotationAxis(&R, &m_right, angle);
		//旋转up和view轴
		D3DXVec3TransformNormal(&m_up,&m_up,&R);
		D3DXVec3TransformNormal(&m_view, &m_view, &R);
		m_focusPoint = m_pos + m_view * m_fFocusDistance;
		break;
	default:
		break;
	}
}

void CCamera::RotateUpY(float angle)
{
	D3DXVECTOR3 UpX = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &UpX, angle);

	//旋转up、view、right轴
	D3DXVec3TransformNormal(&m_up, &m_up, &R);
	D3DXVec3TransformNormal(&m_right,&m_right,&R);
	D3DXVec3TransformNormal(&m_view, &m_view, &R);



	//D3DXMatrixRotationAxis(&R, &m_up, angle);
	////旋转right和view轴
	//D3DXVec3TransformNormal(&m_right,&m_right,&R);
	//D3DXVec3TransformNormal(&m_view, &m_view, &R);
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
	D3DXVECTOR3 v = m_pos - m_focusPoint;
	//建立对应fYaw的旋转矩阵
	D3DXMATRIX matYaw;
	D3DXVECTOR3 upY(0.0f,1.0f,0.0f);//Y轴
	D3DXMatrixRotationAxis(&matYaw, &upY, angle);

	D3DXVec3TransformNormal(&v, &v, &matYaw);   
	//D3DXVec3TransformCoord(&m_up,&m_up,&matYaw); 
	//D3DXVec3TransformNormal(&m_view,&m_view,&matYaw); 
	m_view = v;
	D3DXVec3Normalize(&m_view, &m_view);
	m_pos.x=m_focusPoint.x + v.x;
	m_pos.z=m_focusPoint.z + v.z;
	D3DXVec3Cross(&m_right, &m_view,&m_up);
	//单位化
	D3DXVec3Normalize(&m_right, &m_right);
}