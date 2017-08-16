#ifndef _PAL_PANEL_INTERFACE_H
#define _PAL_PANEL_INTERFACE_H

#include "DexBase/DexObject.h"

class DexPanelInterface:public DexObject
{
	Dex_DeclareClass(DexPanelInterface, DexObject,0)
public:
	DexPanelInterface();
	virtual ~DexPanelInterface();

public:
	virtual bool Initialize() = 0;

public:
	virtual void setVisible(bool visible) = 0;
	virtual bool getVisible() = 0;
	virtual bool Update(int delta);
	virtual bool Render() = 0;
};

#endif