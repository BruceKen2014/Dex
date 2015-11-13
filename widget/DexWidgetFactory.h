

#pragma once
#include <map>
#include "DexWidget.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"
class CDexWidgetFactory
{
private:
	int m_totalWidgetCount;  //记录创建过的widget数量 每new一个widget 该变量+1，即使widget被销毁，该变量也不会减少
	typedef std::map<string, CDexWidget*> TWidgetMap;
	TWidgetMap m_WidgetMap;

public:
	CDexWidgetFactory();
	~CDexWidgetFactory();

private:
	bool parseXmlNode(TiXmlNode* node, int stage, CDexWidget* father = NULL);
	CDexWidget* parseButton(TiXmlAttribute* att, CDexWidget* father = NULL);
	CDexWidget* parseImage(TiXmlAttribute* att, CDexWidget* father = NULL);
	CDexWidget* parseImageSequence(TiXmlAttribute* att, CDexWidget* father = NULL);
	CDexWidget* parseCheckButton(TiXmlAttribute* att, CDexWidget* father = NULL);
	CDexWidget* parseLabel(TiXmlAttribute* att, CDexWidget* father = NULL);

	void parseBaseWidget(CDexWidget* widget, TiXmlAttribute* att);
	CDexWidget* parseAttributeName(TiXmlAttribute* att, EWidgetType type, CDexWidget* father = NULL);
	void parseAttributeVisible(CDexWidget* widget, TiXmlAttribute* att);
	void parseAttributeSize(CDexWidget* widget, TiXmlAttribute* att);
	void parseAttributePos(CDexWidget* widget, TiXmlAttribute* att);
	void parseAttributeOffset(CDexWidget* widget, TiXmlAttribute* att);
	void parseAttributeMask(CDexWidget* widget, TiXmlAttribute* att);
	void parseAttributeMaskColor(CDexWidget* widget, TiXmlAttribute* att);
	void parseAttributeAlpha(CDexWidget* widget, TiXmlAttribute* att);
	void parseAttributeMouseEvent(CDexWidget* widget, TiXmlAttribute* att);
public:
	CDexWidget* createWidget(EWidgetType type, string name="");
	bool removeWidget(string name);

	bool loadWidgetsFromXml(const char* xmlName);
	CDexWidget* findWidget(string name);//只是查找一个widget
	CDexWidget* useWidget(string name); //查找一个widget，并添加引用
};
CDexWidgetFactory* getWidgetFactory();
