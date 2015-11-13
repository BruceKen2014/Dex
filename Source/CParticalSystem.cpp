


#include "CParticalSystem.h"
#include "../DexBase/DexLog.h"


/////////////////////////////////////////////////////////////////////
//��Ч����:CParticalSystem
CParticalSystem::CParticalSystem()
{
	m_FVF          = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_particalBuffer = NULL;
	m_texture = NULL;
	m_color = PARTICAL_COLOR;
	m_isUniqueColor = false;
	m_render        = true;
	m_id            = 0;
}

bool CParticalSystem::LoadTexture(char* filename)
{
	_SafeRelease(m_texture);
	if(FAILED(D3DXCreateTextureFromFile(m_D3DDevice, filename, &m_texture)))
	{
		_Message("��������%sʧ��!", filename);
		return false;
	}
	return true;
}
void CParticalSystem::SetIsUniqueColor(bool isbool)
{
	m_isUniqueColor = isbool;
}
void CParticalSystem::SetRender(bool render)
{
	m_render = render;
}

void CParticalSystem::SetPosition(D3DXVECTOR3 pos)
{
	m_posX = pos.x;
	m_posY = pos.y;
	m_posZ = pos.z;
}
bool CParticalSystem::GetIsUniqueColor()const
{
	return m_isUniqueColor ;
}
bool CParticalSystem::GetRender()const
{
	return m_render ;
}
/////////////////////////////////////////////////////////////////////
//�̻���Ч��:CFireworks
CFireworks::CFireworks()
{
}
CFireworks::CFireworks(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount):CParticalSystem()
{
	m_D3DDevice  =  g_D3DDevice ;         //�豸ָ��
	m_posX   =  posX; 
	m_posY   =  posY;
	m_posZ   =	posZ;
	m_particalCount = particalCount;           //��������
	if(particalCount < 0)		//����������
		m_particalCount = 0;
    m_particalList = NULL;
	m_type  = PS_FIREWORK;
	m_size  = PARTICAL_SIZE;
	m_velocity = PARTICAL_VELOCITY;
}
CFireworks::~CFireworks()
{
	ShutDown();
}

void CFireworks::ShutDown()
{
	if(m_particalList != NULL)
		delete[] m_particalList;
	if(m_particalBuffer != NULL)
	{
		m_particalBuffer->Release();
		m_particalBuffer = NULL;
	}
}

bool CFireworks::Check()
{
	if(m_D3DDevice == NULL)
		return false;
	if(m_particalList == NULL)
		return false;
	if(m_particalBuffer == NULL)
		return false;
	return true;
}

bool CFireworks::CreatePartical()
{
	for(int i = 0; i < m_particalCount; i++)
	{
		m_particalList[i].m_size = m_size;
		m_particalList[i].m_posX = m_posX;
		m_particalList[i].m_posY = m_posY;
		m_particalList[i].m_posZ = m_posZ;
		if(m_launchDistance == 0)
		{//��ʱ�ͱ�ը
			float arf = rand()%180;
			float thta = rand()%180;
			m_particalList[i].m_velocityX = m_velocity * cos(arf) * cos(thta);
			m_particalList[i].m_velocityY = m_velocity * cos(arf) * sin(thta);
			m_particalList[i].m_velocityZ = m_velocity * sin(arf);
		}
		else
		{//�ȷ��䣬��ը  
			m_particalList[i].m_velocityX = m_velocity * m_launchDirection.x;
		    m_particalList[i].m_velocityY = m_velocity * m_launchDirection.y;
		    m_particalList[i].m_velocityZ = m_velocity * m_launchDirection.z;
		}
		m_particalList[i].m_deltaX = m_particalList[i].m_deltaY = m_particalList[i].m_deltaZ = 0;
	} 
	SetColor();
	return true;
}

