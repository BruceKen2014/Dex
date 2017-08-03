




#include "CGUI.h"
#include "../DexBase/DexLog.h"

//=============================================================================
//CGui：GUI类
CGui::CGui()
{
	m_iXPos = 0;
	m_iYPos = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	m_bCommunicate = false;
	m_keyword = GK_BaseGui;
	m_iId = 0;
	m_render = true;
	m_active = true;

	//m_MouseMove = NULL;
	//m_MouseLUp = NULL;
	//m_MouseLDown = NULL;
	//m_MouseRUp = NULL;
	//m_MouseRDown = NULL;
	//m_MouseLDrag = NULL;
	//m_MouseRDrag = NULL;

	m_UiFlag = mouse_none;	 

	CInputSystem::GetInputSingleton().RegisterMouseListener(this);
}
CGui::~CGui()
{
	//_SafeDelete(m_MouseMove);
	//_SafeDelete(m_MouseLUp);
	//_SafeDelete(m_MouseLDown);
	//_SafeDelete(m_MouseRUp);
	//_SafeDelete(m_MouseRDown);
}
bool CGui::OnEvent(stEvent event)
{
	switch(event.id)
	{
	case EVENT_ID_MOUSE_L_DOWN:
		{
			if(m_UiFlag & mouse_l_down)
			{
				//if(CheckPoint(m_MouseLDown))
					if(EventValid(event))
						m_MouseLDown.Handle(this, event);
			}
			break;
		}
	case EVENT_ID_MOUSE_L_UP:
		{
			if(m_UiFlag & mouse_l_up)
			{
				//if(CheckPoint(m_MouseLUp))
					if(EventValid(event))
						m_MouseLUp.Handle(this, event);
		    }
			break;
		}
	case EVENT_ID_MOUSE_R_UP:
		{
			if(m_UiFlag & mouse_r_up)
			{
				//if(CheckPoint(m_MouseRUp))
					if(EventValid(event))
						m_MouseRUp.Handle(this, event);
			}
			break;
		}
	case EVENT_ID_MOUSE_R_DOWN:
		{
			if(m_UiFlag & mouse_r_down)
			{
				//if(CheckPoint(m_MouseRDown))
					if(EventValid(event))
						m_MouseRDown.Handle(this, event);
			}
			break;
		}
	case EVENT_ID_MOUSE_L_DRAG:
		{
			if(m_UiFlag & mouse_l_drag)
			{
				//if(CheckPoint(m_MouseLDrag))
					if(EventValid(event))
					{
						m_MouseRDown.Handle(this, event);
					}
			}
			break;
		}
	default:
		break;
	}
	return false;
}
void CGui::ModifyFlag( int i_o, int flag )
{
	if(i_o == Add_Flag)
	{
		m_UiFlag |= flag;
	}
	else if(i_o == Minus_Flag)
	{
		if(m_UiFlag & flag)
			m_UiFlag = m_UiFlag^flag;
	}
}
GuiReturnValue CGui::OnMouse(TSHORT x, TSHORT y, MouseFlag f)
{
	//if(f == MF_Flag)
	//	return GRV_Flag;
	////_Message("x:%d y:%d m_iXPos + m_iWidth:%d m_iYPos + m_iHeight:%d",x, y, m_iXPos + m_iWidth, m_iYPos + m_iHeight);
	//if( (x > m_iXPos) && (x < (m_iXPos + m_iWidth)) &&		//鼠标坐标落在之内
	//	(y > m_iYPos) && (y < (m_iYPos + m_iHeight)) )
	//{
	//	if( f == MF_MouseMove)	  //鼠标移动
	//	{
	//		return GRV_ButtonOn;   //鼠标第一次移动到按钮之内
	//	}
	//	else if( f == MF_MouseLDown )	  //鼠标左键按下
	//	{
	//		return GRV_ButtonDown;   //按钮被按下
	//	}
	//	else if( f == MF_MouseLUp)	    //鼠标左键弹起
	//	{
	//		return GRV_ButtonUp;   //按钮弹起
	//	}

	//}
	return GRV_Flag;
}

//=============================================================================
//CTextGui:字体GUI类
CTextGui::CTextGui():CGui()
{
	m_keyword = GK_TextGui;
	m_pText = NULL;
    m_pFont = NULL;
	m_crTextColor =  CGUI_TEXTCOLOR;
}

