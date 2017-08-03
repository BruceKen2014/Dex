

#pragma once

#include "..\DexBase\CDexObject.h"
#include "DexGuiStruct.h"
class CDexWidget;
using namespace DexGUI;
#define ACTION_MOVE_TIMER 100  //�ƶ�timer ID 100ms�ı�һ���ƶ��ٶ�
#define ACTION_SIZE_TIMER 101  //�ߴ����timer ID
#define ACTION_SIZE_INTERVAL 15 //�ߴ���ļ��15ms

enum EUiActionType
{
	EAT_NONE,
	EAT_MOVE,    //λ�øı�action
	EAT_SIZE,    //�ߴ�ı�action
	EAT_ALPHA,   //alpha�ı�action
	EAT_ROTATE,  //rotate action
};
class CDexGuiAction:public CDexObject
{
protected:
	EUiActionType m_type;
	int           m_actionId;
	string      m_name;  //�������ƣ�������Բ鿴��
	CDexWidget* m_owner;
	bool        m_valid;   //�����Ƿ���Ч
	bool        m_complete; //�����Ƿ����
	bool        m_deleteSelf;  //complete֮���Ƿ�ɾ���Լ�
	DInt64         m_iAlreadDelta;   //action�Ѿ�ִ�е�ʱ�� ��λms

public:
	CDexGuiAction();
	virtual ~CDexGuiAction();
protected:
	virtual void BeforAction(); //����ִ��ǰ����
	virtual void AfterAction(); //����ִ�к����
	virtual void UpdateThis(int delta) = 0;
	virtual void ShutDown() = 0;
public:
	virtual bool Update(int delta);
	virtual void BeginAction();
	//virtual void ReBeginAction(); //����actionִ����֮��ɾ��������Ҫ�ٴμ����Լ������

	EUiActionType GetActionType();

	void SetName(string name);
	void SetId(int id);
	int  GetId();
	string GetName();
	void ConstrainEnd();  //ǿ�ƽ���action
	bool GetComplete();
	void SetDeleteSelf(bool deleteSelf);
	virtual bool GetWidgetHandle() = 0;
	virtual void SetOwner(CDexWidget* owner);
};


//========================================================================================================

enum EGuiMoveType
{
	GuiMove_NORMAL,  //�����ƶ�
					 //��GuiMove_NORMAL��ص�EGuiMoveParamType������GuiMove_V_T��GuiMove_S_T
	GuiMove_ACCE,    //�ȼ����ƶ�
	
	GuiMove_VACCE,   //�ȼ����ƶ�
					 //��GuiMove_VACCE��ص�EGuiMoveParamType������GuiMove_V_A��GuiMove_S_A_T

	GuiMove_ACCN,    //�ȼ��٣����м�λ���ټ���
};
enum EGuiMoveParamType
{//move action �������
	GuiMove_P_N,  //��Ч
	GuiMove_V_A,  //��ʼ�ٶ� ���ٶ�
	GuiMove_V_T,  //��ʼ�ٶ� ʱ��
	GuiMove_S_T,  //Ŀ������ ʱ��
	GuiMove_S_A_T,//Ŀ������ ʱ�� ���ٶ�
};
class CDexGuiActionMove: public CDexGuiAction
{//��ʱֻʵ�ִ�һ���㵽��һ������ƶ�����֧���м����㣬�պ������չ
protected:
	EGuiMoveType m_moveType; 
	EGuiMoveParamType m_eMoveParamType;
	DexPointF   m_startPos; //��ʼλ��
	DexPointF   m_currPos; //�ؼ���ǰλ��
	DexPointF   m_desPos;  //�ؼ�Ŀ��λ��
	DexPointF   m_middlePos; //�м�λ��
	DexPointF   m_speed;   //�ٶ�
	int         m_oldFlag;
	bool        m_pastMiddle; //�Ƿ�����е�
	float       m_fAcceX;    //���ٶ�
	float       m_fAcceY;
	int         m_iNeedTime; //����action��Ҫ���ѵ�ʱ�� ��λ��ms
public:
	CDexGuiActionMove();
	virtual ~CDexGuiActionMove();

protected:
	virtual void UpdateThis(int delta);
	virtual void BeforAction();
	virtual void AfterAction();
	virtual void ShutDown();
	void         CalParam();  //�������
public:
	virtual void OnTimer(const stTimer& timer);
	virtual void SetOwner(CDexWidget* owner); //ֻ��ͨ�����ð󶨿ؼ�ʱ�趨m_startPos
	void SetMoveType(EGuiMoveType type);
	void SetMoveParamType(EGuiMoveParamType type);
	void SetDestPos(DexPoint desPos);
	void SetDestPos(int x, int y);

	//�����ڵ���SetOwner SetDestPos���ܵ��øú��� 
	void SetSpeed(float speed);//����x y �����ϵ����ٶȣ��ڲ�����ֽ�Ϊx y�����ٶ�
	void SetAccelerate(float acce);

	virtual bool GetWidgetHandle();
	
