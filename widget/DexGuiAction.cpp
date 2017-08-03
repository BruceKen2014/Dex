

#include "..\DexBase\DexLog.h"
#include "DexWidget.h"
#include "DexGuiAction.h"

CDexGuiAction::CDexGuiAction()
{
	m_type = EAT_NONE;
	m_owner = NULL;
	m_actionId = 0;
	m_iAlreadDelta = 0;
	m_complete = false;
	m_valid = false;
	m_deleteSelf = true;
}

CDexGuiAction::~CDexGuiAction()
{
	ShutDown();
}

void CDexGuiAction::ShutDown()
{
	if(m_owner != NULL)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_ok, "�ؼ�%s���ٶ���%s", m_owner->GetName().c_str() , m_name.c_str());
		DexLog::getSingleton()->EndLog();
	}

}

bool CDexGuiAction::GetComplete()
{
	return m_complete;
}

void CDexGuiAction::SetDeleteSelf(bool deleteSelf)
{
	m_deleteSelf = deleteSelf;
}

bool CDexGuiAction::Update(int delta)
{
	DEX_ENSURE_B(CDexObject::Update(delta));
	m_iAlreadDelta += delta;
	UpdateThis(delta);
	if(m_complete)
		AfterAction();
	return true;
}

void CDexGuiAction::BeginAction()
{
	BeforAction();
	m_valid = true;
}

EUiActionType CDexGuiAction::GetActionType()
{
	return m_type;
}

void CDexGuiAction::SetName(string name)
{
	m_name = name;
}

void CDexGuiAction::SetId(int id)
{
	m_actionId = id;
}

int CDexGuiAction::GetId()
{
	return m_actionId;
}

string CDexGuiAction::GetName()
{
	return m_name;
}
void CDexGuiAction::ConstrainEnd()
{
	m_complete = true;
}
void CDexGuiAction::BeforAction()
{
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_ok, "����%s��ʼ",m_name.c_str());
	DexLog::getSingleton()->EndLog();
}

void CDexGuiAction::AfterAction()
{
	DexLog::getSingleton()->BeginLog();
	DexLog::getSingleton()->Log(log_ok, "����%s����",m_name.c_str());
	DexLog::getSingleton()->EndLog();
	if(m_deleteSelf)
		m_owner->EraseAction(this);
}

void CDexGuiAction::SetOwner(CDexWidget* owner)
{
	m_owner = owner;
}

//=================================================================================

CDexGuiActionMove::CDexGuiActionMove()
{
	m_type = EAT_MOVE;
	m_pastMiddle = false;
	m_moveType = GuiMove_NORMAL;
	m_eMoveParamType = GuiMove_P_N;
	m_fAcceX = 0.1f;
	m_fAcceY = 0.1f;
	m_iNeedTime = 0;
	AddTimer(ACTION_MOVE_TIMER, 100); //100ms��һ���ٶ�
}
CDexGuiActionMove::~CDexGuiActionMove()
{

}
void CDexGuiActionMove::ShutDown()
{

}

