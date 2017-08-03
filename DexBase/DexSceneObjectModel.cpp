

#include "DexSceneObjectModel.h"
#include "DexLog.h"
#include "DexObjectFactory.h"
#include "../state/DexGameEngine.h"
#include "../Source/CCommandScript.h"

CDextModel::CDextModel()
{
}
CDextModel::~CDextModel()
{

}

//==========================================================

CDexModelX::CDexModelX()
{
	m_model = NULL;
	m_matBuffer = NULL;
	m_matList = NULL;
	m_textureList = NULL;
}


CDexModelX::~CDexModelX()
{
	_SafeRelease(m_model);
	_SafeRelease(m_matBuffer); 
	_SafeDeleteArr(m_matList);
	for(DWORD i = 0 ; i < m_numMaterials; i++)
		_SafeRelease(m_textureList[i]);
	_SafeDeleteArr(m_textureList);
}

bool CDexModelX::LoadModelFile(const char* filename)
{
	DexLog::getSingleton()->BeginLog();
	DInt64 mseconds = getTime()->GetTotalMillSeconds();
	if(FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM,
		DexGameEngine::getEngine()->GetDevice(), NULL, &m_matBuffer, NULL,
		&m_numMaterials, &m_model))) 
	{
		DexLog::getSingleton()->Log(log_allert, "加载模型%s失败！",filename);
		DexLog::getSingleton()->EndLog();
		return false;
	}
	m_matList = new D3DMATERIAL9[m_numMaterials];
	m_textureList  = new LPDIRECT3DTEXTURE9[m_numMaterials];
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_matBuffer->GetBufferPointer();

	// Loop and load each textture and get each material.
	for(DWORD i = 0; i < m_numMaterials; i++)
	{
		// Copy the materials from the buffer into our list.
		m_matList[i] = mat[i].MatD3D;

		// Load the textures into the list.
		string texname = "model/";  //轉到model文件夾中去找紋理
		texname += mat[i].pTextureFilename;
		if(FAILED(D3DXCreateTextureFromFile(DexGameEngine::getEngine()->GetDevice(),
			texname.c_str(), &m_textureList[i]))) 
		{
			DexLog::getSingleton()->Log(log_allert, "model %s load texture %s failed ！\n",filename, texname.c_str());
			m_textureList[i] = NULL;
		}
	}
	DexLog::getSingleton()->Log(log_ok, "加载模型%s成功！用时:%d ms\n",filename, getTime()->GetTotalMillSeconds() - mseconds);
	DexLog::getSingleton()->EndLog();
	return true;
}

void CDexModelX::Reset()
{
	CDextModel::Reset();
	_SafeRelease(m_model);
	_SafeRelease(m_matBuffer); 
	_SafeDeleteArr(m_matList);
	for(DWORD i = 0 ; i < m_numMaterials; i++)
		_SafeRelease(m_textureList[i]);
	_SafeDeleteArr(m_textureList);
}

bool CDexModelX::Update(int delta)
{
	return true;
}

bool CDexModelX::Render(D3DXMATRIX& node_matrix)
{
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	for(DWORD i = 0; i < m_numMaterials; i++)
	{
		DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &node_matrix);
		DexGameEngine::getEngine()->GetDevice()->SetMaterial(&m_matList[i]);
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, m_textureList[i]);
		m_model->DrawSubset(i);
	}
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	return true;
}

//==========================================.X Animation file=====================================

DexAllocateHierarchy::DexAllocateHierarchy()
{
	m_pBoneMatrices=NULL;
	m_NumBoneMatricesMax=0;
}

DexAllocateHierarchy::~DexAllocateHierarchy()
{
}

bool DexAllocateHierarchy::AllocateName( LPCSTR Name, LPSTR *pNewName )
{
	UINT cbLength;

	if( Name != NULL )
	{
		cbLength = (UINT)strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];
		if (*pNewName == NULL)
			return false;
		memcpy( *pNewName, Name, cbLength*sizeof(CHAR) );
	}
	else
	{
		*pNewName = NULL;
	}

	return true;
}

