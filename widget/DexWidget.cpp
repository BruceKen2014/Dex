


//#include "stdafx.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/CInput.h"
#include "../DexBase/DexLog.h"
#include "../sound/CSoundSystem.h"

#include "DexWidget.h"
#include "DexWidgetContainer.h"
#include "DexDesktop.h"
#include "DexGuiAction.h"
#include "DexWidgetFactory.h"


DexWidget::DexWidget()
{
	m_bClipByParent = true;
	m_enable = true;
	m_visible = true;
	m_mask = false;
	m_mouseOn = false;
	m_maskColor = 0xffffffff;
	m_flag = mouse_l_down;
	m_Drawflag = DexGUI::DRAW_BOTTOM_LEFT;
	m_type = widget_base;
	m_father = NULL;
	m_children = new CDexWidgetContainer();
	m_children->SetOnwer(this);
	m_left_sibling = NULL;
	m_right_sibling = NULL;
	m_alpha = 0xffffffff;
	m_initAlpha = 255;
	m_rotate = 0.0f;
	Enable();
	CInputSystem::GetInputSingleton().RegisterMouseListener(this);
}

DexWidget::~DexWidget()
{
	_SafeDelete(m_children);
}

void DexWidget::ShutDown()
{
	CInputSystem::GetInputSingleton().UnRegisterMouseListener(this);

	//���widget����action Ҫ�ͷ�
	for(list<CDexGuiAction*>::iterator it = m_actions.begin();
		it != m_actions.end();)
	{
		if(*it)
		{
			_SafeDelete(*it);
			m_actions.erase(it);
			it = m_actions.begin();
			continue;
		}
	}
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_ok, "�ؼ�:%s ����", m_name.c_str());
	DexLog::getSingleton()->EndLog();
	getWidgetFactory()->removeWidget(m_name);
}
void DexWidget::Show()
{
	m_visible = true;
}
void DexWidget::Hide()
{
	m_visible = false;
}

bool DexWidget::GetFocus() const
{
	return getDesktop()->getFocusWgt() == this;
}

void DexWidget::SetFocus(bool focus)
{
	if(focus)
	{
		getDesktop()->SetFocusWgt(this);
	}
	else
	{
		if(getDesktop()->getFocusWgt() == this)
		{
			getDesktop()->SetFocusWgt(NULL);
		}
	}
}

bool DexWidget::GetSelect() const
{
	return getDesktop()->getSelectWgt() == this;
}

void DexWidget::SetSelect(bool select)
{
	if(select)
	{
		getDesktop()->SetSelectWgt(this);
	}
	else
	{
		if(getDesktop()->getSelectWgt() == this)
		{
			getDesktop()->SetSelectWgt(NULL);
		}
	}
}

bool DexWidget::Update(int delta)
{
	DEX_ENSURE_B(DexObject::Update(delta));
	for(list<CDexGuiAction*>::iterator it = m_actions.begin();
		it != m_actions.end();)
	{
		if(*it)
		{
			(*it)->Update(delta);
		}
		if(*it == NULL) //����������ɾ�����������*itΪ�գ�
			//������ж�ֱ�ӽ���it++ ����ɵ���������
		{
			m_actions.erase(it);
			it = m_actions.begin();
			continue;
		}
		it++;

	}
	if(m_bClipByParent)
	{
		//TODO:��Ҫÿ֡���ã�����״λ�÷����ı�ʱ����
		if( m_father != NULL)
		{
			DexRectF father_rect = m_father->GetArea();
			m_rectAfterClip = m_rect.Clip(father_rect);
		}
		else
		{
			m_rectAfterClip = m_rect;
		}

	}
	m_children->Update(delta);
	return DexObject::Update(delta);
}

void DexWidget::Create()
{

}

void DexWidget::Render()
{
	DEX_ENSURE(GetVisible());
	RenderThis();
	if(getDesktop()->m_bRenderRect)
		RenderRect();
	if(m_mask) 
		RenderMask();
	m_children->Render();
}
void DexWidget::RenderThis()
{
}
bool DexWidget::CheckPointIn(const DexPoint& pt)
{
	return pt.x >= m_rect.left && pt.x <= m_rect.right
		&& pt.y >= m_rect.top  && pt.y <= m_rect.bottom;
}

