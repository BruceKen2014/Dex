/*
作者：阚正杰
2012.12
  能发出根据玩家所走的不同物体，发出不同声音的对象，
  如草地、石路、桥等
*/

#pragma  once
#include "../DexBase/typedefine.h"


enum
{
	CS_LEFT,    //朝左
	CS_RIGHT,   //朝右
	CS_FRONT,   //朝前
	CS_BACK,    //朝后
};
class CSoundObject
{
protected:
	LPDIRECT3DDEVICE9  m_device;
	LPDIRECT3DVERTEXBUFFER9 m_vb;
	LPDIRECT3DTEXTURE9 m_texture;  //纹理贴图
	D3DXVECTOR3		   m_pos;
	int                m_id;   //对应的ID
	char               m_sndFile[64];  //对应的声音文件
	bool               m_render;

public:
	CSoundObject();
	~CSoundObject();

protected: 
	virtual bool  CreateVB() = 0;  //创建顶点

public:
	virtual void  ShutDown() = 0;   
	virtual void  Update() = 0;
	virtual void  Render() = 0;
	virtual bool  IsOn(D3DXVECTOR3 pos) = 0;  //判断位置是否落在对象上面
	virtual float GetHeight(D3DXVECTOR3 pos) = 0; //传入位置信息，传出这一点的高度（桥、梯子等）
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