

#ifndef _CDEX_PARTICAL_H
#define _CDEX_PARTICAL_H


#include <list>
#include <map>
#include "typedefine.h"
#include "DexSceneObject.h"
//


//一个粒子
class CDexParticalEmit;
class CDexPartical
{
	friend class CDexParticalEmit;
protected:
	D3DXVECTOR3 m_pos;  //粒子位置
	D3DXVECTOR3 m_vel;	//粒子速度
	D3DXVECTOR3 m_acc;  //粒子加速度
	D3DXVECTOR3 m_rotateVel;   //旋转速度 度/s
	D3DXVECTOR3 m_currRotate;  //当前旋转
	D3DXVECTOR3 m_size; //尺寸
	float       m_fScale;  //当前缩放比
	float       m_fU1; 
	float       m_fV1;
	float       m_fU2;	
	float       m_fV2;
	DexColor    m_color;
	int64       m_iCurrLiveTime; //当前已经生存的时间
	CDexParticalEmit* m_pEmit; //粒子所在的发射器
public:
	CDexPartical();
	~CDexPartical();
public:	 
	void Update(int delta, const D3DXVECTOR3& cam_pos);
	void Render();
	D3DXMATRIX GetMatrix(const D3DXVECTOR3& cam_pos);
	bool LiveOver();
};

//粒子发射器 即粒子系统
enum EParticalFaceState
{
	EPFS_X,    //始终面朝X轴
	EPFS_Y,    //始终面朝Y轴
	EPFS_Z,    //始终面朝Z轴
	EPFS_CAM,  //始终面朝摄像机
	EPFS_CAM_Y,  //始终面朝摄像机 但是只旋转Y轴
	EPFS_V_V,  //始终和速度方向垂直 Vertical Velocity
	EPFS_F_V,  //下边 始终和速度方向一致 Face Velocity
	EPFS_F_V_L, //左边 始终和速度方向一致
	EPFS_FREE, //自由模式
	EPFS_NONE
};
////必须采用引用计数 一个纹理可以被多个系统共用
//struct stEffectTex: public CRefCount
//{
//	//文件名与对应的UV坐标
//	LPDIRECT3DTEXTURE9 m_tex;
//	string name;
//	float  u1;
//	float  v1;
//	float  u2;
//	float  v2;
//	stEffectTex()
//	{
//		u1 = 0.0f;
//		v1 = 0.0f;
//		u2 = 1.0f;
//		v2 = 1.0f;
//	}
//	virtual void ShutDown() { _SafeRelease(m_tex);};
//};

enum EEmitType
{//粒子发射器发射粒子的模式
	EET_NORMAL, //正常模式
	EET_EXPLODE, //爆炸模式(对于关键帧有效)		
	EET_FRAME_NORMAL, //在关键帧模式中 某些帧数是NORMAL状态
	EET_ALL
}; 

