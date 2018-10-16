#include "stdafx.h"
#include "DependentShape.h"
#include "ShapeHandler.h"

CDependentShape::CDependentShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CComponentShape(nId, nX, nY, nWidth, nHeight)
{
	m_nLocaInfo = LOCA_DEFAULT;
	m_nType = TYPE_DEPENDENT;
}

CDependentShape::~CDependentShape()
{
}

// 기본 기능
bool CDependentShape::AddShape()
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
	if (m_pInContainerShapeID < 0)
		m_pInContainerShapeID = tmpShapeHandler->m_pRememberRoomIDForDoorWindow; // 자신이 속해있는 Room의 ID

	/// 삽입!
	tmpShapeHandler->m_CaShape.push_back(this);
	dynamic_cast<CBasicContainerShape*>(tmpShapeHandler->GetShapeByID(m_pInContainerShapeID))->m_CaDependent.push_back(this);

	return TRUE;
}

bool CDependentShape::DeleteShape()
{
	// 선택된 방 안의 문의 벡터를 받아 둠
	// 주솟 값을 받아야함!!!! 저기 있는 벡터는 포인터 변수가 아님 꼭 기억해 놓을 것!
	auto tmpDependentPtr = &dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID))->m_CaDependent;

	// 선택된 문의 포인터, 즉 나 자신
	auto tmpSelectedDoorPtr = this;

	tmpSelectedDoorPtr->SetId(-600000);

	//////////////////////////////////////////////////////////////////////////
	//  전체 CShape 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제
	for (long i = CShapeHandler::GetInstance()->m_CaShape.size() - 1; i >= 0; i--)
	{
		int nWasteValue = CShapeHandler::GetInstance()->m_CaShape[i]->GetId();
		if (nWasteValue < -50000)
		{
			CShapeHandler::GetInstance()->m_CaShape.erase(CShapeHandler::GetInstance()->m_CaShape.begin() + i);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Dependent 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제
	for (long i = tmpDependentPtr->size() - 1; i >= 0; i--)
	{

		int nWasteValue = tmpDependentPtr->at(i)->GetId();
		if (nWasteValue < -50000)
		{
			tmpDependentPtr->erase(tmpDependentPtr->begin() + i);
			break;
		}
	}

	SAFE_DELETE(tmpSelectedDoorPtr);//  선택된 문 삭제, delete

	return TRUE;
}

bool CDependentShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;
	CBasicContainerShape *tmpBasicContainerShape = dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));


	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //왼쪽 혹은 오른쪽에 있다면
	{
		// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

		nY += 10 + nRandLocation;
		nHeight += 10 + nRandLocation;

		if (nY < tmpBasicContainerShape->nY)
		{
			nY = tmpBasicContainerShape->nY;
		}
		else if (nHeight > tmpBasicContainerShape->nHeight)
		{
			nHeight = tmpBasicContainerShape->nHeight;
		}
	}
	else if (GetLocaInfo() == LOCA_DOWN || GetLocaInfo() == LOCA_UP) //위쪽 혹은 아래쪽에 있다면
	{
		// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

		nX += 10 + nRandLocation;
		nWidth += 10 + nRandLocation;


		if (nX < tmpBasicContainerShape->nX)
		{
			nX = tmpBasicContainerShape->nX;
		}
		else if (nWidth > tmpBasicContainerShape->nWidth)
		{
			nWidth = tmpBasicContainerShape->nWidth;
		}
	}
	else
	{
		cout << "창문, 문 단일 Copy Error\n";
		return FALSE;
	}

	SetOwnColor();
	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);


	return TRUE;
}

