#ifndef _DEX_SCENE_OBJECT_H
#define _DEX_SCENE_OBJECT_H

#include <list>


#include "DexSceneObjectAction.h"
#include "../Geometry/CRay.h"


extern const D3DXVECTOR3 g_objIniDirection; //所有object的默認方向均為(0,0,-1)
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
	//场景节点
protected:
/*對於場景中獨立存在不綁定在別的object上面的object，對於自身的定位起作用的只有localmatrix與currmatrix，而如果localmatrix定位在原點，那麼最終的position就只由
currmatrix來決定。大多數情況下，localmatrix都定位在原點，不同的是縮放和旋轉而已。
*/
	D3DXMATRIX m_localMatrix;    //本地矩阵
	D3DXMATRIX m_currMatrix;     
	D3DXMATRIX m_finalMatrix;	 //节点的最终矩阵

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

	CDexScene*   m_pScene;       //该object所属的scene
	
	
	DexCollideObject* m_pCollideObject; //测试时由外部new传入 传进来后外部就不用管了由node自行管理删除
	CDexSceneObject*   m_fatherNode;   //该节点的父节点
	typedef std::list<CDexSceneObject*>  TlistObjects;
	
	/*受此node影响的scene Objects，主要在计算matrix起作用，我觉得attackobject与本身并不是父子
	关系，而是一种整体与局部的关系，儿子可以脱离父亲而存在，但局部不能脱离整体而存在，好比父
	亲可以生活在中国(scene1),儿子可以生活在美国(scene2)，但整体与局部的关系比如人于耳环和身体
	的关系，耳环戴在耳朵上，那么人在中国scene1，耳环就会在中国scene1
	*/
	TlistObjects m_listAttachObjects;
	typedef std::list<DexSceneObjectAction*>  TlistObjectActions;
	TlistObjectActions m_listActions;
	bool		m_bRender;  //是否渲染
	bool        m_bCollide; //是否參加碰撞檢測
    bool        m_bAlphaSort;  //是否需要alpha排序
	bool        m_bCullNone;      //是否开启双面渲染(对于竹子等带有alpha属性的对象需要开启)

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
	
	//离开当前的scene 那么所有的attachobject都会离开，没有理由人出国了，戴在耳朵上的耳环却还在国内
	void leaveScene();   
	void enterScene(CDexScene* pScene); //进入pScene
	void bindSceneNode(CDexSceneObject* father); //将该节点绑定到另一个节点上
	void unBindSceneNode(); //從父節點鬆綁
	

	
	//设置本地矩阵并不会改变catmull等路径点信息,如有7个点控制object绕着原点在XOZ平面内旋转，这时改变object localMatrix只会改变本身，并不影响路径点
	//如果要改变路径点，必须亲自对路径点进行矩阵操作，另外，父节点的矩阵会影响路径点，如父节点旋转了，则路径点会改变，在渲染帮助线时要乘以父亲矩阵
	void setLocalMatrix(const D3DXMATRIX& matrix);
	void setLocalMatrix(const D3DXVECTOR3& trans,const D3DXVECTOR3& scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), AXIS axis = AXIS_NONE, float rotate_value = 0.0f);
	void setCurrMatrix(const D3DXVECTOR3& trans,const D3DXVECTOR3& scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), AXIS axis = AXIS_NONE, float rotate_value = 0.0f);
	void setCollideObjectMatrix(const D3DXMATRIX& matrix);
	void setMatrix(const D3DXMATRIX& matrix) { m_finalMatrix = matrix;};
	void Move(const D3DXVECTOR3& dir, float length); //朝dir 移動length
	void Move(const D3DXVECTOR3& delta);          //偏移delta位置
	void FaceTo(const D3DXVECTOR3& pos);			 //讓object面朝某點
	void setPosition(const D3DXVECTOR3& pos);  //setPosition 只會影響currMatrix
	void setPosition(AXIS axis,float value);  
	void scale(const D3DXVECTOR3& value);
	void scale(AXIS axis, float value);
	void rotate(AXIS axis, float degree);  //ra 旋轉軸  degree旋轉量：單位角度
	void rotateRadian(AXIS axis, float radian);	//ra 旋轉軸  degree旋轉量：單位弧度
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
	bool checkCollide(CDexSceneObject* node) ;  //计算和另一个node的碰撞
	bool checkPick(const stRay& ray);    //計算是否被鼠標拾取到
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