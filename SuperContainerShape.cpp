#include "stdafx.h"
#include "SuperContainerShape.h"

CSuperContainerShape::CSuperContainerShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CContainerShape(nId, nX, nY, nWidth, nHeight)
{
	m_nType = TYPE_SUPER_CONTAINER;
}

CSuperContainerShape::~CSuperContainerShape()
{
}

bool CSuperContainerShape::AddShape()
{
	return false;
}

bool CSuperContainerShape::DeleteShape()
{
	return false;
}

bool CSuperContainerShape::CopyShape()
{
	return false;
}

bool CSuperContainerShape::MoveShape(CPoint point)
{
	return false;
}

bool CSuperContainerShape::MagneticShape()
{
	return false;
}

bool CSuperContainerShape::EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus)
{
	return false;
}

bool CSuperContainerShape::WheelShape(short zDelta)
{
	return false;
}
