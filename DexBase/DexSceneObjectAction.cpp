


#include "DexSceneObjectAction.h"
#include "DexSceneObject.h"
#include "DexLog.h"
#include "../Source/CCommandScript.h"
#include "../widget/DexGuiStruct.h"
#include "../state/DexGameEngine.h"


//================DexSceneObjectAction========================================================
DexSceneObjectAction::DexSceneObjectAction()
{
	m_bCycle = true;
	m_bComplete = false;
	m_pSceneObject = NULL;
}

DexSceneObjectAction::~DexSceneObjectAction()
{
}


void DexSceneObjectAction::Reset()
{
	m_bCycle = false;
	m_bComplete = false;
	m_pSceneObject = NULL;
	m_bRender = true;
}

bool DexSceneObjectAction::Update(int delta)
{
	DEX_ENSURE_B(CDexObject::Update(delta));
	DEX_ENSURE_B(m_pSceneObject);
	return true;
}

bool DexSceneObjectAction::Render()
{
//#ifdef _DEBUG
	DEX_ENSURE_B(m_bRender);
//#endif
	return true;
}

bool DexSceneObjectAction::getComplete()const
{
	return m_bComplete;
}

void DexSceneObjectAction::setCycle(bool cycle)
{
	m_bCycle = cycle;
}

void DexSceneObjectAction::setRender(bool render)
{
//#ifdef _DEBUG
	m_bRender = render;
//#endif
}

void DexSceneObjectAction::bindSceneObject(CDexSceneObject* object)
{
	m_pSceneObject = object;
}

bool DexSceneObjectAction::ArchiveIn(DexMem& mem)
{
	DEX_ENSURE_B(CDexObject::ArchiveIn(mem));
	mem>>m_bCycle;
	mem>>m_bComplete;
	mem>>m_bRender;
	return true;
}
bool DexSceneObjectAction::ArchiveOut(DexMem& mem) const
{
	DEX_ENSURE_B(CDexObject::ArchiveOut(mem));
	mem<<m_bCycle;
	mem<<m_bComplete;
	mem<<m_bRender;
	return true;
	//所屬的object不在這裡archive object是action的上層 由object自身archive
}

//============================DexSceneObjectActionCatmull================================

DexSceneObjectActionCatmull::DexSceneObjectActionCatmull()
{
	m_posRate = 0.0f;
	m_speed = 1.0f;
	m_factor = 0.8f;
}
DexSceneObjectActionCatmull::~DexSceneObjectActionCatmull()
{

}

void DexSceneObjectActionCatmull::Reset()
{
	m_catmullPoints.clear();
	m_posRate = 0.0f;
	m_speed = 1.0f;
	m_factor = 0.8f;
}

bool DexSceneObjectActionCatmull::Update(int delta)
{
	DEX_ENSURE_B(DexSceneObjectAction::Update(delta));
	if(m_catmullPoints.size() >= 4)
	{//最少要有4个点才能决定一条曲线段
		float vel = 1.0f/(1000.0f*m_speed);
		m_posRate += delta*vel;
		if(m_posRate >= 1.0f)
		{
			if(m_bCycle)
			{
				m_posRate = 0.0f;
			}
			else
			{
				m_bComplete = true;
				return false;
			}
		}
		float f0 = 1.0f/(m_catmullPoints.size()-3);  //每一小段对应的增加量 n = 7  则分四段 每段 f0 = 0.25f
		int pnt1_index = m_posRate/f0;    //curr_delta所在线段的前一点 curr_delta = 0.5f时  pnt1_index = 0.5f/0.25f = 2 即正好落在第四个点上
		float fm = (m_posRate - f0*pnt1_index)/f0;  //当前所在的线段内的偏移量 curr_delta = 0.6f fm =(0.6-0.25*2)/0.25 = 0.4 即在当前区间的0.4分量上
		pnt1_index += 1; //因为第一个点不参加计算 所以向后偏移一位
		
		D3DXVECTOR3 pos = DexGUI::CDexFun::get_catmull_value(m_catmullPoints[pnt1_index-1].init_pnt, m_catmullPoints[pnt1_index].init_pnt,m_catmullPoints[pnt1_index+1].init_pnt,m_catmullPoints[pnt1_index+2].init_pnt,fm, m_factor);
		
		if(m_pSceneObject->getType() == CCamera::getClassType())
		{//对摄像机进行特别处理
			DexGameEngine::getEngine()->SetCameraPos(DexVector3(pos.x, pos.y, pos.z));
		}
		else
		{
			m_pSceneObject->setPosition(pos);  
		}
	}
	return true;
}

