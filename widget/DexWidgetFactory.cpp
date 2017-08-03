

#include "DexWidgetFactory.h"
#include "DexUiInclude.h"
#include "../DexBase/DexLog.h"
#include "DexWidgetProgressBar.h"
CDexWidgetFactory::CDexWidgetFactory()
{
	m_totalWidgetCount = 0;
}

CDexWidgetFactory::~CDexWidgetFactory()
{

}

CDexWidget* CDexWidgetFactory::createWidget(EWidgetType type, string name)
{
	TWidgetMap::iterator ite = m_WidgetMap.find(name);
	if(ite != m_WidgetMap.end())
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "createWidget要创建的widget重名！");
		DexLog::getSingleton()->EndLog();
		return ite->second;
	}
	CDexWidget* ret = NULL;
	switch(type)
	{
	case widget_image:
		{
			ret = new CDexWidgetImage();
			break;
		}
	case widget_image_sequence:
		{
			ret = new CDexWidgetImageSequence;
			break;
		}
	case widget_button:
		{
			ret = new CDexWidgetButton();
			break;
		}
	case widget_check_button:
		{
			ret = new CDexWidgetCheckBtn();
			break;
		}
	case widget_editbox:
		{
			ret = new CDexWidgetEditBox();
			break;
		}
	case widget_scrollbar_v:
		{
			//ret = new CDexWidgetScrollBarV();
			break;
		}
	case widget_label:
		{
			ret = new CDexWidgetLabel();
			break;
		}	
	case widget_progressBar:
		{
			ret = new DexWidgetProgressBar();
		}
	default:
		break;
	}
	if(ret == NULL)
		return NULL;
	if(name.length() == 0)
	{
		setDexBuffer("widget%d",m_totalWidgetCount);
		name = string(getDexBuffer());
	}
	ret->SetName(name);
	ret->SetId(m_totalWidgetCount++);
	m_WidgetMap[name] = ret;
	return ret;
}

bool CDexWidgetFactory::removeWidget(string name)
{
	TWidgetMap::iterator it = m_WidgetMap.find(name);
	if(it == m_WidgetMap.end())
	{
		DexLog::getSingleton()->Log(log_allert, "试图移除一个不存在的widget%s", name.c_str());
		return false;
	}
	CDexWidget* widget = m_WidgetMap[name];	
	_SafeDelete(widget);
	m_WidgetMap.erase(it);
	return true;
}
bool CDexWidgetFactory::loadWidgetsFromXml(const char* xmlName)
{
	TiXmlDocument Xml(xmlName);
	Xml.LoadFile(); 
	parseXmlNode(Xml.FirstChild(), 0, NULL);
	return true;
}

CDexWidget* CDexWidgetFactory::findWidget(string name)
{
	if(m_WidgetMap.find(name) != m_WidgetMap.end())
	{
		return m_WidgetMap[name];
	}
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_allert,"未找到widget:%s！", name.c_str());
	DexLog::getSingleton()->EndLog();
	return NULL;
}

