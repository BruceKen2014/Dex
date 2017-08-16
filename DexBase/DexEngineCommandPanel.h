

#ifndef _DEX_ENGINE_COMMAND_PANEL_H
#define _DEX_ENGINE_COMMAND_PANEL_H
#include "../widget/DexPanelInterface.h"
#include "DexDVector.h"
class DexWidgetEditBox;
class DexPanelCommand : public DexPanelInterface
{
	Dex_DeclareClass(DexPanelCommand, DexPanelInterface, 0)
protected:
	DexWidgetEditBox*  m_pEditBox;
	DInt8			   m_commandIndex;
	DVector<DString>   m_historyCommands;
public:
	DexPanelCommand();
	virtual ~DexPanelCommand();

public:
	virtual bool Initialize();
public:
	virtual void setVisible(bool visible);
	virtual DBool getVisible();
	virtual bool Update(int delta);
	virtual void OnTimer(const stTimer& timer);
	virtual bool Render();
	void OnEditBoxMiscKey(CEventHandler* sys, DexObject* object, stEvent event);
	void OnUpdateCommand(CEventHandler* sys, DexObject* object, stEvent event);
};
#endif