void DexWidget::ReCalculateUV(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3, const DexRectF& srcRect, const DexRectF& clipRect)
{
	DEX_ENSURE(m_bClipByParent);
	/*
	uv0_____________uv1
	|	_____	|
	|	|	|	|
	|	|___|	|
	uv3|___________|uv2
	��Ȧ��������Դ���Σ��ھ����ǲü�֮��ľ��Σ�ͨ������μ����ھ��ε�uv
	*/
	float image_u_length = uv2.x - uv0.x;
	float image_v_length = uv2.y - uv0.y;
	DexVector2 uv0_clip(uv0), uv1_clip(uv1), uv2_clip(uv2), uv3_clip(uv3);
	float real_width = srcRect.getWidth();
	float real_height = srcRect.getHeight();
	if (clipRect.left > srcRect.left)
	{
		float clip_length = clipRect.left - srcRect.left;
		float clip_rate = clip_length / real_width;
		float clip_u = image_u_length * clip_rate;
		uv0.x += clip_u;
	}
	if (clipRect.top > srcRect.top)
	{
		float clip_length = clipRect.top - srcRect.top;
		float clip_rate = clip_length / real_height;
		float clip_v = image_v_length * clip_rate;
		uv0.y += clip_v;
	}
	if (clipRect.right < srcRect.right)
	{
		float clip_length = srcRect.right - clipRect.right;
		float clip_rate = clip_length / real_width;
		float clip_u = image_u_length * clip_rate;
		uv2.x -= clip_u;
	}
	if (clipRect.bottom < srcRect.bottom)
	{
		float clip_length = srcRect.bottom - clipRect.bottom;
		float clip_rate = clip_length / real_height;
		float clip_v = image_v_length * clip_rate;
		uv2.y -= clip_v;
	}
	uv1.x = uv2.x;
	uv1.y = uv0.y;
	uv3.x = uv0.x;
	uv3.y = uv2.y;
}

void DexWidget::EraseAction(CDexGuiAction* action)
{
	foreach(list<CDexGuiAction*>, it, m_actions)
	{
		if(*it == action)
		{
			_SafeDelete(*it);
			//m_actions.erase(it); ��������erase ��update��erase
			
			break;
		}
	}
}
void DexWidget::StopMoveAction()
{
	foreach(list<CDexGuiAction*>, it, m_actions)
	{
		if((*it)->GetActionType() == EAT_MOVE)
		{
			(*it)->ConstrainEnd();
		}
	}
}

bool DexWidget::OnMouseMove(stEvent event)
{ 
	bool ret = false;
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	if(CheckPointIn(DexPoint(posX.i_member, posY.i_member)))
	{//����ڿؼ�������
		if(!m_mouseOn)
		{//֮ǰ��겻�����棬���ǵ�һ�ν���
			OnMouseMoveIn(event);
			m_MouseMoveIn.Handle(this, event);
		}
		m_MouseMove.Handle(this, event);
		ret = true;
	}
	else
	{//��겻�ڿؼ�������
		if(m_mouseOn)
		{//֮ǰ��������棬�����Ƴ��¼�
			OnMouseMoveOut(event);
			m_MouseMoveOut.Handle(this, event);
			//ret = true; �����ث@�Ƴ��¼�������Ƴ�ȥ�������όӸ��H�����^�m푑�
		}
	}
	return ret;
}