CDexWidget* CDexWidgetFactory::useWidget(string name)
{
	CDexWidget* widget = findWidget(name);
	if(widget != NULL)
	{
		UI_ADD_REF(widget);
		return widget;
	}
	return NULL;
}
CDexWidget* CDexWidgetFactory::parseButton(TiXmlAttribute* att, CDexWidget* father)
{
	CDexWidgetButton* button = NULL;
	while(att != NULL)
	{
		if(strcmp(att->Name(), "name") == 0)
		{//第一个属性必须是name
			button = (CDexWidgetButton*) parseAttributeName(att, widget_button, father);
			if(button == NULL)
				return NULL;
		}
		else if(strcmp(att->Name(), "visible") == 0 || strcmp(att->Name(), "size") == 0 || strcmp(att->Name(), "offset") == 0||
			strcmp(att->Name(), "pos") == 0     || strcmp(att->Name(), "mask") == 0 || strcmp(att->Name(), "alpha") == 0||
			strcmp(att->Name(), "maskcolor") == 0|| strcmp(att->Name(), "flag") == 0)
		{
			parseBaseWidget(button, att);
		}
		else if(strcmp(att->Name(), "normal_image") == 0)
		{//normal_image="1.png,20,20,50,50"
			string str(att->Value());
			vector<string> out_str;
			SplitStr(str,',',out_str);
			if(out_str.size() != 5)
			{
				DexLog::getSingleton()->BeginLog();
				DexLog::getSingleton()->Log(log_allert, "button %s normal_image参数有误,设置失败！", button->GetName().c_str());
				DexLog::getSingleton()->EndLog();
			}
			else
			{
				DexRect rect;
				rect.left = CDexFun::str_to_int(out_str[1]);
				rect.top = CDexFun::str_to_int(out_str[2]);
				rect.right = CDexFun::str_to_int(out_str[3]);
				rect.bottom = CDexFun::str_to_int(out_str[4]);
				button->SetStateTex(btn_normal, out_str[0]);
				button->SetStateTexRect(btn_normal, rect);
			}
		}
		else if(strcmp(att->Name(), "mouseon_image") == 0)
		{//mouseon_image="1.png,20,20,50,50"
			string str(att->Value());
			vector<string> out_str;
			SplitStr(str,',',out_str);
			if(out_str.size() != 5)
			{
				DexLog::getSingleton()->BeginLog();
				DexLog::getSingleton()->Log(log_allert, "button %s mouseon_image参数有误,设置失败！", button->GetName().c_str());
				DexLog::getSingleton()->EndLog();
			}
			else
			{
				DexRect rect;
				rect.left = CDexFun::str_to_int(out_str[1]);
				rect.top = CDexFun::str_to_int(out_str[2]);
				rect.right = CDexFun::str_to_int(out_str[3]);
				rect.bottom = CDexFun::str_to_int(out_str[4]);
				button->SetStateTex(btn_mouse_on, out_str[0]);
				button->SetStateTexRect(btn_mouse_on, rect);
			}
		}
		else if(strcmp(att->Name(), "mousedown_image") == 0)
		{//mousedown_image="1.png,20,20,50,50"
			string str(att->Value());
			vector<string> out_str;
			SplitStr(str,',',out_str);
			if(out_str.size() != 5)
			{
				DexLog::getSingleton()->BeginLog();
				DexLog::getSingleton()->Log(log_allert, "button %s mousedown_image参数有误,设置失败！", button->GetName().c_str());
				DexLog::getSingleton()->EndLog();
			}
			else
			{
				DexRect rect;
				rect.left = CDexFun::str_to_int(out_str[1]);
				rect.top = CDexFun::str_to_int(out_str[2]);
				rect.right = CDexFun::str_to_int(out_str[3]);
				rect.bottom = CDexFun::str_to_int(out_str[4]);
				button->SetStateTex(btn_mouse_down, out_str[0]);
				button->SetStateTexRect(btn_mouse_down, rect);
			}
		}
		else if(strcmp(att->Name(), "mouse_on_sound") == 0)
		{//mouse_on_sound="button_on.wav"
			button->setMouseOnFilename(att->Value());
		}
		else if(strcmp(att->Name(), "mouse_down_sound") == 0)
		{//mouse_down_sound="button_down.wav"
			button->setMouseDownFilename(att->Value());
		}
		att = att->Next();
	}
	return button;
}
CDexWidget* CDexWidgetFactory::parseImage(TiXmlAttribute* att, CDexWidget* father)
{
	CDexWidgetImage* image = NULL;
	while(att != NULL)
	{
		if(strcmp(att->Name(), "name") == 0)
		{//第一个属性必须是name
			
			image = (CDexWidgetImage*)parseAttributeName(att, widget_image, father);
			if(image== NULL)
				return NULL;
		}
		else if(strcmp(att->Name(), "visible") == 0 || strcmp(att->Name(), "size") == 0 || strcmp(att->Name(), "offset") == 0||
				strcmp(att->Name(), "pos") == 0     || strcmp(att->Name(), "mask") == 0 || strcmp(att->Name(), "alpha") == 0||
				strcmp(att->Name(), "maskcolor") == 0|| strcmp(att->Name(), "flag") == 0)
		{
			parseBaseWidget(image, att);
		}
		else if(strcmp(att->Name(), "image") == 0)
		{//image="1.png,20,20,50,50"
			string str(att->Value());
			vector<string> out_str;
			SplitStr(str,',',out_str);
			if(out_str.size() != 5)
			{
				DexLog::getSingleton()->BeginLog();
				DexLog::getSingleton()->Log(log_allert, "image %s image参数有误,设置失败！", image->GetName().c_str());
				DexLog::getSingleton()->EndLog();
			}
			else
			{
				DexRect rect;
				rect.left = CDexFun::str_to_int(out_str[1]);
				rect.top = CDexFun::str_to_int(out_str[2]);
				rect.right = CDexFun::str_to_int(out_str[3]);
				rect.bottom = CDexFun::str_to_int(out_str[4]);
				image->SetTexName(out_str[0]);
				image->SetTexRes(rect);
			}
		}
		else if(strcmp(att->Name(), "mirro") == 0)
		{//mirro="1"
			int bMirro = CDexFun::str_to_int(att->Value());
			image->SetMirro(bMirro);
		}
		att = att->Next();
	}
	return image;
}
CDexWidget* CDexWidgetFactory::parseImageSequence(TiXmlAttribute* att, CDexWidget* father /* = NULL */)
{
	CDexWidgetImageSequence* image_sequence = NULL;
	while(att != NULL)
	{
		if(strcmp(att->Name(), "name") == 0)
		{//第一个属性必须是name

			image_sequence = (CDexWidgetImageSequence*)parseAttributeName(att, widget_image_sequence, father);
			if(image_sequence== NULL)
				return NULL;
		}
		else if(strcmp(att->Name(), "visible") == 0 || strcmp(att->Name(), "size") == 0 || strcmp(att->Name(), "offset") == 0||
			strcmp(att->Name(), "pos") == 0     || strcmp(att->Name(), "mask") == 0 || strcmp(att->Name(), "alpha") == 0||
			strcmp(att->Name(), "maskcolor") == 0|| strcmp(att->Name(), "flag") == 0)
		{
			parseBaseWidget(image_sequence, att);
		}
		else if(strcmp(att->Name(), "image") == 0)
		{//image="女孩0.png,0,0,0,0$女孩1.png,0,0,0,0"
			string str(att->Value());
			vector<string> image_string;
			vector<string> image_info;
			SplitStr(str,'$',image_string);
			for(size_t i = 0; i < image_string.size(); i++)
			{
				image_info.clear();
				SplitStr(image_string[i],',', image_info);
				if(image_info.size() != 5)
				{
					DexLog::getSingleton()->BeginLog();
					DexLog::getSingleton()->Log(log_allert, "image_sequence %s image参数有误,设置失败！", image_sequence->GetName().c_str());
					DexLog::getSingleton()->EndLog();
				}
				else
				{
					DexRect rect;
					rect.left = CDexFun::str_to_int(image_info[1]);
					rect.top = CDexFun::str_to_int(image_info[2]);
					rect.right = CDexFun::str_to_int(image_info[3]);
					rect.bottom = CDexFun::str_to_int(image_info[4]);
					if(rect.right == 0)
					{
						image_sequence->AddImage(image_info[0]);
					}
					else
					{
						image_sequence->AddImage(image_info[0], rect);
					}
				}
			}
		}
		else if(strcmp(att->Name(), "interval") == 0)
		{//mirro="1"
			int interval = CDexFun::str_to_int(att->Value());
			image_sequence->SetInterval(interval);
		}
		att = att->Next();
	}
	return image_sequence;
}
CDexWidget* CDexWidgetFactory::parseCheckButton(TiXmlAttribute* att, CDexWidget* father)
{
	CDexWidgetCheckBtn* check_button = NULL;
	while(att != NULL)
	{
		if(strcmp(att->Name(), "name") == 0)
		{//第一个属性必须是name
			check_button = (CDexWidgetCheckBtn*) parseAttributeName(att, widget_check_button, father);
			if(check_button == NULL)
				return NULL;
		}
		else if(strcmp(att->Name(), "visible") == 0 || strcmp(att->Name(), "size") == 0 || strcmp(att->Name(), "offset") == 0||
			strcmp(att->Name(), "pos") == 0     || strcmp(att->Name(), "mask") == 0 || strcmp(att->Name(), "alpha") == 0||
			strcmp(att->Name(), "maskcolor") == 0|| strcmp(att->Name(), "flag") == 0)
		{
			parseBaseWidget(check_button, att);
		}
		else if(strcmp(att->Name(), "check_image") == 0)
		{//check_image="1.png,20,20,50,50"
			string str(att->Value());
			vector<string> out_str;
			SplitStr(str,',',out_str);
			if(out_str.size() != 5)
			{
				DexLog::getSingleton()->BeginLog();
				DexLog::getSingleton()->Log(log_allert, "CheckButton %s check_image参数有误,设置失败！", check_button->GetName().c_str());
				DexLog::getSingleton()->EndLog();
			}
			else
			{
				DexRect rect;
				rect.left = CDexFun::str_to_int(out_str[1]);
				rect.top = CDexFun::str_to_int(out_str[2]);
				rect.right = CDexFun::str_to_int(out_str[3]);
				rect.bottom = CDexFun::str_to_int(out_str[4]);
				check_button->SetCheckTex(out_str[0], rect);
			}
		}
		else if(strcmp(att->Name(), "ucheck_image") == 0)
		{//ucheck_image="1.png,20,20,50,50"
			string str(att->Value());
			vector<string> out_str;
			SplitStr(str,',',out_str);
			if(out_str.size() != 5)
			{
				DexLog::getSingleton()->BeginLog();
				DexLog::getSingleton()->Log(log_allert, "CheckButton %s ucheck_image参数有误,设置失败！", check_button->GetName().c_str());
				DexLog::getSingleton()->EndLog();
			}
			else
			{
				DexRect rect;
				rect.left = CDexFun::str_to_int(out_str[1]);
				rect.top = CDexFun::str_to_int(out_str[2]);
				rect.right = CDexFun::str_to_int(out_str[3]);
				rect.bottom = CDexFun::str_to_int(out_str[4]);
				check_button->SetUnCheckTex(out_str[0], rect);
			}
		}
		att = att->Next();
	}
	return check_button;
}
CDexWidget* CDexWidgetFactory::parseLabel(TiXmlAttribute* att, CDexWidget* father)
{
	CDexWidgetLabel* label = NULL;
	while(att != NULL)
	{
		if(strcmp(att->Name(), "name") == 0)
		{//第一个属性必须是name
			label = (CDexWidgetLabel*) parseAttributeName(att, widget_label, father);
			if(label == NULL)
				return NULL;
		}
		else if(strcmp(att->Name(), "visible") == 0 || strcmp(att->Name(), "size") == 0 || strcmp(att->Name(), "offset") == 0||
			strcmp(att->Name(), "pos") == 0     || strcmp(att->Name(), "mask") == 0 || strcmp(att->Name(), "alpha") == 0||
			strcmp(att->Name(), "maskcolor") == 0|| strcmp(att->Name(), "flag") == 0)
		{
			parseBaseWidget(label, att);
		}
		else if(strcmp(att->Name(), "string") == 0)
		{//string="label content"
			label->SetText(att->Value());
		}
		else if(strcmp(att->Name(), "TextSize") == 0)
		{//TextSize="20"
			label->SetTextSize(CDexFun::str_to_int(att->Value()));
		}
		else if(strcmp(att->Name(), "TextColor") == 0)
		{//TextColor="1.0f,1.0f,1.0f,1.0f"
			string str(att->Value());
			vector<string> out_str;
			SplitStr(str,',',out_str);
			if(out_str.size() != 4)
			{
				DexLog::getSingleton()->BeginLog();
				DexLog::getSingleton()->Log(log_allert, "label %s TextColor参数有误,设置失败！", label->GetName().c_str());
				DexLog::getSingleton()->EndLog();
			}
			else
			{
				label->SetTextColor(DexColor(CDexFun::str_to_float(out_str[0]),CDexFun::str_to_float(out_str[1]),
					CDexFun::str_to_float(out_str[2]),CDexFun::str_to_float(out_str[3])));
			}
		}
		att = att->Next();
	}
	return label;
}
void CDexWidgetFactory::parseBaseWidget(CDexWidget* widget, TiXmlAttribute* att)
{//暂时设定size属性必须在offset属性之前
	if(strcmp(att->Name(), "visible") == 0)
	{
		parseAttributeVisible(widget, att);
	}
	else if(strcmp(att->Name(), "size") == 0)
	{
		parseAttributeSize(widget, att);
	}
	else if(strcmp(att->Name(), "offset") == 0)
	{
		parseAttributeOffset(widget, att);
	}
	else if(strcmp(att->Name(), "pos") == 0)
	{
		parseAttributePos(widget, att);
	}
	else if(strcmp(att->Name(), "mask") == 0)
	{
		parseAttributeMask(widget, att);
	}
	else if(strcmp(att->Name(), "alpha") == 0)
	{
		parseAttributeAlpha(widget, att);
	}
	else if(strcmp(att->Name(), "maskcolor") == 0)
	{
		parseAttributeMaskColor(widget, att);
	}
	else if(strcmp(att->Name(), "flag") == 0)
	{
		parseAttributeMouseEvent(widget, att);
	}
}
CDexWidget* CDexWidgetFactory::parseAttributeName(TiXmlAttribute* att,EWidgetType type, CDexWidget* father /* = NULL */)
{
	string name;
	if(father != NULL)
		name = father->GetName() +  "." + string(att->Value());
	else
		name = string(att->Value());
	CDexWidget* widget = createWidget(type, name);
	if(widget == NULL)
		return NULL;
	if(father != NULL)
	{
		father->GetChildrenContainer()->AddWidgetToBack(widget);
	}
	return widget;
}
void CDexWidgetFactory::parseAttributeVisible(CDexWidget* widget, TiXmlAttribute* att)
{//visible="1"
	widget->setVisible(atoi(att->Value()));
}
void CDexWidgetFactory::parseAttributeSize(CDexWidget* widget, TiXmlAttribute* att)
{//size="200,300"
	string str(att->Value());
	vector<string> out_str;
	SplitStr(str,',',out_str);
	if(out_str.size() != 2)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "widget %s size参数有误,已被置为0,0", widget->GetName().c_str());
		DexLog::getSingleton()->EndLog();
		widget->Resize(DexSizeF(0,0));
	}
	else
	{
		float width = CDexFun::str_to_float(out_str[0]);
		float height = CDexFun::str_to_float(out_str[1]);
		widget->Resize(DexSizeF(width, height));
	}
}
void CDexWidgetFactory::parseAttributePos(CDexWidget* widget, TiXmlAttribute* att)
{//pos="200,200"
	string str(att->Value());
	vector<string> out_str;
	SplitStr(str,',',out_str);
	if(out_str.size() != 2)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "widget %s pos参数有误,已被置为0,0", widget->GetName().c_str());
		DexLog::getSingleton()->EndLog();
		widget->SetPos(0,0);
	}
	else
	{
		int posx = CDexFun::str_to_float(out_str[0]);
		int posy = CDexFun::str_to_float(out_str[1]);
		widget->SetPos(posx, posy);
	}

}
void CDexWidgetFactory::parseAttributeOffset(CDexWidget* widget, TiXmlAttribute* att)
{//offset="2.0f,2.0f"
	string str(att->Value());
	vector<string> out_str;
	SplitStr(str,',',out_str);
	if(out_str.size() != 2)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "widget %s offset参数有误,已被置为0,0", widget->GetName().c_str());
		DexLog::getSingleton()->EndLog();
		widget->setFatherOffset(0.0f,0.0f);
	}
	else
	{
		float offsetx = CDexFun::str_to_float(out_str[0]);
		float offsety = CDexFun::str_to_float(out_str[1]);
		widget->setFatherOffset(offsetx, offsety);
	}

}
void CDexWidgetFactory::parseAttributeMask(CDexWidget* widget, TiXmlAttribute* att)
{//mask="1"
	int bMask = CDexFun::str_to_int(att->Value());
	widget->OpenMask(bMask);
}
void CDexWidgetFactory::parseAttributeMaskColor(CDexWidget* widget, TiXmlAttribute* att)
{//maskcolor="0,0,255,100"
	string str(att->Value());
	vector<string> out_str;
	SplitStr(str,',',out_str);
	if(out_str.size() != 4)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "widget %s maskcolor参数有误,设置失败！", widget->GetName().c_str());
		DexLog::getSingleton()->EndLog();
	}
	else
	{
		int r  = CDexFun::str_to_int(out_str[0]);
		int g = CDexFun::str_to_int(out_str[1]);
		int b = CDexFun::str_to_int(out_str[2]);
		int a  = CDexFun::str_to_int(out_str[3]);
		widget->SetMaskColor(r,g,b,a);
	}
}
void CDexWidgetFactory::parseAttributeAlpha(CDexWidget* widget, TiXmlAttribute* att)
{//alpha="255"
	int alpha = CDexFun::str_to_int(att->Value());
	widget->SetInitAlpha(alpha);
}
void CDexWidgetFactory::parseAttributeMouseEvent(CDexWidget* widget, TiXmlAttribute* att)
{//flag="mouse_l_down,mouse_l_up"
	string str(att->Value());
	vector<string> out_str;
	SplitStr(str,',',out_str);
	for(size_t i = 0 ; i < out_str.size(); i++)
	{
		if(out_str[i]=="mouse_l_down")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_l_down);
		}
		else if(out_str[i]=="mouse_l_up")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_l_up);
		}
		else if(out_str[i]=="mouse_r_down")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_r_down);
		}
		else if(out_str[i]=="mouse_r_up")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_r_up);
		}
		else if(out_str[i]=="mouse_move")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_move);
		}
		else if(out_str[i]=="mouse_l_drag")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_l_drag);
		}
		else if(out_str[i]=="mouse_r_drag")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);
		}
		else if(out_str[i]=="mouse_m_down")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_m_down);
		}
		else if(out_str[i]=="mouse_m_up")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_m_up);
		}
		else if(out_str[i]=="mouse_wheel_f")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_wheel_f);
		}
		else if(out_str[i]=="mouse_wheel_b")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::mouse_wheel_b);
		}
		else if(out_str[i]=="key_up")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::key_up);
		}
		else if(out_str[i]=="key_down")
		{
			widget->ModifyFlag(Add_Flag, DexGUI::key_down);
		}
	}
}
bool CDexWidgetFactory::parseXmlNode(TiXmlNode* node, int stage, CDexWidget* father)
{
	CDexWidget* this_node = NULL; //本次解析到的node
	switch(node->Type())
	{
	case TiXmlNode::TINYXML_DECLARATION:
		{
			//TiXmlDeclaration* dec = node->ToDeclaration(); 
			//cout<<"<?xml version = \""<<dec->Version()<<"\""<<" encoding = \""<<dec->Encoding()<<"\"";
			break;
		}
	case TiXmlNode::TINYXML_ELEMENT:
		{	
			TiXmlElement* element = node->ToElement();
			
			TiXmlAttribute* att = element->FirstAttribute(); 
			if(strcmp(element->Value(), "Button") == 0)
			{
				this_node = parseButton(att, father);
			}
			else if(strcmp(element->Value(), "Image") == 0)
			{
				this_node = parseImage(att, father);
			}
			else if(strcmp(element->Value(), "CheckButton") == 0)
			{
				this_node = parseCheckButton(att, father);
			}
			else if(strcmp(element->Value(), "ImageSequence") == 0)
			{
				this_node = parseImageSequence(att, father);
			}
			else if(strcmp(element->Value(), "Label") == 0)
			{
				this_node = parseLabel(att, father);
			}
			break;
		}
	case TiXmlNode::TINYXML_TEXT:
		{
			//cout<<"<"<<Node->Name()<<">"<<Node->Value()<<"</"<<Node->Name()<<">"<<endl;
			//cout<<Node->Value()<<endl;
			break;
		}
	case TiXmlNode::TINYXML_COMMENT:
		{
			//cout<<"<!--"<<node->Value()<<"!-->"<<endl;
			break;
		}
	default:
		break;
	}  
	if(node->FirstChild() != NULL)
	{//子节点，递归
		parseXmlNode(node->FirstChild(), stage+1, this_node);
	}
	else
	{
		switch(node->Type())
		{
		case TiXmlNode::TINYXML_DECLARATION:
		case TiXmlNode::TINYXML_ELEMENT:
			//cout<<"/"<<">"<<endl;
			break;
		case TiXmlNode::TINYXML_COMMENT:
			break;
		}
	}
	if(node->NextSibling() != NULL)
	{//兄弟节点，递归
		parseXmlNode(node->NextSibling(), stage, father);
	}
	else if(node->Parent() != NULL)
	{//有父节点
		//for(int i = 0; i < (stage-1)*2; i++)
		//{
		//	cout<<" ";
		//}
		//cout<<"</"<<Node->Parent()->Value()<<">"<<endl;
	}
	if(father == NULL && this_node != NULL)
		getDesktop()->SetRootWidget(this_node);
	return true;
}
CDexWidgetFactory* getWidgetFactory()
{
	static CDexWidgetFactory* g_widgetFactory = new CDexWidgetFactory;
	return g_widgetFactory;
}