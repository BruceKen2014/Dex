

#ifndef _CDEX_PARTICAL_H
#define _CDEX_PARTICAL_H


#include <list>
#include <map>
#include "typedefine.h"
#include "DexSceneObject.h"
//


//һ������
class CDexParticalEmit;
class CDexPartical
{
	friend class CDexParticalEmit;
protected:
	D3DXVECTOR3 m_pos;  //����λ��
	D3DXVECTOR3 m_vel;	//�����ٶ�
	D3DXVECTOR3 m_acc;  //���Ӽ��ٶ�
	D3DXVECTOR3 m_rotateVel;   //��ת�ٶ� ��/s
	D3DXVECTOR3 m_currRotate;  //��ǰ��ת
	D3DXVECTOR3 m_size; //�ߴ�
	float       m_fScale;  //��ǰ���ű�
	float       m_fU1; 
	float       m_fV1;
	float       m_fU2;	
	float       m_fV2;
	DexColor    m_color;
	int64       m_iCurrLiveTime; //��ǰ�Ѿ������ʱ��
	CDexParticalEmit* m_pEmit; //�������ڵķ�����
public:
	CDexPartical();
	~CDexPartical();
public:	 
	void Update(int delta, const D3DXVECTOR3& cam_pos);
	void Render();
	D3DXMATRIX GetMatrix(const D3DXVECTOR3& cam_pos);
	bool LiveOver();
};

//���ӷ����� ������ϵͳ
enum EParticalFaceState
{
	EPFS_X,    //ʼ���泯X��
	EPFS_Y,    //ʼ���泯Y��
	EPFS_Z,    //ʼ���泯Z��
	EPFS_CAM,  //ʼ���泯�����
	EPFS_CAM_Y,  //ʼ���泯����� ����ֻ��תY��
	EPFS_V_V,  //ʼ�պ��ٶȷ���ֱ Vertical Velocity
	EPFS_F_V,  //�±� ʼ�պ��ٶȷ���һ�� Face Velocity
	EPFS_F_V_L, //��� ʼ�պ��ٶȷ���һ��
	EPFS_FREE, //����ģʽ
	EPFS_NONE
};
////����������ü��� һ��������Ա����ϵͳ����
//struct stEffectTex: public CRefCount
//{
//	//�ļ������Ӧ��UV����
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
{//���ӷ������������ӵ�ģʽ
	EET_NORMAL, //����ģʽ
	EET_EXPLODE, //��ըģʽ(���ڹؼ�֡��Ч)		
	EET_FRAME_NORMAL, //�ڹؼ�֡ģʽ�� ĳЩ֡����NORMAL״̬
	EET_ALL
}; 