bool CFireworks::Initial()
{
	m_particalList = new CPartical[m_particalCount];

	if(FAILED(m_D3DDevice->CreateVertexBuffer(m_particalCount * sizeof(CPartical),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, D3DFVF_PARTICAL, D3DPOOL_DEFAULT,
		&m_particalBuffer, NULL))) return false;

	void *ptr2;

	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CPartical),
		(void**)&ptr2, 0))) return false;

	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CPartical));

	m_particalBuffer->Unlock();
	return CreatePartical();
}

void CFireworks::Render()
{
	if(!m_render)
		return;
	if(!Check())
		return;
	void *ptr2;
	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CPartical),
		(void**)&ptr2, 0))) return ;
	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CPartical));
	m_particalBuffer->Unlock();

	m_D3DDevice->SetTexture(0, m_texture);

	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_size));
	//g_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX,FtoDW(10.0f ));
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(5.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	//���þ��󣬷�ֹ��֮ǰ��Ⱦ�����Ӱ��
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, 0, 0, 0);
	m_D3DDevice->SetTransform(D3DTS_WORLD, &translation);
	m_D3DDevice->SetStreamSource(0, m_particalBuffer,
		0, sizeof(CPartical));
	m_D3DDevice->SetFVF(D3DFVF_PARTICAL);
	m_D3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_particalCount);

	//��Ⱦ��Ļ�ԭ����
	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CFireworks::UpdatePartical()
{
	if(!Check())
		return;
	if(m_nowDistance >= m_launchDistance)
	{//��ը������Ӹ���
		for(int i = 0; i < m_particalCount; i++)
		{
			float temp =pow((m_particalList[i].m_posX - m_explodePoint.x), 2) + 
				pow((m_particalList[i].m_posY - m_explodePoint.y), 2) + 
				pow((m_particalList[i].m_posZ - m_explodePoint.z), 2) ;
			if(temp
		       > m_explodeRadius*m_explodeRadius)
			{
			//������������ԭ����Щ���ӳ�����ը�뾶ʱ������Щû��
			//�����������Щ���ڷ��䣬����Щ���ڱ�ը�����
			//���Դ˴�һ�������ӳ�����ը�뾶,�������������ӽ�������
				for(int j = 0; j < m_particalCount; j++)
				{
					m_particalList[j].m_posX = m_posX;
					m_particalList[j].m_posY = m_posY;
					m_particalList[j].m_posZ = m_posZ;
					if(m_launchDistance == 0)
					{//��ʱ�ͱ�ը
						float arf  = ((float)(rand()%1800))/10;
						float thta = ((float)(rand()%1800))/10;
						m_particalList[j].m_velocityX = m_velocity * cos(arf) * cos(thta);
						m_particalList[j].m_velocityY = m_velocity * cos(arf) * sin(thta);
						m_particalList[j].m_velocityZ = m_velocity * sin(arf);
					}
					else
					{//�ȷ��䣬��ը  
						m_particalList[j].m_velocityX = m_velocity * m_launchDirection.x;
						m_particalList[j].m_velocityY = m_velocity * m_launchDirection.y;
						m_particalList[j].m_velocityZ = m_velocity * m_launchDirection.z;
					}
				}
				m_nowDistance = 0;
				break;
			}
			m_particalList[i].m_posX += m_particalList[i].m_velocityX;
			m_particalList[i].m_posY += m_particalList[i].m_velocityY;
			m_particalList[i].m_posZ += m_particalList[i].m_velocityZ;
		}
	}
	else
	{//��ըǰ�����Ӹ���	 
		m_nowDistance += m_velocity;
		for(int i = 0; i < m_particalCount; i++)
		{
			
			m_particalList[i].m_posX += m_particalList[i].m_velocityX;
			m_particalList[i].m_posY += m_particalList[i].m_velocityY;
			m_particalList[i].m_posZ += m_particalList[i].m_velocityZ;

		}		
		if(m_nowDistance >= m_launchDistance)
		{//���������ٶ�
			for(int i = 0; i < m_particalCount; i++)
			{			
				float arf  = ((float)(rand()%1800))/10;
			    float thta = ((float)(rand()%1800))/10;
				m_particalList[i].m_velocityX = m_velocity * cos(arf) * cos(thta);
				m_particalList[i].m_velocityY = m_velocity * cos(arf) * sin(thta);
				m_particalList[i].m_velocityZ = m_velocity * sin(arf);
			}
		}
	}
}

