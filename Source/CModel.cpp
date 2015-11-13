

#include "CModel.h"

CModel::CModel()
{
	m_device             = NULL;
	D3DXMatrixIdentity(&m_Matrix);
	m_posX = m_posY = m_posZ = 0;
	m_desPosX = m_desPosY = m_desPosZ = 0.0f;
	m_scaleX = m_scaleY = m_scaleZ = 1.0f;
	m_rotateX = m_rotateY = m_rotateZ = 0.0f;
	m_VelX = 0.0f;
	m_VelY = 0.0f;
	m_VelZ = 0.0f;
	m_id = 0;
	m_direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_iniDir = m_direction; 
	m_lastDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_synDir = true;
	m_renderSphere = true;
	m_moving = true;
	m_collide = true;
	m_cullNone = false;
#ifdef DEX_DEBGU
	m_sphere = NULL;
#endif
}
void CModel::RenderShpere()
{
#ifdef DEX_DEBGU
	if(m_device == NULL)
		return;
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, m_ball.m_center.x, m_ball.m_center.y, m_ball.m_center.z);
	m_device->SetTransform(D3DTS_WORLD, &translation);
	if(m_sphere != NULL)
		m_sphere->DrawSubset(0);
#endif
}
void CModel::CalCulateMatrix()
{
	//先平移到原点，然后旋转，缩放，再平移到原来位置
	D3DXMATRIX rotate;
	D3DXMATRIX trans;
	D3DXMATRIX scale;
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&trans);
	D3DXMatrixIdentity(&scale);
	//在原点的矩阵旋转
	D3DXMATRIX rotateX;
	D3DXMATRIX rotateY;
	D3DXMATRIX rotateZ;
	D3DXMatrixIdentity(&rotateX);
	D3DXMatrixIdentity(&rotateY);
	D3DXMatrixIdentity(&rotateZ);
	D3DXMatrixRotationX(&rotateX, m_rotateX);
	D3DXMatrixRotationY(&rotateY, m_rotateY);
	D3DXMatrixRotationZ(&rotateZ, m_rotateZ);
	D3DXMatrixMultiply(&rotate, &rotateY, &rotateX);	
	D3DXMatrixMultiply(&rotate, &rotate, &rotateZ);	
	//平移到原点
	D3DXMatrixTranslation(&trans, 0, 0, 0);	 
	//旋转
	D3DXMatrixMultiply(&m_Matrix, &rotate, &trans);	 
	//相乘
	D3DXMatrixScaling(&scale, m_scaleX, m_scaleY, m_scaleZ);
	//乘上缩放矩阵  
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &scale); 

	//最后平移到物体位置
	D3DXMatrixTranslation(&trans, m_posX, m_posY, m_posZ);
	//D3DXMatrixMultiply(&m_Matrix, &rotate, &trans);	
	D3DXMatrixMultiply(&m_Matrix, &m_Matrix, &trans);	
}


bool CModel::CheckPick(stRay ray )
{
	if(GetBall().GetRelation(ray) == 0)
		return true;
	return false;
}

void CModel::SetCullNone(bool b)
{
	m_cullNone = b;
}
bool CModel::GetCullNone()
{
	return m_cullNone;
}
void CModel::SetPositionX(float xPos)
{
	m_posX = xPos;
	CalCulateMatrix();
}
void CModel::SetPositionY(float yPos)
{
	m_posY = yPos;
	CalCulateMatrix();
}
void CModel::SetPositionZ(float zPos)
{
	m_posZ = zPos;
	CalCulateMatrix();
}
void CModel::SetPosition(float xPos, float yPos, float zPos)
{
	m_posX = xPos;
	m_posY = yPos;
	m_posZ = zPos;
	m_ball.m_center.x = m_referBall.m_center.x + m_posX;   //中心要实时移动
	m_ball.m_center.y = m_referBall.m_center.y + m_posY;
	m_ball.m_center.z = m_referBall.m_center.z + m_posZ;
	CalCulateMatrix();
}

