

#include "CWate.h"

CWaterF::CWaterF()
{
	m_device = NULL;
	m_VertexBuffer = NULL;
	m_pTexList.clear();
	m_position = D3DXVECTOR3(0,0,0);
	m_width = 0;
	m_length = 0;
	m_curr = 0;
	m_delay = 0;
}
CWaterF::~CWaterF()
{
	ShutDown();
}

bool CWaterF::Init(LPDIRECT3DDEVICE9 device, const char *filename, int width, int length)
{	 
	DEX_ENSURE_B(device);
	m_device = device;
	if(FAILED(m_device->CreateVertexBuffer(4 * sizeof(stVertex2),
	   D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_T1_N, D3DPOOL_DEFAULT,
	   &m_VertexBuffer, NULL))) 
	   return false;
	stTexture* tex = new stTexture(device, filename);
	m_pTexList.push_back(tex);
	int tex_width = tex->m_iWidth;
	int tex_height = tex->m_iHeight;
	m_width = width;
	m_length = length;

	int zheight= 0;
	m_vectex[0].m_pos = D3DXVECTOR3(0, zheight, length);
	m_vectex[0].SetUV(0,0);

	m_vectex[1].m_pos = D3DXVECTOR3(width, zheight, length);
	m_vectex[1].SetUV((float(m_width))/tex_width,0);

	m_vectex[2].m_pos = D3DXVECTOR3(0, zheight, 0);
	m_vectex[2].SetUV(0, (float(m_length))/tex_height);

	m_vectex[3].m_pos = D3DXVECTOR3(width, zheight, 0);
	m_vectex[3].SetUV((float(m_width))/tex_width,(float(m_length))/tex_height);
	for(int i = 0; i < 4 ; i++)
		m_vectex[0].m_normal = D3DXVECTOR3(0, -1, 0);

	void *ptr;
	if( FAILED(m_VertexBuffer->Lock( 0, 4*sizeof(stVertex2), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, m_vectex, 4 * sizeof(stVertex2));
	if( FAILED(m_VertexBuffer->Unlock() ) ) 
		return false;
	return true;
}

void CWaterF::SetPos(const D3DXVECTOR3& pos)
{
	m_position = pos;
}

void CWaterF::AddTexture(const char*filename)
{
	stTexture* tex = new stTexture(m_device, filename);
	m_pTexList.push_back(tex);
}
void CWaterF::Update()
{
	if(m_delay++ > WATER_DELAY)
	{
		if(++m_curr >= m_pTexList.size())
			m_curr = 0;
		m_delay = 0;
	}
}

void CWaterF::Render()
{
	//m_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//m_device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//m_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_ADD);
	D3DXMATRIX g_translation;
	D3DXMatrixTranslation(&g_translation, m_position.x, m_position.y, m_position.z);
	m_device->SetTransform(D3DTS_WORLD, &g_translation);
	if(m_pTexList[m_curr]->m_pTexture != NULL)
	    m_device->SetTexture(0, m_pTexList[m_curr]->m_pTexture);
	m_device->SetRenderState(D3DRS_LIGHTING, true);
	m_device->SetStreamSource( 0, m_VertexBuffer, 0, sizeof(stVertex2));	
	m_device->SetFVF(FVF_XYZ_T1_N);
	m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	m_device->SetTexture(0, NULL);
	D3DXMatrixTranslation(&g_translation, 0.0f, 0.0f, 0.0f);
	m_device->SetTransform(D3DTS_WORLD, &g_translation);
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	//m_device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//m_device->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
}

void CWaterF::ShutDown()
{
	m_device = NULL;
	_SafeRelease(m_VertexBuffer);
	for(size_t i = 0; i <  m_pTexList.size(); i++)
		_SafeDelete(m_pTexList[i]);
}