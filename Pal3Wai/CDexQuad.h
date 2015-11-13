

#ifndef _CDEX_QUAD_H
#define _CDEX_QUAD_H
#include "../widget/DexGuiStruct.h"

using namespace DexGUI;
/*
����ļ�CTestSprite.h
��ĩ���������Ĳ���ʵ��2Dƽ�������ײ���,ԭ���Ѿ��������Ҳд�˲��Դ��룬
sprite�����ܶ��ʱ�����Ч�ʱȽϵ��£������2D��Ⱦ������Ҫ�Ż����������������ο���Ʋ��֡�
						����2014.10.19
�����ǲ��Դ���
ȫ�ֱ���������
	CDexQuad* g_pQuadTree = NULL;
��ʼ����
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
render������
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
		list<CTestSprite*> list_collide; //ֻ����Ҷ�ӽڵ��sprite��ײ���
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
		bool AddSprite(CTestSprite* sprite, bool flag = false); //flag:true��ʾ���ڸ��׽ڵ���˵ѹ��
		bool RemoveSprite(CTestSprite* sprite, bool flag = false); //flag:true��ʾ�Ѿ������Ƚڵ�ɾ��
		void Update(float delta, int mousex, int mousey);
		void Render();//��Ⱦnode��������

		void InitChildInfo(stQuadNode* child);
		void setChildRect(stQuadNode* child);
	private:
		bool checkt_and_remove1(CTestSprite* sprite); //��list_sprite�в��ң��ҵ���ɾ��
		bool checkt_and_remove2(CTestSprite* sprite); //��list_collide�в��ң��ҵ���ɾ��
		bool check_left_top(const DexRectF& sprite_rect); //�鿴sprite_rect�Ƿ��������Ϸ�
		bool check_left_top_in(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ���ȫ�������Ϸ�
		bool check_right_top(const DexRectF& sprite_rect); //�鿴sprite_rect�Ƿ��������Ϸ�
		bool check_right_top_in(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ���ȫ�������Ϸ�
		bool check_left_bottom(const DexRectF& sprite_rect); //�鿴sprite_rect�Ƿ��������·�
		bool check_left_bottom_in(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ���ȫ�������·�
		bool check_right_bottom(const DexRectF& sprite_rect); //�鿴sprite_rect�Ƿ��������·�
		bool check_right_bottom_in(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ���ȫ�������·�
		bool check_left_right_top(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ�ѹ���������Ͻ�������
		bool check_left_right_bottom(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ�ѹ���������½�������
		bool check_top_bottom_left(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ�ѹ���������½�������
		bool check_top_bottom_right(const DexRectF& sprite_rect);//�鿴sprite_rect�Ƿ�ѹ���������½�������
		bool check_cross_center(const DexRectF& sprite_rect); //�鿴sprite_rect�Ƿ�ѹ��ʮ�ֲ�����
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
Dex������Դ�ع�������
������ϵͳ�е����ӳء��Ĳ����еĽڵ�صȵ�
*/
//class CDexSourcePool
//{
//public:
//	CDexSourcePool()
//	virtual ~CDexSourcePool() = 0;
//
//
//};
//�ڵ��
class CDexQuadNodePool
{
private:
	std::list<CDexQuad::stQuadNode*>  m_nodeList;  //���еĽڵ�����
public:
	CDexQuadNodePool();
	virtual ~CDexQuadNodePool();
public:
	void CreateNode(int number);  
	void Release();    //�ͷ����еĽڵ���Դ
	CDexQuad::stQuadNode* GetNode();      //�Ĳ���ϵͳ��ڵ������һ���ڵ�
	void PushNode(CDexQuad::stQuadNode* node); //�ⲿ����ϵͳ����ϵͳ�������������ӷŻ����ӳ���
};

extern CDexQuadNodePool* getQuadNodePool();
#endif