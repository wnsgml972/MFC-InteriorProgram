#include "stdafx.h"
#include "WindowShape.h"

WindowShape::WindowShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: Shape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
	m_nLocaInfo = LOCA_DEFAULT;
}

WindowShape::~WindowShape()
{
	printf("~WindowShape\n");
}
