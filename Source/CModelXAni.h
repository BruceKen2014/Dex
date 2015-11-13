/*************************************************************
****DemoEx引擎X文件封装类CModelX
****作者：阚正杰(BruceKen)
****日期：2012-11-27
*************************************************************/

#include "CModel.h"

#pragma once

struct stD3DFrameEx: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};

struct stD3DMeshContainerEx: public D3DXMESHCONTAINER
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

class AllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	AllocateHierarchy();
	~AllocateHierarchy();

	D3DXMATRIX*          m_pBoneMatrices;
	UINT                 m_NumBoneMatricesMax;
private:
	bool AllocateName( LPCSTR Name, LPSTR *pNewName );
	HRESULT GenerateGameSkinMesh(LPDIRECT3DDEVICE9 device, stD3DMeshContainerEx *pMeshContainer);

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
class CModelXAni:public CModel
{//暂时只在此加强Y属性，也就是旋转等都是以Y向量旋转的，其他的待日后扩展
protected:
	AllocateHierarchy*          m_pAlloc;
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
	CModelXAni(LPDIRECT3DDEVICE9 device);
	~CModelXAni();

	virtual void            ShutDown();

	HRESULT					LoadXFile(const char* filename);
	virtual void			Render();
	virtual void			Update();
	virtual void			SetAnimation(UINT index);
	virtual void            SetAnimationFromStart();  //切换到另一个Index时，动作从该index初始开始演示
	//void                    SetBlendAnimation(UINT index1, float weight1, UINT index2, float weight2);
	void                    SetMat(D3DXMATRIX mat){m_Matrix=mat;} 	
	D3DXMATRIX              GetMatrix(){return m_Matrix;}


protected:
	virtual void ChangeDir();        //旋转朝向
public:
	virtual void SetScaleX(float xScale);
	virtual void SetScaleY(float yScale);
	virtual void SetScaleZ(float zScale);
	virtual void SetScale(float xScale, float yScale, float zScale);
	virtual void SetScale(float scale);//设置整体缩放
	virtual void SetRotateX(float xRotate);
	virtual void SetRotateY(float yRotate);
	virtual void SetRotateZ(float zRotate);
	virtual void SetRotate(float xRotate, float yRotate, float zRotate);
	virtual void SetVeclX(float velX);
	virtual void SetVeclY(float velY);
	virtual void SetVeclZ(float velZ);
	virtual void SetVel(float velX, float velY, float velZ);
	virtual void SetVel(D3DXVECTOR3 vel);  //设置三个方向的分速度
	virtual void SetVel(float vel);		   //设置总速度,根据朝向求出分速度
	virtual void  SetAniSpeed(float aniSpeed);
	virtual float GetAniSpeed();
	
};

