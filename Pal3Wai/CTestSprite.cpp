

#include "CTestSprite.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"

CTestSprite::CTestSprite()
{
	m_color = 0xff00ff00;
	m_velX = 0.0f;
	m_velY = 0.0f;
	m_pQuadTree = NULL;
	m_collide = false;
}

CTestSprite::~CTestSprite()
{

}

void CTestSprite::setCollide(bool collide)
{
	m_collide = collide;
}

void CTestSprite::setRect(DexRectF rect)
{
	m_rect = rect;
}

void CTestSprite::setVel(float x, float y)
{
	m_velX = x;
	m_velY = y;
}
void CTestSprite::Update(float delta, int mousex, int mousey)
{
	float delta_x = m_velX * delta/1000;
	float delta_y = m_velY * delta/1000;

	m_rect.left += delta_x;
	m_rect.right += delta_x;
	m_rect.top += delta_y;
	m_rect.bottom += delta_y;
	if(m_rect.left <= 0 )
	{
		m_velX = abs(m_velX);
		if(m_rect.right < 0)
		{
			delta_x = m_velX * delta/1000;
			m_rect.left += delta_x;  //保证节点不会跑到四叉树外面去
			m_rect.right += delta_x;
		}

	}
	if(m_rect.right >= 800)
	{
		m_velX = -abs(m_velX);
		if(m_rect.left > 800)
		{
			delta_x = m_velX * delta/1000;
			m_rect.left += delta_x;
			m_rect.right += delta_x;
		}


	}
	if(m_rect.top <= 0)
	{
		m_velY = abs(m_velY);
		if(m_rect.bottom < 0)
		{
			delta_y = m_velY * delta/1000;
			m_rect.top += delta_y;
			m_rect.bottom += delta_y;
		}

	}
	if(m_rect.bottom >= 600)
	{
		m_velY = -abs(m_velY);
		if(m_rect.top > 600)
		{
			delta_y = m_velY * delta/1000;
			m_rect.top += delta_y;
			m_rect.bottom += delta_y;
		}

	}
	
	//m_rects[0].rect = m_rect;
	bool change = false;//sprite所占的节点是否发生了变化
	{
		for(size_t j = 0 ; j < vec_node.size(); j++)
		{
			if(!CDexFun::check_rect_croos(m_rect, vec_node[j]->rect))
			{
				change = true;
				break;
			}
		}
	}
	if(m_velX != 0 || m_velY != 0)
	{
		for(size_t i = 0; i < vec_node.size(); i++)
		{
			//get2DDrawer()->DrawRectLine(vec_node[i]->rect, 0xffff0000,3);
		}
	}

	if(change)
	{//如果超出了节点 将sprite所占节点清空
		vec_node.clear();
		if(m_pQuadTree != NULL)
		{
			m_pQuadTree->RemoveSprite(this);
			m_pQuadTree->AddSprite(this);
		}
	}
	if(vec_node.size() == 0)
	{//
		getLog()->BeginLog();
		getLog()->Log(log_allert,"有sprite不属于任何区域，尝试重新插入...\n");
		if(m_pQuadTree->AddSprite(this))
		{
			getLog()->Log(log_allert,"本次插入成功\n");
		}
		else
		{
			getLog()->Log(log_allert,"本次插入失败，坐标left:%d, right:%d,, top:%d,, bottom:%d\n",
				(int)m_rect.left, (int)m_rect.right, (int)m_rect.top, (int)m_rect.bottom);
		}
		getLog()->EndLog();
	}
}

void CTestSprite::Render()
{
	if(m_collide)
		get2DDrawer()->SetRectColor(DexColor(1.0f, 0, 0));
	else
		get2DDrawer()->SetRectColor(m_color);
	get2DDrawer()->DrawRect(m_rect);
}