void CDexGuiActionMove::CalParam()
{
	float f_time_x = 0.0f; 
	float f_time_y = 0.0f;
	switch(m_moveType)
	{
	case GuiMove_VACCE:
		{//�ȼ���
			switch(m_eMoveParamType)
			{
			case GuiMove_V_A:
				{//������ʼ�ٶ� ���ٶ�
					if(m_speed.x > 0)
					{//��ʼ�ٶ�Ϊ�� Ϊ�����ȼ����ƶ�
						m_fAcceX = -abs(m_fAcceX); //���ٶ�Ϊ��
					}
					else if(m_speed.x < 0)
					{//��ʼ�ٶ�Ϊ�� Ϊ�����ȼ����˶�
						m_fAcceX = abs(m_fAcceX); //���ٶ�Ϊ��
					}
					if(m_speed.y > 0)
					{//��ʼ�ٶ�Ϊ�� Ϊ�����ȼ����ƶ�
						m_fAcceY = -abs(m_fAcceY); //���ٶ�Ϊ��
					}
					else if(m_speed.y < 0)
					{//��ʼ�ٶ�Ϊ�� Ϊ�����ȼ����˶�
						m_fAcceY = abs(m_fAcceY); //���ٶ�Ϊ��
					}
					if(m_fAcceX != 0)
						f_time_x = m_speed.x / m_fAcceX * 1000; 
					if(m_fAcceY != 0)
						f_time_y = m_speed.y / m_fAcceY * 1000;
					m_iNeedTime = (int)(f_time_x > f_time_y ? f_time_x : f_time_y);
					break;
				}
			case GuiMove_S_A_T:
				{
					if(m_iNeedTime != 0)
					{
						if(m_desPos.x > m_startPos.x)
						{//X��ǰ�ȼ���
							m_fAcceX = -abs(m_fAcceX); //���ٶ�Ϊ��
						}
						else if(m_desPos.x < m_startPos.x)
						{
							m_fAcceX = abs(m_fAcceX);
						}
						if(m_desPos.y > m_startPos.y)
						{//X��ǰ�ȼ���
							m_fAcceY = -abs(m_fAcceY); //���ٶ�Ϊ��
						}
						else if(m_desPos.y < m_startPos.y)
						{
							m_fAcceY = abs(m_fAcceY);
						}
						float f_temp_time = (float)m_iNeedTime/1000; //ת��Ϊs
						m_speed.x = (m_desPos.x - m_startPos.x)/f_temp_time - m_fAcceX*f_temp_time/2;
						m_speed.y = (m_desPos.y - m_startPos.y)/f_temp_time - m_fAcceY*f_temp_time/2;
					}
				}
			}
			break;
		}
	case GuiMove_NORMAL:
		{
			switch(m_eMoveParamType)
			{
			case GuiMove_V_T:
				{
					break;
				}
			case GuiMove_S_T:
				{
					m_speed.x = (int)(1000*(m_desPos.x - m_startPos.x) / m_iNeedTime);
					m_speed.y = (int)(1000*(m_desPos.y - m_startPos.y) / m_iNeedTime);
					break;
				}
			}
			break;
		}
	}

}
void CDexGuiActionMove::OnTimer(const stTimer& timer)
{
	int id = timer.id;
	DEX_ENSURE(m_owner);
	if( id == ACTION_MOVE_TIMER)
	{
		switch(m_moveType)
		{
		case GuiMove_VACCE:
			{//�ȼ���
				switch(m_eMoveParamType)
				{
				case GuiMove_V_A:	
				case GuiMove_S_A_T:
					{
						float f_old_speed_x = m_speed.x; 
						m_speed.x += m_fAcceX/10; //�����m_fAcceX��һ��Ҫ���ٵ��� timer��100ms ����Ҫ����10
						if(f_old_speed_x * m_speed.x <= 0)
							//���ٵ��׺� �������������ƶ� ���ٶ�����Ϊ0
							m_speed.x = 0;
						float f_old_speed_y = m_speed.y; 
						m_speed.y += m_fAcceY/10;
						if(f_old_speed_y * m_speed.y <= 0)
							//���ٵ��׺� �������������ƶ� ���ٶ�����Ϊ0
							m_speed.y = 0;
						if(m_speed.x == 0 && m_speed.y == 0)
							//�ƶ�ֹͣ �����action
						{
							m_complete = true;
							return;
						}
						break;
					}
				}
			}
			break;
		}

	}
}


