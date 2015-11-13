

#include "DexCollideObject.h"
#include "../widget/DexGuiStruct.h"
#include "../state/DexGameEngine.h"
#include "DexSceneObject.h"
#include "Dex3DScene.h"
#include "DexObjectFactory.h"


const D3DXVECTOR3 g_objIniDirection = D3DXVECTOR3(0,0,-1);

CDexSceneObject::CDexSceneObject()
{ 
	m_currPos = D3DXVECTOR3(0,0,0);
	m_currScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_currRotate = D3DXVECTOR3(0,0,0);
	m_direction = D3DXVECTOR3(0,0,-1);
	m_fatherNode = NULL;
	m_pCollideObject = NULL;
	m_pScene = NULL;
	m_bRender = true;
	m_bRenderCollideObject = false;
	m_bRenderRouteLine = false;
	m_bAlphaSort = false;
	m_bCullNone = true;
	m_bCollide = true;
	m_listAttachObjects.clear();
	D3DXMatrixIdentity(&m_localMatrix);
	D3DXMatrixIdentity(&m_finalMatrix);
	D3DXMatrixIdentity(&m_currMatrix);
	D3DXMatrixIdentity(&m_collideLocalMatrix);
	D3DXMatrixIdentity(&g_transMatrix);
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);	 
	D3DXMatrixMultiply(&m_localMatrix, &m_localMatrix, &g_transMatrix);
	D3DXMatrixMultiply(&m_finalMatrix, &m_finalMatrix, &g_transMatrix);
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &g_transMatrix);


//#ifdef _DEBUG
	m_bRenderCollideObject = true;
	m_bRenderNodeCube = true;
//#endif

}

CDexSceneObject::~CDexSceneObject()
{
	foreach(TlistObjectActions, ite, m_listActions)
	{
		_SafeRelease((*ite));
	}
	m_listActions.clear();
	_SafeRelease(m_pCollideObject);
}

void CDexSceneObject::_updateCurrMatrix()
{	
	D3DXMatrixIdentity(&m_currMatrix);
	D3DXMatrixIdentity(&g_transMatrix);
	D3DXMatrixIdentity(&g_scaleMatrix);
	D3DXMatrixIdentity(&g_rotateMatrix);
	D3DXMatrixTranslation(&g_transMatrix, m_currPos.x, m_currPos.y, m_currPos.z);	 
	D3DXMatrixScaling(&g_scaleMatrix, m_currScale.x, m_currScale.y, m_currScale.z);	 

	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &g_scaleMatrix);

	D3DXMatrixRotationX(&g_rotateMatrix, m_currRotate.x);	
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &g_rotateMatrix);
	D3DXMatrixIdentity(&g_rotateMatrix);

	D3DXMatrixRotationY(&g_rotateMatrix, m_currRotate.y);	 
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &g_rotateMatrix);
	D3DXMatrixIdentity(&g_rotateMatrix);

	D3DXMatrixRotationZ(&g_rotateMatrix, m_currRotate.z);	 
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &g_rotateMatrix);
	
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &g_transMatrix);
}

bool CDexSceneObject::Update(int delta)
{
	DEX_ENSURE_B(CDexObject::Update(delta));
	//update matrix
	D3DXMatrixMultiply(&m_finalMatrix, &m_localMatrix , &m_currMatrix);
	D3DXMatrixMultiply(&g_transMatrix, &m_collideLocalMatrix , &m_currMatrix);
	if(m_fatherNode != NULL)
	{
		D3DXMatrixMultiply(&m_finalMatrix, &m_finalMatrix ,m_fatherNode->getFinalMatrix());
		D3DXMatrixMultiply(&g_transMatrix, &g_transMatrix , m_fatherNode->getFinalMatrix());
	}
	//update collideobject position
	if(m_pCollideObject != NULL)
	{
		
		m_pCollideObject->SetObjectMatrix(g_transMatrix);
	}
	//update action
	
	foreach(TlistObjectActions, ite, m_listActions)
	{
		(*ite)->Update(delta);
		if((*ite)->getComplete())
		{
			(*ite)->Release();
			ite = m_listActions.erase(ite);
			if(ite == m_listActions.end())
				break;
		}
	}
	//暂时不将子object进行update，子object独立进行Update
	return true;
}