//-----------------------------------------------------------------------------
//Desc: 生成蒙皮网格模型(含有每个顶点的混合权重、索引和一个骨骼组合表)
//-----------------------------------------------------------------------------
HRESULT DexAllocateHierarchy::GenerateGameSkinMesh(LPDIRECT3DDEVICE9 device, stDexMeshContainerEx *pMeshContainer)
{
	DexLog::getSingleton()->BeginLog();
	HRESULT hr = S_OK;

	if (pMeshContainer->pSkinInfo == NULL)
		return hr;

	_SafeRelease( pMeshContainer->MeshData.pMesh );
	_SafeRelease( pMeshContainer->pBoneCombinationBuf );

	D3DCAPS9 d3dCaps;
	device->GetDeviceCaps( &d3dCaps );

	UINT MaxMatrices = 26; 
	pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

	DWORD Flags = D3DXMESHOPT_VERTEXCACHE;
	if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
	{
		Flags |= D3DXMESH_MANAGED;
	}
	else
	{
		DexLog::getSingleton()->Log(log_allert, ".X Animation file shader 錯誤！");
	}


	//_SafeRelease(pMeshContainer->MeshData.pMesh);

	if(1)
	{
		DWORD NumMaxFaceInfl;
		DWORD Flags = D3DXMESHOPT_VERTEXCACHE;

		LPDIRECT3DINDEXBUFFER9 pIB;
		hr = pMeshContainer->pOrigMesh->GetIndexBuffer( &pIB );
		if( FAILED( hr ) )
			return hr;

		hr = pMeshContainer->pSkinInfo->GetMaxFaceInfluences( pIB,
			pMeshContainer->pOrigMesh->GetNumFaces(),
			&NumMaxFaceInfl );
		pIB->Release();
		if( FAILED( hr ) )
			return hr;

		// 12 entry palette guarantees that any triangle (4 independent influences per vertex of a tri)
		// can be handled
		NumMaxFaceInfl = min( NumMaxFaceInfl, 12 );

		if( d3dCaps.MaxVertexBlendMatrixIndex + 1 < NumMaxFaceInfl )
		{
			// HW does not support indexed vertex blending. Use SW instead
			pMeshContainer->NumPaletteEntries = min( 256, pMeshContainer->pSkinInfo->GetNumBones() );
			Flags |= D3DXMESH_SYSTEMMEM;
		}
		else
		{
			// using hardware - determine palette size from caps and number of bones
			// If normals are present in the vertex data that needs to be blended for lighting, then 
			// the number of matrices is half the number specified by MaxVertexBlendMatrixIndex.
			pMeshContainer->NumPaletteEntries = min( ( d3dCaps.MaxVertexBlendMatrixIndex + 1 ) / 2,
				pMeshContainer->pSkinInfo->GetNumBones() );
			Flags |= D3DXMESH_MANAGED;
		}

		hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
			(
			pMeshContainer->pOrigMesh,
			Flags,
			pMeshContainer->NumPaletteEntries,
			pMeshContainer->pAdjacency,
			NULL, NULL, NULL,
			&pMeshContainer->NumInfl,
			&pMeshContainer->NumAttributeGroups,
			&pMeshContainer->pBoneCombinationBuf,
			&pMeshContainer->MeshData.pMesh );
		if( FAILED( hr ) )
			return hr;
		return 1;
	}

	//生成蒙皮网格模型
	hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
		(
		pMeshContainer->pOrigMesh,
		Flags, 
		pMeshContainer->NumPaletteEntries, 
		pMeshContainer->pAdjacency, 
		NULL, NULL, NULL,             
		&pMeshContainer->NumInfl,
		&pMeshContainer->NumAttributeGroups, 
		&pMeshContainer->pBoneCombinationBuf, 
		&pMeshContainer->MeshData.pMesh);
	if (FAILED(hr))
		return hr;


	// FVF has to match our declarator. Vertex shaders are not as forgiving as FF pipeline
	DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
	{
		LPD3DXMESH pMesh;
		hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(pMeshContainer->MeshData.pMesh->GetOptions(), NewFVF, device, &pMesh);
		if (!FAILED(hr))
		{
			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pMesh;
			pMesh = NULL;
		}
	}

	D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
	LPD3DVERTEXELEMENT9 pDeclCur;
	hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
	if (FAILED(hr))
		return hr;

	// the vertex shader is expecting to interpret the UBYTE4 as a D3DCOLOR, so update the type 
	//   NOTE: this cannot be done with CloneMesh, that would convert the UBYTE4 data to float and then to D3DCOLOR
	//          this is more of a "cast" operation
	pDeclCur = pDecl;
	while (pDeclCur->Stream != 0xff)
	{
		if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (pDeclCur->UsageIndex == 0))
			pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		pDeclCur++;
	}

	hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);
	if (FAILED(hr))
		return hr;

	// allocate a buffer for bone matrices, but only if another mesh has not allocated one of the same size or larger
	if( m_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
	{
		m_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		// Allocate space for blend matrices
		delete[] m_pBoneMatrices; 
		m_pBoneMatrices  = new D3DXMATRIX[m_NumBoneMatricesMax];
		if( m_pBoneMatrices == NULL )
		{
			hr = E_OUTOFMEMORY;
			return hr;
		}
	}
	return hr;
}



