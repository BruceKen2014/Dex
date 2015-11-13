
#include "../DexBase/DexLog.h"
#include "../DexBase/CDexTime.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/typedefine.h"

#include "CDexQuad.h"
#include "CTestSprite.h"


CDexQuad::stQuadNode::stQuadNode()
{
	color = 0xffffff00;
	width = 1;
	p_father = NULL;
	level = 0;
	p_left_top_child = NULL;
	p_right_top_child = NULL;
	p_left_bottom_child = NULL;
	p_right_bottom_child = NULL;
}
bool CDexQuad::stQuadNode:: isLeafNode() 
{
	return 		p_left_top_child    == NULL && p_right_top_child == NULL   && 
				p_left_bottom_child == NULL && p_right_bottom_child == NULL;
}
void CDexQuad::stQuadNode::ResetInfo()
{
	p_father = NULL;
	level = 0;
	p_left_top_child = NULL;
	p_right_top_child = NULL;
	p_left_bottom_child = NULL;
	p_right_bottom_child = NULL;
}
void CDexQuad::stQuadNode::try_inset_to_rects(CTestSprite* sprite)
{
	//for(size_t i = 0 ; i < sprite->m_rects.size(); i++)
	//{
	//	DexRectF temp_rect = sprite->m_rects[i].rect;
	//	if(CDexFun::check_rect_croos(rect,temp_rect))
	//	{//只要sprite的其中一个矩形框在本节点范围内，就把本节点加入到sprite的m_rects信息中
	//		sprite->m_rects[i].vec_node.push_back(this);
	//	}
	//}
}
bool CDexQuad::stQuadNode::AddSprite(CTestSprite* sprite, bool flag)
{
	if(level == 0)
	{
		DexRectF sprite_rect = sprite->m_rect;
		if(!CDexFun::check_rect_croos(sprite_rect, rect))
		{
			return false;
		}
	}
	if(level == p_quad_tree->m_level)
	{//已经到最底层了
		if(flag)
		{//压到祖先的线了
			list_collide.push_back(sprite);
		}
		else
		{
			list_sprites.push_back(sprite);
		}
		DexRectF temp_rect = sprite->m_rect;
		if(CDexFun::check_rect_croos(rect,temp_rect))
		{//
			sprite->vec_node.push_back(this);
		}
		return true;
	}
	//未到最底层 则根据情况继续向下分割
	if(1)
	{//未被分割的物体 即小于最小子区域的物体
		DexRectF sprite_rect = sprite->m_rect;
		if(check_left_top(sprite_rect))
		{//落在左上方	
			if(p_left_top_child == NULL)
			{//左上节点为空
				p_left_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_top_child);
			}
			if(check_left_top_in(sprite_rect))
			{//完全落在左上子节点
				p_left_top_child->AddSprite(sprite, flag);
			}
			else
			{//压了左上两条线中的一条或两条
				if(!flag)
				{
					list_sprites.push_back(sprite);
					p_left_top_child->AddSprite(sprite, true);
				}
				else
					p_left_top_child->AddSprite(sprite, flag);
			}
		}
		else if(check_right_top(sprite_rect))
		{//落在右上方
			if(p_right_top_child == NULL)
			{//右上节点为空
				p_right_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_top_child);
			}	
			if(check_right_top_in(sprite_rect))
			{//完全落在右上子节点
				p_right_top_child->AddSprite(sprite, flag);
			}
			else
			{//压了右上两条线中的一条或两条
				if(!flag)
				{
					list_sprites.push_back(sprite);
					p_right_top_child->AddSprite(sprite, true);
				}
				else
					p_right_top_child->AddSprite(sprite, flag);
			}
		}
		else if(check_left_bottom(sprite_rect))
		{//落在左下方	
			if(p_left_bottom_child == NULL)
			{//右上节点为空
				p_left_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_bottom_child);
			}
			if(check_left_bottom_in(sprite_rect))
			{//完全落在左下子节点
				p_left_bottom_child->AddSprite(sprite, flag);
			}
			else
			{
				if(!flag)
				{
					list_sprites.push_back(sprite);
					p_left_bottom_child->AddSprite(sprite, true);
				}
				else
					p_left_bottom_child->AddSprite(sprite, flag);
			}
		}
		else if(check_right_bottom(sprite_rect))
		{//落在右下方
			if(p_right_bottom_child == NULL)
			{//右下节点为空
				p_right_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_bottom_child);
			}
			if(check_right_bottom_in(sprite_rect))
			{//完全落在右下子节点
				p_right_bottom_child->AddSprite(sprite, flag);
			}
			else
			{
				if(!flag)
				{
					list_sprites.push_back(sprite);
					p_right_bottom_child->AddSprite(sprite, true);
				}
				else
					p_right_bottom_child->AddSprite(sprite, flag);
			}
		}
		else if(check_left_right_top(sprite_rect))
		{//压在左上右上交界线上
			if(p_left_top_child == NULL)
			{
				p_left_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_top_child);
			}
			if(p_right_top_child == NULL)
			{
				p_right_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_top_child);
			}
			if(!flag)
			{
				list_sprites.push_back(sprite);
				p_left_top_child->AddSprite(sprite, true);
				p_right_top_child->AddSprite(sprite, true);
			}
			else
			{
				p_left_top_child->AddSprite(sprite, flag);
				p_right_top_child->AddSprite(sprite, flag);
			}
		}
		else if(check_left_right_bottom(sprite_rect))
		{//压在左下右下交界线上
			if(p_left_bottom_child == NULL)
			{
				p_left_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_bottom_child);
			}
			if(p_right_bottom_child == NULL)
			{
				p_right_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_bottom_child);
			}
			if(!flag)
			{
				list_sprites.push_back(sprite);
				p_left_bottom_child->AddSprite(sprite, true);
				p_right_bottom_child->AddSprite(sprite, true);
			}
			else
			{
				p_left_bottom_child->AddSprite(sprite, flag);
				p_right_bottom_child->AddSprite(sprite, flag);
			}
		}
		else if(check_top_bottom_left(sprite_rect))
		{//压在左上左下交界线上
			if(p_left_top_child == NULL)
			{
				p_left_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_top_child);
			}
			if(p_left_bottom_child == NULL)
			{
				p_left_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_bottom_child);
			}
			if(!flag)
			{
				list_sprites.push_back(sprite);
				p_left_top_child->AddSprite(sprite, true);
				p_left_bottom_child->AddSprite(sprite, true);
			}
			else
			{
				p_left_top_child->AddSprite(sprite, flag);
				p_left_bottom_child->AddSprite(sprite, flag);
			}
		}
		else if(check_top_bottom_right(sprite_rect))
		{//压在右上右下交界线上
			if(p_right_top_child == NULL)
			{
				p_right_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_top_child);
			}
			if(p_right_bottom_child == NULL)
			{
				p_right_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_bottom_child);
			}
			if(!flag)
			{
				list_sprites.push_back(sprite);
				p_right_top_child->AddSprite(sprite, true);
				p_right_bottom_child->AddSprite(sprite, true);
			}
			else
			{
				p_right_top_child->AddSprite(sprite, flag);
				p_right_bottom_child->AddSprite(sprite, flag);
			}
		}
		else if(check_cross_center(sprite_rect))
		{//压在十字叉上了
			if(p_left_top_child == NULL)
			{
				p_left_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_top_child);
			}
			if(p_left_bottom_child == NULL)
			{
				p_left_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_left_bottom_child);
			}
			if(p_right_top_child == NULL)
			{
				p_right_top_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_top_child);
			}
			if(p_right_bottom_child == NULL)
			{
				p_right_bottom_child = getQuadNodePool()->GetNode();
				InitChildInfo(p_right_bottom_child);
			}
			if(!flag)
			{
				list_sprites.push_back(sprite); 
				p_left_top_child->AddSprite(sprite, true);
				p_left_bottom_child->AddSprite(sprite, true);
				p_right_top_child->AddSprite(sprite, true);
				p_right_bottom_child->AddSprite(sprite, true);
			}
			else
			{
				p_left_top_child->AddSprite(sprite, false);
				p_left_bottom_child->AddSprite(sprite, false);
				p_right_top_child->AddSprite(sprite, false);
				p_right_bottom_child->AddSprite(sprite, false);
			}	 
		}
	}
	else
	{
		//todo...
	}
	return true;

}