bool CDexSceneObject::Render()
{
	DEX_ENSURE_B(getVisible());

//#ifdef _DEBUG	
	if(m_bCullNone)
		DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else
		DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	if(DexGameEngine::getEngine()->getRenderNodeCube() && m_bRenderNodeCube)
	{
		DexGameEngine::getEngine()->SetMeshColor(DEXCOLOR_GREEN);
		DexGameEngine::getEngine()->RenderCube(m_finalMatrix);
	}
	if(m_bRenderCollideObject && DexGameEngine::getEngine()->getRendeCollideMesh())
	{
		if(m_pCollideObject != NULL)
		{
			m_pCollideObject->RenderMesh();
		}
	}
	if(DexGameEngine::getEngine()->getRenderActionRouteLine() && m_bRenderRouteLine)
	{
		foreach(TlistObjectActions, ite, m_listActions)
		{
			(*ite)->Render();
		}
	}

//#endif

	return true;

}

void CDexSceneObject::Reset()
{
	CDexObject::Reset();
	{
		foreach(TlistObjectActions, ite, m_listActions)
		{
			_SafeRelease((*ite));
		}
	}

	m_listActions.clear();
	_SafeRelease(m_pCollideObject);
	unBindSceneNode();
	if(m_pScene != NULL)
		m_pScene->unRegisterObject(this);
	m_pScene = NULL;
	{
		foreach(TlistObjects, ite, m_listAttachObjects)
		{
			_SafeRelease((*ite));
		}
	}

	m_currPos = D3DXVECTOR3(0,0,0);
	m_currScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_currRotate = D3DXVECTOR3(0,0,0);
	m_direction = D3DXVECTOR3(0,0,-1);
	m_bRender = true;
	m_bRenderCollideObject = false;
	m_bRenderRouteLine = false;
	m_bAlphaSort = false;
	m_bCullNone = true;
	m_bCollide = true;
	D3DXMatrixIdentity(&m_localMatrix);
	D3DXMatrixIdentity(&m_finalMatrix);
	D3DXMatrixIdentity(&m_currMatrix);
	D3DXMatrixIdentity(&m_collideLocalMatrix);
	D3DXMatrixIdentity(&g_transMatrix);
	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);	 
	D3DXMatrixMultiply(&m_localMatrix, &m_localMatrix, &g_transMatrix);
	D3DXMatrixMultiply(&m_finalMatrix, &m_finalMatrix, &g_transMatrix);
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &g_transMatrix);
	m_bRenderCollideObject = true;
	m_bRenderNodeCube = true;
}

bool CDexSceneObject::getAlphaSort() const
{
	return m_bAlphaSort;
}

void CDexSceneObject::setAlphaSort(bool b)
{
	m_bAlphaSort = b;
}

void CDexSceneObject::leaveScene()
{
	DEX_ENSURE(m_pScene);
	m_pScene->unRegisterObject(this);
}

void CDexSceneObject::enterScene(CDexScene* pScene)
{
	DEX_ENSURE(pScene);
	m_pScene->registerObject(this);
}

void CDexSceneObject::unBindSceneNode()
{
	DEX_ENSURE(m_fatherNode);
	foreach(TlistObjects, ite, m_fatherNode->m_listAttachObjects)
	{
		if((*ite) == this)
		{
			m_fatherNode->m_listAttachObjects.erase(ite);
			return ;
		}
	}
	m_fatherNode = NULL;
}

