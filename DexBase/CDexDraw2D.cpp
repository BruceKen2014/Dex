

#include "CDexDraw2D.h"

#include "../Source/CLight.h"
#include "../Source/CModelXAni.h"
#include "../Source/CMyFont.h"
#include "../state/DexGameEngine.h"

#define LINE_COLOR 0xff000000
#define LINE_WIDTH 1.5

CDexDraw2D::CDexDraw2D()
{
	m_pSprite = NULL;
	m_pLine = NULL;
	if(FAILED(D3DXCreateSprite(DexGameEngine::getEngine()->GetDevice(), &m_pSprite)))
	{
		MessageBox(NULL, "创建sprite失败！", "提示", MB_OK);
	}
	if(FAILED(D3DXCreateLine(DexGameEngine::getEngine()->GetDevice(), &m_pLine)))
	{
		MessageBox(NULL, "创建sprite失败！", "提示", MB_OK);
	}
	m_alpha = 255;
	m_LinePoint[0] = D3DXVECTOR2(0,0);
	m_LinePoint[1] = D3DXVECTOR2(0,0);
	m_LineWidth  = LINE_WIDTH;
	m_LineColor = LINE_COLOR;
	m_LineAntialias = true;
}

void CDexDraw2D::BeginDraw2D()
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}
void CDexDraw2D::EndDraw2D()
{
	m_pSprite->End();
}
void CDexDraw2D::SetParam(D3DXVECTOR2& scale_center, D3DXVECTOR2& scale, D3DXVECTOR2& rotate_center,float rotation, D3DXVECTOR2& pos)
{
	D3DXMatrixTransformation2D(&m_mat,&scale_center, 0.0f, &scale, &rotate_center, rotation,  &pos);
	m_pSprite->SetTransform(&m_mat);
}

void CDexDraw2D::SetRect(RECT& rect)
{
	m_rect = rect;
}
void CDexDraw2D::Draw(LPDIRECT3DTEXTURE9 tex)
{
	if(tex == NULL)
		return;
	m_pSprite->Draw(tex, &m_rect, NULL, NULL,m_alpha);
}

void CDexDraw2D::SetLineColor(int LineColor)
{
	m_LineColor = LineColor;
}

void CDexDraw2D::SetLineWidth(float LineWidth)
{
	m_LineWidth = LineWidth;
	m_pLine->SetWidth(m_LineWidth);
}

void CDexDraw2D::EnableAntialias(bool enable)
{
	m_LineAntialias = enable;
	m_pLine->SetAntialias(m_LineAntialias);
}

void CDexDraw2D::DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2)
{
	if(m_pLine == NULL)
		return;
	m_LinePoint[0] = point1;
	m_LinePoint[1] = point2;
	//m_pLine->SetPattern(1);
	m_pLine->Draw(m_LinePoint, 2, m_LineColor);
}

void CDexDraw2D::DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2, int LineColor)
{
	if(m_pLine == NULL)
		return;
	m_LinePoint[0] = point1;
	m_LinePoint[1] = point2;
	m_pLine->Draw(m_LinePoint, 2, LineColor);
}

void CDexDraw2D::DrawLine(D3DXVECTOR2& point1, D3DXVECTOR2& point2, int LineColor, int width)
{
	if(m_pLine == NULL)
		return;
	m_LinePoint[0] = point1;
	m_LinePoint[1] = point2;
	m_pLine->SetWidth(width);
	m_pLine->Draw(m_LinePoint, 2, LineColor);
}

