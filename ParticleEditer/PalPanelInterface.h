#ifndef _PAL_PANEL_INTERFACE_H
#define _PAL_PANEL_INTERFACE_H

#include "DexBase/CDexObject.h"

class PalPanelInterface:public CDexObject
{
	Dex_DeclareClass(PalPanelInterface, CDexObject,0)
public:
	PalPanelInterface();
	virtual ~PalPanelInterface();

public:
	virtual bool Initialize() = 0;

public:
	virtual void setVisible(bool visible) = 0;
	virtual bool getVisible() = 0;
	virtual bool Update(int delta);
	virtual bool Render() = 0;
};

#endif