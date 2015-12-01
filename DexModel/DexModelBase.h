/*
*DexEngine 模型处理基类
*create by Bruce
*Date:2015.11.7
*/

#ifndef _DEX_MODEL_BASE_H_
#define _DEX_MODEL_BASE_H_

#include "../DexBase/CDexObject.h"
class CDexTex;
class DexModelBase:public CDexObject
{
	Dex_DeclareClass(DexModelBase, 0)
public:
	DexModelBase();
	virtual ~DexModelBase();
protected:
	/*
	是否受外部光照影响,
	如果是false,无论外部是否打开光照，内部渲染都会将光照关闭，然后显示默认的材质与贴图
	如果是true,如果引擎也打开灯光的话，渲染时就将光照打开，这时如果外部没有设置灯光信息的话，渲染效果与自发光有关，如果自发光是黑色的
				那么最后就是黑色的。
	默认为true,受光照影响
	*/
	bool	    m_bLightFlag; //is it influenced by engine light


public:
	void SetLightFlag(bool enable);
public:
	virtual bool Update(int delta);
	virtual bool Render();
	virtual bool LoadModel(const char* filename){ return true; }; //日后扩展从字节流中加载文件
protected:
	virtual bool CheckAnimate(){ return false; };
};
#endif