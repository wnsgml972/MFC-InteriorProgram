#include "stdafx.h"
#include "UserObjectShape.h"
#include "ShapeHandler.h"

CUserObjectShape::CUserObjectShape(int nId, int nX, int nY, int nWidth, int nHeight, int m_nIconID)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 111;
	nGreen = 128;
	nBlue = 244;
	this->m_nIconID = m_nIconID;
}

CUserObjectShape::~CUserObjectShape()
{
	printf("~UserObjectShape\n");
}

void CUserObjectShape::SetImageIcon(int ID)
{
	m_icon = AfxGetApp()->LoadIcon(ID);
}
MYICON_INFO CUserObjectShape::MyGetIconInfo(HICON hIcon)
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

bool CUserObjectShape::AddShape()
{
	CShapeHandler *tmpShapeHandler = CShapeHandler::GetInstance();
	SetImageIcon(m_nIconID);

	if (nX < 0)	
	{
		SetRect(10, 10, 10 + MyGetIconInfo(m_icon).nWidth,
			10 + MyGetIconInfo(m_icon).nHeight);
	}
	else //Copy ÇÒ ¶§
	{
		SetRect(nX, nY, nX + MyGetIconInfo(m_icon).nWidth,
			nY + MyGetIconInfo(m_icon).nHeight);
	}

	tmpShapeHandler->m_CaShape.push_back(this);
	return TRUE;
}

void CUserObjectShape::SetOwnColor()
{
	printf("³­ »ö±òÀÌ ¾øÀ¸¿è\n");
}

int CUserObjectShape::GetLocaInfo()
{
	ASSERT(0);
	return -1;
}

bool CUserObjectShape::CopyShape()
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