CTextGui::CTextGui(char * pText, CMyFont * pFont):CGui()
{
	m_keyword = GK_TextGui;
	m_pText = NULL;
	m_pText = (char *)malloc(sizeof(char) *CGUI_TEXTBYTE);
	strcpy(m_pText, pText);

	m_pFont = pFont;
	m_crTextColor = CGUI_TEXTCOLOR;
	DexLog::getSingleton()->Log(log_ok, "新建CTextGui：内容%s", pText);
}

CTextGui::~CTextGui()
{
	ShutDown();
}

void CTextGui::SetText(char * text)
{
	free(m_pText);
	m_pText = NULL;
	m_pText = new char[CGUI_TEXTBYTE];
	strcpy(m_pText, text);
}
void CTextGui::ShutDown()
{
	if(m_pFont != NULL)       //这个指针是从外界传进来的，故只将本成员置为NULL而不在此释放空间，释放由源指针自己进行
		m_pFont = NULL;
	if(m_pText != NULL)		  //本内存是由对象自己申请，故要由自身释放
	{
		free(m_pText);
		m_pText = NULL;
	}
}

void CTextGui::Render()
{
	if(!m_render)
		return;
	//_Message("%s 字体编号%d", m_pText, m_pFont->GetId());
	if(m_pText && m_pFont)     //textGui内容不为空，而且字体不为空
	{
		m_pFont->UseColor(m_crTextColor);
		m_pFont->SetPos(m_iXPos, m_iYPos);
		m_pFont->Print(m_pText);
		m_pFont->ResetColor();
	}
}
//=============================================================================
//CPictureGui:图片GUI类

CPictureGui::CPictureGui():CGui()
{
	m_keyword = GK_PicGui; 

	m_iWidth  = 0;
	m_iHeight = 0;
	m_scale        = D3DXVECTOR2(1.0f, 1.0f);			   //默认不缩放
	m_center       = D3DXVECTOR2(0, 0);
	m_stTexture.m_iHeight = m_stTexture.m_iWidth = 0;
	m_stTexture.m_pTexture = NULL;
	m_pSprite = NULL;
}

CPictureGui::CPictureGui(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, const DexColor& TransColor ):CGui()
{
	m_keyword = GK_PicGui; 

	m_iWidth  = 0;
	m_iHeight = 0;
	m_scale        = D3DXVECTOR2(1.0f, 1.0f);			   //默认不缩放
	m_center       = D3DXVECTOR2(0, 0);
	m_stTexture.m_iHeight = m_stTexture.m_iWidth = 0;
	m_stTexture.m_pTexture = NULL;
	m_pSprite = NULL;

    D3DXIMAGE_INFO info;
    if(D3DXCreateTextureFromFileEx(pD3DDevice, filename, D3DX_DEFAULT_NONPOW2/*使用此参数才不会放大，使用0或者D3DX_DEFAULT图片会被放大一倍*/
	   , D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,0, D3DFMT_UNKNOWN, 
	   D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE,
    getD3DColor(TransColor), &info, NULL, &m_stTexture.m_pTexture) != D3D_OK)
    {
	   MessageBox(NULL, "加载图片GUI失败！", "提示", MB_OK);
	   return ;
    }
    if(FAILED(D3DXCreateSprite(pD3DDevice, &m_pSprite)))
    {
	    MessageBox(NULL, "创建sprite失败！", "提示", MB_OK);
        return ;
    }
    m_iWidth  = m_stTexture.m_iWidth = info.Width;
    m_iHeight = m_stTexture.m_iHeight =info.Height;

    CaculateMatrix();
}

CPictureGui::~CPictureGui()
{
	ShutDown();
}

void CPictureGui::ShutDown()
{
	if(m_stTexture.m_pTexture)
	{
		m_stTexture.m_pTexture->Release();
		m_stTexture.m_pTexture = NULL;
	}
	if(m_pSprite)
	{	
		m_pSprite->Release();
		m_pSprite = NULL;
	}
}
void CPictureGui::CaculateMatrix()
{
	m_center = D3DXVECTOR2(m_iXPos + m_iWidth/2, m_iYPos + m_iHeight/2);
	D3DXMatrixTransformation2D(&m_mat, NULL, 0, &m_scale, &m_center, 0.0f/*不旋转*/, NULL);	
	if(m_pSprite != NULL)
		m_pSprite->SetTransform(&m_mat);
}
void CPictureGui::SetXposition(TSHORT xPosition)
{
	m_iXPos = xPosition;
	CaculateMatrix();
}
void CPictureGui::SetYposition(TSHORT yPosition)
{
	m_iYPos = yPosition;
	CaculateMatrix();
}
void CPictureGui::SetPosition(TSHORT x, TSHORT y)
{
	m_iXPos = x;
	m_iYPos = y;
	CaculateMatrix();
}

