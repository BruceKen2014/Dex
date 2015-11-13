

#include "CBillboard.h"

CBillboard::CBillboard()
{
	 m_device = NULL;
	 m_vertexBuffer = NULL;
	 m_texture = NULL;
	 m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	 m_xScale = 1.0f;
	 m_yScale = 1.0f;
	 m_yRotate = 0.0f;
	 m_width  = 2.0f;   //默认长宽均为2.0f
	 m_height = 2.0f;
	 m_alpha  = 255;;
	 m_render = true;
}

CBillboard::CBillboard(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_vertexBuffer = NULL;
	m_texture = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_xScale = 1.0f;
	m_yScale = 1.0f;
	m_yRotate = 0.0f;
	m_width  = 2.0f;   //默认长宽均为2.0f
	m_height = 2.0f;
	m_alpha  = 255;;
	m_render = true;
    m_device->CreateVertexBuffer(4*sizeof(stBVertex), 0, BILLBOARD_FVF, D3DPOOL_MANAGED, &m_vertexBuffer,0);
}

CBillboard::~CBillboard()
{
	ShutDown();
}
void CBillboard::SetMatrix()
{
	//先平移到原点，然后旋转，缩放，再平移到原来位置
	D3DXMATRIX rotate;
	D3DXMATRIX trans;
	D3DXMATRIX scale;
	D3DXMatrixIdentity(&rotate);
	D3DXMatrixIdentity(&trans);
	D3DXMatrixIdentity(&scale);
	//在原点的矩阵旋转
	D3DXMATRIX rotateY;
	D3DXMatrixIdentity(&rotateY);
	D3DXMatrixRotationY(&rotateY, m_yRotate);
	D3DXMatrixMultiply(&rotate, &rotate, &rotateY);	
	//平移到原点
	D3DXMatrixTranslation(&trans, 0, 0, 0);	 

	//相乘
	D3DXMatrixScaling(&scale, m_xScale, m_yScale, 1.0f);
	//乘上缩放矩阵  
	D3DXMatrixMultiply(&m_matrix, &scale, &trans);

	//旋转
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &rotate);	 



	//最后平移到物体位置
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_Matrix, &rotate, &trans);	
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &trans);	

}
void CBillboard::CreateVertex()
{
	//_SafeRelease(m_vertexBuffer);
	void* ptr;
	stBVertex temp[4]={
		{ D3DXVECTOR3(-1.0f, 2.0f, 0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),0.0f, 0.0f },
		{ D3DXVECTOR3(1.0f, 2.0f, 0.0f),  D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 1.0f, 0.0f },
		{ D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 0.0f, 1.0f },
		{ D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 1.0f, 1.0f }
	}; 
	m_vertexs[0] = temp[0]; 
	m_vertexs[1] = temp[1]; 
	m_vertexs[2] = temp[2]; 
	m_vertexs[3] = temp[3]; 
	m_vertexBuffer->Lock(0, sizeof(m_vertexs),(void**)&ptr,0);
	memcpy(ptr, m_vertexs, sizeof(m_vertexs));
	m_vertexBuffer->Unlock();
}

void CBillboard::UpdateVertex()
{
}

void CBillboard::ShutDown()
{
	_SafeRelease(m_vertexBuffer);
	_SafeRelease(m_texture);
}

void CBillboard::Update(D3DXVECTOR3 camera_pos)
{
	D3DXVECTOR3 v1 = D3DXVECTOR3(0,0,-1);
	D3DXVECTOR3 v2 = camera_pos - m_pos;
	v2.y = 0;
	D3DXVec3Normalize(&v2, &v2);	

	float dot =D3DXVec3Dot(&v1, &v2);
	float cos_radius = dot;  //因为两个皆是单位向量，故除模省去
	float angle_minus =	acos(cos_radius);
	if(DecideISClock(v1, v2))
		m_yRotate = angle_minus;   
	else
		m_yRotate = -angle_minus;
	SetMatrix();
}

void CBillboard::Render()
{
	if(!m_render)
		return;
	if(m_device == NULL)
		return;
	

	//半透明纹理混合
	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);	 
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
	m_device->SetStreamSource(0,m_vertexBuffer,0,sizeof(stBVertex));
	m_device->SetFVF(BILLBOARD_FVF );
	m_device->SetTexture(0,m_texture);
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, 0, 0, 0);
	m_device->SetTransform( D3DTS_WORLD, &trans);
	m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);	 
	//m_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);   
	//m_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
}

bool CBillboard::LoadTexture(char *filename)
{
	_SafeRelease(m_texture);
	D3DXIMAGE_INFO info;
	if(D3DXCreateTextureFromFileEx(m_device, filename, D3DX_DEFAULT_NONPOW2/*使用此参数才不会放大，使用0或者D3DX_DEFAULT图片会被放大一倍*/
		, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,0, D3DFMT_UNKNOWN, 
		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE,
		NULL, &info, NULL, &m_texture) != D3D_OK)
	{
		_Message("加载纹理%s失败!",filename);
		return false;
	}

	return true;
}
//alpha
void  CBillboard::SetAlpha(float alpha)
{
	m_alpha = alpha;
}
float CBillboard::GetAlpha()
{
	return m_alpha;
}