//关键帧
struct stFrame
{
	stFrame():frame(0),param1(0),param2(0) {}
	int16 frame;    //有效帧 对于explode模式来说 是要爆炸的帧
	                //对于frame_normal来说是 有效的开始帧
	int16 param1;   //对于explode来说 是该帧要爆炸的粒子数量
					//对于frame_normal来说 是有效的结束帧
	int16 param2; 
};
class CDexTex;	
//一个粒子发射器
//TODO 将发射器数据achive进文件 同时可以achive加载该格式文件
//lua提供创建特效实例函数，并支持对指定ID实例的特效进行添加粒子发射器的添加
class CDexParticalEmit: public CDexSceneObject
{
	Dex_DeclareClass(CDexParticalEmit,4)
	friend class CDexPartical;
public:
	CDexParticalEmit();
	virtual ~CDexParticalEmit();

public:
	EEmitType          m_EmitType;
	int16              m_iCurFrame;  //当前是第几帧 (如果是爆炸模式的话 在update之前要设置帧数)
	typedef map<int16, stFrame>  TMapFrame;
	TMapFrame        m_keyFrame; //粒子关键帧及该帧对应的数据   
	std::list<CDexPartical*>  m_particalList;	  //生存粒子链表
	CDexTex*                  m_pDexTex;
	int8                      m_iTexFrameX; //纹理横向有几帧资源 
	int8                      m_iTexFrameTotal; //纹理一共有几帧资源(不用纵向帧数的原因是 用这个变量的话 图片最后一行可以不是满的) 
	EParticalFaceState m_eParticalFaceState;  //粒子的朝向状态
	//D3DXVECTOR3 m_EmitPos;        //发射器的位置 这个变量已经不再起作用 若想要改变发射器位置 请直接设置object的 localmatrix
	//发射器的朝向
	//暂时的做法是：如果一个粒子要跟随发射器的旋转角度 那么初始的速度方向必须是(0,0,-1) 加速度方向也必须是(0,0,-1)
	//D3DXVECTOR3 m_EmitFaceVec;     //此变量已经无用 发射器本没有朝向的概念 只有发射粒子有速度方向 要改变请直接设置localmatrix
	D3DXVECTOR3 m_createMaxSize;  //最大尺寸
	D3DXVECTOR3 m_createMinSize;  //最小尺寸
	D3DXVECTOR3 m_createPosMin;   //粒子生成最小偏移量
	D3DXVECTOR3 m_createPosMax;   //粒子生成最大偏移量
	D3DXVECTOR3 m_rotateInit;     //粒子的初始旋转参数 以(0,0,-1)为参照
	D3DXVECTOR3 m_rotateVelMax;      //最大旋转速度 度/s
	D3DXVECTOR3 m_rotateVelMin;      //最小旋转速度 度/s
	int64       m_iParticalLiveTime; //粒子生存时间
	int64       m_iCreateLastParticalTime; //生成上一个粒子的时间
	int32       m_createInterval; //生成粒子间隔 ms
	D3DXVECTOR3 m_velMin;          //粒子生成最小速度
	D3DXVECTOR3 m_velMax;         //粒子生成最大速度
	D3DXVECTOR3 m_acc;            //粒子加速度
	float       m_fScaleStart;    //起始缩放比
	float       m_fScaleMiddle;	  //中间缩放比
	float       m_fScaleEnd;      //结束缩放比	 
	float       m_fMiddleTimeRate;  //中间生命比率(0.5)则为生命值的一半
	//bool        m_bFolloEmitRotate; //粒子是否跟随发射器旋转
	bool        m_SeeTexState;      //查看未add的纹理状态
	DexColor   m_startColor;
	DexColor   m_middleColor;
	DexColor   m_endColor;
	

protected:
	void _UpdateNormal(int delta, const D3DXVECTOR3& cam_pos);
	void _UpdateExplode(int delta, const D3DXVECTOR3& cam_pos);
	void _UpdateFrameNormal(int delta, const D3DXVECTOR3& cam_pos);
	void _InitParticalData(CDexPartical* partical);	 //normal和explode等只影响生成粒子的时机 不影响数据 故数据赋值提出来
public:
	void SetCurrFrame(int16 frame_index);
	void SetTexFile(string filename);
	bool AddKeyFrame(int16 frame_index, const stFrame data); //添加关键帧
	bool AddKeyFrame(int16 frame_index, int16 count);
	bool AddKeyFrame(const stFrame& data);
	bool AddKeyFrameDur(int16 frame_start, int16 frame_end);
	void PreInitMemRes();  //为系统预分配粒子内存 一边渲染一边申请 严重影响开始的帧数
	bool Update(int delta);
	bool Render();
	void Reset();

	void SetPos(const D3DXVECTOR3& pos);
	bool GetSeeTexState() { return m_SeeTexState;};
	void SetSeeTexState(bool see) { m_SeeTexState = see;};
	void SetParticalFace(EParticalFaceState state) { m_eParticalFaceState = state;};
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
public:
	bool Archive(DexMem& mem, bool mode);

public:
	bool saveToInitFile(const char* filename);
	bool initFromInitFile(const char* filename);
};


//特效监视器
//根据具体需求进行继承实现接口  如怪物被玩家释放一个技能 在技能结束后 在end里面进行一些特殊操作
class CDexEffectInstance;
class CDexEffectNotify
{
public:
	CDexEffectNotify(){};
	virtual ~CDexEffectNotify(){};

public:
	virtual void OnEffectBegin(CDexEffectInstance* effect) = 0;
	virtual void OnEffectEnd(CDexEffectInstance* effect) = 0;
};
class CDexEffectInstance: public CDexSceneObject
{//特效实例
	Dex_DeclareClass(CDexEffectInstance,0)
protected:
	CDexEffectNotify*  m_pEffectBeginNotify; //由外部传入
protected:
	bool m_bCycle; //特效是否反复播放
	bool m_bEnd;   //如果特效不是反复播的 判断是否播完
public:
	CDexEffectInstance();
	virtual ~CDexEffectInstance();

public:
	void RegisterNotify(CDexEffectNotify* notify);

public:
	virtual bool Update(int delta);
	virtual bool Render();
	virtual bool LoadInitFile(const char* filename) { return true;};
	virtual void StartFromBegin() {};
	void setCycle(bool cycle);
	bool getCycle() const;
	bool getEnd() const;
};

