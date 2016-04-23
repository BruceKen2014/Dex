

#include "IDexRender.h"


IDexRender::IDexRender()
{

}

IDexRender::~IDexRender()
{

}

void IDexRender::SetMaterial(const DexMaterial& material)
{
	m_Material = material;
}

void IDexRender::SetLightFlag(char flag)
{
	m_iLightFlag = flag;
}

void IDexRender::SetAmbient(const DexColor ambient)
{
	m_colorAmbient = ambient;
}

void IDexRender::SetShader(IDexShader* shader)
{
	m_pShader = shader;
}

void IDexRender::RenderShader()
{
	if (m_pShader != nullptr)
	{
		m_pShader->Render();
	}
}