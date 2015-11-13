


#include "CTexture.h"
#include "../state/DexGameEngine.h"
#include "../DexBase/CDexDraw2D.h"

CDexTex::CDexTex()
{
	m_tex = NULL;
	m_width = m_height = 0;
}
CDexTex::~CDexTex()
{
	_SafeRelease(m_tex);
}

bool CDexTex::LoadTex(string filename, const DexColor& transcolor)
{
	name = filename;
	D3DXIMAGE_INFO info;
	if(D3DXCreateTextureFromFileEx(DexGameEngine::getEngine()->GetDevice(), filename.c_str() , D3DX_DEFAULT_NONPOW2/*使用此参数才不会放大，使用0或者D3DX_DEFAULT图片会被放大一倍*/
		, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,0, D3DFMT_UNKNOWN, 
		D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE,
		getD3DColor(transcolor), &info, NULL, &m_tex) != D3D_OK)
	{
		//_Message("加载纹理%s失败!",filename);
		return false;
	}
	m_width = info.Width;
	m_height = info.Height;

	return true;
}
std::string CDexTex::FileName()
{
	std::vector<std::string> out;
	SplitStr(name,'/', out);
	if(out.size() == 0)
		return "";
	return out[out.size() - 1];
}

CTexture::CTexture()
{
	m_render = true;
}
void CTexture::SetXMirro(bool b)
{
	m_xMirro = b;
	if(m_xMirro)
		m_scale.x = -fabs(m_scale.x);
	else
		m_scale.x = fabs(m_scale.x);
}

bool CTexture::GetXMirro()
{
	return m_xMirro;
}

void CTexture::SetYMirro(bool b)
{
	m_yMirro = b;
	if(m_yMirro)
		m_scale.y = -fabs(m_scale.y);
	else
		m_scale.y = fabs(m_scale.y);
}

bool CTexture::GetYMirro()
{
	return m_yMirro;
}

void CTexture::SetRender(bool b)
{
	m_render = b;
}

bool CTexture::CompareFile(const char* filename)
{
	return (strcmp(m_fileName, filename) == 0);	
}

CTextureEx::CTextureEx():CTexture()
{
	m_pTexture     = NULL;
	m_scale        = D3DXVECTOR2(1.0f, 1.0f);
	m_center       = D3DXVECTOR2(0, 0);;
	m_iWidth       = 0;
	m_iHeight      = 0;
	m_iSrcWidth    = 0;
	m_iSrcHeight   = 0;
	m_fRotation    = 0.0f;
	m_crTransColor = -1;
	m_xMirro       = false;
	m_yMirro       = false;
	m_crAlpha      = 0xFFFFFFFF;     //默认完全不透明 

}

CTextureEx::~CTextureEx()
{
	ShutDown();
}

void CTextureEx::ShutDown()
{
	if(m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	_SafeDeleteArr(m_fileName);
}

BOOL CTextureEx::Load(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, const DexColor& TransColor)
{
   D3DXIMAGE_INFO info;
   if(D3DXCreateTextureFromFileEx(pD3DDevice, filename, D3DX_DEFAULT_NONPOW2/*使用此参数才不会放大，使用0或者D3DX_DEFAULT图片会被放大一倍*/
	   , D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,0, D3DFMT_UNKNOWN, 
	   D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE,
	  getD3DColor(TransColor), &info, NULL, &m_pTexture) != D3D_OK)
   {
	   _Message("加载纹理%s失败!",filename);
	   return FALSE;
   }
   m_fileName = new char[MAX_IMAGE_NAME_BYTE];
   memset(m_fileName, 0, MAX_IMAGE_NAME_BYTE);
   strcpy(m_fileName, filename);
   m_crTransColor = TransColor;
   m_iWidth = m_iSrcWidth = info.Width;
   m_iHeight = m_iSrcHeight = info.Height;

   return TRUE;
}

void CTextureEx::Draw(LPDIRECT3DDEVICE9 pD3DDevice, float xPos, float yPos, int flag)
{
	DrawPart(pD3DDevice, xPos, yPos, 0, 0, m_iSrcWidth, m_iSrcHeight, flag);
}

void CTextureEx::DrawPart(LPDIRECT3DDEVICE9 pD3DDevice, float xPos, float yPos, float srcXPos, float srcYPos, float width, float height,int flag)
{	
	if(!m_render)
		return;
	D3DXVECTOR2 pos(xPos, yPos);
	if(m_xMirro)
	{
		pos.x += m_iWidth; 
	}
	if(m_yMirro)
	{
		pos.y +=m_iHeight;
	}
	if(flag == DexGUI::DRAW_TOP_RIGHT)
		pos.x -=m_iWidth;
	else if(flag == DexGUI::DRAW_BOTTOM_LEFT)
		pos.y -=m_iHeight;
	else if(flag == DexGUI::DRAW_BOTTOM_RIGHT)
	{
		pos.x -= m_iWidth;
		pos.y -= m_iHeight;
	}
	else if(flag == DexGUI::DRAW_TOP_CENTER)
	{
		pos.x -= m_iWidth/2;
	}
	RECT Rect;
	Rect.left = srcXPos;
	Rect.right = srcXPos + width;
	Rect.top = srcYPos;
	Rect.bottom = srcYPos + height;
	D3DXVECTOR2 p0,p1,p2,p3;
	p0.x = ((float)srcXPos)/m_iSrcWidth;
	p0.y = ((float)srcYPos)/m_iSrcHeight;
	p2.x = ((float)(srcXPos+width))/m_iSrcWidth;
	p2.y = ((float)(srcYPos+height))/m_iSrcHeight;
	p1.x = p2.x;
	p1.y = p0.y;
	p3.x = p0.x;
	p3.y = p2.y;

	get2DDrawer()->ResetMatrix();	
	get2DDrawer()->SetAlpha(m_crAlpha);
	get2DDrawer()->SetScale(width, height);
	get2DDrawer()->SetUV(p0, p1, p2, p3);
	get2DDrawer()->SetRotate(m_fRotation);

	get2DDrawer()->SetTrans(pos.x,pos.y);
	get2DDrawer()->Draw(m_pTexture);
}


 void CTextureEx::SetScaleX(float scale)
{
	m_scale.x = scale; 
	m_iWidth = m_iSrcWidth * scale;
}

 void CTextureEx::SetScaleY(float scale) 
{
	m_scale.y = scale; 
	m_iHeight = m_iSrcHeight *scale;
}

 void CTextureEx::SetScale(float xScale, float yScale)
{ 
	m_scale.x = xScale;
	m_iWidth = m_iSrcWidth * xScale;

	m_scale.y = yScale;
	m_iHeight = m_iSrcHeight *yScale;
}

 void CTextureEx::SetAlpha(int alpha)
 { 
	 m_crAlpha = D3DCOLOR_ARGB(alpha, 255,255,255);
 }
 void CTextureEx::SetAlphaColor(int alphaColor)
 {
	 m_crAlpha = alphaColor;
 }
