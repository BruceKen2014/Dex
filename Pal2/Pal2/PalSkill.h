

class CPalPlayer;

//技能的攻击动作类型
enum EPalSkillAttackActionType
{
	PSAAT_NOTMOVE,  //原地施法 
	PSAAT_TARGET,   //跑到目标身边施法
};
//释放技能时摄像机的动作
enum EPalSkillAttackCameraType
{
	PSACT_NOTMOVE,  //不移动
};
struct stPlaSkill
{
	EPalSkillAttackActionType attackType;
	std::string startEffet; //起手特效文件
	std::string flyEffet;   //飞行特效文件
	std::string attackEffet; //到达目标后攻击特效文件
};
class CPalBuffer
{
protected:
	bool   m_good;  //是否有益
	string m_self;  //添加buffer时  作用于自己的表达式
	string m_target;//添加buffer时  作用于目标的表达式
};
class CPalSkill
{
protected:
	stPlaSkill m_data;
};

//战斗场景数据
class CPalBatterContent
{
protected:
	static CPalBatterContent* g_pBatterContent;
	CPalPlayer* attacker;  //本次战斗的发起者
	CPalSkill*  skill;     //发起本次战斗所使用的技能
	std::vector<CPalPlayer*>  m_vecTargets; //目标列表(可以攻击敌人，也可以使己方加血)
	
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