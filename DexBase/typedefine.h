
/*************************************************************
****���ݽṹ�ڴ˶���
Ҳ������һЩ�����г��õı����͹��ܺ���
****���ߣ�������(BruceKen)
****���ڣ�2012-10-29
*************************************************************/
#ifndef _typedefine_H
#define _typedefine_H

#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <vector>
#include <math.h>
#include <time.h>

extern "C"
{  
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
} 
/*
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib ,"lua.lib")
*/


#include "DexType.h"
#include "DexColor.h"
#include "DexVertex.h"
#include "DexDefine.h"


using namespace std;
//����ģʽ����
template <typename T>
class CSingle
{
private:
	CSingle(){};
public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	}
};

enum DEMO_SAMPLE_TYPE
{
	DEMOEX_SAMPLE_NONE,
	DEMOEX_SAMPLE_2,
	DEMOEX_SAMPLE_4,
	DEMOEX_SAMPLE_8,
	DEMOEX_SAMPLE_16,
};

enum DEMOEX_FOG_TYPE
{
	DEMOEX_VERTEX_FOG,
	DEMOEX_PIXEL_FOG,
};

typedef enum 
{//���������
	DEMOEX_FOG_LINER,
	DEMOEX_FOG_EXP,
	DEMOEX_FOG_EXP2,
}DEMOEX_FOG_MODE;
enum DEMOEX_SAM_STATE
{
	SAM_POINT,
	SAM_LINER,
	SAM_ANISOTROPY,
};
typedef struct _stFog
{
	//��Ч����
	DEMOEX_FOG_TYPE   m_fogType;  
	DEMOEX_FOG_MODE   m_fogMode;
	float             m_fogStart;
	float             m_fogEnd;
	float             m_fogdensity;	    //��Ũ��
	DexColor          m_fogColor; 
	bool              m_rangerFog;
	_stFog()
	{
		m_fogType   = DEMOEX_VERTEX_FOG; 
		m_fogMode   = DEMOEX_FOG_LINER;
		m_fogStart  = 1.0f;
		m_fogEnd    = 1000.0f;
		m_fogdensity= 0.005f;
		m_fogColor  = DexColor(1.0f, 1.0f, 1.0f);
		m_rangerFog = false;	
	}
} stFog;


#ifndef	 DEXIMAGE_FORMAT
typedef	 D3DXIMAGE_FILEFORMAT  DEXIMAGE_FORMAT;
#endif

//float DWORD ת��
inline unsigned long  FtoDW(float val) 
{
	return *((unsigned long*)&val);
}

//����fabs��������,����double,float,long double��Ч	 
template<typename T>
bool   _equal(T f1,T f2,T pre = 1e-04f) { if(fabs((f1 - f2)) < pre) return true ; else return false;};




//���溯���ӿ� ������������������
extern int GetClientWidth();
extern int GetClientHeight();

extern int GetWorldX(float posX);  //posXΪ��Ļ���꣬ת��Ϊ3D�ռ����꣨������GUIת����������3D�ռ��ڵ����ʰȡ��
extern int GetWorldY(float posZ);
extern D3DXVECTOR2 GetWorldPos(const D3DXVECTOR2& pos);
void SplitStr(string str, char split_char, vector<string> &out);
bool   HandleCommandArgu(char* argus); //�����������ʱ��������� ������δ�����������ʵ��

extern stVertex0          g_pVertexList0[4];//������Ⱦϵͳ���õĶ����뻺���� ������Ⱦϵͳ����ȾǰҪ����������Ҫ������������
extern LPDIRECT3DVERTEXBUFFER9 g_pBuffer0;
extern stVertex1          g_pVertexList1[4];//������Ⱦϵͳ���õĶ����뻺���� ������Ⱦϵͳ����ȾǰҪ����������Ҫ������������
extern LPDIRECT3DVERTEXBUFFER9 g_pBuffer1;
bool InitVertexBuff();    
bool IniEngineData();   //������ �����豸��ʼ���� ������ô˺��� ��������һЩ���ݵĳ�ʼ��
extern char* getDexBuffer(); //����һ�����õĴ�СΪ2048�ֽڵĻ�����
extern bool  setDexBuffer(char* data,...); //���û���������

extern D3DXMATRIX g_scaleMatrix;
extern D3DXMATRIX g_transMatrix;
extern D3DXMATRIX g_rotateMatrix;
extern D3DXMATRIX g_matrixTemp;
extern D3DXMATRIX g_matrix;

struct stTexture
{
	LPDIRECT3DTEXTURE9 m_pTexture;
	int m_iWidth;
	int m_iHeight;

	stTexture();
	stTexture(LPDIRECT3DDEVICE9 pD3DDevice, const char* filename);
	~stTexture();
};

char *del(char *p,char *q);

HRESULT FindMediaFile( TCHAR* strPath, TCHAR* strFilename ); 
//��ó�������·��
void  _GetExeParth(char * path);
//================================================================
void _Message(char * message, ...);

//���������
float _GetDistance(D3DXVECTOR3 point1, D3DXVECTOR3 point2);

//�����¾����������ж���˳ʱ�뻹����ʱ��ѡ��
//����true��˳ʱ�� false:��ʱ��
bool  DecideISClock(D3DXVECTOR3 old, D3DXVECTOR3 _new); 

#ifndef TINT
typedef  int           TINT;
#else
typedef  int           TINT;  
#endif

#ifndef TSHORT
typedef  short	       TSHORT;
#endif

#ifndef UCHAR 
typedef  unsigned char UCHAR;
#endif

#ifndef ULONG 
typedef  unsigned long ULONG;
#endif

//#ifndef BOOL 
//typedef  bool  BOOL;
//#endif

#endif