/*************************************************************
 ****DemoEx引擎精灵动画处理文件CSprite.h
 ****简介：CSprite提供一个接口，具体由两种不同的sprite实现
 ****作者：BruceKen
 *************************************************************/


#ifndef _CSPRITE_H
#define _CSPRITE_H

#include "CTexture.h"
#include "CMyFont.h"


typedef WORD        SPRITEACTION;
const SPRITEACTION  SA_NONE      = 0x0000L,
                    SA_KILL      = 0x0001L,
                    SA_ADDSPRITE = 0x0002L;

typedef WORD        BOUNDSACTION;
const BOUNDSACTION  BA_STOP   = 0,
                    BA_WRAP   = 1,
                    BA_BOUNCE = 2,
                    BA_DIE    = 3;

class CSprite	
{
protected:
	CMyFont *    m_pFont;
	char *       m_name;
	char *       m_pNote;  
	short        m_iNumFrames;             //总帧数
	short        m_iStartFrame;
    short        m_iEndFrame  ;			   //起始帧数和结束帧数，默认为第0帧和最后一帧
	int          m_iCurFrame;			   //这样可以在某些环境下截取一幅动画中的某些帧而不必从头到尾播放全部帧
	int          m_iZOrder;
	long         m_iFrameDelay;
	long         m_iFrameTrigger;	  
	bool         m_bHidden;
	bool         m_bDying;
	bool         m_bOneCycle;
	bool         m_bCycle; 
	BOOL         m_bShowNote;
	RECT		 m_rcPosition;
	RECT         m_rcCollision;  
	RECT         m_rcBounds;
	RECT         m_rcNoteRect;
	POINT        m_ptVelocity;
	BOUNDSACTION m_baBoundsAction;
	int          m_alpha;                   //子画面半透明度

protected:
    virtual void    UpdateFrame() = 0;
	virtual void    CalcCollisionRect() = 0;	 //计算碰撞矩形
	virtual void    CalcNoteRect() = 0;	         //计算注释显示矩形
	virtual void    ShutDown() = 0;
public:
	CSprite() {};
	virtual ~CSprite() {};
    virtual SPRITEACTION   Update() = 0 ;
	virtual CSprite* AddSprite() = 0;

	virtual void    Draw(LPDIRECT3DDEVICE9 pD3DDevice) = 0 ;
	virtual BOOL    IsPointInside(int x, int y) = 0;
	virtual BOOL    TestCollision(CSprite * pTestSprite) = 0;

	virtual void	GetName(char * in) = 0     ;
	virtual int     GetWidth() = 0             ;
	virtual int     GetHeight() = 0            ;
	virtual int     GetStartFrame() = 0        ;
	virtual int     GetEndFrame() = 0          ;
	virtual RECT&   GetCollision() = 0         ;
    virtual RECT&   GetPosition() = 0          ;
	virtual POINT   GetVelocity() = 0          ;
	virtual int     GetZOrder() = 0            ;
	virtual BOOL    GetIsCycle() = 0           ; //返回当前sprite是否自动更新


	virtual void    SetAlpha(int alpha) = 0         ;
	virtual void    SetPosition(int x, int y) = 0     ;
	virtual void    SetPosition(POINT ptPosition)= 0  ;
	virtual void    SetVelocity(int x, int y)= 0      ;
	virtual void    SetXVelocity(int x)  = 0          ;
	virtual void    SetYVelocity(int y) = 0           ;
    virtual void    SetVelocity(POINT ptVelocity) = 0 ;
	virtual void    SetZOrder(int iZOrder) = 0        ;
	virtual void    SetBounds(RECT& rcBounds) = 0     ;
	virtual void    SetBoundsAction(BOUNDSACTION ba)=0;
	virtual void    SetHidden(bool bHidden) = 0       ;
	virtual void    SetCurrFrame(int i)  = 0          ;   //设置当前为第几帧
	virtual void    SetNumFrames(int iNumFrames) = 0  ;
	virtual void    SetStartFrame(int i ) = 0	      ;
	virtual void    SetEndFrame(int i)  = 0           ;
    virtual void    SetFrameDelay(int iFrameDelay) = 0;
	virtual void    SetOneCycle(bool bCycle) = 0      ;

