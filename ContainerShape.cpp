#include "stdafx.h"
#include "ContainerShape.h"

CContainerShape::CContainerShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
	m_nType = TYPE_DEFAULT;
}

CContainerShape::~CContainerShape()
{

}