void CPictureGui::Render()
{
	if(!m_render)
		return;
	RECT Rect = {0, 0, m_iWidth, m_iHeight};
	if(m_pSprite)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		  m_pSprite->Draw(m_stTexture.m_pTexture, &Rect, NULL, &D3DXVECTOR3(m_iXPos,m_iYPos, 0), 0xFFFFFFFF);
		m_pSprite->End();
	}
}
//=============================================================================
//CPicMvGui:具有移动状态的图片GUI
CPicMvGui::CPicMvGui(LPDIRECT3DDEVICE9 pD3DDevice):CGui()
{
	m_device = pD3DDevice;
	m_tex = NULL;
	m_flag = CPICMVGUI_ONCE;
	m_tex = new CTextureMove(pD3DDevice);
}
CPicMvGui::~CPicMvGui()
{
	ShutDown();
}

void CPicMvGui::ShutDown()
{
	_SafeDelete(m_tex);
}

void CPicMvGui::Render()
{
	if(m_tex != NULL)
	{
		m_tex->Update();
		m_tex->Render();
		if(m_tex->GetComplete())
		{
			switch(m_flag)
			{
			case CPICMVGUI_FLAG:
				break;
			case CPICMVGUI_ONCE:
				break;
			case CPICMVGUI_CYCLE:
				m_tex->Reset();
				break;
			}
		}
	}
}

bool CPicMvGui::LoadTexture(char* filename)
{
	if(m_device == NULL || m_tex == NULL)
		return false;
	return m_tex->Load(m_device, filename);
}

void CPicMvGui::SetStartPos(TSHORT x, TSHORT y)
{
	if(m_tex != NULL)
		m_tex->SetStartPos(x, y);
}

void CPicMvGui::SetEndPos(TSHORT x, TSHORT y)
{
	if(m_tex != NULL)
		m_tex->SetEndPos(x, y);
}

void CPicMvGui::SetSpeed(float speed)
{
	if(m_tex != NULL)
		m_tex->SetSpeed(speed);
}

void CPicMvGui::SetFlag(int flag)
{
	m_flag = flag;
}




//=============================================================================
//CButtonGui:按钮GUI类(派生自图片GUI类)

CButtonGui::CButtonGui():CPictureGui()
{
	m_keyword = GK_ButtonGui; 
	m_bCommunicate = true; 

	m_bsState = BS_NORMAL;
	ModifyFlag(Add_Flag, mouse_l_down | mouse_l_up | mouse_r_down|mouse_r_up|mouse_move |mouse_l_drag | mouse_r_drag);
}

CButtonGui::CButtonGui(LPDIRECT3DDEVICE9 pD3DDevice, char *filename, DexColor TransColor):CPictureGui()
{

	m_keyword = GK_ButtonGui; 
	m_bCommunicate = true;

	 D3DXIMAGE_INFO info;
    if(D3DXCreateTextureFromFileEx(pD3DDevice, filename, D3DX_DEFAULT_NONPOW2/*使用此参数才不会放大，使用0或者D3DX_DEFAULT图片会被放大一倍*/
	   , D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT,0, D3DFMT_UNKNOWN, 
	   D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE ,D3DX_FILTER_TRIANGLE,
     getD3DColor(TransColor), &info, NULL, &m_stTexture.m_pTexture) != D3D_OK)
    {
	   MessageBox(NULL, "加载图片GUI失败！", "提示", MB_OK);
	   return ;
    }
    if(FAILED(D3DXCreateSprite(pD3DDevice, &m_pSprite)))
    {
	    MessageBox(NULL, "创建sprite失败！", "提示", MB_OK);
        return ;
    }
    m_iWidth  = m_stTexture.m_iWidth = info.Width;
    m_stTexture.m_iHeight =info.Height;
	m_iHeight = info.Height/3;
	//m_iHeight = info.Height/4;暂时不加入无效按钮

    CaculateMatrix();
	m_bsState = BS_NORMAL;
	ModifyFlag(Add_Flag, mouse_l_down | mouse_l_up | mouse_r_down|mouse_r_up|mouse_move|mouse_l_drag| mouse_r_drag);
}