HRESULT DexAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	stDexFrameEx *pFrame;

	*ppNewFrame = NULL;

	pFrame = new stDexFrameEx;
	if (pFrame == NULL)
	{
		hr = E_OUTOFMEMORY;
		delete pFrame;
		return hr;
	}

	hr = AllocateName(Name, &pFrame->Name);
	if (FAILED(hr))
	{
		delete pFrame;
		return hr;
	}

	// initialize other data members of the frame
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;

	*ppNewFrame = pFrame;
	pFrame = NULL;

	delete pFrame;
	return hr;
}


//-----------------------------------------------------------------------------
// Desc: 在这里是调用了成员函数 GenerateGameSkinMesh(pMeshContainer);
//       是在这里加载了蒙皮信息
//-----------------------------------------------------------------------------
HRESULT DexAllocateHierarchy::CreateMeshContainer(LPCSTR Name, 
	CONST D3DXMESHDATA *pMeshData,
	CONST D3DXMATERIAL *pMaterials, 
	CONST D3DXEFFECTINSTANCE *pEffectInstances, 
	DWORD NumMaterials, 
	CONST DWORD *pAdjacency, 
	LPD3DXSKININFO pSkinInfo, 
	LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
	HRESULT hr;
	stDexMeshContainerEx *pMeshContainer = NULL;
	LPDIRECT3DDEVICE9 device = NULL;
	UINT NumFaces;
	UINT iMaterial;
	UINT iBone, cBones;

	LPD3DXMESH pMesh = NULL;

	*ppNewMeshContainer = NULL;

	// this sample does not handle patch meshes, so fail when one is found
	if (pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		hr = E_FAIL;
		return hr;
	}

	// get the pMesh interface pointer out of the mesh data structure
	pMesh = pMeshData->pMesh;
	pMesh->GetDevice( &device );

	// this sample does not FVF compatible meshes, so fail when one is found
	if (pMesh->GetFVF() == 0)
	{
		hr = E_FAIL;
		return hr;
	}

	// allocate the overloaded structure to return as a D3DXMESHCONTAINER
	pMeshContainer = new stDexMeshContainerEx;
	if (pMeshContainer == NULL)
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}
	memset(pMeshContainer, 0, sizeof(stDexMeshContainerEx));

	// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
	hr = AllocateName(Name, &pMeshContainer->Name);
	if (FAILED(hr))
	{
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return hr;
	}

	NumFaces = pMesh->GetNumFaces();

	// if no normals are in the mesh, add them
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		// clone the mesh to make room for the normals
		hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
			pMesh->GetFVF() | D3DFVF_NORMAL, 
			device, &pMeshContainer->MeshData.pMesh );
		if (FAILED(hr))
		{
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}

		// get the new pMesh pointer back out of the mesh container to use
		// NOTE: we do not release pMesh because we do not have a reference to it yet
		pMesh = pMeshContainer->MeshData.pMesh;

		// now generate the normals for the pmesh
		D3DXComputeNormals( pMesh, NULL );
	}
	else  // if no normals, just add a reference to the mesh for the mesh container
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();
	}

	// allocate memory to contain the material information.  This sample uses
	//   the D3D9 materials and texture names instead of the EffectInstance style materials
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[NumFaces*3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		hr = E_OUTOFMEMORY;
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}
		return hr;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces*3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	// if materials provided, copy them
	if (NumMaterials > 0)            
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
			{
				//TCHAR file[1000];
				//FindMediaFile(file,pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				
				// 根据纹理的文件名创建纹理资源，如果创建失败，纹理指针必须赋成NULL
				// MessageNULL(file);
				//D3DXCreateTextureFromFileEx(device, file, 
				//	D3DX_DEFAULT_NONPOW2, 
				//	D3DX_DEFAULT_NONPOW2, 
				//	D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
				//	D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0,0,0), NULL, NULL, 
				//	&pMeshContainer->ppTextures[iMaterial]);
				string texname = "model/";  //轉到model文件夾中去找紋理
				texname += pMeshContainer->pMaterials[iMaterial].pTextureFilename;
				if( FAILED( D3DXCreateTextureFromFile( device, texname.c_str(),
					&pMeshContainer->ppTextures[iMaterial] ) ) )
				{
					DexLog::getSingleton()->BeginLog();
					DexLog::getSingleton()->Log(log_allert, "load model texture %s failed! \n", texname.c_str());
					pMeshContainer->ppTextures[iMaterial] = NULL;
					DexLog::getSingleton()->EndLog();
				}

				// don't remember a pointer into the dynamic memory, just forget the name after loading
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
			}
		}
	}
	else // if no materials provided, use a default one
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	// if there is skinning information, save off the required data and then setup for HW skinning
	if (pSkinInfo != NULL)
	{
		// first save off the SkinInfo and original mesh data
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if (pMeshContainer->pBoneOffsetMatrices == NULL)
		{
			hr = E_OUTOFMEMORY;
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}

		// get each of the bone offset matrices so that we don't need to get them later
		for (iBone = 0; iBone < cBones; iBone++)
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
		}

		// GenerateGameSkinMesh will take the general skinning information and transform it to a HW friendly version
		hr = GenerateGameSkinMesh(device, pMeshContainer );
		if (FAILED(hr))
		{       
			if (pMeshContainer != NULL)
			{
				DestroyMeshContainer(pMeshContainer);
			}
			return hr;
		}
	}

	*ppNewMeshContainer = pMeshContainer;
	pMeshContainer = NULL;

	// call Destroy function to properly clean up the memory allocated 
	if (pMeshContainer != NULL)
	{
		DestroyMeshContainer(pMeshContainer);
	}
	return hr;
}




