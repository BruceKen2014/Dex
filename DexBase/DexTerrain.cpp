


#include "DexTerrain.h"
#include "../Source/CCommandScript.h"
#include "DexLog.h"
#include "../state/DexGameEngine.h"
#include "../Source/CTexture.h"
#include "../widget/CDexUiSrcMgr.h"
#include "CFile.h"

//================================================================================
CDexTerrain::CDexTerrain()
{
	m_vb = NULL;
	m_ib = NULL;
	m_lineB = NULL;
	m_render = true;
	m_pTex = NULL;
	m_pos = D3DXVECTOR3(-Terrain_row*Terrain_cell_height/2,0,-Terrain_col*Terrain_cell_width/2);
	m_renderStyle = TERRIAN_LINE;
	for(int row = 0; row < Terrain_row; row++)
	{
		for(int col = 0; col < Terrain_col; col++)
		{
			m_height[row][col] = 0;
		}
	}
	int num = Terrain_row * Terrain_col;
	m_vbVertex = new stVertex1[num];

}

CDexTerrain::~CDexTerrain()
{
	_SafeRelease(m_vb);
	_SafeRelease(m_ib);
	_SafeRelease(m_lineB);
	_SafeDeleteArr(m_vbVertex);
	_REDUCEREF(m_pTex);
}

void CDexTerrain::_updateVertex()
{
	for(int i = 0; i < Terrain_row; i++)
	{
		for(int j = 0; j< Terrain_col; j++)
		{
			m_vbVertex[i*Terrain_col + j].m_pos.y = m_height[i][j]; 
		}

	}
	void *ptr;
	if( FAILED(m_vb->Lock( 0, Terrain_row *  Terrain_col * sizeof(stVertex1), (void**)&ptr, 0 ) ) )
		return;	
	memcpy(ptr, m_vbVertex, Terrain_row *  Terrain_col  * sizeof(stVertex1));
	if( FAILED(m_vb->Unlock() ) ) 
		return; 
}


