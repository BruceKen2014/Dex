

#ifndef _CDEX_QUAD_H
#define _CDEX_QUAD_H
#include "../widget/DexGuiStruct.h"

using namespace DexGUI;
/*
相关文件CTestSprite.h
周末两天在做四叉树实现2D平面快速碰撞检测,原理已经搞清楚，也写了测试代码，
sprite数量很多的时候绘制效率比较底下，想必是2D渲染流程需要优化，尤其是线条矩形框绘制部分。
						——2014.10.19
以下是测试代码
全局变量声明：
	CDexQuad* g_pQuadTree = NULL;
初始化：
	g_pQuadTree = new CDexQuad(DexRectF(0,0, WINDOW_WIDTH, WINDOW_HEIGHT), 4);
	float xPos = -5;
	float yPos = -5;
	float width = 10;
	float height = 10;
	float velX = 00.0f;// + 10.0f * RandFloat();
	float velY = 20.0f ;//+ 10.0f * RandFloat();
	for(int i = 0 ; i < 500; i++)
	{
	CTestSprite* sprite = new CTestSprite();

	sprite->setVel(velX, velY);
	sprite->setRect(DexRectF(xPos, yPos, xPos + width, yPos + height));
	g_pQuadTree->AddSprite(sprite);	
	xPos = 1+ (WINDOW_WIDTH-50) * RandFloat();
	yPos = 1+(WINDOW_HEIGHT-50) * RandFloat();
	width = 5 + 5.0f * RandFloat();
	height = 5 + 5.0f * RandFloat();
	//velX = 0.0f ;//+ 50.0f * RandFloat();
	//velY = 0.0f ;//+ 50.0f * RandFloat();
	velX = 10.0f + 10.0f * RandFloat();
	velY = 10.0f + 10.0f * RandFloat();
	}
render函数：
	g_pQuadTree->Update(g_delta, g_iXMouse, g_iYMouse);
	g_pQuadTree->Render();	
*/
class CTestSprite;
class CDexQuad
{
public:
	struct stQuadNode
	{
		static float tick ;
		DexRectF rect;
		int width;
		int level;
		DWORD color;
		list<CTestSprite*> list_sprites;
		list<CTestSprite*> list_collide; //只参与叶子节点的sprite碰撞检测
		CDexQuad*   p_quad_tree;
		stQuadNode* p_father;
		stQuadNode* p_left_top_child;
		stQuadNode* p_left_bottom_child;
		stQuadNode* p_right_top_child;
		stQuadNode* p_right_bottom_child;
		stQuadNode();
		bool isLeafNode();
		void ResetInfo();
		void try_inset_to_rects(CTestSprite* sprite);
		bool AddSprite(CTestSprite* sprite, bool flag = false); //flag:true表示对于父亲节点来说压线
		bool RemoveSprite(CTestSprite* sprite, bool flag = false); //flag:true表示已经被祖先节点删了
		void Update(float delta, int mousex, int mousey);
		void Render();//渲染node的轮廓线

		void InitChildInfo(stQuadNode* child);
		void setChildRect(stQuadNode* child);
	private:
		bool checkt_and_remove1(CTestSprite* sprite); //从list_sprite中查找，找到就删除
		bool checkt_and_remove2(CTestSprite* sprite); //从list_collide中查找，找到就删除
		bool check_left_top(const DexRectF& sprite_rect); //查看sprite_rect是否落在左上方
		bool check_left_top_in(const DexRectF& sprite_rect);//查看sprite_rect是否完全落在左上方
		bool check_right_top(const DexRectF& sprite_rect); //查看sprite_rect是否落在右上方
		bool check_right_top_in(const DexRectF& sprite_rect);//查看sprite_rect是否完全落在右上方
		bool check_left_bottom(const DexRectF& sprite_rect); //查看sprite_rect是否落在左下方
		bool check_left_bottom_in(const DexRectF& sprite_rect);//查看sprite_rect是否完全落在左下方
		bool check_right_bottom(const DexRectF& sprite_rect); //查看sprite_rect是否落在左下方
		bool check_right_bottom_in(const DexRectF& sprite_rect);//查看sprite_rect是否完全落在左下方
		bool check_left_right_top(const DexRectF& sprite_rect);//查看sprite_rect是否压在左上右上交界线上
		bool check_left_right_bottom(const DexRectF& sprite_rect);//查看sprite_rect是否压在左下右下交界线上
		bool check_top_bottom_left(const DexRectF& sprite_rect);//查看sprite_rect是否压在左上左下交界线上
		bool check_top_bottom_right(const DexRectF& sprite_rect);//查看sprite_rect是否压在右上右下交界线上
		bool check_cross_center(const DexRectF& sprite_rect); //查看sprite_rect是否压在十字叉上了
	};
	enum EChild
	{
		E_LEFT_TOP,
		E_LEFT_BOTTOM,
		E_RIGHT_TOP,
		E_RIGHT_BOTTOM
	};
protected:
	stQuadNode* m_pRoot;
	int         m_level;
public:
	CDexQuad(DexRectF rect, int level);
	virtual ~CDexQuad();

	bool  AddSprite(CTestSprite* sprite);
	bool  RemoveSprite(CTestSprite* sprite);

	void Update(float delta, int mousex, int mousey);
	void Render();


};


/*
Dex引擎资源池公共父类
如粒子系统中的粒子池、四叉树中的节点池等等
*/
//class CDexSourcePool
//{
//public:
//	CDexSourcePool()
//	virtual ~CDexSourcePool() = 0;
//
//
//};
//节点池
class CDexQuadNodePool
{
private:
	std::list<CDexQuad::stQuadNode*>  m_nodeList;  //空闲的节点链表
public:
	CDexQuadNodePool();
	virtual ~CDexQuadNodePool();
public:
	void CreateNode(int number);  
	void Release();    //释放所有的节点资源
	CDexQuad::stQuadNode* GetNode();      //四叉树系统向节点池申请一个节点
	void PushNode(CDexQuad::stQuadNode* node); //外部粒子系统将本系统生命结束的粒子放回粒子池中
};

extern CDexQuadNodePool* getQuadNodePool();
#endif