void CModel::SetPosition(D3DXVECTOR3 pos)
{
	m_posX = pos.x;
	m_posY = pos.y;
	m_posZ = pos.z;
	m_ball.m_center.x = m_referBall.m_center.x + m_posX;   //中心要实时移动
	m_ball.m_center.y = m_referBall.m_center.y + m_posY;
	m_ball.m_center.z = m_referBall.m_center.z + m_posZ;
	CalCulateMatrix();
}


void CModel::Move(D3DXVECTOR3 vec, float length)
{
	D3DXVec3Normalize(&vec, &vec);
	m_posX +=  vec.x*length;
	m_posY +=  vec.y*length;
	m_posZ +=  vec.z*length;

	m_ball.m_center.x = m_referBall.m_center.x + m_posX;   //中心要实时移动
	m_ball.m_center.y = m_referBall.m_center.y + m_posY;
	m_ball.m_center.z = m_referBall.m_center.z + m_posZ;

	SetPosition(m_posX, m_posY, m_posZ);
}
void CModel::SetScaleX(float xScale)
{
	m_scaleX = xScale;
	CalCulateMatrix();
}
void CModel::SetScaleY(float yScale)
{
	m_scaleY = yScale;
	CalCulateMatrix();
}
void CModel::SetScaleZ(float zScale)
{
	m_scaleZ = zScale;
	CalCulateMatrix();
}
void CModel::SetScale(float xScale, float yScale, float zScale)
{
	m_scaleX = xScale;
	m_scaleY = yScale;
	m_scaleZ = zScale;
	CalCulateMatrix();
}
void CModel::GetScale(float& xScale, float& yScale, float&zScale)
{
	xScale = m_scaleX;
	yScale = m_scaleY;
	zScale = m_scaleZ;
}
void CModel::SetScale(float scale)
{
	m_scaleX = scale;
	m_scaleY = scale;
	m_scaleZ = scale;
	m_referBall.m_center.y = m_iniBall.m_center.y + (scale-1)* m_iniBall.m_radius;   //中心要随着缩放而上移
	if(m_referBall.m_center.y < 0 )
		m_referBall.m_center.y = 0;

	m_referBall.m_radius   = m_iniBall.m_radius * scale;
	m_ball = m_referBall;
#ifdef DEX_DEBGU
	_SafeRelease(m_sphere);
	D3DXCreateSphere(m_device, m_ball.m_radius, 20, 20, &m_sphere, NULL);
#endif
	CalCulateMatrix();
}
void CModel::SetRotateX(float xRotate)
{
	m_rotateX = xRotate;
	CalCulateMatrix();
}
void CModel::SetRotateY(float yRotate)
{
	m_rotateY = yRotate;
	CalCulateMatrix();
}
void CModel::SetRotateZ(float zRotate)
{
	m_rotateZ = zRotate;
	CalCulateMatrix();
}
void CModel::SetRotate(float xRotate, float yRotate, float zRotate)
{
	m_rotateX = xRotate;
	m_rotateY = yRotate;
	m_rotateZ = zRotate;
	CalCulateMatrix();
}

float CModel::GetRotateY()
{
	return m_rotateY;
}

void CModel::SetVeclX(float velX)
{
	m_VelX = velX;
}

void CModel::SetVeclY(float velY)
{
	m_VelY = velY;
}

void CModel::SetVeclZ(float velZ)
{
	m_VelZ = velZ;
}

void CModel::SetVel(float velX, float velY, float velZ)
{
	m_VelX = velX;
	m_VelY = velY;
	m_VelZ = velZ;
}

void CModel::SetVel(D3DXVECTOR3 vel)
{
	m_VelX = vel.x;
	m_VelY = vel.y;
	m_VelZ = vel.z;
}

