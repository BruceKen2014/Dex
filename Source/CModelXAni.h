/*************************************************************
****DemoEx����X�ļ���װ��CModelX
****���ߣ�������(BruceKen)
****���ڣ�2012-11-27
*************************************************************/

#include "CModel.h"

#pragma once

struct stD3DFrameEx: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};

struct stD3DMeshContainerEx: public D3DXMESHCONTAINER
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
{//��ʱֻ�ڴ˼�ǿY���ԣ�Ҳ������ת�ȶ�����Y������ת�ģ������Ĵ��պ���չ
protected:
	AllocateHierarchy*          m_pAlloc;
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
	CModelXAni(LPDIRECT3DDEVICE9 device);
	~CModelXAni();

	virtual void            ShutDown();

	HRESULT					LoadXFile(const char* filename);
	virtual void			Render();
	virtual void			Update();
	virtual void			SetAnimation(UINT index);
	virtual void            SetAnimationFromStart();  //�л�����һ��Indexʱ�������Ӹ�index��ʼ��ʼ��ʾ
	//void                    SetBlendAnimation(UINT index1, float weight1, UINT index2, float weight2);
	void                    SetMat(D3DXMATRIX mat){m_Matrix=mat;} 	
	D3DXMATRIX              GetMatrix(){return m_Matrix;}


protected:
	virtual void ChangeDir();        //��ת����
public:
	virtual void SetScaleX(float xScale);
	virtual void SetScaleY(float yScale);
	virtual void SetScaleZ(float zScale);
	virtual void SetScale(float xScale, float yScale, float zScale);
	virtual void SetScale(float scale);//������������
	virtual void SetRotateX(float xRotate);
	virtual void SetRotateY(float yRotate);
	virtual void SetRotateZ(float zRotate);
	virtual void SetRotate(float xRotate, float yRotate, float zRotate);
	virtual void SetVeclX(float velX);
	virtual void SetVeclY(float velY);
	virtual void SetVeclZ(float velZ);
	virtual void SetVel(float velX, float velY, float velZ);
	virtual void SetVel(D3DXVECTOR3 vel);  //������������ķ��ٶ�
	virtual void SetVel(float vel);		   //�������ٶ�,���ݳ���������ٶ�
	virtual void  SetAniSpeed(float aniSpeed);
	virtual float GetAniSpeed();
	
};

