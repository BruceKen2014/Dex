

/*************************************************************
****DemoEx��������ϵͳCPariticalSystem.h
****��飺CPariticalSystem����ϵͳ����
          ���ɴ����������������������Ч
****@���ߣ�������(BruceKen)
****@����:2012-10-14 
*************************************************************/

#ifndef _CPARTICALSYSTEM_H
#define _CPARTICALSYSTEM_H


#include <iostream>
#include <vector>
#include <math.h>
#include "../DexBase/typedefine.h"
//#include <d3d9.h>
//#include <d3dx9.h>

#define D3DFVF_PARTICAL (D3DFVF_XYZ | D3DFVF_DIFFUSE ) 

#define PARTICAL_VELOCITY  0.5f    //Ĭ�ϵ������ٶ�(���������ϵĺ��ٶ�)
#define PARTICAL_SIZE      0.5f    //Ĭ�ϵ����Ӵ�С
#define PARTICAL_COLOR     DexColor(1.0f, 1.0f, 1.0f)	 //Ĭ�ϵ�������ɫ

typedef enum
{
	PS_NOTYPE,         //ϵͳ����
	PS_FIREWORK = 1,   //�̻�����ϵͳ
	PS_RAIN     = 2,   //��ѩ����ϵͳ
	PS_FIRE     = 3,   //��������ϵͳ	
	PS_WATERFALL= 4,   //�ٲ�����ϵͳ
}PS_TYPE;	  //����ϵͳ����


struct CPartical
{
	//����������Ⱥ����Ͷ����ʽ���Ⱥ�˳����ͬ��������Ⱦ���ܲ���ȷ
	float m_posX, m_posY, m_posZ;    //����λ��
	DexColor m_color;                   //������ɫ 
	float m_size;                    //���Ӵ�С
	float m_velocityX, m_velocityY, m_velocityZ;  //�����ٶ�
	float m_deltaX, m_deltaY, m_deltaZ;           //���ٶ�
public:
	CPartical()
	{
		m_posX = m_posY = m_posZ; 
		m_color = D3DCOLOR_XRGB(255,255,255);
		m_size = PARTICAL_SIZE; 
		m_velocityX = m_velocityY = m_velocityZ = 0;
		m_deltaX = m_deltaY = m_deltaZ = 0;
	}
};
	 
///////////////////////////////////////////////////////////
//����ϵͳ����
class CParticalSystem      
{
protected:
	PS_TYPE	          m_type;              //ϵͳ���� ���̻�����ѩ��
	int               m_id;                //ϵͳID
	DWORD             m_FVF;               //����ϵͳ�����ӵĶ�������
	LPDIRECT3DDEVICE9 m_D3DDevice;         //�豸ָ��	
	LPDIRECT3DTEXTURE9  m_texture;         //����
	LPDIRECT3DVERTEXBUFFER9 m_particalBuffer;  //���ӻ���	
	float             m_size;              //ϵͳ�е����Ӵ�С  
	float             m_velocity;          //�����ٶ�
	float    m_posX, m_posY, m_posZ;       //ϵͳλ��
	int         m_particalCount;           //��������
	DexColor         m_color;             //ϵͳ�е�������ɫ
	bool              m_isUniqueColor;     //ϵͳ�Ƿ��ǵ�ɫ��(�������ͷײ�ɫ)
	bool              m_render;            //�Ƿ�����Ⱦ

public:
	CParticalSystem();
	virtual ~CParticalSystem(){};
    virtual void ShutDown() = 0;
	virtual bool Check() = 0; 
public:
	virtual bool Initial() = 0;
	virtual void Render() = 0;
	virtual void UpdatePartical() = 0;

