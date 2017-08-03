

#ifndef _DEX_WIDGET_PROGRESSBAR_H
#define _DEX_WIDGET_PROGRESSBAR_H

#include "DexWidget.h"
#include "../DexBase/DexString.h"
#include "../DexMath/DexVector2.h"
class DexWidgetProgressBar :public CDexWidget
{
public:
	typedef enum EProgressBarType
	{
		EPBT_L2R,//������
		EPBT_B2T,//���µ���   
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
	virtual bool Update(int delta);
	virtual void RenderThis();
protected:
	CDexTex* m_pBackImage;
	CDexTex* m_pFrontImage;
	DexRectF  m_rectBackImageSrcArea; //�����е�����
	DexRectF  m_rectFrontImageSrcArea; //�����е�����
	DexRectF  m_rectProgress; //�������ڿؼ���ʵ��ռ�ݵ�����
	EProgressBarType m_eProgressType;
	DFloat32   m_fProgress;
protected:
	virtual void ShutDown();
	void CalculateImageUv(DexVector2& uv0, DexVector2& uv1, DexVector2& uv2, DexVector2& uv3, const DexRectF& rect, CDexTex* pImage);
};
#endif