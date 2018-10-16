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

	m_nType = TYPE_OBJECT;
}

// 기본 기능
CObjectShape::~CObjectShape()
{
	cout << "~ObjectShape" << endl;
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
	auto tmpShape = this;	
	CShapeHandler::GetInstance()->m_CaShape.erase(CShapeHandler::GetInstance()->m_CaShape.begin() + CShapeHandler::GetInstance()->GetCurrentSelectedIndex());
	SAFE_DELETE(tmpShape);//  선택된 object 삭제, delete

	return TRUE;
}

bool CObjectShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;

	nX += 10 + nRandLocation;
	nWidth += 10 + nRandLocation;
	nY += 10 + nRandLocation;
	nHeight += 10 + nRandLocation;

	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);

	return TRUE;
}

bool CObjectShape::MoveShape(CPoint point)
{
	int tmpX, tmpY, tmpWidth, tmpHeight;

	// 밑에처럼 다른 문 벡터나 창문 벡터처럼 한번에 해도 되지만 좀 알아보기 쉽게 방은 그냥 템프 하나 구해놈
	tmpX = point.x + m_nMoveSubVal[0];
	tmpY = point.y + m_nMoveSubVal[1];
	tmpWidth = point.x + m_nMoveSubVal[2];
	tmpHeight = point.y + m_nMoveSubVal[3];

	// 창문이나 문이 그려지면 그 DrawRange 범위 밖으로도 나가지면 안 됨!!
	if (tmpX < 0 + CShapeHandler::GetInstance()->m_nDrawSelectRange || tmpWidth > 765 - CShapeHandler::GetInstance()->m_nDrawSelectRange 
		|| tmpY < 0 + CShapeHandler::GetInstance()->m_nDrawSelectRange || tmpHeight > 720 - CShapeHandler::GetInstance()->m_nDrawSelectRange) // 방 범위 제어 문
	{
		return FALSE;
	}
	else
	{
		nX = tmpX;
		nY = tmpY;
		nWidth = tmpWidth;
		nHeight = tmpHeight;
	}

	return TRUE;
}

