




#include "CModelX.h"


CModelX::CModelX()
{
}

CModelX::CModelX(LPDIRECT3DDEVICE9 device, char* filename):CModel()
{
	m_device = device; 
	//m_posX = m_posY = m_posZ = 0;
	//m_scaleX = m_scaleY = m_scaleZ = 1.0f;
	//m_rotateX = m_rotateY = m_rotateZ = 0.0f;
	DInt64 mseconds = DexTime::getSingleton()->GetTotalMillSeconds();
	if(FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM,
		m_device, NULL, &m_matBuffer, NULL,
		&m_numMaterials, &m_model))) 
	{
		MessageBox(NULL, "加载Model.x失败！",TEXT("提示"),MB_OK);
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "加载模型%s失败！",filename);
		DexLog::getSingleton()->EndLog();
		return ;
	}
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_ok, "加载模型%s成功！用时:%dms",filename, DexTime::getSingleton()->GetTotalMillSeconds() - mseconds);
	DexLog::getSingleton()->EndLog();
	m_matList = new D3DMATERIAL9[m_numMaterials];
	m_textureList  = new LPDIRECT3DTEXTURE9[m_numMaterials];
	D3DXMATERIAL* mat = (D3DXMATERIAL*)m_matBuffer->GetBufferPointer();

	// Loop and load each textture and get each material.
	for(DWORD i = 0; i < m_numMaterials; i++)
	{
		// Copy the materials from the buffer into our list.
		m_matList[i] = mat[i].MatD3D;

		// Load the textures into the list.
		if(FAILED(D3DXCreateTextureFromFile(m_device,
			mat[i].pTextureFilename,
			&m_textureList[i]))) 
			m_textureList[i] = NULL;
	}
	unsigned char * data;
	m_model->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&data );
	D3DXVECTOR3 center;
	memcpy(&center, &m_iniBall.m_center, sizeof(D3DXVECTOR3));
	D3DXComputeBoundingSphere((D3DXVECTOR3 *)data,m_model->GetNumVertices(), D3DXGetFVFVertexSize(m_model->GetFVF()), 
		&center, &m_iniBall.m_radius);
	m_model->UnlockVertexBuffer();
	m_referBall = m_iniBall;
	m_referBall.m_radius =	m_iniBall.m_radius * BALL_SCALE; //稍微缩放，提高精度
	m_ball = m_referBall;

#ifdef DEX_DEBGU
	D3DXCreateSphere(m_device, m_ball.m_radius, 20, 20, &m_sphere, NULL);
#endif

}

CModelX::~CModelX()
{
	ShutDown();
}
void CModelX::ShutDown()
{
	_SafeRelease(m_model);
	_SafeRelease(m_matBuffer); 
	_SafeDeleteArr(m_matList);
	for(DWORD i = 0 ; i < m_numMaterials; i++)
		_SafeRelease(m_textureList[i]);
	
}

void CModelX::Update()
{
	if(m_VelX != 0 || m_VelY != 0 || m_VelZ != 0)
	{
		m_posX += m_VelX;
		m_posY += m_VelY;
		m_posZ += m_VelZ;

		m_ball.m_center.x = m_referBall.m_center.x + m_posX;   //中心要实时移动
		m_ball.m_center.y = m_referBall.m_center.y + m_posY;
		m_ball.m_center.z = m_referBall.m_center.z + m_posZ;

		SetPosition(m_posX, m_posY, m_posZ);
	}

}
void CModelX::Render()
{
	D3DXMATRIX translation;
	D3DXMATRIX scalMatrix;
	D3DXMATRIX rotation;
	D3DXMATRIX worldMatrix;

	////m_device->SetRenderState()	
	//D3DXMatrixTranslation(&translation, m_posX, m_posY, m_posZ);
	//	static float rotate = 0.0f;	
	//	D3DXMatrixRotationYawPitchRoll(&rotation, m_rotateY, m_rotateX, m_rotateZ);	   //先绕Z轴旋转，再绕X轴旋转，最后绕Y轴旋转

	//	D3DXMatrixScaling(&scalMatrix, m_scaleX, m_scaleY, m_scaleZ);
	//	D3DXMatrixMultiply(&worldMatrix,&rotation,&scalMatrix);
	//	D3DXMatrixMultiply(&worldMatrix,&worldMatrix,&translation);
		
	//CalCulateMatrix();
	
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	if(m_cullNone)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	for(DWORD i = 0; i < m_numMaterials; i++)
	{
		m_device->SetTransform(D3DTS_WORLD, &m_Matrix);
		m_device->SetMaterial(&m_matList[i]);
		m_device->SetTexture(0, m_textureList[i]);

		m_model->DrawSubset(i);
	}
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	if(m_cullNone)
		m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	if(m_renderSphere)
		RenderShpere();
	//矩阵重置，否则其他的渲染会受影响

	D3DXMATRIX _translation;
	D3DXMatrixTranslation(&_translation, 0, 0, 0);
	m_device->SetTransform(D3DTS_WORLD, &_translation);
}
//void CModelX::SetPositionX(float xPos)
//{
//	m_posX = xPos;
//}
//void CModelX::SetPositionY(float yPos)
//{
//	m_posY = yPos;
//}
//void CModelX::SetPositionZ(float zPos)
//{
//	m_posZ = zPos;
//}
//void CModelX::SetPosition(float xPos, float yPos, float zPos)
//{
//	m_posX = xPos;
//	m_posY = yPos;
//	m_posZ = zPos;
//}
//
//void CModelX::SetPosition(D3DXVECTOR3 pos)
//{
//	m_posX = pos.x;
//	m_posY = pos.y;
//	m_posZ = pos.z;
//}
//void CModelX::SetScaleX(float xScale)
//{
//	m_scaleX = xScale;
//}
//void CModelX::SetScaleY(float yScale)
//{
//	m_scaleY = yScale;
//}
//void CModelX::SetScaleZ(float zScale)
//{
//	m_scaleZ = zScale;
//}
//void CModelX::SetScale(float xScale, float yScale, float zScale)
//{
//	m_scaleX = xScale;
//	m_scaleY = yScale;
//	m_scaleZ = zScale;
//}
//void CModelX::SetScale(float scale)
//{
//	m_scaleX = scale;
//	m_scaleY = scale;
//	m_scaleZ = scale;
//}
//void CModelX::SetRotateX(float xRotate)
//{
//	m_rotateX = xRotate;
//}
//void CModelX::SetRotateY(float yRotate)
//{
//	m_rotateY = yRotate;
//}
//void CModelX::SetRotateZ(float zRotate)
//{
//	m_rotateZ = zRotate;
//}
//void CModelX::SetRotate(float xRotate, float yRotate, float zRotate)
//{
//	m_rotateX = xRotate;
//	m_rotateY = yRotate;
//	m_rotateZ = zRotate;
//}