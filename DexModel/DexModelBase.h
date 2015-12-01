/*
*DexEngine ģ�ʹ������
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
	�Ƿ����ⲿ����Ӱ��,
	�����false,�����ⲿ�Ƿ�򿪹��գ��ڲ���Ⱦ���Ὣ���չرգ�Ȼ����ʾĬ�ϵĲ�������ͼ
	�����true,�������Ҳ�򿪵ƹ�Ļ�����Ⱦʱ�ͽ����մ򿪣���ʱ����ⲿû�����õƹ���Ϣ�Ļ�����ȾЧ�����Է����йأ�����Է����Ǻ�ɫ��
				��ô�����Ǻ�ɫ�ġ�
	Ĭ��Ϊtrue,�ܹ���Ӱ��
	*/
	bool	    m_bLightFlag; //is it influenced by engine light


public:
	void SetLightFlag(bool enable);
public:
	virtual bool Update(int delta);
	virtual bool Render();
	virtual bool LoadModel(const char* filename){ return true; }; //�պ���չ���ֽ����м����ļ�
protected:
	virtual bool CheckAnimate(){ return false; };
};
#endif