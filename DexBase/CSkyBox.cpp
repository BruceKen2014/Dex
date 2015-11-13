



#include "CSkyBox.h"



CSky::CSky()
{
	m_device = NULL;
	m_VertexBuffer = NULL;
	m_render  = true;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

CSky::~CSky()
{
}

bool CSky::GetRender()
{
	return m_render;
}

void CSky::SetRender(bool render)
{
	m_render = render;
}

D3DXVECTOR3 CSky::GetPosition()
{
	return m_position;
}

void CSky::SetPosition(D3DXVECTOR3 position)
{
	m_position = position;
}

void CSky::SetPosition(float posX, float posY,float posZ)
{
	m_position = D3DXVECTOR3(posX, posY, posZ);
}

///////////////////////////////////////////////////////////

CSkyBox::CSkyBox():CSky()
{
	for(short i = 0 ; i < SKY_FACE_COUNT; i++)
		m_texture[i] = NULL;
	m_width = m_height = m_length = 0;
}

CSkyBox::~CSkyBox()
{
	ShutDown();
}

bool CSkyBox::Check()
{
	if(m_device == NULL)
		return false;
	return true;
}

void CSkyBox::ShutDown()
{
	for(short i = 0;i < SKY_FACE_COUNT; i++)
		_SafeRelease(m_texture[i]);
	_SafeRelease(m_VertexBuffer);
}

bool CSkyBox::Initialize(LPDIRECT3DDEVICE9 device)
{
	if(device == NULL)
		return false;
	m_device = device;
	if(FAILED(m_device->CreateVertexBuffer(SKY_VERTEX_COUNT * sizeof(Sky_Vertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, SKY_XYZ_TEX1, D3DPOOL_DEFAULT,
		&m_VertexBuffer, NULL))) 
		return false;
	return true;
}

void CSkyBox::SetSize(float width, float height, float length)
{
	if(width < 0)
		m_width = 0;
	else
		m_width = width;
	if(height < 0)
		m_height = 0;
	else
		m_height = height;
	if(length < 0)
		m_length = 0;
	else
		m_length = length;
	CalCoor();
}

void CSkyBox::CalCoor()
{
	for(short i = 0 ; i < SKY_VERTEX_COUNT; i++ )
		m_vertex[i].m_pos = m_position;

	m_vertex[1].m_pos.y -= m_height;

	m_vertex[2].m_pos.x += m_width;
	m_vertex[2].m_pos.y -= m_height;

	m_vertex[3].m_pos.x += m_width;

	m_vertex[4].m_pos.z += m_length;

	m_vertex[5].m_pos.y  = m_vertex[1].m_pos.y;
	m_vertex[5].m_pos.z  = m_vertex[4].m_pos.z;

	m_vertex[6].m_pos.x  = m_vertex[2].m_pos.x;
	m_vertex[6].m_pos.y  = m_vertex[5].m_pos.y;
	m_vertex[6].m_pos.z  = m_vertex[5].m_pos.z;

    m_vertex[7].m_pos.x  = m_vertex[2].m_pos.x;
	m_vertex[7].m_pos.z  = m_vertex[4].m_pos.z;
}


void CSkyBox::SetFace(SKY_FACE sky_face, char *filename)
{
	switch(sky_face)
	{
	case SF_FRONT:
		_SafeRelease(m_texture[SF_FRONT]);
		D3DXCreateTextureFromFile(m_device, filename, &m_texture[SF_FRONT]);
		break;
	case SF_BACK:
		_SafeRelease(m_texture[SF_BACK]);
		D3DXCreateTextureFromFile(m_device, filename, &m_texture[SF_BACK]);
		break;
	case SF_LEFT:
		_SafeRelease(m_texture[SF_LEFT]);
		D3DXCreateTextureFromFile(m_device, filename, &m_texture[SF_LEFT]);
		break;
	case SF_RIGHT:
		_SafeRelease(m_texture[SF_RIGHT]);
		D3DXCreateTextureFromFile(m_device, filename, &m_texture[SF_RIGHT]);
		break;
	case SF_BOTTOM:
		_SafeRelease(m_texture[SF_BOTTOM]);
		D3DXCreateTextureFromFile(m_device, filename, &m_texture[SF_BOTTOM]);
		break;
	case SF_TOP:
		_SafeRelease(m_texture[SF_TOP]);
		D3DXCreateTextureFromFile(m_device, filename, &m_texture[SF_TOP]);
		break;
	default:
		break;
	}
}

void CSkyBox::Render()
{
	Sky_Vertex  temp[4];
	for(int short i = 0; i < SKY_FACE_COUNT; i++)
	{
		//按照前后左右上下的顺序渲染
		switch(i)
		{
		case 0:	//前
			temp[0].m_pos = m_vertex[1].m_pos;
			temp[1].m_pos = m_vertex[0].m_pos;
			temp[2].m_pos = m_vertex[2].m_pos;
			temp[3].m_pos = m_vertex[3].m_pos;
			m_device->SetTexture(0, m_texture[SF_FRONT]);
			break;
		case 1://后
			temp[0].m_pos = m_vertex[6].m_pos;
			temp[1].m_pos = m_vertex[7].m_pos;
			temp[2].m_pos = m_vertex[5].m_pos;
			temp[3].m_pos = m_vertex[4].m_pos;
			m_device->SetTexture(0, m_texture[SF_BACK]);
			break;
		case 2://左
			temp[0].m_pos = m_vertex[5].m_pos;
			temp[1].m_pos = m_vertex[4].m_pos;
			temp[2].m_pos = m_vertex[1].m_pos;
			temp[3].m_pos = m_vertex[0].m_pos;
			m_device->SetTexture(0, m_texture[SF_LEFT]);
			break;
		case 3://右
			temp[0].m_pos = m_vertex[2].m_pos;
			temp[1].m_pos = m_vertex[3].m_pos;
			temp[2].m_pos = m_vertex[6].m_pos;
			temp[3].m_pos = m_vertex[7].m_pos;
			m_device->SetTexture(0, m_texture[SF_RIGHT]);
			break;
		case 4://上
			temp[0].m_pos = m_vertex[0].m_pos;
			temp[1].m_pos = m_vertex[4].m_pos;
			temp[2].m_pos = m_vertex[3].m_pos;
			temp[3].m_pos = m_vertex[7].m_pos;
			m_device->SetTexture(0, m_texture[SF_TOP]);
			break;
		case 5://下
			temp[0].m_pos = m_vertex[1].m_pos;
			temp[1].m_pos = m_vertex[5].m_pos;
			temp[2].m_pos = m_vertex[2].m_pos;
			temp[3].m_pos = m_vertex[6].m_pos;
			m_device->SetTexture(0, m_texture[SF_BOTTOM]);
			break;
		default:
			break;
		}
		float l1 = 0.0f;
		float l2 = 1.0f;
		temp[0].tu = l1;
		temp[0].tv = l2;	
		temp[1].tu = l1;
		temp[1].tv = l1;
		temp[2].tu = l2;
		temp[2].tv = l2;
		temp[3].tu = l2;
		temp[3].tv = l1;
			
		Sky_Vertex tem = temp[1];
		temp[1] = temp[2];
		temp[2] = tem;
		void *ptr;
		if( FAILED(m_VertexBuffer->Lock( 0, 4*sizeof(Sky_Vertex), (void**)&ptr, 0 ) ) )
			return;	
		memcpy(ptr, temp, 4 * sizeof(Sky_Vertex));
		if( FAILED(m_VertexBuffer->Unlock() ) ) 
			return;

		m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);   //消除天空盒的裂缝问题
		m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

		m_device->SetRenderState(D3DRS_LIGHTING, false);
		m_device->SetStreamSource( 0, m_VertexBuffer, 0, sizeof(Sky_Vertex));	
		m_device->SetFVF(SKY_XYZ_TEX1);
		m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
	} 
	
	//这里要将UV设置还原，否则会影响后续的纹理渲染
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP); 
	m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGround::CGround()
{
	m_device = NULL;	
	m_VertexBuffer = NULL;
	m_texture = NULL;
	
	m_xPos = 0;
	m_yPos = 0;
	m_zPos =0;

	m_Width = 0;
	m_Length = 0;
}

CGround::~CGround()
{
	ShutDown();
}

void CGround::ShutDown()
{
	_SafeRelease(m_VertexBuffer);
	_SafeRelease(m_texture);
}

bool CGround::Initialize(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_device = device;
	m_xPos = pos.x;
	m_yPos = pos.y;
	m_zPos = pos.z;

	m_Width = size.x;
	m_Length = size.y;

	D3DXVECTOR3 position;
	position.x = m_xPos;
	position.y = m_yPos;
	position.z = m_zPos;

	for(short i = 0 ; i < 4; i++ )
		m_vertex[i].m_pos = position;

	m_vertex[0].tu = 0.0f;
	m_vertex[0].tv = 50.0f;

	m_vertex[1].m_pos.z += m_Length;
	m_vertex[1].tu = 0.0f;
	m_vertex[1].tv = 0.0f;

	m_vertex[2].m_pos.x += m_Width;
	m_vertex[2].tu = 50.0f;
	m_vertex[2].tv = 50.0f;

	m_vertex[3].m_pos.x += m_Width;
	m_vertex[3].m_pos.z += m_Length;
	m_vertex[3].tu = 50.0f;
	m_vertex[3].tv = 0.0f;

	if(FAILED(m_device->CreateVertexBuffer(4 * sizeof(Sky_Vertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, SKY_XYZ_TEX1, D3DPOOL_DEFAULT,
		&m_VertexBuffer, NULL))) 
		return false;

	void *ptr;
	if( FAILED(m_VertexBuffer->Lock( 0, 4*sizeof(Sky_Vertex), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, m_vertex, 4 * sizeof(Sky_Vertex));
	if( FAILED(m_VertexBuffer->Unlock() ) ) 
		return false;
	return true;
}

void CGround::SetTexture(char *filename)
{
	D3DXCreateTextureFromFile(m_device, filename, &m_texture);
}

void CGround::Render()
{
	m_device->SetTexture(0, m_texture);
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	m_device->SetStreamSource( 0, m_VertexBuffer, 0, sizeof(Sky_Vertex));	
	m_device->SetFVF(SKY_XYZ_TEX1);
	m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
}
