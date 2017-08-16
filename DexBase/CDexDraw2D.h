


#ifndef CDEX_DRAW_2D_H
#define CDEX_DRAW_2D_H

#include "typedefine.h"
#include "../DexBase/DexObject.h"
#include "../widget/DexGuiStruct.h"
#include <map>
#include <string>
#include "../DexMath/DexVector2.h"
//ͳһ�ô˶������2D����Ļ���
//����sprite���л���
class CDexDraw2D
{
private:
	//�������
	LPD3DXSPRITE       m_pSprite ;
	D3DXMATRIX         m_mat;
	int                m_alpha;
	RECT               m_rect;	
	
	//�������
	LPD3DXLINE         m_pLine;
	float              m_LineWidth; //�߿�
	int                m_LineColor; //��ɫ
	bool               m_LineAntialias;  //�Ƿ������������
	D3DXVECTOR2        m_LinePoint[2];  //����һ��ʼ���������ڴ棬�ڼ�ֻҪ�ı��ڴ��ֵ�Ϳ�����
private:
	CDexDraw2D();
	~CDexDraw2D()
	{
		if(m_pSprite != NULL)
		{	
			m_pSprite->Release();
			m_pSprite = NULL;
		}
		if(m_pLine != NULL)
		{	
			m_pLine->Release();
			m_pLine = NULL;
		}
	}

public:
	static CDexDraw2D& get2DDrawer()
	{
		static CDexDraw2D Drawer;
		return Drawer;
	}


	//---------------����Ϊ����2Dtexture����װ����Ҫֱ�ӻ���������ֱ��ʹ��CTextureEx-----------------
	void BeginDraw2D();
	void EndDraw2D();
	//�������������� ������ ��ת���� ��ת�Ƕ�(����)
	void SetParam(D3DXVECTOR2& scale_center, D3DXVECTOR2& scale, D3DXVECTOR2& rotate_center,float rotation, D3DXVECTOR2& pos);
	//����Դ�����������
	void SetRect(RECT& rect);
	void SetAlpha(int alpha) { m_alpha = alpha;};	
	//�ڵ���Draw���л���֮ǰ �������SetParam SetRect SetAlpha ���в�����Ԥ����
	void Draw(LPDIRECT3DTEXTURE9 tex);


	//--------------------����Ϊֱ�߻�����غ���--------------------------------------------
	void SetLineColor(int LineColor); //����������ɫ ��0xffffffff(��D3DCOLOR_ARGB(255, 255,255,255))
	void SetLineWidth(float LineWidth); //�����߿�
	void EnableAntialias(bool enable);  //�Ƿ��������Ч��
	void SetPattern();
	int   GetLineColor() { return m_LineColor;};
	float GetLineWidth() { return m_LineWidth;};
	
	void DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2);
	void DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2, int LineColor);
	void DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2, int LineColor, int width);
};


//����2DԪ�� ��������ͶӰʵ��
//�����ռ��о���һ����߶�Ϊ1�������Σ��е���ԭ��


using namespace DexGUI;
using namespace std;
class CModel;
class CMyFont;
class CDexDraw2DOrth:public DexObject
{
	Dex_DeclareClass(CDexDraw2DOrth, DexObject,0)
public:
	static const stEvent g_moveCamera;//();
protected:
	D3DXMATRIX m_matrix;
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuff;	 //��ʼDrawPrimitive�����ã��ֲ���DrawPrimitiveUp֮�������ô�
	stVertex1* m_point;

	int       m_lineWidth;//�߿�
	stVertex1 m_line[2];	//�߶ζ���

	D3DMATERIAL9 m_material;//Բ��Ĳ��ʣ����Ըı�Բ�����ɫ
	LPD3DXMESH m_mesh;  //��Բ����Ⱦ��2D�������ƽ��Բ��

	D3DXMATRIX m_project;   //ͶӰ����

	D3DXVECTOR3 m_2dCameraPos;
	D3DXMATRIX m_view;		//��ͼ����

	D3DXMATRIX m_3Dproject;     //����3DͶӰ����  
	D3DXMATRIX m_3Dview;		//����3D��ͼ����

	D3DXMATRIX m_scale;	   //���ž���
	float      m_xScale;   //�洢X����ϵ������ƽ��ת��ʱҪ�õ�
	float      m_yScale;   //
	D3DXMATRIX m_trans;	   //ƽ�ƾ���
	D3DXMATRIX m_rotateZ;  //��ת���� ����ֻΧ��Z����ת