const stEvent CDexDraw2DOrth::g_moveCamera = stEvent("moveCamera", 1);
CDexDraw2DOrth::CDexDraw2DOrth()
{
	D3DXMatrixIdentity(&m_matrix);
	D3DXMatrixIdentity(&m_view);

	m_2dCameraPos = D3DXVECTOR3(0,0,-1);

	D3DXMatrixIdentity(&m_scale);
	D3DXMatrixIdentity(&m_trans);
	D3DXMatrixIdentity(&m_rotateZ);
	DexGameEngine::getEngine()->GetDevice()->CreateVertexBuffer(4*sizeof(stVertex1), D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1,D3DPOOL_DEFAULT, &m_VertexBuff, NULL);
	DexGameEngine::getEngine()->GetDevice()->CreateVertexBuffer(2*sizeof(stVertex1), D3DUSAGE_WRITEONLY, FVF_XYZ_DIFF_TEX1,D3DPOOL_DEFAULT, &m_VertexBuff, NULL);
	
	memset(&m_material,0, sizeof(D3DMATERIAL9));
	m_material.Ambient = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
	m_material.Diffuse = D3DXCOLOR(0.0f,1.0f,0.0f,1.0f);
	m_material.Emissive = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_material.Specular = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
	//由于是2D的 我们只要一个坐标看上去很平滑就行了
	D3DXCreateSphere(DexGameEngine::getEngine()->GetDevice(), 1.0, 40, 2, &m_mesh, NULL);

	m_VertexBuff->Lock(0, 4*sizeof(stVertex1), (void**)&m_point, 0);
	m_point[0].m_color = 0xffffffff;
	m_point[1].m_color = 0xffffffff;
	m_point[2].m_color = 0xffffffff;
	m_point[3].m_color = 0xffffffff;

	m_point[0].m_pos.x = m_point[3].m_pos.x = -0.5;
	m_point[1].m_pos.x = m_point[2].m_pos.x =  0.5;

	m_point[0].m_pos.y = m_point[1].m_pos.y = 0.5;
	m_point[2].m_pos.y = m_point[3].m_pos.y = -0.5;

	m_point[0].m_pos.z = m_point[1].m_pos.z = m_point[2].m_pos.z = m_point[3].m_pos.z = 1.0f;

	m_line[0].m_pos = D3DXVECTOR3(0,0,1);
	m_line[1].m_pos = D3DXVECTOR3(0,0,1);
	m_lineWidth = 1;
	m_point[0].SetUV(0.0f, 0.0f);
	m_point[1].SetUV(1.0f, 0.0f);
	m_point[2].SetUV(1.0f, 1.0f);
	m_point[3].SetUV(0.0f, 1.0f);
	m_VertexBuff->Unlock();

	//5号到50号字体
	for(int i = 0; i < 10; i++)
	{
		m_fontMap[(i+1)*5] = new CMyFont(DexGameEngine::getEngine()->GetDevice());
		m_fontMap[(i+1)*5]->SetTextSize((i+1)*5);
	}
	m_iCurrTextSize = 10;
}
CDexDraw2DOrth::~CDexDraw2DOrth()
{
	_SafeRelease(m_VertexBuff);
	_SafeRelease(m_mesh);
	std::map<int, CMyFont*>::iterator it = m_fontMap.begin();
	while(it != m_fontMap.end())
	{
		_SafeDelete(it->second);
		it->second = NULL;
		it = m_fontMap.erase(it);
	}
}

