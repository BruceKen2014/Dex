



#include "CMyFont.h"


CMyFont::CMyFont(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_device = pD3DDevice;
   ZeroMemory(&m_stFont, sizeof(D3DXFONT_DESCW));
   m_textSize = 20;

   m_stFont.CharSet = GB2312_CHARSET;
   memcpy(m_stFont.FaceName, "ËÎÌו", sizeof(m_stFont.FaceName));
   m_stFont.Height = 20;
   m_stFont.Width = 0;
   m_stFont.Weight = 400;
   m_stFont.Italic = FALSE;
   m_stFont.PitchAndFamily = DEFAULT_PITCH|FF_SWISS;
   m_stFont.OutputPrecision = OUT_DEFAULT_PRECIS;
   m_stFont.MipLevels = D3DX_DEFAULT;
   m_stFont.Quality = DEFAULT_QUALITY;
   D3DXCreateFontIndirect(m_device, (const D3DXFONT_DESCA *)&m_stFont, &m_Font);

   m_crFontColor = D3DCOLOR_ARGB(255, 0 ,0 ,0);
   m_useColor = m_crFontColor;
   m_bUseColor = false;

   m_Rect.left = m_Rect.top = 0; 
   m_Rect.right = m_Rect.bottom = 500;

   m_iId = 0;

   //if(FAILED(D3DXCreateFont(g_D3DDevice, 18, 0, FW_BOLD, 1, false,
   //   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
   //   DEFAULT_PITCH | FF_DONTCARE, "Arial",
   //   &g_Font))) return ;
}
CMyFont::~CMyFont()
{
	ShutDown();
}

void CMyFont::ShutDown()
{
	if(m_Font != NULL)
	{
		m_Font->Release();
		m_Font = NULL;
	}
}

void CMyFont::SetRect(RECT rect)
{
	m_Rect = rect;
}

void CMyFont::SetPos(int x, int y)
{
	long w = m_Rect.right - m_Rect.left;
	long h = m_Rect.bottom - m_Rect.top; 
	m_Rect.left = x;
	m_Rect.top = y;
	m_Rect.right = m_Rect.left + w;
	m_Rect.bottom = m_Rect.top + h;
}

void CMyFont::SetSize(int w, int h)
{
	m_Rect.right = m_Rect.left + w;
	m_Rect.bottom = m_Rect.top + h;
}

void CMyFont::ResetFont()
{  
	if(m_Font != NULL)
	{
		m_Font->Release();
		m_Font = NULL;
	}
	m_stFont.CharSet = GB2312_CHARSET;
	memcpy(m_stFont.FaceName, "ËÎÌו", sizeof(m_stFont.FaceName));
	m_stFont.Height = m_textSize;
	m_stFont.Width = 0;
	m_stFont.Weight = 400;
	m_stFont.Italic = FALSE;
	m_stFont.PitchAndFamily = DEFAULT_PITCH|FF_SWISS;
	m_stFont.OutputPrecision = OUT_DEFAULT_PRECIS;
	m_stFont.MipLevels = D3DX_DEFAULT;
	m_stFont.Quality = DEFAULT_QUALITY;
	D3DXCreateFontIndirect(m_device, (const D3DXFONT_DESCA *)&m_stFont, &m_Font);
}
void CMyFont::SetTextSize(int size)
{
	m_textSize = size;
	ResetFont();
}

void CMyFont::SetRect(int x, int y, int w, int h)
{
	m_Rect.left = x;
	m_Rect.right = x + w;
	m_Rect.top = y;
	m_Rect.bottom = y + h;
}

void CMyFont::Print(char * ch,...)
{
	if(m_Font == NULL)
		return ;
	va_list v_p;
	va_start(v_p, ch);
	TCHAR szText[1024];
	wvsprintf(szText, ch,v_p);
	va_end(v_p);
	if(m_bUseColor)
		m_Font->DrawText(NULL, szText, -1, &m_Rect, DT_LEFT | DT_TOP | DT_WORDBREAK,getD3DColor(m_useColor));
	else
	    m_Font->DrawText(NULL, szText, -1, &m_Rect, DT_LEFT | DT_TOP | DT_WORDBREAK, getD3DColor(m_crFontColor));
	m_bUseColor = false;
}