bool CDexTerrain::CreateTerrain()
{
	if(FAILED(DexGameEngine::getEngine()->GetDevice()->CreateVertexBuffer(Terrain_row * Terrain_col * sizeof(stVertex1),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1, D3DPOOL_DEFAULT,
		&m_vb, NULL))) 
		return false;

	int num = Terrain_row * Terrain_col;
	for(int i = 0; i < Terrain_row; i++)
	{
		for(int j = 0; j< Terrain_col; j++)
		{
			m_vbVertex[i*Terrain_col + j].m_pos.x += j * Terrain_cell_width;
			m_vbVertex[i*Terrain_col + j].m_pos.z += i * Terrain_cell_height;
			m_vbVertex[i*Terrain_col + j].m_pos.y += 0.0f;//m_height[i][j] * m_scale; 
			if(j%2 == 0)
			{
				m_vbVertex[i*Terrain_col + j].m_u = 0.0f ;
			}
			else
				m_vbVertex[i*Terrain_col + j].m_u = 1.0f ;
			if(i%2 == 0)
			{
				m_vbVertex[i*Terrain_col + j].m_v = 0.0f ;
			}
			else
				m_vbVertex[i*Terrain_col + j].m_v = 1.0f ;
		}

	}
	void *ptr;
	if( FAILED(m_vb->Lock( 0, num * sizeof(stVertex1), (void**)&ptr, 0 ) ) )
		return false;	
	memcpy(ptr, m_vbVertex, num * sizeof(stVertex1));
	if( FAILED(m_vb->Unlock() ) ) 
		return false; 


	int *tempindex = new int[(Terrain_row-1)*(Terrain_col-1)*6];
	for(int i = 0; i < Terrain_row - 1;i++)
	{
		for(int j = 0; j < Terrain_col -1; j++)
		{
			tempindex[(i*(Terrain_col-1)+j)*6] = Terrain_col*i + j;
			tempindex[(i*(Terrain_col-1)+j)*6 +1] = Terrain_col*(i+1) + j;
			tempindex[(i*(Terrain_col-1)+j)*6 +2] = Terrain_col*i + j+1;

			tempindex[(i*(Terrain_col-1)+j)*6 +3] = Terrain_col*i + j+1 ;
			tempindex[(i*(Terrain_col-1)+j)*6 +4] = Terrain_col*(i+1) + j;
			tempindex[(i*(Terrain_col-1)+j)*6 +5] = Terrain_col*(i+1) + j+1;
		}
	}
	if(FAILED(DexGameEngine::getEngine()->GetDevice()->CreateIndexBuffer((Terrain_row-1)*(Terrain_col-1)*6 * sizeof(int),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT,
		&m_ib, NULL))) 
		return false;
	void *ptr2;
	if( FAILED(m_ib->Lock( 0, (Terrain_row-1)*(Terrain_col-1)*6 * sizeof(int), (void**)&ptr2, 0 ) ) )
		return false;	
	memcpy(ptr2, tempindex, (Terrain_row-1)*(Terrain_col-1)*6 * sizeof(int));
	if( FAILED(m_ib->Unlock() ) ) 
		return false;  
	_SafeDeleteArr(tempindex); 

	//线条索引
	int line_number = ((Terrain_col-1)*Terrain_row + (Terrain_row-1)*Terrain_col)*2;
	tempindex = new int[line_number];
	for(int i = 0; i < Terrain_row - 1;i++)
	{
		for(int j = 0; j < Terrain_col - 1; j++)
		{
			tempindex[i*(Terrain_col-1)*4 + j*4] = Terrain_col*i + j;
			tempindex[i*(Terrain_col-1)*4 + j*4 + 1] = Terrain_col*i + j + 1;

			tempindex[i*(Terrain_col-1)*4 + j*4 + 2] = Terrain_col*i + j;
			tempindex[i*(Terrain_col-1)*4 + j*4 + 3] = Terrain_col*(i+1) + j;
		}
	}
	int row_number = (Terrain_col-1)*(Terrain_row-1)*2;
	int col_number = (Terrain_row-1)*(Terrain_col-1)*2;
	int start = line_number - row_number - col_number; 
	//最上面一层
	for(int j = 0; j < Terrain_col - 1; j++)
	{
		tempindex[start + j*2] = (Terrain_row-1)*Terrain_col + j;
		tempindex[start + j*2 + 1] = (Terrain_row-1)*Terrain_col + j + 1;
	}
	//最右面一层
	start = line_number - 2*(Terrain_row-1); 
	for(int i = 0; i < Terrain_row - 1; i++)
	{
		tempindex[start + i*2] = Terrain_row*i + Terrain_col - 1;
		tempindex[start + i*2 + 1] = Terrain_row*(i+1) + Terrain_col - 1;
	}
	if(FAILED(DexGameEngine::getEngine()->GetDevice()->CreateIndexBuffer(line_number* sizeof(int),
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
	return true;
}

void CDexTerrain::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

int CDexTerrain::GetHeight(int row, int col)
{
	if(row < 0 || row >= Terrain_row || col < 0 || col >= Terrain_col)
		return 0;
	return m_height[row][col];
}

void CDexTerrain::SetHeight(int row, int col, int height)
{
	DEX_ENSURE(row >= 0 && row < Terrain_row);
	DEX_ENSURE(col >= 0 && col < Terrain_col);
	m_height[row][col] = height;
	_updateVertex();
}

float CDexTerrain::GetHeight(D3DXVECTOR3 pos)
{
	return GetHeight(pos.x, pos.z);
}

float CDexTerrain::GetHeight(float x, float z)
{
	int row = (z - m_pos.z)/Terrain_cell_height;
	int col = (x - m_pos.x)/Terrain_cell_width;
	if(row > Terrain_row-2 || col > Terrain_col-2 || row < 1 || col < 1)
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
	float A = m_height[row][col]; 
	float B = m_height[row][col+1];
	float C = m_height[row+1][col]; 
	float D = m_height[row+1][col+1]; 
	float dx = (x - m_pos.x)/Terrain_cell_width - col;
	float dz = (z - m_pos.z)/Terrain_cell_height - row;
	//if(dz + dx < 1.0f)
	//{//在ABC内
		return A + (B-A)*dx + (C-A)*dz;
	//}
	//else
	//{
	//	return D + (D-C)*dx + (D-B)*dz;
	//}
}
void CDexTerrain::SetRenderStyle(int style)
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
void CDexTerrain::Reset()
{

}
bool CDexTerrain::Update(int delta)
{
	DEX_ENSURE_B(CDexSceneObject::Update(delta));
	return true;
}
bool CDexTerrain::Render()
{
	DEX_ENSURE_B(CDexSceneObject::Render());
	D3DXMATRIX tran;
	D3DXMatrixTranslation(&tran, m_pos.x, m_pos.y, m_pos.z);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &tran);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	DexGameEngine::getEngine()->GetDevice()->SetStreamSource( 0, m_vb, 0, sizeof(stVertex1));	
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);				
	if(m_renderStyle == TERRIAN_LINE)
	{
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
		DexGameEngine::getEngine()->GetDevice()->SetIndices(m_lineB);	
		DexGameEngine::getEngine()->GetDevice()->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, Terrain_row*Terrain_col, 0, (Terrain_col-1)*Terrain_row + (Terrain_row-1)*Terrain_col);
	}
	else if(m_renderStyle == TERRIAN_TRI)
	{	
		if(m_pTex != NULL)
			DexGameEngine::getEngine()->GetDevice()->SetTexture(0, m_pTex->GetTexPt());
		else
			DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
		DexGameEngine::getEngine()->GetDevice()->SetIndices(m_ib);	
		DexGameEngine::getEngine()->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, Terrain_row*Terrain_col, 0, (Terrain_row-1)*(Terrain_col-1)*2);
	}
	D3DXMatrixTranslation(&tran, 0, 0, 0);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &tran);
	return true;
}

