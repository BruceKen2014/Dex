

#pragma once

#include "../DexBase/CListener.h"
class DexWidget;
class CDexWidgetContainer
{
protected:
	std::list<DexWidget*> m_WidgetList; //ֱ��widget
	int m_totalCount; //�ܹ���widget����
	DexWidget*  m_onwer; //��container�����ĵ�widget�����û����ΪNULL
public:
	CDexWidgetContainer();
	~CDexWidgetContainer();

protected:
	void _UpdateTotalCounts();  //ÿ���һ��widget�������¼����ܹ���widget����
public:
	void Hide();
	void Update(int delta);
	void Render();
	void Enable(bool child = false);
	void Disable(bool child = false);
	bool OnEvent(stEvent event);
	void OnFatherSetAlpha(int alpha);
	void Offset(const DexGUI::DexPoint& delta); //container�е�����widget���궼ƽ��delta  
	void Offset(const DexGUI::DexPointF& delta); 
	void Scale(float x, float y);
	void SetOnwer(DexWidget* onwer);
	bool Empty();  //�Ƿ�Ϊ��
	int  GetThisWidgetCount();  //��ȡֱ����container�µĿؼ�����
	int  GetTotalWidgetCount(); //��ȡ��contianer�µ����пؼ�����
	void AddWidgetToFront(DexWidget* widget);  //ͷ������widget
	void AddWidgetToBack(DexWidget* widget, bool asChild = true); //β������widget  asChild:�Ƿ���Ϊ�ӿؼ�
	void InsertWidgetFront(DexWidget* wait_insert, int id);  //���б��б��Ϊid֮ǰ����wait_insert
	void InsertWidgetBack(DexWidget* wait_insert, int id);  //���б��б��Ϊid֮�����wait_insert
	/*
	void RemoveWidget(CDexWidget* widget);    //��m_WidgetList���Ƴ�widget
	CDexWidget* FindWidget(std::string widget_name);
	CDexWidget* FindWidget(int id);
	void BringToRenderTop(int id);
	void BringToRenderTop(string name);
	*/
};