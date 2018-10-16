#include "stdafx.h"
#include "RoomShape.h"

CRoomShape::CRoomShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CBasicContainerShape(nId, nX, nY, nWidth, nHeight)
{
	m_nType = TYPE_BASIC_CONTAINER;
}

CRoomShape::~CRoomShape()
{
	printf("~RoomShape\n");
}

void CRoomShape::SetOwnColor()
{
	nRed = 0;  nGreen = 0;  nBlue = 0;
}