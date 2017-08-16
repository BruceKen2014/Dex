

#include "DexWidget.h"
#include "DexWidgetContainer.h"

CDexWidgetContainer::CDexWidgetContainer()
{
	m_WidgetList.clear();
	m_totalCount = 0;
	m_onwer = NULL;
}
CDexWidgetContainer::~CDexWidgetContainer()
{
	//foreach( std::list<CDexWidget*>, it, m_WidgetList)
	//{
	//	if(*it != NULL)
	//	{
	//		UI_DROP_REF(*it);
	//		if(*it == NULL) //�п����ѱ����� �����ж��� ��ֹ����Ұָ��
	//			it = m_WidgetList.begin();
	//	}
	//}
}

void CDexWidgetContainer::Hide()
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->Hide();  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}
void CDexWidgetContainer::Update(int delta)
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->Update(delta);  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}
void CDexWidgetContainer::Render()
{
	//��Ⱦ ����ǰ������Ⱦ��ԭ��(��Ϊ������ܻḲ��ǰ���)
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->Render();  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}
void CDexWidgetContainer::Enable(bool child /* = false */)
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->Enable(child);  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}
void CDexWidgetContainer::Disable(bool child /* = false */)
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->Disable(child);  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}
bool CDexWidgetContainer::OnEvent(stEvent event)
{
	//�¼����� ���Ӻ�ǰ��ԭ��(��Ϊ������ܻ��ǰ����ȽӴ����¼�)
	DEX_ENSURE_B(m_WidgetList.size() != 0);
	bool deal = false;
	DexWidget* wdt = m_WidgetList.back();
	do 
	{
		deal = wdt->OnEvent(event);  
		if(deal)
		{//�Ѿ�������������󴫵��¼�
			break;
		}
		wdt = wdt->m_left_sibling;
	} while (wdt != NULL);
	return deal;
}

void CDexWidgetContainer::OnFatherSetAlpha(int alpha)
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		float alphaPercent = ((float)wdt->GetInitAlpha())/wdt->GetFather()->GetInitAlpha();
		wdt->SetAlpha(alpha*alphaPercent, true);  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}

void CDexWidgetContainer::Offset(const DexGUI::DexPoint& delta)
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->OffSet(delta);  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}

void CDexWidgetContainer::Offset(const DexGUI::DexPointF& delta)
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->OffSet(delta);  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}

void CDexWidgetContainer::Scale(float x, float y)
{
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		wdt->Scale(x, y, true, true);  
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}
void CDexWidgetContainer::SetOnwer(DexWidget* onwer)
{
	m_onwer = onwer;
}

void CDexWidgetContainer::_UpdateTotalCounts()
{
	m_totalCount = 0;
	DEX_ENSURE(m_WidgetList.size() != 0);
	DexWidget* wdt = m_WidgetList.front();
	do 
	{
		m_totalCount ++;  //����Ҫ��һ��
		m_totalCount += wdt->m_children->GetTotalWidgetCount();
		wdt = wdt->m_right_sibling;
	} while (wdt != NULL);
}

bool CDexWidgetContainer::Empty()
{
	return m_WidgetList.empty();
}

int CDexWidgetContainer::GetThisWidgetCount()
{
	return (int)m_WidgetList.size();
}

int CDexWidgetContainer::GetTotalWidgetCount()
{
	return m_totalCount;
}

void CDexWidgetContainer::AddWidgetToFront(DexWidget* widget)
{
	DEX_ENSURE(widget);
	//UI_ADD_REF(widget); 
	widget->SetFather(m_onwer);
	if(!m_WidgetList.empty())
	{
		(m_WidgetList.front())->SetLeftSibling(widget);
		widget->SetRightSibling(m_WidgetList.front());
	}

	m_WidgetList.push_front(widget);
	_UpdateTotalCounts();
}

void CDexWidgetContainer::AddWidgetToBack(DexWidget* widget, bool asChild)
{
	DEX_ENSURE(widget);
	//UI_ADD_REF(widget);
	if(asChild && m_onwer != NULL)
		widget->SetFather(m_onwer);
	if(!m_WidgetList.empty())
	{
		(m_WidgetList.back())->SetRightSibling(widget);
		widget->SetLeftSibling(m_WidgetList.back());
	}
	m_WidgetList.push_back(widget);
	_UpdateTotalCounts();
}

void CDexWidgetContainer::InsertWidgetFront(DexWidget* wait_insert, int id)
{
	DEX_ENSURE(wait_insert);
	wait_insert->SetFather(m_onwer);
	if(m_WidgetList.empty())
	{
		AddWidgetToBack(wait_insert);
	}
	else
	{
		foreach( std::list<DexWidget*>, it, m_WidgetList)
		{
			if((*it)->GetId() == id)
			{
				DexWidget* left = (*it)->m_left_sibling;
				m_WidgetList.insert(it, wait_insert);
				//����֮��Ҫ���֮ǰ�ڵ�ͱ��ڵ��ǰ��ƥ������
				(*it)->SetLeftSibling(wait_insert);
				wait_insert->SetRightSibling(*it);
				wait_insert->SetLeftSibling(left);
				break;
			}
		}
	}
	_UpdateTotalCounts();

}
void CDexWidgetContainer::InsertWidgetBack(DexWidget* wait_insert, int id)
{
	DEX_ENSURE(wait_insert);
	wait_insert->SetFather(m_onwer);
	if(m_WidgetList.empty())
	{
		AddWidgetToBack(wait_insert);
	}
	else
	{
		foreach( std::list<DexWidget*>, it, m_WidgetList)
		{
			if((*it)->GetId() == id)
			{//�ҵ��� ����������֮����� ��˻�Ҫit++һ��
				it++;
				if(it == m_WidgetList.end())
				{//��������
					AddWidgetToBack(wait_insert);
				}
				else
				{
					DexWidget* left = (*it)->m_left_sibling;
					m_WidgetList.insert(it, wait_insert);
					//����֮��Ҫ���֮ǰ�ڵ�ͱ��ڵ��ǰ��ƥ������
					(*it)->SetLeftSibling(wait_insert);
					wait_insert->SetRightSibling(*it);
					wait_insert->SetLeftSibling(left);
				}
				break;
			}
		}
	}
	_UpdateTotalCounts();
}