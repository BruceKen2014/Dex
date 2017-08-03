

#pragma once

#include "..\DexBase\CDexObject.h"
#include "DexGuiStruct.h"
class CDexWidget;
using namespace DexGUI;
#define ACTION_MOVE_TIMER 100  //移动timer ID 100ms改变一次移动速度
#define ACTION_SIZE_TIMER 101  //尺寸更改timer ID
#define ACTION_SIZE_INTERVAL 15 //尺寸更改间隔15ms

enum EUiActionType
{
	EAT_NONE,
	EAT_MOVE,    //位置改变action
	EAT_SIZE,    //尺寸改变action
	EAT_ALPHA,   //alpha改变action
	EAT_ROTATE,  //rotate action
};
class CDexGuiAction:public CDexObject
{
protected:
	EUiActionType m_type;
	int           m_actionId;
	string      m_name;  //动作名称（方便调试查看）
	CDexWidget* m_owner;
	bool        m_valid;   //动作是否有效
	bool        m_complete; //动作是否完成
	bool        m_deleteSelf;  //complete之后是否删除自己
	DInt64         m_iAlreadDelta;   //action已经执行的时间 单位ms

public:
	CDexGuiAction();
	virtual ~CDexGuiAction();
protected:
	virtual void BeforAction(); //动作执行前调用
	virtual void AfterAction(); //动作执行后调用
	virtual void UpdateThis(int delta) = 0;
	virtual void ShutDown() = 0;
public:
	virtual bool Update(int delta);
	virtual void BeginAction();
	//virtual void ReBeginAction(); //用于action执行完之后不删除自身，需要再次激活自己的情况

	EUiActionType GetActionType();

	void SetName(string name);
	void SetId(int id);
	int  GetId();
	string GetName();
	void ConstrainEnd();  //强制结束action
	bool GetComplete();
	void SetDeleteSelf(bool deleteSelf);
	virtual bool GetWidgetHandle() = 0;
	virtual void SetOwner(CDexWidget* owner);
};


//========================================================================================================

enum EGuiMoveType
{
	GuiMove_NORMAL,  //匀速移动
					 //与GuiMove_NORMAL相关的EGuiMoveParamType参数：GuiMove_V_T、GuiMove_S_T
	GuiMove_ACCE,    //匀加速移动
	
	GuiMove_VACCE,   //匀减速移动
					 //与GuiMove_VACCE相关的EGuiMoveParamType参数：GuiMove_V_A、GuiMove_S_A_T

	GuiMove_ACCN,    //先加速，到中间位置再减速
};
enum EGuiMoveParamType
{//move action 输入参数
	GuiMove_P_N,  //无效
	GuiMove_V_A,  //初始速度 加速度
	GuiMove_V_T,  //初始速度 时间
	GuiMove_S_T,  //目标坐标 时间
	GuiMove_S_A_T,//目标坐标 时间 加速度
};
class CDexGuiActionMove: public CDexGuiAction
{//暂时只实现从一个点到另一个点的移动，不支持中间多个点，日后可以扩展
protected:
	EGuiMoveType m_moveType; 
	EGuiMoveParamType m_eMoveParamType;
	DexPointF   m_startPos; //开始位置
	DexPointF   m_currPos; //控件当前位置
	DexPointF   m_desPos;  //控件目标位置
	DexPointF   m_middlePos; //中间位置
	DexPointF   m_speed;   //速度
	int         m_oldFlag;
	bool        m_pastMiddle; //是否过了中点
	float       m_fAcceX;    //加速度
	float       m_fAcceY;
	int         m_iNeedTime; //本次action需要花费的时间 单位：ms
public:
	CDexGuiActionMove();
	virtual ~CDexGuiActionMove();

protected:
	virtual void UpdateThis(int delta);
	virtual void BeforAction();
	virtual void AfterAction();
	virtual void ShutDown();
	void         CalParam();  //计算参数
public:
	virtual void OnTimer(const stTimer& timer);
	virtual void SetOwner(CDexWidget* owner); //只能通过设置绑定控件时设定m_startPos
	void SetMoveType(EGuiMoveType type);
	void SetMoveParamType(EGuiMoveParamType type);
	void SetDestPos(DexPoint desPos);
	void SetDestPos(int x, int y);

	//必须在调用SetOwner SetDestPos才能调用该函数 
	void SetSpeed(float speed);//传入x y 方向上的总速度，内部计算分解为x y方向速度
	void SetAccelerate(float acce);

	virtual bool GetWidgetHandle();
	
	//取得移动完成的毫秒数 如 从一点到一点 需要500ms
	//当action在xy方向同时运动的时候 该方法不知道要取得那个方向上花费的时间
	//所以action在xy方向花费的时间不一样时 取花费最长的方向时间
	int GetTime(); 

	//给定初始速度、加速度 可与其他move action搭配使用
	void SetParam_V_A(float fSpeedX, float fSpeedY, float fAcceX, float fAcceY);
	//给定初始速度、时间   可与其他move action搭配使用
	void SetParam_V_T(float fSpeedX, float fSpeedY, int time);
	//给定目标位置、移动时间
	//此时目标的最终位置是确定的，所以不可以将该类型的move action与其他类型的move action搭配使用  否则最终位置会不准确
	void SetParam_S_T(float fDestX, float fDestY, int time);
	void SetParam_S_T(DexPointF desPos, int time);
	