//粒子特效实例：其中包括了多个粒子特效
//常用于有先后关系的技能特效
class CDexEffectEmitInstance: public CDexEffectInstance 
{
	Dex_DeclareClass(CDexEffectEmitInstance,4)
protected:				  
	int16  m_iPerFrameTime; //每一帧时间 单位:ms 如 1000/60 = 16ms 则一秒有60帧  
	int16  m_iTotalFrames;	//特效的总帧数
	int64  m_iTotalTime;    //特效总共过去的时间
	typedef std::list<CDexParticalEmit*> TListEmitter;
	TListEmitter m_EmiterList;

public:
	CDexEffectEmitInstance();
	virtual ~CDexEffectEmitInstance();
public:
	void AddEmitter(CDexParticalEmit* emitter);
	void SetTotalFrames(int16 frames)  { m_iTotalFrames = frames;};
	virtual void StartFromBegin() ;
	bool Update(int delta);
	bool Render();
	virtual void Reset();
public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
public:
	bool Archive(DexMem& mem, bool mode);

public:
	virtual bool LoadInitFile(const char* filename);

	//临时函数 为了测试用
	void bindSceneNode(CDexSceneObject* node)
	{
		foreach(TListEmitter, ite, m_EmiterList)
		{
			(*ite)->bindSceneNode(node);
		}
	}
};

//粒子池 负责所有粒子系统的内存管理
class CDexClassParticalPool
{
private:
	std::list<CDexPartical*>  m_particalListFree;  //空闲的粒子链表
public:
/*
0   1
2   3
*/
	stVertex1          m_pParticalList[4];		 //所有粒子系统共用的顶点与缓冲区
	LPDIRECT3DVERTEXBUFFER9 m_pBuffer;
public:
	CDexClassParticalPool();
	~CDexClassParticalPool();
public:
	void CreatePartical(int iCount);  //创建对应数量的粒子 当一个系统创建的时候 为该系统申请内存
	void FreePartical(int iCount);    //释放对应数量的粒子 当一个粒子系统消亡的时候 为该系统释放内存
	CDexPartical* GetPartical();      //外部粒子系统向内存池申请一颗粒子
	void PushPartical(CDexPartical* pPartical); //外部粒子系统将本系统生命结束的粒子放回粒子池中
};

extern CDexClassParticalPool* getParticalPool();


class CDexPieceEffectInstance: public CDexEffectInstance
{
	Dex_DeclareClass(CDexPieceEffectInstance,4)
public:
	typedef struct _stPiece 
	{
		D3DXVECTOR3 point[4];
		DexColor    m_color;
		CDexPieceEffectInstance* instance;
		float alreadyTime;
		_stPiece();
		void Update(int delta);
	}stPiece;
	typedef struct _stPieceControlPoint
	{
		D3DXVECTOR3 pnt1;
		D3DXVECTOR3 pnt2;
		_stPieceControlPoint();
	}stPieceControlPoint;
protected:
	typedef std::list<stPiece*> TListPiece;
	TListPiece m_listPiece;
	CDexTex*        m_pDexTex;
	//暂时用catmull曲线来控制面片轨迹
	std::vector<_stPieceControlPoint> m_vecControlPoints;
	float m_fFactor;
	float m_fSpeed;
	float m_fCurrPosRate;
	float m_fLastPosRate;
public:
	float m_fMiddleTimeRate;  //中间生命比率(0.5)则为生命值的一半
	int16 m_iPieceLiveTime;  //面片生存时间 单位ms
	DexColor m_startColor;
	DexColor m_middleColor;
	DexColor m_endColor;
	bool  m_bRenderPieceRoute;
public:
	CDexPieceEffectInstance();
	virtual ~CDexPieceEffectInstance();

public:
	virtual void Reset();
	virtual bool Render();
	virtual bool Update(int delta);
public:
	virtual bool LoadInitFile(const char* filename);
	void PreInitMemRes();
	void setRenderPieceRoute(bool b);
	void addControlPoint(const D3DXVECTOR3& pnt1, const D3DXVECTOR3& pnt2);
	void setFactor(float factor);
	void setSpeed(float speed);
	bool setTexture(string filename);
	void setPieceLiveTime(int16 time);
};

//面片池 负责所有面片系统的内存管理
class CDexPiecePool
{
private:
	typedef std::list<CDexPieceEffectInstance::stPiece *> TlistPiece;
	TlistPiece  m_listFreePiece;  //空闲的面片链表
public:
	CDexPiecePool();
	virtual ~CDexPiecePool();
public:
	void CreatePiece(int iCount);  //创建对应数量的粒子 当一个系统创建的时候 为该系统申请内存
	void FreePiece(int iCount);    //释放对应数量的粒子 当一个粒子系统消亡的时候 为该系统释放内存
	CDexPieceEffectInstance::stPiece * GetPiece();      //外部粒子系统向内存池申请一颗粒子
	void Push(CDexPieceEffectInstance::stPiece * pPartical); //外部粒子系统将本系统生命结束的粒子放回粒子池中
};

extern CDexPiecePool* getPiecePool();




#endif