void CDexGuiActionMove::UpdateThis(int delta)
{
	//����λ��ʱ ��x y��������ֱ����
	DEX_ENSURE(m_owner);
	if(m_valid)
	{
		switch(m_moveType)
		{
		case GuiMove_VACCE:
			{//�ȼ���
				switch(m_eMoveParamType)
				{
				case GuiMove_V_A:
					{//������ʼ�ٶ� ���ٶ�
						m_currPos = m_owner->GetPos();
						m_currPos.x += m_speed.x * delta/1000;
						m_currPos.y += m_speed.y * delta/1000;
						m_owner->SetPos(m_currPos);
						break;
					}
				case GuiMove_S_A_T:
					{
						m_currPos = m_owner->GetPos();
						m_currPos.x += m_speed.x * delta/1000;
						m_currPos.y += m_speed.y * delta/1000;
						m_owner->SetPos(m_currPos);
						if(m_iAlreadDelta > m_iNeedTime)
							m_complete = true;
						break;
					}
				}
				break;
			}
		case  GuiMove_NORMAL:
			{//����
				if(m_iAlreadDelta > m_iNeedTime)
				{//�����˶���ʱ�䵽
					m_complete = true;
					return;
				}
				m_currPos = m_owner->GetPos();
				m_currPos.x += m_speed.x * delta/1000;
				m_currPos.y += m_speed.y * delta/1000;
				m_owner->SetPos(m_currPos);
				break;
			}
		}
		
	}
	//if(m_valid)
	//{
	//	if(m_currPos.x != m_desPos.x)
	//		m_currPos.x += m_speed.x;
	//	if(m_currPos.y != m_desPos.y)
	//		m_currPos.y += m_speed.y;
	//	if(m_desPos.x >= m_startPos.x)
	//	{//��x�������ƶ�
	//		if(m_currPos.x > m_desPos.x)
	//		{//x�Ѿ����� 
	//			m_currPos.x = m_desPos.x;
	//		}		
	//	}
	//	else
	//	{//��x�������ƶ�
	//		if(m_currPos.x < m_desPos.x)
	//		{//x�Ѿ����� 
	//			m_currPos.x = m_desPos.x;
	//		}	
	//	}
	//	if(m_desPos.y >= m_startPos.y)
	//	{//��y�������ƶ�
	//		if(m_currPos.y > m_desPos.y)
	//		{//x�Ѿ����� 
	//			m_currPos.y = m_desPos.y;
	//		}		
	//	}
	//	else
	//	{//��y�������ƶ�
	//		if(m_currPos.y < m_desPos.y)
	//		{//x�Ѿ����� 
	//			m_currPos.y = m_desPos.y;
	//		}	
	//	}
	//	m_owner->SetPos(m_currPos);
	//	if(abs(m_currPos.x - m_desPos.x) < abs(m_speed.x) &&
	//		abs(m_currPos.y - m_desPos.y) < abs(m_speed.y))
	//		m_complete = true;
	//	if(!m_pastMiddle)
	//	{//��δ���е�
	//		if(m_desPos.x >= m_startPos.x)
	//		{
	//			if(m_currPos.x >= m_middlePos.x)
	//			{
	//				if(m_desPos.y >= m_startPos.y)
	//				{
	//					if(m_currPos.y >= m_middlePos.y)
	//						m_pastMiddle = true;
	//				}
	//				else 
	//				{
	//					if(m_currPos.y <= m_middlePos.y)
	//						m_pastMiddle = true;
	//				}
	//			}
	//		}
	//		else 
	//		{
	//			if(m_currPos.x <= m_middlePos.x)
	//			{
	//				if(m_desPos.y >= m_startPos.y)
	//				{
	//					if(m_currPos.y >= m_middlePos.y)
	//						m_pastMiddle = true;
	//				}
	//				else 
	//				{
	//					if(m_currPos.y <= m_middlePos.y)
	//						m_pastMiddle = true;
	//				}
	//			}
	//		}

	//	}
	//}
}

void CDexGuiActionMove::BeforAction()
{
	CDexGuiAction::BeforAction();
	DEX_ENSURE(m_owner);
	m_owner->Disable(true);
	//m_oldFlag = m_owner->GetWidgetFlag();
	//m_owner->SetWidgetFlag(mouse_none);
}

void CDexGuiActionMove::AfterAction()
{
	DEX_ENSURE(m_owner);
	m_valid = false;
	m_owner->Enable(true);
	CDexGuiAction::AfterAction();
	//m_owner->SetWidgetFlag(m_oldFlag);
}
void CDexGuiActionMove::SetOwner(CDexWidget* owner)
{
	DEX_ENSURE(owner);
	CDexGuiAction::SetOwner(owner);
	m_startPos.x = owner->GetPos().x;
	m_startPos.y = owner->GetPos().y;
	m_currPos = m_startPos;
}

void CDexGuiActionMove::SetMoveType(EGuiMoveType type)
{
	m_moveType = type;
	CalParam();
}

void CDexGuiActionMove::SetMoveParamType(EGuiMoveParamType type)
{
	m_eMoveParamType = type;
	CalParam();
}
void CDexGuiActionMove::SetDestPos(DexPoint desPos)
{
	m_desPos.x = desPos.x;
	m_desPos.y = desPos.y;

	m_middlePos = (m_currPos + m_desPos)/2;
}

void CDexGuiActionMove::SetDestPos(int x, int y)
{
	m_desPos.x = x;
	m_desPos.y = y;
	m_middlePos = (m_currPos + m_desPos)/2;
}

void CDexGuiActionMove::SetSpeed(float speed)
{
	float xLength = m_desPos.x - m_currPos.x; //����Ϊ��ֵ �������������˶�
	float yLength = m_desPos.y - m_currPos.y; //
	float xyLength = sqrt(xLength*xLength + yLength*yLength);
	if(xyLength == 0)
		return;
	m_speed.x = speed * xLength/xyLength;
	m_speed.y = speed * yLength/xyLength;
	float bili = m_speed.x/m_speed.y;
}