//render

void CBillboard::SetRender(bool _render)
{
	m_render = _render;
}

bool CBillboard::GetRender()
{
	return m_render;
}

//scale
void  CBillboard::SetXScale(float scale)
{
	 m_xScale = scale;
	 m_width  =	scale * 2;  //放大倍数乘以默认宽度
	 SetMatrix();
}
void  CBillboard::SetYScale(float scale)
{
	 m_yScale = scale;
	 m_height = scale * 2;
	 SetMatrix();
}
void  CBillboard::SetScale(float xScale, float yScale)
{
	m_xScale = xScale;
	m_width  = xScale * 2;

	m_yScale = yScale;
	m_height = yScale * 2;
	SetMatrix();
}
void  CBillboard::SetScale(D3DXVECTOR2 scale)
{
	m_xScale = scale.x;
	m_width  = scale.x * 2;

	m_yScale = scale.y;
	m_height = scale.y * 2;
	SetMatrix();
}
void  CBillboard::SetScale(float scale)
{
	m_xScale = m_yScale = scale;
	m_width  = scale * 2;
	m_height = scale * 2;
	SetMatrix();
}
float CBillboard::GetXScale()
{
	return m_xScale;
}
float CBillboard::GetYScale()
{
	return m_yScale;
}

//宽高

void CBillboard::SetWidth(float width)
{
	m_width = width;
	m_xScale = m_width/2;
}

void CBillboard::SetHeight(float height)
{
	m_height = height;
	m_yScale = m_height/2;
}

void CBillboard::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;

	m_xScale = m_width/2;
	m_yScale = m_height/2;
}

void CBillboard::SetSize(D3DXVECTOR2 size)
{
	m_width = size.x;
	m_height = size.y;

	m_xScale = m_width/2;
	m_yScale = m_height/2;
}

void CBillboard::SetSize(float size)
{
	m_width = size;
	m_height = size;

	m_xScale = m_width/2;
	m_yScale = m_height/2;
}


float CBillboard::GetWidth()
{
	return m_width;
}

float CBillboard::GetHeight()
{
	return m_height;
}
//pos
void  CBillboard::SetPos(float xPos, float yPos, float zPos)
{
	m_pos.x = xPos;
	m_pos.y = yPos;
	m_pos.z = zPos;
	SetMatrix();
}
void  CBillboard::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
	SetMatrix();
}
D3DXVECTOR3 CBillboard::GetPos()
{
	return m_pos;
}



CBillBoardAni::CBillBoardAni():CBillboard()
{
	m_tex = NULL;
	m_type = MUL;
	m_xFrame = m_frames = 1;
	m_currFrame = 0;
	m_aniSpeed = m_currCount = 0;
	m_changeByCamera = false;
	m_dir = D3DXVECTOR3(1,0,0);
}
CBillBoardAni::CBillBoardAni(LPDIRECT3DDEVICE9 device):CBillboard(device)
{
	m_tex = NULL;
	m_type = MUL;
	m_xFrame = m_frames = 1;
	m_currFrame = 0;
	m_aniSpeed = m_currCount = 0;
	m_changeByCamera = false;
	m_dir = D3DXVECTOR3(1,0,0);

	SetDir(m_dir);
}
CBillBoardAni::~CBillBoardAni()
{
	ShutDown();
}

void CBillBoardAni::ShutDown()
{
	CBillboard::ShutDown();
	_REDUCEREF(m_tex);
}

