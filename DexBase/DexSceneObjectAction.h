
#ifndef _DEX_SCENE_OBJECT_ACTIOIN_H
#define _DEX_SCENE_OBJECT_ACTIOIN_H

#include "typedefine.h"
#include "CDexObject.h"

class CDexSceneObject;
class DexSceneObjectAction: public CDexObject
{
	Dex_DeclareClass(DexSceneObjectAction,0)
protected:
	bool m_bCycle;      //是否循环
	bool m_bComplete;
	CDexSceneObject* m_pSceneObject; //该action所属的scene object
	
//#ifdef _DEBUG
	bool m_bRender;  //是否渲染catmull-rom路线 方便查看
//#endif

public:
	DexSceneObjectAction();
	virtual ~DexSceneObjectAction();

public:
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render();  //一些action需要渲染帮助线条 如catmull-rom曲线
	
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
		D3DXVECTOR3 init_pnt;  //初始点 在初始化处赋值
		D3DXVECTOR3 real_pnt;  //实时点 
	}stCatmullPnt ;
	DexSceneObjectActionCatmull();
	virtual ~DexSceneObjectActionCatmull();

protected:
	//以下参数用于控制自动更新时 catmull-rom曲线的控制
	std::vector<stCatmullPnt>  m_catmullPoints;  //catmull曲线控制点
	float m_posRate; //当前所在曲线的比率
	float m_speed;   //跑完一趟所有点的时间 单位s
	float m_factor;  //曲线弯曲度控制参数

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
	float m_fCyclePerSecond;  //每秒旋转多少圈
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