bool DexSceneObjectActionCatmull::Render()
{
	DEX_ENSURE_B(DexSceneObjectAction::Render());
	DEX_ENSURE_B(DexGameEngine::getEngine()->getRendeCollideMesh());
//#ifdef _DEBUG
	if(m_catmullPoints.size() >= 4)
	{
		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);
		if(m_pSceneObject->getFatherNode() != NULL)
			D3DXMatrixMultiply(&matrix, &matrix, m_pSceneObject->getFatherNode()->getFinalMatrix());
		//else
		//	matrix = *m_pSceneObject->getCurrMatrix();
		//DexLog::getSingleton()->BeginLog();
		for(size_t i = 0 ; i < m_catmullPoints.size(); i++)
		{//获得实时的控制点位置
			//DexLog::getSingleton()->Log(log_ok, "点初始位置x:%d y:%d z:%d\n",(int) m_catmullPoints[i].init_pnt.x, (int)m_catmullPoints[i].init_pnt.y, (int)m_catmullPoints[i].init_pnt.z);
			D3DXVec3TransformCoord(&m_catmullPoints[i].real_pnt, &m_catmullPoints[i].init_pnt, &matrix);
			//DexLog::getSingleton()->Log(log_allert, "转换后x:%d y:%d z:%d\n",(int) m_catmullPoints[i].real_pnt.x, (int)m_catmullPoints[i].real_pnt.y, (int)m_catmullPoints[i].real_pnt.z);
		}
		//DexLog::getSingleton()->EndLog();

		for(size_t i = 1; i < m_catmullPoints.size()-2; i++)
		{
			int cut = 10; //两点间画多少线段描述曲线
			float add = 1.0f/10;
			DexColor color(0.0f, 0.0f, 1.0f, 1.0f);
			for(float f = 0.0f; f < 1.0f;)
			{
				D3DXVECTOR3 p1;
				D3DXVECTOR3 p2; 
				p1 = DexGUI::CDexFun::get_catmull_value(m_catmullPoints[i-1].real_pnt, m_catmullPoints[i].real_pnt,m_catmullPoints[i+1].real_pnt,m_catmullPoints[i+2].real_pnt,f, m_factor);
				p2 = DexGUI::CDexFun::get_catmull_value(m_catmullPoints[i-1].real_pnt, m_catmullPoints[i].real_pnt,m_catmullPoints[i+1].real_pnt,m_catmullPoints[i+2].real_pnt,f+add, m_factor);
					
				//D3DXMATRIX matrix;// = *m_pSceneObject->getFinalMatrix();
				//if(m_pSceneObject->getFatherNode() != NULL)
				//{
				//	//跳过因自身被曲线改变的currmatrix 
				//	D3DXMatrixMultiply(&matrix, m_pSceneObject->getLocalMatrix(), m_pSceneObject->getFatherNode()->getFinalMatrix());
				//	D3DXVec3TransformCoord(&p1, &p1, &matrix);
				//	D3DXVec3TransformCoord(&p2, &p2, &matrix);
				//}
				//else
				//{//如果没有父亲节点 必然是根节点 又是按照catmull曲线的 就不作任何操作 因为路径点在设置localmatrix时已经被修改了
				//	matrix = *m_pSceneObject->getLocalMatrix();
				//	D3DXVec3TransformCoord(&p1, &p1, &matrix);
				//	D3DXVec3TransformCoord(&p2, &p2, &matrix);
				//}

				//float f0 = 1.0f/(m_catmullPoints.size()-3);  //每一小段对应的增加量 n = 7  则分四段 每段 f0 = 0.25f
				//int pnt1_index = m_posRate/f0;    //curr_delta所在线段的前一点 curr_delta = 0.5f时  pnt1_index = 0.5f/0.25f = 2 即正好落在第四个点上
				//float fm = (m_posRate - f0*pnt1_index)/f0;  //当前所在的线段内的偏移量 curr_delta = 0.6f fm =(0.6-0.25*2)/0.25 = 0.4 即在当前区间的0.4分量上
				//pnt1_index += 1; //因为第一个点不参加计算 所以向后偏移一位
				//D3DXVECTOR3 pos = DexGUI::CDexFun::get_catmull_value(m_catmullPoints[pnt1_index-1], m_catmullPoints[pnt1_index],m_catmullPoints[pnt1_index+1],m_catmullPoints[pnt1_index+2],fm, m_factor);
				//
				//D3DXMATRIX trans;
				//D3DXMatrixIdentity(&trans);
				//D3DXMatrixTranslation(&trans, -pos.x, -pos.y, -pos.z);
				//matrix = matrix * trans;
					
				DexGameEngine::getEngine()->Render3DLine(p1, p2, color, color);
				color.r += add;
				f += add;
			}
		}	
	}
