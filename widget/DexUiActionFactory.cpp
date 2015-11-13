

#include "..\DexBase\typedefine.h"
#include "DexUiActionFactory.h"

CDexUiFactory* CDexUiFactory::g_factory = NULL;

CDexUiFactory::CDexUiFactory()
{
	m_iActionCounts = 0;
	m_iActiveActionCounts = 0;
}
CDexUiFactory::~CDexUiFactory()
{
	for(TUiActionMap::iterator ite = m_mapActions.begin() ; ite != m_mapActions.end(); ite++)
	{
		if((ite->second) != NULL)
		{
			delete ite->second;
		}
	}
}
CDexGuiAction* CDexUiFactory::createAction(EUiActionType type, string name)
{
	CDexGuiAction* action = NULL;
	switch(type)
	{
	case EAT_MOVE:
		{
			action = new CDexGuiActionMove();
			break;
		}
	case EAT_SIZE:
		{
			action = new CDexGuiActionSizeChange();
			break;
		}
	case EAT_ALPHA:
		{
			action = new CDexGuiActionAlphaChange();
			break;
		}
	case EAT_ROTATE:
		{
			action = new CDexGuiActionRotate();
			break;
		}
	default:
		break;
	}
	DEX_ENSURE_P(action);
	m_mapActions[m_iActionCounts] = action;
	action->SetName(name);
	action->SetId(m_iActionCounts++);
	m_iActiveActionCounts++;
	return action;
}

bool CDexUiFactory::removeAction(int actionId)
{
	TUiActionMap::iterator ite = m_mapActions.find(actionId);
	DEX_ENSURE_B(ite != m_mapActions.end());
	delete ite->second;
	m_mapActions.erase(ite);
	return true;
}