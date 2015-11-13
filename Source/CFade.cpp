





#include "CFade.h"


CFade::CFade()
{
	//m_vertex  = NULL;
	m_device  = NULL;
	m_pVB     = NULL;
	m_fadeIn  = true;     //默认正在淡入
	m_fadeOut = false;

	m_r     = 255;
	m_g     = 255;
	m_b     = 0;

	m_alpha = 255;

	m_width = 100;
	m_height= 100;
}
CFade::~CFade()
{
	ShutDown();
}
bool CFade::Check()
{
	if(m_device == NULL)
		return false;
	return true;
}

void CFade::ShutDown()
{
	_SafeRelease(m_pVB);
}

void CFade::Initialize(LPDIRECT3DDEVICE9 device)
{
	m_device = device;

	if(FAILED(m_device->CreateVertexBuffer(4 * sizeof(stVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_XYZ_DIFFUSE, D3DPOOL_DEFAULT,
		&m_pVB, NULL))) return ;

	PrepareFade();
}

void CFade::SetFadeColor(int r, int g, int b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

void CFade::SetFadeIn(bool fade_in /* = true */)
{
	if(fade_in)
	{
		m_alpha  = 255;
		m_fadeIn = true;
		m_fadeOut = false;	//淡入的同时不可能再淡出
	}
}
void CFade::SetFadeOut(bool fade_out /* = true */)
{
	if(fade_out)
	{	
		m_alpha   = 0;
		m_fadeOut = true;	
		m_fadeIn  = false;
	}
}

void CFade::SetRange(int width, int height)
{
	if(width < 0 || width > 1024)
		m_width = 100;
	else 
		m_width = width;

	if(height < 0 || height > 768)
		m_height = 100;
	else
		m_height = height;
}

bool CFade::FadeComplete()
{
	if(m_fadeIn)
	{
		if(m_alpha <= 0)
		{	
			SetFadeOut();
			return true;
		}
		return false;
	}
	if(m_fadeOut)
	{
		if(m_alpha >= 255)
		{  
			SetFadeIn();
			return true;
		}
		return false;
	}
	return true;
}

void CFade::PrepareFade()
{
	D3DXMATRIX mat;

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	D3DXMatrixOrthoLH(&mat, m_width, m_height, 0.0, 100.0);
	m_device->SetTransform( D3DTS_PROJECTION, &mat );

	// this world matrix, combined with orthogonal projection, causes the 
	// texture to completely and exactly fill the rendering surface.
	D3DXMATRIX matWorld,matTrans,matScale;
	D3DXMatrixScaling(&matScale, m_width, m_height, 1.0);

	// move the quad left and up 0.5 units, so that the texels are perfectly
	// centered on the screen pixels.
	D3DXMatrixMultiply(&matWorld, &matScale, D3DXMatrixTranslation(&matTrans, -0.5f, -0.5f, 0));

	//D3DXMatrixMultiply(&matWorld, &matWorld, &matScale);
	m_device->SetTransform( D3DTS_WORLD, &matWorld );
}
void CFade::Update()
{
	
	//m_alpha = 255;

	FadeComplete();
	//if(FadeComplete())
	//	return;
	if(m_fadeIn)
		m_alpha--;
	if(m_fadeOut) 
		m_alpha++;
}

void CFade::Render()
{
	m_vertex[0].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_vertex[0].color = DexColor( m_r, m_g, m_b,m_alpha);
	//m_vertex[0].tu = m_vertex[0].tv = 1.0;

	m_vertex[1].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	m_vertex[1].color = DexColor(m_r, m_g, m_b,m_alpha);
	//m_vertex[1].tu = m_vertex[1].tv = 1.0;

	m_vertex[2].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	m_vertex[2].color = DexColor(m_r, m_g, m_b,m_alpha);
	//m_vertex[2].tu = m_vertex[2].tv = 1.0;

	m_vertex[3].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);	
	m_vertex[3].color = DexColor(m_r, m_g, m_b,m_alpha);
	//m_vertex[3].tu = m_vertex[3].tv = 1.0;

	void *ptr;
	if( FAILED(m_pVB->Lock( 0, 4*sizeof(stVertex), (void**)&ptr, 0 ) ) )
		return;	

	memcpy(ptr, m_vertex, 4 * sizeof(stVertex));

	if( FAILED(m_pVB->Unlock() ) ) 
		return;

	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	m_device->SetStreamSource( 0, m_pVB, 0, sizeof(stVertex));	
	m_device->SetFVF(D3DFVF_XYZ_DIFFUSE);
	m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	//D3DXMATRIX mat2;
	//D3DXMatrixPerspectiveFovLH(&mat2, 45.0f,
	//	float(640)/float(480), 0.1f, 1000.0f);

	//m_device->SetTransform(D3DTS_PROJECTION, &mat2);
}