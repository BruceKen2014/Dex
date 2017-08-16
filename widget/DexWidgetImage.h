

#pragma once
#include "DexWidget.h"

class CDexTex;
class DexWidgetImage: public DexWidget
{
protected:
	stUiImage* m_pImg;
	CDexTex* m_tex;
	string   m_texname;
	
	DexRectF  m_srcArea; //纹理中的区域
	bool     m_Mirro;  //是否镜像


protected:
	virtual void ShutDown();
	void CalculateUv(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3);
public:
	DexWidgetImage();
	~DexWidgetImage();

public:
	virtual bool Update(int delta);
	virtual void RenderThis();
	CDexTex* getText() {return m_tex;};

public:
	void SetTexName(string texname); //将显示挂到对应名称的纹理资源上
	void SetTexRes(const DexRect& rect); //设置源区域
	void SetTexRes(const DexRectF& rect); //设置源区域
	void SetMirro(bool mirro);
};