CButtonGui::~CButtonGui()
{
	ShutDown();
}

bool CButtonGui::OnEvent(stEvent event)
{
	switch(event.id)
	{
	case EVENT_ID_MOUSE_L_DOWN:
		{
			if(m_UiFlag & mouse_l_down)
			{
				if(EventValid(event))
				{
					m_bsState = BS_DOWN;	
				    //if(CheckPoint(m_MouseLDown))
					    m_MouseLDown.Handle(this, event);
				}
			}
			break;
		}
	case EVENT_ID_MOUSE_L_UP:
		{		
			if(m_UiFlag & mouse_l_up)
			{
				if(EventValid(event))
				{
					m_bsState = BS_ON;		
					//if(CheckPoint(m_MouseLUp))
						m_MouseLUp.Handle(this, event);
				}   
			}
			break;
		}
	case EVENT_ID_MOUSE_R_UP:
		{
			if(m_UiFlag & mouse_r_up)
			{   	
				if(EventValid(event))
				{
					m_bsState = BS_ON;	 	
					//if(CheckPoint(m_MouseRUp))
					{
						m_MouseRUp.Handle(this, event);
					}
				}

			}
			break;
		}
	case EVENT_ID_MOUSE_R_DOWN:
		{
			if(m_UiFlag & mouse_l_up)
			{
				if(EventValid(event))
				{
					m_bsState = BS_DOWN;	
					//if(CheckPoint(m_MouseRDown))
					{
						m_MouseRDown.Handle(this, event);
					}
				}

			}
			break;
		}
	case EVENT_ID_MOUSE_MOVE:
		{
			if(m_UiFlag & mouse_move)
			{
				if(EventValid(event))
				{
					m_bsState = BS_ON;	   
					//if(CheckPoint(m_MouseMove))
						m_MouseMove.Handle(this, event);
				}
				else 
				{
					m_bsState = BS_NORMAL;
				}
			}
				break;
		}
	case EVENT_ID_MOUSE_L_DRAG:
		{
			if(m_UiFlag & mouse_l_drag)
			{
				if(EventValid(event))
				{
					event.pop_args(stArgs());
					event.pop_args(stArgs()); //将两个位置信息弹出
					stArgs offset_y;  event.pop_args(offset_y);
					stArgs offset_x;  event.pop_args(offset_x);
					SetPosition(m_iXPos + offset_x.i_member, m_iYPos += offset_y.i_member);
					//if(CheckPoint(m_MouseLDrag))
						m_MouseLDrag.Handle(this, event);
				}
			}
			break;
		}
	case EVENT_ID_MOUSE_R_DRAG:
		{
			if(m_UiFlag & mouse_r_drag)
			{
				if(EventValid(event))
				{
					event.pop_args(stArgs());
					event.pop_args(stArgs()); //将两个位置信息弹出
					stArgs offset_y;  event.pop_args(offset_y);
					stArgs offset_x;  event.pop_args(offset_x);
                    SetPosition(m_iXPos + offset_x.i_member, m_iYPos += offset_y.i_member); 
					//if(CheckPoint(m_MouseRDown))
						m_MouseRDown.Handle(this, event);
				}
			}
			break;
		}
	default:
		break;
	}
	return false;
}
bool CButtonGui::EventValid(stEvent event)
{
	switch(event.id)
	{
	case EVENT_ID_MOUSE_L_DOWN:
	case EVENT_ID_MOUSE_L_UP:
    case EVENT_ID_MOUSE_L_DRAG:
	case EVENT_ID_MOUSE_R_DRAG:
		{
			int posY = 0;
			stArgs arg;
			event.pop_args(arg);
			posY = arg.i_member;
			int posX = 0;
			event.pop_args(arg);
			posX = arg.i_member;
			if( (posX > m_iXPos) && (posX < (m_iXPos + m_iWidth)) &&		//鼠标坐标落在按钮之内
				(posY > m_iYPos) && (posY < (m_iYPos + m_iHeight)) )
			{//事件有效
				return true;
			}
		}
	case EVENT_ID_MOUSE_R_UP:
	case EVENT_ID_MOUSE_R_DOWN:
	case EVENT_ID_MOUSE_MOVE:
		{
			int posY = 0;
			stArgs arg;
			event.pop_args(arg);
			posY = arg.i_member;
			int posX = 0;
			event.pop_args(arg);
			posX = arg.i_member;
			if( (posX > m_iXPos) && (posX < (m_iXPos + m_iWidth)) &&		//鼠标坐标落在按钮之内
				(posY > m_iYPos) && (posY < (m_iYPos + m_iHeight)) )
			{//事件有效
				return true;
			}
		}
	default:
		break;
	}
	//事件无效
	return false;
}

