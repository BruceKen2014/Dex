


//#include "stdafx.h"
#include "../DexBase/typedefine.h"
#include "CSprite.h"


//========================================CSpriteOne=============================================
//-----------------构造、析构、私有及保护方法--------------------------
CSpriteOne::CSpriteOne(LPDIRECT3DDEVICE9 pD3DDevice, CTexture * texture, RECT& rcBounds, char * name, 
		int totalFrame  , int wFrame ,int hFrame ,int FrameDelay, BOUNDSACTION baBoundsAction )
{
	m_Texture = texture;
	m_pFont = new CMyFont(pD3DDevice);
	m_name = (char *)malloc(sizeof(char) *32);
	strcpy(m_name, name);
	m_pNote = (char *)malloc(sizeof(char) *32);
	strcpy(m_pNote, name);
	m_iNumFrames     =  totalFrame      ;             //总帧数
	m_iWNumFrames    =  wFrame		    ;
	m_iHNumFrames    =  hFrame		    ;
	m_iStartFrame    =  0               ;
    m_iEndFrame      =  totalFrame - 1  ;			   //起始帧数和结束帧数，默认为第0帧和最后一帧
	m_iCurFrame      =  0               ;			   //这样可以在某些环境下截取一幅动画中的某些帧而不必从头到尾播放全部帧
	m_iZOrder        =  0               ;
	m_iFrameDelay    =  FrameDelay      ;
	m_iFrameTrigger  =  0               ;
	m_ptVelocity.x   =  0               ;
	m_ptVelocity.y   =  0               ;	  
	m_bHidden        =  false           ;
    m_bDying         =  false           ;
	m_bOneCycle      =  false           ;
	m_bCycle         =  true            ;   
	m_bShowNote      =  FALSE           ;
	m_baBoundsAction =  baBoundsAction  ;
	m_alpha          =  255             ;                   //子画面半透明度

	if(m_Texture)
		SetRect(&m_rcPosition, 0, 0, m_Texture->GetWidth()/wFrame , m_Texture->GetHeight()/hFrame);
	else
		SetRect(&m_rcPosition, 0, 0, 100, 100);
	CopyRect(&m_rcBounds, &rcBounds);
	CalcCollisionRect();
	CalcNoteRect();
}

CSpriteOne::~CSpriteOne()
{
	ShutDown();
}
void CSpriteOne::UpdateFrame()
{
    if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0))
    {
      m_iFrameTrigger = m_iFrameDelay;
      if (++m_iCurFrame > m_iEndFrame)
      {
        if (m_bOneCycle)
          m_bDying = TRUE;
        else
          m_iCurFrame = m_iStartFrame;
      }
    }
}