void CBillBoardAni::CreateVertex()
{
	//_SafeRelease(m_vertexBuffer);
	void* ptr;
	 stBVertex temp[4]={
		{ D3DXVECTOR3(-1.0f, 2.0f, 0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),0.0f, 0.0f },
		{ D3DXVECTOR3(1.0f, 2.0f, 0.0f),  D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 1.0f, 0.0f },
		{ D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 0.0f, 1.0f },
		{ D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 1.0f, 1.0f }
	}; 
	 m_vertexs[0] = temp[0]; 
	 m_vertexs[1] = temp[1]; 
	 m_vertexs[2] = temp[2]; 
	 m_vertexs[3] = temp[3]; 
	 //m_vertexs = temp; 
	//m_vertexs[0] =  { D3DXVECTOR3(-1.0f, 2.0f, 0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),0.0f, 0.0f }; 
	//m_vertexs[1] =  { D3DXVECTOR3(1.0f, 2.0f, 0.0f),  D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 1.0f, 0.0f}; 
	//m_vertexs[2] =  { D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 0.0f, 1.0f }; 
	//m_vertexs[3] =  { D3DXVECTOR3(1.0f, 0.0f, 0.0f),  D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), 1.0f, 1.0f }; 
	m_vertexs[0].m_u = 0.0f;
	m_vertexs[0].m_v = 0.0f;

	m_vertexs[1].m_u = 1.0f/(float)m_xFrame;
	m_vertexs[1].m_v = 0.0f;

	int y = m_frames/m_xFrame;	 //几行
	if(m_xFrame*y < m_frames)
		y++;
	m_vertexs[2].m_u = 0.0f;
	m_vertexs[2].m_v = 1.0f/(float)y; 

	m_vertexs[3].m_u = 1.0f/(float)m_xFrame;
	m_vertexs[3].m_v = 1.0f/(float)y; 

	m_vertexBuffer->Lock(0, sizeof(m_vertexs),(void**)&ptr,0);
	memcpy(ptr, m_vertexs, sizeof(m_vertexs));
	m_vertexBuffer->Unlock();
}
void CBillBoardAni::UpdateVertex()
{
	if(m_type != MUL)
		return;
	m_currCount++;
	if(m_currCount > m_aniSpeed)
	{
		m_currCount = 0;
		if(++m_currFrame >= m_frames)
		{//回到第0帧
			m_currFrame = 0;
		}  
		float x_del = 1.0f/(float)m_xFrame;  //横向帧坐标间隔
		float y_del = 1.0f/(float)m_yRows;   //纵向帧间隔
		short row = m_currFrame/m_xFrame;  //得到当前行数
		//if(m_xFrame*row < m_currFrame)
		//	row++;
		short col =  m_currFrame - m_xFrame*row;  //得到当前列数 
		m_vertexs[0].m_u = col * x_del;
		m_vertexs[0].m_v = row * y_del;

		m_vertexs[1].m_u = (col+1) * x_del;
		m_vertexs[1].m_v = row * y_del;


		m_vertexs[2].m_u = col * x_del;
		m_vertexs[2].m_v = (row+1) * y_del; 

		m_vertexs[3].m_u = (col+1) * x_del;
		m_vertexs[3].m_v = (row+1) * y_del; 
		void* ptr;
		m_vertexBuffer->Lock(0, sizeof(m_vertexs),(void**)&ptr,0);
		memcpy(ptr, m_vertexs, sizeof(m_vertexs));
		m_vertexBuffer->Unlock();
	}
}
void CBillBoardAni::Update(D3DXVECTOR3 camera_pos)
{
	UpdateVertex();

	if(!m_changeByCamera)
	{
		//SetMatrix();
		return;
	}
	CBillboard::Update(camera_pos);
}

void CBillBoardAni::Render()
{
	if(!m_render)
		return;
	if(m_device == NULL)
		return;	 
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
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
	m_device->SetStreamSource(0,m_vertexBuffer,0,sizeof(stBVertex));
	m_device->SetFVF(BILLBOARD_FVF );
	if(m_tex != NULL)
		m_device->SetTexture(0,m_tex->GetTexPt());
	else
		m_device->SetTexture(0, NULL);
	m_device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, 0, 0, 0);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_device->SetTransform( D3DTS_WORLD, &trans);
	m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CBillBoardAni::SetTex(CDexTex* tex)
{
	m_tex = tex;
}
void CBillBoardAni::SetFrame(int frames)
{
	m_frames = frames;
}
void CBillBoardAni::SetChangeByCamera(bool b)
{
	m_changeByCamera = b;
}

void CBillBoardAni::SetFrame(int total_frame, int xframe)
{
	m_frames = total_frame;
	m_xFrame = xframe;
	m_yRows = m_frames/m_xFrame;	 
	if(m_xFrame*m_yRows < m_frames)
		m_yRows++;
}
void CBillBoardAni::SetFrameX(int xFrame)
{
	m_xFrame = xFrame;
	m_yRows = m_frames/m_xFrame;	 
	if(m_xFrame*m_yRows < m_frames)
		m_yRows++;
}

void CBillBoardAni::SetDir(D3DXVECTOR3 dir)
{
	m_dir = dir; 
	D3DXVECTOR3 v1 = D3DXVECTOR3(0,0,-1);
	D3DXVECTOR3 v2 = m_dir;
	v2.y = 0;
	D3DXVec3Normalize(&v2, &v2);	

	float dot =D3DXVec3Dot(&v1, &v2);
	float cos_radius = dot;  //因为两个皆是单位向量，故除模省去
	float angle_minus =	acos(cos_radius);
	if(DecideISClock(v1, v2))
		m_yRotate = angle_minus;   
	else
		m_yRotate = -angle_minus;
	SetMatrix();
}

void CBillBoardAni::SetDir(float x, float y, float z)
{
	SetDir(D3DXVECTOR3(x, y, z));
}

void CBillBoardAni::SetSpeed(int speed)
{
	m_aniSpeed = speed;
}