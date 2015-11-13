/*
/*************************************************************
****Dex引擎地形类
****作者：阚正杰(BruceKen)
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
	CN_SEE, //可见


};
enum
{
	CQN_LT, //左上	
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
#define HEIGHT_SIZE 1024	  //地图最大宽度  


class  CTerrain
{
protected:
	LPDIRECT3DDEVICE9 m_device;
	LPDIRECT3DVERTEXBUFFER9 m_vb;
	LPDIRECT3DINDEXBUFFER9  m_ib; //三角形索引
	LPDIRECT3DINDEXBUFFER9  m_lineB; //线段索引
	LPDIRECT3DTEXTURE9   m_texture;
	int m_renderStyle;  //渲染方式（线性、三角形渲染）

	//CQuadNode* m_rootNode;
	D3DXVECTOR3 m_pos;
	bool m_render;

	int m_row;
	int m_col;
	int m_cellWidth;
	int   m_cellHeight;
	float m_scale;  //高度缩放
	float **m_height;

public:
	CTerrain(LPDIRECT3DDEVICE9 device);
	~CTerrain();

public:
	virtual void ShutDown();

	virtual bool  LoadIni(char* filename, char* field);
	virtual bool  CreateTerrain(char* filename, int cellWidth, int cellHeight,float scale); //這裡的filename是一個高度圖文件
	virtual bool  CreateTerrain2(char* filename, int cellWidth, int cellHeight,float scale,bool flag);//這裡的filename是一個自定義編輯的高度數據文件 
	virtual bool  LoadTexture(char* filename);
	virtual void  SetPos(D3DXVECTOR3 pos);
	virtual float GetHeight(float x, float z);
	virtual float GetHeight(D3DXVECTOR3 pos);
	virtual void  SetRenderStyle(int style);

	virtual bool Create();
	virtual void Render();
};