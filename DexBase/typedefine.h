
/*************************************************************
****数据结构在此定义
也定义了一些工程中常用的变量和功能函数
****作者：阚正杰(BruceKen)
****日期：2012-10-29
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
//单例模式基类
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
{//雾化混合因子
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
	//雾效参数
	DEMOEX_FOG_TYPE   m_fogType;  
	DEMOEX_FOG_MODE   m_fogMode;
	float             m_fogStart;
	float             m_fogEnd;
	float             m_fogdensity;	    //雾化浓度
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

//float DWORD 转换
inline unsigned long  FtoDW(float val) 
{
	return *((unsigned long*)&val);
}

//由于fabs函数限制,仅对double,float,long double有效	 
template<typename T>
bool   _equal(T f1,T f2,T pre = 1e-04f) { if(fabs((f1 - f2)) < pre) return true ; else return false;};




//引擎函数接口 具体数据由主程序传入
extern int GetClientWidth();
extern int GetClientHeight();

extern int GetWorldX(float posX);  //posX为屏幕坐标，转换为3D空间坐标（仅用于GUI转换，不用于3D空间内的鼠标拾取）
extern int GetWorldY(float posZ);
extern D3DXVECTOR2 GetWorldPos(const D3DXVECTOR2& pos);
void SplitStr(string str, char split_char, vector<string> &out);
bool   HandleCommandArgu(char* argus); //处理程序启动时的命令参数 具体如何处理由主程序实现

extern stVertex0          g_pVertexList0[4];//所有渲染系统共用的顶点与缓冲区 各个渲染系统在渲染前要根据自身需要进行数据重置
extern LPDIRECT3DVERTEXBUFFER9 g_pBuffer0;
extern stVertex1          g_pVertexList1[4];//所有渲染系统共用的顶点与缓冲区 各个渲染系统在渲染前要根据自身需要进行数据重置
extern LPDIRECT3DVERTEXBUFFER9 g_pBuffer1;
bool InitVertexBuff();    
bool IniEngineData();   //主程序 窗口设备初始化后 必须调用此函数 进行引擎一些数据的初始化
extern char* getDexBuffer(); //返回一个公用的大小为2048字节的缓冲区
extern bool  setDexBuffer(char* data,...); //设置缓冲区数据

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
//获得程序运行路径
void  _GetExeParth(char * path);
//================================================================
void _Message(char * message, ...);

//求两点距离
float _GetDistance(D3DXVECTOR3 point1, D3DXVECTOR3 point2);

//根据新旧两个朝向判断是顺时针还是逆时针选择
//返回true：顺时针 false:逆时针
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