bool CDependentShape::MoveShape(CPoint point)
{
	// 창문 문만 클릭했을 시!!!

	CBasicContainerShape *tmpBasicContainerShape = dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));
	int tmpX, tmpY, tmpWidth, tmpHeight;
	 
	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //왼쪽 혹은 오른쪽에 있다면
	{
		//cout << "왼오\n";

		//////////////////////////////////////////////////////////////////////////
		// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

		tmpY = point.y + m_nMoveSubVal[1];
		tmpHeight = point.y + m_nMoveSubVal[3];

		if (tmpY <= tmpBasicContainerShape->nY || tmpHeight >= tmpBasicContainerShape->nHeight)
		{
			return FALSE;
		}
		else
		{
			nY = tmpY;
			nHeight = tmpHeight;
		}
	}
	else if (GetLocaInfo() == LOCA_UP || GetLocaInfo() == LOCA_DOWN) //위쪽 혹은 아래쪽에 있다면
	{
		//cout << "위아래\n";

		//////////////////////////////////////////////////////////////////////////
		// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

		tmpX = point.x + m_nMoveSubVal[0];
		tmpWidth = point.x + m_nMoveSubVal[2];

		if (tmpX <= tmpBasicContainerShape->nX || tmpWidth >= tmpBasicContainerShape->nWidth)
		{
			return FALSE;
		}
		else
		{
			nX = tmpX;
			nWidth = tmpWidth;
		}
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

bool CDependentShape::MagneticShape() // 얘는 마그네틱이 필요 없음!
{
	return FALSE;
}

bool CDependentShape::EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus)
{
	CBasicContainerShape *tmpBasicContainerShape = dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));;
	int tmpSub = MY_ERROR;


	if (GetLocaInfo() == LOCA_UP || GetLocaInfo() == LOCA_DOWN)
	{
		if (m_nSubShapeStatus[0] != MY_ERROR) //change X
		{
			if (nX <= tmpBasicContainerShape->nX) //작아질 때
			{
				tmpSub = m_nRememberShapeStatus[0] - tmpBasicContainerShape->nX;
				nX = tmpBasicContainerShape->nX;
				nWidth = m_nRememberShapeStatus[0] + m_nRememberShapeStatus[2] - tmpSub;
			}
			if (nX + m_nRememberShapeStatus[2] >= tmpBasicContainerShape->nWidth)  //커질 때
			{
				tmpSub = tmpBasicContainerShape->nWidth - (m_nRememberShapeStatus[0] + m_nRememberShapeStatus[2]);
				nWidth = tmpBasicContainerShape->nWidth;
				nX = m_nRememberShapeStatus[0] + tmpSub;
			}
		}
		if (m_nSubShapeStatus[2] != MY_ERROR) //change Width
		{
			if (nWidth >= tmpBasicContainerShape->nWidth)
				nWidth = tmpBasicContainerShape->nWidth;
		}
	}
	else if (GetLocaInfo() == LOCA_RIGHT || GetLocaInfo() == LOCA_LEFT)
	{
		if (m_nSubShapeStatus[1] != MY_ERROR) //change Y
		{
			if (nY <= tmpBasicContainerShape->nY) //작아질 때
			{
				tmpSub = m_nRememberShapeStatus[1] - tmpBasicContainerShape->nY;
				nY = tmpBasicContainerShape->nY;
				nHeight = m_nRememberShapeStatus[1] + m_nRememberShapeStatus[3] - tmpSub;
			}
			if (nY + m_nRememberShapeStatus[3] >= tmpBasicContainerShape->nHeight)  //커질 때
			{
				tmpSub = tmpBasicContainerShape->nHeight - (m_nRememberShapeStatus[1] + m_nRememberShapeStatus[3]);
				nHeight = tmpBasicContainerShape->nHeight;
				nY = m_nRememberShapeStatus[1] + tmpSub;
			}
		}
		if (m_nSubShapeStatus[3] != MY_ERROR) //change Height
		{
			if (nHeight >= tmpBasicContainerShape->nHeight)
				nHeight = tmpBasicContainerShape->nHeight;
		}
	}

	SetRect(nX, nY, nWidth, nHeight);

	return TRUE;
}

bool CDependentShape::WheelShape(short zDelta)
{
	// 내가 속해있는 방!
	CBasicContainerShape *tmpBasicContainerShape =
		dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));

	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //왼쪽 혹은 오른쪽에 있다면
	{
		//////////////////////////////////////////////////////////////////////////
		// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

		if (zDelta > 100)
		{
			if (!(nY - 2 < tmpBasicContainerShape->nY))
			{
				nY -= 2;
			}
			if (!(nHeight + 2 > tmpBasicContainerShape->nHeight))
			{
				nHeight += 2;
			}
		}
		else
		{
			if (!(nHeight - nY < 28))
			{
				nY += 2;
				nHeight -= 2;
			}
		}
	}
	else if (GetLocaInfo() == LOCA_UP || GetLocaInfo() == LOCA_DOWN) //위쪽 혹은 아래쪽에 있다면
	{
		//////////////////////////////////////////////////////////////////////////
		// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

		if (zDelta > 100)
		{
			if (!(nX - 2 < tmpBasicContainerShape->nX))
			{
				nX -= 2;
			}
			if (!(nWidth + 2 > tmpBasicContainerShape->nWidth))
			{
				nWidth += 2;
			}
		}
		else
		{
			if (!(nWidth - nX < 28))
			{
				nX += 2;
				nWidth -= 2;
			}
		}
	}
	else
	{
		cout << "창문, 문 단일 Copy Error\n";
		return FALSE;
	}

	return TRUE;
}

// 추가 기능
int CDependentShape::GetLocaInfo()
{
	return m_nLocaInfo;
}