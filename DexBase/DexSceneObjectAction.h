
#ifndef _DEX_SCENE_OBJECT_ACTIOIN_H
#define _DEX_SCENE_OBJECT_ACTIOIN_H

#include "typedefine.h"
#include "CDexObject.h"

class CDexSceneObject;
class DexSceneObjectAction: public CDexObject
{
	Dex_DeclareClass(DexSceneObjectAction,0)
protected:
	bool m_bCycle;      //�Ƿ�ѭ��
	bool m_bComplete;
	CDexSceneObject* m_pSceneObject; //��action������scene object
	
//#ifdef _DEBUG
	bool m_bRender;  //�Ƿ���Ⱦcatmull-rom·�� ����鿴
//#endif

public:
	DexSceneObjectAction();
	virtual ~DexSceneObjectAction();

public:
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render();  //һЩaction��Ҫ��Ⱦ�������� ��catmull-rom����
	
	bool   getComplete()   const;
	void   setCycle(bool cycle);
	void   setRender(bool render);
	void   bindSceneObject(CDexSceneObject* object);
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
};

class DexSceneObjectActionCatmull: public DexSceneObjectAction
{
	Dex_DeclareClass(DexSceneObjectActionCatmull, 10)
	friend class CDexSceneObject;
public:
	typedef struct
	{
		D3DXVECTOR3 init_pnt;  //��ʼ�� �ڳ�ʼ������ֵ
		D3DXVECTOR3 real_pnt;  //ʵʱ�� 
	}stCatmullPnt ;
	DexSceneObjectActionCatmull();
	virtual ~DexSceneObjectActionCatmull();

protected:
	//���²������ڿ����Զ�����ʱ catmull-rom���ߵĿ���
	std::vector<stCatmullPnt>  m_catmullPoints;  //catmull���߿��Ƶ�
	float m_posRate; //��ǰ�������ߵı���
	float m_speed;   //����һ�����е��ʱ�� ��λs
	float m_factor;  //���������ȿ��Ʋ���

public:
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render();

public:
	void addCatmullPoint(const D3DXVECTOR3& pos);
	void setCatmullFactor(float factor);
	void setCatmullSpeed(float time);
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;

public:
	bool LoadInitFile(const char* filename);
};

class DexSceneObjectActionRotate: public DexSceneObjectAction
{
	Dex_DeclareClass(DexSceneObjectActionRotate, 10)
		friend class CDexSceneObject;
	typedef enum
	{
		ROTATEX,
		ROTATEY,
		ROTATEZ,
		FREE,
		COUNT
	}ERotateActionFlag;
public:
	DexSceneObjectActionRotate();
	virtual ~DexSceneObjectActionRotate();

protected:
	ERotateActionFlag m_rotateFlag;
	float m_fCurrRadian;
	float m_fCyclePerSecond;  //ÿ����ת����Ȧ
public:
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render();
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;

public:
	bool LoadInitFile(const char* filename);
};


#endif