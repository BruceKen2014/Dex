

#include "CSpell.h"

CSpell::CSpell(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_pos  = D3DXVECTOR3(0.0, 0.0, 0.0);
	m_texture = NULL;
	m_vb = NULL;
	m_clock = true;
	m_speed = 1.0f;
	m_yRotate = 0.0f;
	m_width = m_length = 0.0f;
}

CSpell::~CSpell()
{
	ShutDown();
}

bool CSpell::LoadTexture(char *filename)
{
	_SafeRelease(m_texture);
	if(FAILED(D3DXCreateTextureFromFile(m_device, filename, &m_texture)))
	{
		_Message("加载纹理%s失败!", filename);
		return false;
	}
	return true;
}

bool CSpell::CreateVB()
{  
	if(FAILED(m_device->CreateVertexBuffer(4 * sizeof(stVertex1),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_vb, NULL))) 
		return false;
	stVertex1 temp[4];
	temp[0].m_pos = D3DXVECTOR3(-m_width/2, 0, -m_length/2);
	temp[1].m_pos = D3DXVECTOR3(-m_width/2, 0, m_length/2);
	temp[2].m_pos = D3DXVECTOR3(m_width/2, 0, -m_length/2);
	temp[3].m_pos = D3DXVECTOR3(m_width/2, 0, m_length/2);

	temp[0].SetUV(0.0f, 1.0f); 
	temp[1].SetUV(0.0f, 0.0f); 
	temp[2].SetUV(1.0f, 1.0f); 
	temp[3].SetUV(1.0f, 0.0f); 

	void *ptr;
	if( FAILED(m_vb->Lock( 0, 4 * sizeof(stVertex1), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, temp, 4 * sizeof(stVertex1));
	if( FAILED(m_vb->Unlock() ) ) 
		return false; 
	return true;
}
void CSpell::ShutDown()
{
	_SafeRelease(m_texture);
	_SafeRelease(m_vb);
}

void CSpell::Update()
{
	m_yRotate += m_speed;
	D3DXMATRIX rotate;
	D3DXMATRIX trans;
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&trans);
	D3DXMatrixIdentity(&m_matrix);
	//在原点的矩阵旋转
	D3DXMATRIX rotateY;
	D3DXMatrixIdentity(&rotateY);
	D3DXMatrixRotationY(&rotateY, m_yRotate);
	D3DXMatrixMultiply(&rotate, &rotate, &rotateY);	
	//平移到原点
	D3DXMatrixTranslation(&trans, 0, 0, 0);	 
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &trans);

	//旋转
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &rotate);	 



	//最后平移到物体位置
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_Matrix, &rotate, &trans);	
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &trans);	
}

void CSpell::Render()
{
	if(m_device == NULL)
		return;
	//半透明纹理混合
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	//设置alpha值（把纹理alpha和顶点alpha进行乘法运算）
	//m_device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE ); 
	//m_device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); 
	//m_device->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );

	m_device->SetTransform( D3DTS_WORLD, &m_matrix);
	//m_device->SetRenderState(D3DRS_ALPHAREF, 0x01);
	//m_device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	//m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetStreamSource(0,m_vb,0,sizeof(stVertex1));
	m_device->SetFVF(FVF_XYZ_DIFF_TEX1);
	m_device->SetTexture(0,m_texture);
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, 0, 0, 0);
	m_device->SetTransform( D3DTS_WORLD, &trans);
	m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);   
	//m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
}

void CSpell::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CSpell::SetPos(float x, float y, float z)
{
	m_pos.x = x;
	m_pos.y = y;
	m_pos.z = z;
}

D3DXVECTOR3 CSpell::GetPos()
{
	return m_pos;
}

void CSpell::SetClockRotate(bool b)
{
	m_clock = b;
}

bool CSpell::GetClockRotate()
{
	return m_clock;
}

void CSpell::SetSpeed(float f)
{
	m_speed = f;
}

float CSpell::GetSpeed()
{
	return m_speed;
}

void CSpell::SetSize(float width, float length)
{
	m_width = width;
	m_length = length;
}