void CFireworks::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_texture = texture;
}

void CFireworks::SetParticalVelocity(float velocity)
{
	m_velocity = velocity;
}

void CFireworks::SetParticalSize(float size)
{
	m_size = size;
}
void CFireworks::SetColor(DexColor color /*= PARTICAL_COLOR*/)
{
	m_color = color;
	if(!Check())
		return;
	if(m_isUniqueColor)
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
			m_particalList[i].m_color = m_color; 
		}
	}
	else
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
            m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		}
	}
}

void CFireworks::SetFirework(D3DXVECTOR3 direction, float distance, float radius)
{;
	D3DXVec3Normalize(&m_launchDirection, &direction);   //������λ��
	m_launchDistance  = distance;
	m_nowDistance     = 0;
	m_explodeRadius   = radius;
	//���㱬ը������
	m_explodePoint.x  = m_posX + distance * m_launchDirection.x;
	m_explodePoint.y  = m_posY + distance * m_launchDirection.y;
	m_explodePoint.z  = m_posZ + distance * m_launchDirection.z;
}



////////////////////////////////////////////////////////////////////
//��ѩ��Ч��:CRainSystem
CRainSystem::CRainSystem()
{
}
CRainSystem::CRainSystem(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount):CParticalSystem()
{
	m_D3DDevice  =  g_D3DDevice ;         //�豸ָ��
	m_posX   =  posX; 
	m_posY   =  posY;
	m_posZ   =	posZ;
	m_particalCount = particalCount;           //��������
	if(particalCount < 0)		//����������
		m_particalCount = 0;

	m_particalList = NULL;
	m_type  = PS_RAIN;
	m_size  = PARTICAL_SIZE;
	m_velocity = PARTICAL_VELOCITY;
}
CRainSystem::~CRainSystem()
{
	ShutDown();
}

void CRainSystem::ShutDown()
{
	_SafeDeleteArr(m_particalList);
	_SafeRelease(m_particalBuffer);
	_SafeRelease(m_texture);
}

bool CRainSystem::Check()
{
	if(m_D3DDevice == NULL)
		return false;
	if(m_particalList == NULL)
		return false;
	if(m_particalBuffer == NULL)
		return false;
	return true;
}

bool CRainSystem::CreatePartical()
{
	for(int i = 0; i < m_particalCount; i++)
	{
		m_particalList[i].m_size = m_size;
		m_particalList[i].m_posX = m_width * (RandFloat() - 0.5) + m_posX;
		m_particalList[i].m_posY = m_height * (RandFloat() - 0.5) + m_posY;
		m_particalList[i].m_posZ = m_depth * (RandFloat() - 0.5) + m_posZ;

		m_particalList[i].m_velocityX = RandFloat() * 0.1;
		m_particalList[i].m_velocityY = m_velocity + RandFloat() * m_velocity;
		m_particalList[i].m_velocityZ = 0;
		if(m_isUniqueColor)
			m_particalList[i].m_color = m_color;
		else
			m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		m_particalList[i].m_deltaX = m_particalList[i].m_deltaY = m_particalList[i].m_deltaZ = 0;
	}
	getLog()->BeginLog();
	getLog()->Log(log_ok, "��ʼ����ѩ����ϵͳ�ɹ���");
	getLog()->EndLog();
	return true;
}

bool CRainSystem::Initial()
{
	m_particalList = new CPartical[m_particalCount];

	if(FAILED(m_D3DDevice->CreateVertexBuffer(m_particalCount * sizeof(CPartical),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, D3DFVF_PARTICAL, D3DPOOL_DEFAULT,
		&m_particalBuffer, NULL))) return false;

	void *ptr2;

	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CPartical),
		(void**)&ptr2, 0))) return false;

	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CPartical));

	m_particalBuffer->Unlock();
	return CreatePartical();
}