void CButtonGui::ShutDown()
{
	CPictureGui::ShutDown();
}

GuiReturnValue CButtonGui::OnMouse(TSHORT x, TSHORT y, MouseFlag f)
{ 
	if(f == MF_Flag || m_bsState == BS_USELESS)
		return GRV_Flag;
	//_Message("x:%d y:%d m_iXPos + m_iWidth:%d m_iYPos + m_iHeight:%d",x, y, m_iXPos + m_iWidth, m_iYPos + m_iHeight);
	if( (x > m_iXPos) && (x < (m_iXPos + m_iWidth)) &&		//鼠标坐标落在按钮之内
		(y > m_iYPos) && (y < (m_iYPos + m_iHeight)) )
	{
		if( f == MF_MouseMove)	  //鼠标移动
		{
			if( m_bsState != BS_DOWN && m_bsState != BS_ON && m_bsState != BS_USELESS)
			{
				m_bsState = BS_ON;
				return GRV_ButtonOn;   //鼠标第一次移动到按钮之内
			}
			return GRV_Flag;
		}
		else if( f == MF_MouseLDown )	  //鼠标左键按下
		{
			if(m_bsState == BS_USELESS)
				return GRV_ButtonDownUseless;
			else
			{
				if(m_bsState == BS_DOWN)   //若之前按钮已经被按下，则不做任何改动
					return GRV_Flag;
				else
				{
					m_bsState = BS_DOWN;
					return GRV_ButtonDown;   //按钮被按下
				}
			}
		}
		else if( f == MF_MouseLUp)	    //鼠标左键弹起
		{
			if(m_bsState == BS_USELESS)
				return GRV_Flag;
			else
			{
				m_bsState = BS_ON;
				return GRV_ButtonUp;   //按钮弹起
			}
		}
		
	}
    m_bsState = BS_NORMAL;
	return GRV_Flag;
}
void CButtonGui::HaveInvalide()
{
	m_iHeight = m_iHeight * 0.75;	//默认不含无效按钮，如果含有则调整高度
}
void CButtonGui::Render()
{
	if(!m_render)
		return;
	RECT Rect; 
	Rect.left  = 0;
	Rect.right = m_iWidth;
	if(m_bsState == BS_NORMAL)
	{
		Rect.top    = 0;
		Rect.bottom = m_iHeight;
	}
	else if(m_bsState == BS_ON)
	{
		Rect.top    = m_iHeight;
		Rect.bottom = m_iHeight *2;
	}
	else if(m_bsState == BS_DOWN)
	{
		Rect.top    = m_iHeight *2;
		Rect.bottom = m_iHeight *3;
	}
	else 
	{
		Rect.top    = m_iHeight * 3;
		Rect.bottom = m_iHeight * 4;
	}
	if(m_pSprite)
	{
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
		  m_pSprite->Draw(m_stTexture.m_pTexture, &Rect, NULL, &D3DXVECTOR3(m_iXPos,m_iYPos, 0), 0xFFFFFFFF);
		m_pSprite->End();
	}
}

//=============================================================================
//CButtonText
CButtonText::CButtonText():CTextGui()
{
	m_keyword = GK_ButtonText; 
	m_bCommunicate = true; 
}

CButtonText::CButtonText(char *pText, CMyFont *pFont):CTextGui(pText, pFont)
{
	m_keyword = GK_ButtonText;
	m_bCommunicate = true; 
}

CButtonText::~CButtonText()
{
	ShutDown();
}

void CButtonText::Render()
{
	if(!m_render)
		return;
	//_Message("%s 字体编号%d", m_pText, m_pFont->GetId());
	if(m_pText == NULL || m_pFont == NULL)     //textGui内容不为空，而且字体不为空
	{
		return;
	} 
	m_pFont->SetPos(m_iXPos, m_iYPos);
	if(m_bsState == BS_NORMAL)
	{
		m_pFont->Print(m_pText);
	}
	else if(m_bsState == BS_ON)
	{
		m_pFont->UseColor(DexColor(0.5f, 0.5f, 0.5f));
		m_pFont->Print(m_pText);
		m_pFont->ResetColor();
	}
	else if(m_bsState == BS_DOWN)
	{
		m_pFont->UseColor(DexColor(0.0f, 0.5f, 0.0f));
		m_pFont->Print(m_pText);
		m_pFont->ResetColor();
	}
	else 
	{
		m_pFont->SetPos(m_iXPos, m_iYPos);
	}
	
}

