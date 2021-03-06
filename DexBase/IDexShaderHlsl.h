
#ifndef _DEX_SHADER_HLSL_H
#define _DEX_SHADER_HLSL_H

#include <d3dx9.h>
#include "IDexShader.h"

class IDexShaderHlsl: public IDexShader
{
	Dex_DeclareClass(IDexShaderHlsl, IDexShader, 0)
protected:
	LPD3DXEFFECT pFxEffect;
public:
	IDexShaderHlsl();
	virtual ~IDexShaderHlsl();
public:
	virtual void Render() = 0;
};
#endif