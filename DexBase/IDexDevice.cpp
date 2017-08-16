

#include "IDexDevice.h"

IDexDevice::IDexDevice() :DexObject()
{

}

IDexDevice::~IDexDevice()
{

}

void IDexDevice::SetViewMatrixType(eVIEWMATRIXTYPE eType)
{
	m_eViewMatrixType = eType;
}

void IDexDevice::LookAt(const DexVector3& eye, const DexVector3& lookat, const DexVector3& up)
{
	if (m_eViewMatrixType == VIEWMATRIXTYPE_LEFTHAND)
		LookAtLH(eye, lookat, up);
	else 
		LookAtRH(eye, lookat, up);
}