void CDexSceneObject::bindSceneNode(CDexSceneObject* father)
{
	DEX_ENSURE(father);
	m_fatherNode = father;
	father->m_listAttachObjects.push_back(this);
}

void CDexSceneObject::addAction(DexSceneObjectAction* action)
{
	m_listActions.push_back(action);
	action->bindSceneObject(this);
}

void CDexSceneObject::SetRenderNodeCube(bool render)
{
//#ifdef _DEBUG
	m_bRenderNodeCube = render;
//#endif
}

void CDexSceneObject::setRenderActionRouteLine(bool render)
{
	m_bRenderRouteLine = render;
}

void CDexSceneObject::setLocalMatrix(const D3DXMATRIX& matrix)
{
	m_localMatrix = matrix;
}

void CDexSceneObject::setLocalMatrix(const D3DXVECTOR3& trans,const D3DXVECTOR3& scale, AXIS axis, float rotate_value)
{
	D3DXMATRIX localTransMatrix;
	D3DXMATRIX localRotateMatrix;
	D3DXMATRIX localScaleMatrix;
	D3DXMatrixIdentity(&localTransMatrix);
	D3DXMatrixIdentity(&localRotateMatrix);
	D3DXMatrixIdentity(&localScaleMatrix);
	D3DXMatrixIdentity(&m_localMatrix);
	if(axis == AXIS_X)
	{
		D3DXMatrixRotationX(&localRotateMatrix, rotate_value);
	}
	else if(axis == AXIS_Y)
	{
		D3DXMatrixRotationY(&localRotateMatrix, rotate_value);
	}
	else if(axis == AXIS_Z)
	{
		D3DXMatrixRotationZ(&localRotateMatrix, rotate_value);
	}
	D3DXMatrixScaling(&localScaleMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&localTransMatrix, trans.x, trans.y, trans.z);
	D3DXMatrixMultiply(&m_localMatrix, &m_localMatrix, &localScaleMatrix);
	D3DXMatrixMultiply(&m_localMatrix, &m_localMatrix, &localRotateMatrix);
	D3DXMatrixMultiply(&m_localMatrix, &m_localMatrix, &localTransMatrix);
}

void CDexSceneObject::setCurrMatrix(const D3DXVECTOR3& trans,const D3DXVECTOR3& scale, AXIS axis, float rotate_value)
{
	D3DXMATRIX currTransMatrix;
	D3DXMATRIX currRotateMatrix;
	D3DXMATRIX currScaleMatrix;
	D3DXMatrixIdentity(&currTransMatrix);
	D3DXMatrixIdentity(&currRotateMatrix);
	D3DXMatrixIdentity(&currScaleMatrix);
	D3DXMatrixIdentity(&m_currMatrix);
	if(axis == AXIS_X)
	{
		D3DXMatrixRotationX(&currRotateMatrix, rotate_value);
	}
	else if(axis == AXIS_Y)
	{
		D3DXMatrixRotationY(&currRotateMatrix, rotate_value);
	}
	else if(axis == AXIS_Z)
	{
		D3DXMatrixRotationZ(&currRotateMatrix, rotate_value);
	}
	D3DXMatrixScaling(&currScaleMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&currTransMatrix, trans.x, trans.y, trans.z);
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &currScaleMatrix);
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &currRotateMatrix);
	D3DXMatrixMultiply(&m_currMatrix, &m_currMatrix, &currTransMatrix);
}

void CDexSceneObject::setCollideObjectMatrix(const D3DXMATRIX& matrix)
{
	m_collideLocalMatrix = matrix;
}

void CDexSceneObject::Move(const D3DXVECTOR3& dir, float length)
{
	D3DXVECTOR3 d = dir;
	D3DXVec3Normalize(&d, &d);
	D3DXVECTOR3 pos = m_currPos;
	pos.x +=  d.x*length;
	pos.y +=  d.y*length;
	pos.z +=  d.z*length;
	setPosition(pos);
}