void CRainSystem::Render()
{
	if(!m_render)
		return;
	if(!Check())
		return;
	void *ptr2;
	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CPartical),
		(void**)&ptr2, 0))) return ;
	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CPartical));
	m_particalBuffer->Unlock();

	m_D3DDevice->SetTexture(0, m_texture);

	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_size));
	//g_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX,FtoDW(10.0f ));
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.5f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	//���þ��󣬷�ֹ��֮ǰ��Ⱦ�����Ӱ��
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, 0, 0, 0);
	m_D3DDevice->SetTransform(D3DTS_WORLD, &translation);
	m_D3DDevice->SetStreamSource(0, m_particalBuffer,
		0, sizeof(CPartical));
	m_D3DDevice->SetFVF(D3DFVF_PARTICAL);
	m_D3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_particalCount);

	//��Ⱦ��Ļ�ԭ����
	m_D3DDevice->SetTexture(0, NULL);
	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CRainSystem::UpdatePartical()
{
	if(!Check())
		return;
	for(int i = 0; i < m_particalCount; i++)
	{
		m_particalList[i].m_posX += m_particalList[i].m_velocityX;
		m_particalList[i].m_posY -= m_particalList[i].m_velocityY;
		m_particalList[i].m_posZ += m_particalList[i].m_velocityZ;

		//����Խ�紦��
		if((m_particalList[i].m_posY > (m_posY + m_height/2) || m_particalList[i].m_posY <( m_posY - m_height/2)))

		{
			m_particalList[i].m_posX = m_width * (RandFloat() - 0.5) + m_posX;
			m_particalList[i].m_posY = m_height/2 + m_posY;
		    m_particalList[i].m_posZ = m_depth * (RandFloat() - 0.5) + m_posZ;
		}  			
		//|| (m_particalList[i].m_posY > (m_posY + m_height/2) || m_particalList[i].m_posX <( m_posY - m_height/2))
		//	|| (m_particalList[i].m_posZ > (m_posZ + m_depth/2) || m_particalList[i].m_posX <( m_posZ - m_depth/2)))
	}
}

void CRainSystem::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_texture = texture;
}
void CRainSystem::SetParticalVelocity(float velocity)
{
	m_velocity = velocity;
}

void CRainSystem::SetParticalSize(float size)
{
	m_size = size;
}
void CRainSystem::SetColor(DexColor color /*= PARTICAL_COLOR*/)
{
	m_color = color;
	if(!Check())
		return;
	if(m_isUniqueColor)
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
			m_particalList[i].m_color = m_color; 
		}
	}
	else
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
			m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		}
	}
}
void CRainSystem::SetRainPS(float width, float height, float _depth)
{
	m_width = width;
	m_height = height; 
	m_depth = _depth;
}
////////////////////////////////////////////////////////////////////
//������Ч��:CFireSystem
CFireSystem::CFireSystem()
{
}
CFireSystem::CFireSystem(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount):CParticalSystem()
{
	m_D3DDevice  =  g_D3DDevice ;         //�豸ָ��
	m_posX   =  posX; 
	m_posY   =  posY;
	m_posZ   =	posZ;
	m_particalCount = particalCount;           //��������
	if(particalCount < 0)		//����������
		m_particalCount = 0;

	m_particalList = NULL;
	m_type  = PS_FIRE;
	m_size  = PARTICAL_SIZE;
	m_velocity = PARTICAL_VELOCITY;
}
CFireSystem::~CFireSystem()
{
	ShutDown();
}

void CFireSystem::ShutDown()
{
	_SafeDeleteArr(m_particalList);
	_SafeRelease(m_particalBuffer);
	_SafeRelease(m_texture);
}