bool CDexQuad::stQuadNode::checkt_and_remove1(CTestSprite* sprite)
{
	foreach(list<CTestSprite*> , ite,list_sprites)
	{
		if(*ite == sprite)
		{
			*ite = NULL;
			ite = list_sprites.erase(ite);
			return true;
		}
	}
	return false;
}
bool CDexQuad::stQuadNode::checkt_and_remove2(CTestSprite* sprite)
{
	foreach(list<CTestSprite*> , ite,list_collide)
	{
		if(*ite == sprite)
		{
			*ite = NULL;
			ite = list_collide.erase(ite);
			return true;
		}
	}
	return false;
}
bool CDexQuad::stQuadNode::check_left_top(const DexRectF& sprite_rect)
{
	return sprite_rect.right <=  (rect.left + rect.getWidth()/2)
		&& sprite_rect.bottom <= (rect.top + rect.getHeight()/2);
}

bool CDexQuad::stQuadNode::check_left_top_in(const DexRectF& sprite_rect)
{
	return sprite_rect.right <=  (rect.left + rect.getWidth()/2)
		&& sprite_rect.bottom <= (rect.top + rect.getHeight()/2)
		&& sprite_rect.left >= rect.left 
		&& sprite_rect.top >= rect.top ;
}
bool CDexQuad::stQuadNode::check_right_top(const DexRectF& sprite_rect)
{
	return sprite_rect.left >= (rect.left + rect.getWidth()/2)
	&& sprite_rect.bottom <= (rect.top + rect.getHeight()/2);
}
bool CDexQuad::stQuadNode::check_right_top_in(const DexRectF& sprite_rect)
{	
	return sprite_rect.left >= (rect.left + rect.getWidth()/2)
	&& sprite_rect.bottom <= (rect.top + rect.getHeight()/2)
	&& sprite_rect.right <= rect.right
	&& sprite_rect.top >= rect.top ;
}
bool CDexQuad::stQuadNode::check_left_bottom(const DexRectF& sprite_rect)
{
	return sprite_rect.right <=  (rect.left + rect.getWidth()/2)
		&& sprite_rect.top >= (rect.top + rect.getHeight()/2);
}
bool CDexQuad::stQuadNode::check_left_bottom_in(const DexRectF& sprite_rect)
{
	return sprite_rect.right <=  (rect.left + rect.getWidth()/2)
		&& sprite_rect.top >= (rect.top + rect.getHeight()/2)
		&& sprite_rect.left >= rect.left
		&& sprite_rect.bottom <= rect.bottom ;
}
bool CDexQuad::stQuadNode::check_right_bottom(const DexRectF& sprite_rect)
{
	return  sprite_rect.left >= (rect.left + rect.getWidth()/2)
		&&   sprite_rect.top >= (rect.top + rect.getHeight()/2)  ;

}
bool CDexQuad::stQuadNode::check_right_bottom_in(const DexRectF& sprite_rect)
{
	return  sprite_rect.left >= (rect.left + rect.getWidth()/2)
		&&   sprite_rect.top >= (rect.top + rect.getHeight()/2) 
		&& sprite_rect.right <= rect.right 
		&& sprite_rect.bottom <= rect.bottom;
}
bool CDexQuad::stQuadNode::check_left_right_top(const DexRectF& sprite_rect)
{
	return sprite_rect.left <(rect.left + rect.getWidth()/2) 
		&& sprite_rect.right >(rect.left + rect.getWidth()/2) 
		&&  sprite_rect.bottom <(rect.top + rect.getHeight()/2);
}

