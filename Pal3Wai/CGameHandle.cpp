

/*
处理游戏的交互函数
*/

#include "../DexBase/typedefine.h"
#include "../Source/CEngine_h.h"
#include "PalDefine.h"
#include "CNpc.h"
#include "CEnemy.h"
#include "CSpell.h"
#include "CFightLabel.h"
#include "CSkill.h"
#include "CSoundObject.h"
#include "CTestPanel.h"


#include <map>


//外部已实现的函数，本文件要调用的
extern bool  DoLuaFile(char* file);

extern HWND g_hwnd;
extern HINSTANCE g_hInstance;

extern CTestPanel* g_testPanel;

extern LPDIRECT3DDEVICE9 g_D3DDevice;
extern D3DXMATRIX g_projection;
extern D3DXMATRIX g_ViewMatrix;
extern D3DXMATRIX g_worldMatrix;  //世界坐标矩阵

extern CCamera g_camera;

extern HCURSOR       g_Cursors[3];

extern PAL_STATE   g_palState; //主状态
extern PAL2_STATE	g_State2;	//副状态
extern CGuiSystem*   g_pGuiInterface; //界面GUI
extern CGuiSystem*	 g_pGuiTown ;	 //城镇GUI(野外、城镇暂时公用一套GUI)
extern CGuiSystem*	 g_pGuiFight; //战斗GUI



extern lua_State *L;
extern CTerrain*     g_terrain ;	    //地形
extern CEnemy*   g_Jintian;
extern CEnemy* g_SelectEnemy;
extern CPick*         g_pPick;

extern CSpell * spell;
extern std::vector<CCollideBox*>  g_collideBox; 
extern std::vector<CCollideSphere*>  g_collideSphere;
extern std::vector<CModel*>  g_modelVector;
extern std::vector<CNpc*>  g_npcVector;
extern std::vector<CModelXAni*>  g_aniModelVector;	    //具有动态属性的物体
extern std::vector<CSkill*> g_Skills;
extern std::vector<CSoundObject*> g_sndObject; 

extern std::vector<CEnemy*> g_enemyVector;    //战斗系统中的敌人

extern CFightLabel* g_FightLabel;  //战斗进度条类

extern CSpeak*	g_speakFrame;
extern std::map<int, CTextureEx*>  g_headmap ;
extern std::vector<TalkStruct*> g_talk;
extern size_t g_talkcontrol;
extern int   g_clickId;
extern size_t g_clickVIndex;

extern DexVector3    g_crossPoint;

extern PAL2_STATE	g_State2;	
extern int  g_iXMouse, g_iYMouse;
extern bool g_isFightComplete;
extern bool g_isSelectEnemy;
extern bool g_isAttacking;    //是否正在攻击

extern int  g_FightStage;     //战斗阶段

extern D3DXVECTOR3      g_playPos;   //切换到战斗状态时保存景天位置，战斗结束时切换回来
extern D3DXVECTOR3      g_playDir;   //保存朝向

extern D3DXVECTOR3      g_cameraPos; //保存摄像机位置


//外部声明由本文件实现的
extern void  MouseMoveIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseMoveInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseMoveTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseMoveOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseMoveFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);


extern void  MouseLDownIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLDownInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLDownTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLDownOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLDowhFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);

extern void  MouseLUpIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLUpInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLUpTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLUpOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseLUpFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);

extern void  MouseRUpInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseRUpTown(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseRUpOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag);
extern void  MouseRUpFight(TSHORT xPos, TSHORT yPos, MouseFlag flag);


extern void  KeyUpTown();
extern void  KeyUpOutdoors();
extern void  KeyUpFight();

extern void  KeyDownTown();
extern void  KeyDownOutdoors();
extern void  KeyDownFight();
 
extern bool  ApplyIntroduce();
extern bool  ApplyInterface();
extern bool  ApplyTown();
extern bool  ApplyOutdoors();
extern bool  ApplyLoading();
extern bool  ApplySpeak();
extern bool  ApplyFight();

extern void  ReleaseIntroduce();
extern void  ReleaseInterface();
extern void  ReleaseTown();
extern void  ReleaseOutdoors();
extern void  ReleaseLoading();
extern void  ReleaseSpeak();
extern void  ReleaseFight();