void CDexGuiActionMove::SetAccelerate(float acce)
{
	m_fAcceX = acce;
	m_fAcceY = acce;
	CalParam();
}

bool CDexGuiActionMove::GetWidgetHandle()
{
	return false;
}

int CDexGuiActionMove::GetTime()
{
	return  m_iNeedTime;
}

void CDexGuiActionMove::SetParam_V_A(float fSpeedX, float fSpeedY, float fAcceX, float fAcceY)
{
	m_speed.x = fSpeedX;
	m_speed.y = fSpeedY;
	m_fAcceX = fAcceX;
	m_fAcceY = fAcceY;
	CalParam();
}
void CDexGuiActionMove::SetParam_V_T(float fSpeedX, float fSpeedY, int time)
{
	m_speed.x = fSpeedX;
	m_speed.y = fSpeedY;
	m_iNeedTime = time;
}

void CDexGuiActionMove::SetParam_S_T(float fDestX, float fDestY, int time)
{
	m_desPos.x = fDestX;
	m_desPos.y = fDestY;
	m_iNeedTime = time;
	CalParam();
}

void CDexGuiActionMove::SetParam_S_T(DexPointF desPos, int time)
{
	m_desPos = desPos;
	m_iNeedTime = time;
	CalParam();
}

void CDexGuiActionMove::SetParam_S_A_T(float fDestX, float fDestY, float fAcces, int fTime, bool EndZero)
{
	m_desPos.x = fDestX;
	m_desPos.y = fDestY;
	m_fAcceX = fAcces;
	m_fAcceY = fAcces;
	m_iNeedTime = fTime;
	if(EndZero)
	{
		float SX = abs(m_desPos.x - m_startPos.x);
		float SY = abs(m_desPos.y - m_startPos.y); 
		m_fAcceX = 2*SX/pow((float)m_iNeedTime/1000, 2.0f);
		m_fAcceY = 2*SY/pow((float)m_iNeedTime/1000, 2.0f);
	}
	CalParam();
}


//=======================================================================================

CDexGuiActionSizeChange::CDexGuiActionSizeChange()
{
	m_type = EAT_SIZE;
	m_changeChildren = false;
	AddTimer(ACTION_SIZE_TIMER, 20);//20ms
}

CDexGuiActionSizeChange::~CDexGuiActionSizeChange()
{
}

void CDexGuiActionSizeChange::ShutDown()
{

}
void CDexGuiActionSizeChange::BeforAction()
{
	CDexGuiAction::BeforAction();
}

void CDexGuiActionSizeChange::AfterAction()
{
	m_valid = false;
	CDexGuiAction::AfterAction();
	
	//m_owner->SetWidgetFlag(m_oldFlag);
}

void CDexGuiActionSizeChange::UpdateThis(int delta)
{
/*ԭ�����´����Ƿ���timer���еģ������������Ļ�����һ��panelһ���ƶ�һ������ʱ���ӿؼ��������صĶ���
ԭ�����ƶ�actionÿ֡��update�ж��д��������ŵ�timer��һ����������ɿ�������һ֡��move�ı��˸��ӿؼ���λ�ã�
��һ֡������Ч���ˣ����ӿؼ��Ĵ�С�����˵����������ӿؼ���������Ķ������
*/
	DEX_ENSURE(m_owner);
	DEX_ENSURE(m_valid);
	float xDelta = m_sizeDelta.x*(float(delta)/ACTION_SIZE_INTERVAL);
	float yDelta = m_sizeDelta.y*(float(delta)/ACTION_SIZE_INTERVAL);

	m_currSize.x += xDelta; 
	m_currSize.y += yDelta;
	DexSizeF size(m_currSize.x, m_currSize.y);
	m_owner->Resize(size, m_changeChildren);	
	if(abs(m_currSize.x - m_destSize.x) < abs(xDelta))
	{//�ߴ�������
		DexSize size(m_destSize.x, m_destSize.y);
		m_owner->Resize(size, m_changeChildren);
		m_complete = true;
	}
}

