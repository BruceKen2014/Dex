

#ifndef _DEX_SCENE_OBJECT_MODEL_H
#define _DEX_SCENE_OBJECT_MODEL_H


#include "DexSceneObject.h"


//========================================model interface====================================
class CDextModel: public CDexObject
{
	Dex_DeclareClass(CDextModel,0)
public:
	CDextModel();
	virtual ~CDextModel();
	virtual bool Update(int delta) = 0;
	virtual bool Render(D3DXMATRIX& node_matrix) = 0;
	virtual bool LoadModelFile(const char* filename) = 0;
};



//=============================================.X file static=====================================
class CDexModelX: public CDextModel
{
	Dex_DeclareClass(CDexModelX,10)
protected:
	LPD3DXMESH        m_model;            //网格模型接口
	DWORD             m_numMaterials;	  
	LPD3DXBUFFER      m_matBuffer;	      //材质缓存
	D3DMATERIAL9*     m_matList;
	LPDIRECT3DTEXTURE9* m_textureList;

public:
	CDexModelX();
	virtual ~CDexModelX();

public:
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render(D3DXMATRIX& node_matrix);
	virtual bool LoadModelFile(const char* filename);
};

//==========================================.X Animation file=====================================

struct stDexFrameEx: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};

struct stDexMeshContainerEx: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;            //纹理数组
	LPD3DXMESH           pOrigMesh;             //原始网格
	DWORD                NumInfl;               //每个顶点最多受多少骨骼的影响
	DWORD                NumAttributeGroups;    //属性组数量,即子网格数量
	LPD3DXBUFFER         pBoneCombinationBuf;   //骨骼结合表
	D3DXMATRIX**         ppBoneMatrixPtrs;      //存放骨骼的组合变换矩阵
	D3DXMATRIX*          pBoneOffsetMatrices;   //存放骨骼的初始变换矩阵
	DWORD                NumPaletteEntries;     //骨骼数量上限
};

class DexAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	DexAllocateHierarchy();
	~DexAllocateHierarchy();

	D3DXMATRIX*          m_pBoneMatrices;
	UINT                 m_NumBoneMatricesMax;
private:
	bool AllocateName( LPCSTR Name, LPSTR *pNewName );
	HRESULT GenerateGameSkinMesh(LPDIRECT3DDEVICE9 device, stDexMeshContainerEx *pMeshContainer);

public:
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)( THIS_ LPCSTR              Name, 
		CONST D3DXMESHDATA*       pMeshData,
		CONST D3DXMATERIAL*       pMaterials, 
		CONST D3DXEFFECTINSTANCE* pEffectInstances, 
		DWORD                     NumMaterials, 
		CONST DWORD *             pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);    
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};
class CDexModelXAni:public CDextModel
{
	Dex_DeclareClass(CDexModelXAni, 10)
protected:
	DexAllocateHierarchy*          m_pAlloc;
	LPD3DXFRAME					m_pFrameRoot;
	D3DXMATRIX*                 m_pBoneMatrices;
	UINT                        m_NumBoneMatricesMax;

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;	   //X动画控制器
	ID3DXAnimationSet*			m_pAnimationSet1;	
	ID3DXAnimationSet*			m_pAnimationSet2;
protected:
	//为各级Frame框架(各个骨骼)的组合变换矩阵开辟空间
	HRESULT						SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
	//被SetupBoneMatrixPointers调用
	HRESULT						SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	//计算各级Frame框架的组合变幻矩阵
	void						UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
	//画出各级骨骼
	void						DrawFrame(LPD3DXFRAME pFrame);
	//画出蒙皮
	void						DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);	

protected:
	float             m_aniSpeed;       //X动画播放速度              

public:
	CDexModelXAni();
	virtual ~CDexModelXAni();
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render(D3DXMATRIX& node_matrix);
	virtual bool LoadModelFile(const char* filename);
	virtual void			SetAnimation(UINT index);
	virtual void            SetAnimationFromStart();  //切换到另一个Index时，动作从该index初始开始演示
	//void                    SetBlendAnimation(UINT index1, float weight1, UINT index2, float weight2);
	virtual void  SetAniSpeed(float aniSpeed);
	virtual float GetAniSpeed();

};

//========================================model factory==============================================
class CDexModelFactory:public CDexObject
{
	Dex_DeclareClass(CDexModelFactory, 1)
	friend class CDexObjectFactroy;
private:
	typedef std::map<string, CDextModel*> TMapModel;
	TMapModel m_MapModel;
	static CDexModelFactory* g_pModelFactory;
private:
	CDexModelFactory();
	virtual ~CDexModelFactory();
public:
	static CDexModelFactory* getModelFactory();
	bool LoadResource(const char* filename);
	CDextModel* FindModel(const char* modelname);
};

//===============================scene object model ==========================================

class CDexSceneObjectModel: public CDexSceneObject
{
	Dex_DeclareClass(CDexSceneObjectModel, 10)
protected:
	CDextModel* m_pModel;
public:
	CDexSceneObjectModel();
	virtual ~CDexSceneObjectModel();
public:
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render();
	virtual bool LoadModelFile(const char* filename, bool ani = false) ;
	virtual void setModel(CDextModel* model);

	CDexModelXAni* getAniModel();
};

#endif