

#include "DexCollideObject.h"
#include "../state/DexGameEngine.h"


DexCollideObject::DexCollideObject()
{
	m_pos = D3DXVECTOR3(0,0,0);
}

DexCollideObject::~DexCollideObject()
{
}

 void DexCollideObject::SetObjectMatrix(const D3DXMATRIX& matrix)
 {
	 m_pos.x = matrix._41;
	 m_pos.y = matrix._42;
	 m_pos.z = matrix._43;
 }

 bool DexCollideObject::ArchiveIn(DexMem& mem)
 {
	 DEX_ENSURE_B(DexObject::ArchiveIn(mem));
	 //mem>>m_pos;
	 return true;
 }

 bool DexCollideObject::ArchiveOut(DexMem& mem) const
 {
	 DEX_ENSURE_B(DexObject::ArchiveOut(mem));
	//mem<<m_pos;
	 return true;
 }

//=============DexCollideObjectBox======================================================
DexCollideObjectBox::DexCollideObjectBox()
{
	m_box = new CBox;
}

bool DexCollideObjectBox::Initialize(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	m_pos = pos;
	m_box->Initialize(DexVector3(pos.x, pos.y, pos.y), DexVector3(size.x, size.y, size.y));
	return true;
}

void DexCollideObjectBox::Reset()
{
	Super::Reset();
}

DexCollideObjectBox::~DexCollideObjectBox()
{
	_SafeDelete(m_box);
}

void DexCollideObjectBox::RenderMesh()
{
	DexGameEngine::getEngine()->SetMeshColor(DexColor(1.0f, 0.0f, 0.0f, 0.5f));
	DexGameEngine::getEngine()->RenderCube(m_box->GetCenter(), m_box->getSize());
}

bool DexCollideObjectBox::CheckCollide(const DexCollideObject* object)
{
	DEX_ENSURE_B(object);
	if(object->getType() == DexCollideObjectBox::getClassType())
	{
		return m_box->CheckCollide(((DexCollideObjectBox*)object)->getBox());
	}
	else if(object->getType() == DexCollideObjectSphere::getClassType())
	{
		return m_box->CheckCollide(((DexCollideObjectSphere*)object)->getBall());
	}
	return false;
}

void DexCollideObjectBox::SetObjectMatrix(const D3DXMATRIX& matrix)
{
	Super::SetObjectMatrix(matrix);
	m_box->setCenter(DexVector3(m_pos.x, m_pos.y, m_pos.z));
	//m_box->setCenter(pos);
}

void DexCollideObjectBox::Move(const D3DXVECTOR3& delta)
{
	D3DXVECTOR3 new_pos = m_pos + delta;
	m_box->setCenter(DexVector3(new_pos.x, new_pos.y, new_pos.z));
}

void DexCollideObjectBox::getCenter(D3DXVECTOR3& center)
{
	center = D3DXVECTOR3(m_box->GetCenter().x, m_box->GetCenter().y, m_box->GetCenter().z);
}


bool DexCollideObjectBox::ArchiveIn(DexMem& mem)
{
	DEX_ENSURE_B(Super::ArchiveIn(mem));
	bool box;
	mem>>box;
	if(box)
	{
		_SafeDelete(m_box);
		DexVector3 box_size;
		DexVector3 box_center;
		mem>>box_center;
		mem>>box_size;
		m_box->Initialize(box_center, box_size, true);
		m_pos = D3DXVECTOR3(box_center.x, box_center.y, box_center.z);
	}
	return true;
}

bool DexCollideObjectBox::ArchiveOut(DexMem& mem) const
{
	DEX_ENSURE_B(Super::ArchiveOut(mem));
	if(m_box != NULL)
	{
		mem<<(bool)true;	
		DexVector3 box_size = m_box->getSize();
		DexVector3 box_center = m_box->GetCenter();
		mem<<box_center;
		mem<<box_size;
	}
	else
	{
		mem<<(bool)false;
	}
	return true;
}

CBox* DexCollideObjectBox::getBox()
{
	return m_box;
}

//=============DexCollideObjectSphere======================================================
DexCollideObjectSphere::DexCollideObjectSphere()
{
}

bool DexCollideObjectSphere::Initialize(const D3DXVECTOR3& pos, float radius)
{
	m_pos = pos;
	memcpy(&m_ball.m_center, &pos, sizeof(D3DXVECTOR3));
	m_ball.m_radius = radius;
	return true;
}

DexCollideObjectSphere::~DexCollideObjectSphere()
{
}

void DexCollideObjectSphere::RenderMesh()
{
	DexGameEngine::getEngine()->SetMeshColor(DexColor(1.0f, 0.0f, 0.0f, 0.5f));
	DexGameEngine::getEngine()->RenderSphere(m_ball.m_center, m_ball.m_radius);
}

bool DexCollideObjectSphere::CheckCollide(const DexCollideObject* object)
{
	DEX_ENSURE_B(object);
	if(object->getType() == DexCollideObjectBox::getClassType())
	{
		return ((DexCollideObjectBox*)object)->getBox()->CheckCollide(m_ball);
	}
	else if(object->getType() == DexCollideObjectSphere::getClassType())
	{
		return m_ball.GetRelation(((DexCollideObjectSphere*)object)->getBall());
	}
	return false;
}

void DexCollideObjectSphere::SetObjectMatrix(const D3DXMATRIX& matrix)
{
	Super::SetObjectMatrix(matrix);
	memcpy(&m_ball.m_center, &m_pos, sizeof(m_pos));
}

void DexCollideObjectSphere::Move(const D3DXVECTOR3& delta)
{
	D3DXVECTOR3 new_pos = m_pos + delta;
	memcpy(&m_ball.m_center, &new_pos, sizeof(new_pos));
}

void DexCollideObjectSphere::getCenter(D3DXVECTOR3& center)
{
	memcpy(&center, &m_ball.m_center, sizeof(D3DXVECTOR3));
}

stBall DexCollideObjectSphere::getBall()
{
	return m_ball;
}

bool DexCollideObjectSphere::ArchiveIn(DexMem& mem)
{
	DEX_ENSURE_B(Super::ArchiveIn(mem));
	mem>>m_ball;
	memcpy(&m_pos, &m_ball.m_center, sizeof(m_pos));
	return true;
}

bool DexCollideObjectSphere::checkPick(const stRay& ray)
{
	if(m_ball.GetRelation(ray) == 0)
		return true;
	return false;
}

bool DexCollideObjectSphere::ArchiveOut(DexMem& mem) const
{
	DEX_ENSURE_B(Super::ArchiveOut(mem));
	mem<<m_ball;
	return true;
}