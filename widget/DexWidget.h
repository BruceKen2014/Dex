

#ifndef DEX_WIDGET_H
#define DEX_WIDGET_H

//#include "CTexture.h"
#include <list>
#include <string>
#include "../DexBase/typedefine.h"
#include "../DexBase/CReference.h"
#include "../DexBase/CDelegate.h"
#include "../DexBase/DexObject.h"
#include "../Source/CTexture.h"
#include "DexGuiStruct.h"
#include "../DexMath/DexVector2.h"
#include "../DexBase/DexVirtualKey.h"


#define UI_ADD_REF(p)    _ADDREF(p)
#define UI_DROP_REF(p)   _REDUCEREF(p)

namespace DexGUI
{
	enum
	{//��Ӧ���¼�����
		mouse_none   = 0,
		mouse_l_down = 1 <<0,	 //����������
		mouse_l_up   = 1 <<1,	 //����������
		mouse_r_down = 1 <<2,	 //����Ҽ�����
		mouse_r_up   = 1 <<3,	 //����Ҽ�����
		mouse_move   = 1 <<4,	 //�������ƶ�
		mouse_l_drag = 1 <<5,	 //��������ק
		mouse_r_drag = 1 <<6,	 //����Ҽ���ק
		mouse_m_down = 1 <<7,    //�����ְ���
		mouse_m_up	 = 1 <<8,    //�����ֵ���
		mouse_wheel_f= 1 <<9,    //������ǰ��
		mouse_wheel_b= 1 <<10,   //�����ֺ��

		key_up		 = 1 <<11,   //���̰�������
		key_down     = 1 <<12,   //���̰�������

		catch_event  = 1 <<31,   //�ؼ���Ч����������Ӧ�¼�
	};
	enum EWidgetType
	{
		widget_base,
		widget_image, //ͼƬ�ؼ�
		widget_image_sequence, //ͼƬ���пؼ�
		widget_label,  //�ı��ؼ�
		widget_button, //��ť
		widget_text_buttn,  //�ı���ť
		widget_check_button, //��ѡ��
		widget_editbox,
		widget_scrollbar_v,
		widget_scrollbar_h,
		widget_progressBar,
		widget_form,  //���
	};
	enum EModiFyType
	{
		Add_Flag,
		Minus_Flag
	};

	struct stUiImage
	{
		CDexTex* m_tex;
		DexRect  m_srcArea; //�����е�����
		bool     m_Mirro;  //�Ƿ���
		stUiImage()
		{
			m_tex = NULL;
			m_Mirro = false;
		}
		~stUiImage()
		{
			UI_DROP_REF(m_tex);
		}
	};

}
using namespace DexGUI;

