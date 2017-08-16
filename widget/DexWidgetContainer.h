

#pragma once

#include "../DexBase/CListener.h"
class DexWidget;
class CDexWidgetContainer
{
protected:
	std::list<DexWidget*> m_WidgetList; //直属widget
	int m_totalCount; //总共的widget数量
	DexWidget*  m_onwer; //该container所属的的widget，如果没有则为NULL
public:
	CDexWidgetContainer();
	~CDexWidgetContainer();

protected:
	void _UpdateTotalCounts();  //每添加一个widget，都重新计算总共的widget数量
public:
	void Hide();
	void Update(int delta);
	void Render();
	void Enable(bool child = false);
	void Disable(bool child = false);
	bool OnEvent(stEvent event);
	void OnFatherSetAlpha(int alpha);
	void Offset(const DexGUI::DexPoint& delta); //container中的所有widget坐标都平移delta  
	void Offset(const DexGUI::DexPointF& delta); 
	void Scale(float x, float y);
	void SetOnwer(DexWidget* onwer);
	bool Empty();  //是否为空
	int  GetThisWidgetCount();  //获取直属本container下的控件数量
	int  GetTotalWidgetCount(); //获取本contianer下的所有控件数量
	void AddWidgetToFront(DexWidget* widget);  //头部插入widget
	void AddWidgetToBack(DexWidget* widget, bool asChild = true); //尾部插入widget  asChild:是否作为子控件
	void InsertWidgetFront(DexWidget* wait_insert, int id);  //在列表中编号为id之前插入wait_insert
	void InsertWidgetBack(DexWidget* wait_insert, int id);  //在列表中编号为id之后插入wait_insert
	/*
	void RemoveWidget(CDexWidget* widget);    //从m_WidgetList中移除widget
	CDexWidget* FindWidget(std::string widget_name);
	CDexWidget* FindWidget(int id);
	void BringToRenderTop(int id);
	void BringToRenderTop(string name);
	*/
};