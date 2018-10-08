#include "stdafx.h"
#include "WindowShape.h"

CWindowShape::CWindowShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
	m_nLocaInfo = 0; //LOCA_DEFAULT
}

CWindowShape::~CWindowShape()
{
	printf("~WindowShape\n");
}
