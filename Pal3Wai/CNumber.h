
/*
Dex����������
*/
#include "../DexBase/typedefine.h"
#include "../Source/CTexture.h"
#include "../DexBase/CIni.h"

#pragma  once

enum
{
	CN_POSITIVE, //����
	CN_NEGATIVE, //����
	CN_NOSIGN,   //�޷���
};
enum
{
	NUMBER_DEFAUT, //Ĭ������
	NUMBER_PIC,    //��������
};
//���ֻ���
class CNumber
{
	//LPDIRECT3DTEXTURE9 m_texture;  //����ͼƬ����
protected:
	LPDIRECT3DDEVICE9 m_device;
	int  m_number ; //Ҫ��ʾ������
	CTexture* m_texture;
	std::vector<int>  m_numberVec; //�����������洢����ת����ĸ�λ��������
	TSHORT m_startPosX, m_startPosY; //����λ��
	int   m_NumberWidth;  //ÿ�������ַ��Ŀ��
	int   m_offSet; //����֮��ļ��(��������Ĭ����һ���ַ����)
	bool  m_OpenSign; //�Ƿ���������ʾ
public:
	CNumber(LPDIRECT3DDEVICE9 device);
	virtual ~CNumber();	 
	virtual void ShutDown();
public:	  
	virtual void Update();
	virtual void Render();
	virtual bool LoadTexture(char* filename);
	virtual void SetNumber(int number);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetOpenSign(bool openSign);
	

};

//��̬����	

class CNumberAni: public CNumber
{
protected:
	int m_type;	 //��������
	TSHORT m_currPosX, m_currPosY;
	TSHORT m_endPosX, m_endPosY;
	TSHORT m_speed;  //�ƶ��ٶ�	
	TSHORT m_alpha;  //��ǰalpha
	TSHORT m_alphaSpeed; //alpha�����ٶ�
	bool   m_complete; //�Ƿ��ƶ����

public:
	CNumberAni(LPDIRECT3DDEVICE9 device);
	virtual ~CNumberAni();
public:
	virtual void Update() ;	 
	virtual void Render();

	virtual bool LoadIni(char* filename, char*filed);

	virtual void SetSpeed(int speed);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetEndPos(TSHORT endX, TSHORT endY);
	virtual void ResetAlpha();  //����alpha
	virtual bool GetComplete(); //�����Ƿ���ʾ���
	
};