void  MouseMoveIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	if(g_pGuiInterface == NULL)
		return;
	for(size_t i = 0 ; i < g_pGuiInterface->GetGuiVector().size(); i++)
	{
		if(g_pGuiInterface->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
		{
			if(g_pGuiInterface->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseMove) == GRV_ButtonOn)
			{
				CSound::getSoundSingleton().Play(PAL_SOUND_BUTTON_ON);
				break;
			}
		}
	}
}

void  MouseMoveInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	//for(size_t i = 0 ; i < g_pGuiInterface->GetGuiVector().size(); i++)
	//{
	//	if(g_pGuiInterface->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
	//	{
	//		if(g_pGuiInterface->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseMove) == GRV_ButtonOn)
	//		{
	//			if(g_pSound != NULL)
	//			{
	//				CSound::getSoundSingleton().Play(PAL_SOUND_BUTTON_ON);
	//			}
	//			break;
	//		}
	//	}
	//}
}


void MouseMoveTown(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	if(g_pGuiTown == NULL)
		return;
	for(size_t i = 0 ; i < g_pGuiTown->GetGuiVector().size(); i++)
	{
		if(g_pGuiTown->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
		{
			if(g_pGuiTown->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseMove) == GRV_ButtonOn)
			{
				//CSound::getSoundSingleton().Play("Button_On.wav");
				break;
			} 
		}
	}
	stRay ray = g_pPick->GetRay(g_projection, g_ViewMatrix, xPos, yPos);
	for(size_t i = 0; i < g_npcVector.size(); i++)
	{
		if(g_npcVector[i]->CheckPick(ray))
		{
			SetCursor(g_Cursors[1]);
			break;
		} 
		else
		{
			SetCursor(g_Cursors[0]);
		}
	}
}
void  MouseMoveOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
}

void  MouseMoveFight(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	//if(g_isAttacking)
	//	//正在攻击，不响应操作
	//	return ;
	switch(g_FightStage)
	{
	case FIGHT_MOVE_CAMERA:
		{
			break;
		}
	case FIGHT_SELECT_ORDER:
		{
			if(g_pGuiFight != NULL)
			{
				for(size_t i = 0 ; i < g_pGuiFight->GetGuiVector().size(); i++)
				{
					if(g_pGuiFight->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
					{
						if(g_pGuiFight->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseMove) == GRV_ButtonOn)
						{
							CSound::getSoundSingleton().Play(PAL_SOUND_BUTTON_ON);
							break;
						}
					}
				}
			}
			break;
		}
	case FIGHT_SELECT_TARGET:
		{
			stRay ray = g_pPick->GetRay(g_projection, g_ViewMatrix, xPos, yPos);
			bool isOn = false;
			D3DXVECTOR3 pos; 
			for(size_t i = 0; i < g_enemyVector.size(); i++)
			{
				if(g_enemyVector[i]->GetCurrHp() == 0)
					continue;
				if(g_enemyVector[i]->CheckPick(ray))
				{
					pos = g_enemyVector[i]->GetPosition(); 
					pos.y += 1;
					spell->SetPos(pos);
					g_Jintian->FaceTo(pos);
					g_FightLabel->SetSelectHead(g_enemyVector[i]->GetFightHead());
					isOn = true;
					break;
				}
			}
			if(!isOn)
			{
				SetCursor(g_Cursors[0]);
				pos = g_Jintian->GetPosition(); 
				pos.y += 1;
				spell->SetPos(pos);
				g_FightLabel->SetSelectHead(NULL);
			}
			else
			{
				SetCursor(g_Cursors[2]);
			}
			break;
		}
	case FIGHT_ATTACKING:
		{
			break;
		}
	case FIGHT_WIN:
		{
			break;
		}
	default:
		break;
	}
}

void  MouseLDownIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	for(size_t i = 0 ; i < g_pGuiInterface->GetGuiVector().size(); i++)
	{
		if(g_pGuiInterface->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
		{
			if(g_pGuiInterface->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLDown) == GRV_ButtonDown)
			{
				CSound::getSoundSingleton().Play(PAL_SOUND_BUTTON_DOWN);
				break;
			}
		}
	}
}