//�ؼ�֡
struct stFrame
{
	stFrame():frame(0),param1(0),param2(0) {}
	int16 frame;    //��Ч֡ ����explodeģʽ��˵ ��Ҫ��ը��֡
	                //����frame_normal��˵�� ��Ч�Ŀ�ʼ֡
	int16 param1;   //����explode��˵ �Ǹ�֡Ҫ��ը����������
					//����frame_normal��˵ ����Ч�Ľ���֡
	int16 param2; 
};
class CDexTex;	
//һ�����ӷ�����
//TODO ������������achive���ļ� ͬʱ����achive���ظø�ʽ�ļ�
//lua�ṩ������Чʵ����������֧�ֶ�ָ��IDʵ������Ч����������ӷ����������
class CDexParticalEmit: public CDexSceneObject
{
	Dex_DeclareClass(CDexParticalEmit,4)
	friend class CDexPartical;
public:
	CDexParticalEmit();
	virtual ~CDexParticalEmit();

public:
	EEmitType          m_EmitType;
	int16              m_iCurFrame;  //��ǰ�ǵڼ�֡ (����Ǳ�ըģʽ�Ļ� ��update֮ǰҪ����֡��)
	typedef map<int16, stFrame>  TMapFrame;
	TMapFrame        m_keyFrame; //���ӹؼ�֡����֡��Ӧ������   
	std::list<CDexPartical*>  m_particalList;	  //������������
	CDexTex*                  m_pDexTex;
	int8                      m_iTexFrameX; //��������м�֡��Դ 
	int8                      m_iTexFrameTotal; //����һ���м�֡��Դ(��������֡����ԭ���� ����������Ļ� ͼƬ���һ�п��Բ�������) 
	EParticalFaceState m_eParticalFaceState;  //���ӵĳ���״̬
	//D3DXVECTOR3 m_EmitPos;        //��������λ�� ��������Ѿ����������� ����Ҫ�ı䷢����λ�� ��ֱ������object�� localmatrix
	//�������ĳ���
	//��ʱ�������ǣ����һ������Ҫ���淢��������ת�Ƕ� ��ô��ʼ���ٶȷ��������(0,0,-1) ���ٶȷ���Ҳ������(0,0,-1)
	//D3DXVECTOR3 m_EmitFaceVec;     //�˱����Ѿ����� ��������û�г���ĸ��� ֻ�з����������ٶȷ��� Ҫ�ı���ֱ������localmatrix
	D3DXVECTOR3 m_createMaxSize;  //���ߴ�
	D3DXVECTOR3 m_createMinSize;  //��С�ߴ�
	D3DXVECTOR3 m_createPosMin;   //����������Сƫ����
	D3DXVECTOR3 m_createPosMax;   //�����������ƫ����
	D3DXVECTOR3 m_rotateInit;     //���ӵĳ�ʼ��ת���� ��(0,0,-1)Ϊ����
	D3DXVECTOR3 m_rotateVelMax;      //�����ת�ٶ� ��/s
	D3DXVECTOR3 m_rotateVelMin;      //��С��ת�ٶ� ��/s
	int64       m_iParticalLiveTime; //��������ʱ��
	int64       m_iCreateLastParticalTime; //������һ�����ӵ�ʱ��
	int32       m_createInterval; //�������Ӽ�� ms
	D3DXVECTOR3 m_velMin;          //����������С�ٶ�
	D3DXVECTOR3 m_velMax;         //������������ٶ�
	D3DXVECTOR3 m_acc;            //���Ӽ��ٶ�
	float       m_fScaleStart;    //��ʼ���ű�
	float       m_fScaleMiddle;	  //�м����ű�
	float       m_fScaleEnd;      //�������ű�	 
	float       m_fMiddleTimeRate;  //�м���������(0.5)��Ϊ����ֵ��һ��
	//bool        m_bFolloEmitRotate; //�����Ƿ���淢������ת
	bool        m_SeeTexState;      //�鿴δadd������״̬
	DexColor   m_startColor;
	DexColor   m_middleColor;
	DexColor   m_endColor;
	

protected:
	void _UpdateNormal(int delta, const D3DXVECTOR3& cam_pos);
	void _UpdateExplode(int delta, const D3DXVECTOR3& cam_pos);
	void _UpdateFrameNormal(int delta, const D3DXVECTOR3& cam_pos);
	void _InitParticalData(CDexPartical* partical);	 //normal��explode��ֻӰ���������ӵ�ʱ�� ��Ӱ������ �����ݸ�ֵ�����
public:
	void SetCurrFrame(int16 frame_index);
	void SetTexFile(string filename);
	bool AddKeyFrame(int16 frame_index, const stFrame data); //��ӹؼ�֡
	bool AddKeyFrame(int16 frame_index, int16 count);
	bool AddKeyFrame(const stFrame& data);
	bool AddKeyFrameDur(int16 frame_start, int16 frame_end);
	void PreInitMemRes();  //ΪϵͳԤ���������ڴ� һ����Ⱦһ������ ����Ӱ�쿪ʼ��֡��
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


//��Ч������
//���ݾ���������м̳�ʵ�ֽӿ�  ����ﱻ����ͷ�һ������ �ڼ��ܽ����� ��end�������һЩ�������
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
{//��Чʵ��
	Dex_DeclareClass(CDexEffectInstance,0)
protected:
	CDexEffectNotify*  m_pEffectBeginNotify; //���ⲿ����
protected:
	bool m_bCycle; //��Ч�Ƿ񷴸�����
	bool m_bEnd;   //�����Ч���Ƿ������� �ж��Ƿ���
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

//������Чʵ�������а����˶��������Ч
//���������Ⱥ��ϵ�ļ�����Ч
class CDexEffectEmitInstance: public CDexEffectInstance 
{
	Dex_DeclareClass(CDexEffectEmitInstance,4)
protected:				  
	int16  m_iPerFrameTime; //ÿһ֡ʱ�� ��λ:ms �� 1000/60 = 16ms ��һ����60֡  
	int16  m_iTotalFrames;	//��Ч����֡��
	int64  m_iTotalTime;    //��Ч�ܹ���ȥ��ʱ��
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

	//��ʱ���� Ϊ�˲�����
	void bindSceneNode(CDexSceneObject* node)
	{
		foreach(TListEmitter, ite, m_EmiterList)
		{
			(*ite)->bindSceneNode(node);
		}
	}
};

//���ӳ� ������������ϵͳ���ڴ����
class CDexClassParticalPool
{
private:
	std::list<CDexPartical*>  m_particalListFree;  //���е���������
public:
/*
0   1
2   3
*/
	stVertex1          m_pParticalList[4];		 //��������ϵͳ���õĶ����뻺����
	LPDIRECT3DVERTEXBUFFER9 m_pBuffer;
public:
	CDexClassParticalPool();
	~CDexClassParticalPool();
public:
	void CreatePartical(int iCount);  //������Ӧ���������� ��һ��ϵͳ������ʱ�� Ϊ��ϵͳ�����ڴ�
	void FreePartical(int iCount);    //�ͷŶ�Ӧ���������� ��һ������ϵͳ������ʱ�� Ϊ��ϵͳ�ͷ��ڴ�
	CDexPartical* GetPartical();      //�ⲿ����ϵͳ���ڴ������һ������
	void PushPartical(CDexPartical* pPartical); //�ⲿ����ϵͳ����ϵͳ�������������ӷŻ����ӳ���
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
	//��ʱ��catmull������������Ƭ�켣
	std::vector<_stPieceControlPoint> m_vecControlPoints;
	float m_fFactor;
	float m_fSpeed;
	float m_fCurrPosRate;
	float m_fLastPosRate;
public:
	float m_fMiddleTimeRate;  //�м���������(0.5)��Ϊ����ֵ��һ��
	int16 m_iPieceLiveTime;  //��Ƭ����ʱ�� ��λms
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

//��Ƭ�� ����������Ƭϵͳ���ڴ����
class CDexPiecePool
{
private:
	typedef std::list<CDexPieceEffectInstance::stPiece *> TlistPiece;
	TlistPiece  m_listFreePiece;  //���е���Ƭ����
public:
	CDexPiecePool();
	virtual ~CDexPiecePool();
public:
	void CreatePiece(int iCount);  //������Ӧ���������� ��һ��ϵͳ������ʱ�� Ϊ��ϵͳ�����ڴ�
	void FreePiece(int iCount);    //�ͷŶ�Ӧ���������� ��һ������ϵͳ������ʱ�� Ϊ��ϵͳ�ͷ��ڴ�
	CDexPieceEffectInstance::stPiece * GetPiece();      //�ⲿ����ϵͳ���ڴ������һ������
	void Push(CDexPieceEffectInstance::stPiece * pPartical); //�ⲿ����ϵͳ����ϵͳ�������������ӷŻ����ӳ���
};

extern CDexPiecePool* getPiecePool();




#endif