bool CDexQuad::stQuadNode::check_left_right_bottom(const DexRectF& sprite_rect)
{
	return sprite_rect.left <(rect.left + rect.getWidth()/2) 
		&& sprite_rect.right >(rect.left + rect.getWidth()/2) 
		&&  sprite_rect.top > (rect.top + rect.getHeight()/2);
}

bool CDexQuad::stQuadNode::check_top_bottom_left(const DexRectF& sprite_rect)
{
	return sprite_rect.top <(rect.top + rect.getHeight()/2)
		&& sprite_rect.bottom >(rect.top + rect.getHeight()/2) 
		&& sprite_rect.right < (rect.left + rect.getWidth()/2);
}
bool CDexQuad::stQuadNode::check_top_bottom_right(const DexRectF& sprite_rect)
{
	return sprite_rect.top <(rect.top + rect.getHeight()/2)
		&& sprite_rect.bottom >(rect.top + rect.getHeight()/2) 
		&& sprite_rect.left > (rect.left + rect.getWidth()/2);
}
bool CDexQuad::stQuadNode::check_cross_center(const DexRectF& sprite_rect)
{
	return sprite_rect.top <(rect.top + rect.getHeight()/2)
		&&  sprite_rect.bottom >(rect.top + rect.getHeight()/2) 
		&&  sprite_rect.left<(rect.left + rect.getWidth()/2) 
		&&  sprite_rect.right >(rect.left + rect.getWidth()/2);
}


