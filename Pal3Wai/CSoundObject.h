/*
���ߣ�������
2012.12
  �ܷ�������������ߵĲ�ͬ���壬������ͬ�����Ķ���
  ��ݵء�ʯ·���ŵ�
*/

#pragma  once
#include "../DexBase/typedefine.h"


enum
{
	CS_LEFT,    //����
	CS_RIGHT,   //����
	CS_FRONT,   //��ǰ
	CS_BACK,    //����
};
class CSoundObject
{
protected:
	LPDIRECT3DDEVICE9  m_device;
	LPDIRECT3DVERTEXBUFFER9 m_vb;
	LPDIRECT3DTEXTURE9 m_texture;  //������ͼ
	D3DXVECTOR3		   m_pos;
	int                m_id;   //��Ӧ��ID
	char               m_sndFile[64];  //��Ӧ�������ļ�
	bool               m_render;

public:
	CSoundObject();
	~CSoundObject();

protected: 
	virtual bool  CreateVB() = 0;  //��������

public:
	virtual void  ShutDown() = 0;   
	virtual void  Update() = 0;
	virtual void  Render() = 0;
	virtual bool  IsOn(D3DXVECTOR3 pos) = 0;  //�ж�λ���Ƿ����ڶ�������
	virtual float GetHeight(D3DXVECTOR3 pos) = 0; //����λ����Ϣ��������һ��ĸ߶ȣ��š����ӵȣ�
	virtual bool  LoadTexture(char* filename);
	virtual void  SetSoundFile(char* filename);
	virtual void  SetRender(bool render);
	virtual bool  GetRender();
	virtual void  SetPos(D3DXVECTOR3 pos);
	virtual void  SetPos(float x, float y, float z);
	virtual D3DXVECTOR3 GetPos();
	virtual void  SetId(int id);
	virtual int   GetId();
	virtual void  GetSndFile(char* file);
};