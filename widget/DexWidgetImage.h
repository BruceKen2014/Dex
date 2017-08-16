

#pragma once
#include "DexWidget.h"

class CDexTex;
class DexWidgetImage: public DexWidget
{
protected:
	stUiImage* m_pImg;
	CDexTex* m_tex;
	string   m_texname;
	
	DexRectF  m_srcArea; //�����е�����
	bool     m_Mirro;  //�Ƿ���


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
	void SetTexName(string texname); //����ʾ�ҵ���Ӧ���Ƶ�������Դ��
	void SetTexRes(const DexRect& rect); //����Դ����
	void SetTexRes(const DexRectF& rect); //����Դ����
	void SetMirro(bool mirro);
};