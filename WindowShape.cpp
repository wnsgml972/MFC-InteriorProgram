#include "stdafx.h"
#include "WindowShape.h"

CWindowShape::CWindowShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CDependentShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
}

CWindowShape::~CWindowShape()
{
	printf("~WindowShape\n");
}

void CWindowShape::SetOwnColor()
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
}
