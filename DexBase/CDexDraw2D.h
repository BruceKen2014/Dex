


#ifndef CDEX_DRAW_2D_H
#define CDEX_DRAW_2D_H

#include "typedefine.h"
#include "../DexBase/DexObject.h"
#include "../widget/DexGuiStruct.h"
#include <map>
#include <string>
#include "../DexMath/DexVector2.h"
//统一用此对象进行2D纹理的绘制
//采用sprite进行绘制
class CDexDraw2D
{
private:
	//纹理相关
	LPD3DXSPRITE       m_pSprite ;
	D3DXMATRIX         m_mat;
	int                m_alpha;
	RECT               m_rect;	
	
	//线条相关
	LPD3DXLINE         m_pLine;
	float              m_LineWidth; //线宽
	int                m_LineColor; //颜色
	bool               m_LineAntialias;  //是否开启线条抗锯齿
	D3DXVECTOR2        m_LinePoint[2];  //程序一开始申请两点内存，期间只要改变内存的值就可以了
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


	//---------------以下为辅助2Dtexture而封装，若要直接绘制纹理，请直接使用CTextureEx-----------------
	void BeginDraw2D();
	void EndDraw2D();
	//参数：缩放中心 缩放量 旋转中心 旋转角度(弧度)
	void SetParam(D3DXVECTOR2& scale_center, D3DXVECTOR2& scale, D3DXVECTOR2& rotate_center,float rotation, D3DXVECTOR2& pos);
	//设置源纹理矩形区域
	void SetRect(RECT& rect);
	void SetAlpha(int alpha) { m_alpha = alpha;};	
	//在调用Draw进行绘制之前 必须调用SetParam SetRect SetAlpha 进行参数的预设置
	void Draw(LPDIRECT3DTEXTURE9 tex);


	//--------------------以下为直线绘制相关函数--------------------------------------------
	void SetLineColor(int LineColor); //设置线条颜色 如0xffffffff(即D3DCOLOR_ARGB(255, 255,255,255))
	void SetLineWidth(float LineWidth); //设置线宽
	void EnableAntialias(bool enable);  //是否开启抗锯齿效果
	void SetPattern();
	int   GetLineColor() { return m_LineColor;};
	float GetLineWidth() { return m_LineWidth;};
	
	void DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2);
	void DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2, int LineColor);
	void DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2, int LineColor, int width);
};


//绘制2D元素 采用正交投影实现
//正交空间中就是一个宽高都为1的正方形，中点在原点


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
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuff;	 //开始DrawPrimitive起作用，现采用DrawPrimitiveUp之后已无用处
	stVertex1* m_point;

	int       m_lineWidth;//线宽
	stVertex1 m_line[2];	//线段顶点

	D3DMATERIAL9 m_material;//圆球的材质，用以改变圆球的颜色
	LPD3DXMESH m_mesh;  //将圆球渲染到2D上面便是平面圆了

	D3DXMATRIX m_project;   //投影矩阵

	D3DXVECTOR3 m_2dCameraPos;
	D3DXMATRIX m_view;		//视图矩阵

	D3DXMATRIX m_3Dproject;     //保存3D投影矩阵  
	D3DXMATRIX m_3Dview;		//保存3D视图矩阵

	D3DXMATRIX m_scale;	   //缩放矩阵
	float      m_xScale;   //存储X缩放系数，在平移转换时要用到
	float      m_yScale;   //
	D3DXMATRIX m_trans;	   //平移矩阵
	D3DXMATRIX m_rotateZ;  //旋转矩阵 这里只围绕Z轴旋转

	std::map<int, CMyFont*> m_fontMap; //不同字体大小对应不同的指针
	int        m_iCurrTextSize;

public:		
	enum
	{
		DRAW_CIRCLE_CENTER,  //画圆时 以圆心为Pos
		DRAW_CIRCLE_LEFTTOP	 //画圆时 以圆的外切矩形左上顶点为pos
	};

public:
	CDexDraw2DOrth();
	~CDexDraw2DOrth();
	//渲染纹理与渲染矩形采用两种不同的坐标
	//渲染纹理直接在平移原点周围的4个点
	//渲染线段由于线段的斜率各不同，必须分开设置各个点的坐标