void  MouseLDownInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	//for(size_t i = 0 ; i < g_pGuiInterface->GetGuiVector().size(); i++)
	//{
	//	if(g_pGuiInterface->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
	//	{
	//		if(g_pGuiInterface->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLDown) == GRV_ButtonDown)
	//		{
	//			CSound::getSoundSingleton().Play(PAL_SOUND_BUTTON_DOWN);
	//			break;
	//		}
	//	}
	//}
}

void MouseLDownTown(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	g_testPanel->OnMouseLDown(xPos, yPos);
	if(g_State2 == PAL2_SPEAK)
		return;
	static int de = 15;
	if(de-- < 0)
	{
		for(size_t i = 0 ; i < g_sndObject.size(); i++)
		{
			if(g_sndObject[i]->IsOn(g_Jintian->GetPosition()))
			{
				char info[32];
				sprintf(info, "开始播放音效");
				//OutputDebugStr(info);
				char sndFile[64];
				g_sndObject[i]->GetSndFile(sndFile);
				CSound::getSoundSingleton().Play(sndFile);
				sprintf(info, "完成播放音效");
				//OutputDebugStr(info);
				de = 15;
				break;
			}
		}
	}
	for(size_t i = 0 ; i < g_pGuiTown->GetGuiVector().size(); i++)
	{
		if(g_pGuiTown->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
		{
			if(g_pGuiTown->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLDown) == GRV_ButtonDown)
			{
				//CSound::getSoundSingleton().Play("Button_Down.wav");
				break;
			}
		}
	}
	CPlane plane(DexVector3(-1000, 0, -1000), DexVector3(-1000, 0, 0), DexVector3(1000, 0, 1000));
	stRay ray = g_pPick->GetRay(g_projection, g_ViewMatrix, xPos, yPos);
	//测试鼠标点击时，地面是否能够接受到射线
	if(plane.GetRelation(ray))
	{
		g_crossPoint = plane.GetCrossPoint(ray);
		//g_Jintian->SetPosition(g_crossPoint);
	}
	DexVector3  set_vector;
	memcpy(&set_vector, &(g_crossPoint - g_Jintian->GetBall().m_center), sizeof(D3DXVECTOR3));;
	set_vector.y = 0;

	g_Jintian->SetDirection(D3DXVECTOR3(set_vector.x, set_vector.y, set_vector.z));
	g_Jintian->SetVel(0.0f);
	g_Jintian->SetAnimation(JINTTIAN_RUN);
	g_Jintian->SetMoving(true);
	for(size_t i = 0; i < g_sndObject.size(); i++)
	{
		if(g_sndObject[i]->IsOn(g_Jintian->GetPosition()))
		{
			g_Jintian->SetPositionY(g_sndObject[i]->GetHeight(g_Jintian->GetPosition()));
			break;
		}
	}

	stBall player_ball = g_Jintian->GetBall();
	DexVector3 player_direction;
	memcpy(&player_direction, &g_Jintian->GetDirection(), sizeof(DexVector3));
	stBall temp_ball;
	bool collide = false;
	DexVector3 vec = g_camera.GetView();	   
	vec.y = 0;
	for( std::vector<CModel*>::iterator it = g_modelVector.begin(); it != g_modelVector.end(); it++)
	{
		if(!(*it)->GetCollide())
			continue;
		temp_ball = (*it)->GetBall();
		if(player_ball.GetRelation(temp_ball) == 0)
		{//相交,检测到碰撞 
			collide = true;
			DexVector3 temp_vector = g_Jintian->GetBall().m_center - (*it)->GetBall().m_center;
			if(player_direction.Dot(temp_vector) > 0)
			{//背着物体前进
				collide = false;
			}
			break;
		}	
	}
	for( std::vector<CModelXAni*>::iterator it = g_aniModelVector.begin(); it != g_aniModelVector.end(); it++)
	{
		if(!(*it)->GetCollide())
			continue;
		temp_ball = (*it)->GetBall();
		if(player_ball.GetRelation(temp_ball) == 0)
		{//相交,检测到碰撞 
			collide = true;
			DexVector3 temp_vector;
			memcpy(&temp_vector, &(g_Jintian->GetBall().m_center - (*it)->GetBall().m_center), sizeof(D3DXVECTOR3));
			if (player_direction.Dot(temp_vector) > 0)
			{//背着物体前进
				collide = false;
			}
			break;
		}	
	}
	if(!collide)
	{
		for( std::vector<CNpc*>::iterator it = g_npcVector.begin(); it != g_npcVector.end(); it++)
		{
			if(!(*it)->GetCollide())
				continue;
			temp_ball = (*it)->GetBall();
			if(player_ball.GetRelation(temp_ball) == 0)
			{//相交,检测到碰撞 
				collide = true;
				DexVector3 temp_vector;
				memcpy(&temp_vector, &(g_Jintian->GetBall().m_center - (*it)->GetBall().m_center), sizeof(D3DXVECTOR3));
				if (player_direction.Dot(temp_vector) > 0)
				{//背着物体前进
					collide = false;
				}
				break;
			}	

		}
	}
	if(!collide)
	{
		for( std::vector<CCollideSphere*>::iterator it = g_collideSphere.begin(); it != g_collideSphere.end(); it++)
		{
			if((*it)->GetRelation(g_Jintian->GetBall()) == 0)
			{//相交,检测到碰撞 
				collide = true;

				DexVector3 temp_vector;
				memcpy(&temp_vector, &(g_Jintian->GetBall().m_center - (*it)->m_center), sizeof(D3DXVECTOR3));
				if (player_direction.Dot(temp_vector) > 0)
				{//背着物体前进
					collide = false;
				}
				break;
			}	

		}
	}
	if(!collide)
	{
		for( std::vector<CCollideBox*>::iterator it = g_collideBox.begin(); it != g_collideBox.end(); it++)
		{
			DexVector3 inci;
			if((*it)->CheckCollide(g_Jintian->GetBall(), inci))
			{//检测到碰撞 
				collide = true;
				DexVector3 bcenter = g_Jintian->GetBall().m_center;
				DexVector3 temp_vector = inci - bcenter;
				temp_vector.y = 0;
				temp_vector.Normalize();
				if (player_direction.Dot(temp_vector) > 0)
				{//背着物体前进

					collide = false;
					//g_Jintian->SetDirection(temp_vector);

					//g_Jintian->Move(player_direction, -4.0f);
					//g_camera.MoveCamera(set_vector, -4.0f, true);
					g_Jintian->SetVel(0.0f);
					//g_camera.MoveCamera(set_vector, -4.0f, true);

					//g_camera.MoveCamera(-temp_vector, -2.0f, true);
				}
				//if(D3DXVec3Dot(&player_direction, &temp_vector) <= 0)
				//{//朝着物体前进
				//	D3DXVECTOR3 pos1 = g_Jintian->GetPosition();
				//	D3DXVECTOR3 temp = pos1 + temp_vector * 2;
				//	g_Jintian->SetPosition(temp);
				//	temp =g_camera.GetPosition() + temp_vector *2;
				//	g_camera.SetPosition(temp);
				//}
				break;
			}	

		}
	}
	
	if(!collide)
	{
		g_camera.MoveCamera(set_vector,4.0f, true);
		g_Jintian->Move(D3DXVECTOR3(set_vector.x, set_vector.y, set_vector.z), 4.0f);
		//g_camera.MoveCamera(D3DXVECTOR3(0.0f, 1.0f, 0.0f),delta, true);
	}

}



void MouseLDownOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	static int de = 15;
	if(de-- < 0)
	{
		CSound::getSoundSingleton().Play("sound\\walk_grass2.wav");
		de = 15;
	}
	CPlane plane(DexVector3(-1000, g_Jintian->GetPosition().y, -1000), DexVector3(-1000, g_Jintian->GetPosition().y, 0), DexVector3(1000, g_Jintian->GetPosition().y, 1000)); 
	stRay ray = g_pPick->GetRay(g_projection, g_ViewMatrix, xPos, yPos);
	//测试鼠标点击时，地面是否能够接受到射线
	if(plane.GetRelation(ray))
	{
		g_crossPoint = plane.GetCrossPoint(ray);
		//g_Jintian->SetPosition(g_crossPoint);
	}

	DexVector3 set_vector = g_crossPoint - g_Jintian->GetBall().m_center;
	set_vector.y = 0;

	g_Jintian->SetDirection(D3DXVECTOR3(set_vector.x, set_vector.y, set_vector.z));
	g_Jintian->SetVel(0.0f);
	g_Jintian->SetAnimation(JINTTIAN_RUN);
	g_Jintian->SetMoving(true);

	g_camera.MoveCamera(set_vector,4.0f, true);
	g_Jintian->Move(D3DXVECTOR3(set_vector.x, set_vector.y, set_vector.z), 4.0f);


	float playerY = g_terrain->GetHeight(g_Jintian->GetPosition().x, g_Jintian->GetPosition().z);
	float delta = playerY - g_Jintian->GetPosition().y;  
	g_Jintian->SetPositionY(playerY);
	g_camera.MoveCamera(DexVector3(0,1,0),delta, true);
}
void  MouseLDowhFight(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	switch(g_FightStage)
	{
	case FIGHT_MOVE_CAMERA:
		{
			break;
		}
	case FIGHT_SELECT_ORDER:
		{
			for(size_t i = 0 ; i < g_pGuiFight->GetGuiVector().size(); i++)
			{
				if(g_pGuiFight->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
				{
					if(g_pGuiFight->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLDown) == GRV_ButtonDown)
					{
						CSound::getSoundSingleton().Play(PAL_SOUND_BUTTON_DOWN);
						break;
					}
				}
			}
			break;
		}
	case FIGHT_SELECT_TARGET:
		{
			break;
		}
	case FIGHT_ATTACKING:
		{
			break;
		}
	case FIGHT_WIN:
		{
			break;
		}
	default:
		break;
	}
}