void CDexDraw2DOrth::BeginDraw2D()
{
	D3DXMatrixOrthoLH(&m_project, GetClientWidth(), GetClientHeight(), 0.1f, 1000.0f);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);//要将灯光关闭

	DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_PROJECTION, &m_3Dproject);
	DexGameEngine::getEngine()->GetDevice()->GetTransform(D3DTS_VIEW, &m_3Dview);

	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_PROJECTION, &m_project);

	D3DXVECTOR3 focus = m_2dCameraPos;
	focus.z = 0;
	D3DXVECTOR3 up = D3DXVECTOR3(0,1,0);
	D3DXMatrixLookAtLH(&m_view, &m_2dCameraPos, &focus, &up);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_VIEW, &m_view);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); //开启顶点与纹理的alpha混合
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);	//关闭深度检测，如果不关闭的话，圆球最终的绘制只和位置有关，而不和渲染顺序有关
}
void CDexDraw2DOrth::EndDraw2D()
{
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_PROJECTION, &m_3Dproject);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_VIEW, &m_3Dview);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}
void CDexDraw2DOrth::ResetMatrix()
{
	D3DXMatrixIdentity(&m_matrix);
	m_point[0].SetUV(0.0f, 0.0f);
	m_point[1].SetUV(1.0f, 0.0f);
	m_point[2].SetUV(1.0f, 1.0f);
	m_point[3].SetUV(0.0f, 1.0f);
}
void CDexDraw2DOrth::SetUV(D3DXVECTOR2 p0,D3DXVECTOR2 p1,D3DXVECTOR2 p2,D3DXVECTOR2 p3)
{
	m_point[0].SetUV(p0.x, p0.y);
	m_point[1].SetUV(p1.x, p1.y);
	m_point[2].SetUV(p2.x, p2.y);
	m_point[3].SetUV(p3.x, p3.y);
}
void CDexDraw2DOrth::SetUV(const DexVector2& p0, const DexVector2& p1, const DexVector2& p2, const DexVector2& p3)
{
	m_point[0].SetUV(p0.x, p0.y);
	m_point[1].SetUV(p1.x, p1.y);
	m_point[2].SetUV(p2.x, p2.y);
	m_point[3].SetUV(p3.x, p3.y);
}
void CDexDraw2DOrth::SetScale(float xScale, float yScale)
{
	m_xScale = xScale;
	m_yScale = yScale;
	D3DXMatrixIdentity(&m_scale);
	D3DXMatrixScaling(&m_scale, xScale, yScale, 1.0f);
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &m_scale);
}
void CDexDraw2DOrth::SetRotate(float rotate)
{
	D3DXMatrixIdentity(&m_rotateZ);
	D3DXMatrixRotationZ(&m_rotateZ, rotate);
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &m_rotateZ);
}
void CDexDraw2DOrth::SetTrans(int xPos, int yPos)
{
	D3DXMatrixIdentity(&m_trans);
	
	//先平移到左上角
	float tranx1 = -(GetClientWidth()/2 - m_xScale/2) ;
	float trany1 = (GetClientHeight()/2 - m_yScale/2);

	tranx1 += xPos;
	trany1 -= yPos;

	D3DXMatrixTranslation(&m_trans, tranx1, trany1, 0);	
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &m_trans);
}
void CDexDraw2DOrth::SetAlpha(int alpha)
{
	for(int i = 0 ; i < 4; i++)
		m_point[i].m_color = alpha;
}

void CDexDraw2DOrth::Draw(LPDIRECT3DTEXTURE9 tex)
{
	if(tex == NULL)
		return;
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &m_matrix);	 
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	DexGameEngine::getEngine()->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	DexGameEngine::getEngine()->GetDevice()->SetTexture(0,tex);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	//m_device->SetStreamSource(0, m_VertexBuff, 0, sizeof(stVertex1));
	//m_device->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, 2);
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, m_point, sizeof(stVertex1));

	DexGameEngine::getEngine()->GetDevice()->SetTexture(0,NULL);

	ResetMatrix();
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &m_matrix);	 

	//绘制之后将alpha 颜色置为默认值
	m_point[0].m_color = 0xffffffff;
	m_point[1].m_color = 0xffffffff;
	m_point[2].m_color = 0xffffffff;
	m_point[3].m_color = 0xffffffff;
}

void CDexDraw2DOrth::SetLineWidth(int lineWidth)
{
	m_lineWidth = lineWidth;
}

void CDexDraw2DOrth::SetCameraPos(float x, float y)
{
	stEvent event = g_moveCamera;
	stArgs delta_x = x - m_2dCameraPos.x;
	stArgs delta_y = -(y - m_2dCameraPos.y); //3d Y方向与2D的Y方向是相反的
	event.push_args(delta_x);
	event.push_args(delta_y);
	m_2dCameraPos.x = x;
	m_2dCameraPos.y = y;
	NotifyEvent(event);
}