void CDexSceneObject::Move(const D3DXVECTOR3& delta)
{
	D3DXVECTOR3 pos = m_currPos;
	pos += delta;
	setPosition(pos);
}

void CDexSceneObject::FaceTo(const D3DXVECTOR3& pos)
{
	D3DXVECTOR3 vec;
	vec.x = pos.x - getPosition().x;
	vec.y = pos.y - getPosition().y;
	vec.z = pos.z - getPosition().z;
	setDirection(vec);
}

const D3DXMATRIX*  CDexSceneObject::getFinalMatrix() 
{
	return &m_finalMatrix;
}

const D3DXMATRIX*  CDexSceneObject::getLocalMatrix() const
{
	return &m_localMatrix;
}

const D3DXMATRIX*  CDexSceneObject::getCurrMatrix() const
{
	return &m_currMatrix;
}

CDexSceneObject* CDexSceneObject::getFatherNode() const
{
	return m_fatherNode;
}

D3DXMATRIX  CDexSceneObject::getSelfCurrMatrix() const
{
	D3DXMATRIX matrix;
	D3DXMatrixMultiply(&matrix, &m_localMatrix , &m_currMatrix);
	return matrix;
}

D3DXVECTOR3 CDexSceneObject::getPosition()
{
	D3DXMatrixMultiply(&m_finalMatrix, &m_localMatrix , &m_currMatrix);//fix bug 在未update之前調用會用舊的數據
	return D3DXVECTOR3(m_finalMatrix._41, m_finalMatrix._42, m_finalMatrix._43);
}
void CDexSceneObject::setPosition(const D3DXVECTOR3& pos)
{
	m_currPos = pos;
	_updateCurrMatrix();
}

void CDexSceneObject::setPosition(AXIS axis, float value)
{
	if(axis == AXIS_X)
		m_currPos.x = value;
	else if(axis == AXIS_Y)
		m_currPos.y = value;
	else if(axis == AXIS_Z)
		m_currPos.z = value;
	_updateCurrMatrix();
}

void CDexSceneObject::scale(const D3DXVECTOR3& value)
{
	m_currScale = value;
	_updateCurrMatrix();
}

void CDexSceneObject::scale(AXIS axis, float value)
{
	if(axis == AXIS_X)
	{
		m_currScale.x = value;
	}
	else if(axis == AXIS_Y)
	{
		m_currScale.y = value;
	}
	else if(axis == AXIS_Z)
	{
		m_currScale.z = value;
	}
	_updateCurrMatrix();
}

void CDexSceneObject::rotate(AXIS axis, float degree)
{
	rotateRadian(axis, _getRadian(degree));
}

void CDexSceneObject::rotateRadian(AXIS axis, float radian)
{
	if(axis == AXIS_X)
	{
		m_currRotate.x = radian;
	}
	else if(axis == AXIS_Y)
	{
		m_currRotate.y = radian;
	}
	else if(axis == AXIS_Z)
	{
		m_currRotate.z = radian;
	}
	_updateCurrMatrix();
}

void CDexSceneObject::setDirection(const D3DXVECTOR3& dir)
{
	m_direction = dir;
	m_direction.y = 0;
	D3DXVec3Normalize(&m_direction, &m_direction);	

	float dot =D3DXVec3Dot(&g_objIniDirection, &m_direction);
	float cos_radius = dot;  //因为两个皆是单位向量，故除模省去
	float angle_minus =	acos(cos_radius);

	if(DecideISClock(g_objIniDirection, m_direction))
		m_currRotate.y =  angle_minus;   
	else
		m_currRotate.y =  -angle_minus;   
	_updateCurrMatrix();
}


bool CDexSceneObject::checkCollide(CDexSceneObject* node)
{
	DEX_ENSURE_B(node && node->getCollideObject() && node->getCollide());
	DEX_ENSURE_B(m_pCollideObject && m_bCollide);

	return m_pCollideObject->CheckCollide(node->getCollideObject());
}