bool CDexQuad::stQuadNode::RemoveSprite(CTestSprite* sprite, bool flag)
{
	if(level == p_quad_tree->m_level)
	{//已经是叶子层了
		bool ret = false;
		ret = checkt_and_remove2(sprite) || checkt_and_remove1(sprite);
		if(list_sprites.size() == 0 && list_collide.size() == 0)
		{//叶子里面什么也没有了，将自身从四叉树中删除(返回节点池)
			stQuadNode* node = this;
			while(node != NULL && node->isLeafNode())
			{	
				if(node->p_father != NULL)
				{
					if(node->p_father->p_left_top_child == node)
						node->p_father->p_left_top_child = NULL;
					else if(node->p_father->p_left_bottom_child == node)
						node->p_father->p_left_bottom_child = NULL;
					else if(node->p_father->p_right_top_child == node)
						node->p_father->p_right_top_child = NULL;
					else if(node->p_father->p_right_bottom_child == node)
						node->p_father->p_right_bottom_child = NULL;
				}
				stQuadNode* node_father = node->p_father;
				node->ResetInfo();
				getQuadNodePool()->PushNode(node);
				node = node_father;
			}
		}
		return ret;
	}
	else
	{
		DexRectF sprite_rect = sprite->m_rectInsert; //用插入时的rect
		if(check_left_top(sprite_rect))
		{//落在左上方	
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_left_top_child != NULL)
						p_left_top_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					if(p_left_top_child != NULL)
						find = p_left_top_child->RemoveSprite(sprite, flag);
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_left_top_child != NULL)
					p_left_top_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_right_top(sprite_rect))
		{//落在右上方
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_right_top_child != NULL)
						p_right_top_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					if(p_right_top_child != NULL)
						find = p_right_top_child->RemoveSprite(sprite, flag);
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_right_top_child != NULL)
					p_right_top_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_left_bottom(sprite_rect))
		{//落在左下方	
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_left_bottom_child != NULL)
						p_left_bottom_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					if(p_left_bottom_child != NULL)
						find = p_left_bottom_child->RemoveSprite(sprite, flag);
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_left_bottom_child != NULL)
					p_left_bottom_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_right_bottom(sprite_rect))
		{//落在右下方
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_right_bottom_child != NULL)
						p_right_bottom_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					if(p_right_bottom_child != NULL)
						find = p_right_bottom_child->RemoveSprite(sprite, flag);
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_right_bottom_child != NULL)
					p_right_bottom_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_left_right_top(sprite_rect))
		{//压在左上右上交界线上
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_left_top_child != NULL)
						p_left_top_child->RemoveSprite(sprite, true);
					if(p_right_top_child != NULL)
						p_right_top_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					bool find1 = false;
					bool find2 = false;
					if(p_left_top_child != NULL)
						find1 = p_left_top_child->RemoveSprite(sprite, flag);
					if(p_right_top_child != NULL)
						find2 = p_right_top_child->RemoveSprite(sprite, flag);
					find = find1 || find2;
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_left_top_child != NULL)
					p_left_top_child->RemoveSprite(sprite, flag);
				if(p_right_top_child != NULL)
					p_right_top_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_left_right_bottom(sprite_rect))
		{//压在左下右下交界线上
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_left_bottom_child != NULL)
						p_left_bottom_child->RemoveSprite(sprite, true);
					if(p_right_bottom_child != NULL)
						p_right_bottom_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					bool find1 = false;
					bool find2 = false;
					if(p_left_bottom_child != NULL)
						find1 = p_left_bottom_child->RemoveSprite(sprite, flag);
					if(p_right_bottom_child != NULL)
						find2 = p_right_bottom_child->RemoveSprite(sprite, flag);
					find = find1 || find2;
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_left_bottom_child != NULL)
					p_left_bottom_child->RemoveSprite(sprite, flag);
				if(p_right_bottom_child != NULL)
					p_right_bottom_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_top_bottom_left(sprite_rect))
		{//压在左上左下交界线上
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_left_top_child != NULL)
						p_left_top_child->RemoveSprite(sprite, true);
					if(p_left_bottom_child != NULL)
						p_left_bottom_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					bool find1 = false;
					bool find2 = false;
					if(p_left_top_child != NULL)
						find1 = p_left_top_child->RemoveSprite(sprite, flag);
					if(p_left_bottom_child != NULL)
						find2 = p_left_bottom_child->RemoveSprite(sprite, flag);
					find = find1 || find2;
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_left_top_child != NULL)
					p_left_top_child->RemoveSprite(sprite, flag);
				if(p_left_bottom_child != NULL)
					p_left_bottom_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_top_bottom_right(sprite_rect))
		{//压在右上右下交界线上
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_right_top_child != NULL)
						p_right_top_child->RemoveSprite(sprite, true);
					if(p_right_bottom_child != NULL)
						p_right_bottom_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					bool find1 = false;
					bool find2 = false;
					if(p_right_top_child != NULL)
						find1 = p_right_top_child->RemoveSprite(sprite, flag);
					if(p_right_bottom_child != NULL)
						find2 = p_right_bottom_child->RemoveSprite(sprite, flag);
					find = find1 || find2;
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_right_top_child != NULL)
					p_right_top_child->RemoveSprite(sprite, flag);
				if(p_right_bottom_child != NULL)
					p_right_bottom_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
		else if(check_cross_center(sprite_rect))
		{//压在十字叉上了
			if(!flag)
			{//不在祖先身上	
				bool find = checkt_and_remove1(sprite);
				if(find)
				{//在自己身上找到了这个sprite
					if(p_left_top_child != NULL)
						p_left_top_child->RemoveSprite(sprite,true);
					if(p_right_top_child != NULL)
						p_right_top_child->RemoveSprite(sprite,true);
					if(p_left_bottom_child != NULL)
						p_left_bottom_child->RemoveSprite(sprite, true);
					if(p_right_bottom_child != NULL)
						p_right_bottom_child->RemoveSprite(sprite, true);
				}
				else
				{//也不在自己身上
					bool find1 = false;
					bool find2 = false;
					bool find3 = false;
					bool find4 = false;
					if(p_left_top_child != NULL)
						find1 = p_left_top_child->RemoveSprite(sprite,flag);
					if(p_right_top_child != NULL)
						find2 = p_right_top_child->RemoveSprite(sprite,flag);
					if(p_left_bottom_child != NULL)
						find3 = p_left_bottom_child->RemoveSprite(sprite, flag);
					if(p_right_bottom_child != NULL)
						find4 = p_right_bottom_child->RemoveSprite(sprite, flag);
					find = find1 || find2 || find3 || find4;
				}
				return find;
			}
			else
			{//在祖先身上
				if(p_left_top_child != NULL)
					p_left_top_child->RemoveSprite(sprite,flag);
				if(p_right_top_child != NULL)
					p_right_top_child->RemoveSprite(sprite,flag);
				if(p_left_bottom_child != NULL)
					p_left_bottom_child->RemoveSprite(sprite, flag);
				if(p_right_bottom_child != NULL)
					p_right_bottom_child->RemoveSprite(sprite, flag);
				return true;
			}
		}
	}
	return false;
}