void CDexDraw2DOrth::DrawLine(const D3DXVECTOR2& point1, const D3DXVECTOR2& point2, const DexColor& color1, const DexColor& color2)
{
	
	m_line[0].m_pos.x = -GetClientWidth()/2;
	m_line[0].m_pos.x += point1.x;
	m_line[0].m_pos.y = GetClientHeight()/2;
	m_line[0].m_pos.y -= point1.y;
	m_line[0].m_color =getD3DColor(color1);

	m_line[1].m_pos.x = -GetClientWidth()/2;
	m_line[1].m_pos.x += point2.x;
	m_line[1].m_pos.y = GetClientHeight()/2;
	m_line[1].m_pos.y -= point2.y;
	m_line[1].m_color = getD3DColor(color2);

	//线宽多1的话也就是多绘制一条直线
	int i = 0;
	
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	DexGameEngine::getEngine()->GetDevice()->SetTexture(0,NULL); //这里要将第一纹理设置为空 否则不画图片直接画线条会出现 错误
	do 
	{
		DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_LINELIST, 1, m_line, sizeof(stVertex1));	
		if(point1.x == point2.x)
		{//垂直的直线
			m_line[0].m_pos.x += 1;
			m_line[1].m_pos.x += 1;
		}
		else if(point1.y == point2.y)
		{//水平的直线
			m_line[0].m_pos.y += 1;
			m_line[1].m_pos.y += 1;
		}
		else if(point1.y > point2.y)
		{//左低右高
			m_line[0].m_pos.x += 1;
			m_line[1].m_pos.x += 1;
			m_line[0].m_pos.y += 1;
			m_line[1].m_pos.y += 1;
		}
		else
		{//左高右低
			m_line[0].m_pos.x -= 1;
			m_line[1].m_pos.x -= 1;
			m_line[0].m_pos.y += 1;
			m_line[1].m_pos.y += 1;
		}
		i++;
	} while (i < m_lineWidth);
}

void CDexDraw2DOrth::DrawLine(const DexPoint& point1, const DexPoint& point2, const DexColor& color1 /* = 0xffffffff */, const DexColor& color2 /* = 0xffffffff */)
{
	D3DXVECTOR2 p1,p2;
	p1.x = point1.x;
	p1.y = point1.y;
	p2.x = point2.x;
	p2.y = point2.y;
	DrawLine(p1, p2, color1, color2);
}

void CDexDraw2DOrth::DrawLine(const DexPointF& point1, const DexPointF& point2, const DexColor& color1 /* = 0xffffffff */, const DexColor& color2 /* = 0xffffffff */)
{
	D3DXVECTOR2 p1,p2;
	p1.x = point1.x;
	p1.y = point1.y;
	p2.x = point2.x;
	p2.y = point2.y;
	DrawLine(p1, p2, color1, color2);
}

void CDexDraw2DOrth::DrawRectLine(const DexRect& rect,const DexColor& color1, int lineWidth)
{
	SetLineWidth(lineWidth);
	DrawLine(DexPoint(rect.left, rect.top), DexPoint(rect.right, rect.top), color1, color1);
	DrawLine(DexPoint(rect.right, rect.top), DexPoint(rect.right, rect.bottom), color1, color1);
	DrawLine(DexPoint(rect.left, rect.bottom), DexPoint(rect.right, rect.bottom), color1, color1);
	DrawLine(DexPoint(rect.left, rect.top), DexPoint(rect.left, rect.bottom), color1, color1);
}
void CDexDraw2DOrth::DrawRectLine(const DexRectF& rect,const DexColor& color1, int lineWidth)
{
	SetLineWidth(lineWidth);
	DrawLine(DexPoint(rect.left, rect.top), DexPoint(rect.right, rect.top), color1, color1);
	DrawLine(DexPoint(rect.right, rect.top), DexPoint(rect.right, rect.bottom), color1, color1);
	DrawLine(DexPoint(rect.left, rect.bottom), DexPoint(rect.right, rect.bottom), color1, color1);
	DrawLine(DexPoint(rect.left, rect.top), DexPoint(rect.left, rect.bottom), color1, color1);
}

