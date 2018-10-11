#include "stdafx.h"
#include "RoomShape.h"
#include "ShapeHandler.h"

CRoomShape::CRoomShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
}

CRoomShape::~CRoomShape()
{
	printf("~RoomShape\n");
}

bool CRoomShape::AddShape()
{
	CShapeHandler *tmpShapeHandler = CShapeHandler::GetInstance();

	//////////////////////////////////////////////////////////////////////////
	// 마지막 검사, 기준 재조정
	if (nX > nWidth)
	{
		swap(nX, nWidth);
	}
	if (nY > nHeight)
	{
		swap(nY, nHeight);
	}
	tmpShapeHandler->m_CaShape.push_back(this);
	return TRUE;
}

void CRoomShape::SetOwnColor()
{
	nRed = 0;  nGreen = 0;  nBlue = 0;
}

int CRoomShape::GetLocaInfo()
{
	ASSERT(0);
	return -1;
}

bool CRoomShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;

	nX += 10 + nRandLocation;
	nWidth += 10 + nRandLocation;
	nY += 10 + nRandLocation;
	nHeight += 10 + nRandLocation;

	SetOwnColor();
	SetId(CShapeHandler::GetInstance()->MakeAutoIncId());
	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);

	return TRUE;
}
