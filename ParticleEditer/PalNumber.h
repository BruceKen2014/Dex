#pragma  once
#include "DexBase/typedefine.h"
#include "DexBase/DexObject.h"
#include "Source/CTexture.h"
#include "DexBase/CIni.h"

#include "PalTypeDefine.h"

//���ֻ���
class PalNumber
{
public:
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
	//LPDIRECT3DTEXTURE9 m_texture;  //����ͼƬ����
protected:
	int  m_number ; //Ҫ��ʾ������
	CTexture* m_texture;
	std::vector<int>  m_numberVec; //�����������洢����ת����ĸ�λ��������
	TSHORT m_startPosX, m_startPosY; //����λ��
	int   m_NumberWidth;  //ÿ�������ַ��Ŀ��
	int   m_offSet; //����֮��ļ��(��������Ĭ����һ���ַ����)
	bool  m_OpenSign; //�Ƿ���������ʾ
public:
	PalNumber();
	virtual ~PalNumber();	 
	virtual void ShutDown();
public:	  
	virtual void Update();
	virtual void Render();
	virtual bool LoadTexture(const char* filename);
	virtual void SetNumber(int number);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetOpenSign(bool openSign);


};

//��̬����	

class PalNumberAni: public PalNumber
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
	PalNumberAni();
	virtual ~PalNumberAni();
public:
	virtual void Update() ;	 
	virtual void Render();

	virtual bool LoadIni(const char* filename, char*filed);

	virtual void SetSpeed(int speed);
	virtual void SetPos(TSHORT posX, TSHORT posY);
	virtual void SetEndPos(TSHORT endX, TSHORT endY);
	virtual void ResetAlpha();  //����alpha
	virtual bool GetComplete(); //�����Ƿ���ʾ���

};


class PalNumberManager: public DexObject
{
protected:
	std::list<PalNumberAni*>  g_listNumbers;

public:
	PalNumberManager();
	~PalNumberManager();

public:
	PalNumberAni* createNumberAni(EPlayerType type, bool skill_good, EPalPlayerHpType hpType);
	bool Render();
	bool Update(int delta);

};