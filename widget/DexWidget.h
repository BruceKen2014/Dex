

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
	{//响应的事件类型
		mouse_none   = 0,
		mouse_l_down = 1 <<0,	 //鼠标左键按下
		mouse_l_up   = 1 <<1,	 //鼠标左键弹起
		mouse_r_down = 1 <<2,	 //鼠标右键按下
		mouse_r_up   = 1 <<3,	 //鼠标右键弹起
		mouse_move   = 1 <<4,	 //鼠标鼠标移动
		mouse_l_drag = 1 <<5,	 //鼠标左键拖拽
		mouse_r_drag = 1 <<6,	 //鼠标右键拖拽
		mouse_m_down = 1 <<7,    //鼠标滚轮按下
		mouse_m_up	 = 1 <<8,    //鼠标滚轮弹起
		mouse_wheel_f= 1 <<9,    //鼠标滚轮前滚
		mouse_wheel_b= 1 <<10,   //鼠标滚轮后滚

		key_up		 = 1 <<11,   //键盘按键弹起
		key_down     = 1 <<12,   //键盘按键按下

		catch_event  = 1 <<31,   //控件有效，即可以响应事件
	};
	enum EWidgetType
	{
		widget_base,
		widget_image, //图片控件
		widget_image_sequence, //图片序列控件
		widget_label,  //文本控件
		widget_button, //按钮
		widget_text_buttn,  //文本按钮
		widget_check_button, //单选框
		widget_editbox,
		widget_scrollbar_v,
		widget_scrollbar_h,
		widget_progressBar,
		widget_form,  //面板
	};
	enum EModiFyType
	{
		Add_Flag,
		Minus_Flag
	};

	struct stUiImage
	{
		CDexTex* m_tex;
		DexRect  m_srcArea; //纹理中的区域
		bool     m_Mirro;  //是否镜像
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
	CEventDispather  m_MouseMUp;  //滚轮按下
	CEventDispather  m_MouseMDown;
	CEventDispather  m_MouseWheelFront; //滚轮前滚
	CEventDispather  m_MouseWheelBack;  //滚轮后滚
	CEventDispather  m_MouseLDrag;  
	CEventDispather  m_MouseRDrag;  
	CEventDispather  m_KeyUp;  
	CEventDispather  m_KeyDown;  
	CEventDispather  EDResize;
	CEventDispather  EDSetPos;

protected:
	virtual void ShutDown() ;  //在此释放资源，析构函数不进行资源释放

protected:
	bool        m_bClipByParent;    //是否被父节点裁剪
	bool	    m_enable;           //是否有效
	bool	    m_visible;	        //是否可见
	bool        m_mask;             //是否开启蒙板
	bool        m_mouseOn;          //鼠标是否停留在上面
	DexColor    m_maskColor;        //蒙板颜色
	int         m_initAlpha;        //初始透明度 无论之后如果更改透明度 该值都不变
	int         m_alpha;            //透明度
	int		    m_flag;				//接受哪些鼠标事件 如果不接受鼠标事件 则置为mouse_none
	int         m_id;
	float       m_rotate;           //旋转角度
	EDraw_Flag m_Drawflag;
	EWidgetType m_type;
	std::string m_name;  //控件名称
	DexPointF m_offFatherPos; //相对父UI的偏移量
	DexRectF  m_rect;         //占据的区域
	DexRectF  m_rectAfterClip; //被父节点裁剪后占据的区域
	string m_mouseOnSoundFilename;
	string m_mouseDownSoundFilename;
	string m_mouseDownUnvalid; //如果一个控件无效 鼠标点击的声音
	//std::list<CDexGuiValidArea*> m_area;  //实际占据的区域列表

protected:
	list<CDexGuiAction*> m_actions;
	DexWidget* m_father;       //父widget
	DexWidget* m_left_sibling; //同一层级的上个widget
	DexWidget* m_right_sibling; //同一层级的下个widget
	CDexWidgetContainer* m_children;

public:
	virtual void OnKeyChar(stEvent event); //处理字符按键
	virtual void OnKeyDown(stEvent event); //处理字符以外的按键
protected:
	//子类重载以下方法进行特殊处理
	virtual bool OnMouseMove(stEvent event);//返回值表明是否真正對事件進行了處理
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

	//子类重载以下方法进行特殊判断
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

	bool CheckPointIn(const DexPoint& pt); //判断一点是否落在控件内
	void ReCalculateUV(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3, const DexRectF& srcRect, const DexRectF& clipRect);//控件被裁减之后，重新计算裁减矩形对应的uv
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

	virtual void RenderRect(); //渲染包围矩形框
	virtual void RenderMask(); //渲染蒙板

public:
	virtual bool OnEvent(stEvent event);	  //监听到事件，
	virtual bool EventValid(stEvent event);   //决定是否处理该事件

public:
	int  GetId() ;
	DexPointF GetPos();
	DexRectF  GetArea();   //如果被父节点裁剪 则返回裁剪后的区域
	DexRectF  GetAreaReal(); //返回真正所占的区域 不计算裁剪
	void SetClipByParent(bool clipByParent);
	bool GetClipByParent();
	bool GetVisible();
	void setVisible(bool visible);
	CDexWidgetContainer* GetChildrenContainer();
public:
	void AddAction(CDexGuiAction* action);
	void EraseAction(CDexGuiAction* action); //删除action，action本身进行update，判断完成后，调用Owner将自己删除
	void StopMoveAction();  //如果widget在移动，停止，即删除掉所有move action
	virtual void ModifyFlag(EModiFyType i_o, int flag);
	virtual void Enable(bool child = false); //是否关联到所有的子控件，false:只影响自身
	virtual void Disable(bool child = false);//是否关联到所有的子控件，false:只影响自身
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
	void SetPos(const DexPoint& pos);  //设置新坐标
	void SetPos(const DexPointF& pos); 
	void SetPos(int x, int y);
	void setFatherOffset(float x, float y);
	DexPointF getFatherOffset();
	void OpenMask(bool open);
	void SetMaskColor(DUChar r, DUChar g, DUChar b);
	void SetMaskAlpha(DUChar a);
	void SetMaskColor(const DexColor& color);
	void SetMaskColor(DUChar r, DUChar g, DUChar b, DUChar a);
 
	//用初始透明度来重置自身的alpha childeren：是否让子控件重置子控件的透明度
	//void ResetAlphaByInitAlpha(bool children);
	void SetInitAlpha(int alpha);
	int  GetInitAlpha();
	void SetAlpha(int alpha, bool children = false); //children:设置alpha值后 是否更改子控件的alpha
	int  GetOnlyAlpha();
	void OffSet(const DexPoint& delta);  //平移相应的数值
	void OffSet(const DexPointF& delta); 
	 //缩放 是否影响子控件 是否需要根据父控件的位置进行自适应
	void Scale(float x, float y, bool scaleChild = false, bool changePos = false);
	virtual void Resize(const DexSize& size, bool resizeChild = false);  //重新设置尺寸大小 设为虚函数 各子类可能要进行不同的处理
	virtual void Resize(const DexSizeF& size, bool resizeChild = false);
	virtual void Resize(DInt32 width, DInt32 height, DBool resizeChild = false);
	void SetRightSibling(DexWidget* widget) { m_right_sibling = widget;};
	void SetLeftSibling(DexWidget* widget) { m_left_sibling = widget;};
public:
};



#endif