void  MouseLUpIntroduce(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	if(g_pGuiInterface == NULL)
		return;
	for(size_t i = 0 ; i < g_pGuiInterface->GetGuiVector().size(); i++)
	{
		if(g_pGuiInterface->GetGuiVector()[i] != NULL)
		{
			if(g_pGuiInterface->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
			{
				if(g_pGuiInterface->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLUp) == GRV_ButtonUp)
				{
					int id = g_pGuiInterface->GetGuiVector()[i]->GetId();
					switch(id)
					{
					case GUI_ID_RETURN:
						{
							g_palState = PAL_INTERFACE;
							ReleaseIntroduce();
							ApplyInterface();
							break;
						}
					}
					break;
					//根据不同的ID在此进行不同的处理
				}
			}
		}
	}
}


void  MouseLUpInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	return;
	if(g_pGuiInterface == NULL)
		return;
	for(size_t i = 0 ; i < g_pGuiInterface->GetGuiVector().size(); i++)
	{
		if(g_pGuiInterface->GetGuiVector()[i] != NULL)
		{
			if(g_pGuiInterface->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
			{
				if(g_pGuiInterface->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLUp) == GRV_ButtonUp)
				{
					int id = g_pGuiInterface->GetGuiVector()[i]->GetId();
					switch(id)
					{
					case GUI_ID_NEWSTART:
						{
							g_palState = PAL_TOWN;
							ReleaseInterface();
							ApplyTown();
							break;
						}
					case GUI_ID_ABOUT:
						{
							g_palState = PAL_INTRODUCE;
							ReleaseInterface();
							ApplyIntroduce();
							break;
						}
					case GUI_ID_LEAVE:
						{
							PostQuitMessage(0);
						}
					}
					break;
					//根据不同的ID在此进行不同的处理
				}
			}
		}
	}
}