	std::map<int, CMyFont*> m_fontMap; //��ͬ�����С��Ӧ��ͬ��ָ��
	int        m_iCurrTextSize;

public:		
	enum
	{
		DRAW_CIRCLE_CENTER,  //��Բʱ ��Բ��ΪPos
		DRAW_CIRCLE_LEFTTOP	 //��Բʱ ��Բ�����о������϶���Ϊpos
	};

public:
	CDexDraw2DOrth();
	~CDexDraw2DOrth();
	//��Ⱦ��������Ⱦ���β������ֲ�ͬ������
	//��Ⱦ����ֱ����ƽ��ԭ����Χ��4����
	//��Ⱦ�߶������߶ε�б�ʸ���ͬ������ֿ����ø����������
public:
	static CDexDraw2DOrth& get2DDrawer()
	{
		static CDexDraw2DOrth Drawer;
		return Drawer;
	}
	void BeginDraw2D();	  //��Ҫ�ڴ˱���һЩ3D��Ⱦ������Ȼ����EndDraw2D�и�λ
	void EndDraw2D() ;
	void ResetMatrix();
	void SetUV(D3DXVECTOR2 p0,D3DXVECTOR2 p1,D3DXVECTOR2 p2,D3DXVECTOR2 p3);
	void SetUV(const DexVector2& p0, const DexVector2& p1, const DexVector2& p2, const DexVector2& p3);
	void SetScale(float xScale, float yScale);    //���� x y �����ţ����ﴫ�����ʵ������Ⱦ����Ļ�������ߣ�
	void SetTrans(int xPos, int yPos);   //������Ļ���꣬�������ڽ���ƽ��ת��(�紫����Ļ�ϵ�20 20)
	void SetRotate(float rotate);		    	  //������ת����
	void SetAlpha(int alpha);  //�˴�alphaΪARGB��ʽ
	void Draw(LPDIRECT3DTEXTURE9 tex);

	//camera
	D3DXVECTOR3 GetCameraPos() { return m_2dCameraPos;};
	void SetCameraPos(float x, float y);

	//line
	void SetLineWidth(int lineWidth);
	void DrawLine(const D3DXVECTOR2& point1, const D3DXVECTOR2& point2, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	void DrawLine(const DexPoint& point1, const DexPoint& point2, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	void DrawLine(const DexPointF& point1, const DexPointF& point2, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	void DrawRectLine(const DexRect& rect,const DexColor& color1 = 0xffffffff, int lineWidth = 1); //���ƾ��ο�
	void DrawRectLine(const DexRectF& rect,const DexColor& color1 = 0xffffffff, int lineWidth = 1); //���ƾ��ο�

	//Rect ��������4������
	void SetRectColor(const DexColor& color); //����4������ɫΪͳһ����ɫ	
	void SetRectColor(const DexColor& c0, const DexColor& c1, const DexColor& c2, const DexColor& c3); //�ֱ�����4�������ɫ
	void SetRectAlpha(float alpha); //alpha͸���ȣ��������ö�����ɫ֮��ֻ��ı䶥��͸���ȵ����  
	void SetRectAlpha(float alpha0, float alpha1, float alpha2, float alpha3);	//�ֱ�����͸����
			//����������
	void DrawRect(const DexRect& rect, float rotate = 0.0f);
	void DrawRect(const DexRectF& rect, float rotate = 0.0f);
	void DrawRect(D3DXVECTOR2 pos, int width, int height, float rotate = 0.0f);

	//circle �ж������Ƿ�����Բ��UI�� ȡ�����Բ�ľ���Ȼ��Ͱ뾶�Ƚϱ�����
	void SetCircleColor(int a, int r, int g, int b);
	void SetCircleAlpha(int alpha);
	void DrawCircle(const DexPointF& pos, float radius, int flag = DRAW_CIRCLE_CENTER);
	void DrawCircle(const DexPoint& pos, float radius, int flag = DRAW_CIRCLE_CENTER);
	void DrawCircle(const D3DXVECTOR2& pos, float radius, int flag = DRAW_CIRCLE_CENTER);

	//triangle
		//���ǿ�
	void DrawTriangleLine(DexPoint& point1, DexPoint& point2, DexPoint& point3, const DexColor& color1 = 0xffffffff);

	//��ģ����Ⱦ����Ļ��   ����rotate��ʱֻ�ṩΧ������Y����ת�Ĺ���
	void RenderModel(CModel* model,float xPos, float yPos, float xScale, float yScale, float zScale, float rotate = 0.0f);
	void RenderModel(CModel* model,D3DXVECTOR2 pos, float xScale, float yScale, float zScale, float rotate = 0.0f);
	

	//string
	void SetTextSize(int textsize); //5~50 ��5����������
	void SetTextColor(const DexColor& color);
	void DrawString(int xPos, int yPos, char* str, ...);
	void DrawString(DexRectF& rect, char* str, ...);

};
CDexDraw2DOrth* get2DDrawer();
/*�÷�ʾ��
get2DDrawer()->BeginDraw2D();
get2DDrawer()->SetRectColor(0xffffff00);
get2DDrawer()->SetRectAlpha(alp);
get2DDrawer()->DrawRect(D3DXVECTOR2(500,0), 150, 150);



get2DDrawer()->SetCircleColor(255, 0, 255 ,255);
get2DDrawer()->SetCircleAlpha(100);
get2DDrawer()->DrawCircle(D3DXVECTOR2(300,150), 50, CDexDraw2DOrth::DRAW_CIRCLE_LEFTTOP);
get2DDrawer()->SetCircleColor(255, 0, 255 ,0);
get2DDrawer()->DrawCircle(D3DXVECTOR2(300,150), 50, CDexDraw2DOrth::DRAW_CIRCLE_CENTER);



get2DDrawer()->ResetMatrix();	
get2DDrawer()->SetAlpha(DexColor(255, 255, 255, alp));
get2DDrawer()->SetScale(300, 300);
get2DDrawer()->SetRotate(float(alp)/255);

get2DDrawer()->SetTrans(200,200);
get2DDrawer()->Draw(g_Ex->GetTexPoint());

get2DDrawer()->RenderModel(g_Jintian, 600, 400, 3, 3,3, scale2);
get2DDrawer()->EndDraw2D();
*/
#endif