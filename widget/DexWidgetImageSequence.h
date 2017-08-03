


#pragma  once


#include "../DexBase/typedefine.h"
#include "DexWidget.h"
#include <vector>
//ͼƬ����
class CDexTex;
class CDexWidgetImageSequence: public CDexWidget
{
protected:
	 typedef std::vector<stUiImage*>  TImageVec;
	 TImageVec m_ImageVec;
	 DInt16      m_currIndex;  //��ǰ��ʾ�ڼ���
	 DInt16      m_interval;   //�������λ����
	 DInt16      m_currPast;   //��ǰͼƬ��ʾ�Ѿ���ȥ��ʱ��
	 bool       m_stop;       //�Ƿ���ͣ
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
	bool AddImage(string texname, const DexRect& rect = DexRect(), bool mirro = false); //Ĭ������ͼ ������
};