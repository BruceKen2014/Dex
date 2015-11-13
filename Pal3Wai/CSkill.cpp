

#include "CSkill.h"

CSkill::CSkill(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_enmy = NULL;
	m_id = 0;
	m_type = SKILL_FLAG;
	m_isComplete = false;
	m_num = 0;
}

void CSkill::SetEnmy(CEnemy* enmy)
{
	m_enmy = enmy;
}

CEnemy* CSkill::GetEnmy()
{
	return m_enmy;
}

int CSkill::GetNum()
{
	return rand()%(int(m_num*0.4))+m_num*0.8;
}

CSkillAddBlood::CSkillAddBlood(LPDIRECT3DDEVICE9 device):CSkill(device)
{
	m_particalList = NULL;
	m_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	m_texture = NULL;
	m_particalBuffer = NULL;
	m_color = DexColor(1.0f, 1.0f, 1.0f);
	m_size = 5;
	m_velocity = 0.5;
	m_particalCount = 100;
	m_width = 50;
	m_height = 50;
	m_depth = 50;
	m_time = 200;
	m_currTime = 0;
}

CSkillAddBlood::~CSkillAddBlood()
{
	ShutDown();
}

void CSkillAddBlood::ShutDown()
{
	_SafeDeleteArr(m_particalList);
	_SafeRelease(m_particalBuffer);
	_SafeRelease(m_texture);
}
void CSkillAddBlood::Update()
{
	if(m_currTime >= m_time)
		return;
	if(++m_currTime >= m_time)
	{
		CSound::getSoundSingleton().Play(m_sndFile, PAL_SOUND_INDEX1);
		m_isComplete = true;
	}
	for(int i = 0; i < m_particalCount; i++)
	{
		m_particalList[i].m_posX += m_particalList[i].m_velocityX;
		m_particalList[i].m_posY += m_particalList[i].m_velocityY;
		m_particalList[i].m_posZ += m_particalList[i].m_velocityZ;
	}
}

void CSkillAddBlood::Render()
{
	//if(m_currTime >= m_time)
	//	return;
	if(m_device == NULL)
		return;
	void *ptr2;
	if(FAILED(m_particalBuffer->Lock(0, m_particalCount * sizeof(CPartical),
		(void**)&ptr2, 0))) return ;
	memcpy(ptr2, m_particalList, m_particalCount * sizeof(CPartical));
	m_particalBuffer->Unlock();

	m_device->SetTexture(0, m_texture);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	m_device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
	m_device->SetRenderState(D3DRS_POINTSIZE, FtoDW(m_size));
	//g_D3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX,FtoDW(10.0f ));
	m_device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.5f));
	m_device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	m_device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	m_device->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));
	if(g_OpenFog)
		m_device->SetRenderState(D3DRS_FOGENABLE, false);

	//设置矩阵，防止受之前渲染物体的影响
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, 0, 0, 0);
	m_device->SetTransform(D3DTS_WORLD, &translation);
	m_device->SetStreamSource(0, m_particalBuffer,
		0, sizeof(CPartical));
	m_device->SetFVF(D3DFVF_PARTICAL);
	m_device->DrawPrimitive(D3DPT_POINTLIST, 0, m_particalCount);

	//渲染完改回原设置
	if(g_OpenFog)
		m_device->SetRenderState(D3DRS_FOGENABLE, g_OpenFog);
	m_device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_device->SetRenderState(D3DRS_ZENABLE, TRUE);
}


bool CSkillAddBlood::LoadTexture(char* filename)
{
	if(m_device == NULL)
		return false;
	_SafeRelease(m_texture);
	if(FAILED(D3DXCreateTextureFromFile(m_device, filename, &m_texture)))
	{
		_Message("加载纹理%s失败!", filename);
		return false;
	}
	return true;
}

bool CSkillAddBlood::CreatePartical()
{
	for(int i = 0; i < m_particalCount; i++)
	{
		m_particalList[i].m_size = m_size;

		m_particalList[i].m_posX = rand()%(int)m_width +  m_posX - m_width/2;
		m_particalList[i].m_posY = rand()%(int)m_height +  m_posY;
		m_particalList[i].m_posZ = rand()%(int)m_depth +  m_posZ - m_depth/2;
		m_particalList[i].m_color = m_color;

		float arf = rand()%180;
		float thta = rand()%180;
		//m_particalList[i].m_velocityX = m_velocity * cos(arf) * cos(thta);
		m_particalList[i].m_velocityX = 0;
		m_particalList[i].m_velocityY = abs(m_velocity * cos(arf) * sin(thta));
		m_particalList[i].m_velocityZ = 0;
		//m_particalList[i].m_velocityZ = m_velocity * sin(arf);

		//if(m_launchDistance == 0)
		//{//当时就爆炸
		//	float arf = rand()%180;
		//	float thta = rand()%180;
		//	m_particalList[i].m_velocityX = m_velocity * cos(arf) * cos(thta);
		//	m_particalList[i].m_velocityY = m_velocity * cos(arf) * sin(thta);
		//	m_particalList[i].m_velocityZ = m_velocity * sin(arf);
		//}
		//else
		//{//先发射，后爆炸  
		//	m_particalList[i].m_velocityX = m_velocity * m_launchDirection.x;
		//	m_particalList[i].m_velocityY = m_velocity * m_launchDirection.y;
		//	m_particalList[i].m_velocityZ = m_velocity * m_launchDirection.z;
		//}
		m_particalList[i].m_deltaX = m_particalList[i].m_deltaY = m_particalList[i].m_deltaZ = 0;
	} 
	//SetColor();
	return true;
}
void CSkillAddBlood::LoadIni(char* filename, char*filed)
{
	m_size = CIni::GetFloat(filename, filed, "size");
    int r  = CIni::GetInt(filename, filed, "colorR");
	int g  = CIni::GetInt(filename, filed, "colorG");
	int b  = CIni::GetInt(filename, filed, "colorB");
	m_type = CIni::GetInt(filename, filed, "type");
	m_num = CIni::GetInt(filename, filed, "num");
	m_width = CIni::GetFloat(filename, filed, "width");
	m_height = CIni::GetFloat(filename, filed, "height");
	m_depth = CIni::GetFloat(filename, filed, "depth");
	m_velocity = CIni::GetFloat(filename, filed, "vel"); 

	m_color = DexColor(r,g,b);			
	m_time = CIni::GetInt(filename, filed, "time");
	m_particalCount = CIni::GetInt(filename, filed, "count");	
	CIni::GetString(filename, filed, "tex", m_file);
	CIni::GetString(filename, filed, "snd", m_sndFile);
}
bool CSkillAddBlood::Initial()
{
	if(m_device == NULL)
		 return false;
	if(!LoadTexture(m_file))
		 return false;
	m_particalList = new CPartical[m_particalCount];
	if(FAILED(m_device->CreateVertexBuffer(m_particalCount * sizeof(CPartical),
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

void CSkillAddBlood::RePlay()
{
	m_currTime = 0;
	m_isComplete = false;
	CreatePartical();
}