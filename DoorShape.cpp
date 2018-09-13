#include "stdafx.h"
#include "DoorShape.h"


DoorShape::DoorShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: Shape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 211;
	nGreen = 162;
	nBlue = 127;
	nShapeVectorIndex = -1; //MY_ERROR
}

DoorShape::~DoorShape()
{
}