void CModel::SetVel(float vel)
{
	m_vel = vel;
	GetDirection();
	//求出分速度
	m_VelX = vel * m_direction.x;
	m_VelY = vel * m_direction.y;
	m_VelZ = vel * m_direction.z;
}
void CModel::IniDirection(D3DXVECTOR3 dir)
{
	dir.y = 0;
	D3DXVec3Normalize(&dir, &dir);	
	m_iniDir = m_direction = dir;
}
void CModel::IniDirection(float x, float y, float z)
{
	D3DXVECTOR3 dir(x, 0, z);
	D3DXVec3Normalize(&dir, &dir);	
	m_iniDir = m_direction = dir;
}
void CModel::SetDirection(D3DXVECTOR3 dir)
{
	dir.y = 0;
	m_direction = dir;
	D3DXVec3Normalize(&m_direction, &m_direction);	

	float dot =D3DXVec3Dot(&m_iniDir, &m_direction);
	float cos_radius = dot;  //因为两个皆是单位向量，故除模省去
	float angle_minus =	acos(cos_radius);
	if(DecideISClock(m_iniDir, m_direction))
		SetRotateY(angle_minus);   
	else
		SetRotateY(-angle_minus);   
	m_lastDir = m_direction;  
	SetVel(m_vel);

}
void CModel::SetDirection(float x, float y, float z)
{
	m_direction.x = x;
	m_direction.y = 0;
	m_direction.z = z;
	D3DXVec3Normalize(&m_direction, &m_direction);	

	float dot =D3DXVec3Dot(&m_iniDir, &m_direction);
	float cos_radius = dot;  //因为两个皆是单位向量，故除模省去
	float angle_minus =	acos(cos_radius);
	if(DecideISClock(m_iniDir, m_direction))
		SetRotateY(angle_minus);   
	else
		SetRotateY(-angle_minus);   
	m_lastDir = m_direction;  
	SetVel(m_vel);

}

void CModel::SetBallScale(float scale)
{
	if(scale <= 0)
		return;
	m_referBall.m_radius *= scale;
	m_ball.m_radius *= scale;
#ifdef DEX_DEBGU
	_SafeRelease(m_sphere);
	D3DXCreateSphere(m_device, m_ball.m_radius, 20, 20, &m_sphere, NULL);
#endif
}

D3DXVECTOR3 CModel::GetBallTop()
{
	D3DXVECTOR3 p = m_ball.m_center;
	p.y += m_ball.m_radius;
	return p;
}

void CModel::SetCollide(bool collide)
{
	m_collide = collide;
}

bool CModel::GetCollide()
{
	return m_collide;
}

void CModel::SetId(int _id)
{
	m_id = _id;
}

int CModel::GetId()
{
	return m_id;
}

stBall CModel::GetBall()
{
	return m_ball;
}
D3DXVECTOR3 CModel::GetPosition()  const
{
	D3DXVECTOR3 pos;
	pos.x = m_posX;
	pos.y = m_posY;
	pos.z = m_posZ;
	return pos;
}

D3DXVECTOR3 CModel::GetDirection()
{
	D3DXVec3Normalize(&m_direction, &m_direction);
	return m_direction;
}

float CModel::GetVelX()  { return m_VelX;};
float CModel::GetVelY()  { return m_VelY;};
float CModel::GetVelZ()  { return m_VelZ;};
D3DXVECTOR3 CModel::GetVel()
{
	D3DXVECTOR3 vel;
	vel.x = m_VelX ;
	vel.y = m_VelY ;
	vel.z = m_VelZ ;
	return vel;
}

void CModel::FaceTo(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 vec;
	vec.x = pos.x - m_posX;
	vec.y = pos.y - m_posY;
	vec.z = pos.z - m_posZ;
	SetDirection(vec);
}

void CModel::FaceTo(float x, float y, float z)
{
	D3DXVECTOR3 vec;
	vec.x = x - m_posX;
	vec.y = y - m_posY;
	vec.z = z - m_posZ;
	SetDirection(vec);
}