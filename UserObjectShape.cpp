#include "stdafx.h"
#include "UserObjectShape.h"

UserObjectShape::UserObjectShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: Shape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 111;
	nGreen = 128;
	nBlue = 244;
}

UserObjectShape::~UserObjectShape()
{
	printf("~UserObjectShape\n");
}

void UserObjectShape::SetImage()
{
}