	//给定目标坐标、加速度、时间
	//此时对应方向上的最大初始速度为2S/t 结束时正好速度为0 此时传入的acces=2s/t^2
	//最小速度为s/t 此时传入的acces = 0
	//参数EndZero 结束时速度是否为0（为true时fAcces不起效果）
	void SetParam_S_A_T(float fDestX, float fDestY, float fAcces, int fTime, bool EndZero = true);
	
};
//使用例子，move为action,m_backImage为绑定控件
//CDexGuiActionMove* move = new CDexGuiActionMove;
//move->SetName("移动action");
//m_backImage->AddAction(move);
//move->SetDestPos(300, 300);
//move->SetSpeed(6);
//move->SetAccelerate(3);
//move->SetMoveType(GuiMove_ACCE);
//move->BeginAction();

//========================================================================================================

class CDexGuiActionSizeChange: public CDexGuiAction
{
protected:
	bool m_changeChildren;  //改变尺寸时，是否改变子控件
	DexPointF  m_startSize;  //开始尺寸
	DexPointF  m_currSize;   //当前尺寸
	DexPointF  m_destSize;   //目标尺寸
	DexPointF m_sizeDelta;  //横纵更改量

public:
	CDexGuiActionSizeChange();
	virtual ~CDexGuiActionSizeChange();

protected:
	virtual void UpdateThis(int delta);
	virtual void BeforAction();
	virtual void AfterAction();
	virtual void ShutDown();
public:
	virtual void OnTimer(const stTimer& timer);
	virtual void SetOwner(CDexWidget* owner); //只能通过设置绑定控件时设定m_startRect
	void SetDestSize(DexPointF desSize);
	void SetDestSize(DexPoint desSize);
	void SetDestSize(int sizeX, int sizeY);
	void SetChangeChildren(bool changeChildren);
 
	void SetSizeMSeconds(int mseconds); //多久完成尺寸的改变 单位ms

	virtual bool GetWidgetHandle() { return false;};
};
//使用例子 change为action,m_backImage为绑定控件
//CDexGuiActionSizeChange* change = new CDexGuiActionSizeChange;
//change->SetName("尺寸改变aciton");
//m_backImage->AddAction(change);
//change->SetDestSize(50, 50);
//change->SetSizeMSeconds(1000);
//change->SetChangeChildren(true);
//change->BeginAction();

//========================================================================================================

enum EAlphaType
{
	EATA_TOP_STOP, //alpha不断增加，达到上限后停止
	EATA_LOW_STOP, //alpha不断减少，达到下限后停止
	EATA_CYCLE  ,  //在上限下限之间循环
};
class CDexGuiActionAlphaChange: public CDexGuiAction
{
protected:
	EAlphaType m_alphaType;
	int        m_alphaTop;  //alpha变化上限
	int        m_alphaLow;  //alpha变化下限
	float      m_ownerAlpha;  //保存当前owner alpha,float值
	float      m_alphaDelta; //每ms的alpha更改量
	int        m_alphaChangeTime; //完成alpha的时间 单位ms
	bool       m_bigger; //alpha是否正在增加
	bool       m_children; //alpha action是否影响子控件

public:
	CDexGuiActionAlphaChange();
	virtual ~CDexGuiActionAlphaChange();

protected:
	virtual void UpdateThis(int delta);
	virtual void BeforAction();
	virtual void AfterAction();
	virtual void ShutDown();

	void UpdateAlphaChangeTime();
public:
	virtual void SetOwner(CDexWidget* owner);
	void SetAlphaChangeType(EAlphaType type);
	void SetAlphaTop(int alpha);
	void SetAlphaLow(int alpha);
	void SetAlphaRange(int top, int low);
	void SetAlphaChangeTime(int times);

	virtual bool GetWidgetHandle() { return true;};
};
//例
//CDexGuiActionAlphaChange* alpha = new CDexGuiActionAlphaChange;
//alpha->SetName("alpha改变action");
//m_backImage->AddAction(alpha);
//alpha->SetAlphaChangeType(EATA_TOP_STOP);
//alpha->SetAlphaTop(m_backImage->GetInitAlpha());
//alpha->SetAlphaChangeTime(1000);
////m_backImage->Show();
//alpha->BeginAction();

//========================================================================================================

#define  GUI_ACTION_FOREVER_CYCLE   -1
//只针对控件自身 不影响子控件
class CDexGuiActionRotate: public CDexGuiAction
{
protected:
	int        m_cyclesPerSecond;   //每秒转几圈 负数表示往反方向转
	float      m_rotatePerMSecond;  //每毫秒所转的弧度(根据m_cyclesPerSecond计算得来) 负数表示往反方向转
	int        m_cycles;            //共需转几圈 -1表示无限旋转
	float      m_rotateTemp;        //未转足一圈时的弧度计数
	int        m_alreadyCycles;     //已经转的圈数

public:
	CDexGuiActionRotate();
	virtual ~CDexGuiActionRotate();

protected:
	virtual void UpdateThis(int delta);
	virtual void BeforAction();
	virtual void AfterAction();
	virtual void ShutDown();

public:
	void SetCyclesPerSecond(float cyclesPerSecond);
	void SetCycles(int cycles);

	virtual bool GetWidgetHandle() { return true;};
};