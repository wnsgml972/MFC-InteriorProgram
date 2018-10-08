#include "stdafx.h"
#include "RoomShape.h"

CRoomShape::CRoomShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
}

CRoomShape::~CRoomShape()
{
	printf("~RoomShape\n");
}