void CSpriteOne::CalcCollisionRect()
{
    int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
    int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
    CopyRect(&m_rcCollision, &m_rcPosition);
    InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

void CSpriteOne::CalcNoteRect()
{
	m_rcNoteRect.left = m_rcPosition.left - 20;
	m_rcNoteRect.right = m_rcPosition.right + 20;
	m_rcNoteRect.top = m_rcPosition.top -20;
	m_rcNoteRect.bottom = m_rcPosition.bottom;
}

void CSpriteOne::ShutDown()
{
	if(m_Texture != NULL)     //这个指针是从外界传进来的，故只将本成员置为NULL而不在此释放空间，释放由源指针自己进行
	{
		m_Texture = NULL;
	}
	if(m_name != NULL)		  //本内存是由对象自己申请，故要由自身释放
	{
		free(m_name);
		m_name = NULL;
	}
	if(m_pNote != NULL)
	{
		free(m_pNote);
		m_pNote = NULL;
	}
	if(m_pFont != NULL)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

//------------------------------公有方法----------------------------------
SPRITEACTION CSpriteOne::Update()
{
  if(m_bCycle)	//精灵是否自动播放循环动画以及更新自身位置
  {
	  if (m_bDying)
		return SA_KILL;
	  UpdateFrame();

	  POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
	  ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	  ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	  ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	  ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	  ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	  ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;

	  if (m_baBoundsAction == BA_WRAP)
	  {
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left)
		  ptNewPosition.x = m_rcBounds.right;
		else if (ptNewPosition.x> m_rcBounds.right)
		  ptNewPosition.x = m_rcBounds.left - ptSpriteSize.x;
		if ((ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top)
		  ptNewPosition.y = m_rcBounds.bottom;
		else if (ptNewPosition.y> m_rcBounds.bottom)
		  ptNewPosition.y = m_rcBounds.top - ptSpriteSize.y;
	  }
	  else if (m_baBoundsAction == BA_BOUNCE)
	  {
		BOOL bBounce = FALSE;
		POINT ptNewVelocity = m_ptVelocity;
		if (ptNewPosition.x < m_rcBounds.left)
		{
		  bBounce = TRUE;
		  ptNewPosition.x = m_rcBounds.left;
		  ptNewVelocity.x = -ptNewVelocity.x;
		}
		else if ((ptNewPosition.x + ptSpriteSize.x) > m_rcBounds.right)
		{
		  bBounce = TRUE;
		  ptNewPosition.x = m_rcBounds.right - ptSpriteSize.x;
		  ptNewVelocity.x = -ptNewVelocity.x;
		}
		if (ptNewPosition.y < m_rcBounds.top)
		{
		  bBounce = TRUE;
		  ptNewPosition.y = m_rcBounds.top;
		  ptNewVelocity.y = -ptNewVelocity.y;
		}
		else if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom)
		{
		  bBounce = TRUE;
		  ptNewPosition.y = m_rcBounds.bottom - ptSpriteSize.y;
		  ptNewVelocity.y = -ptNewVelocity.y;
		}
		if (bBounce)
		  SetVelocity(ptNewVelocity);
	  }
	  else if (m_baBoundsAction == BA_DIE)
	  {
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
		  ptNewPosition.x > m_rcBounds.right ||
		  (ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
		  ptNewPosition.y > m_rcBounds.bottom)
		  return SA_KILL;
	  }
	  else
	  {
		if (ptNewPosition.x  < m_rcBounds.left ||
		  ptNewPosition.x > (m_rcBounds.right - ptSpriteSize.x))
		{
		  ptNewPosition.x = max(m_rcBounds.left, min(ptNewPosition.x,
			m_rcBounds.right - ptSpriteSize.x));
		  SetVelocity(0, 0);
		}
		if (ptNewPosition.y  < m_rcBounds.top ||
		  ptNewPosition.y > (m_rcBounds.bottom - ptSpriteSize.y))
		{
		  ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y,
			m_rcBounds.bottom - ptSpriteSize.y));
		  SetVelocity(0, 0);
		}
	  }
	  SetPosition(ptNewPosition);
	  return SA_NONE;

  }
  else
	  return SA_NONE;

}

void CSpriteOne::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
    if (m_Texture != NULL && !m_bHidden)
    {	
        if (m_iHNumFrames == 1 && m_iWNumFrames == 1)
		    m_Texture->Draw(pD3DDevice, m_rcPosition.left, m_rcPosition.top);  
	    else
            m_Texture->DrawPart(pD3DDevice, m_rcPosition.left, m_rcPosition.top,
                m_iCurFrame % m_iWNumFrames * GetWidth() , m_iCurFrame / m_iWNumFrames * GetHeight(), GetWidth(), GetHeight());
	    if(m_bShowNote && m_pNote != NULL)
		{
			m_pFont->SetPos(m_rcNoteRect.left, m_rcNoteRect.top);
		    m_pFont->Print(m_pNote );
		}
    }
}

void CSpriteOne::SetVelocity(int x, int y)
{
    m_ptVelocity.x = x;
    m_ptVelocity.y = y;
}

void CSpriteOne::SetVelocity( POINT ptVelocity)
{
    m_ptVelocity.x = ptVelocity.x;
    m_ptVelocity.y = ptVelocity.y;
}

void CSpriteOne::SetPosition(POINT ptPosition)
{
    OffsetRect(&m_rcPosition, ptPosition.x - m_rcPosition.left, ptPosition.y - m_rcPosition.top);
    CalcCollisionRect();
    CalcNoteRect();
}

void CSpriteOne::SetPosition(int x, int y)  
{
    OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
    CalcCollisionRect();
    CalcNoteRect();
}

BOOL CSpriteOne::IsPointInside(int x, int y)
{
    POINT ptPoint;
    ptPoint.x = x;
    ptPoint.y = y;
    return PtInRect(&m_rcPosition, ptPoint);
}

BOOL CSpriteOne::TestCollision(CSprite *pTestSprite)
{
    RECT& rcTest = pTestSprite->GetCollision();
    return m_rcCollision.left <= rcTest.right &&
         rcTest.left <= m_rcCollision.right &&
         m_rcCollision.top <= rcTest.bottom &&
         rcTest.top <= m_rcCollision.bottom;
}

void CSpriteOne::GetName(char * in)
{
	if(in != NULL)
	{
		free(in);
		in = NULL;
	}
	in = (char *)malloc(sizeof(char) * 32);
	strcpy(in, m_name);
	
}

void CSpriteOne::SetCurrFrame(int i)
{
    if(i < m_iNumFrames && i>=0)
	    m_iCurFrame = i ;
	else
	    m_iCurFrame = 0;
}