void CDexDraw2DOrth::SetRectColor(const DexColor& color)
{
	for(int i = 0 ; i< 4; i++)
		m_point[i].m_color = getD3DColor(color);
}
void CDexDraw2DOrth::SetRectAlpha(float alpha)
{
	for(int i = 0 ; i< 4; i++)
	{
		m_point[i].m_color &= 0x00ffffff;  
		m_point[i].m_color |= (int(alpha*255)) << 24;
	}
}
void CDexDraw2DOrth::SetRectAlpha(float alpha0, float alpha1, float alpha2, float alpha3)
{
	m_point[0].m_color &= 0x00ffffff;
	m_point[0].m_color |=  (int(alpha0*255)) << 24;
	m_point[1].m_color &= 0xff00ffff;
	m_point[1].m_color |=  (int(alpha1*255)) << 16;
	m_point[2].m_color &= 0xffff00ff;
	m_point[2].m_color |=  (int(alpha2*255)) << 8;
	m_point[3].m_color &= 0xffffff00;
	m_point[3].m_color |=  (int(alpha3*255)) << 0;
}
void CDexDraw2DOrth::SetRectColor(const DexColor& c0, const DexColor& c1, const DexColor& c2, const DexColor& c3)
{
	m_point[0].m_color = getD3DColor(c0);
	m_point[1].m_color = getD3DColor(c1);
	m_point[2].m_color = getD3DColor(c2);
	m_point[3].m_color = getD3DColor(c3);
}
void CDexDraw2DOrth::DrawRect(D3DXVECTOR2 pos, int width, int height, float rotate)
{
	ResetMatrix();
	SetScale(width, height);
	SetRotate(rotate);
	SetTrans(pos.x, pos.y);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &m_matrix);
	DexGameEngine::getEngine()->GetDevice()->SetFVF(FVF_XYZ_DIFF_TEX1);
	DexGameEngine::getEngine()->GetDevice()->SetTexture(0, NULL);
	DexGameEngine::getEngine()->GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, m_point, sizeof(stVertex1));

	//绘制之后将alpha 颜色置为默认值
	m_point[0].m_color = 0xffffffff;
	m_point[1].m_color = 0xffffffff;
	m_point[2].m_color = 0xffffffff;
	m_point[3].m_color = 0xffffffff;

	ResetMatrix();
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &m_matrix);	 
}

void CDexDraw2DOrth::DrawRect(const DexRect& rect, float rotate)
{
	DrawRect(D3DXVECTOR2(rect.left, rect.top), rect.right - rect.left, rect.bottom - rect.top, rotate);
}
void CDexDraw2DOrth::DrawRect(const DexRectF& rect, float rotate)
{
	DrawRect(D3DXVECTOR2(rect.left, rect.top), rect.right - rect.left, rect.bottom - rect.top, rotate);
}
void CDexDraw2DOrth::SetCircleColor(int a, int r, int g, int b)
{
	m_material.Ambient = D3DXCOLOR(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
	m_material.Diffuse = D3DXCOLOR(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
	m_material.Emissive = D3DXCOLOR(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
	m_material.Specular = D3DXCOLOR(r/255.0f,g/255.0f,b/255.0f,a/255.0f);
}
void CDexDraw2DOrth::SetCircleAlpha(int alpha)
{
	m_material.Ambient.a = alpha/255.0f;
	m_material.Diffuse.a = alpha/255.0f;
}
void CDexDraw2DOrth::DrawCircle(const DexPoint& pos, float radius, int flag)
{
	D3DXVECTOR2 pnt;
	pnt.x = pos.x;
	pnt.y = pos.y;
	DrawCircle(pnt, radius, flag);
}
void CDexDraw2DOrth::DrawCircle(const DexPointF& pos, float radius, int flag)
{
	D3DXVECTOR2 pnt;
	pnt.x = pos.x;
	pnt.y = pos.y;
	DrawCircle(pnt, radius, flag);
}
void CDexDraw2DOrth::DrawCircle(const D3DXVECTOR2& pos, float radius, int flag)
{
	if(m_mesh == NULL)
		return;
	ResetMatrix();
	SetScale(radius, radius); 
	SetRotate(0.0f);
	D3DXMatrixIdentity(&m_trans); 
	float tranx1 = -GetClientWidth()/2 ;
	float trany1 = GetClientHeight()/2;
	if(flag == DRAW_CIRCLE_CENTER)
	{
		tranx1 += pos.x;
		trany1 -= pos.y;
	}	
	else
	{
		tranx1 += radius;
		trany1 -= radius;;
		tranx1 += pos.x;
		trany1 -= pos.y;
	}
	D3DXMatrixTranslation(&m_trans, tranx1, trany1, radius+1);//为了防止圆球穿过摄像机 要向Z轴多平移一个单位	
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &m_trans);

	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);	 //用材质的话要开启灯光
	DexGameEngine::getEngine()->GetDevice()->SetMaterial(&m_material);
	DexGameEngine::getEngine()->GetDevice()->SetTexture(0,NULL);
	//m_device->SetTexture(0, NULL);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &m_matrix);	 
	m_mesh->DrawSubset(0);
	
	D3DXMatrixIdentity(&m_matrix);
	DexGameEngine::getEngine()->GetDevice()->SetTransform(D3DTS_WORLD, &m_matrix);	 
	DexGameEngine::getEngine()->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);	 //渲染之后灯光关闭
}