void MouseLUpTown(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	if(g_State2 == PAL2_SPEAK)
	{
		static int con = 0;
		static int i = 0;
		if(con == 0)
		{
			//if(g_talkcontrol > g_talk.size())
			// return;
			g_speakFrame->SetSpeed(1);
			con = 1;
		}
		else
		{
			if(g_talkcontrol >= g_talk.size())
			{//对话进行完了
				for(std::vector<TalkStruct*>::iterator it = g_talk.begin(); it != g_talk.end();)
				{
					_SafeDelete(*it);
					g_talk.erase(it);
					it = g_talk.begin();
				}
				g_speakFrame->SetWords("");
				g_State2 = PAL2_FLAG;	
				if(!g_npcVector[g_clickVIndex]->GetStay())
					g_npcVector[g_clickVIndex]->ChangeWalkIndex();
				g_npcVector[g_clickVIndex]->SetMoving(true);
				D3DXVECTOR3 vec = g_npcVector[g_clickVIndex]->GetNextNode() - g_npcVector[g_clickVIndex]->GetPosition();
				vec.y = 0;
				D3DXVec3Normalize(&vec, &vec);
				g_npcVector[g_clickVIndex]->SetDirection(vec);
				return;
			}
			g_speakFrame->SetHead(g_headmap[g_talk[g_talkcontrol]->m_head], g_talk[g_talkcontrol]->m_dir);
			g_speakFrame->SetWords(g_talk[g_talkcontrol]->m_words);
			g_speakFrame->ReSetCurrLength();
			g_speakFrame->SetSpeed(100);
			g_talkcontrol++;
			con = 0;
		}
	}
	else
	{
		g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(JINGTIAN_STAND);
		for(size_t i = 0 ; i < g_pGuiTown->GetGuiVector().size(); i++)
		{
			if(g_pGuiTown->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
			{
				if(g_pGuiTown->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLUp) == GRV_ButtonUp)
				{
					int id = g_pGuiTown->GetGuiVector()[i]->GetId();
					//根据不同的ID在此进行不同的处理
					switch (id)
					{
					case GUI_ID_LEAVE2:
						{
							_SafeDelete(g_testPanel);
							PostQuitMessage(0);
							//g_palState = PAL_INTERFACE;
							//CMusicMidi::getMidiSingleton().Play(PAL_MUSIC_WENQING);
							//ApplyInterface();
							//ReleaseTown();
							break;
						}
					}
					break;
				}
			}
		}
		stRay ray = g_pPick->GetRay(g_projection, g_ViewMatrix, xPos, yPos);
		for(size_t i = 0; i < g_npcVector.size(); i++)
		{
			if(g_npcVector[i]->CheckPick(ray))
			{
				if(D3DXVec3Length(&(g_Jintian->GetPosition() - g_npcVector[i]->GetPosition())) < 50)
				{
					g_Jintian->FaceTo(g_npcVector[i]->GetPosition());
					g_clickId = g_npcVector[i]->GetId();
					g_clickVIndex  = i;
					g_State2 = PAL2_SPEAK;
					g_npcVector[i]->SetMoving(false);
					g_npcVector[i]->ChangeStandIndex();
					D3DXVECTOR3 vec = g_Jintian->GetPosition() - g_npcVector[i]->GetPosition();
					vec.y = 0;
					D3DXVec3Normalize(&vec, &vec);
					g_npcVector[i]->SetDirection(vec);
					g_talkcontrol = 0;
					DoLuaFile(LUAFILE_SPEAK);
				}  
				break;
			}
		}
		//static float sca = 1.0f;
		//g_Jintian->SetScale(sca);
		//sca +=1.0f;
	}
}

void  MouseLUpOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	g_Jintian->SetMoving(true);
	g_Jintian->SetAnimation(JINGTIAN_STAND);
}

