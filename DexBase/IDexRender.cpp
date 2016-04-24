

#include "IDexRender.h"


IDexRender::IDexRender()
{

}

IDexRender::~IDexRender()
{
	for (DMap<DString, IDexShader*>::iterator ite = m_mapShaders.begin();
		ite != m_mapShaders.end(); ++ite)
	{
		_SafeDelete(ite->second);
	}
	m_mapShaders.clear();
}

IDexShader* IDexRender::GetShader(DString shader)
{
	IDexShader* ret = nullptr;
	DMap<DString, IDexShader*>::iterator ite = m_mapShaders.find(shader);
	if (ite != m_mapShaders.end())
	{
		ret = ite->second;
	}
	return ret;
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