void DexWidget::OnMouseMoveIn(stEvent event)
{
	m_mouseOn = true;
	SetFocus(true);
	m_MouseMoveIn.Handle(this, event);
	//getDesktop()->SetUiEvent(false); �Ƴ����M�п����漰���ɂ��ؼ� ��˲��ث@
	if(m_mouseOnSoundFilename != "")
		CSound::getSoundSingleton().Play((char*)m_mouseOnSoundFilename.c_str(), 1);
}
void DexWidget::OnMouseMoveOut(stEvent event)
{
	m_mouseOn = false;
	SetFocus(false);
	m_MouseMoveOut.Handle(this, event);
	//getDesktop()->SetUiEvent(false);// �����ث@�Ƴ��¼�������Ƴ�ȥ�������όӸ��H�����^�m푑�
}
bool DexWidget::OnMouseLUp(stEvent event)
{
	m_MouseLUp.Handle(this, event);
	getDesktop()->SetUiEvent(false);
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_ok, "�ؼ�%s���������������¼�", m_name.c_str());
	DexLog::getSingleton()->EndLog();
	return true;
}
void DexWidget::OnMouseLDown(stEvent event)
{	
	SetSelect(true);
	m_MouseLDown.Handle(this, event);
	getDesktop()->SetUiEvent(false);

	//getDesktop()->SetUiEvent(false);
	//DexLog::getSingleton()->BeginLog();
	//DexLog::getSingleton()->Log(log_ok, "�ؼ�%s���������������¼�", m_name.c_str());
	//DexLog::getSingleton()->EndLog();
}
void DexWidget::OnMouseRUp(stEvent event)
{
	m_MouseRUp.Handle(this, event);
	getDesktop()->SetUiEvent(false);
}
void DexWidget::OnMouseRDown(stEvent event)
{
	m_MouseRDown.Handle(this, event);
	getDesktop()->SetUiEvent(false);
}
void DexWidget::OnMouseMUp(stEvent event)
{
	m_MouseMUp.Handle(this, event);
	getDesktop()->SetUiEvent(false);
}
void DexWidget::OnMouseMDown(stEvent event)
{
	m_MouseMDown.Handle(this, event);
	getDesktop()->SetUiEvent(false);
}
void DexWidget::OnMouseWheelFront(stEvent event)
{
	m_MouseWheelFront.Handle(this, event);
	getDesktop()->SetUiEvent(false);
}
void DexWidget::OnMouseWheelBack(stEvent event)
{
	m_MouseWheelBack.Handle(this, event);
	getDesktop()->SetUiEvent(false);
}
void DexWidget::OnMouseLDrag(stEvent event)
{	
	m_MouseLDrag.Handle(this, event);
	getDesktop()->SetDragingWgt(this);
}
void DexWidget::OnMouseRDrag(stEvent event)
{
	m_MouseRDrag.Handle(this, event);
	getDesktop()->SetDragingWgt(this);
}

void DexWidget::OnKeyDown(stEvent event)
{

}
void DexWidget::OnKeyChar(stEvent event)
{

}

bool DexWidget::MouseMoveValid(stEvent event)
{	
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_move);
	
	return true;
}
bool DexWidget::MouseLUpValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_l_up);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseLDownValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	if(!GetEable())
	{
		if(m_mouseDownUnvalid != "")
			CSound::getSoundSingleton().Play((char*)m_mouseDownUnvalid.c_str(), 1);
		return false;
	}

	DEX_ENSURE_B(m_flag & mouse_l_down);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseRUpValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_r_up);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseRDownValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_r_down);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseMUpValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_m_up);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseMDownValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_m_down);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseWheelFrontValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_wheel_f);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseWheelBackValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_wheel_b);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseLDragValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_l_drag);
	DEX_ENSURE_B(getDesktop()->getDragingWgt() == NULL);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}
bool DexWidget::MouseRDragValid(stEvent event)
{
	DEX_ENSURE_B(getDesktop()->getUiEvent()); 
	DEX_ENSURE_B(GetVisible());
	DEX_ENSURE_B(GetEable());
	DEX_ENSURE_B(m_flag & mouse_r_drag);
	DEX_ENSURE_B(getDesktop()->getDragingWgt() == NULL);
	stArgs posX, posY; 
	event.pop_args(posY);
	event.pop_args(posX);
	return CheckPointIn(DexPoint(posX.i_member, posY.i_member));
}