void  MouseLUpFight(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
	switch(g_FightStage)
	{
	case FIGHT_MOVE_CAMERA:
		{
			break;
		}
	case FIGHT_SELECT_ORDER:
		{
			if(!g_isAttacking && g_pGuiFight != NULL)
			{
				for(size_t i = 0 ; i < g_pGuiFight->GetGuiVector().size(); i++)
				{   			
					if(g_pGuiFight->GetGuiVector()[i]->OnMouse(g_iXMouse, g_iYMouse, MF_MouseLUp) == GRV_ButtonUp)
					{
						if(g_pGuiFight->GetGuiVector()[i]->GetIsCommunicate())   //是交互性GUI
						{
							//CSound::getSoundSingleton().Play("Button_On.wav");
							int id = g_pGuiFight->GetGuiVector()[i]->GetId();
							switch(id)
							{
							case 110: //击
								{
									g_isSelectEnemy = true;
									//进入选取敌人阶段
									g_FightStage = FIGHT_SELECT_TARGET;
									//g_isAttacking = true;
									break;
								}
							case 111://技
								{
									g_FightStage = FIGHT_SKILLING;
									CSkillAddBlood * temp = new CSkillAddBlood(g_D3DDevice);
									temp->SetPosition(g_Jintian->GetPosition());
									temp->LoadIni("ini\\skill.ini", "AddBlood");
									if(!temp->Initial())
									{
										_Message("failed!");
										_SafeDelete(temp);
									}
									g_Skills.push_back(temp);
								}
							}
						} 
						break;
					}
				}
			}
			break;
		}
	case FIGHT_SELECT_TARGET:
		{
			stRay ray = g_pPick->GetRay(g_projection, g_ViewMatrix, xPos, yPos);
			D3DXVECTOR3 pos; 
			for(size_t i = 0; i < g_enemyVector.size(); i++)
			{
				if(g_enemyVector[i]->GetCurrHp() <= 0)
					continue;
				if(g_enemyVector[i]->CheckPick(ray))
				{//点击了某个敌人
					//g_enemyVector[i]->SetAttacking(true);
					//将法术圆环放回景天脚下
					g_SelectEnemy = g_enemyVector[i]; 
					pos = g_Jintian->GetPosition(); 
					pos.y += 1;
					spell->SetPos(pos);
					g_isSelectEnemy = false; 
					g_Jintian->SetDestPos(g_enemyVector[i]->GetPosition());
					g_Jintian->SetAttacking(true);
					g_FightStage = FIGHT_ATTACKING;//进入攻击阶段
					g_FightLabel->SetSelectHead(NULL);
					SetCursor(g_Cursors[0]);
					break;
				}
			}
			break;
		}
	case FIGHT_ATTACKING:
		{
			break;
		}
	case FIGHT_WIN:
		{
			break;
		}
	default:
		break;
	}
}

void  MouseRUpInterface(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
}
void  MouseRUpTown(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
}
void  MouseRUpOutdoors(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
#ifdef DEX_DEBGU
	g_palState = PAL_FIGHT;


	ApplyFight();
#endif
}
void  MouseRUpFight(TSHORT xPos, TSHORT yPos, MouseFlag flag)
{
#ifdef DEX_DEBGU
	g_palState = PAL_OUTDOORS;

	g_camera.SetPosition(DexVector3(g_cameraPos.x, g_cameraPos.y, g_cameraPos.y));
	g_camera.SetFocus(DexVector3(g_playPos.x, g_playPos.y, g_playPos.y)); 
	g_camera.EnableMove(false);

	g_Jintian->SetPosition(g_playPos);
	g_Jintian->SetDirection(g_playDir);
	ReleaseFight();
#endif
	SetCursor(g_Cursors[0]);
	if(g_FightStage == FIGHT_SELECT_TARGET)
		g_FightStage = FIGHT_SELECT_ORDER;
}

void KeyUpTown()
{	
#ifdef DEX_DEBUG
	if(g_input->KeyUp(DIK_UP))
	{
		//CSound::getSoundSingleton().Play("1.wav");
		g_State2 = PAL2_SPEAK;
		g_talkcontrol = 0;
		for(std::vector<TalkStruct*>::iterator it = g_talk.begin(); it != g_talk.end();)
		{
			_SafeDelete(*it);
			g_talk.erase(it);
			it = g_talk.begin();
		}
		if(int error = luaL_dofile(L, "script\\jingtian.lua") != 0)   //打开文件并编译
			_Message("对话脚本出错！");
	}
#endif
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_T))
	{	
		if(CMusicMidi::getMidiSingleton().IsPlaying())
			CMusicMidi::getMidiSingleton().Stop();
		else
			CMusicMidi::getMidiSingleton().Play();
	}
}
void KeyUpOutdoors()
{

}
void KeyUpFight()
{
#ifdef DEX_DEBUG
	if(g_input->KeyUp(DIK_LEFT))
	{
		//g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
	if(g_input->KeyUp(DIK_RIGHT))
	{
		//g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
	if(g_input->KeyUp(DIK_UP))
	{
		//g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
	if(g_input->KeyUp(DIK_DOWN))
	{
		//g_Jintian->SetMoving(false);
		g_Jintian->SetAnimation(0);
	}
#endif
}



void KeyDownTown()
{
#ifdef DEX_DEBGU
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_A))
	{
		g_camera.RotateFocus(-0.2f);
	}	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_D))
	{
		g_camera.RotateFocus(0.2f);
	}
	float vel = 4.0f;	
	DexVector3 vec = g_camera.GetView();
	vec.y = 0;
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_S))
	{	
		g_camera.MoveCamera(CCamera::DIR_VIEW, -vel);
	}	
	if(CInputSystem::GetInputSingleton().KeyDown(DIK_W))
	{	
		g_camera.MoveCamera(CCamera::DIR_VIEW, vel);
	}