//#endif
	return true;
}

void DexSceneObjectActionCatmull::addCatmullPoint(const D3DXVECTOR3& pos)
{
	stCatmullPnt pnt;
	pnt.init_pnt = pos;
	pnt.real_pnt = pos;
	m_catmullPoints.push_back(pnt);
}

void DexSceneObjectActionCatmull::setCatmullFactor(float factor)
{
	m_factor = factor;
}

void DexSceneObjectActionCatmull::setCatmullSpeed(float time)
{
	m_speed = time;
}

bool DexSceneObjectActionCatmull::ArchiveIn(DexMem& mem)
{
	DEX_ENSURE_B(DexSceneObjectAction::ArchiveIn(mem));
	size_t pnt_size;
	mem>>pnt_size;
	stCatmullPnt pnt;
	for(size_t i = 0 ; i < pnt_size; i++)
	{
		mem>>pnt.init_pnt;
		pnt.real_pnt = pnt.init_pnt;
		m_catmullPoints.push_back(pnt);
	}
	mem>>m_posRate;
	mem>>m_speed;
	mem>>m_factor;
	return true;
}
bool DexSceneObjectActionCatmull::ArchiveOut(DexMem& mem) const
{
	DEX_ENSURE_B(DexSceneObjectAction::ArchiveOut(mem));
	mem<<m_catmullPoints.size();
	for(size_t i = 0 ;i < m_catmullPoints.size(); i++)
	{
		mem<<m_catmullPoints[i].init_pnt; //這裡只需archive init_pnt real由程序實時計算
	}
	mem<<m_posRate;
	mem<<m_speed;
	mem<<m_factor;
	return true;
}

bool DexSceneObjectActionCatmull::LoadInitFile(const char* filename)
{
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		pScript->CloseScript();
		return false;
	}
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar();
		int posX = pScript->GetIntParam();
		int posY = pScript->GetIntParam();
		int posZ = pScript->GetIntParam();
		addCatmullPoint(D3DXVECTOR3(posX, posY, posZ));
	}
	pScript->CloseScript();
	return true;
}

//============================DexSceneObjectActionRotate================================

DexSceneObjectActionRotate::DexSceneObjectActionRotate()
{
	m_rotateFlag = ROTATEY;
	m_fCurrRadian = 0.0f;
	m_fCyclePerSecond = 1.0f;
}

DexSceneObjectActionRotate::~DexSceneObjectActionRotate()
{
}

void DexSceneObjectActionRotate::Reset()
{
	m_rotateFlag = ROTATEY;
	m_fCyclePerSecond = 1.0f;
}

bool DexSceneObjectActionRotate::Update(int delta)
{
	DEX_ENSURE_B(DexSceneObjectAction::Update(delta));
	m_fCurrRadian += m_fCyclePerSecond * PI	* delta/1000.0f;
	if(m_rotateFlag == ROTATEX)
		m_pSceneObject->rotateRadian(CDexSceneObject::AXIS_X, m_fCurrRadian);
	else if(m_rotateFlag == ROTATEY)
		m_pSceneObject->rotateRadian(CDexSceneObject::AXIS_Y, m_fCurrRadian);
	else if(m_rotateFlag == ROTATEZ)
		m_pSceneObject->rotateRadian(CDexSceneObject::AXIS_Z, m_fCurrRadian);
	return true;
}

bool DexSceneObjectActionRotate::Render()
{
	return true;
}

bool DexSceneObjectActionRotate::ArchiveIn(DexMem& mem)
{
	return true;
}

bool DexSceneObjectActionRotate::ArchiveOut(DexMem& mem) const
{
	return true;
}

bool DexSceneObjectActionRotate::LoadInitFile(const char* filename)
{
	CommandScript* pScript = getComandScript();
	if(!pScript->OpenScript(filename))
	{
		pScript->CloseScript();
		return false;
	}
	char str[16];
	pScript->GetStringParam(str);
	m_fCyclePerSecond = pScript->GetFloatParam();
	if(strcmp(str, "X")==0)
	{
		m_rotateFlag = ROTATEX;
	}
	else if(strcmp(str, "Y")==0)
	{
		m_rotateFlag = ROTATEY;
	}
	else if(strcmp(str, "Z")==0)
	{
		m_rotateFlag = ROTATEZ;
	}
	pScript->CloseScript();
	return true;
}