HRESULT DexAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	if(pFrameToFree != NULL)
	{
		_SafeDeleteArr( pFrameToFree->Name );
		_SafeDelete( pFrameToFree );
	}
	return S_OK; 
}


HRESULT DexAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	if(pMeshContainerBase == NULL)
		return S_OK;

	// 先转为扩展型以免内存泄漏
	stDexMeshContainerEx *pMeshContainer = (stDexMeshContainerEx*)pMeshContainerBase;

	_SafeDeleteArr( pMeshContainer->Name );
	_SafeDeleteArr( pMeshContainer->pAdjacency );
	_SafeDeleteArr( pMeshContainer->pMaterials );
	_SafeDeleteArr( pMeshContainer->pBoneOffsetMatrices );
	_SafeDeleteArr( pMeshContainer->ppTextures );

	_SafeDeleteArr( pMeshContainer->ppBoneMatrixPtrs );
	_SafeRelease( pMeshContainer->pBoneCombinationBuf );
	_SafeRelease( pMeshContainer->MeshData.pMesh );
	_SafeRelease( pMeshContainer->pSkinInfo );
	_SafeRelease( pMeshContainer->pOrigMesh );
	_SafeDelete( pMeshContainer );
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////
CDexModelXAni::CDexModelXAni()
{
	m_pAnimController    = NULL;
	m_pAnimationSet1     = NULL;
	m_pAnimationSet2	 = NULL;
	m_pFrameRoot         = NULL;
	m_pBoneMatrices      = NULL;
	m_NumBoneMatricesMax = 0;
	m_pAlloc             = new DexAllocateHierarchy();
	m_aniSpeed = 10;  
}

CDexModelXAni::~CDexModelXAni()
{
	D3DXFrameDestroy(m_pFrameRoot, m_pAlloc);
	_SafeRelease(m_pAnimationSet1);
	_SafeRelease(m_pAnimationSet2);
	_SafeRelease(m_pAnimController);
	_SafeDelete(m_pAlloc);
}

void CDexModelXAni::Reset()
{
	CDextModel::Reset();
	D3DXFrameDestroy(m_pFrameRoot, m_pAlloc);
	_SafeRelease(m_pAnimationSet1);
	_SafeRelease(m_pAnimationSet2);
	_SafeRelease(m_pAnimController);
	//_SafeDelete(m_pAlloc);
}


HRESULT CDexModelXAni::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
{
	HRESULT hr;

	if (pFrame->pMeshContainer != NULL)
	{
		hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameSibling);
		if (FAILED(hr))
			return hr;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

HRESULT CDexModelXAni::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iBone, cBones;
	stDexFrameEx *pFrame;

	stDexMeshContainerEx *pMeshContainer = (stDexMeshContainerEx*)pMeshContainerBase;

	// if there is a skinmesh, then setup the bone matrices
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			pFrame = (stDexFrameEx*)D3DXFrameFind( m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone) );
			if (pFrame == NULL)
				return E_FAIL;
			D3DXMATRIX temp = pFrame->CombinedTransformationMatrix;
			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}
	return S_OK;
}

