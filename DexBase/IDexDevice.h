

#ifndef _I_DEX_DEVICE_H
#define _I_DEX_DEVICE_H
#include "CDexObject.h"
#include "DexColor.h"
#include "DexPrimitive.h"
#include "../DexMath/DexMatrix.h"
typedef enum _eDEXRENDERSTATE
 {
 	DEXRENDERSTATE_ZENABLE					,   //是否开启Z buff
 	DEXRENDERSTATE_ZWRITEENABLE              ,   //是否开启深度写入
 	DEXRENDERSTATE_ALPHATESTENABLE           ,   //是否开启alpha测试
 	DEXRENDERSTATE_SRCBLEND                  ,   
 	DEXRENDERSTATE_DESTBLEND                 ,   
 	DEXRENDERSTATE_CULLMODE                  ,   // CULL mode 
 	DEXRENDERSTATE_ALPHABLENDENABLE          ,   //是否开启alpha混合
 	DEXRENDERSTATE_FOGENABLE                 ,   //是否开启雾效
 	DEXRENDERSTATE_FOGCOLOR                  ,   // fog color 
 	DEXRENDERSTATE_FOGTABLEMODE              ,   // fog mode
 	DEXRENDERSTATE_FOGSTART                  ,   // fog start (for both vertex and pixel fog) 
 	DEXRENDERSTATE_FOGEND                    ,   // Fog end      
 	DEXRENDERSTATE_FOGDENSITY                ,   // Fog density  
 	DEXRENDERSTATE_RANGEFOGENABLE            ,   // Enables range-based fog
 	DEXRENDERSTATE_FOGVERTEXMODE             ,   // fog mode
 	
 	DEXRENDERSTATE_STENCILENABLE             ,   // BOOL enable/disable stenciling 
 	DEXRENDERSTATE_LIGHTING                  ,   // light
 	DEXRENDERSTATE_AMBIENT                   ,   //ambient color
 	DEXRENDERSTATE_POINTSIZE                 ,   /* float point size */
 	DEXRENDERSTATE_POINTSIZE_MIN             ,   /* float point size min threshold */
 	DEXRENDERSTATE_POINTSPRITEENABLE         ,   /* BOOL point texture coord control */
 	DEXRENDERSTATE_POINTSCALEENABLE          ,   /* BOOL point size scale enable */
 	DEXRENDERSTATE_POINTSCALE_A              ,   /* float point attenuation A value */
 	DEXRENDERSTATE_POINTSCALE_B              ,   /* float point attenuation B value */
 	DEXRENDERSTATE_POINTSCALE_C              ,   /* float point attenuation C value */
 	DEXRENDERSTATE_POINTSIZE_MAX             ,   /* float point size max threshold */
 	DEXRENDERSTATE_INDEXEDVERTEXBLENDENABLE  ,   //顶点颜色混合
 	DEXRENDERSTATE_BLENDOP                   ,   //BLENDOP setting
}eDEXRENDERSTATE;
 
 typedef enum _eDEXBLEND {
 	DEXBLEND_ZERO               = 1,
 	DEXBLEND_ONE                = 2,
 	DEXBLEND_SRCCOLOR           = 3,
 	DEXBLEND_INVSRCCOLOR        = 4,
 	DEXBLEND_SRCALPHA           = 5,
 	DEXBLEND_INVSRCALPHA        = 6,
 	DEXBLEND_DESTALPHA          = 7,
 	DEXBLEND_INVDESTALPHA       = 8,
 	DEXBLEND_DESTCOLOR          = 9,
 	DEXBLEND_INVDESTCOLOR       = 10,
 	DEXBLEND_SRCALPHASAT        = 11,
 	DEXBLEND_BOTHSRCALPHA       = 12,
 	DEXBLEND_BOTHINVSRCALPHA    = 13,
 	DEXBLEND_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
 } eDEXBLEND;
 typedef enum _eDEXTRANSFORMSTATETYPE {
	 DEXTRANSFORMSTATETYPE_VIEW,
	 DEXTRANSFORMSTATETYPE_PROJECTION,
	 DEXTRANSFORMSTATETYPE_WORLD,
	 DEXTRANSFORMSTATETYPE_WORLD1,
	 DEXTRANSFORMSTATETYPE_WORLD2,
	 DEXTRANSFORMSTATETYPE_WORLD3,
	 DEXTRANSFORMSTATETYPE_TEXTURE0,
	 DEXTRANSFORMSTATETYPE_TEXTURE1,
	 DEXTRANSFORMSTATETYPE_TEXTURE2,
	 DEXTRANSFORMSTATETYPE_TEXTURE3,
	 DEXTRANSFORMSTATETYPE_TEXTURE4,
	 DEXTRANSFORMSTATETYPE_TEXTURE5,
	 DEXTRANSFORMSTATETYPE_TEXTURE6,
	 DEXTRANSFORMSTATETYPE_TEXTURE7,
	 DEXTRANSFORMSTATETYPE_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
 } DEXTRANSFORMSTATETYPE;
 typedef enum _eVIEWMATRIXTYPE
 {
	 VIEWMATRIXTYPE_LEFTHAND,
	 VIEWMATRIXTYPE_RIGHTHAND
 }eVIEWMATRIXTYPE;
#define DEXCLEAR_TARGET            0x00000001l  /* Clear target surface */
#define DEXCLEAR_ZBUFFER           0x00000002l  /* Clear target z buffer */
#define DEXCLEAR_STENCIL           0x00000004l  /* Clear stencil planes */


#define DEXFVF_POSITION			   0x00000001
#define DEXFVF_NORMAL			   0x00000002
#define DEXFVF_TEXTURE0			   0x00000004
class IDexDevice:public CDexObject
{
	Dex_DeclareClass(IDexDevice, 0)
public:
	IDexDevice();
	virtual ~IDexDevice();
public:
	virtual bool InitDevice(bool bFullScreen, int16 iWindowWidth, int16 iWindowHeight, void* param1, void* param2) = 0;
	virtual bool SetTransform(DEXTRANSFORMSTATETYPE transformStateType, const DexMatrix4x4* matrix) = 0;
	virtual bool SetRenderState(eDEXRENDERSTATE eRenderState, int64 value) = 0;
	virtual bool SetFVF(int64 iFVF) = 0;
	virtual bool DrawPrimitiveUp(DexPrimitivetType eType, uint32 iPrimitiveCount, const void* pVertexStreamData, uint32 iVertexStreamStride) = 0;
	virtual bool Clear(int64 flags, const DexColor& color, float32 fZ, int64 sStencil) = 0;
	virtual bool BeginScene() = 0;
	virtual bool EndScene() = 0;
	//for固定管线，需要设置FVF
	//virtual bool DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 fvf, int32 stridesize) = 0;
	//for shader，不需要设置FVF
	//virtual bool DrawPrimitive(DexPrimitivetType type, const void* vertexs, int32 vertexCount, const void* indices, int32 primitiveCount, int32 stridesize) = 0;
public:
	void SetViewMatrixType(eVIEWMATRIXTYPE eType);
	void LookAt(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up);
protected:
	virtual void LookAtLH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up) = 0;
	virtual void LookAtRH(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up) = 0;
protected:
	eVIEWMATRIXTYPE	m_eViewMatrixType; 
};


#endif