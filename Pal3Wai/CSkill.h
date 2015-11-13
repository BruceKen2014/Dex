

#pragma once 

#include "../Source/CParticalSystem.h"
#include "../DexBase/CIni.h"
#include "../sound/CSoundSystem.h"
#include "CEnemy.h"

extern bool g_OpenFog;
enum
{
	SKILL_FLAG,
	SKILL_ATTACK,
	SKILL_ADDBLOOD,
};
//��������
class CSkill
{
protected:
	LPDIRECT3DDEVICE9 m_device;	 
	CEnemy*           m_enmy;   //�������ĸ����˵�ָ��
	char              m_sndFile[64];
	int               m_id;     //ID
	int               m_type;   //����
	int               m_num;    //����ֵ���߼�Ѫ��
	float	m_posX, m_posY, m_posZ;    //λ��
	
	bool m_isComplete;  //�����Ƿ����

public:
	CSkill(LPDIRECT3DDEVICE9 device);
	virtual ~CSkill() {};

public:
	virtual void ShutDown() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void SetEnmy(CEnemy* enmy) ;
	virtual CEnemy* GetEnmy();
	virtual void SetPosition(D3DXVECTOR3 pos)  { m_posX = pos.x;m_posY = pos.y;m_posZ = pos.z;};

	virtual bool GetIsComplete() { return m_isComplete;};
	virtual int  GetId()		 { return m_id;};
	virtual int  GetType()       { return m_type;};
	virtual int  GetNum();
};

//������Ѫ
class CSkillAddBlood:public CSkill
{
protected:
	//pos����Ϊ��������������
	char                m_file[64];
	CPartical*          m_particalList;            //��������
	DWORD               m_FVF;               //����ϵͳ�����ӵĶ�������
	LPDIRECT3DTEXTURE9  m_texture;           //����
	LPDIRECT3DVERTEXBUFFER9 m_particalBuffer;  //���ӻ���	
	float               m_size;              //ϵͳ�е����Ӵ�С  
	float               m_velocity;          //�����ٶ�
	float               m_width, m_height, m_depth;
	int                 m_particalCount;           //��������
	int                 m_time;             //ϵͳ����ʱ��
	int                 m_currTime;         //��ǰ�Ѿ�����ʱ��
	DexColor           m_color;             //ϵͳ�е�������ɫ
	//bool              m_isUniqueColor;     //ϵͳ�Ƿ��ǵ�ɫ��(�������ͷײ�ɫ)
	//bool              m_render;            //�Ƿ�����Ⱦ

public:
	CSkillAddBlood(LPDIRECT3DDEVICE9 device);
	virtual ~CSkillAddBlood() ;

public:
	virtual void ShutDown() ;
	virtual void Update() ;
	virtual void Render() ;

protected:
	virtual bool CreatePartical();
public:
	virtual void LoadIni(char* filename, char*filed);
	virtual bool LoadTexture(char* filename);  //��������
	virtual bool Initial();
	void         RePlay();  //ʱ�䵽��֮����ͷ����
	//virtual void SetTexture(LPDIRECT3DTEXTURE9 texture) = 0;
	//virtual void SetParticalVelocity(float velocity) = 0; //���������ٶȣ��ݲ���ʵʱ�ı���Ⱦ�������ٶ�
	//virtual void SetParticalSize(float size) = 0;     //�������Ӵ�С����ʵʱ�ı����Ӵ�С
	//virtual void SetColor(D3DXCOLOR color = PARTICAL_COLOR) = 0;	//����������ɫ       
	//virtual void SetIsUniqueColor(bool isbool);       //�����Ƿ�ɫ��
	//virtual void SetRender(bool render);              //�Ƿ���Ⱦ
	//virtual bool GetIsUniqueColor() const;
	//virtual bool GetRender() const;
};