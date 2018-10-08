#include "stdafx.h"
#include "DoorShape.h"


CDoorShape::CDoorShape(int nId, int nX, int nY, int nWidth, int nHeight) : CShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 211;
	nGreen = 162;
	nBlue = 127;
	m_nLocaInfo = 0; //LOCA_DEFAULT
}

CDoorShape::~CDoorShape()
{
	printf("~DoorShape\n");
}
