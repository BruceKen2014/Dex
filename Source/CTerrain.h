/*
/*************************************************************
****Dex���������
****���ߣ�������(BruceKen)
*************************************************************
*/

#pragma  once

#include "../DexBase/typedefine.h"
#include "../DexBase/CIni.h"
#include <fstream>

typedef struct _Terrain_Vertex
{
	D3DXVECTOR3 m_pos;
	DexColor  m_color;
	float tu, tv;

public:
	_Terrain_Vertex()
	{
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_color = D3DCOLOR_XRGB(255, 255, 255);
		tu = tv = 0.0f;
	}
}Terrain_Vertex;

enum
{
	CN_SEE, //�ɼ�


};
enum
{
	CQN_LT, //����	
	CQN_RT,
	CQN_LB,
	CQN_RB,
};	 
enum{  
	TERRIAN_LINE,
	TERRIAN_TRI,

};
class CQuadNode
{
public:
	int    m_centerIndex;
	int    m_CornerIndex[4];
	CQuadNode* m_parent;
	CQuadNode* m_pChildNode[4];
public:
	CQuadNode();
	~CQuadNode();
public:
	void  AddChild(int index);
};
#define HEIGHT_SIZE 1024	  //��ͼ�����  


class  CTerrain
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DVERTEXBUFFER9 m_vb;
	LPDIRECT3DINDEXBUFFER9  m_ib; //����������
	LPDIRECT3DINDEXBUFFER9  m_lineB; //�߶�����
	LPDIRECT3DTEXTURE9   m_texture;
	int m_renderStyle;  //��Ⱦ��ʽ�����ԡ���������Ⱦ��

	//CQuadNode* m_rootNode;
	D3DXVECTOR3 m_pos;
	bool m_render;

	int m_row;
	int m_col;
	int m_cellWidth;
	int   m_cellHeight;
	float m_scale;  //�߶�����
	float **m_height;

public:
	CTerrain(LPDIRECT3DDEVICE9 device);
	~CTerrain();

public:
	virtual void ShutDown();

	virtual bool  LoadIni(char* filename, char* field);
	virtual bool  CreateTerrain(char* filename, int cellWidth, int cellHeight,float scale); //�@�e��filename��һ���߶ȈD�ļ�
	virtual bool  CreateTerrain2(char* filename, int cellWidth, int cellHeight,float scale,bool flag);//�@�e��filename��һ���Զ��x��݋�ĸ߶Ȕ����ļ� 
	virtual bool  LoadTexture(char* filename);
	virtual void  SetPos(D3DXVECTOR3 pos);
	virtual float GetHeight(float x, float z);
	virtual float GetHeight(D3DXVECTOR3 pos);
	virtual void  SetRenderStyle(int style);

	virtual bool Create();
	virtual void Render();
};