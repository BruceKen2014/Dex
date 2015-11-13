/*************************************************************
****DemoEx���������װ��
    ����D3D����������Ͻ���������ת�Ȳ���
****���ߣ�������(BruceKen)
*************************************************************/

#ifndef _CTEXTRUE_H
#define _CTEXTRUE_H
#include <string>
#include "../DexBase/typedefine.h"
#include "../DexBase/CReference.h"

class CDexTex:public CRefCount
{//��DX texture�Ļ����ϼ����߳�Ա
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
	std::string Name()   { return name;}; //����·������������
	std::string FileName(); //����ȡ������ļ��� ������·��
};
enum 
{
	DRAW_TOP_LEFT, //�������
	DRAW_TOP_RIGHT, //���ϽǶ�׼
	DRAW_BOTTOM_LEFT, //���½Ƕ�׼
	DRAW_BOTTOM_RIGHT, //���½Ƕ�׼
	DRAW_TOP_CENTER,  //�����м��׼
};

#define MAX_IMAGE_NAME_BYTE  64
class CTexture	 :public CRefCount
{
protected:
	char*              m_fileName;   //��Ӧ���ļ�����
	LPDIRECT3DTEXTURE9 m_pTexture;
    D3DXMATRIX         m_mat;			      //����任����
	D3DXVECTOR2        m_scale;
	D3DXVECTOR2        m_center;
	float              m_iWidth;			  //����������ź��ʵ�ʿ��
	float              m_iHeight;
	float              m_iSrcWidth;			  //����Դ���
	float              m_iSrcHeight;
	float              m_fRotation;           //������ת�Ƕ�
	DexColor		   m_crTransColor;        //������͸��ͨ����λͼ͸��ɫ,����������͸��ͨ������Ϊ-1
	int                m_crAlpha;             //����͸����
	bool               m_xMirro;              //�Ƿ��������ʾ
	bool               m_yMirro;              //�Ƿ���������ʾ
	bool               m_render;  //�Ƿ���Ⱦ

protected:
	virtual void ShutDown() = 0;
public:
	CTexture();
	virtual ~CTexture() {};
	virtual BOOL  Load(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, const DexColor& TransColor = NULL) = 0;
	//flag:ָ��xPos yPos������
	//0�����ϽǶ�׼(�������)  1:ͼƬ�����Ϸ���׼ xPos yPos
	//2��ͼƬ�����½Ƕ�׼      3��ͼƬ�����½Ƕ�׼
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
	virtual void  SetAlpha(int alpha) = 0;	//ֻ����͸���� ȡֵ0~255
	virtual void  SetAlphaColor(int alphaColor) =0;	 //����͸���ȵ�ͬʱ���ñ�����ͨ����ɫ ȡֵ����0xff00ff00(��ȫ��͸����������ɫ)
	virtual void  SetAlphaColor(int alpha, int red, int green, int blue){SetAlphaColor(D3DCOLOR_ARGB(alpha, red,green,blue));}
	virtual void  SetRender(bool b) ; //�Ƿ���ʾ
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