	virtual void    SetNote(char * ch) = 0            ;   //设置注释内容
	virtual void    SetShowNote(bool b) = 0           ;	  //设置是否显示注释	  
	virtual bool    IsHidden() = 0                    ;
}; 

//=============================================================================
//CSpriteOne:多帧图片全在一张纹理中
class CSpriteOne:public CSprite	    
{
protected:
	CTexture *      m_Texture; 
	short           m_iWNumFrames;
	short           m_iHNumFrames;
protected:
	virtual void    UpdateFrame();
	virtual void    CalcCollisionRect();	 //计算碰撞矩形
	virtual void    CalcNoteRect();	         //计算注释显示矩形
	virtual void    ShutDown();
public:
	CSpriteOne(LPDIRECT3DDEVICE9 pD3DDevice, CTexture * texture, RECT& rcBounds, char * name = "默认", 
		int totalFrame = 1 , int wFrame = 1,int hFrame = 1, int FrameDelay = 0, BOUNDSACTION baBoundsAction = BA_STOP) ;
	virtual ~CSpriteOne()                              ;
    virtual SPRITEACTION   Update()                    ;
	virtual CSprite*       AddSprite()  { return NULL;};

	virtual void    Draw(LPDIRECT3DDEVICE9 pD3DDevice)  ;
	virtual BOOL    IsPointInside(int x, int y)         ;
	virtual BOOL    TestCollision(CSprite * pTestSprite);

	virtual void	GetName(char * in)     ;
	virtual int     GetWidth() { return m_Texture->GetWidth() / m_iWNumFrames;}             ;
	virtual int     GetHeight(){ return m_Texture->GetHeight() / m_iHNumFrames;}            ;
	virtual int     GetStartFrame() {return m_iStartFrame;}        ;
	virtual int     GetEndFrame() {return m_iEndFrame;}          ;
	virtual RECT&   GetCollision() { return m_rcCollision;}         ;
	virtual RECT&   GetPosition(){ return m_rcPosition;}           ;
	virtual POINT   GetVelocity(){ return m_ptVelocity ;}           ;
	virtual int     GetZOrder(){return m_iZOrder;}             ;       ;
	virtual BOOL    GetIsCycle(){return m_bCycle;}            ; //返回当前sprite是否自动更新


	virtual void    SetAlpha(int alpha) { m_alpha = alpha; m_Texture->SetAlpha(m_alpha);}         ;
	virtual void    SetPosition(int x, int y)      ;
	virtual void    SetPosition(POINT ptPosition)  ;
	virtual void    SetVelocity(int x, int y)      ;
	virtual void    SetXVelocity(int x) {m_ptVelocity.x = x;}            ;
	virtual void    SetYVelocity(int y) {m_ptVelocity.y = y;}           ;
    virtual void    SetVelocity(POINT ptVelocity)  ;
	virtual void    SetZOrder(int iZOrder) {m_iZOrder = iZOrder ;}         ;
	virtual void    SetBounds(RECT& rcBounds){CopyRect(&m_rcBounds, &rcBounds);}      ;
	virtual void    SetBoundsAction(BOUNDSACTION ba){m_baBoundsAction = ba;};
	virtual void    SetHidden(bool bHidden){m_bHidden = bHidden; }        ;
	virtual void    SetCurrFrame(int i)            ;   //设置当前为第几帧
	virtual void    SetNumFrames(int iNumFrames)   ;
	virtual void    SetStartFrame(int i ){m_iStartFrame = m_iCurFrame = i;} 	       ;
	virtual void    SetEndFrame(int i)             ;
    virtual void    SetFrameDelay(int iFrameDelay){m_iFrameDelay = iFrameDelay;} ;
	virtual void    SetOneCycle(bool bCycle){m_bOneCycle = bCycle ;}       ;

