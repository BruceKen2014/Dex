


#include "CTerrain.h"
#include "CCommandScript.h"
#include "../DexBase/DexLog.h"


CQuadNode::CQuadNode()
{
	m_centerIndex = 0;
	for(int i =0; i < 4; i++)
		m_CornerIndex[i] = 0;
	m_parent = NULL;
	for(int i =0; i < 4; i++)
		m_pChildNode[i] = NULL;
}
CQuadNode::~CQuadNode()
{
}
void CQuadNode::AddChild(int index)
{
	if(m_CornerIndex[CQN_RT] - m_CornerIndex[CQN_LT] <= 1)
		//不可再分
		return;
	int center;
	int lt, rt, lb,rb;
	CQuadNode* child = new CQuadNode();
	switch(index)
	{  
	case CQN_LT:
		{
			lt =  m_CornerIndex[CQN_LT];
			rt = (m_CornerIndex[CQN_RT] + m_CornerIndex[CQN_LT])/2; 
			lb = (m_CornerIndex[CQN_LT] + m_CornerIndex[CQN_LB])/2;
			rb =  m_centerIndex;
			break;
		}
	case CQN_RT:
		{
			lt = (m_CornerIndex[CQN_RT] + m_CornerIndex[CQN_LT])/2;
			rt =  m_CornerIndex[CQN_RT]; 
			lb =  m_centerIndex;
			rb = (m_CornerIndex[CQN_RT] + m_CornerIndex[CQN_RB])/2;
			break;
		}
	case CQN_LB:
		{
			lt = (m_CornerIndex[CQN_LT] + m_CornerIndex[CQN_LB])/2;
			rt =  m_centerIndex; 
			lb =  m_CornerIndex[CQN_LB];
			rb = (m_CornerIndex[CQN_LB] + m_CornerIndex[CQN_RB])/2;
			break;
		}
	case CQN_RB:
		{
			lt =  m_centerIndex;
			rt = (m_CornerIndex[CQN_RT] + m_CornerIndex[CQN_RB])/2; 
			lb = (m_CornerIndex[CQN_LB] + m_CornerIndex[CQN_RB])/2;
			rb =  m_CornerIndex[CQN_RB];
			break;
		}
	default:
		{
			_SafeDelete(child);
			return;
		}
	}
	center = (lt + rt + lb + rb)/4;
	child->m_parent = this;
	child->m_centerIndex = center;
	child->m_CornerIndex[CQN_LT] = lt;
	child->m_CornerIndex[CQN_RT] = rt;
	child->m_CornerIndex[CQN_LB] = lb;
	child->m_CornerIndex[CQN_RB] = rb;
	m_pChildNode[index] = child; 
}

//================================================================================
CTerrain::CTerrain(LPDIRECT3DDEVICE9 device)
{
	m_device = device;
	m_vb = NULL;
	m_ib = NULL;
	m_lineB = NULL;
	m_render = true;
	m_pos = D3DXVECTOR3(0,0,0);
	m_row = m_col = 4;
	m_cellWidth = m_cellHeight = 30;
	m_scale = 1.0f;			 
	m_renderStyle = TERRIAN_TRI;

	//m_rootNode = NULL;
	m_texture = NULL;
}

CTerrain::~CTerrain()
{
	ShutDown();
}

void CTerrain::ShutDown()
{
	_SafeRelease(m_vb);
	_SafeRelease(m_ib);
	_SafeRelease(m_lineB);
	_SafeRelease(m_texture);
	for(int i =0 ; i <m_row; i++ )
		_SafeDeleteArr(m_height[i]);
	_SafeDeleteArr(m_height);
}

bool CTerrain::LoadIni(char* filename, char* field)
{
	char file[64];
	char texture[64];
	float scale;
	CIni::GetString(filename, field, "ter", file);
	CIni::GetString(filename, field, "texture", texture);
	m_cellWidth = CIni::GetInt(filename, field, "cWidth");
	m_cellHeight = CIni::GetInt(filename, field, "cHeight");
	scale = CIni::GetFloat(filename, field, "scale");
	m_pos = D3DXVECTOR3(CIni::GetFloat(filename, field, "posx"),
		CIni::GetFloat(filename, field, "posy"),
		CIni::GetFloat(filename, field, "posz"));
	if(!CreateTerrain(file, m_cellWidth, m_cellHeight, scale))
		return false;
	if(!LoadTexture(texture))
		return false;
	return true;
}

