

#ifndef _CDEX_TESTSPRITE_H
#define _CDEX_TESTSPRITE_H
#include "../widget/DexGuiStruct.h"
#include "CDexQuad.h"
#include <vector>

using namespace DexGUI;
using namespace std;

class CTestSprite
{
	//friend struct stQuadNode;
public:
	DexRectF m_rect;
	DexRectF m_rectInsert; //保存插入时的rect
	float   m_velX;  //一秒运动多少
	float   m_velY;
	bool    m_collide;
	DWORD   m_color;
	CDexQuad* m_pQuadTree;
	vector<CDexQuad::stQuadNode *> vec_node;

public:
	CTestSprite();
	~CTestSprite();

	void setCollide(bool collide);
	void setRect(DexRectF rect);
	void setVel(float x, float y);
	void Update(float delta, int mousex, int mousey);
	void Render();
};
#endif