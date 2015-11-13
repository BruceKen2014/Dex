

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
	 DEX_ENSURE_B(CDexObject::ArchiveIn(mem));
	 //mem>>m_pos;
	 return true;
 }

 bool DexCollideObject::ArchiveOut(DexMem& mem) const
 {
	 DEX_ENSURE_B(CDexObject::ArchiveOut(mem));
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
	m_box->Initialize(pos, size);
	return true;
}

void DexCollideObjectBox::Reset()
{
	DexCollideObject::Reset();
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
	DexCollideObject::SetObjectMatrix(matrix);
	m_box->setCenter(m_pos);
	//m_box->setCenter(pos);
}

void DexCollideObjectBox::Move(const D3DXVECTOR3& delta)
{
	D3DXVECTOR3 new_pos = m_pos + delta;
	m_box->setCenter(new_pos);
}

void DexCollideObjectBox::getCenter(D3DXVECTOR3& center)
{
	center = m_box->GetCenter();
}


bool DexCollideObjectBox::ArchiveIn(DexMem& mem)
{
	DEX_ENSURE_B(DexCollideObject::ArchiveIn(mem));
	bool box;
	mem>>box;
	if(box)
	{
		_SafeDelete(m_box);
		D3DXVECTOR3 box_size;
		D3DXVECTOR3 box_center;
		mem>>box_center;
		mem>>box_size;
		m_box->Initialize(box_center, box_size, true);
		m_pos = box_center;
	}
	return true;
}

bool DexCollideObjectBox::ArchiveOut(DexMem& mem) const
{
	DEX_ENSURE_B(DexCollideObject::ArchiveOut(mem));
	if(m_box != NULL)
	{
		mem<<(bool)true;	
		D3DXVECTOR3 box_size = m_box->getSize();
		D3DXVECTOR3 box_center = m_box->GetCenter();
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
	m_ball.m_center = pos;
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
	DexCollideObject::SetObjectMatrix(matrix);
	m_ball.m_center = m_pos;
}

void DexCollideObjectSphere::Move(const D3DXVECTOR3& delta)
{
	D3DXVECTOR3 new_pos = m_pos + delta;
	m_ball.m_center = new_pos;
}

void DexCollideObjectSphere::getCenter(D3DXVECTOR3& center)
{
	center = m_ball.m_center;
}

stBall DexCollideObjectSphere::getBall()
{
	return m_ball;
}

bool DexCollideObjectSphere::ArchiveIn(DexMem& mem)
{
	DEX_ENSURE_B(DexCollideObject::ArchiveIn(mem));
	mem>>m_ball;
	m_pos = m_ball.m_center;
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
	DEX_ENSURE_B(DexCollideObject::ArchiveOut(mem));
	mem<<m_ball;
	return true;
}