	virtual void    SetNote(char * ch)             ;   //设置注释内容
	virtual void    SetShowNote(bool b){m_bShowNote = b;}            ;   //设置是否显示注释	  
	virtual bool    IsHidden(){return m_bHidden;}                     ;
};

//=============================================================================
//CSpriteMul:每一帧都是独立的一张图片，子动画由多张图片构成
class CSpriteMul:public CSprite	    
{
protected:
	CTextureEx **     m_ppTexture;              //指向多幅图片的数组
protected:
	virtual void    UpdateFrame();
	virtual void    CalcCollisionRect();	 //计算碰撞矩形
	virtual void    CalcNoteRect();	         //计算注释显示矩形
	virtual void    ShutDown();
public:
	CSpriteMul(LPDIRECT3DDEVICE9 pD3DDevice, CTextureEx ** texture, RECT& rcBounds, char * name = "默认", 
		int totalFrame = 1 ,int FrameDelay = 0, BOUNDSACTION baBoundsAction = BA_STOP) ;
	virtual ~CSpriteMul()                              ;
    virtual SPRITEACTION   Update()                   ;
	virtual CSprite*       AddSprite()  { return NULL;};

	virtual void    Draw(LPDIRECT3DDEVICE9 pD3DDevice)  ;
	virtual BOOL    IsPointInside(int x, int y) {return false;}        ;
	virtual BOOL    TestCollision(CSprite * pTestSprite) {return false;};

	virtual void	GetName(char * in)   {}  ;
	virtual int     GetWidth() { return 0;}             ;
	virtual int     GetHeight(){ return 0;}            ;
	virtual int     GetStartFrame() {return m_iStartFrame;}        ;
	virtual int     GetEndFrame() {return m_iEndFrame;}          ;
	virtual RECT&   GetCollision() { return m_rcCollision;}         ;
	virtual RECT&   GetPosition(){ return m_rcPosition;}           ;
	virtual POINT   GetVelocity(){ return m_ptVelocity ;}           ;
	virtual int     GetZOrder(){return m_iZOrder;}             ;       ;
	virtual BOOL    GetIsCycle(){return m_bCycle;}            ; //返回当前sprite是否自动更新


	virtual void    SetAlpha(int alpha) { m_alpha = alpha;}         ;
	virtual void    SetPosition(int x, int y)      ;
	virtual void    SetPosition(POINT ptPosition){}  ;
	virtual void    SetVelocity(int x, int y)      {};
	virtual void    SetXVelocity(int x) {m_ptVelocity.x = x;}            ;
	virtual void    SetYVelocity(int y) {m_ptVelocity.y = y;}           ;
	virtual void    SetVelocity(POINT ptVelocity) {} ;
	virtual void    SetZOrder(int iZOrder) {m_iZOrder = iZOrder ;}         ;
	virtual void    SetBounds(RECT& rcBounds){CopyRect(&m_rcBounds, &rcBounds);}      ;
	virtual void    SetBoundsAction(BOUNDSACTION ba){m_baBoundsAction = ba;};
	virtual void    SetHidden(bool bHidden){m_bHidden = bHidden; }        ;
	virtual void    SetCurrFrame(int i)  {}          ;   //设置当前为第几帧
	virtual void    SetNumFrames(int iNumFrames) {}  ;
	virtual void    SetStartFrame(int i ){m_iStartFrame = m_iCurFrame = i;} 	       ;
	virtual void    SetEndFrame(int i)    {}         ;
    virtual void    SetFrameDelay(int iFrameDelay){m_iFrameDelay = iFrameDelay;} ;
	virtual void    SetOneCycle(bool bCycle){m_bOneCycle = bCycle ;}       ;

	virtual void    SetNote(char * ch)       {}      ;   //设置注释内容
	virtual void    SetShowNote(bool b){m_bShowNote = b;}            ;   //设置是否显示注释	  
	virtual bool    IsHidden(){return m_bHidden;}                     ;
};


#endif