void CDexDraw2DOrth::DrawTriangleLine(DexPoint& point1, DexPoint& point2, DexPoint& point3, const DexColor& color1 /* = 0xffffffff */)
{
	DrawLine(point1, point2, color1);
	DrawLine(point2, point3, color1);
	DrawLine(point1, point3, color1);
}

void CDexDraw2DOrth::RenderModel(CModel* model,float xPos, float yPos, float xScale, float yScale, float zScale, float rotate /* = 0.0f */)
{
	RenderModel(model, D3DXVECTOR2(xPos, yPos), xScale, yScale, zScale, rotate);
}

void CDexDraw2DOrth::RenderModel(CModel* model,D3DXVECTOR2 pos, float xScale, float yScale, float zScale, float rotate /* = 0.0f */)
{
	DEX_ENSURE(model);

	//保留原参数
	D3DXVECTOR3 old = model->GetPosition();	 
	float oldXScale, oldYScale, oldZScale;
	model->GetScale(oldXScale, oldYScale, oldZScale);
	float oldRotateY = model->GetRotateY();
	//设置2D参数
	model->SetRotateY(rotate);
	model->SetScale(xScale, yScale, zScale);
	D3DXVECTOR2 world_pos = GetWorldPos(pos);
	model->SetPosition(D3DXVECTOR3(world_pos.x, world_pos.y, 200));  //这里Z设为200 防止 模型穿过摄像机
	model->Render();
	//原参数复位
	model->SetRotateY(oldRotateY);
	model->SetScale(oldXScale, oldYScale, oldZScale);
	model->SetPosition(old);
}

void CDexDraw2DOrth::SetTextSize(int textsize)
{
	std::map<int, CMyFont*>::iterator  it = m_fontMap.find(textsize);
	DEX_ENSURE(it != m_fontMap.end());
	m_iCurrTextSize = textsize;
}
void CDexDraw2DOrth::SetTextColor(const DexColor& color)
{
	m_fontMap[m_iCurrTextSize]->UseColor(color);
}
void CDexDraw2DOrth::DrawString(int xPos, int yPos, char* str, ...)
{
	va_list v_p;
	va_start(v_p, str);
	TCHAR szText[1024];
	wvsprintf(szText, str, v_p);
	m_fontMap[m_iCurrTextSize]->SetPos(xPos, yPos);
	m_fontMap[m_iCurrTextSize]->Print(szText);
}

void CDexDraw2DOrth::DrawString(DexRectF& rect, char* str, ...)
{
	va_list v_p;
	va_start(v_p, str);
	TCHAR szText[1024];
	wvsprintf(szText, str, v_p);
	m_fontMap[m_iCurrTextSize]->SetRect((int)rect.left, (int)rect.top, rect.getWidth(), rect.getHeight());
	m_fontMap[m_iCurrTextSize]->Print(szText);
}


CDexDraw2DOrth* get2DDrawer()
{
	static CDexDraw2DOrth* Drawer = new CDexDraw2DOrth;
	return Drawer;
}
