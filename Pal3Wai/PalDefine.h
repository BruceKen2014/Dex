

#pragma  once



#define WINDOW_CLASS      "Pal"
#define WINDOW_NAME       "����С��֮�����ħƪ"
#define WINDOW_POSITION_X 100
#define WINDOW_POSITION_Y 100
#define WINDOW_WIDTH      800
#define WINDOW_HEIGHT     600	 
#define FULLSCREEN        0

#define PROJECT_FOV      45.0f
#define PROJECT_ASPECT	 ((float)WINDOW_WIDTH)/((float)WINDOW_HEIGHT)
#define PROJECT_NEAR     0.1f
#define PROJECT_FAR      10000.0f

#define PAL_MUSIC_TOWN         "music\\8.mid"	   //�������׳�
#define PAL_MUSIC_TOWN_WIN     "music\\9.mid"  //��������
#define PAL_MUSIC_OUTDOORS     "music\\10.mid"

#define PAL_MUSIC_WENQING	   "music\\7.mid"
#define PAL_MUSIC_FIGHT        "music\\1.mid"
#define PAL_MUSIC_FIGHT2       "music\\18.mid"

#define PAL_SOUND_BUTTON_ON      "sound\\button_on.wav"
#define PAL_SOUND_BUTTON_DOWN    "sound\\button_down.wav"
#define PAL_SOUND_LOSE           "sound\\lose.wav"
#define PAL_SOUND_WIN            "sound\\win.wav"

#define PAL_SOUND_WALK_TOWN      "sound\\walk_grass.wav"

#define PAL_SOUND_INDEX0         0  //����ͨ��0 Ĭ��
#define PAL_SOUND_INDEX1         1  //����ͨ��1
#define PAL_SOUND_INDEX2         2  //����ͨ��2
#define PAL_SOUND_INDEX3         3  //����ͨ��3

#define IDI_ICON1                     001



#define PAL_CREATE_TOWN        "script\\create_town.lua"
#define PAL_CREATE_OUTDOOR     "script\\create_outdoor.lua"

#define PAL_DATA_INI        "ini\\data.ini"
#define PAL_GUI_INI_TOWN    "ini\\GuiGame.ini"
#define PAL_FONT            "ini\\Font.ini"
#define PAL_INTERFACE_GUI   "ini\\InterGui.ini"
#define PAL_INTRODUCE_GUI   "ini\\IntroGui.ini"
#define PAL_FIGHT_GUI       "ini\\FightGui.ini"
#define PAL_FIGHT_GUI_BACK  "ini\\FightBack.ini"
#define LUAFILE_SPEAK       "script\\speak.lua"

#define HEAD_JINGTIAN       "res\\head2D\\jingtian.png"
#define HEAD_JINGTIAN_ANGRY	"res\\head2D\\jingtian_angry.png"
#define HEAD_JINGTIAN_JING	"res\\head2D\\jingtian_jingya.png"
#define	HEAD_LONGKUI        "res\\head2D\\longkui.png"
#define HEAD_JINGJING       "res\\head2D\\jingjing.png"
#define HEAD_DRAGON         "res\\head2D\\zhulong.png"
#define HEAD_SHOUZI         "res\\head2d\\shou.png"

#define SPEAK_JINGTIAN_NORMAL   10
#define SPEAK_JINGTIAN_ANGRY    (SPEAK_JINGTIAN_NORMAL+1)
#define SPEAK_DRAGON            (SPEAK_JINGTIAN_NORMAL+2)
#define SPEAK_JINGTIAN_JING     (SPEAK_JINGTIAN_NORMAL+3)
#define SPEAK_SHOUZI            (SPEAK_JINGTIAN_NORMAL+4)

#define SPEAK_JINGJING_NORAML   20
#define SPEAK_LONGKUI_NORMAL    40

//ͼƬ
#define GUI_ID_INTRODUCE        39   //���ܱ���
#define GUI_ID_INFOBACKTOWER    40	 //���汳�� ��������
#define GUI_ID_ABOUT            120  //������Ϸ
#define GUI_ID_NEWSTART         121	 //�µĿ�ʼ
#define GUI_ID_LEAVE            122  //�뿪��Ϸ
#define GUI_ID_RETURN           123  //���ܽ����лص����水ť
#define GUI_ID_LEAVE2           130  //�뿪��Ϸ2 �����е�gui

 /*
#define  SWITCHPARA(para) \
	switch(para)          \
	{

#define  CASEPARA(para_value, xPos, yPos, flag, onfun)  \	 
	case para_value: \      
	{				 \
		onfun(xPos, yPos, flag);\
		break;                  \
	}
#define  ENDSWITCH    \
	default:          \
		break;        \
	}
	*/


//��״̬
typedef enum 
{
	PAL_FLAG,
	PAL_INTERFACE,  //���� 
	PAL_INTRODUCE,  //����
	PAL_LOADING,    //����
	PAL_TOWN,       //����
	PAL_OUTDOORS,   //Ұ��

	PAL_FIGHT,      //ս��
}  PAL_STATE;
enum
{
	FIGHT_FLAG,         //û�в���
	FIGHT_MOVE_CAMERA,   //������ƶ��׶�
	FIGHT_LABEL,         //�������ƶ��׶�
	FIGHT_SELECT_ORDER, //ѡȡָ��׶�
	FIGHT_SELECT_TARGET,  //ѡȡĿ��׶�
	FIGHT_ATTACKING,    //���ڹ����׶Σ������Լ��������˺͵��˹����Լ���
	FIGHT_SKILLING,     //ʩչ�����׶�	 ��
	FIGHT_LOSE,         //ս��ʧ��
	FIGHT_WIN,           //ʤ��
};
//��״̬
typedef enum
{
	PAL2_FLAG,
	PAL2_SPEAK,      //��̸

}PAL2_STATE;

#define  FirstState PAL_TOWN //PAL_INTERFACE
