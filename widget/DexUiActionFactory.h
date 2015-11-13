

#ifndef  DEX_UIACTION_FACTORY_H
#define  DEX_UIACTION_FACTORY_H
#include <map>

#include "DexGuiAction.h"
class CDexUiFactory
{
private:
	CDexUiFactory();
	~CDexUiFactory();
	static CDexUiFactory* g_factory;
	int    m_iActionCounts;  //总共创建的actions
	int    m_iActiveActionCounts; //尚在的actions
	typedef std::map<int, CDexGuiAction*> TUiActionMap;
	TUiActionMap m_mapActions;
public:
	static CDexUiFactory* getUiActionFactory()
	{
		if(g_factory == NULL)
			g_factory = new CDexUiFactory;
		return g_factory;
	}

public:
	CDexGuiAction* createAction(EUiActionType type, string name="");
	bool removeAction(int actionId);

};
#endif