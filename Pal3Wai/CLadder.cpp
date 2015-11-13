


#include "CLadder.h"

CLadder::CLadder(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 pos, float width, float length, float height, int dir, int id):CSoundObject()
{
	m_device = device;
	m_pos = pos;
	m_width = width;
	m_length = length;
	m_height = height;
	m_dir = dir;
	m_id   = id;
	CreateVB();
}

bool CLadder::CreateVB()
{
	if(FAILED(m_device->CreateVertexBuffer(LADDER_COUNT * sizeof(stVertex1),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_vb, NULL))) 
		return false;
	m_point[0].m_pos = m_pos;
	m_point[0].m_u = 0;
	m_point[0].m_v = L_TITLE_Y;

	m_point[1].m_pos = m_pos;
	m_point[1].m_u = L_TITLE_X;
	m_point[1].m_v = L_TITLE_Y;

	m_point[2].m_pos = m_pos;
	m_point[2].m_u = 0;
	m_point[2].m_v = 0;

	m_point[3].m_pos = m_pos;
	m_point[3].m_u = L_TITLE_X;
	m_point[3].m_v = 0;
	switch(m_dir)
	{
	case CS_LEFT:
		{
			m_point[1].m_pos.z += m_length;		  //   \										
			m_point[2].m_pos.x -= m_width;		  //	\ 
			m_point[2].m_pos.y += m_height;		  //   	 \ 										  
			m_point[3].m_pos.x -= m_width;		  	  
			m_point[3].m_pos.y += m_height;		  
			m_point[3].m_pos.z += m_length;
			break;
		}
	case CS_RIGHT:
		{
			m_point[1].m_pos.z -= m_length;		  //   	/									
			m_point[2].m_pos.x += m_width;		  //   /	 
			m_point[2].m_pos.y += m_height;		  //  / 	  										  
			m_point[3].m_pos.x += m_width;		  	  
			m_point[3].m_pos.y += m_height;		  
			m_point[3].m_pos.z -= m_length;
			break;
		}
	case CS_FRONT:
		{
			m_point[1].m_pos.x += m_width;		  //  个  个 										
			m_point[2].m_pos.y += m_height;		  //  |    |	 
			m_point[2].m_pos.z += m_length;		  //  |    |	  										  
			m_point[3].m_pos.x += m_width;		  // 0|	   |
			m_point[3].m_pos.y += m_height;		  
			m_point[3].m_pos.z += m_length;
			break;
		}
	case CS_BACK:
		{
			m_point[1].m_pos.x -= m_width;		  //  |    |0 										
			m_point[2].m_pos.y += m_height;		  //  |    |	 
			m_point[2].m_pos.z -= m_length;		  //  |    |	  										  
			m_point[3].m_pos.x -= m_width;		  //  V	   V
			m_point[3].m_pos.y += m_height;		  
			m_point[3].m_pos.z -= m_length;
			break;
		}
    default:
		{
			m_point[1].m_pos.z += m_length;		  //   \										
			m_point[2].m_pos.x -= m_width;		  //	\ 
			m_point[2].m_pos.y += m_height;		  //   	 \ 										  
			m_point[3].m_pos.x -= m_width;		  	  
			m_point[3].m_pos.y += m_height;		  
			m_point[3].m_pos.z += m_length;
		    break;
		}
	}
	stVertex1 temp = m_point[1];
	m_point[1] = m_point[2];
	m_point[2] = temp;
	void *ptr;
	if( FAILED(m_vb->Lock( 0, LADDER_COUNT * sizeof(stVertex1), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, m_point, LADDER_COUNT * sizeof(stVertex1));
	if( FAILED(m_vb->Unlock() ) ) 
		return false; 
	return true;
}

void CLadder::ShutDown()
{
	_SafeRelease(m_vb);
	_SafeRelease(m_texture);
}

void CLadder::Update()
{
}

void CLadder::Render()
{
	if(!m_render)
		return;
	if(m_device == NULL)
		return;
	if(m_texture != NULL)
	    m_device->SetTexture(0, m_texture);
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	m_device->SetStreamSource( 0, m_vb, 0, sizeof(stVertex1));	
	m_device->SetFVF(FVF_XYZ_DIFF_TEX1);
	m_device->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
}

bool CLadder::IsOn(D3DXVECTOR3 pos)
{
	switch(m_dir)
	{
	case CS_LEFT:
		{	
			if(  pos.x <= m_pos.x && pos.x >= (m_pos.x - m_width)
				&& pos.z >= m_pos.z && pos.z <= (m_pos.z + m_length))
			{
				float percent = abs(pos.x - m_pos.x)/m_width; 
				if(abs(m_pos.y + m_height*percent - pos.y) < 10) //高度与该处的高度相差10，认为处在梯子上
					return true;
			}
			return false;
		}
	case CS_RIGHT:
		{	
			if(  pos.x >= m_pos.x && pos.x <= (m_pos.x + m_width)
				&& pos.z <= m_pos.z && pos.z >= (m_pos.z - m_length))
			{
				float percent = abs(pos.x - m_pos.x)/m_width; 
				if(abs(m_pos.y + m_height*percent - pos.y) < 10) //高度与该处的高度相差10，认为处在梯子上
					return true;
			}
			return false;
		}
	case CS_FRONT:
		{	
			if(  pos.x >= m_pos.x && pos.x <= (m_pos.x + m_width)
				&& pos.z >= m_pos.z && pos.z <= (m_pos.z + m_length))
			{
				float percent = abs(pos.z - m_pos.z)/m_length;;
				if(abs(m_pos.y + m_height*percent - pos.y) < 10) //高度与该处的高度相差10，认为处在梯子上
					return true;
			}
			return false;
		}
	case CS_BACK:
		{	
			if(  pos.x <= m_pos.x && pos.x >= (m_pos.x - m_width)
			&& pos.z <= m_pos.z && pos.z >= (m_pos.z - m_length))
			{
				float percent = abs(pos.z - m_pos.z)/m_length;;
				if(abs(m_pos.y + m_height*percent - pos.y) < 10) //高度与该处的高度相差10，认为处在梯子上
					return true;
			}
			return false;
		}
	default:
		break;
	}
	return false;
}


float CLadder::GetHeight(D3DXVECTOR3 pos)
{
	float percent = 0;
	switch(m_dir)
	{
	case CS_LEFT:
	case CS_RIGHT:
		{
			percent = abs(pos.x - m_pos.x)/m_width; 
			break;
		}
	case CS_BACK:
	case CS_FRONT:
		{
			percent = abs(pos.z - m_pos.z)/m_length;
			break;
		}
	default:
		break;
	}	
	return m_pos.y + m_height*percent;
}