bool CObjectShape::MagneticShape()
{
	int m_nSelectRange = CShapeHandler::GetInstance()->m_nSelectRange;


	//전체 Shape중 인접한 Shape가 있는지
	//뒤에서부터 검색해야 함!
#pragma warning(push)
#pragma warning(disable: 4018)
	for (long i = CShapeHandler::GetInstance()->m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		auto tmpIndexShape = CShapeHandler::GetInstance()->m_CaShape[i];

		if (this == tmpIndexShape || tmpIndexShape->m_nType != TYPE_BASIC_CONTAINER) // 서로 같거나 Room이 아니면 넘김
		{
			continue;
		}

		int nShapeTempX = nX;
		int nShapeTempWidth = nWidth;
		int nShapeTempY = nY;
		int nShapeTempHeight = nHeight;

		int naDirectionControl[4] = { 0 };
		int nAnyReferenceX[5] = { 0 };
		int nSelectedReferenceX[5] = { 0 };
		int nAnyReferenceY[5] = { 0 };
		int nSelectedReferenceY[5] = { 0 };

		// 1. Select > Any
		// 네모를 하나의 라인 하나당 Range가 존재하는 네모로 봤을 때
		// 각각 위에서부터 범위를 조사해감
		// 위 -> 아래 -> 왼쪽 -> 오른쪽 
		// Select 기준에서 Magnetic Range를 만들고 Any Shape의 다섯 좌표가 그 Range 안에 들어간다면 그 방향으로 선택
		// 2. Select < Any
		// 위와 같은 알고리즘으로 그 반대의 Any Shape의 기준에서 Magnetic Range를 만들어 놓고
		// 그 범위 안에 Select Shape의 다섯 좌표가 들어가는지 똑같은 방식으로 조사함 

		 CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nAnyReferenceX, tmpIndexShape->nX, tmpIndexShape->nWidth);
		 CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nSelectedReferenceX, nShapeTempX, nShapeTempWidth);
		 CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nAnyReferenceY, tmpIndexShape->nY, tmpIndexShape->nHeight);
		 CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nSelectedReferenceY, nShapeTempY, nShapeTempHeight);

		// 5개의 정점을 모두 Magnetic Range안에 들어가는지 찾음
		for (int j = 0; j < 5; j++)
		{
			if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempY - m_nSelectRange <= tmpIndexShape->nHeight && tmpIndexShape->nHeight <= nShapeTempY + m_nSelectRange)
				|| (tmpIndexShape->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= tmpIndexShape->nWidth + m_nSelectRange) && (tmpIndexShape->nHeight - m_nSelectRange <= nShapeTempY && nShapeTempY <= tmpIndexShape->nHeight + m_nSelectRange))
			{ // 위
				naDirectionControl[0]++;
			}
			else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempWidth - m_nSelectRange <= tmpIndexShape->nX && tmpIndexShape->nX <= nShapeTempWidth + m_nSelectRange)
				|| (tmpIndexShape->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= tmpIndexShape->nHeight + m_nSelectRange) && (tmpIndexShape->nX - m_nSelectRange <= nShapeTempWidth && nShapeTempWidth <= tmpIndexShape->nX + m_nSelectRange))
			{ // 오른쪽
				naDirectionControl[1]++;
			}
			else if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempHeight - m_nSelectRange <= tmpIndexShape->nY && tmpIndexShape->nY <= nShapeTempHeight + m_nSelectRange)
				|| (tmpIndexShape->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= tmpIndexShape->nWidth + m_nSelectRange) && (tmpIndexShape->nY - m_nSelectRange <= nShapeTempHeight && nShapeTempHeight <= tmpIndexShape->nY + m_nSelectRange))
			{ // 아래
				naDirectionControl[2]++;
			}
			else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempX - m_nSelectRange <= tmpIndexShape->nWidth && tmpIndexShape->nWidth <= nShapeTempX + m_nSelectRange)
				|| (tmpIndexShape->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= tmpIndexShape->nHeight + m_nSelectRange) && (tmpIndexShape->nWidth - m_nSelectRange <= nShapeTempX && nShapeTempX <= tmpIndexShape->nWidth + m_nSelectRange))
			{ // 왼쪽
				naDirectionControl[3]++;
			}
		}

		// 3개 이상 범위에 들어오면 성공
		for (int j = 0; j < 4; j++)
		{
			if (naDirectionControl[j] >= 3)
			{
				int nSubMoving;
				switch (j)
				{
				case 0: //위
					//cout << "Mag 위쪽 " << endl;
					nSubMoving = (nY - tmpIndexShape->nHeight);
					nHeight -= nSubMoving;
					nY = tmpIndexShape->nHeight;

					break;
				case 1: //오른쪽
					//cout << "Mag 오른쪽 " << endl;
					nSubMoving = (nWidth - tmpIndexShape->nX);
					nX -= nSubMoving;
					nWidth = tmpIndexShape->nX;

					break;
				case 2: //아래
					//cout << "Mag 아래 " << endl;
					nSubMoving = (nHeight - tmpIndexShape->nY);
					nY -= nSubMoving;
					nHeight = tmpIndexShape->nY;

					break;
				case 3: //왼쪽
					//cout << "Mag 왼쪽 " << endl;
					nSubMoving = (nX - tmpIndexShape->nWidth);
					nWidth -= nSubMoving;
					nX = tmpIndexShape->nWidth;

					break;
				}
			}
		}
	}

	return TRUE;
}

bool CObjectShape::EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus)
{
	// 이미지 크기 바꾸기~
	if (m_nSubShapeStatus[2] != MY_ERROR) //change Width
	{

	}
	if (m_nSubShapeStatus[3] != MY_ERROR) //change Height
	{

	}

	SetRect(nX, nY, nWidth, nHeight);

	return TRUE;

}

bool CObjectShape::WheelShape(short zDelta)
{
	// 이미지 크기 바꾸기~
	if (zDelta > 100) //크게 할 때
	{
		if (nX < 5 || nWidth > 740 || nY < 5 || nHeight > 700)
		{
			return FALSE;
		}

		nX -= 2;
		nWidth += 2;
		nY -= 2;
		nHeight += 2;
	}
	else //작게 할 때
	{
		if (nWidth - nX < 30 || nHeight - nY < 30)
		{
			return FALSE;
		}

		nX += 2;
		nWidth -= 2;
		nY += 2;
		nHeight -= 2;
	}

	return TRUE;
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
