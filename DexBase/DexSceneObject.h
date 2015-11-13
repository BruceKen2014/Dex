#ifndef _DEX_SCENE_OBJECT_H
#define _DEX_SCENE_OBJECT_H

#include <list>


#include "DexSceneObjectAction.h"
#include "../Geometry/CRay.h"


extern const D3DXVECTOR3 g_objIniDirection; //����object��Ĭ�J�������(0,0,-1)
class DexCollideObject;
class CDexScene;
class CDexSceneObject: public CDexObject
{
	Dex_DeclareClass(CDexSceneObject,0)
	friend class CDexScene;
public:
	typedef enum 
	{
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		AXIS_NONE
	}AXIS;
	//�����ڵ�
protected:
/*��춈����Ъ������ڲ������ڄe��object�����object���������Ķ�λ�����õ�ֻ��localmatrix�ccurrmatrix�������localmatrix��λ��ԭ�c�����N��K��position��ֻ��
currmatrix��Q�����������r�£�localmatrix����λ��ԭ�c����ͬ���ǿs�ź����D���ѡ�
*/
	D3DXMATRIX m_localMatrix;    //���ؾ���
	D3DXMATRIX m_currMatrix;     
	D3DXMATRIX m_finalMatrix;	 //�ڵ�����վ���

	D3DXMATRIX m_collideLocalMatrix;

	D3DXVECTOR3 m_currPos;
	D3DXVECTOR3 m_currScale;
//	union
	//{
		D3DXVECTOR3 m_currRotate;
	//	struct
	//	{
	//		float m_fRotateX;
	//		float m_fRotateY;
	//		float m_fRotateZ;
	//	};
	//};
	D3DXVECTOR3  m_direction;

	CDexScene*   m_pScene;       //��object������scene
	
	
	DexCollideObject* m_pCollideObject; //����ʱ���ⲿnew���� ���������ⲿ�Ͳ��ù�����node���й���ɾ��
	CDexSceneObject*   m_fatherNode;   //�ýڵ�ĸ��ڵ�
	typedef std::list<CDexSceneObject*>  TlistObjects;
	
	/*�ܴ�nodeӰ���scene Objects����Ҫ�ڼ���matrix�����ã��Ҿ���attackobject�뱾�����Ǹ���
	��ϵ������һ��������ֲ��Ĺ�ϵ�����ӿ������븸�׶����ڣ����ֲ�����������������ڣ��ñȸ�
	�׿����������й�(scene1),���ӿ�������������(scene2)����������ֲ��Ĺ�ϵ�������ڶ���������
	�Ĺ�ϵ���������ڶ����ϣ���ô�����й�scene1�������ͻ����й�scene1
	*/
	TlistObjects m_listAttachObjects;
	typedef std::list<DexSceneObjectAction*>  TlistObjectActions;
	TlistObjectActions m_listActions;
	bool		m_bRender;  //�Ƿ���Ⱦ
	bool        m_bCollide; //�Ƿ񅢼���ײ�z�y
    bool        m_bAlphaSort;  //�Ƿ���Ҫalpha����
	bool        m_bCullNone;      //�Ƿ���˫����Ⱦ(�������ӵȴ���alpha���ԵĶ�����Ҫ����)

//#ifdef _DEBUG
	bool m_bRenderNodeCube;
	bool m_bRenderCollideObject;
	bool m_bRenderRouteLine;
//#endif

public:
	CDexSceneObject();
	virtual ~CDexSceneObject();

protected:
	void _updateCurrMatrix();
public:
	virtual bool Update(int delta);
	virtual bool Render();
	virtual void Reset();
	void addAction(DexSceneObjectAction* action);
	bool getAlphaSort() const;
	void setAlphaSort(bool b);
	
	//�뿪��ǰ��scene ��ô���е�attachobject�����뿪��û�������˳����ˣ����ڶ����ϵĶ���ȴ���ڹ���
	void leaveScene();   
	void enterScene(CDexScene* pScene); //����pScene
	void bindSceneNode(CDexSceneObject* father); //���ýڵ�󶨵���һ���ڵ���
	void unBindSceneNode(); //�ĸ����c󠽉
	

	
	//���ñ��ؾ��󲢲���ı�catmull��·������Ϣ,����7�������object����ԭ����XOZƽ������ת����ʱ�ı�object localMatrixֻ��ı䱾������Ӱ��·����
	//���Ҫ�ı�·���㣬�������Զ�·������о�����������⣬���ڵ�ľ����Ӱ��·���㣬�縸�ڵ���ת�ˣ���·�����ı䣬����Ⱦ������ʱҪ���Ը��׾���
	void setLocalMatrix(const D3DXMATRIX& matrix);
	void setLocalMatrix(const D3DXVECTOR3& trans,const D3DXVECTOR3& scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), AXIS axis = AXIS_NONE, float rotate_value = 0.0f);
	void setCurrMatrix(const D3DXVECTOR3& trans,const D3DXVECTOR3& scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), AXIS axis = AXIS_NONE, float rotate_value = 0.0f);
	void setCollideObjectMatrix(const D3DXMATRIX& matrix);
	void setMatrix(const D3DXMATRIX& matrix) { m_finalMatrix = matrix;};
	void Move(const D3DXVECTOR3& dir, float length); //��dir �Ƅ�length
	void Move(const D3DXVECTOR3& delta);          //ƫ��deltaλ��
	void FaceTo(const D3DXVECTOR3& pos);			 //׌object�泯ĳ�c
	void setPosition(const D3DXVECTOR3& pos);  //setPosition ֻ��Ӱ�currMatrix
	void setPosition(AXIS axis,float value);  
	void scale(const D3DXVECTOR3& value);
	void scale(AXIS axis, float value);
	void rotate(AXIS axis, float degree);  //ra ���D�S  degree���D������λ�Ƕ�
	void rotateRadian(AXIS axis, float radian);	//ra ���D�S  degree���D������λ����
	void setDirection(const D3DXVECTOR3& dir);
	void SetRender(bool render)   { m_bRender = render;};
	bool getRender()              { return m_bRender;};
	void SetRenderNodeCube(bool render);
	void setRenderActionRouteLine(bool render);
	
	void setCollideObject(DexCollideObject* object);
	void setCullNone(bool b);
	void setCollide(bool b);
	bool getCollide();
	DexCollideObject* getCollideObject() ;
	bool checkCollide(CDexSceneObject* node) ;  //�������һ��node����ײ
	bool checkPick(const stRay& ray);    //Ӌ���Ƿ����ʰȡ��
	bool getVisible();
	D3DXVECTOR3 getDirection() { return m_direction;};
	D3DXVECTOR3 getPosition();
	const D3DXMATRIX*  getFinalMatrix() ;
	const D3DXMATRIX* getLocalMatrix() const;
	const D3DXMATRIX* getCurrMatrix() const;
	CDexSceneObject* getFatherNode() const;
	D3DXMATRIX  getSelfCurrMatrix() const;

public:
	virtual bool ArchiveIn(DexMem& mem);
	virtual bool ArchiveOut(DexMem& mem) const;
};

#endif