

#pragma once
#include <map>
#include "DexWidget.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"
class CDexWidgetFactory
{
private:
	int m_totalWidgetCount;  //��¼��������widget���� ÿnewһ��widget �ñ���+1����ʹwidget�����٣��ñ���Ҳ�������
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
	DexWidget* findWidget(string name);//ֻ�ǲ���һ��widget
	DexWidget* useWidget(string name); //����һ��widget�����������
};
CDexWidgetFactory* getWidgetFactory();