#endif
}

void KeyDownOutdoors()
{
#ifdef DEX_DEBUG
	if(g_input->KeyDown(DIK_A))
	{
		g_camera.RotateFocus(-0.2f);
	}	
	if(g_input->KeyDown(DIK_D))
	{
		g_camera.RotateFocus(0.2f);
	}
	float vel = 4.0f;	
	D3DXVECTOR3 vec = g_camera.GetView();
	vec.y = 0;
	if(g_input->KeyDown(DIK_S))
	{	
		g_camera.MoveCamera(CCamera::DIR_VIEW, -vel);
	}	
	if(g_input->KeyDown(DIK_W))
	{	
		g_camera.MoveCamera(CCamera::DIR_VIEW, vel);
	}	
#endif
}
void KeyDownFight()
{
#ifdef DEX_DEBUG
	//升降摄像机
	if(g_input->KeyDown(DIK_SPACE))
	{
		g_camera.MoveCamera(DIR_UP,5);
	}
	if(g_input->KeyDown(DIK_LCONTROL))
	{
		g_camera.MoveCamera(DIR_UP, -5);
	}

	//旋转摄像机
	if(g_input->KeyDown(DIK_A))
	{
		g_camera.RotateFocus(-0.2f);
	}	
	if(g_input->KeyDown(DIK_D))
	{
		g_camera.RotateFocus(0.2f);
	}
	float vel = 4.0f;	
	D3DXVECTOR3 vec = g_camera.GetView();
	vec.y = 0;
	if(g_input->KeyDown(DIK_S))
	{	
		g_camera.MoveCamera(CCamera::DIR_VIEW, -vel);
	}	
	if(g_input->KeyDown(DIK_W))
	{	
		g_camera.MoveCamera(CCamera::DIR_VIEW, vel);
	}	

	//移动景天
	if(g_input->KeyDown(DIK_LEFT))
	{
		g_Jintian->SetDirection(-g_camera.GetRight());
		g_Jintian->SetMoving(true);
		g_Jintian->SetAnimation(3);
		g_Jintian->Move(-g_camera.GetRight(), 4.0f);
	}
	if(g_input->KeyDown(DIK_RIGHT))
	{
		g_Jintian->SetDirection(g_camera.GetRight());
		g_Jintian->SetMoving(true);
		g_Jintian->SetAnimation(3);
		g_Jintian->Move(g_camera.GetRight(), 4.0f);
		//g_camera.MoveCamera(g_camera.GetRight(), vel, true);
	}
	if(g_input->KeyDown(DIK_UP))
	{
		g_Jintian->SetAnimation(3);
		g_Jintian->SetDirection(vec);
		g_Jintian->SetMoving(true);
		g_Jintian->Move(vec, 4.0f);
		//CMusicMidi::getMidiSingleton().Play("3.mid");
		//g_camera.MoveCamera(vec, vel, true);
	}	
	if(g_input->KeyDown(DIK_DOWN))
	{
		g_Jintian->SetDirection(-vec);
		g_Jintian->SetMoving(true);
		g_Jintian->SetAnimation(3);
		g_Jintian->Move(-vec, 4.0f);

		//CMusicMidi::getMidiSingleton().Play("1.mid");
		//g_camera.MoveCamera(-vec, vel, true);
	}
#endif
}
