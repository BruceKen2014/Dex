

#ifndef _DEX_SHADER_H
#define _DEX_SHADER_H

#include "IDexVertexDecl.h"
#include "CDexObject.h"
class IDexShader
{
protected:
	CDexObject* m_pTarget;
	IDexVertexDecl* m_pDeclaration;
public:
	IDexShader(){};
	virtual ~IDexShader(){};
public:
	void SetTarget(CDexObject* obj){ m_pTarget = obj;}
	virtual void Render() = 0;
};
#endif