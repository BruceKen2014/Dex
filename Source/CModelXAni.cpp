




#include "CModelXAni.h"



AllocateHierarchy::AllocateHierarchy()
{
	m_pBoneMatrices=NULL;
	m_NumBoneMatricesMax=0;
}

AllocateHierarchy::~AllocateHierarchy()
{
}

bool AllocateHierarchy::AllocateName( LPCSTR Name, LPSTR *pNewName )
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
HRESULT AllocateHierarchy::GenerateGameSkinMesh(LPDIRECT3DDEVICE9 device, stD3DMeshContainerEx *pMeshContainer)
{
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
		MessageBox(0, "出错了！", 0, 0);


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



HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	HRESULT hr = S_OK;
	stD3DFrameEx *pFrame;

	*ppNewFrame = NULL;

	pFrame = new stD3DFrameEx;
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
HRESULT AllocateHierarchy::CreateMeshContainer(LPCSTR Name, 
											   CONST D3DXMESHDATA *pMeshData,
											   CONST D3DXMATERIAL *pMaterials, 
											   CONST D3DXEFFECTINSTANCE *pEffectInstances, 
											   DWORD NumMaterials, 
											   CONST DWORD *pAdjacency, 
											   LPD3DXSKININFO pSkinInfo, 
											   LPD3DXMESHCONTAINER *ppNewMeshContainer) 
{
	HRESULT hr;
	stD3DMeshContainerEx *pMeshContainer = NULL;
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
	pMeshContainer = new stD3DMeshContainerEx;
	if (pMeshContainer == NULL)
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}
	memset(pMeshContainer, 0, sizeof(stD3DMeshContainerEx));

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
				TCHAR file[1000];
				FindMediaFile(file,pMeshContainer->pMaterials[iMaterial].pTextureFilename);
				// 根据纹理的文件名创建纹理资源，如果创建失败，纹理指针必须赋成NULL
				// MessageNULL(file);
				//D3DXCreateTextureFromFileEx(device, file, 
				//	D3DX_DEFAULT_NONPOW2, 
				//	D3DX_DEFAULT_NONPOW2, 
				//	D3DX_FROM_FILE, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
				//	D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0,0,0), NULL, NULL, 
				//	&pMeshContainer->ppTextures[iMaterial]);
				if( FAILED( D3DXCreateTextureFromFile( device, pMeshContainer->pMaterials[iMaterial].pTextureFilename,
					&pMeshContainer->ppTextures[iMaterial] ) ) )
					pMeshContainer->ppTextures[iMaterial] = NULL;


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




HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	if(pFrameToFree != NULL)
	{
		_SafeDeleteArr( pFrameToFree->Name );
		_SafeDelete( pFrameToFree );
	}
	return S_OK; 
}


HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	if(pMeshContainerBase == NULL)
		return S_OK;

	// 先转为扩展型以免内存泄漏
	stD3DMeshContainerEx *pMeshContainer = (stD3DMeshContainerEx*)pMeshContainerBase;

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
CModelXAni::CModelXAni(LPDIRECT3DDEVICE9 device)
{
	m_device             = device;
	m_pAnimController    = NULL;
	m_pAnimationSet1     = NULL;
	m_pAnimationSet2	 = NULL;
	m_pFrameRoot         = NULL;
	m_pBoneMatrices      = NULL;
	m_NumBoneMatricesMax = 0;
	m_pAlloc             = new AllocateHierarchy();

	D3DXMatrixIdentity(&m_Matrix);
	m_posX = m_posY = m_posZ = 0;
	m_scaleX = m_scaleY = m_scaleZ = 1.0f;
	m_rotateX = m_rotateY = m_rotateZ = 0.0f;
	m_VelX = 0.0f;
	m_VelY = 0.0f;
	m_VelZ = 0.0f;
	m_direction = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_iniDir = m_direction; 
	m_lastDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_synDir = true;

	m_aniSpeed = 10;  
	m_renderSphere = true;
#ifdef DEX_DEBGU
	m_sphere = NULL;
#endif
	
}

CModelXAni::~CModelXAni()
{
	ShutDown();
}

void CModelXAni::ShutDown()
{
	D3DXFrameDestroy(m_pFrameRoot, m_pAlloc);
	_SafeRelease(m_pAnimationSet1);
	_SafeRelease(m_pAnimationSet2);
	_SafeRelease(m_pAnimController);
	_SafeDelete(m_pAlloc);
}


HRESULT CModelXAni::SetupBoneMatrixPointers(LPD3DXFRAME pFrame)
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

HRESULT CModelXAni::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	UINT iBone, cBones;
	stD3DFrameEx *pFrame;

	stD3DMeshContainerEx *pMeshContainer = (stD3DMeshContainerEx*)pMeshContainerBase;

	// if there is a skinmesh, then setup the bone matrices
	if (pMeshContainer->pSkinInfo != NULL)
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if (pMeshContainer->ppBoneMatrixPtrs == NULL)
			return E_OUTOFMEMORY;

		for (iBone = 0; iBone < cBones; iBone++)
		{
			pFrame = (stD3DFrameEx*)D3DXFrameFind( m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone) );
			if (pFrame == NULL)
				return E_FAIL;
			D3DXMATRIX temp = pFrame->CombinedTransformationMatrix;
			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
		}
	}
	return S_OK;
}