D3DXVECTOR3 CDexTerrain::GetPosition()
{
	return m_pos;
}

bool CDexTerrain::SetTexture(const char* texname)
{
	_REDUCEREF(m_pTex);
	m_pTex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(texname);
	if(m_pTex != NULL)
	{
		_ADDREF(m_pTex);
		return true;	
	}
	return false;
	
}

bool CDexTerrain::InitFromFile(const char* filename)
{
	getLog()->BeginLog();
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		getLog()->Log(log_allert, "open terrain file %s failed!\n",filename );
		getLog()->EndLog();
		pScript->CloseScript();
		return false;
	}

	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	for(int row = 0 ; row < Terrain_row; row++)
	{	
		pScript->SetCurrChar();
		for(int col = 0 ; col < Terrain_col; col++)
		{
			m_height[row][col] = pScript->GetIntParam();
		}
		pScript->MoveCurrLine();
	}
	_updateVertex();
	pScript->CloseScript();
	getLog()->Log(log_ok, "open terrain file%s successfully!\n",filename );
	getLog()->EndLog();

	return true;
}


bool CDexTerrain::SaveToFile(const char* filename)
{
	getLog()->BeginLog();

	CFile* file = getFileHandler();
	if(!file->CreateCFile((char*)filename))
	{
		getLog()->Log(log_allert, "save terrian file % failed！\n", filename);
		getLog()->EndLog();
		return false;
	}
	for(int row = 0; row < Terrain_row; row++)
	{
		for(int col = 0; col < Terrain_col; col++)
		{
			file->InNumber(m_height[row][col]);
			file->InBlankChar();
		}
		file->InBlankLine();
	}
	file->Close();
	getLog()->Log(log_ok, "save terrian file %s successfully！\n", filename);
	getLog()->EndLog();
	return true;
}
