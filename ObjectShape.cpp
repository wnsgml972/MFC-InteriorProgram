#include "stdafx.h"
#include "ObjectShape.h"
#include "ShapeHandler.h"

CObjectShape::CObjectShape(int nId, int nX, int nY, int nWidth, int nHeight, int m_nIconID)
	: CComponentShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 111;
	nGreen = 128;
	nBlue = 244;
	this->m_nIconID = m_nIconID;
}

// 기본 기능
CObjectShape::~CObjectShape()
{
}

bool CObjectShape::AddShape()
{
	CShapeHandler *tmpShapeHandler = CShapeHandler::GetInstance();
	SetImageIcon(m_nIconID);

	if (nX < 0)
	{
		SetRect(10, 10, 10 + MyGetIconInfo(m_icon).nWidth,
			10 + MyGetIconInfo(m_icon).nHeight);
	}
	else //Copy 할 때
	{
		SetRect(nX, nY, nX + MyGetIconInfo(m_icon).nWidth,
			nY + MyGetIconInfo(m_icon).nHeight);
	}

	tmpShapeHandler->m_CaShape.push_back(this);
	return TRUE;
}

bool CObjectShape::DeleteShape()
{
	return false;
}

bool CObjectShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;

	nX += 10 + nRandLocation;
	nWidth += 10 + nRandLocation;
	nY += 10 + nRandLocation;
	nHeight += 10 + nRandLocation;

	SetId(CShapeHandler::GetInstance()->MakeAutoIncId());
	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);

	return TRUE;
}

bool CObjectShape::MoveShape()
{
	return false;
}

bool CObjectShape::MagneticShape()
{
	return false;
}

bool CObjectShape::EditShape()
{
	return false;
}

bool CObjectShape::WheelShape()
{
	return false;
}

void CObjectShape::SetImageIcon(int nImgIconID)
{
	m_icon = AfxGetApp()->LoadIcon(nImgIconID);
}

MYICON_INFO CObjectShape::MyGetIconInfo(HICON hIcon)
{
	MYICON_INFO myinfo;
	ZeroMemory(&myinfo, sizeof(myinfo));

	ICONINFO info;
	ZeroMemory(&info, sizeof(info));

	BOOL bRes = FALSE;

	bRes = GetIconInfo(hIcon, &info);
	if (!bRes)
		return myinfo;

	BITMAP bmp;
	ZeroMemory(&bmp, sizeof(bmp));

	if (info.hbmColor)
	{
		const int nWrittenBytes = GetObject(info.hbmColor, sizeof(bmp), &bmp);
		if (nWrittenBytes > 0)
		{
			myinfo.nWidth = bmp.bmWidth;
			myinfo.nHeight = bmp.bmHeight;
			myinfo.nBitsPerPixel = bmp.bmBitsPixel;
		}
	}
	else if (info.hbmMask)
	{
		// Icon has no color plane, image data stored in mask
		const int nWrittenBytes = GetObject(info.hbmMask, sizeof(bmp), &bmp);
		if (nWrittenBytes > 0)
		{
			myinfo.nWidth = bmp.bmWidth;
			myinfo.nHeight = bmp.bmHeight / 2;
			myinfo.nBitsPerPixel = 1;
		}
	}

	if (info.hbmColor)
		DeleteObject(info.hbmColor);
	if (info.hbmMask)
		DeleteObject(info.hbmMask);

	return myinfo;
}