void CSpriteOne::SetEndFrame(int i)
{
    if(i < m_iNumFrames && i>=0)
	    m_iEndFrame = i ;
	else
	    m_iEndFrame = m_iNumFrames - 1;
}

void CSpriteOne::SetNumFrames(int iNumFrames)
{

    m_iNumFrames = iNumFrames;
    m_iEndFrame = m_iNumFrames;
}

void CSpriteOne::SetNote(char *ch)
{
	if(m_pNote != NULL)
	{
		free(m_pNote);
		m_pNote = NULL;
	}
	m_pNote = (char *)malloc(sizeof(char) *32);
	strcpy(m_pNote, ch);
}




//========================================CSpriteMul=============================================
//-----------------构造、析构、私有及保护方法--------------------------

CSpriteMul::CSpriteMul(LPDIRECT3DDEVICE9 pD3DDevice, CTextureEx **texture, RECT &rcBounds, char *name, 
					   int totalFrame , int FrameDelay, BOUNDSACTION baBoundsAction)
{
	m_ppTexture = NULL;
	m_ppTexture = texture;
	m_pFont = new CMyFont(pD3DDevice);
	m_name = (char *)malloc(sizeof(char) *32);
	strcpy(m_name, name);
	m_pNote = (char *)malloc(sizeof(char) *32);
	strcpy(m_pNote, name);
	m_iNumFrames     =  totalFrame      ;             //总帧数
	m_iStartFrame    =  0               ;
    m_iEndFrame      =  totalFrame - 1  ;			   //起始帧数和结束帧数，默认为第0帧和最后一帧
	m_iCurFrame      =  0               ;			   //这样可以在某些环境下截取一幅动画中的某些帧而不必从头到尾播放全部帧
	m_iZOrder        =  0               ;
	m_iFrameDelay    =  FrameDelay      ;
	m_iFrameTrigger  =  0               ;
	m_ptVelocity.x   =  0               ;
	m_ptVelocity.y   =  0               ;	  
	m_bHidden        =  false           ;
    m_bDying         =  false           ;
	m_bOneCycle      =  false           ;
	m_bCycle         =  true            ;   
	m_bShowNote      =  FALSE           ;
	m_baBoundsAction =  baBoundsAction  ;
	m_alpha          =  255             ;                   //子画面半透明度

	if(m_ppTexture)
		SetRect(&m_rcPosition, 0, 0, m_ppTexture[0]->GetWidth() , m_ppTexture[0]->GetHeight());
	else
		SetRect(&m_rcPosition, 0, 0, 100, 100);
	CopyRect(&m_rcBounds, &rcBounds);
	CalcCollisionRect();
	CalcNoteRect();
}

CSpriteMul::~CSpriteMul()
{
	ShutDown();
}

void CSpriteMul::CalcCollisionRect()
{
    int iXShrink = (m_rcPosition.left - m_rcPosition.right) / 12;
    int iYShrink = (m_rcPosition.top - m_rcPosition.bottom) / 12;
    CopyRect(&m_rcCollision, &m_rcPosition);
    InflateRect(&m_rcCollision, iXShrink, iYShrink);
}

void CSpriteMul::CalcNoteRect()
{
	m_rcNoteRect.left = m_rcPosition.left - 20;
	m_rcNoteRect.right = m_rcPosition.right + 20;
	m_rcNoteRect.top = m_rcPosition.top -20;
	m_rcNoteRect.bottom = m_rcPosition.bottom;
}

void CSpriteMul::UpdateFrame()
{
    if ((m_iFrameDelay >= 0) && (--m_iFrameTrigger <= 0))
    {
      m_iFrameTrigger = m_iFrameDelay;
      if (++m_iCurFrame > m_iEndFrame)
      {
        if (m_bOneCycle)
          m_bDying = TRUE;
        else
          m_iCurFrame = m_iStartFrame;
      }
    }
}

