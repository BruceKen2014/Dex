

#pragma once
#include <map>
#include "DexWidget.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"
class CDexWidgetFactory
{
private:
	int m_totalWidgetCount;  //记录创建过的widget数量 每new一个widget 该变量+1，即使widget被销毁，该变量也不会减少
	typedef std::map<string, DexWidget*> TWidgetMap;
	TWidgetMap m_WidgetMap;

public:
	CDexWidgetFactory();
	~CDexWidgetFactory();

private:
	bool parseXmlNode(TiXmlNode* node, int stage, DexWidget* father = NULL);
	DexWidget* parseButton(TiXmlAttribute* att, DexWidget* father = NULL);
	DexWidget* parseImage(TiXmlAttribute* att, DexWidget* father = NULL);
	DexWidget* parseImageSequence(TiXmlAttribute* att, DexWidget* father = NULL);
	DexWidget* parseCheckButton(TiXmlAttribute* att, DexWidget* father = NULL);
	DexWidget* parseLabel(TiXmlAttribute* att, DexWidget* father = NULL);

	void parseBaseWidget(DexWidget* widget, TiXmlAttribute* att);
	DexWidget* parseAttributeName(TiXmlAttribute* att, EWidgetType type, DexWidget* father = NULL);
	void parseAttributeVisible(DexWidget* widget, TiXmlAttribute* att);
	void parseAttributeSize(DexWidget* widget, TiXmlAttribute* att);
	void parseAttributePos(DexWidget* widget, TiXmlAttribute* att);
	void parseAttributeOffset(DexWidget* widget, TiXmlAttribute* att);
	void parseAttributeMask(DexWidget* widget, TiXmlAttribute* att);
	void parseAttributeMaskColor(DexWidget* widget, TiXmlAttribute* att);
	void parseAttributeAlpha(DexWidget* widget, TiXmlAttribute* att);
	void parseAttributeMouseEvent(DexWidget* widget, TiXmlAttribute* att);
public:
	DexWidget* createWidget(EWidgetType type, string name="");
	bool removeWidget(string name);

	bool loadWidgetsFromXml(const char* xmlName);
	DexWidget* findWidget(string name);//只是查找一个widget
	DexWidget* useWidget(string name); //查找一个widget，并添加引用
};
CDexWidgetFactory* getWidgetFactory();
