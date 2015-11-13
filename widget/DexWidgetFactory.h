

#pragma once
#include <map>
#include "DexWidget.h"
#include "../xml/tinystr.h"
#include "../xml/tinyxml.h"
class CDexWidgetFactory
{
private:
	int m_totalWidgetCount;  //��¼��������widget���� ÿnewһ��widget �ñ���+1����ʹwidget�����٣��ñ���Ҳ�������
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
	CDexWidget* findWidget(string name);//ֻ�ǲ���һ��widget
	CDexWidget* useWidget(string name); //����һ��widget�����������
};
CDexWidgetFactory* getWidgetFactory();
