
#ifndef _DEX_TERRAIN_H
#define _DEX_TERRAIN_H

#include "DexSceneObject.h"

const float Terrain_cell_width = 20.0f;
const float Terrain_cell_height = 20.0f;
const int Terrain_row = 200;
const int Terrain_col = 200;
class CDexTex;
class CDexTerrain: public CDexSceneObject
{
	Dex_DeclareClass(CDexTerrain,2)
public:
	enum{  
		TERRIAN_LINE,
		TERRIAN_TRI,

	};
protected:
	LPDIRECT3DVERTEXBUFFER9 m_vb;
	stVertex1 *m_vbVertex ;
	LPDIRECT3DINDEXBUFFER9  m_ib; //三角形索引
	LPDIRECT3DINDEXBUFFER9  m_lineB; //线段索引
	int m_renderStyle;  //渲染方式（线性、三角形渲染）
	CDexTex*   m_pTex;

	//CQuadNode* m_rootNode;
	D3DXVECTOR3 m_pos;
	bool m_render;

	int m_height[Terrain_row][Terrain_col];

protected:
	void _updateVertex();
public:
	CDexTerrain();
	virtual ~CDexTerrain();

public:
	virtual bool  CreateTerrain(); 
	virtual void  SetPos(D3DXVECTOR3 pos);

	virtual void  SetHeight(int row, int col, int height);
	        int   GetHeight(int row, int col);
	virtual float GetHeight(float x, float z);
	virtual float GetHeight(D3DXVECTOR3 pos);
	virtual void  SetRenderStyle(int style);
	virtual D3DXVECTOR3  GetPosition();
	virtual bool  SetTexture(const char* texname);

	bool InitFromFile(const char* filename);
	bool SaveToFile(const char* filename);

	virtual void Reset();
	virtual bool Update(int delta);
	virtual bool Render();
};
#endif