void CDexQuad::stQuadNode::InitChildInfo(stQuadNode* child)
{
	child->p_father = this;
	child->p_quad_tree = p_quad_tree;
	child->level = level + 1;
	setChildRect(child);
}
void CDexQuad::stQuadNode::setChildRect(stQuadNode* child)
{
	if(child == p_left_top_child)
	{
		child->rect.left  = rect.left;
		child->rect.right = rect.left + rect.getWidth()/2;
		child->rect.top   = rect.top;
		child->rect.bottom= rect.top + rect.getHeight()/2; 
	}
	else if(child == p_left_bottom_child)
	{
		child->rect.left  = rect.left;
		child->rect.right = rect.left + rect.getWidth()/2;
		child->rect.top   = rect.top + rect.getHeight()/2;
		child->rect.bottom= rect.bottom; 
	}
	else if(child == p_right_top_child)
	{
		child->rect.left  = rect.left + rect.getWidth()/2;
		child->rect.right = rect.right;
		child->rect.top   = rect.top;
		child->rect.bottom= rect.top + rect.getHeight()/2; 
	}
	else if(child == p_right_bottom_child)
	{
		child->rect.left  = rect.left + rect.getWidth()/2;
		child->rect.right = rect.right;
		child->rect.top   = rect.top + rect.getHeight()/2;
		child->rect.bottom= rect.bottom; 
	}
}
void CDexQuad::stQuadNode::Update(float delta, int mousex, int mousey)
{
	if(p_left_top_child != NULL)
		p_left_top_child->Update(delta,mousex,mousey);
	if(p_right_top_child != NULL)
		p_right_top_child->Update(delta,mousex,mousey);
	if(p_left_bottom_child != NULL)
		p_left_bottom_child->Update(delta,mousex,mousey);
	if(p_right_bottom_child != NULL)
		p_right_bottom_child->Update(delta,mousex,mousey);

	bool find = false;
	{
		foreach(list<CTestSprite*>, ite, list_sprites)
		{
			if(*ite != NULL)
			{
				if(CDexFun::check_point_in_rect(DexPointF(mousex,mousey), (*ite)->m_rect))
				{
					find = true;
					break;
				}
			}
		}
	}

	if(!find)
	{
		//foreach(list<CTestSprite*>, ite, list_collide)
		//{
		//	if(*ite != NULL)
		//	{
		//		if(CDexFun::check_point_in_rect(DexPointF(mousex,mousey), (*ite)->m_rect))
		//		{
		//			find = true;
		//			break;
		//		}
		//	}
		//}
	}
	if(find)
	{
		color = 0xffff0000;
		width = 3;
	}
	else
	{
		color = 0xffffff00;
		width = 1;
	}
	{
		list<CTestSprite*>::iterator ite2;
		for(list<CTestSprite*>::iterator ite = list_sprites.begin(); ite != list_sprites.end();)
		{
			ite2 = ++ite; //先保存迭代器的下一个指针
			ite--;        //将迭代器指回来
			if(*ite != NULL)
			{
				for(list<CTestSprite*>::iterator ite3 = list_sprites.begin(); ite3 != list_sprites.end();ite3++)
				{
					if(*ite == *ite3)
						continue;
					if(CDexFun::check_rect_croos((*ite)->m_rect, (*ite3)->m_rect))
					{
						(*ite)->m_collide = true;
						break;
					}
					else
					{
						(*ite)->m_collide = false;
					}
				}
				if(!(*ite)->m_collide)
				{//没有检测到内部的 检测压线的
					for(list<CTestSprite*>::iterator ite3 = list_collide.begin(); ite3 != list_collide.end();ite3++)
					{
						if(CDexFun::check_rect_croos((*ite)->m_rect, (*ite3)->m_rect))
						{
							(*ite)->m_collide = true;
							//OutputDebugStr("检测到与父亲的压线物体碰撞\n");
							break;
						}
						else
						{
							(*ite)->m_collide = false;
						}
					}
				}
				(*ite)->Render();
				(*ite)->Update(delta,mousex,mousey);
			}
			ite = ite2;//因为update操作有可能造成迭代器失效 用ite2赋值
		}
	}
}
void CDexQuad::stQuadNode::Render()
{
	//if(isLeafNode())
		
	//if(p_left_top_child != NULL)
	//	p_left_top_child->Render();
	//if(p_right_top_child != NULL)
	//	p_right_top_child->Render();
	//if(p_left_bottom_child != NULL)
	//	p_left_bottom_child->Render();
	//if(p_right_bottom_child != NULL)
	//	p_right_bottom_child->Render();	

	//get2DDrawer()->DrawRectLine(rect, color, width);
	//foreach(list<CTestSprite*>, ite, list_sprites)
	//{
	//	if(*ite != NULL)
	//		(*ite)->Render();
	//}
}
CDexQuad::CDexQuad(DexRectF rect, int level)
{
	m_level = level;
	getQuadNodePool()->CreateNode(pow(4.0f,level)/2);
	m_pRoot = getQuadNodePool()->GetNode();
	m_pRoot->p_quad_tree = this;
	m_pRoot->rect = rect;

}
CDexQuad::~CDexQuad()
{
}
bool CDexQuad::AddSprite(CTestSprite* sprite)
{
	DEX_ENSURE_B(m_pRoot && sprite);
	sprite->m_pQuadTree = this;
	sprite->m_rectInsert = sprite->m_rect;
	return m_pRoot->AddSprite(sprite, false);
}