void CButtonText::ShutDown()
{
	CTextGui::ShutDown();
}

GuiReturnValue CButtonText::OnMouse(TSHORT x, TSHORT y, MouseFlag f)
{
	if(f == MF_Flag)
		return GRV_Flag;
	//_Message("x:%d y:%d m_iXPos + m_iWidth:%d m_iYPos + m_iHeight:%d",x, y, m_iXPos + m_iWidth, m_iYPos + m_iHeight);
	if( (x > m_iXPos) && (x < (m_iXPos + m_iWidth)) &&		//鼠标坐标落在按钮之内
		(y > m_iYPos) && (y < (m_iYPos + m_iHeight)) )
	{
		if( f == MF_MouseMove)	  //鼠标移动
		{
			if( m_bsState != BS_DOWN && m_bsState != BS_ON && m_bsState != BS_USELESS)
			{
				m_bsState = BS_ON;
				return GRV_ButtonOn;   //鼠标第一次移动到按钮之内
			}
			return GRV_Flag;
		}
		else if( f == MF_MouseLDown )	  //鼠标左键按下
		{
			if(m_bsState == BS_USELESS)
				return GRV_ButtonDownUseless;
			else
			{
				if(m_bsState == BS_DOWN)   //若之前按钮已经被按下，则不做任何改动
					return GRV_Flag;
				else
				{
					m_bsState = BS_DOWN;
					return GRV_ButtonDown;   //按钮被按下
				}
			}
		}
		else if( f == MF_MouseLUp)	    //鼠标左键弹起
		{
			if(m_bsState == BS_USELESS)
				return GRV_Flag;
			else
			{
				m_bsState = BS_ON;
				return GRV_ButtonUp;   //按钮弹起
			}
		}

	}
	m_bsState = BS_NORMAL;
	return GRV_Flag;
}




//=============================================================================
//CGuiSystem:系统GUI类

CGuiSystem::CGuiSystem(LPDIRECT3DDEVICE9 pD3DDevice)
{
	//m_vFont.clear();
	//m_vFont.reserve(2);
	//m_vGuis.clear();
	//m_vGuis.reserve(2);    //先开辟容量为2，但里面并没有一个实例


	
	CMyFont* deft = new CMyFont(pD3DDevice);
	m_vFont.push_back(deft);
}

CGuiSystem::~CGuiSystem()
{
	ShutDown();
}

void CGuiSystem::ShutDown()
{	
	for(size_t i = 0; i < m_vFont.size(); i++)
	{
		if(m_vFont[i] != NULL)
		{
			delete m_vFont[i];	  //释放字体对象
			m_vFont[i] = NULL;
		}
	}
	m_vFont.clear();  

	for(size_t i = 0; i < m_vGuis.size(); i++)
	{
		if(m_vGuis[i] != NULL)
		{
			delete m_vGuis[i];	  //释放GUI对象
			m_vGuis[i] = NULL;
		}
	}
	m_vGuis.clear();
	m_vGuiUnique.clear();

}

bool CGuiSystem::AddGui(CGui *Gui)
{
	m_vGuis.push_back(Gui);
	m_vGuis.reserve(m_vGuis.capacity()+1);   //增加一个数据空间
	return true;
}

void CGuiSystem::Render()
{
	for(size_t i = 0; i < m_vGuis.size(); i++)
		if(m_vGuis[i] != NULL)
		{
			//排除单独渲染的id
			std::map<int, CGui*>::iterator it = m_vGuiUnique.find(m_vGuis[i]->GetId());
			if(it == m_vGuiUnique.end())
			{//该GUI不在单独gui中	
				if(m_vGuis[i]->GetRender())
					m_vGuis[i]->Render();
			
			}

		}
}