public:
	static CDexDraw2DOrth& get2DDrawer()
	{
		static CDexDraw2DOrth Drawer;
		return Drawer;
	}
	void BeginDraw2D();	  //主要在此保存一些3D渲染参数，然后在EndDraw2D中复位
	void EndDraw2D() ;
	void ResetMatrix();
	void SetUV(D3DXVECTOR2 p0,D3DXVECTOR2 p1,D3DXVECTOR2 p2,D3DXVECTOR2 p3);
	void SetUV(const DexVector2& p0, const DexVector2& p1, const DexVector2& p2, const DexVector2& p3);
	void SetScale(float xScale, float yScale);    //设置 x y 的缩放（这里传入的其实就是渲染到屏幕的纹理宽高）
	void SetTrans(int xPos, int yPos);   //传入屏幕坐标，函数体内进行平移转换(如传入屏幕上的20 20)
	void SetRotate(float rotate);		    	  //设置旋转弧度
	void SetAlpha(int alpha);  //此处alpha为ARGB形式
	void Draw(LPDIRECT3DTEXTURE9 tex);

	//camera
	D3DXVECTOR3 GetCameraPos() { return m_2dCameraPos;};
	void SetCameraPos(float x, float y);

	//line
	void SetLineWidth(int lineWidth);
	void DrawLine(const D3DXVECTOR2& point1, const D3DXVECTOR2& point2, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	void DrawLine(const DexPoint& point1, const DexPoint& point2, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	void DrawLine(const DexPointF& point1, const DexPointF& point2, const DexColor& color1 = 0xffffffff, const DexColor& color2 = 0xffffffff);
	void DrawRectLine(const DexRect& rect,const DexColor& color1 = 0xffffffff, int lineWidth = 1); //绘制矩形框
	void DrawRectLine(const DexRectF& rect,const DexColor& color1 = 0xffffffff, int lineWidth = 1); //绘制矩形框

	//Rect 与纹理公用4个顶点
	void SetRectColor(const DexColor& color); //设置4个点颜色为统一的颜色	
	void SetRectColor(const DexColor& c0, const DexColor& c1, const DexColor& c2, const DexColor& c3); //分别设置4个点的颜色
	void SetRectAlpha(float alpha); //alpha透明度，用在设置顶点颜色之后，只想改变顶点透明度的情况  
	void SetRectAlpha(float alpha0, float alpha1, float alpha2, float alpha3);	//分别设置透明度
			//绘制填充矩形
	void DrawRect(const DexRect& rect, float rotate = 0.0f);
	void DrawRect(const DexRectF& rect, float rotate = 0.0f);
	void DrawRect(D3DXVECTOR2 pos, int width, int height, float rotate = 0.0f);

	//circle 判断鼠标点是否落在圆形UI内 取鼠标点和圆心距离然后和半径比较便是了
	void SetCircleColor(int a, int r, int g, int b);
	void SetCircleAlpha(int alpha);
	void DrawCircle(const DexPointF& pos, float radius, int flag = DRAW_CIRCLE_CENTER);
	void DrawCircle(const DexPoint& pos, float radius, int flag = DRAW_CIRCLE_CENTER);
	void DrawCircle(const D3DXVECTOR2& pos, float radius, int flag = DRAW_CIRCLE_CENTER);

	//triangle
		//三角框
	void DrawTriangleLine(DexPoint& point1, DexPoint& point2, DexPoint& point3, const DexColor& color1 = 0xffffffff);

	//将模型渲染到屏幕上   这里rotate暂时只提供围绕自身Y轴旋转的功能
	void RenderModel(CModel* model,float xPos, float yPos, float xScale, float yScale, float zScale, float rotate = 0.0f);
	void RenderModel(CModel* model,D3DXVECTOR2 pos, float xScale, float yScale, float zScale, float rotate = 0.0f);
	

	//string
	void SetTextSize(int textsize); //5~50 被5整除的字体
	void SetTextColor(const DexColor& color);
	void DrawString(int xPos, int yPos, char* str, ...);
	void DrawString(DexRectF& rect, char* str, ...);

};
CDexDraw2DOrth* get2DDrawer();
/*用法示例
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