void CModelXAni::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	stD3DFrameEx *pFrame = (stD3DFrameEx*)pFrameBase;

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

void CModelXAni::DrawFrame(LPD3DXFRAME pFrame)
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


void CModelXAni::DrawMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase)
{
	stD3DMeshContainerEx *pMeshContainer = (stD3DMeshContainerEx*)pMeshContainerBase;
	stD3DFrameEx *pFrame = (stD3DFrameEx*)pFrameBase;
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
			m_device->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_0WEIGHTS);
		}
		else
		{
			m_device->SetRenderState( D3DRS_VERTEXBLEND, pMeshContainer->NumInfl - 1);
		}

		if( pMeshContainer->NumInfl )
			m_device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);

		// for each attribute group in the mesh, calculate the set of matrices in the palette and then draw the mesh subset
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>( pMeshContainer->pBoneCombinationBuf->GetBufferPointer
			() );
		//m_device->SetTransform(D3DTS_WORLD, &((stD3DFrameEx*)pFrameBase)->CombinedTransformationMatrix);
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
				 
					m_device->SetTransform( D3DTS_WORLDMATRIX( iPaletteEntry ), &matTemp );
				}
			}

			// setup the material of the mesh subset - REMEMBER to use the original pre-skinning attribute id to get the correct material id
			m_device->SetMaterial( &pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D );
			m_device->SetTexture( 0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId] );

			// finally draw the subset with the current world matrix palette and material state
			pMeshContainer->MeshData.pMesh->DrawSubset( iAttrib);
		}

		// reset blending state
		m_device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		m_device->SetRenderState( D3DRS_VERTEXBLEND, 0);
	}
}


HRESULT CModelXAni::LoadXFile(const char* filename)
{
	HRESULT hr;

	int64 mseconds = getTime()->GetTotalMillSeconds();
	//从.X文件加载层次框架和动画数据
	hr = D3DXLoadMeshHierarchyFromX(filename, D3DXMESH_MANAGED, m_device, 
		m_pAlloc, NULL, &m_pFrameRoot, &m_pAnimController);	  //最后一个参数：动画控制器
	if (FAILED(hr))
		return hr;

	//建立各级框架的组合变换矩阵
	SetupBoneMatrixPointers(m_pFrameRoot); 
	D3DXVECTOR3 center;
	memcpy(&center, &m_iniBall.m_center, sizeof(D3DXVECTOR3));
	D3DXFrameCalculateBoundingSphere(m_pFrameRoot, &center,
		&m_iniBall.m_radius);
	m_ball = m_iniBall;
	m_referBall = m_iniBall;
#ifdef DEX_DEBGU
	D3DXCreateSphere(m_device, m_ball.m_radius, 10, 10, &m_sphere, NULL);
#endif

	int index = 4;
	if(m_pAnimController != NULL)
	{
		m_pAnimController->GetAnimationSet(index, &m_pAnimationSet1);
		m_pAnimController->SetTrackAnimationSet(0, m_pAnimationSet1);
		m_pAnimController->ResetTime();
	}	
	getLog()->BeginLog();
	getLog()->Log(log_ok, "加载模型%s成功！用时:%dms",filename, getTime()->GetTotalMillSeconds() - mseconds);
	getLog()->EndLog();
	return S_OK;

}

void CModelXAni::Render()
{

	if(m_cullNone)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	UpdateFrameMatrices(m_pFrameRoot, &m_Matrix);

	//m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	///evice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//绘制frame
	DrawFrame(m_pFrameRoot); 

	if(m_renderSphere)
		RenderShpere();
	if(m_cullNone)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//矩阵重置，否则其他的渲染会受影响
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, 0, 0, 0);
	m_device->SetTransform(D3DTS_WORLD, &translation);
}
void CModelXAni::Update()
{
	if(GetMoving())
	{//开启了移动状态
		m_posX += m_VelX;
		m_posY += m_VelY;
		m_posZ += m_VelZ;

		m_ball.m_center.x = m_referBall.m_center.x + m_posX;   //中心要实时移动
		m_ball.m_center.y = m_referBall.m_center.y + m_posY;
		m_ball.m_center.z = m_referBall.m_center.z + m_posZ;

		SetPosition(m_posX, m_posY, m_posZ);
	}
	if ( m_pAnimController != NULL)
		m_pAnimController->AdvanceTime( m_aniSpeed/1000, NULL );  
}

void CModelXAni::SetAnimation(UINT index)
{
	if(m_pAnimController == NULL)
		return;
	m_pAnimController->GetAnimationSet(index, &m_pAnimationSet1);
	m_pAnimController->SetTrackAnimationSet(0, m_pAnimationSet1);
	m_pAnimController->ResetTime();
}
void CModelXAni::SetAnimationFromStart()
{
	if(m_pAnimController == NULL)
		return;
	m_pAnimController->SetTrackPosition(0, 0);

}



