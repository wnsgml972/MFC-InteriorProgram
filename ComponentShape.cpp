#include "stdafx.h"
#include "ComponentShape.h"

CComponentShape::CComponentShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
	m_nType = TYPE_DEFAULT;
}

CComponentShape::~CComponentShape()
{

}