bool CFireSystem::Check()
{
	if(m_D3DDevice == NULL)
		return false;
	if(m_particalList == NULL)
		return false;
	if(m_particalBuffer == NULL)
		return false;
	return true;
}

bool CFireSystem::CreatePartical()
{
	D3DXVECTOR3 temp_vector;
	D3DXVECTOR3 normal_vector;
	for(int i = 0; i < m_particalCount; i++)
	{
		//����360�����ȡһ������������λ���Ժ���Ի���뾶����Ϊ���Ӿ�������ĵ�λ��
		//����λ�� = �������� + λ��
		temp_vector = D3DXVECTOR3(RandFloat(), RandFloat(), RandFloat()); 
		D3DXVec3Normalize(&normal_vector, &temp_vector);
		m_particalList[i].m_size = m_size;
		m_particalList[i].m_posX = m_posX + m_radius * normal_vector.x;
		m_particalList[i].m_posY = m_posY + m_radius * normal_vector.y;
		m_particalList[i].m_posZ = m_posZ + m_radius * normal_vector.z;

		m_particalList[i].m_velocityX = 0;
		m_particalList[i].m_velocityY = m_velocity + RandFloat() * m_velocity;	  //��ʱ��ֻ��Y�ٶ�
		m_particalList[i].m_velocityZ = 0;
		m_particalList[i].m_lifetime = m_particalList[i].m_currentime = RandFloat() * m_pow; 
		if(m_isUniqueColor)
			m_particalList[i].m_color = getD3DColor(m_color);
		else
			m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		m_particalList[i].m_deltaX = m_wave * normal_vector.x;
		m_particalList[i].m_deltaY = m_wave * normal_vector.y;
		m_particalList[i].m_deltaY = abs(m_particalList[i].m_deltaY);  //Y����ֻ�����ϲ���
		m_particalList[i].m_deltaZ = m_wave * normal_vector.z;;
	}
	getLog()->BeginLog();
	getLog()->Log(log_ok, "��ʼ����������ϵͳ�ɹ���");
	getLog()->EndLog();
	return true;
}

bool CFireSystem::Initial()
{
	m_particalList = new CFirePartical[m_particalCount];

	if(FAILED(m_D3DDevice->CreateVertexBuffer(m_particalCount * sizeof(CFirePartical),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, D3DFVF_PARTICAL, D3DPOOL_DEFAULT,
		&m_particalBuffer, NULL))) return false;

	void *ptr2;

	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CFirePartical),
		(void**)&ptr2, 0))) return false;

	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CFirePartical));

	m_particalBuffer->Unlock();
	return CreatePartical();
}