void CModelXAni::ChangeDir()
{
	m_direction.x = m_VelX;
	m_direction.y = 0;
	m_direction.z = m_VelZ;
	
	D3DXVec3Normalize(&m_direction, &m_direction);	

	if(_equal(m_direction.x,  m_lastDir.x, 0.001f)	   //浮点数的精度问题
	&& _equal(m_direction.z,  m_lastDir.z, 0.001f)	
	)
		return;
	//每次都是将新的朝向与<初始化>的朝向进行计算
	float dot =D3DXVec3Dot(&m_iniDir, &m_direction);
	float cos_radius = dot;  //因为两个皆是单位向量，故除模省去
	float angle_minus = acos(cos_radius) ;
	//float angle_minus = _getDegree(radius_minus);
	if(_equal(angle_minus, 0.0f, 0.01f))
		angle_minus = 0.0f;
	if(DecideISClock(m_iniDir, m_direction))
		SetRotateY(angle_minus);   
	else
		SetRotateY(-angle_minus);   
	m_lastDir = m_direction;
}

void CModelXAni::SetScaleX(float xScale)
{
	m_scaleX = xScale;
	CalCulateMatrix();
}
void CModelXAni::SetScaleY(float yScale)
{
	m_scaleY = yScale;
	CalCulateMatrix();
}
void CModelXAni::SetScaleZ(float zScale)
{
	m_scaleZ = zScale;
	CalCulateMatrix();
}
void CModelXAni::SetScale(float xScale, float yScale, float zScale)
{
	m_scaleX = xScale;
	m_scaleY = yScale;
	m_scaleZ = zScale;
	CalCulateMatrix();
}
void CModelXAni::SetScale(float scale)
{
	m_scaleX = scale;
	m_scaleY = scale;
	m_scaleZ = scale;
	m_referBall.m_center.y = m_iniBall.m_center.y + (scale-1)* m_iniBall.m_radius;   //中心要随着缩放而上移
	m_referBall.m_radius   = m_iniBall.m_radius * scale;
	m_ball = m_referBall;
#ifdef DEX_DEBGU
	_SafeRelease(m_sphere);
	D3DXCreateSphere(m_device, m_ball.m_radius, 20, 20, &m_sphere, NULL);
#endif
	CalCulateMatrix();
}
void CModelXAni::SetRotateX(float xRotate)
{
	m_rotateX = xRotate;
	CalCulateMatrix();
}
void CModelXAni::SetRotateY(float yRotate)
{
	m_rotateY = yRotate;
	CalCulateMatrix();
}
void CModelXAni::SetRotateZ(float zRotate)
{
	m_rotateZ = zRotate;
	CalCulateMatrix();
}
void CModelXAni::SetRotate(float xRotate, float yRotate, float zRotate)
{
	m_rotateX = xRotate;
	m_rotateY = yRotate;
	m_rotateZ = zRotate;
	CalCulateMatrix();
}

void CModelXAni::SetVeclX(float velX)
{
	m_VelX = velX;
	if(m_VelX == 0 && m_VelY == 0 && m_VelZ == 0)	//只是设置速度为0，朝向没必要变
		return;
	if(m_synDir)
		ChangeDir();
}

void CModelXAni::SetVeclY(float velY)
{
	m_VelY = velY;
	if(m_VelX == 0 && m_VelY == 0 && m_VelZ == 0)	//只是设置速度为0，朝向没必要变
		return;
	if(m_synDir)
		ChangeDir();
}

void CModelXAni::SetVeclZ(float velZ)
{
	m_VelZ = velZ;
	if(m_VelX == 0 && m_VelY == 0 && m_VelZ == 0)	//只是设置速度为0，朝向没必要变
		return;
	if(m_synDir)
		ChangeDir();
}

void CModelXAni::SetVel(float velX, float velY, float velZ)
{
	m_VelX = velX;
	m_VelY = velY;
	m_VelZ = velZ;
	if(m_VelX == 0 && m_VelY == 0 && m_VelZ == 0)	//只是设置速度为0，朝向没必要变
		return;
	if(m_synDir)
		ChangeDir();
}

void CModelXAni::SetVel(D3DXVECTOR3 vel)
{
	m_VelX = vel.x;
	m_VelY = vel.y;
	m_VelZ = vel.z;
	if(m_VelX == 0 && m_VelY == 0 && m_VelZ == 0)	//只是设置速度为0，朝向没必要变
		return;
	if(m_synDir)
		ChangeDir();
}

void CModelXAni::SetVel(float vel)
{
	m_vel = vel;
	GetDirection();
	//求出分速度
	m_VelX = vel * m_direction.x;
	m_VelY = vel * m_direction.y;
	m_VelZ = vel * m_direction.z;
	//if(m_synDir)
	//	ChangeDir();
}


void CModelXAni::SetAniSpeed(float aniSpeed)
{
	m_aniSpeed = aniSpeed;
}

float CModelXAni::GetAniSpeed()
{
	return m_aniSpeed;
}
