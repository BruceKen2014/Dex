

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
	LPD3DXMESH        m_model;            //����ģ�ͽӿ�
	DWORD             m_numMaterials;	  
	LPD3DXBUFFER      m_matBuffer;	      //���ʻ���
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
	LPDIRECT3DTEXTURE9*  ppTextures;            //��������
	LPD3DXMESH           pOrigMesh;             //ԭʼ����
	DWORD                NumInfl;               //ÿ����������ܶ��ٹ�����Ӱ��
	DWORD                NumAttributeGroups;    //����������,������������
	LPD3DXBUFFER         pBoneCombinationBuf;   //������ϱ�
	D3DXMATRIX**         ppBoneMatrixPtrs;      //��Ź�������ϱ任����
	D3DXMATRIX*          pBoneOffsetMatrices;   //��Ź����ĳ�ʼ�任����
	DWORD                NumPaletteEntries;     //������������
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

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;	   //X����������
	ID3DXAnimationSet*			m_pAnimationSet1;	
	ID3DXAnimationSet*			m_pAnimationSet2;
protected:
	//Ϊ����Frame���(��������)����ϱ任���󿪱ٿռ�
	HRESULT						SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
	//��SetupBoneMatrixPointers����
	HRESULT						SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	//�������Frame��ܵ���ϱ�þ���
	void						UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );
	//������������
	void						DrawFrame(LPD3DXFRAME pFrame);
	//������Ƥ
	void						DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);	

protected:
	float             m_aniSpeed;       //X���������ٶ�              

public:
	CDexModelXAni();
	virtual ~CDexModelXAni();
	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render(D3DXMATRIX& node_matrix);
	virtual bool LoadModelFile(const char* filename);
	virtual void			SetAnimation(UINT index);
	virtual void            SetAnimationFromStart();  //�л�����һ��Indexʱ�������Ӹ�index��ʼ��ʼ��ʾ
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