void CDexModelXAni::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	stDexFrameEx *pFrame = (stDexFrameEx*)pFrameBase;

	if (pParentMatrix != NULL)
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	else
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}

void CDexModelXAni::DrawFrame(LPD3DXFRAME pFrame)
{
	LPD3DXMESHCONTAINER pMeshContainer;
	pMeshContainer = pFrame->pMeshContainer;

	//先绘出蒙皮
	while (pMeshContainer != NULL)
	{
		DrawMeshContainer(pMeshContainer, pFrame); //调用子函数
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	//后绘框架，采用递归调用
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}


void CDexModelXAni::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	stDexMeshContainerEx *pMeshContainer = (stDexMeshContainerEx*)pMeshContainerBase;
	stDexFrameEx *pFrame = (stDexFrameEx*)pFrameBase;
	//UINT iMaterial;
	UINT iAttrib;
	LPD3DXBONECOMBINATION pBoneComb;
	UINT iMatrixIndex;
	UINT iPaletteEntry;

	D3DXMATRIXA16 matTemp;

	if(pMeshContainer->pSkinInfo   !=   NULL)   
	{   
		// set the number of vertex blend indices to be blended
		if( pMeshContainer->NumInfl == 1 )
		{
			DexGameEngine::getEngine()->GetDevice()->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
		}
		else
		{
			DexGameEngine::getEngine()->GetDevice()->SetRenderState( D3DRS_VERTEXBLEND, pMeshContainer->NumInfl - 1);
		}

		if( pMeshContainer->NumInfl )
			DexGameEngine::getEngine()->GetDevice()->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);

		// for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>( pMeshContainer->pBoneCombinationBuf->GetBufferPointer
			() );
		//DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &((stDexFrameEx*)pFrameBase)->CombinedTransformationMatrix);
		for( iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++ )
		{//遍历网格
			// first calculate all the world matrices
			int tr = pMeshContainer->NumPaletteEntries;
			for( iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry )
			{
				iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
				if( iMatrixIndex != UINT_MAX )
				{
					D3DXMATRIX m1 = (pMeshContainer->pBoneOffsetMatrices[iMatrixIndex]);
					D3DXMATRIX m2 = *(pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
					D3DXMatrixMultiply( &matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex] );

					DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLDMATRIX( iPaletteEntry ), &matTemp );
				}
			}

			// setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
			DexGameEngine::getEngine()->GetDevice()->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
			DexGameEngine::getEngine()->GetDevice()->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );

			// finally draw the subset with the current world matrix palette and material state
			pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib);
		}

		// reset blending state
		DexGameEngine::getEngine()->GetDevice()->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		DexGameEngine::getEngine()->GetDevice()->SetRenderState( D3DRS_VERTEXBLEND, 0);
	}
}


bool CDexModelXAni::LoadModelFile(const char* filename)
{
	HRESULT hr;

	DInt64 mseconds = getTime()->GetTotalMillSeconds();
	//从.X文件加载层次框架和动画数据
	hr = D3DXLoadMeshHierarchyFromX(filename, D3DXMESH_MANAGED, DexGameEngine::getEngine()->GetDevice(), 
		m_pAlloc, NULL, &m_pFrameRoot, &m_pAnimController);	  //最后一个参数：动画控制器
	if (FAILED(hr))
		return false;

	//建立各级框架的组合变换矩阵
	SetupBoneMatrixPointers(m_pFrameRoot); 

	int index = 4;
	if(m_pAnimController != NULL)
	{
		m_pAnimController->GetAnimationSet(index, &m_pAnimationSet1);
		m_pAnimController->SetTrackAnimationSet(0, m_pAnimationSet1);
		m_pAnimController->ResetTime();
	}	
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_ok, "加载animation模型%s成功！用时:%d ms\n",filename, getTime()->GetTotalMillSeconds() - mseconds);
	DexLog::getSingleton()->EndLog();
	return true;

}

