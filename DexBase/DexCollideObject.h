
#ifndef DEX_COLLIDE_OBJECT
#define DEX_COLLIDE_OBJECT

#include "../Geometry/CBall.h"
#include "../Geometry/CBox.h"
#include "CDexObject.h"

class DexCollideObject: public CDexObject
{
	Dex_DeclareClass(DexCollideObject, CDexObject, 0)
protected:
	D3DXVECTOR3   m_pos;
public:
	DexCollideObject();
	virtual ~DexCollideObject();

public:
	virtual void RenderMesh() = 0 ;
	virtual bool CheckCollide(const DexCollideObject* object) = 0;
	virtual void SetObjectMatrix(const D3DXMATRIX& matrix);
	virtual void Move(const D3DXVECTOR3& delta) = 0;
	virtual bool checkPick(const stRay& ray) { return 0;};
	virtual void getCenter(D3DXVECTOR3& center) = 0;
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
};
class DexCollideObjectBox: public DexCollideObject
{
	Dex_DeclareClass(DexCollideObjectBox, DexCollideObject, 10)
protected:
	CBox* m_box;
public:
	DexCollideObjectBox();
	virtual ~DexCollideObjectBox();

public:
	virtual bool Initialize(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);
	virtual void Reset();
public:
	virtual void RenderMesh();
	virtual bool CheckCollide(const DexCollideObject* object);
	virtual void SetObjectMatrix(const D3DXMATRIX& matrix);
	virtual void Move(const D3DXVECTOR3& delta);
	virtual void getCenter(D3DXVECTOR3& center);
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
public:
	CBox*  getBox();
};
class DexCollideObjectSphere: public DexCollideObject
{
	Dex_DeclareClass(DexCollideObjectSphere, DexCollideObject, 10)
protected:
	stBall  m_ball;
public:
	DexCollideObjectSphere();
	virtual ~DexCollideObjectSphere();

public:
	virtual bool Initialize(const D3DXVECTOR3& pos, float radius);
public:
	virtual void RenderMesh();
	virtual bool CheckCollide(const DexCollideObject* object);
	virtual void SetObjectMatrix(const D3DXMATRIX& matrix);
	virtual void Move(const D3DXVECTOR3& delta);
	virtual void getCenter(D3DXVECTOR3& center);
	virtual bool checkPick(const stRay& ray);
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
public:
	stBall  getBall();
};

#endif