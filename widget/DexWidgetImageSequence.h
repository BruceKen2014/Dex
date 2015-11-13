


#pragma  once


#include "../DexBase/typedefine.h"
#include "DexWidget.h"
#include <vector>
//图片序列
class CDexTex;
class CDexWidgetImageSequence: public CDexWidget
{
protected:
	 typedef std::vector<stUiImage*>  TImageVec;
	 TImageVec m_ImageVec;
	 int16      m_currIndex;  //当前显示第几张
	 int16      m_interval;   //间隔：单位毫秒
	 int16      m_currPast;   //当前图片显示已经过去的时间
	 bool       m_stop;       //是否暂停
protected:
	virtual void ShutDown();
public:
	CDexWidgetImageSequence();
	~CDexWidgetImageSequence();

public:
	virtual bool Update(int delta);
	virtual void RenderThis();

public:
	void Stop();
	void Play();
	bool getStop();
	void SetInterval(int interval);
	bool AddImage(string texname, const DexRect& rect = DexRect(), bool mirro = false); //默认整张图 不镜像
};