void CSpriteMul::ShutDown()
{
	if(m_ppTexture != NULL)     //这个指针是从外界传进来的，故只将本成员置为NULL而不在此释放空间，释放由源指针自己进行
	{
		for(int i = 0; i < m_iNumFrames; i++ )
			m_ppTexture[i] = NULL;
		m_ppTexture = NULL;
	}
	if(m_name != NULL)		  //本内存是由对象自己申请，故要由自身释放
	{
		free(m_name);
		m_name = NULL;
	}
	if(m_pNote != NULL)
	{
		free(m_pNote);
		m_pNote = NULL;
	}
	if(m_pFont != NULL)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

SPRITEACTION CSpriteMul::Update()
{
  if(m_bCycle)	//精灵是否自动播放循环动画以及更新自身位置
  {
	  if (m_bDying)
		return SA_KILL;
	  UpdateFrame();

	  POINT ptNewPosition, ptSpriteSize, ptBoundsSize;
	  ptNewPosition.x = m_rcPosition.left + m_ptVelocity.x;
	  ptNewPosition.y = m_rcPosition.top + m_ptVelocity.y;
	  ptSpriteSize.x = m_rcPosition.right - m_rcPosition.left;
	  ptSpriteSize.y = m_rcPosition.bottom - m_rcPosition.top;
	  ptBoundsSize.x = m_rcBounds.right - m_rcBounds.left;
	  ptBoundsSize.y = m_rcBounds.bottom - m_rcBounds.top;

	  if (m_baBoundsAction == BA_WRAP)
	  {
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left)
		  ptNewPosition.x = m_rcBounds.right;
		else if (ptNewPosition.x> m_rcBounds.right)
		  ptNewPosition.x = m_rcBounds.left - ptSpriteSize.x;
		if ((ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top)
		  ptNewPosition.y = m_rcBounds.bottom;
		else if (ptNewPosition.y> m_rcBounds.bottom)
		  ptNewPosition.y = m_rcBounds.top - ptSpriteSize.y;
	  }
	  else if (m_baBoundsAction == BA_BOUNCE)
	  {
		BOOL bBounce = FALSE;
		POINT ptNewVelocity = m_ptVelocity;
		if (ptNewPosition.x < m_rcBounds.left)
		{
		  bBounce = TRUE;
		  ptNewPosition.x = m_rcBounds.left;
		  ptNewVelocity.x = -ptNewVelocity.x;
		}
		else if ((ptNewPosition.x + ptSpriteSize.x) > m_rcBounds.right)
		{
		  bBounce = TRUE;
		  ptNewPosition.x = m_rcBounds.right - ptSpriteSize.x;
		  ptNewVelocity.x = -ptNewVelocity.x;
		}
		if (ptNewPosition.y < m_rcBounds.top)
		{
		  bBounce = TRUE;
		  ptNewPosition.y = m_rcBounds.top;
		  ptNewVelocity.y = -ptNewVelocity.y;
		}
		else if ((ptNewPosition.y + ptSpriteSize.y) > m_rcBounds.bottom)
		{
		  bBounce = TRUE;
		  ptNewPosition.y = m_rcBounds.bottom - ptSpriteSize.y;
		  ptNewVelocity.y = -ptNewVelocity.y;
		}
		if (bBounce)
		  SetVelocity(ptNewVelocity);
	  }
	  else if (m_baBoundsAction == BA_DIE)
	  {
		if ((ptNewPosition.x + ptSpriteSize.x) < m_rcBounds.left ||
		  ptNewPosition.x > m_rcBounds.right ||
		  (ptNewPosition.y + ptSpriteSize.y) < m_rcBounds.top ||
		  ptNewPosition.y > m_rcBounds.bottom)
		  return SA_KILL;
	  }
	  else
	  {
		if (ptNewPosition.x  < m_rcBounds.left ||
		  ptNewPosition.x > (m_rcBounds.right - ptSpriteSize.x))
		{
		  ptNewPosition.x = max(m_rcBounds.left, min(ptNewPosition.x,
			m_rcBounds.right - ptSpriteSize.x));
		  SetVelocity(0, 0);
		}
		if (ptNewPosition.y  < m_rcBounds.top ||
		  ptNewPosition.y > (m_rcBounds.bottom - ptSpriteSize.y))
		{
		  ptNewPosition.y = max(m_rcBounds.top, min(ptNewPosition.y,
			m_rcBounds.bottom - ptSpriteSize.y));
		  SetVelocity(0, 0);
		}
	  }
	  SetPosition(ptNewPosition);
	  return SA_NONE;
  }
  else
	  return SA_NONE;
}

void CSpriteMul::Draw(LPDIRECT3DDEVICE9 pD3DDevice)
{
    if (m_ppTexture != NULL && !m_bHidden)
		m_ppTexture[m_iCurFrame]->Draw(pD3DDevice, m_rcPosition.left, m_rcPosition.top);

    if(m_bShowNote  && m_pNote != NULL)
	{
		m_pFont->SetPos( m_rcNoteRect.left, m_rcNoteRect.top);
		m_pFont->Print(m_pNote);
	}
}

void CSpriteMul::SetPosition(int x, int y)  
{
    OffsetRect(&m_rcPosition, x - m_rcPosition.left, y - m_rcPosition.top);
    CalcCollisionRect();
    CalcNoteRect();
}


