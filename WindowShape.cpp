#include "stdafx.h"
#include "WindowShape.h"
#include "RoomShape.h"
#include "ShapeHandler.h"

CWindowShape::CWindowShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
	m_nLocaInfo = 0; //LOCA_DEFAULT
}

CWindowShape::~CWindowShape()
{
	printf("~WindowShape\n");
}

bool CWindowShape::AddShape()
{
	CShapeHandler *tmpShapeHandler = CShapeHandler::GetInstance();
	if (tmpShapeHandler->m_CaShape.size() == 0)
		return FALSE;

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

	// Copy 할 때 또 대입 되는것을 방지 함!
	if (m_nLocaInfo <= 0)
		m_nLocaInfo = tmpShapeHandler->m_RememberLocationForDoorWindow;
	if (m_pInRoomShapeID < 0)
		m_pInRoomShapeID = tmpShapeHandler->m_pRememberRoomIDForDoorWindow; // 자신이 속해있는 Room의 ID


	/// 삽입!
	tmpShapeHandler->m_CaShape.push_back(this);	
	dynamic_cast<CRoomShape*>(tmpShapeHandler->GetShapeByID(m_pInRoomShapeID))->m_CaWindow.push_back(this);

	return TRUE;
}

void CWindowShape::SetOwnColor()
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
}

int CWindowShape::GetLocaInfo()
{
	return m_nLocaInfo;
}

bool CWindowShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;
	CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInRoomShapeID));


	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //왼쪽 혹은 오른쪽에 있다면
	{
		// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

		nY += 10 + nRandLocation;
		nHeight += 10 + nRandLocation;

		if (nY < tmpRoomShape->nY)
		{
			nY = tmpRoomShape->nY;
		}
		else if (nHeight > tmpRoomShape->nHeight)
		{
			nHeight = tmpRoomShape->nHeight;
		}
	}
	else if (GetLocaInfo() == LOCA_DOWN || GetLocaInfo() == LOCA_UP) //위쪽 혹은 아래쪽에 있다면
	{
		// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

		nX += 10 + nRandLocation;
		nWidth += 10 + nRandLocation;


		if (nX < tmpRoomShape->nX)
		{
			nX = tmpRoomShape->nX;
		}
		else if (nWidth > tmpRoomShape->nWidth)
		{
			nWidth = tmpRoomShape->nWidth;
		}
	}
	else
	{
		cout << "창문, 문 단일 Copy Error\n";
		return FALSE;
	}

	SetOwnColor();
	SetId(CShapeHandler::GetInstance()->MakeAutoIncId());
	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);


	return TRUE;
}