bool CDexSceneObject::checkPick(const stRay& ray)
{
	DEX_ENSURE_B(m_pCollideObject != NULL);
	return m_pCollideObject->checkPick(ray);
}

bool CDexSceneObject::getVisible()
{
	if(m_fatherNode != NULL)
	{
		DEX_ENSURE_B(m_fatherNode->getVisible());
	}
	return m_bRender;
}

void CDexSceneObject::setCollideObject(DexCollideObject* object)
{
	//删除之前的collideobject
	_SafeRelease(m_pCollideObject);
	m_pCollideObject = object;
}

void CDexSceneObject::setCollide(bool b)
{
	m_bCollide = b;
}

void CDexSceneObject::setCullNone(bool b)
{
	m_bCullNone = b;
}

bool CDexSceneObject::getCollide()
{
	return m_bCollide;
}

DexCollideObject* CDexSceneObject::getCollideObject()
{
	return m_pCollideObject;
}

bool CDexSceneObject::ArchiveIn(DexMem& mem)
{
	DEX_ENSURE_B(CDexObject::ArchiveIn(mem));
	mem>>m_localMatrix;
	mem>>m_currMatrix;
	//mem>>m_finalMatrix;
	bool collide_object;
	mem>>collide_object;
	if(collide_object)
	{
		_SafeDelete(m_pCollideObject);
		mem.ReadString(getDexBuffer());
		m_pCollideObject =(DexCollideObject*)CDexObjectFactroy::getObjectFactory()->queryObject(getDexBuffer());
		m_pCollideObject->ArchiveIn(mem);
	}
	size_t children;
	mem>>children;
	for(size_t i = 0; i < children; ++i)
	{
		CDexSceneObject* object = NULL;
		mem.ReadString(getDexBuffer());
		object =(CDexSceneObject*)CDexObjectFactroy::getObjectFactory()->queryObject(getDexBuffer());
		object->ArchiveIn(mem);
		object->bindSceneNode(this);
	}
	size_t action;
	mem>>action;
	for(size_t i = 0; i < action; ++i)
	{
		DexSceneObjectAction* object = NULL;
		mem.ReadString(getDexBuffer());
		object =(DexSceneObjectAction*)CDexObjectFactroy::getObjectFactory()->queryObject(getDexBuffer());
		object->ArchiveIn(mem);
		object->bindSceneObject(this);
	}
	return true;
}
bool CDexSceneObject::ArchiveOut(DexMem& mem) const
{
	DEX_ENSURE_B(CDexObject::ArchiveOut(mem));
	mem<<m_localMatrix;
	mem<<m_currMatrix;
	//mem<<m_finalMatrix;
	if(m_pCollideObject != NULL)
	{
		mem<<(bool)true;
		setDexBuffer((char*)m_pCollideObject->getType().c_str());
		mem.WriteString(getDexBuffer());
		m_pCollideObject->ArchiveOut(mem);
	}
	else
	{
		mem<<(bool)false;
	}
	//不需要将父节点archive 由父节点自己调用archive
	//attach object
	mem<<m_listAttachObjects.size();
	{
		foreach_const(TlistObjects, ite, m_listAttachObjects)
		{
			setDexBuffer((char*)(*ite)->getType().c_str());
			mem.WriteString(getDexBuffer());
			(*ite)->ArchiveOut(mem);
		}
	}
	//action 
	mem<<m_listActions.size();
	{
		foreach_const(TlistObjectActions, ite, m_listActions)
		{
			setDexBuffer((char*)(*ite)->getType().c_str());
			mem.WriteString(getDexBuffer());
			(*ite)->ArchiveOut(mem);
		}
	}
	mem<<m_bRender;
	mem<<m_bRenderNodeCube;
	mem<<m_bRenderCollideObject;
	return true;
}