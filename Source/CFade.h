
/*************************************************************
****DemoEx����ʵ�ֽ�����������ЧCFade.h
****@���ߣ�������(BruceKen)
****@����:2012-11-11(��������������˲���T.T) 
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
	stVertex m_vertex[4];	       //��������

	LPDIRECT3DDEVICE9 m_device ;    //�豸ָ��
	LPDIRECT3DVERTEXBUFFER9 m_pVB;	//���㻺��

	float       m_r, m_g, m_b;          //���뵭������ɫ

	float       m_alpha;              //��ǰalpha
	
	bool      m_fadeIn;             //�Ƿ���
	bool      m_fadeOut;            //�Ƿ񵭳�

	int       m_width, m_height;    //�������

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

	virtual void SetRange(int width, int height);	  //�����������

    virtual bool FadeComplete();    //���뵭���Ƿ��Ѿ����,��ɺ���ʲô������ѡ���෴���뵭����Ҳ���Ա���ԭ״�����ı�alpha��

	virtual void PrepareFade();	    //Ϊ���뵭����Ԥ����
	virtual void Update();          //����alpha
	virtual void Render();
};
#endif