bool CDexQuad::RemoveSprite(CTestSprite* sprite)
{
	DEX_ENSURE_B(m_pRoot);
	m_pRoot->RemoveSprite(sprite, false);
	return true;
}
void CDexQuad::Update(float delta, int mousex, int mousey)
{
	DEX_ENSURE(m_pRoot);
	m_pRoot->Update(delta,mousex,mousey);
}
void CDexQuad::Render()
{
	m_pRoot->Render();
}



CDexQuadNodePool::CDexQuadNodePool()
{

}
CDexQuadNodePool::~CDexQuadNodePool()
{

}

void CDexQuadNodePool::CreateNode(int number)
{	
	int64 time1 = getTime()->GetTotalMillSeconds();
	for(int i = 0; i < number; i++)
	{
		CDexQuad::stQuadNode* newNode = new CDexQuad::stQuadNode;
		m_nodeList.push_back(newNode);
	}
	getLog()->BeginLog();
	getLog()->Log(log_ok, "四叉树申请%d个节点...，耗时%d ms", number, getTime()->GetTotalMillSeconds()- time1);
	getLog()->EndLog();
}

void CDexQuadNodePool::Release()
{
	foreach(std::list<CDexQuad::stQuadNode*>, ite, m_nodeList)
	{	 
		if(*ite != NULL)
		{
			delete *ite;
			*ite = NULL;
			ite = m_nodeList.erase(ite);
		}
	}
}


CDexQuad::stQuadNode* CDexQuadNodePool::GetNode()
{
	CDexQuad::stQuadNode* node = NULL;
	if(m_nodeList.size() == 0)
	{
		node = new CDexQuad::stQuadNode;
		return node;
	}
	else
	{
		node = *(m_nodeList.begin());
		m_nodeList.pop_front();
		return node;
	}

}

void CDexQuadNodePool::PushNode(CDexQuad::stQuadNode* node)
{
	DEX_ENSURE(node);
	m_nodeList.push_back(node);
}

CDexQuadNodePool* getQuadNodePool()
{
	static CDexQuadNodePool* g_pNodePool = NULL ;
	if(g_pNodePool == NULL)
		g_pNodePool = new CDexQuadNodePool;
	return g_pNodePool;
}