void CGuiSystem::LoadFont(LPDIRECT3DDEVICE9 pD3DDevice, char * filename)
{
	CommandScript * pScript = new CommandScript();
	if(!pScript->OpenScript(filename))
	{
		_SafeDelete(pScript);
		return;
	}

	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar() ;
		CurrChar = pScript->GetCurrChar();
		CurrLine = pScript->GetCurrLine(); 
		if(strlen(Script[CurrLine]) == 1 || 
	      (Script[CurrLine][CurrChar] == '-' 
	           && Script[CurrLine][CurrChar+1] == '-'))  	//略过注释或空白行
           continue;
		TSHORT fontid = pScript->GetIntParam();	
		TSHORT text_size = pScript->GetIntParam();
		TSHORT alpha = pScript->GetIntParam();
		TSHORT r = pScript->GetIntParam();
		TSHORT g = pScript->GetIntParam();
		TSHORT b = pScript->GetIntParam();
		CMyFont* temp = new CMyFont(pD3DDevice);
		temp->SetId(fontid);
		temp->SetColor(DexColor(alpha, r, g ,b));
		temp->SetTextSize(text_size);
		m_vFont.push_back(temp);
	}
	_SafeDelete(pScript);
}

void CGuiSystem::LoadGui(LPDIRECT3DDEVICE9 pD3DDevice, char *filename)
{
	CommandScript * pScript = new CommandScript();
	if(!pScript->OpenScript(filename))
	{
		_SafeDelete(pScript);
		return;
	}

	char pic[MAX_PARAM_SIZE];
	char ** Script = pScript->GetScript();
	int  CurrChar = 0;
	int  CurrLine = 0;
	for( ;pScript->GetCurrLine() < pScript->GetScriptLine(); pScript->MoveCurrLine())
	{	
		pScript->SetCurrChar() ;
		CurrChar = pScript->GetCurrChar();
		CurrLine = pScript->GetCurrLine(); 
		if(strlen(Script[CurrLine]) == 1 || 
	      (Script[CurrLine][CurrChar] == '-' 
	           && Script[CurrLine][CurrChar+1] == '-'))  	//略过注释或空白行
           continue;
		if(atoi(&Script[CurrLine][CurrChar]) == GK_TextGui)     //字体GUI
		{
			pScript->GetIntParam();   //跳过第一个关键字
			CTextGui* temptext = new CTextGui(); 
			temptext->SetKeyword(GK_TextGui);
			temptext->SetId(pScript->GetIntParam());
			temptext->SetPosition(pScript->GetIntParam(), pScript->GetIntParam());
            //根据字体id设置字体指针
			int fontId = pScript->GetIntParam();
		    bool findFont = false;  //是否能在字体容器中找到所需的字体
			for(int i = 0; i < m_vFont.size(); i++)
			{
				if(m_vFont[i]->GetId() == fontId)
				{
					temptext->SetFont(m_vFont[i]);
					findFont = true;
					break;
				}
			}
			if(!findFont)    //未找到对应字体，则设置默认字体
			{
				temptext->SetFont(m_vFont[0]);
				//_Message("%d", m_vFont[0]->GetId());
			}
			char text[CGUI_TEXTBYTE];	 
			int a, r, g, b;
			a = pScript->GetIntParam(); 
			r = pScript->GetIntParam(); 
			g = pScript->GetIntParam(); 
			b = pScript->GetIntParam(); 
			pScript->MoveCurrChar();      //跳过第一个文本括号
			pScript->GetTextguiString(text);	
			temptext->SetText(text);
			temptext->SetColor(DexColor(a, r ,g ,b));
			m_vGuis.push_back(temptext);
		}
		else if	(atoi(&Script[CurrLine][CurrChar]) == GK_PicGui)	  //图片GUI
		{
			pScript->GetIntParam();   //跳过第一个关键字
			TSHORT id;
			TSHORT xPos, yPos;
			id = pScript->GetIntParam();
			xPos = pScript->GetIntParam();
			yPos = pScript->GetIntParam();
			
			//取出图片字符串
			char pic[CGUI_TEXTBYTE];
			pScript->GetStringParam(pic);	
		    CPictureGui* temppic = new CPictureGui(pD3DDevice, pic);
			temppic->SetKeyword(GK_PicGui);
			temppic->SetId(id);
			temppic->SetPosition(xPos, yPos);
			m_vGuis.push_back(temppic);

		}
		else if	(atoi(&Script[CurrLine][CurrChar]) == GK_PicMvGui)	  //移动图片GUI
		{
			pScript->GetIntParam();   //跳过第一个关键字
			TSHORT id;
			TSHORT stx, sty, edx, edy, flag;
			float speed;
			id = pScript->GetIntParam();
			stx = pScript->GetIntParam();
			sty = pScript->GetIntParam();
			edx = pScript->GetIntParam();
			edy = pScript->GetIntParam();
			speed = pScript->GetFloatParam();
			flag = pScript->GetIntParam();

			//取出图片字符串
			char pic[CGUI_TEXTBYTE];
			pScript->GetStringParam(pic);	
			CPicMvGui* temppic = new CPicMvGui(pD3DDevice);
			if(temppic->LoadTexture(pic))
			{
				temppic->SetKeyword(GK_PicMvGui);
				temppic->SetId(id);
				temppic->SetStartPos(stx, sty);
				temppic->SetEndPos(edx, edy);
				temppic->SetSpeed(speed);
				temppic->SetFlag(flag);
				m_vGuis.push_back(temppic);
			}
			else
			{
				_SafeDelete(temppic);
			}

		}
		else if	(atoi(&Script[CurrLine][CurrChar]) == GK_ButtonGui)	   //按钮GUI
		{
			pScript->GetIntParam();   //跳过第一个关键字
			TSHORT id;
			TSHORT xPos, yPos, have_useless;
			id = pScript->GetIntParam();
			xPos = pScript->GetIntParam();
			yPos = pScript->GetIntParam();
			have_useless = pScript->GetIntParam();
			
			//取出图片字符串
			char pic[CGUI_TEXTBYTE];
			pScript->GetStringParam(pic);
			int state = pScript->GetIntParam();
			CButtonGui* button = new CButtonGui(pD3DDevice, pic);
			if(button != NULL)
			{
				button->SetKeyword(GK_ButtonGui);
				button->SetId(id);
				button->SetPosition(xPos, yPos);
				if(have_useless != 0)
					button->HaveInvalide();
				m_vGuis.push_back(button);
			}
			if(have_useless)
				button->SetState(state);
		}

		else if	(atoi(&Script[CurrLine][CurrChar]) == GK_ButtonText)	   //文本按钮GUI
		{
			pScript->GetIntParam();   //跳过第一个关键字
			CButtonText* temptext = new CButtonText(); 
			temptext->SetKeyword(GK_ButtonText);
			temptext->SetId(pScript->GetIntParam());
			temptext->SetPosition(pScript->GetIntParam(), pScript->GetIntParam());
			temptext->SetSize(pScript->GetIntParam(), pScript->GetIntParam());
			//根据字体id设置字体指针
			int fontId = pScript->GetIntParam();
			bool findFont = false;  //是否能在字体容器中找到所需的字体
			for(int i = 0; i < m_vFont.size(); i++)
			{
				if(m_vFont[i]->GetId() == fontId)
				{
					temptext->SetFont(m_vFont[i]);
					findFont = true;
					break;
				}
			}
			if(!findFont)    //未找到对应字体，则设置默认字体
			{
				temptext->SetFont(m_vFont[0]);
				//_Message("%d", m_vFont[0]->GetId());
			}
			char text[CGUI_TEXTBYTE];
			pScript->MoveCurrChar();      //跳过第一个文本括号
			pScript->GetTextguiString(text);	
			temptext->SetText(text);
			m_vGuis.push_back(temptext);
		}
	}
	_SafeDelete(pScript);
}

void CGuiSystem::SetUniqueId(int id)
{
	//m_vInts.push_back(id);
	for(size_t i = 0; i < m_vGuis.size(); i++)
	{
		if(m_vGuis[i] != NULL)
		{
			if(m_vGuis[i]->GetId() == id)
			{//找到了id对应的指针
				m_vGuiUnique.insert(std::pair<int , CGui*>(id, m_vGuis[i]));
				break;
			}
		}
	}
}

void CGuiSystem::Render(int id)
{
	std::map<int, CGui*>::iterator it = m_vGuiUnique.find(id);
	if(it != m_vGuiUnique.end())
	{//找到了GUI
		if(it->second != NULL)
		{
			(it->second)->Render();
		}
	}
	//for(size_t i = 0; i < m_vGuis.size(); i++)
	//	if(m_vGuis[i] != NULL)
	//	{
	//		if(m_vGuis[i]->GetId() == id && m_vGuis[i]->GetRender())
	//		{
	//			m_vGuis[i]->Render();
	//			break;
	//		}
	//	}
}
