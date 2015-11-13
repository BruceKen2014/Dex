
/*************************************************************
****DemoEx引擎实现渐进、渐出等效CFade.h
****@作者：阚正杰(BruceKen)
****@日期:2012-11-11(光棍节在码代码的伤不起T.T) 
*************************************************************/
#ifndef _CFADE_H
#define _CFADE_H

#include "../DexBase/typedefine.h"



#define D3DFVF_XYZ_DIFFUSE (D3DFVF_XYZ | D3DFVF_DIFFUSE)
class CFade
{
protected:
	struct stVertex
	{
		D3DXVECTOR3 position;
		DexColor color;
	};
	stVertex m_vertex[4];	       //顶点链表

	LPDIRECT3DDEVICE9 m_device ;    //设备指针
	LPDIRECT3DVERTEXBUFFER9 m_pVB;	//顶点缓存

	float       m_r, m_g, m_b;          //淡入淡出的颜色

	float       m_alpha;              //当前alpha
	
	bool      m_fadeIn;             //是否淡入
	bool      m_fadeOut;            //是否淡出

	int       m_width, m_height;    //区域宽、高

	bool Check();

public:
	CFade();
	virtual ~CFade();
	virtual void ShutDown();

public:
	virtual void Initialize(LPDIRECT3DDEVICE9 device);

	virtual void SetFadeColor(int r, int g, int b);
	virtual void SetFadeIn(bool fade_in = true);
	virtual void SetFadeOut(bool fade_out = true);

	virtual void SetRange(int width, int height);	  //设置区域宽、高

    virtual bool FadeComplete();    //淡入淡出是否已经完成,完成后做什么（可以选择相反淡入淡出，也可以保持原状，不改变alpha）

	virtual void PrepareFade();	    //为淡入淡出作预处理
	virtual void Update();          //更新alpha
	virtual void Render();
};
#endif