void CDexGuiActionSizeChange::OnTimer(const stTimer& timer)
{
	DEX_ENSURE(m_owner);
	DEX_ENSURE(m_valid);
	if(timer.id == ACTION_SIZE_TIMER)
	{	
		//timerֻ��ʾ��ȥ��ʱ�����Դ���һ����ʱ��������ǰ��ȥ��ʱ�䲢��һ����interval
		//ʵ�ʹ�ȥ��ʱ���� delay,����Ҫ����
		//float xDelta = m_sizeDelta.x*(float(timer.delay)/ACTION_SIZE_INTERVAL);
		//float yDelta = m_sizeDelta.y*(float(timer.delay)/ACTION_SIZE_INTERVAL);

		//m_currSize.x += xDelta; 
		//m_currSize.y += yDelta;
		//m_owner->Resize(m_currSize, m_changeChildren);	
		//if(abs(m_currSize.x - m_destSize.x) < abs(xDelta))
		//{//�ߴ�������
		//	m_tempTime = getTime()->GetTotalMillSeconds() - m_tempTime;
		//	m_owner->Resize(m_destSize, m_changeChildren);
		//	m_complete = true;
		//}

	}
}

void CDexGuiActionSizeChange::SetOwner(CDexWidget* owner)
{
	DEX_ENSURE(owner);
	CDexGuiAction::SetOwner(owner);
	m_startSize = owner->GetSize();
	m_currSize = m_startSize;
}

void CDexGuiActionSizeChange::SetDestSize(DexPointF desSize)
{
	m_destSize = desSize;
}

void CDexGuiActionSizeChange::SetDestSize(int sizeX, int sizeY)
{
	SetDestSize(DexPointF(sizeX, sizeY));
}

void CDexGuiActionSizeChange::SetDestSize(DexPoint desSize)
{
	SetDestSize(DexPointF(desSize.x, desSize.y));
}

void CDexGuiActionSizeChange::SetChangeChildren(bool changeChildren)
{
	m_changeChildren = changeChildren;
}
void CDexGuiActionSizeChange::SetSizeMSeconds(int mseconds)
{
	float width_delta = m_destSize.x - m_startSize.x;
	float height_delta = m_destSize.y - m_startSize.y;

	m_sizeDelta.x = width_delta*ACTION_SIZE_INTERVAL/mseconds;
	if(height_delta > 0)
		m_sizeDelta.y = abs(m_sizeDelta.x * height_delta/width_delta);
	else
		m_sizeDelta.y = -abs(m_sizeDelta.x* height_delta/width_delta);
}

//=======================================================================================

CDexGuiActionAlphaChange::CDexGuiActionAlphaChange()
{
	m_alphaTop = 255;
	m_alphaLow = 0;
	m_alphaDelta = 0.0f;
	m_alphaChangeTime = 0;
	m_ownerAlpha = 0.0f;
	m_bigger = false;
	m_alphaType = EATA_TOP_STOP;
	m_children = true;
}

CDexGuiActionAlphaChange::~CDexGuiActionAlphaChange()
{

}

void CDexGuiActionAlphaChange::ShutDown()
{

}

void CDexGuiActionAlphaChange::UpdateThis(int delta)
{
	DEX_ENSURE(m_valid);
	DEX_ENSURE(m_owner);
	int alphaDelta = m_alphaDelta*delta;
	switch (m_alphaType)
	{
	case EATA_TOP_STOP:
		{
			if(m_ownerAlpha < m_alphaTop)
			{
				m_ownerAlpha += alphaDelta;
				if(m_ownerAlpha > m_alphaTop)
				{
					m_ownerAlpha = m_alphaTop;
					m_complete = true;
				}
				m_owner->SetAlpha((int)m_ownerAlpha, m_children);
			}
			break;
		}
	case EATA_LOW_STOP:
		{
			if(m_ownerAlpha > m_alphaLow)
			{
				m_ownerAlpha -= alphaDelta;
				if(m_ownerAlpha < m_alphaLow)
				{
					m_ownerAlpha = m_alphaLow;
					m_complete = true;
				}
				m_owner->SetAlpha((int)m_ownerAlpha, m_children);
			}
			break;
		}
	case EATA_CYCLE:
		{
			if(m_bigger)
			{
				m_ownerAlpha += alphaDelta;
				if(m_ownerAlpha > m_alphaTop)
				{
					m_ownerAlpha = m_alphaTop;
					m_bigger = false;
				}	
			}
			else
			{
				m_ownerAlpha -= alphaDelta;
				if(m_ownerAlpha < m_alphaLow)
				{
					m_ownerAlpha = m_alphaLow;
					m_bigger = true;
				}
			}
			m_owner->SetAlpha((int)m_ownerAlpha, m_children);
			break;
		}
		
	}
}

