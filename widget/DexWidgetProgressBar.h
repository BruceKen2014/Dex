

#ifndef _DEX_WIDGET_PROGRESSBAR_H
#define _DEX_WIDGET_PROGRESSBAR_H

#include "DexWidget.h"
#include "../DexBase/DexString.h"
#include "../DexMath/DexVector2.h"
class DexWidgetProgressBar :public DexWidget
{
	Dex_DeclareClass(DexWidgetProgressBar, DexWidget, 0)
public:
	typedef enum EProgressBarType
	{
		EPBT_L2R,//从左到右
		EPBT_B2T,//从下到上   
	};

public:
	DexWidgetProgressBar();
	~DexWidgetProgressBar();
public:
	CDexTex* getBackImage();;
	CDexTex* getFrontImage();;
	void     SetProgressBarType(EProgressBarType type);
	void     SetProgress(DFloat32 fProgress);
	void SetProgressImage(DString backImageName, const DexRectF& backSrcRect, DString FrontImageName, const DexRectF& FrontSrcRect);
public:
	virtual bool Update(int delta) override;
	virtual void RenderThis() override;
protected:
	CDexTex* m_pBackImage;
	CDexTex* m_pFrontImage;
	DexRectF  m_rectBackImageSrcArea; //纹理中的区域
	DexRectF  m_rectFrontImageSrcArea; //纹理中的区域
	DexRectF  m_rectProgress; //进度条在控件中实际占据的区域
	EProgressBarType m_eProgressType;
	DFloat32   m_fProgress;
protected:
	virtual void ShutDown() override;
	void CalculateImageUv(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3, const DexRectF& rect, CDexTex* pImage);
};
#endif