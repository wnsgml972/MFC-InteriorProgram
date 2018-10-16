#include "stdafx.h"
#include "DoorShape.h"

CDoorShape::CDoorShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CDependentShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 211;
	nGreen = 162;
	nBlue = 127;

	m_nType = TYPE_DEPENDENT;
}

CDoorShape::~CDoorShape()
{
	printf("~DoorShape\n");
}

void CDoorShape::SetOwnColor()
{
	nRed = 211;
	nGreen = 162;
	nBlue = 127;
}
