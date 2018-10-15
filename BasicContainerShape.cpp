#include "stdafx.h"
#include "BasicContainerShape.h"
#include "ShapeHandler.h"

CBasicContainerShape::CBasicContainerShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CContainerShape(nId,  nX, nY, nWidth, nHeight)
{
}

CBasicContainerShape::~CBasicContainerShape()
{
	printf("~CBasicContainerShape\n");
}

bool CBasicContainerShape::AddShape()
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

bool CBasicContainerShape::DeleteShape()
{
	return false;
}

bool CBasicContainerShape::CopyShape()
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

bool CBasicContainerShape::MoveShape()
{
	return false;
}

bool CBasicContainerShape::MagneticShape()
{
	return false;
}

bool CBasicContainerShape::EditShape()
{
	return false;
}

bool CBasicContainerShape::WheelShape()
{
	return false;
}