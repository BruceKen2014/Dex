

class CPalPlayer;

//���ܵĹ�����������
enum EPalSkillAttackActionType
{
	PSAAT_NOTMOVE,  //ԭ��ʩ�� 
	PSAAT_TARGET,   //�ܵ�Ŀ�����ʩ��
};
//�ͷż���ʱ������Ķ���
enum EPalSkillAttackCameraType
{
	PSACT_NOTMOVE,  //���ƶ�
};
struct stPlaSkill
{
	EPalSkillAttackActionType attackType;
	std::string startEffet; //������Ч�ļ�
	std::string flyEffet;   //������Ч�ļ�
	std::string attackEffet; //����Ŀ��󹥻���Ч�ļ�
};
class CPalBuffer
{
protected:
	bool   m_good;  //�Ƿ�����
	string m_self;  //���bufferʱ  �������Լ��ı��ʽ
	string m_target;//���bufferʱ  ������Ŀ��ı��ʽ
};
class CPalSkill
{
protected:
	stPlaSkill m_data;
};

//ս����������
class CPalBatterContent
{
protected:
	static CPalBatterContent* g_pBatterContent;
	CPalPlayer* attacker;  //����ս���ķ�����
	CPalSkill*  skill;     //���𱾴�ս����ʹ�õļ���
	std::vector<CPalPlayer*>  m_vecTargets; //Ŀ���б�(���Թ������ˣ�Ҳ����ʹ������Ѫ)
	
private:
	CPalBatterContent()
	{
		attacker = NULL;
		skill = NULL;
	}
	~CPalBatterContent();

public:
	static CPalBatterContent* getPalBatterContent()
	{
		if(g_pBatterContent == NULL)
			g_pBatterContent = new CPalBatterContent();
		return g_pBatterContent;
	}
};