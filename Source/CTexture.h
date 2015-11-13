/*************************************************************
****DemoEx引擎纹理封装类
    可在D3D的纹理基础上进行缩放旋转等操作
****作者：阚正杰(BruceKen)
*************************************************************/

#ifndef _CTEXTRUE_H
#define _CTEXTRUE_H
#include <string>
#include "../DexBase/typedefine.h"
#include "../DexBase/CReference.h"

class CDexTex:public CRefCount
{//在DX texture的基础上加入宽高成员
private:	 
	LPDIRECT3DTEXTURE9 m_tex;
	std::string name;
	int m_width; 
	int m_height;

public:
	CDexTex();
	virtual ~CDexTex();
public:
	bool LoadTex(std::string filname, const DexColor& transcolor = NULL);
	int  GetWidth() { return m_width;};
	int  GetHeight() { return m_height;};
	void SetTex(LPDIRECT3DTEXTURE9 texture) { m_tex = texture;};
	LPDIRECT3DTEXTURE9 GetTexPt() { return m_tex;};
	std::string Name()   { return name;}; //包括路径的纹理名称
	std::string FileName(); //仅仅取纹理的文件名 不包括路径
};
enum 
{
	DRAW_TOP_LEFT, //正常输出
	DRAW_TOP_RIGHT, //右上角对准
	DRAW_BOTTOM_LEFT, //左下角对准
	DRAW_BOTTOM_RIGHT, //右下角对准
	DRAW_TOP_CENTER,  //上面中间对准
};

#define MAX_IMAGE_NAME_BYTE  64
class CTexture	 :public CRefCount
{
protected:
	char*              m_fileName;   //对应的文件名称
	LPDIRECT3DTEXTURE9 m_pTexture;
    D3DXMATRIX         m_mat;			      //纹理变换矩阵
	D3DXVECTOR2        m_scale;
	D3DXVECTOR2        m_center;
	float              m_iWidth;			  //纹理加载缩放后的实际宽度
	float              m_iHeight;
	float              m_iSrcWidth;			  //纹理源宽度
	float              m_iSrcHeight;
	float              m_fRotation;           //纹理旋转角度
	DexColor		   m_crTransColor;        //不含有透明通道的位图透明色,若纹理本身有透明通道则置为-1
	int                m_crAlpha;             //纹理透明度
	bool               m_xMirro;              //是否横向镜像显示
	bool               m_yMirro;              //是否纵向镜像显示
	bool               m_render;  //是否渲染

protected:
	virtual void ShutDown() = 0;
public:
	CTexture();
	virtual ~CTexture() {};
	virtual BOOL  Load(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, const DexColor& TransColor = NULL) = 0;
	//flag:指定xPos yPos的意义
	//0：左上角对准(正常输出)  1:图片的右上方对准 xPos yPos
	//2：图片的左下角对准      3：图片的右下角对准
	virtual void  Draw(LPDIRECT3DDEVICE9 pD3DDevice, float xPos, float yPos, int flag =0) = 0;
	virtual void  DrawPart(LPDIRECT3DDEVICE9 pD3DDevice, float xPos, float yPos, float srcXPos, float srcYPos, float width, float height, int flag =0) = 0;
	virtual long  GetWidth() { return m_iWidth;};
	virtual long  GetHeight(){ return m_iHeight;};
	virtual long  GetSrcWidth() { return m_iSrcWidth;};
	virtual long  GetSrcHeight(){ return m_iSrcHeight;};
	virtual LPDIRECT3DTEXTURE9 GetTexPoint() = 0;
	virtual void  SetScaleX(float scale) = 0;
	virtual void  SetScaleY(float scale) = 0;
	virtual void  SetScale(float xScale, float yScale) = 0;
	virtual void  SetRotation(float rotation) = 0;
	virtual void  SetXMirro(bool b);
	virtual bool  GetXMirro();
	virtual void  SetYMirro(bool b);
	virtual bool  GetYMirro();
	virtual void  SetAlpha(int alpha) = 0;	//只设置透明度 取值0~255
	virtual void  SetAlphaColor(int alphaColor) =0;	 //设置透明度的同时设置保留的通道颜色 取值规则0xff00ff00(完全不透明，保留绿色)
	virtual void  SetAlphaColor(int alpha, int red, int green, int blue){SetAlphaColor(D3DCOLOR_ARGB(alpha, red,green,blue));}
	virtual void  SetRender(bool b) ; //是否显示
	bool CompareFile(const char* filename);
};

class CTextureEx:public CTexture
{
protected:
	virtual void ShutDown();
public:
	CTextureEx();
	~CTextureEx();
	BOOL Load(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, const DexColor& TransColor = NULL);
	void Draw(LPDIRECT3DDEVICE9 pD3DDevice, float xPos, float yPos, int flag =0);
	void DrawPart(LPDIRECT3DDEVICE9 pD3DDevice, float xPos, float yPos, float srcXPos, float srcYPos, float width, float height, int flag =0);
	virtual LPDIRECT3DTEXTURE9 GetTexPoint() { return m_pTexture;};

	void SetScaleX(float scale);
	void SetScaleY(float scale);
	void SetScale(float xScale, float yScale);
	void SetRotation(float rotation) { m_fRotation = rotation;};
	void SetAlpha(int alpha);
    void SetAlphaColor(int alphaColor);	
};



#endif