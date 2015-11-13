

#include "DexSceneObjectFlower.h"
#include "../Source/CTexture.h"
#include "../widget/CDexUiSrcMgr.h"
#include "../state/DexGameEngine.h"

CDexSceneObjectFlower::CDexSceneObjectFlower()
{
	m_pTex = NULL;
	m_fSwingFactor = 0.3f;
	m_fCurrSwing = 0.0f;
	m_fSwingSpeed = 2.0f;
	m_bSwingFron = true;
	m_bAlphaSort = true;
	m_bRenderNodeCube = false;
}

CDexSceneObjectFlower::~CDexSceneObjectFlower()
{
	_REDUCEREF(m_pTex);
}

bool CDexSceneObjectFlower::Update(int delta)
{
	DEX_ENSURE_B(CDexSceneObject::Update(delta));

	if(m_bSwingFron)
	{
		m_fCurrSwing +=  2 * m_fSwingFactor * delta/1000.0f/m_fSwingSpeed;
		if(m_fCurrSwing >= m_fSwingFactor)
		{
			m_fCurrSwing = m_fSwingFactor;
			m_bSwingFron = false;
		}
	}
	else
	{
		m_fCurrSwing -=  2 * m_fSwingFactor * delta/1000.0f/m_fSwingSpeed;
		if(m_fCurrSwing <= -m_fSwingFactor)
		{
			m_fCurrSwing = -m_fSwingFactor;
			m_bSwingFron = true;
		}
	}
	return true;
}

bool CDexSceneObjectFlower::Render()
{
	DEX_ENSURE_B(CDexSceneObject::Render());

	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);	  //必须禁止深度缓冲的写入
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );	
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE ); 
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1 );
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &m_finalMatrix);
	bool fog = DexGameEngine::getEngine()->OpenFog();
	if(fog)
		DexGameEngine::getEngine()->DisableFog();
	if(m_pTex != NULL)
		DexGameEngine::getEngine()->GetDevice()->SetTexture(0, m_pTex->GetTexPt());
	g_pVertexList1[0].m_color = getD3DColor(DEXCOLOR_WHITE);
	g_pVertexList1[0].m_u = 0.0f;
	g_pVertexList1[0].m_v = 0.0f;
	g_pVertexList1[1].m_color = getD3DColor(DEXCOLOR_WHITE);
	g_pVertexList1[1].m_u = 1.0f;
	g_pVertexList1[1].m_v = 0.0f;
	g_pVertexList1[2].m_color = getD3DColor(DEXCOLOR_WHITE);
	g_pVertexList1[2].m_u = 0.0f;
	g_pVertexList1[2].m_v = 1.0f;
	g_pVertexList1[3].m_color = getD3DColor(DEXCOLOR_WHITE);
	g_pVertexList1[3].m_u = 1.0f;
	g_pVertexList1[3].m_v = 1.0f;
	//画XOY平面内的矩形
	g_pVertexList1[0].m_pos = D3DXVECTOR3(-1.0f + m_fCurrSwing, 2.0f, 0.0f);
	g_pVertexList1[1].m_pos = D3DXVECTOR3(1.0f + m_fCurrSwing, 2.0f, 0.0f);
	g_pVertexList1[2].m_pos = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	g_pVertexList1[3].m_pos = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_pVertexList1, sizeof(stVertex1));
	//画YOZ平面内的矩形
	g_pVertexList1[0].m_pos = D3DXVECTOR3(0.0f, 2.0f, -1.0f + m_fCurrSwing);
	g_pVertexList1[1].m_pos = D3DXVECTOR3(0.0f, 2.0f, 1.0f + m_fCurrSwing);
	g_pVertexList1[2].m_pos = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	g_pVertexList1[3].m_pos = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_pVertexList1, sizeof(stVertex1));

	D3DXMatrixTranslation(&g_transMatrix, 0, 0, 0);
	DexGameEngine::getEngine()->GetDevice()->SetTransform( D3DTS_WORLD, &g_transMatrix);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);		  //状态恢复
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	if(fog)
		DexGameEngine::getEngine()->EnableFog();
	return true;

}

void CDexSceneObjectFlower::Reset()
{
	CDexSceneObject::Reset();
	m_fSwingFactor = 1.0f;
	m_fCurrSwing = 0.0f;
	m_fSwingSpeed = 1.0f;
	m_bSwingFron = true;
	_REDUCEREF(m_pTex);
}

void CDexSceneObjectFlower::SetTexture(string filename)
{
	_REDUCEREF(m_pTex);
	m_pTex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(filename.c_str());
	_ADDREF(m_pTex);
}

void CDexSceneObjectFlower::setSwingFactor(float factor)
{
	m_fSwingFactor = factor;
}

void CDexSceneObjectFlower::setSwingTime(float time)
{
	m_fSwingSpeed = time;
}