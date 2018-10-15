#include "stdafx.h"
#include "SuperContainerShape.h"

CSuperContainerShape::CSuperContainerShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CContainerShape(nId, nX, nY, nWidth, nHeight)
{
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

bool CSuperContainerShape::MoveShape()
{
	return false;
}

bool CSuperContainerShape::MagneticShape()
{
	return false;
}

bool CSuperContainerShape::EditShape()
{
	return false;
}

bool CSuperContainerShape::WheelShape()
{
	return false;
}