class CDexGuiAction;
class CDexWidgetContainer;
class DexWidget:public CRefCount, public DexObject
{
	Dex_DeclareClass(DexWidget, DexObject, 0)
	friend class CDexWidgetContainer;
public:
	CEventDispather  m_MouseMove;
	CEventDispather  m_MouseMoveIn;
	CEventDispather  m_MouseMoveOut;
	CEventDispather  m_MouseLUp;
	CEventDispather  m_MouseLDown;
	CEventDispather  m_MouseRUp;
	CEventDispather  m_MouseRDown;
	CEventDispather  m_MouseMUp;  //���ְ���
	CEventDispather  m_MouseMDown;
	CEventDispather  m_MouseWheelFront; //����ǰ��
	CEventDispather  m_MouseWheelBack;  //���ֺ��
	CEventDispather  m_MouseLDrag;  
	CEventDispather  m_MouseRDrag;  
	CEventDispather  m_KeyUp;  
	CEventDispather  m_KeyDown;  
	CEventDispather  EDResize;
	CEventDispather  EDSetPos;

protected:
	virtual void ShutDown() ;  //�ڴ��ͷ���Դ������������������Դ�ͷ�

protected:
	bool        m_bClipByParent;    //�Ƿ񱻸��ڵ�ü�
	bool	    m_enable;           //�Ƿ���Ч
	bool	    m_visible;	        //�Ƿ�ɼ�
	bool        m_mask;             //�Ƿ����ɰ�
	bool        m_mouseOn;          //����Ƿ�ͣ��������
	DexColor    m_maskColor;        //�ɰ���ɫ
	int         m_initAlpha;        //��ʼ͸���� ����֮���������͸���� ��ֵ������
	int         m_alpha;            //͸����
	int		    m_flag;				//������Щ����¼� �������������¼� ����Ϊmouse_none
	int         m_id;
	float       m_rotate;           //��ת�Ƕ�
	EDraw_Flag m_Drawflag;
	EWidgetType m_type;
	std::string m_name;  //�ؼ�����
	DexPointF m_offFatherPos; //��Ը�UI��ƫ����
	DexRectF  m_rect;         //ռ�ݵ�����
	DexRectF  m_rectAfterClip; //�����ڵ�ü���ռ�ݵ�����
	string m_mouseOnSoundFilename;
	string m_mouseDownSoundFilename;
	string m_mouseDownUnvalid; //���һ���ؼ���Ч �����������
	//std::list<CDexGuiValidArea*> m_area;  //ʵ��ռ�ݵ������б�

protected:
	list<CDexGuiAction*> m_actions;
	DexWidget* m_father;       //��widget
	DexWidget* m_left_sibling; //ͬһ�㼶���ϸ�widget
	DexWidget* m_right_sibling; //ͬһ�㼶���¸�widget
	CDexWidgetContainer* m_children;

public:
	virtual void OnKeyChar(stEvent event); //�����ַ�����
	virtual void OnKeyDown(stEvent event); //�����ַ�����İ���
protected:
	//�����������·����������⴦��
	virtual bool OnMouseMove(stEvent event);//����ֵ�����Ƿ��������¼��M����̎��
	virtual void OnMouseMoveIn(stEvent event);
	virtual void OnMouseMoveOut(stEvent event);
	virtual bool OnMouseLUp(stEvent event);
	virtual void OnMouseLDown(stEvent event);
	virtual void OnMouseRUp(stEvent event);
	virtual void OnMouseRDown(stEvent event);
	virtual void OnMouseMUp(stEvent event);
	virtual void OnMouseMDown(stEvent event);
	virtual void OnMouseWheelFront(stEvent event);
	virtual void OnMouseWheelBack(stEvent event);
	virtual void OnMouseLDrag(stEvent event);
	virtual void OnMouseRDrag(stEvent event);
	
	//virtual void OnMouseLDrag(stEvent event);
	//virtual void OnMouseRDrag(stEvent event);

	//�����������·������������ж�
	virtual bool MouseMoveValid(stEvent event);
	virtual bool MouseLUpValid(stEvent event); 
	virtual bool MouseLDownValid(stEvent event);
	virtual bool MouseRUpValid(stEvent event);
	virtual bool MouseRDownValid(stEvent event);
	virtual bool MouseMUpValid(stEvent event);
	virtual bool MouseMDownValid(stEvent event);
	virtual bool MouseWheelFrontValid(stEvent event);
	virtual bool MouseWheelBackValid(stEvent event);
	virtual bool MouseLDragValid(stEvent event);
	virtual bool MouseRDragValid(stEvent event);

	bool CheckPointIn(const DexPoint& pt); //�ж�һ���Ƿ����ڿؼ���
	void ReCalculateUV(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3, const DexRectF& srcRect, const DexRectF& clipRect);//�ؼ����ü�֮�����¼���ü����ζ�Ӧ��uv
public:
	DexWidget();
	virtual ~DexWidget();
public:

	void Show();
	void Hide();
	bool GetFocus() const;
	void SetFocus(bool focus = true);
	bool GetSelect() const;
	void SetSelect(bool select);
	virtual void Create();
	virtual bool Update(int delta); 
	virtual void RenderThis();
	virtual void Render();

	virtual void RenderRect(); //��Ⱦ��Χ���ο�
	virtual void RenderMask(); //��Ⱦ�ɰ�

public:
	virtual bool OnEvent(stEvent event);	  //�������¼���
	virtual bool EventValid(stEvent event);   //�����Ƿ�����¼�

public:
	int  GetId() ;
	DexPointF GetPos();
	DexRectF  GetArea();   //��������ڵ�ü� �򷵻زü��������
	DexRectF  GetAreaReal(); //����������ռ������ ������ü�
	void SetClipByParent(bool clipByParent);
	bool GetClipByParent();
	bool GetVisible();
	void setVisible(bool visible);
	CDexWidgetContainer* GetChildrenContainer();
public:
	void AddAction(CDexGuiAction* action);
	void EraseAction(CDexGuiAction* action); //ɾ��action��action�������update���ж���ɺ󣬵���Owner���Լ�ɾ��
	void StopMoveAction();  //���widget���ƶ���ֹͣ����ɾ��������move action
	virtual void ModifyFlag(EModiFyType i_o, int flag);
	virtual void Enable(bool child = false); //�Ƿ���������е��ӿؼ���false:ֻӰ������
	virtual void Disable(bool child = false);//�Ƿ���������е��ӿؼ���false:ֻӰ������
	void setMouseOnFilename(string filename);
	void setMouseDownFilename(string filename);
	void setMouseDownUnvalid(string filename);
	void setDrawFlag(EDraw_Flag flag);
	float GetWidth();
	float GetHeight();
	void  SetRotate(float rotate);
	float GetRotate();
	DexPointF GetSize();
	string    GetName();
	bool GetEable();
	bool GetFlag(int flag);
	DexWidget* GetFather();
	int  GetWidgetFlag();
	void SetWidgetFlag(int flag);
	void SetId(int id);
	void SetName(string name);
	void SetFather(DexWidget* widget);
	void SetPos(const DexPoint& pos);  //����������
	void SetPos(const DexPointF& pos); 
	void SetPos(int x, int y);
	void setFatherOffset(float x, float y);
	DexPointF getFatherOffset();
	void OpenMask(bool open);
	void SetMaskColor(DUChar r, DUChar g, DUChar b);
	void SetMaskAlpha(DUChar a);
	void SetMaskColor(const DexColor& color);
	void SetMaskColor(DUChar r, DUChar g, DUChar b, DUChar a);
 
	//�ó�ʼ͸���������������alpha childeren���Ƿ����ӿؼ������ӿؼ���͸����
	//void ResetAlphaByInitAlpha(bool children);
	void SetInitAlpha(int alpha);
	int  GetInitAlpha();
	void SetAlpha(int alpha, bool children = false); //children:����alphaֵ�� �Ƿ�����ӿؼ���alpha
	int  GetOnlyAlpha();
	void OffSet(const DexPoint& delta);  //ƽ����Ӧ����ֵ
	void OffSet(const DexPointF& delta); 
	 //���� �Ƿ�Ӱ���ӿؼ� �Ƿ���Ҫ���ݸ��ؼ���λ�ý�������Ӧ
	void Scale(float x, float y, bool scaleChild = false, bool changePos = false);
	virtual void Resize(const DexSize& size, bool resizeChild = false);  //�������óߴ��С ��Ϊ�麯�� ���������Ҫ���в�ͬ�Ĵ���
	virtual void Resize(const DexSizeF& size, bool resizeChild = false);
	virtual void Resize(DInt32 width, DInt32 height, DBool resizeChild = false);
	void SetRightSibling(DexWidget* widget) { m_right_sibling = widget;};
	void SetLeftSibling(DexWidget* widget) { m_left_sibling = widget;};
public:
};



#endif