	//ȡ���ƶ���ɵĺ����� �� ��һ�㵽һ�� ��Ҫ500ms
	//��action��xy����ͬʱ�˶���ʱ�� �÷�����֪��Ҫȡ���Ǹ������ϻ��ѵ�ʱ��
	//����action��xy���򻨷ѵ�ʱ�䲻һ��ʱ ȡ������ķ���ʱ��
	int GetTime(); 

	//������ʼ�ٶȡ����ٶ� ��������move action����ʹ��
	void SetParam_V_A(float fSpeedX, float fSpeedY, float fAcceX, float fAcceY);
	//������ʼ�ٶȡ�ʱ��   ��������move action����ʹ��
	void SetParam_V_T(float fSpeedX, float fSpeedY, int time);
	//����Ŀ��λ�á��ƶ�ʱ��
	//��ʱĿ�������λ����ȷ���ģ����Բ����Խ������͵�move action���������͵�move action����ʹ��  ��������λ�û᲻׼ȷ
	void SetParam_S_T(float fDestX, float fDestY, int time);
	void SetParam_S_T(DexPointF desPos, int time);
	
	//����Ŀ�����ꡢ���ٶȡ�ʱ��
	//��ʱ��Ӧ�����ϵ�����ʼ�ٶ�Ϊ2S/t ����ʱ�����ٶ�Ϊ0 ��ʱ�����acces=2s/t^2
	//��С�ٶ�Ϊs/t ��ʱ�����acces = 0
	//����EndZero ����ʱ�ٶ��Ƿ�Ϊ0��ΪtrueʱfAcces����Ч����
	void SetParam_S_A_T(float fDestX, float fDestY, float fAcces, int fTime, bool EndZero = true);
	
};
//ʹ�����ӣ�moveΪaction,m_backImageΪ�󶨿ؼ�
//CDexGuiActionMove* move = new CDexGuiActionMove;
//move->SetName("�ƶ�action");
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
	bool m_changeChildren;  //�ı�ߴ�ʱ���Ƿ�ı��ӿؼ�
	DexPointF  m_startSize;  //��ʼ�ߴ�
	DexPointF  m_currSize;   //��ǰ�ߴ�
	DexPointF  m_destSize;   //Ŀ��ߴ�
	DexPointF m_sizeDelta;  //���ݸ�����

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
	virtual void SetOwner(CDexWidget* owner); //ֻ��ͨ�����ð󶨿ؼ�ʱ�趨m_startRect
	void SetDestSize(DexPointF desSize);
	void SetDestSize(DexPoint desSize);
	void SetDestSize(int sizeX, int sizeY);
	void SetChangeChildren(bool changeChildren);
 
	void SetSizeMSeconds(int mseconds); //�����ɳߴ�ĸı� ��λms

	virtual bool GetWidgetHandle() { return false;};
};
//ʹ������ changeΪaction,m_backImageΪ�󶨿ؼ�
//CDexGuiActionSizeChange* change = new CDexGuiActionSizeChange;
//change->SetName("�ߴ�ı�aciton");
//m_backImage->AddAction(change);
//change->SetDestSize(50, 50);
//change->SetSizeMSeconds(1000);
//change->SetChangeChildren(true);
//change->BeginAction();

//========================================================================================================

enum EAlphaType
{
	EATA_TOP_STOP, //alpha�������ӣ��ﵽ���޺�ֹͣ
	EATA_LOW_STOP, //alpha���ϼ��٣��ﵽ���޺�ֹͣ
	EATA_CYCLE  ,  //����������֮��ѭ��
};
class CDexGuiActionAlphaChange: public CDexGuiAction
{
protected:
	EAlphaType m_alphaType;
	int        m_alphaTop;  //alpha�仯����
	int        m_alphaLow;  //alpha�仯����
	float      m_ownerAlpha;  //���浱ǰowner alpha,floatֵ
	float      m_alphaDelta; //ÿms��alpha������
	int        m_alphaChangeTime; //���alpha��ʱ�� ��λms
	bool       m_bigger; //alpha�Ƿ���������
	bool       m_children; //alpha action�Ƿ�Ӱ���ӿؼ�

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
//��
//CDexGuiActionAlphaChange* alpha = new CDexGuiActionAlphaChange;
//alpha->SetName("alpha�ı�action");
//m_backImage->AddAction(alpha);
//alpha->SetAlphaChangeType(EATA_TOP_STOP);
//alpha->SetAlphaTop(m_backImage->GetInitAlpha());
//alpha->SetAlphaChangeTime(1000);
////m_backImage->Show();
//alpha->BeginAction();

//========================================================================================================

#define  GUI_ACTION_FOREVER_CYCLE   -1
//ֻ��Կؼ����� ��Ӱ���ӿؼ�
class CDexGuiActionRotate: public CDexGuiAction
{
protected:
	int        m_cyclesPerSecond;   //ÿ��ת��Ȧ ������ʾ��������ת
	float      m_rotatePerMSecond;  //ÿ������ת�Ļ���(����m_cyclesPerSecond�������) ������ʾ��������ת
	int        m_cycles;            //����ת��Ȧ -1��ʾ������ת
	float      m_rotateTemp;        //δת��һȦʱ�Ļ��ȼ���
	int        m_alreadyCycles;     //�Ѿ�ת��Ȧ��

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