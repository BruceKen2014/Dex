
#ifndef _PAL_OPENGL_SCENE_H
#define _PAL_OPENGL_SCENE_H
#include <map>
#include "state/DexGameState.h"
#include "PalTypeDefine.h"

class PalOpenGLTestScene : public DexGameState
{
	Dex_DeclareClass(PalOpenGLTestScene, 0)
public:
	PalOpenGLTestScene();
	virtual ~PalOpenGLTestScene();


public:
	virtual bool ApplyRes();
	virtual void ReleaseRes();

	virtual bool Update(int delta);
	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();

	virtual void  MouseLDown(int xPos, int yPos);
};

#endif