bool CDexModelXAni::Render(D3DXMATRIX& node_matrix)
{
	UpdateFrameMatrices(m_pFrameRoot, &node_matrix);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	///evice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//绘制frame
	DrawFrame(m_pFrameRoot); 
	
	//矩阵重置，否则其他的渲染会受影响
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, 0, 0, 0);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &translation);
	return true;
}
bool CDexModelXAni::Update(int delta)
{
	if ( m_pAnimController != NULL)
		m_pAnimController->AdvanceTime( m_aniSpeed/1000, NULL );  
	return true;
}

void CDexModelXAni::SetAnimation(UINT index)
{
	if(m_pAnimController == NULL)
		return;
	m_pAnimController->GetAnimationSet(index, &m_pAnimationSet1);
	m_pAnimController->SetTrackAnimationSet(0, m_pAnimationSet1);
	m_pAnimController->ResetTime();
}
void CDexModelXAni::SetAnimationFromStart()
{
	if(m_pAnimController == NULL)
		return;
	m_pAnimController->SetTrackPosition(0, 0);

}
void CDexModelXAni::SetAniSpeed(float aniSpeed)
{
	m_aniSpeed = aniSpeed;
}

float CDexModelXAni::GetAniSpeed()
{
	return m_aniSpeed;
}
//========================================model factory==============================================

CDexModelFactory* CDexModelFactory::g_pModelFactory = NULL;
CDexModelFactory::CDexModelFactory()
{

}
CDexModelFactory::~CDexModelFactory()
{

}
CDexModelFactory* CDexModelFactory::getModelFactory()
{
	if(g_pModelFactory == NULL)
	{
		g_pModelFactory = Dex_QueryObject(CDexModelFactory);
	}
	return g_pModelFactory;
}
bool CDexModelFactory::LoadResource(const char* filename)
{
	//加载资源
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		pScript->CloseScript();
		return false;
	}
	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar() ;
		CurrChar = pScript->GetCurrChar();
		CurrLine = pScript->GetCurrLine(); 
		if(strlen(Script[CurrLine]) == 1 || 
			(Script[CurrLine][CurrChar] == '-' 
			&& Script[CurrLine][CurrChar+1] == '-'))  	//略过注释或空白行
			continue;
		//取出字符串
		char pic[MAX_PARAM_SIZE];
		pScript->GetStringParam(pic);

		CDextModel* model = Dex_QueryObject(CDexModelX);
		if(model != NULL)
		{
			if(model->LoadModelFile((const char*)pic))
			{
				m_MapModel[pic] = model;
			}
		}
	}
	pScript->CloseScript();
	return true;
}
CDextModel* CDexModelFactory::FindModel(const char* modelname)
{
	if(m_MapModel.find(modelname) != m_MapModel.end())
	{
		return m_MapModel[modelname];
	};
	return NULL;
}
//===============================scene object model ==========================================
CDexSceneObjectModel::CDexSceneObjectModel()
{
	m_pModel = NULL;
}

CDexSceneObjectModel::~CDexSceneObjectModel()
{
	_SafeRelease(m_pModel);
}

void CDexSceneObjectModel::Reset()
{
	CDexSceneObject::Reset();
	_SafeRelease(m_pModel);
}

bool CDexSceneObjectModel::Update(int delta)
{
	DEX_ENSURE_B(CDexSceneObject::Update(delta));
	DEX_ENSURE_B(m_pModel != NULL);
	m_pModel->Update(delta);
	return true;
}

bool CDexSceneObjectModel::Render()
{
	DEX_ENSURE_B(CDexSceneObject::Render());
	DEX_ENSURE_B(m_pModel != NULL);
	if(m_bCullNone)
		DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pModel->Render(m_finalMatrix);
	if(m_bCullNone)
		DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return true;
}

bool CDexSceneObjectModel::LoadModelFile(const char* filename, bool ani /* = false */)
{
	DEX_ENSURE_B(filename);
	if(ani)
	{//animation model
		_SafeRelease(m_pModel);
		m_pModel = Dex_QueryObject(CDexModelXAni);
		DEX_ENSURE_B(m_pModel);
		return m_pModel->LoadModelFile(filename);
	}
	else
	{//static model
		m_pModel = CDexModelFactory::getModelFactory()->FindModel(filename);
		return m_pModel!=NULL;
	}
}

void CDexSceneObjectModel::setModel(CDextModel* model)
{
	m_pModel = model;
}

CDexModelXAni*  CDexSceneObjectModel::getAniModel()
{
	DEX_ENSURE_P(m_pModel != NULL);
	if(m_pModel->getType() == CDexModelXAni::getClassType())
	{
		return (CDexModelXAni*)m_pModel;
	}
	return NULL;
}



















