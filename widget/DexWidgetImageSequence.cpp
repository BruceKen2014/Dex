

#include "../Source/CTexture.h"
#include "../DexBase/CDexDraw2D.h"
#include "../DexBase/DexLog.h"
#include "DexWidgetImageSequence.h"
#include "CDexUiSrcMgr.h"
#include "DexWidgetContainer.h"


//m_imagesequence = (CDexWidgetImageSequence*) getWidgetFactory()->findWidget("background.lovelygirl");
//m_imagesequence->AddImage("女孩0.png");
//m_imagesequence->AddImage("女孩1.png");
//m_imagesequence->AddImage("女孩2.png");
//m_imagesequence->AddImage("女孩3.png");
//m_imagesequence->AddImage("女孩4.png");
//m_imagesequence->AddImage("女孩5.png");
//m_imagesequence->SetInterval(100);
//m_imagesequence->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);
//m_imagesequence->Resize(DexSize(71, 103));
//g_wgtImagechild2->ModifyFlag(Add_Flag, DexGUI::mouse_r_drag);

CDexWidgetImageSequence::CDexWidgetImageSequence()
{
	m_type = widget_image_sequence;


	m_currIndex = 0;
	m_interval = 0;
	m_currPast = 0;
	m_stop = false;
}

CDexWidgetImageSequence::~CDexWidgetImageSequence()
{
	
}
void CDexWidgetImageSequence::ShutDown()
{	
	for( TImageVec::iterator it= (m_ImageVec).begin(); it != (m_ImageVec).end();)
	{
		_SafeDelete(*it);
		if(*it == NULL)
		{
			m_ImageVec.erase(it);
			it = m_ImageVec.begin();
		}
	}
	CDexWidget::ShutDown();
}
bool CDexWidgetImageSequence::Update(int delta)
{
	DEX_ENSURE_B(CDexWidget::Update(delta));
	DEX_ENSURE_B(!m_stop);
	m_currPast += delta;
	if(m_currPast >= m_interval)
	{
		m_currPast = 0;
		m_currIndex++;
		if(m_currIndex >= m_ImageVec.size())
		{
			m_currIndex = 0;
		}
	}
	return true;
}
void CDexWidgetImageSequence::RenderThis()
{
	DEX_ENSURE(m_ImageVec.size() != 0);
	DEX_ENSURE(m_ImageVec[m_currIndex]->m_tex != NULL);

	D3DXVECTOR2 v1, v2, v3, v4;
	// v1 v2
	// v4 v3
	v1.x = (float)m_ImageVec[m_currIndex]->m_srcArea.left/(float)m_ImageVec[m_currIndex]->m_tex->GetWidth();
	v1.y = (float)m_ImageVec[m_currIndex]->m_srcArea.top /(float)m_ImageVec[m_currIndex]->m_tex->GetHeight();
	v3.x = (float)m_ImageVec[m_currIndex]->m_srcArea.right/(float)m_ImageVec[m_currIndex]->m_tex->GetWidth();
	v3.y = (float)m_ImageVec[m_currIndex]->m_srcArea.bottom /(float)m_ImageVec[m_currIndex]->m_tex->GetHeight();
	v2.x = v3.x;
	v2.y = v1.y;
	v4.x = v1.x;
	v4.y = v3.y;
	if(m_ImageVec[m_currIndex]->m_Mirro)
	{
		float coor = v2.x;
		v2.x = v3.x = v1.x;
		v1.x = v4.x = coor;
	}
	get2DDrawer()->ResetMatrix();
	get2DDrawer()->SetScale(m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
	get2DDrawer()->SetUV(v1, v2, v3, v4);
	get2DDrawer()->SetAlpha(m_alpha);
	get2DDrawer()->SetTrans(m_rect.left, m_rect.top);
	get2DDrawer()->Draw(m_ImageVec[m_currIndex]->m_tex->GetTexPt());
}

void CDexWidgetImageSequence::SetInterval(int interval)
{
	m_interval = interval;
}
void CDexWidgetImageSequence::Stop()
{
	m_stop = true;
}
void CDexWidgetImageSequence::Play()
{
	m_stop = false;
}

bool CDexWidgetImageSequence::getStop()
{
	return m_stop;
}
bool CDexWidgetImageSequence::AddImage(string texname, const DexRect& rect, bool mirro)
{
	stUiImage* temp = new stUiImage;
	temp->m_tex = getUiSrcMgrSingleton()->getUiTexFactory()->FindTex(texname.c_str());
	if(temp->m_tex == NULL)
	{
		DexLog::getSingleton()->BeginLog();
		DexLog::getSingleton()->Log(log_allert, "CDexWidgetImageSequence::AddImage调用失败：未找到图片%s", texname.c_str());
		DexLog::getSingleton()->EndLog();
		return false;
	}
	if(rect.right == 0)
	{
		temp->m_srcArea = DexRect(0,0,temp->m_tex->GetWidth(), temp->m_tex->GetHeight());
	}
	else
		temp->m_srcArea = rect;
	temp->m_Mirro = mirro;
	m_ImageVec.push_back(temp);
	return true;
}