bool DexWidget::OnEvent(stEvent event)
{
	bool visible = GetVisible();
	DEX_ENSURE_B(getDesktop()->getUiEvent()); //ȷ�����¼����Դ���
	DEX_ENSURE_B(visible);
	DEX_ENSURE_B(GetFlag(catch_event)); //���һ���ؼ�����Ӧ�¼� ��ô�����ӿؼ�Ҳ����Ӧ�¼�
	bool child_deal_event = m_children->OnEvent(event); //������ӿؼ� ��ݹ鵽�ӿؼ����д���
	                            //��һ��ͼƬ����һ���Ӱ�ť�����������ĵ����Χ�ڰ�ť��ͼƬ�Ĺ�ͬ�����ڣ������ȴ���ť
	if(child_deal_event)
	{
		//DexLog::getSingleton()->BeginLog();
		//DexLog::getSingleton()->Log(log_ok,"�¼����ؼ�%s�ӿؼ��������ؼ����ٴ���\n", GetName().c_str());
		//DexLog::getSingleton()->EndLog();
		return true;
	}
	bool deal = false;
	switch(event.id)
	{
	case EVENT_ID_MOUSE_MOVE:
		{
			if(MouseMoveValid(event))
			{
				deal = OnMouseMove(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_L_UP:
		{
			if(MouseLUpValid(event))
			{
				deal = OnMouseLUp(event);
				//if(deal)
				//{
				//	DexLog::getSingleton()->BeginLog();
				//	DexLog::getSingleton()->Log(log_ok,"�ؼ�%s�����¼����I����\n", GetName().c_str());
				//	DexLog::getSingleton()->EndLog();
				//}
			}
			break;
		}
	case EVENT_ID_MOUSE_L_DOWN:
		{	
			if(MouseLDownValid(event))
			{
				deal = true;
				OnMouseLDown(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_R_UP:
		{
			if(MouseRUpValid(event))
			{
				deal = true;
				OnMouseRUp(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_R_DOWN:
		{
			if(MouseRDownValid(event))
			{
				deal = true;
				OnMouseRDown(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_M_UP:
		{
			if(MouseMUpValid(event))
			{
				deal = true;
				OnMouseMUp(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_M_DOWN:
		{
			if(MouseMDownValid(event))
			{
				deal = true;
				OnMouseMDown(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_WHEEL_F:
		{
			if(MouseWheelFrontValid(event))
			{
				deal = true;
				OnMouseWheelFront(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_WHEEL_B:
		{
			if(MouseWheelBackValid(event))
			{
				deal = true;
				OnMouseWheelBack(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_L_DRAG:
		{
			if(MouseLDragValid(event))
			{
				deal = true;
				OnMouseLDrag(event);
			}
			break;
		}
	case EVENT_ID_MOUSE_R_DRAG:
		{
			if(MouseRDragValid(event))
			{
				deal = true;
				OnMouseRDrag(event);
			}
			break;
		}
	default:
		break;
	}
	return deal;
}
bool DexWidget::EventValid(stEvent event)
{
	return false;
}
void DexWidget::RenderRect()
{
	if (getDesktop()->getSelectWgt() == this)
		get2DDrawer()->DrawRectLine(DexRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), DEXCOLOR_RED);
	else if (getDesktop()->getFocusWgt() == this)
		get2DDrawer()->DrawRectLine(DexRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), DEXCOLOR_YELLOW);
	else
		get2DDrawer()->DrawRectLine(DexRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom), DEXCOLOR_GREEN);
}
void DexWidget::RenderMask()
{
	get2DDrawer()->SetRectColor(m_maskColor);
	get2DDrawer()->DrawRect(m_rect);
}

int DexWidget::GetId()
{
	return m_id;
}
DexPointF DexWidget::GetPos()
{
	return DexPointF(m_rect.left, m_rect.top);
}

DexRectF DexWidget::GetArea()
{
	if(m_bClipByParent && m_father != NULL)
		return m_rectAfterClip;
	return m_rect;
}

DexRectF DexWidget::GetAreaReal()
{
	return m_rect;
}

bool DexWidget::GetVisible()
{
	if(m_father != NULL && !m_father->GetVisible()) //�ݹ鵽���ϲ� 
		return false;
	return m_visible;
}

void DexWidget::SetClipByParent(bool clipByParent)
{
	m_bClipByParent = clipByParent;
}

bool DexWidget::GetClipByParent()
{
	return m_bClipByParent;
}

void DexWidget::setVisible(bool visible)
{
	m_visible = visible;
}

CDexWidgetContainer* DexWidget::GetChildrenContainer() 
{
	return m_children;
}
void DexWidget::AddAction(CDexGuiAction* action)
{
	DEX_ENSURE(action);
	action->SetOwner(this);
	m_actions.push_back(action);
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_ok, "�ؼ�%s��Ӷ���%s", m_name.c_str(), action->GetName().c_str());
	DexLog::getSingleton()->EndLog();
}
void DexWidget::ModifyFlag( EModiFyType i_o, int flag )
{
	if(i_o == Add_Flag)
	{
		m_flag |= flag;
	}
	else if(i_o == Minus_Flag)
	{
		if(m_flag & flag)
			m_flag = m_flag^flag;
	}
}
void DexWidget::Enable(bool child)
{
	ModifyFlag(Add_Flag, catch_event);
	if(child)
		m_children->Enable(true);
}
void DexWidget::Disable(bool child)
{
	ModifyFlag(Minus_Flag, catch_event);
	if(child)
		m_children->Disable(true);
}

void DexWidget::setMouseOnFilename(string filename)
{
	m_mouseOnSoundFilename = filename;
}
void DexWidget::setMouseDownFilename(string filename)
{
	m_mouseDownSoundFilename = filename;
}
void DexWidget::setMouseDownUnvalid(string filename)
{
	m_mouseDownUnvalid = filename;
}

void DexWidget::setDrawFlag(EDraw_Flag flag)
{
	m_Drawflag = flag;
	switch(flag)
	{
	case DexGUI::DRAW_TOP_CENTER:
		{
			int offsetX = m_rect.getWidth()/2;
			m_rect.left -= offsetX;
			m_rect.right -= offsetX;
		}
	}
}

float DexWidget::GetWidth()
{
	return m_rect.right - m_rect.left;
}

float DexWidget::GetHeight()
{
	return m_rect.bottom - m_rect.top;
}

void DexWidget::SetRotate(float rotate)
{
	m_rotate = rotate;
}
float DexWidget::GetRotate()
{
	return m_rotate;
}

DexPointF DexWidget::GetSize()
{
	return DexPointF(GetWidth(), GetHeight());
}

string DexWidget::GetName()
{
	return m_name;
}
bool DexWidget::GetEable()
{
	return GetFlag(catch_event);
}
bool DexWidget::GetFlag(int flag)
{
	return m_flag & flag;
}

DexWidget* DexWidget::GetFather()
{
	return m_father;
}
int DexWidget::GetWidgetFlag()
{
	return m_flag;
}

void DexWidget::SetWidgetFlag(int flag)
{
	m_flag = flag;
}
void DexWidget::SetId(int id)
{
	m_id = id;
}
void DexWidget::SetName(string name)
{ 
	m_name = name;
}

void DexWidget::SetFather(DexWidget* widget) 
{ 
	DEX_ENSURE(widget);
	m_father = widget;
	//������Ը������ƫ����
	m_offFatherPos =  GetPos() - m_father->GetPos();
}

void DexWidget::SetPos(const DexPoint& pos)
{
	DexPointF temp;
	temp.x = pos.x;
	temp.y = pos.y;
	DexPointF delta = temp - GetPos();
	OffSet(delta);
	if(m_father != NULL)
		m_offFatherPos =  GetPos() - m_father->GetPos();
	EDSetPos.Handle(this, stEvent());
}
void DexWidget::SetPos(const DexPointF& pos)
{
	DexPointF delta = pos - GetPos();
	OffSet(delta);
	if(m_father != NULL)
		m_offFatherPos =  GetPos() - m_father->GetPos();
	EDSetPos.Handle(this, stEvent());
}

void DexWidget::setFatherOffset(float x, float y)
{
	m_offFatherPos.x = x;
	m_offFatherPos.y = y;
	float width = m_rect.right - m_rect.left;
	float height = m_rect.bottom - m_rect.top;
	m_rect.left  = m_father->GetArea().left + x;
	m_rect.right = m_rect.left + width;
	m_rect.top   = m_father->GetArea().top + y;
	m_rect.bottom = m_rect.top + height;
	m_children->Offset(DexPointF(x,y));
}

DexPointF DexWidget::getFatherOffset()
{
	return m_offFatherPos;
}

void DexWidget::SetPos(int x, int y)
{
	SetPos(DexPoint(x, y));
}

void DexWidget::OpenMask(bool open)
{
	m_mask = open;
}

void DexWidget::SetMaskColor(DUChar r, DUChar g, DUChar b)
{
	m_maskColor.Set(r,g,b);
}

void DexWidget::SetMaskAlpha(DUChar a)
{
	m_maskColor.a = a;
}

void DexWidget::SetMaskColor(const DexColor& color)
{
	m_maskColor = color;
}
void DexWidget::SetMaskColor(DUChar r, DUChar g, DUChar b, DUChar a/* =125 */)
{
	SetMaskAlpha(a);
	SetMaskColor(r,g,b);
}

void DexWidget::SetInitAlpha(int alpha)
{
	m_initAlpha = alpha;
	SetAlpha(m_initAlpha);
}

int DexWidget::GetInitAlpha()
{
	return m_initAlpha;
}
void DexWidget::SetAlpha(int alpha, bool children)
{
	if(alpha > 255)
		alpha = 255;
	if(alpha < 0)
		alpha = 0;
	m_alpha &= 0x00ffffff;
	m_alpha |= ((int)alpha)<<24;
	if(children)
		m_children->OnFatherSetAlpha(alpha);
}

int DexWidget::GetOnlyAlpha()
{
	return m_alpha>>24;
}

void DexWidget::OffSet(const DexPoint& delta)
{
	m_rect.left += delta.x;
	m_rect.right += delta.x;
	m_rect.top += delta.y;
	m_rect.bottom += delta.y;
	if(m_father != NULL)
		m_offFatherPos = GetPos() - m_father->GetPos();
	//��widget���õ�ָ��λ��֮�󣬻�Ҫ����widget�µ�������widget��������Ӧ��ƽ��
	m_children->Offset(delta);
}

void DexWidget::OffSet(const DexPointF& delta)
{
	m_rect.left += delta.x;
	m_rect.right += delta.x;
	m_rect.top += delta.y;
	m_rect.bottom += delta.y;
	//��widget���õ�ָ��λ��֮�󣬻�Ҫ����widget�µ�������widget��������Ӧ��ƽ��
	m_children->Offset(delta);
}
void DexWidget::Scale(float x, float y, bool scaleChild, bool changePos)
{
	if(changePos)
	{
		if(m_father != NULL)
		{
			DexPointF Pos = GetPos();
			if(x > 1.0f)
			{//x�Ŵ�
				Pos.x += m_offFatherPos.x*(x-1.0f);
			}
			else
				Pos.x -= m_offFatherPos.x*(1.0f-x);
			if(y > 1.0f)
			{//y�Ŵ�
				Pos.y += m_offFatherPos.y*(y-1.0f);
			}
			else
				Pos.y -= m_offFatherPos.y*(1.0f-y);
			SetPos(Pos);
		}

	}
	float old_width = m_rect.right - m_rect.left;
	float old_height = m_rect.bottom - m_rect.top;
	float new_width = old_width*x;
	float new_height = old_height*y;

	Resize(DexSize(new_width, new_height), scaleChild);
}
void DexWidget::Resize(const DexSize& size, bool resizeChild)
{
	Resize(DexSizeF(size.w, size.h), resizeChild);
}

void DexWidget::Resize(const DexSizeF& size, bool resizeChild)
{
	float old_width = m_rect.right - m_rect.left;
	float old_height = m_rect.bottom - m_rect.top;
	//m_rect.right = m_rect.left + old_width + size.x;
	//m_rect.bottom = m_rect.top + old_height + size.y;
	float xScale = size.w/old_width;
	float yScale = size.h/old_height;
	m_rect.right = m_rect.left + size.w;
	m_rect.bottom = m_rect.top + size.h;
	if(resizeChild)
	{//���ؼ����ô�С���Ӱ���ӿؼ������ӿؼ��ڸı�ߴ��ͬʱ
		//Ҫ��������ԭ����pos����һ����ƽ�� ʹ����������ȥ���������ŵ�
		if(old_width != 0 && old_height != 0)
			m_children->Scale(xScale, yScale);
	}
	m_rectAfterClip = m_rect;
	EDResize.Handle(this, stEvent());
}

void DexWidget::Resize(DInt32 width, DInt32 height, DBool resizeChild /* = false */)
{
	Resize(DexSizeF(width, height), resizeChild);
}