void CFireSystem::Render()
{
	if(!m_render)
		return;
	if(!Check())
		return;
	void *ptr2;
	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CFirePartical),
		(void**)&ptr2, 0))) return ;
	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CFirePartical));
	m_particalBuffer->Unlock();

	m_D3DDevice->SetTexture(0, m_texture);

	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	  
	m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	//m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_size));
	//g_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX,FtoDW(10.0f ));
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.5f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	//���þ��󣬷�ֹ��֮ǰ��Ⱦ�����Ӱ��
	//D3DXMATRIX translation;
	//D3DXMatrixTranslation(&translation, 0, 0, 0);
	//m_D3DDevice->SetTransform(D3DTS_WORLD, &translation);
	m_D3DDevice->SetStreamSource(0, m_particalBuffer,
		0, sizeof(CFirePartical));
	m_D3DDevice->SetFVF(m_FVF);
	m_D3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_particalCount);

	//��Ⱦ��Ļ�ԭ����
	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CFireSystem::UpdatePartical()
{
	if(!Check())
		return;	  
	D3DXVECTOR3 temp_vector;
	D3DXVECTOR3 normal_vector;
	for(int i = 0; i < m_particalCount; i++)
	{
		temp_vector = D3DXVECTOR3(RandFloat(), RandFloat(), RandFloat()); 
		D3DXVec3Normalize(&normal_vector, &temp_vector);
		m_particalList[i].m_deltaX = m_wave * normal_vector.x;
		m_particalList[i].m_deltaY = m_wave * normal_vector.y;
		//m_particalList[i].m_deltaY = abs(m_particalList[i].m_deltaY);  //Y����ֻ�����ϲ���
		m_particalList[i].m_deltaZ = m_wave * normal_vector.z;;

		//��ȡ�����Ӳ�������ٶ�
		m_particalList[i].m_velocityX += m_particalList[i].m_deltaX;
		m_particalList[i].m_velocityY += m_particalList[i].m_deltaY;
		m_particalList[i].m_velocityZ += m_particalList[i].m_deltaZ;
		//�ٸ����ٶȼ���λ��
		m_particalList[i].m_posX += m_particalList[i].m_velocityX;
		m_particalList[i].m_posY += m_particalList[i].m_velocityY;
		m_particalList[i].m_posZ += m_particalList[i].m_velocityZ;
		m_particalList[i].m_currentime--;
		if(m_particalList[i].m_currentime <= 0)
		{//������������
			temp_vector = D3DXVECTOR3(RandFloat(), RandFloat(), RandFloat()); 
			D3DXVec3Normalize(&normal_vector, &temp_vector);
			m_particalList[i].m_size = m_size;
			m_particalList[i].m_posX = m_posX + m_radius * normal_vector.x;
			m_particalList[i].m_posY = m_posY + m_radius * normal_vector.y;
			m_particalList[i].m_posZ = m_posZ + m_radius * normal_vector.z;

			m_particalList[i].m_velocityX = 0;
			m_particalList[i].m_velocityY = m_velocity + RandFloat() * m_velocity;	  //��ʱ��ֻ��Y�ٶ�
			m_particalList[i].m_velocityZ = 0;
			m_particalList[i].m_lifetime = m_particalList[i].m_currentime = RandFloat() * m_pow; 
			if(m_isUniqueColor)
				m_particalList[i].m_color = getD3DColor(m_color);
			else
				m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		}
		//�������ӵ���ɫ�仯
		if(m_isUniqueColor)
		{//��ɫ����,������ɫ��˥�����
			float stage = m_particalList[i].m_currentime/m_particalList[i].m_lifetime;
			short color_r = 255 * stage;
			if(color_r < m_r)
				color_r = m_r;
			short color_g = 255 * stage;
			if(color_g < m_g)
				color_g = m_g;
			short color_b = 255 * stage;
			if(color_b < m_b)
				color_b = m_b;
			m_particalList[i].m_color = D3DCOLOR_XRGB(color_r, color_g, color_b);
		}
	}
}

void CFireSystem::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_texture = texture;
}
void CFireSystem::SetParticalVelocity(float velocity)
{
	m_velocity = velocity;
}