	virtual bool LoadTexture(char* filename);  //��������
	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture) = 0;
	virtual void SetParticalVelocity(float velocity) = 0; //���������ٶȣ��ݲ���ʵʱ�ı���Ⱦ�������ٶ�
	virtual void SetParticalSize(float size) = 0;     //�������Ӵ�С����ʵʱ�ı����Ӵ�С
	virtual void SetColor(DexColor color = PARTICAL_COLOR) = 0;	//����������ɫ       
	virtual void SetIsUniqueColor(bool isbool);       //�����Ƿ�ɫ��
	virtual void SetRender(bool render);              //�Ƿ���Ⱦ
	
	virtual void SetPosition(D3DXVECTOR3 pos);
	virtual bool GetIsUniqueColor() const;
	virtual bool GetRender() const;
	//ע��					 
	//��Ⱦ�ڼ�ɵ���SetIsUniqueColor ��	 SetColor����������̬�ı�������ɫ
	//����ԭ���ǲ�ɫ �������Ϊ��ɫ ��������Ϊ
	//SetIsUniqueColor(true)
	//SetColor(color)
	//��֮ͬ��
	//SetIsUniqueColor(false)
	//SetColor()
};

///////////////////////////////////////////////////////////
//�̻�����ϵͳ
class CFireworks: public CParticalSystem	
{
//��һ�汾:�������뱬ը�ٶȷֿ�
protected: 
	//ԭ����ϵͳλ����Ϣ��Ϊ�����
	CPartical * m_particalList;            //��������
	D3DXVECTOR3 m_explodePoint;     //��ը��
	D3DXVECTOR3 m_launchDirection;  //���䷽����(0,1,0) ��Y���������� 
	float m_launchDistance;         //������룬���ӷ���㵽��ը��ľ���
	float m_nowDistance;            //��ǰ����
	float m_explodeRadius;          //��ը�뾶
public:
	CFireworks();
	CFireworks(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CFireworks();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	//ֻ������Ⱦǰ���õķ���
	
	
	//��Ⱦ�ڼ���Ȼ���Ե��õķ���	
	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(DexColor color = PARTICAL_COLOR);
public:	
	//�����̻�ϵͳ���в���
	virtual void SetFirework(D3DXVECTOR3 direction, float distance, float radius);  

};

///////////////////////////////////////////////////////////
//��ѩ����ϵͳ
class CRainSystem: public CParticalSystem
{
//��һ�汾���������ѩ�ķ������ԣ���������
protected:
	//ԭ����λ����Ϣ��Ϊ����������ĵ�
	CPartical * m_particalList;            //��������
	float m_width, m_height, m_depth; //������ĳ�������
public:
	CRainSystem();
	CRainSystem(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CRainSystem();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(DexColor color = PARTICAL_COLOR);

public:
	//������ѩϵͳ���в���
	virtual void SetRainPS(float width, float height, float _depth);
};

///////////////////////////////////////////////////////////
//��������ϵͳ
class CFireSystem: public CParticalSystem
{
	//��һ�汾
protected:
	struct CFirePartical
	{
		//����������Ⱥ����Ͷ����ʽ���Ⱥ�˳����ͬ��������Ⱦ���ܲ���ȷ
		float m_posX, m_posY, m_posZ;    //����λ��
		ULONG m_color;                   //������ɫ 
		float m_size;                    //���Ӵ�С
		float m_velocityX, m_velocityY, m_velocityZ;  //�����ٶ�
		float m_deltaX, m_deltaY, m_deltaZ;           //���ٶ�
		float m_lifetime;                //���ӵ�����ʱ��
		float m_currentime;              //���ӵĵ�ǰ����
	public:
		CFirePartical()
		{
			m_posX = m_posY = m_posZ; 
			m_color = D3DCOLOR_XRGB(255,255,255);
			m_size = PARTICAL_SIZE; 
			m_velocityX = m_velocityY = m_velocityZ = 0;
			m_deltaX = m_deltaY = m_deltaZ = 0;
			m_lifetime = m_currentime = 0;
		}
	};
	CFirePartical * m_particalList;
	//ԭ����λ����Ϣ��Ϊ�������Ӳ��������ĵ�
	//����ģ��ԭ��Ϊ����һ����������������������ӣ���������ȼ��
	float   m_radius;	  //��������İ뾶
	float   m_pow;        //��������������������˻������������ʱ��
	float   m_wave;       //����Ĳ���ָ��,�⽫Ӱ�����Ĳ�������,ͨ�����ӵļ��ٶ�ָ�����Ч��
	short   m_r, m_g, m_b; //������ɫ
	                       //ԭ����������ɫ��ϢΪ��ɫ�����������ĵ���ɫ
public:
	CFireSystem();
	CFireSystem(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CFireSystem();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(DexColor color = PARTICAL_COLOR); //������������Ϊ����ɫ
	
	virtual void SetFireColor(short r, short g, short b);//����ͳһ����������ɫ���������û����������ɫ

public:
	//���û���ϵͳ���в���
	virtual void SetFirePS(float radius, float _pow, float wave, short r, short g, short b);
};





///////////////////////////////////////////////////////////
//�ٲ�����ϵͳ(�Ƚ��ٲ��������㷨��õ�ʱ����ʵ��)
/*class CWaterfall:public	 CParticalSystem
{
	//ԭ�ȵ�λ����Ϣ����Ϊ�ٲ�ˮ��������
	//����ģ���ԭ��Ϊһ��С������ָ��λ��������к
protected:
	struct CWaterfallPartical
	{
		//����������Ⱥ����Ͷ����ʽ���Ⱥ�˳����ͬ��������Ⱦ���ܲ���ȷ
		float m_posX, m_posY, m_posZ;    //����λ��
		ULONG m_color;                   //������ɫ 
		float m_size;                    //���Ӵ�С
		float m_velocityX, m_velocityY, m_velocityZ;  //�����ٶ�
		float m_deltaX, m_deltaY, m_deltaZ;           //���ٶ�
		float m_lifetime;                //���ӵ�����ʱ��
		float m_currentime;              //���ӵĵ�ǰ����
		float m_weight;                  //���ӵ�����
	public:
		CWaterfallPartical()
		{
			m_posX = m_posY = m_posZ; 
			m_color = D3DCOLOR_XRGB(255,255,255);
			m_size = PARTICAL_SIZE; 
			m_velocityX = m_velocityY = m_velocityZ = 0;
			m_deltaX = m_deltaY = m_deltaZ = 0;
			m_lifetime = m_currentime = 0;
			m_weight = 0;
		}
	};
protected:
	CWaterfallPartical * m_particalList;
	D3DXVECTOR3 m_direction;  //ˮ���ķ���
	float m_width, m_length;  //ˮ���ĳ���
	float m_height;           //�ٲ��ĸ߶ȣ��⽫Ӱ�����ӵ�����ʱ��
	float m_gravity;          //�������ٶ�
public:
	CWaterfall();
	CWaterfall(LPDIRECT3DDEVICE9 g_D3DDevice, float posX, float posY, float posZ, int particalCount);
	~CWaterfall();
	virtual void ShutDown();
	virtual bool Check();
protected:
	virtual bool CreatePartical();
public:
	virtual bool Initial();
	virtual void Render();
	virtual void UpdatePartical();

	virtual void SetTexture(LPDIRECT3DTEXTURE9 texture);
	virtual void SetParticalVelocity(float velocity);
	virtual void SetParticalSize(float size);
	virtual void SetColor(D3DXCOLOR color = PARTICAL_COLOR); //������������Ϊ����ɫ

public:
	//�����ٲ�ϵͳ���в���
	virtual void SetWaterfall(D3DXVECTOR3 direction, float width, float length, float height, float gravity);
};
*/
///////////////////////////////////////////////////////////
//����ϵͳ������
//�ӽű��м�������ϵͳ����
class CParticalController
{
protected:
	std::vector<CParticalSystem*> m_particalvector;   //����ϵͳ����
};
#endif