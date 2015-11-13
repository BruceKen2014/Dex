

#pragma once

#include "DexWidget.h"


class CDexWidgetCheckBtn: public CDexWidget
{
protected:
	stUiImage*  m_checkTex;  //选中纹理
	stUiImage*  m_uCheckTex;  //非选中纹理

	bool        m_checked;

protected:
	virtual void ShutDown();

protected:
	virtual bool OnMouseLUp(stEvent event);

public:
	CDexWidgetCheckBtn();
	~CDexWidgetCheckBtn();

public:
	virtual bool Update(int delta);
	virtual void RenderThis();


public:
	bool GetChecked();
	void SetChecked(bool checked);
	void SetCheckTex(string texname, const DexRect& rect = DexRect(), bool mirro = false);
	void SetUnCheckTex(string texname, const DexRect& rect = DexRect(), bool mirro = false);
};