void CFireSystem::SetParticalSize(float size)
{
	m_size = size;
}
void CFireSystem::SetColor(DexColor color /*= PARTICAL_COLOR*/)
{
	m_color = color;
	if(!Check())
		return;
	if(m_isUniqueColor)
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
			m_particalList[i].m_color = getD3DColor(m_color);
		}
	}
	else
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
			m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		}
	}
}
void CFireSystem::SetFireColor(short r, short g, short b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}
void CFireSystem::SetFirePS(float radius, float _pow, float wave, short r, short g, short b)
{
	m_radius = radius;
	m_pow = _pow; 
	m_wave = wave;
	m_r = r;
	m_g = g;
	m_b = b;
}
////////////////////////////////////////////////////////////////////
//�ٲ���Ч��:CWaterfall
/*CWaterfall::CWaterfall()
{
}
CWaterfall::CWaterfall(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount):CParticalSystem()
{
	m_D3DDevice  =  g_D3DDevice ;         //�豸ָ��
	m_posX   =  posX; 
	m_posY   =  posY;
	m_posZ   =	posZ;
	m_particalCount = particalCount;           //��������
	if(particalCount < 0)		//����������
		m_particalCount = 0;

	m_particalList = NULL;
	m_type  = PS_WATERFALL;
	m_size  = PARTICAL_SIZE;
	m_velocity = PARTICAL_VELOCITY;
}
CWaterfall::~CWaterfall()
{
	ShutDown();
}

void CWaterfall::ShutDown()
{
	if(m_particalList != NULL)
		delete[] m_particalList;
	if(m_particalBuffer != NULL)
	{
		m_particalBuffer->Release();
		m_particalBuffer = NULL;
	}
}

bool CWaterfall::Check()
{
	if(m_D3DDevice == NULL)
		return false;
	if(m_particalList == NULL)
		return false;
	if(m_particalBuffer == NULL)
		return false;
	return true;
}

bool CWaterfall::CreatePartical()
{
	D3DXVECTOR3 temp_vector;
	D3DXVECTOR3 normal_vector;
	for(int i = 0; i < m_particalCount; i++)
	{
		//����360�����ȡһ������������λ���Ժ���Ի���뾶����Ϊ���Ӿ�������ĵ�λ��
		//����λ�� = �������� + λ��
		temp_vector = D3DXVECTOR3(RandFloat(), RandFloat(), RandFloat()); 
		D3DXVec3Normalize(&normal_vector, &temp_vector);
		m_particalList[i].m_size = m_size;
		m_particalList[i].m_posX = m_posX + m_radius * normal_vector.x;
		m_particalList[i].m_posY = m_posY + m_radius * normal_vector.y;
		m_particalList[i].m_posZ = m_posZ + m_radius * normal_vector.z;

		m_particalList[i].m_velocityX = 0;
		m_particalList[i].m_velocityY = m_velocity + RandFloat() * m_velocity;	  //��ʱ��ֻ��Y�ٶ�
		m_particalList[i].m_velocityZ = 0;
		m_particalList[i].m_lifetime = m_particalList[i].m_currentime = RandFloat() * m_pow; 
		if(m_isUniqueColor)
			m_particalList[i].m_color = m_color;
		else
			m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		m_particalList[i].m_deltaX = m_wave * normal_vector.x;
		m_particalList[i].m_deltaY = m_wave * normal_vector.y;
		m_particalList[i].m_deltaY = abs(m_particalList[i].m_deltaY);  //Y����ֻ�����ϲ���
		m_particalList[i].m_deltaZ = m_wave * normal_vector.z;;
	}
	return true;
}

bool CWaterfall::Initial()
{
	m_particalList = new CFirePartical[m_particalCount];

	if(FAILED(m_D3DDevice->CreateVertexBuffer(m_particalCount * sizeof(CFirePartical),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, D3DFVF_PARTICAL, D3DPOOL_DEFAULT,
		&m_particalBuffer, NULL))) return false;

	void *ptr2;

	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CFirePartical),
		(void**)&ptr2, 0))) return false;

	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CFirePartical));

	m_particalBuffer->Unlock();
	return CreatePartical();
}

void CWaterfall::Render()
{
	if(!m_render)
		return;
	if(!Check())
		return;
	void *ptr2;
	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CFirePartical),
		(void**)&ptr2, 0))) return ;
	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CFirePartical));
	m_particalBuffer->Unlock();

	m_D3DDevice->SetTexture(0, m_texture);

	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_size));
	//g_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX,FtoDW(10.0f ));
	m_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.5f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

	//���þ��󣬷�ֹ��֮ǰ��Ⱦ�����Ӱ��
	//D3DXMATRIX translation;
	//D3DXMatrixTranslation(&translation, 0, 0, 0);
	//m_D3DDevice->SetTransform(D3DTS_WORLD, &translation);
	m_D3DDevice->SetStreamSource(0, m_particalBuffer,
		0, sizeof(CFirePartical));
	m_D3DDevice->SetFVF(m_FVF);
	m_D3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_particalCount);

	//��Ⱦ��Ļ�ԭ����
	m_D3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CWaterfall::UpdatePartical()
{
	if(!Check())
		return;	  
	D3DXVECTOR3 temp_vector;
	D3DXVECTOR3 normal_vector;
	for(int i = 0; i < m_particalCount; i++)
	{
		temp_vector = D3DXVECTOR3(RandFloat(), RandFloat(), RandFloat()); 
		D3DXVec3Normalize(&normal_vector, &temp_vector);
		m_particalList[i].m_deltaX = m_wave * normal_vector.x;
		m_particalList[i].m_deltaY = m_wave * normal_vector.y;
		//m_particalList[i].m_deltaY = abs(m_particalList[i].m_deltaY);  //Y����ֻ�����ϲ���
		m_particalList[i].m_deltaZ = m_wave * normal_vector.z;;

		//��ȡ�����Ӳ�������ٶ�
		m_particalList[i].m_velocityX += m_particalList[i].m_deltaX;
		m_particalList[i].m_velocityY += m_particalList[i].m_deltaY;
		m_particalList[i].m_velocityZ += m_particalList[i].m_deltaZ;
		//�ٸ����ٶȼ���λ��
		m_particalList[i].m_posX += m_particalList[i].m_velocityX;
		m_particalList[i].m_posY += m_particalList[i].m_velocityY;
		m_particalList[i].m_posZ += m_particalList[i].m_velocityZ;
		m_particalList[i].m_currentime--;
		if(m_particalList[i].m_currentime <= 0)
		{//������������
			temp_vector = D3DXVECTOR3(RandFloat(), RandFloat(), RandFloat()); 
			D3DXVec3Normalize(&normal_vector, &temp_vector);
			m_particalList[i].m_size = m_size;
			m_particalList[i].m_posX = m_posX + m_radius * normal_vector.x;
			m_particalList[i].m_posY = m_posY + m_radius * normal_vector.y;
			m_particalList[i].m_posZ = m_posZ + m_radius * normal_vector.z;

			m_particalList[i].m_velocityX = 0;
			m_particalList[i].m_velocityY = m_velocity + RandFloat() * m_velocity;	  //��ʱ��ֻ��Y�ٶ�
			m_particalList[i].m_velocityZ = 0;
			m_particalList[i].m_lifetime = m_particalList[i].m_currentime = RandFloat() * m_pow; 
			if(m_isUniqueColor)
				m_particalList[i].m_color = m_color;
			else
				m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		}
		//�������ӵ���ɫ�仯
		if(m_isUniqueColor)
		{//��ɫ����,������ɫ��˥�����
			float stage = m_particalList[i].m_currentime/m_particalList[i].m_lifetime;
			short color_r = 255 * stage;
			if(color_r < m_r)
				color_r = m_r;
			short color_g = 255 * stage;
			if(color_g < m_g)
				color_g = m_g;
			short color_b = 255 * stage;
			if(color_b < m_b)
				color_b = m_b;
			m_particalList[i].m_color = D3DCOLOR_XRGB(color_r, color_g, color_b);
		}
	}
}

void CWaterfall::SetTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_texture = texture;
}
void CWaterfall::SetParticalVelocity(float velocity)
{
	m_velocity = velocity;
}

void CWaterfall::SetParticalSize(float size)
{
	m_size = size;
}
void CWaterfall::SetColor(D3DXCOLOR color)
{
	m_color = color;
	if(!Check())
		return;
	if(m_isUniqueColor)
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
			m_particalList[i].m_color = m_color;
		}
	}
	else
	{
		for(int i = 0 ; i < m_particalCount; i++)
		{
			m_particalList[i].m_color = D3DCOLOR_XRGB(rand()%255, rand()%255,rand()%255);
		}
	}
}
void CWaterfall::SetFireColor(short r, short g, short b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}
void CWaterfall::SetWaterfall(D3DXVECTOR3 direction, float width, float length, float height, float gravity)
{
	m_direction = direction;
	m_width = width; 
	m_length = length;
	m_height = height;
	m_gravity = gravity;
}	 */