void CDexGuiActionAlphaChange::BeforAction()
{
	CDexGuiAction::BeforAction();
	DEX_ENSURE(m_owner);
}

void CDexGuiActionAlphaChange::AfterAction()
{
	m_valid = false;
	CDexGuiAction::AfterAction();
	
}

void CDexGuiActionAlphaChange::UpdateAlphaChangeTime()
{
	DEX_ENSURE(m_alphaChangeTime != 0);
	DEX_ENSURE(m_owner != NULL);
	switch(m_alphaType)
	{
	case EATA_TOP_STOP:
		{
			int alpha_delta = m_alphaTop - m_owner->GetOnlyAlpha();
			DEX_ENSURE(alpha_delta > 0);
			m_alphaDelta = alpha_delta/float(m_alphaChangeTime);
			break;
		}

	case EATA_LOW_STOP:
		{
			int alpha_delta = m_owner->GetOnlyAlpha() - m_alphaLow;
			DEX_ENSURE(alpha_delta > 0);
			m_alphaDelta = alpha_delta/float(m_alphaChangeTime);
			break;
		}
	case EATA_CYCLE:
		{
			int alpha_delta = m_alphaTop - m_alphaLow;
			DEX_ENSURE(alpha_delta > 0);
			m_alphaDelta = alpha_delta/float(m_alphaChangeTime);
			break;
		}
	}
}

void CDexGuiActionAlphaChange::SetOwner(CDexWidget* owner)
{
	DEX_ENSURE(owner);
	CDexGuiAction::SetOwner(owner);
	m_ownerAlpha = m_owner->GetOnlyAlpha();
	m_alphaTop = m_owner->GetOnlyAlpha(); 
	m_alphaLow = m_owner->GetOnlyAlpha(); 
}

void CDexGuiActionAlphaChange::SetAlphaTop(int alpha)
{
	m_alphaTop = alpha;
	UpdateAlphaChangeTime();
}

void CDexGuiActionAlphaChange::SetAlphaLow(int alpha)
{
	m_alphaLow = alpha;
	UpdateAlphaChangeTime();
}

void CDexGuiActionAlphaChange::SetAlphaRange(int top, int low)
{
	DEX_ENSURE(top > low);
	m_alphaTop = top;
	m_alphaLow = low;
	UpdateAlphaChangeTime();
}

void CDexGuiActionAlphaChange::SetAlphaChangeTime(int times)
{
	m_alphaChangeTime = times;
	UpdateAlphaChangeTime();

}

void CDexGuiActionAlphaChange::SetAlphaChangeType(EAlphaType type)
{
	m_alphaType = type;
}

//=================================================================================

CDexGuiActionRotate::CDexGuiActionRotate()
{
	m_cyclesPerSecond = 0;
	m_rotatePerMSecond = 0.0f;
	m_cycles = 0;
	m_alreadyCycles = 0;
	m_rotateTemp = 0.0f;
}
CDexGuiActionRotate::~CDexGuiActionRotate()
{

}

void CDexGuiActionRotate::ShutDown()
{

}
void CDexGuiActionRotate::UpdateThis(int delta)
{
	DEX_ENSURE(m_owner);
	DEX_ENSURE(m_valid);
	float rotate = m_owner->GetRotate();
	
	rotate += m_rotatePerMSecond*delta;

	m_owner->SetRotate(rotate);

	m_rotateTemp += abs(m_rotatePerMSecond*delta);
	if(m_rotateTemp >= 2*PI)
	{
		m_rotateTemp = 0.0f;
		m_alreadyCycles++;
	}
	if(m_cycles != GUI_ACTION_FOREVER_CYCLE)
	{//����������ת
		if(m_alreadyCycles >= m_cycles)
		{
			m_complete = true;
		}
	}
}
void CDexGuiActionRotate::BeforAction()
{
	CDexGuiAction::BeforAction();
}
void CDexGuiActionRotate::AfterAction()
{
	m_valid = true;
	CDexGuiAction::AfterAction();
}

void CDexGuiActionRotate::SetCyclesPerSecond(float cyclesPerSecond)
{
	cyclesPerSecond = -cyclesPerSecond;
	m_rotatePerMSecond = 2 * PI * cyclesPerSecond/1000.0f; //���Ϊ�� ��ôÿms��������ת
	m_cyclesPerSecond = abs(cyclesPerSecond); //Ϊ�˼������� ����תΪ����
}

void CDexGuiActionRotate::SetCycles(int cycles)
{
	m_cycles = cycles;
}