bool CTerrain::CreateTerrain(char* filename, int cellWidth, int cellHeight,float scale)
{
	m_cellWidth = cellWidth;
	m_cellHeight = cellHeight;
	m_scale = scale;


	m_row = 257;
	m_col = 257;
	m_height = new float*[m_row];
	for(int i =0 ; i <m_row; i++ )
		m_height[i] = new float[m_col];
	std::ifstream inFile(filename, std::ios_base::binary);
	if(inFile)
	{
		_Message("打开高度图失败！");
		return false;
	}
	std::vector<float> trfloat;
	
	for(int i = 0;i < m_row; i++)
		for(int j = 0; j< m_col;j++)
		{
			float t =  inFile.get()*m_scale;
			m_height[i][j] = t;
			if(t != 0)
				trfloat.push_back(t);
		}
	inFile.close();
	if(FAILED(m_device->CreateVertexBuffer(m_row * m_col * sizeof(Terrain_Vertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_vb, NULL))) 
		return false;

	int num = m_row * m_col;
	Terrain_Vertex *temp = new Terrain_Vertex[num];
	for(int i = 0; i < m_row; i++)
	{
		for(int j = 0; j< m_col; j++)
		{
			temp[i*m_col + j].m_pos.x += j * m_cellWidth;
			temp[i*m_col + j].m_pos.z += i * m_cellHeight;
			temp[i*m_col + j].m_pos.y += m_height[i][j] * m_scale; 
			temp[i*m_col + j].tu = (float)j/m_col*10; 
			temp[i*m_col + j].tv = ((float)i)/m_row*10; 
			D3DXVECTOR3 tt = temp[i*m_col + j].m_pos; 
		}

	}
	void *ptr;
	if( FAILED(m_vb->Lock( 0, num * sizeof(Terrain_Vertex), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, temp, num * sizeof(Terrain_Vertex));
	if( FAILED(m_vb->Unlock() ) ) 
		return false; 
	_SafeDeleteArr(temp);


	int *tempindex = new int[(m_row-1)*(m_col-1)*6];
	std::vector<int> vec;
	for(int i = 0; i < m_row -1;i++)
	{
		for(int j = 0; j < m_col -1; j++)
		{
			tempindex[(i*(m_col-1)+j)*6] = m_col*i + j;
			tempindex[(i*(m_col-1)+j)*6 +1] = m_col*(i+1) + j;
			tempindex[(i*(m_col-1)+j)*6 +2] = m_col*i + j+1;

			tempindex[(i*(m_col-1)+j)*6 +3] = m_col*i + j+1 ;
			tempindex[(i*(m_col-1)+j)*6 +4] = m_col*(i+1) + j;
			tempindex[(i*(m_col-1)+j)*6 +5] = m_col*(i+1) + j+1;

			vec.push_back(m_col*i + j);
			vec.push_back(m_col*(i+1) + j);
			vec.push_back(m_col*i + j+1);
			vec.push_back(m_col*i + j+1);
			vec.push_back(m_col*(i+1) + j);
			vec.push_back(m_col*(i+1) + j+1);
		}
	}
	if(FAILED(m_device->CreateIndexBuffer((m_row-1)*(m_col-1)*6 * sizeof(int),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT,
		&m_ib, NULL))) 
		return false;
	void *ptr2;
	if( FAILED(m_ib->Lock( 0, (m_row-1)*(m_col-1)*6 * sizeof(int), (void**)&ptr2, 0 ) ) )
		return false;	
	memcpy(ptr2, tempindex, (m_row-1)*(m_col-1)*6 * sizeof(int));
	if( FAILED(m_ib->Unlock() ) ) 
		return false;  
	_SafeDeleteArr(tempindex); 

	//线条索引
	int line_number = ((m_col-1)*m_row + (m_row-1)*m_col)*2;
	tempindex = new int[line_number];
	for(int i = 0; i < m_row - 1;i++)
	{
		for(int j = 0; j < m_col - 1; j++)
		{
			tempindex[i*(m_col-1)*4 + j*4] = m_col*i + j;
			tempindex[i*(m_col-1)*4 + j*4 + 1] = m_col*i + j + 1;

			tempindex[i*(m_col-1)*4 + j*4 + 2] = m_col*i + j;
			tempindex[i*(m_col-1)*4 + j*4 + 3] = m_col*(i+1) + j;
		}
	}
	int row_number = (m_col-1)*(m_row-1)*2;
	int col_number = (m_row-1)*(m_col-1)*2;
	int start = line_number - row_number - col_number; 
	//最上面一层
	for(int j = 0; j < m_col - 1; j++)
	{
		tempindex[start + j*2] = (m_row-1)*m_col + j;
		tempindex[start + j*2 + 1] = (m_row-1)*m_col + j + 1;
	}
	//最右面一层
	start = line_number - 2*(m_row-1); 
	for(int i = 0; i < m_row - 1; i++)
	{
		tempindex[start + i*2] = m_row*i + m_col - 1;
		tempindex[start + i*2 + 1] = m_row*(i+1) + m_col - 1;
	}
	if(FAILED(m_device->CreateIndexBuffer(line_number* sizeof(int),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT,
		&m_lineB, NULL))) 
		return false;
	void *ptr3;
	if( FAILED(m_lineB->Lock( 0, line_number * sizeof(int), (void**)&ptr3, 0 ) ) )
		return false;	
	memcpy(ptr3, tempindex, line_number * sizeof(int));
	if( FAILED(m_lineB->Unlock() ) ) 
		return false;  
	_SafeDeleteArr(tempindex); 


	//m_rootNode = new CQuadNode();

	
	return true;
}

bool CTerrain::CreateTerrain2(char* filename, int cellWidth, int cellHeight,float scale, bool flag)
{
	getLog()->BeginLog();
	m_cellWidth = cellWidth;
	m_cellHeight = cellHeight;
	m_scale = scale;
	
	CommandScript* pScript =  getComandScript();
	if(!pScript->OpenScript(filename))
	{
		getLog()->Log(log_allert, "加載高度數據文件失敗!\n");
		return false;
	}
	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	int temprow = 0;
	m_col = pScript->GetIntParam();//第一行表示有多少列
	m_row = pScript->GetScriptLine() - 1; //除去第一行表示列數的數據
	pScript->MoveCurrLine();//向下移動一行
	//m_height = new float*[m_row];
	//for(int i =0 ; i <m_row; i++ )
	//	m_height[i] = new float[m_col];
	int curr_row = 0;
	for( ;curr_row < m_row; ++curr_row)
	{	
		pScript->SetCurrChar() ;
		for(int i = 0 ; i < m_col; ++i)
		{
			m_height[curr_row][i] = pScript->GetIntParam() * m_scale;
		}
		pScript->MoveCurrLine();
	}
	if(FAILED(m_device->CreateVertexBuffer(m_row * m_col * sizeof(Terrain_Vertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_vb, NULL))) 
		return false;

	int num = m_row * m_col;
	Terrain_Vertex *temp = new Terrain_Vertex[num];
	for(int i = 0; i < m_row; i++)
	{
		for(int j = 0; j< m_col; j++)
		{
			temp[i*m_col + j].m_pos.x += j * m_cellWidth;
			temp[i*m_col + j].m_pos.z += i * m_cellHeight;
			temp[i*m_col + j].m_pos.y += m_height[i][j] * m_scale; 
			temp[i*m_col + j].tu = (float)j/m_col*10; 
			temp[i*m_col + j].tv = ((float)i)/m_row*10; 
			D3DXVECTOR3 tt = temp[i*m_col + j].m_pos; 
		}

	}
	void *ptr;
	if( FAILED(m_vb->Lock( 0, num * sizeof(Terrain_Vertex), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, temp, num * sizeof(Terrain_Vertex));
	if( FAILED(m_vb->Unlock() ) ) 
		return false; 
	_SafeDeleteArr(temp);


	int *tempindex = new int[(m_row-1)*(m_col-1)*6];
	std::vector<int> vec;
	for(int i = 0; i < m_row -1;i++)
	{
		for(int j = 0; j < m_col -1; j++)
		{
			tempindex[(i*(m_col-1)+j)*6] = m_col*i + j;
			tempindex[(i*(m_col-1)+j)*6 +1] = m_col*(i+1) + j;
			tempindex[(i*(m_col-1)+j)*6 +2] = m_col*i + j+1;

			tempindex[(i*(m_col-1)+j)*6 +3] = m_col*i + j+1 ;
			tempindex[(i*(m_col-1)+j)*6 +4] = m_col*(i+1) + j;
			tempindex[(i*(m_col-1)+j)*6 +5] = m_col*(i+1) + j+1;

			vec.push_back(m_col*i + j);
			vec.push_back(m_col*(i+1) + j);
			vec.push_back(m_col*i + j+1);
			vec.push_back(m_col*i + j+1);
			vec.push_back(m_col*(i+1) + j);
			vec.push_back(m_col*(i+1) + j+1);
		}
	}
	if(FAILED(m_device->CreateIndexBuffer((m_row-1)*(m_col-1)*6 * sizeof(int),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT,
		&m_ib, NULL))) 
		return false;
	void *ptr2;
	if( FAILED(m_ib->Lock( 0, (m_row-1)*(m_col-1)*6 * sizeof(int), (void**)&ptr2, 0 ) ) )
		return false;	
	memcpy(ptr2, tempindex, (m_row-1)*(m_col-1)*6 * sizeof(int));
	if( FAILED(m_ib->Unlock() ) ) 
		return false;  
	_SafeDeleteArr(tempindex); 

	//线条索引
	int line_number = ((m_col-1)*m_row + (m_row-1)*m_col)*2;
	tempindex = new int[line_number];
	for(int i = 0; i < m_row - 1;i++)
	{
		for(int j = 0; j < m_col - 1; j++)
		{
			tempindex[i*(m_col-1)*4 + j*4] = m_col*i + j;
			tempindex[i*(m_col-1)*4 + j*4 + 1] = m_col*i + j + 1;

			tempindex[i*(m_col-1)*4 + j*4 + 2] = m_col*i + j;
			tempindex[i*(m_col-1)*4 + j*4 + 3] = m_col*(i+1) + j;
		}
	}
	int row_number = (m_col-1)*(m_row-1)*2;
	int col_number = (m_row-1)*(m_col-1)*2;
	int start = line_number - row_number - col_number; 
	//最上面一层
	for(int j = 0; j < m_col - 1; j++)
	{
		tempindex[start + j*2] = (m_row-1)*m_col + j;
		tempindex[start + j*2 + 1] = (m_row-1)*m_col + j + 1;
	}
	//最右面一层
	start = line_number - 2*(m_row-1); 
	for(int i = 0; i < m_row - 1; i++)
	{
		tempindex[start + i*2] = m_row*i + m_col - 1;
		tempindex[start + i*2 + 1] = m_row*(i+1) + m_col - 1;
	}
	if(FAILED(m_device->CreateIndexBuffer(line_number* sizeof(int),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT,
		&m_lineB, NULL))) 
		return false;
	void *ptr3;
	if( FAILED(m_lineB->Lock( 0, line_number * sizeof(int), (void**)&ptr3, 0 ) ) )
		return false;	
	memcpy(ptr3, tempindex, line_number * sizeof(int));
	if( FAILED(m_lineB->Unlock() ) ) 
		return false;  
	_SafeDeleteArr(tempindex); 


	//m_rootNode = new CQuadNode();

	getLog()->EndLog();
	return true;
}

bool CTerrain::Create()
{
	if(FAILED(m_device->CreateVertexBuffer(m_row * m_col * sizeof(Terrain_Vertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_vb, NULL))) 
		return false;

	int num = m_row * m_col;
	float height[16] = { 0, 2, 4, 3,2 ,4 ,6, 2, 4, 3, 1, 9, 1, 3, 4, 1};
	for(int i = 0; i < 16;i++)
		height[i] *= 10;
	Terrain_Vertex *temp = new Terrain_Vertex[num];
	for(int i = 0; i < m_row; i++)
	{
		for(int j=0; j< m_col; j++)
		{
			temp[i*m_col + j].m_pos.x += j * m_cellWidth;
			temp[i*m_col + j].m_pos.z += i * m_cellHeight;
			temp[i*m_col + j].m_pos.y += height[m_col*i + j]; 
			temp[i*m_col + j].tu = (float)j/(float)m_col; 
			temp[i*m_col + j].tv = (float)i/(float)m_row; 
			D3DXVECTOR3 tt = temp[i*m_col + j].m_pos; 
		}

	}
	void *ptr;
	if( FAILED(m_vb->Lock( 0, num * sizeof(Terrain_Vertex), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, temp, num * sizeof(Terrain_Vertex));
	if( FAILED(m_vb->Unlock() ) ) 
		return false; 
	delete[] (temp);	
	int *tempindex = new int[(m_row-1)*(m_col-1)*6];
	for(int i = 0; i < m_row -1;i++)
	{
		for(int j = 0; j < m_col -1; j++)
		{
			tempindex[(i*(m_col-1)+j)*6] = m_col*i + j;
			tempindex[(i*(m_col-1)+j)*6 +1] = m_col*(i+1) + j;
			tempindex[(i*(m_col-1)+j)*6 +2] = m_col*i + j+1;

			tempindex[(i*(m_col-1)+j)*6 +3] = m_col*i + j+1 ;
			tempindex[(i*(m_col-1)+j)*6 +4] = m_col*(i+1) + j;
			tempindex[(i*(m_col-1)+j)*6 +5] = m_col*(i+1) + j+1;
		}
	}
	if(FAILED(m_device->CreateIndexBuffer((m_row-1)*(m_col-1)*6 * sizeof(int),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT,
		&m_ib, NULL))) 
		return false;
	void *ptr2;
	if( FAILED(m_ib->Lock( 0, (m_row-1)*(m_col-1)*6 * sizeof(int), (void**)&ptr2, 0 ) ) )
		return false;	
	memcpy(ptr2, tempindex, (m_row-1)*(m_col-1)*6 * sizeof(int));
	if( FAILED(m_ib->Unlock() ) ) 
		return false;  
	delete[] (tempindex); 
	return true;
}

bool CTerrain::LoadTexture(char* filename)
{
	_SafeRelease(m_texture);
	D3DXCreateTextureFromFile(m_device, filename, &m_texture);
	return true;
}

void CTerrain::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

float CTerrain::GetHeight(D3DXVECTOR3 pos)
{
	return GetHeight(pos.x, pos.z);
}

float CTerrain::GetHeight(float x, float z)
{
	int row = (z - m_pos.z)/m_cellHeight;
	int col = (x - m_pos.x)/m_cellWidth;
	if(row > m_row-2 || col > m_col-2 || row < 1 || col < 1)
	   //防止数组越界
	   return 0;
	//消除偏差
/*
//  C   D
//  *---*
//  | \ |
//  *---*  
//  A   B
	*/
	float A = m_height[row][col]*m_scale; 
	float B = m_height[row][col+1]*m_scale;
	float C = m_height[row+1][col]*m_scale; 
	float D = m_height[row+1][col+1]*m_scale; 
	float dx = (x - m_pos.x)/m_cellWidth - col;
	float dz = (z - m_pos.z)/m_cellHeight - row;
	//if(dz + dx < 1.0f)
	//{//在ABC内
		return A + (B-A)*dx + (C-A)*dz;
	//}
	//else
	//{
	//	return D + (D-C)*dx + (D-B)*dz;
	//}
}
void CTerrain::SetRenderStyle(int style)
{
	switch (style)
	{
	case TERRIAN_LINE:
	case TERRIAN_TRI:
		m_renderStyle  = style;
		break;
	default:
		m_renderStyle = TERRIAN_TRI; 
	}
}
void CTerrain::Render()
{
	D3DXMATRIX tran;
	D3DXMatrixTranslation(&tran, m_pos.x, m_pos.y, m_pos.z);
	m_device->SetTransform( D3DTS_WORLD, &tran);
	m_device->SetRenderState(D3DRS_LIGHTING, false);
	m_device->SetStreamSource( 0, m_vb, 0, sizeof(Terrain_Vertex));	
	m_device->SetFVF(FVF_XYZ_DIFF_TEX1);				
	if(m_renderStyle == TERRIAN_LINE)
	{
		m_device->SetTexture(0, NULL);
		m_device->SetIndices(m_lineB);	
		m_device->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, m_row*m_col, 0, (m_col-1)*m_row + (m_row-1)*m_col);
	}
	else if(m_renderStyle == TERRIAN_TRI)
	{	
		m_device->SetTexture(0, m_texture);
		m_device->SetIndices(m_ib);	
		m_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_row*m_col, 0, (m_row-1)*(m_col-1)*2);
	}
	D3DXMatrixTranslation(&tran, 0, 0, 0);
	m_device->SetTransform( D3DTS_WORLD, &tran);
}