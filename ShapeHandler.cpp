#include "stdafx.h"
#include "ShapeHandler.h"
#include "RoomShape.h"

ShapeHandler* ShapeHandler::instance = NULL;

ShapeHandler* ShapeHandler::GetInstance()
{
	if (!instance) {
		instance = new ShapeHandler();
	}
	return instance;
}
int ShapeHandler::MakeAutoIncId()
{
	m_nAutoIncId += 1;
	return m_nAutoIncId;
}
ShapeHandler::ShapeHandler()
{
	CFileManager = FileManager::GetInstance();
	m_nAutoIncId = 0;
	m_nSelectRange = 20;
	m_nDrawRange = 7;
	m_nDrawSelectRange = 7;
	NoAddShape = FALSE;
}
ShapeHandler::~ShapeHandler()
{
	SAFE_DELETE(instance);
}



//////////////////////////////////////////////////////////////////////////
/// CRUD
void ShapeHandler::AddShape(int nX, int nY, int nWidth, int nHeight)
{
	if (NoAddShape == TRUE)
	{
		cout << "No Shape\n";
		NoAddShape = FALSE;
		return;
	}
	if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_ROOM)
	{
		//////////////////////////////////////////////////////////////////////////
		// 마지막 검사
		if (nX > nWidth)
		{
			swap(nX, nWidth);
		}
		if (nY > nHeight)
		{
			swap(nY, nHeight);
		}
		m_CaShape.push_back(new RoomShape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_DOOR)
	{
		//////////////////////////////////////////////////////////////////////////
		// 마지막 검사
		if (nX > nWidth)
		{
			swap(nX, nWidth);
		}
		if (nY > nHeight)
		{
			swap(nY, nHeight);
		}

		DoorShape *CDoorShape = new DoorShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);

		CDoorShape->pInRoomShapePointer = m_pRememberRoomPtrForDoorWindow; // 자신이 속해있는 Room의 포인터


		/// 삽입!
		m_CaShape.push_back(CDoorShape);
		dynamic_cast<RoomShape*>(CDoorShape->pInRoomShapePointer)->m_CaDoor.push_back(CDoorShape);
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_WINDOW)
	{
		//////////////////////////////////////////////////////////////////////////
		// 마지막 검사
		if (nX > nWidth)
		{
			swap(nX, nWidth);
		}
		if (nY > nHeight)
		{
			swap(nY, nHeight);
		}

		WindowShape *CWindowShape = new WindowShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);

		CWindowShape->pInRoomShapePointer = m_pRememberRoomPtrForDoorWindow; // 자신이 속해있는 Room의 포인터

		/// 삽입!
		m_CaShape.push_back(CWindowShape);
		dynamic_cast<RoomShape*>(CWindowShape->pInRoomShapePointer)->m_CaWindow.push_back(CWindowShape);
	}
	else //error
	{
		cout << "Add Shape Error\n";
	}
}
bool ShapeHandler::DeleteShapeById(int nId) // 만약 Room 이라면 그 안에 존재하는 Door와 Window 모두 벡터에서 같이 삭제해야 함
{
	return false;
}
void ShapeHandler::UpdateShapeById(int nId)
{

}
int ShapeHandler::FindVectorIndexById(int nId)
{
	return 0;
}



//////////////////////////////////////////////////////////////////////////
/// 기능
void ShapeHandler::CopyShapeById(int nId)
{

}
void ShapeHandler::Undo()
{

}
void ShapeHandler::Redo()
{

}

void ShapeHandler::Clear()
{
	instance->m_CaShape.clear();
}

/*
		- Move
1. MouseDown 시 선택된 Shape의 기준 꼭지점과 클릭한 point와의 차이점을 구해놓는다.
2. MouseMove에서 구해놓은 차이와 현재 드래그 되는 좌표값을 더해준다!,
		1) OnPaint에서 InitVector를 통해 계속해서 벡터에 있는 애들을 그리고 있다.
		2) 그러므로 Vector안에 있는 Shape의 좌표값만 바꿔주면 자동으로 바뀐 위치에서 그려지게 된다!
		3) 우리는 Vector안의 Shape 좌표 값만 MouseMove에서 계속 바꿔주면 OnPaint에서 알아서 그려준다.
3. 그 값은 InteriorProgramView를 넘으면 안 된다.
4. 그 외 프로그램 상 각종 예외처리
*/
void ShapeHandler::Move(CPoint point) //door list와 window list를 같이 움직인다.
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return;
	}

	Shape *tmpShape = m_CaShape.at(index);
	int tmpX, tmpY, tmpWidth, tmpHeight;

	if (typeid(*tmpShape) == typeid(RoomShape)) //RoomShape Move시 그룹화 된 문과 창문을 같이 움직여야 됨!
	{
		RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(m_CaShape.at(index));

		// 밑에처럼 다른 문 벡터나 창문 벡터처럼 한번에 해도 되지만 좀 알아보기 쉽게 방은 그냥 템프 하나 구해놈
		//Room
		tmpX = point.x + tmpRoomShape->m_nMoveSubVal[0];
		tmpY = point.y + tmpRoomShape->m_nMoveSubVal[1];
		tmpWidth = point.x + tmpRoomShape->m_nMoveSubVal[2];
		tmpHeight = point.y + tmpRoomShape->m_nMoveSubVal[3];

		// 창문이나 문이 그려지면 그 DrawRange 범위 밖으로도 나가지면 안 됨!!
		if (tmpX < 0 + m_nDrawSelectRange || tmpWidth > 765 - m_nDrawSelectRange || tmpY < 0 + m_nDrawSelectRange || tmpHeight > 720 - m_nDrawSelectRange) // 방 범위 제어 문
		{
			return;
		}
		else
		{
			tmpRoomShape->nX = tmpX;
			tmpRoomShape->nY = tmpY;
			tmpRoomShape->nWidth = tmpWidth;
			tmpRoomShape->nHeight = tmpHeight;

		}
		// 방에 대한 것을 먼저 해야하는 게 방 클릭 후 범위 밖으로 나가면 다른 창문이나 문은 움직이면 안 되므로
		// 위에서 먼저 체크하고 리턴해버려야 함

		//In Room Door
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = 0; i < tmpRoomShape->m_CaDoor.size(); i++)
#pragma warning(pop)
		{
			// 문 범위 제어 문
			if (point.x + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[0] < 0 || point.x + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[2] > 765 || point.y + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[1] < 0 || point.y + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[3] > 720)
			{
				return;
			}
			else
			{
				tmpRoomShape->m_CaDoor[i]->nX = point.x + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[0];
				tmpRoomShape->m_CaDoor[i]->nY = point.y + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[1];
				tmpRoomShape->m_CaDoor[i]->nWidth = point.x + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[2];
				tmpRoomShape->m_CaDoor[i]->nHeight = point.y + tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[3];
			}
		}

		//In Room Window
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = 0; i < tmpRoomShape->m_CaWindow.size(); i++)
#pragma warning(push)
		{
			// 창문 범위 제어 문
			if (point.x + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[0] < 0 || point.x + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[2] > 765 || point.y + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[1] < 0 || point.y + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[3] > 720)
			{
				return;
			}
			else
			{
				tmpRoomShape->m_CaWindow[i]->nX = point.x + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[0];
				tmpRoomShape->m_CaWindow[i]->nY = point.y + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[1];
				tmpRoomShape->m_CaWindow[i]->nWidth = point.x + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[2];
				tmpRoomShape->m_CaWindow[i]->nHeight = point.y + tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[3];
			}
		}
	}
	else if (typeid(*tmpShape) == typeid(DoorShape) || typeid(*tmpShape) == typeid(WindowShape))
	{
		// 창문 문만 클릭했을 시!!!

		RoomShape *tmpRoomShape;

		if (typeid(*tmpShape) == typeid(DoorShape))
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->pInRoomShapePointer);
		}
		else if (typeid(*tmpShape) == typeid(WindowShape))
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->pInRoomShapePointer);
		}
		else
		{
			cout << "Move 형 변환 Error\n";
		}

		if (tmpShape->nX + m_nDrawRange == tmpRoomShape->nX || tmpShape->nWidth - m_nDrawRange == tmpRoomShape->nWidth) //왼쪽 혹은 오른쪽에 있다면
		{
			//cout << "왼오\n";

			//////////////////////////////////////////////////////////////////////////
			// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

			tmpY = point.y + tmpShape->m_nMoveSubVal[1];
			tmpHeight = point.y + tmpShape->m_nMoveSubVal[3];

			if (tmpY <= tmpRoomShape->nY || tmpHeight >= tmpRoomShape->nHeight)
			{
				return;
			}
			else
			{
				tmpShape->nY = tmpY;
				tmpShape->nHeight = tmpHeight;
			}
		}
		else if (tmpShape->nY + m_nDrawRange == tmpRoomShape->nY || tmpShape->nHeight - m_nDrawRange == tmpRoomShape->nHeight) //위쪽 혹은 아래쪽에 있다면
		{
			//cout << "위아래\n";

			//////////////////////////////////////////////////////////////////////////
			// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

			tmpX = point.x + tmpShape->m_nMoveSubVal[0];
			tmpWidth = point.x + tmpShape->m_nMoveSubVal[2];

			if (tmpX <= tmpRoomShape->nX || tmpWidth >= tmpRoomShape->nWidth)
			{
				return;
			}
			else
			{
				tmpShape->nX = tmpX;
				tmpShape->nWidth = tmpWidth;
			}
		}
		else
		{
			cout << "창문, 문 단일 Move Error\n";
		}
	}
	else
	{
		cout << "Move Error\n";
	}
}
void ShapeHandler::Select(CPoint point)
{
	// point는 current mouse point
	// 제일 뒤에서부터 검색햐야 함!  z-index
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		int nShapeTempX = m_CaShape.at(i)->nX;
		int nShapeTempY = m_CaShape.at(i)->nY;
		int nShapeTempWidth = m_CaShape.at(i)->nWidth;
		int nShapeTempHeight = m_CaShape.at(i)->nHeight;

		// 네모를 하나의 라인 하나당 Range가 존재하는 네모로 봤을 때
		// 각각 위에서부터 범위를 조사해감
		// 위 -> 오른쪽 -> 아래 -> 왼쪽
		if ((nShapeTempX - m_nSelectRange <= point.x && point.x <= nShapeTempWidth + m_nSelectRange && nShapeTempY - m_nSelectRange <= point.y && point.y <= nShapeTempY + m_nSelectRange) ||
			(nShapeTempWidth - m_nSelectRange <= point.x && point.x <= nShapeTempWidth + m_nSelectRange && nShapeTempY - m_nSelectRange <= point.y && point.y <= nShapeTempHeight + m_nSelectRange) ||
			(nShapeTempX - m_nSelectRange <= point.x && point.x <= nShapeTempWidth + m_nSelectRange && nShapeTempHeight - m_nSelectRange <= point.y && point.y <= nShapeTempHeight + m_nSelectRange) ||
			(nShapeTempX - m_nSelectRange <= point.x && point.x <= nShapeTempX + m_nSelectRange && nShapeTempY - m_nSelectRange <= point.y && point.y <= nShapeTempHeight + m_nSelectRange))
		{
			m_CaShape.at(i)->SetColor(255, 0, 0);
			m_CaShape.at(i)->bSelectedState = TRUE;
			break;
		}
	}
}

void ShapeHandler::SetDoorWindowRange(bool bDragFlag, CPoint &OldMousePoint, CPoint &CurrentMousePoint)
{

	// 먼저 그려져야 할 Room 범위를 찾음! ,제일 뒤에서부터 검색햐야 함!  z-index

	bool bAllNoTouch = TRUE; // 어느 곳에도 속하지 않음


#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		//Room이 아니면 Continue, Room안에서 Door나 Window를 찾아야 하기 때문
		if (!(typeid(*m_CaShape.at(i)) == typeid(RoomShape)))
		{
			continue;
		}

		int nShapeTempX = m_CaShape.at(i)->nX;
		int nShapeTempY = m_CaShape.at(i)->nY;
		int nShapeTempWidth = m_CaShape.at(i)->nWidth;
		int nShapeTempHeight = m_CaShape.at(i)->nHeight;

		// 즉 기준 꼭지점을 왼쪽 위로 함
		// 그려지는 도형의 기준 꼭지점을 찾음
		int tempX, tempY, tempWidth, tempHeight;
		if (OldMousePoint.x >= CurrentMousePoint.x)
		{
			tempX = CurrentMousePoint.x;
			tempWidth = OldMousePoint.x;
		}
		else
		{
			tempX = OldMousePoint.x;
			tempWidth = CurrentMousePoint.x;
		}
		if (OldMousePoint.y >= CurrentMousePoint.y)
		{
			tempY = CurrentMousePoint.y;
			tempHeight = OldMousePoint.y;
		}
		else
		{
			tempY = OldMousePoint.y;
			tempHeight = CurrentMousePoint.y;
		}

		// 네모를 하나의 라인 하나당 Range가 존재하는 네모로 봤을 때
		// 각각 위에서부터 범위를 조사해감
		// 위 -> 오른쪽 -> 아래 -> 왼쪽
		if (nShapeTempX - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempWidth + m_nDrawSelectRange && nShapeTempY - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempY + m_nDrawSelectRange)
		{
			// 위, y축 고정
			OldMousePoint.y = m_CaShape.at(i)->nY -m_nDrawRange;
			CurrentMousePoint.y = m_CaShape.at(i)->nY +m_nDrawRange;

			// x축은 Room을 넘어가지 않게 만듬
			if (OldMousePoint.x <= nShapeTempX)
			{
				OldMousePoint.x = nShapeTempX;
			}
			else if (OldMousePoint.x >= nShapeTempWidth)
			{
				OldMousePoint.x = nShapeTempWidth;
			}
			if (CurrentMousePoint.x <= nShapeTempX)
			{
				CurrentMousePoint.x = nShapeTempX;
			}
			else if (CurrentMousePoint.x >= nShapeTempWidth)
			{
				CurrentMousePoint.x = nShapeTempWidth;
			}

			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;
			break;
		}
		else if (nShapeTempWidth - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempWidth + m_nDrawSelectRange && nShapeTempY - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempHeight + m_nDrawSelectRange)
		{
			// 오른쪽, x축 고정
			OldMousePoint.x = m_CaShape.at(i)->nWidth - m_nDrawRange;
			CurrentMousePoint.x = m_CaShape.at(i)->nWidth + m_nDrawRange;

			// y축은 Room을 넘어가지 않게 만듬
			if (OldMousePoint.y <= nShapeTempY)
			{
				OldMousePoint.y = nShapeTempY;
			}
			else if (OldMousePoint.y >= nShapeTempHeight)
			{
				OldMousePoint.y = nShapeTempHeight;
			}
			if (CurrentMousePoint.y <= nShapeTempY)
			{
				CurrentMousePoint.y = nShapeTempY;
			}
			else if (CurrentMousePoint.y >= nShapeTempHeight)
			{
				CurrentMousePoint.y = nShapeTempHeight;
			}

			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;

			break;
		}
		else if (nShapeTempX - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempWidth + m_nDrawSelectRange && nShapeTempHeight - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempHeight + m_nDrawSelectRange)
		{
			// 아래, y축 고정
			OldMousePoint.y = m_CaShape.at(i)->nHeight - m_nDrawRange;
			CurrentMousePoint.y = m_CaShape.at(i)->nHeight + m_nDrawRange;


			// x축은 Room을 넘어가지 않게 만듬
			if (OldMousePoint.x <= nShapeTempX)
			{
				OldMousePoint.x = nShapeTempX;
			}
			else if (OldMousePoint.x >= nShapeTempWidth)
			{
				OldMousePoint.x = nShapeTempWidth;
			}
			if (CurrentMousePoint.x <= nShapeTempX)
			{
				CurrentMousePoint.x = nShapeTempX;
			}
			else if (CurrentMousePoint.x >= nShapeTempWidth)
			{
				CurrentMousePoint.x = nShapeTempWidth;
			}

			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;

			break;
		}
		else if (nShapeTempX - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempX + m_nDrawSelectRange && nShapeTempY - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempHeight + m_nDrawSelectRange)
		{
			// 왼쪽, x축 고정
			OldMousePoint.x = m_CaShape.at(i)->nX - m_nDrawRange;
			CurrentMousePoint.x = m_CaShape.at(i)->nX + m_nDrawRange;

			// y축은 Room을 넘어가지 않게 만듬
			if (OldMousePoint.y <= nShapeTempY)
			{
				OldMousePoint.y = nShapeTempY;
			}
			else if (OldMousePoint.y >= nShapeTempHeight)
			{
				OldMousePoint.y = nShapeTempHeight;
			}
			if (CurrentMousePoint.y <= nShapeTempY)
			{
				CurrentMousePoint.y = nShapeTempY;
			}
			else if (CurrentMousePoint.y >= nShapeTempHeight)
			{
				CurrentMousePoint.y = nShapeTempHeight;
			}

			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;

			break;
		}
		else
		{
			if (!bDragFlag && bAllNoTouch) //드래그 중이 아닐 때만
			{
				NoAddShape = TRUE;
				cout << "else!\n";
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// Select Shape
void ShapeHandler::InitSelect()
{
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < m_CaShape.size(); i++)
#pragma warning(pop)
	{
		if (m_CaShape.at(i)->bSelectedState)
		{
			m_CaShape.at(i)->bSelectedState = FALSE;

			if (typeid(*m_CaShape.at(i)) == typeid(RoomShape))
			{
				m_CaShape.at(i)->SetColor(0, 0, 0);
			}
			else if (typeid(*m_CaShape.at(i)) == typeid(DoorShape))
			{
				m_CaShape.at(i)->SetColor(211, 162, 127);
			}
			else if (typeid(*m_CaShape.at(i)) == typeid(WindowShape))
			{
				m_CaShape.at(i)->SetColor(80, 188, 223);
			}
			else
			{
				cout << "InitSelected Error\n";
			}

		}
	}

}
int ShapeHandler::GetCurrentSelectedIndex()
{
	int nResult = MY_ERROR;

#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < m_CaShape.size(); i++)
#pragma warning(pop)
	{
		if (m_CaShape.at(i)->bSelectedState == TRUE)
		{
			nResult = i;
			break;
		}
	}
	return nResult;
}
int ShapeHandler::HowManySelected()
{
	int nResult = MY_ERROR;

#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < m_CaShape.size(); i++)
#pragma warning(pop)
	{
		if (m_CaShape.at(i)->bSelectedState == TRUE)
		{
			nResult++;
		}
	}

	return nResult;
}
int ShapeHandler::UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight)
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}
	Shape *tmpShape = m_CaShape.at(index);
	tmpShape->SetRect(nX, nY, nWidth, nHeight);

	return MY_SUCCES;
}
int ShapeHandler::DeleteSelectedShape() // 만약 Room 이라면 그 안에 존재하는 Door와 Window 모두 벡터에서 같이 삭제해야 함
{
	int nSelectedIndex = GetCurrentSelectedIndex();

	if (nSelectedIndex == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}
	//////////////////////////////////////////////////////////////////////////
	// [ 그룹화 삭제 알고리즘 ]
	if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(RoomShape))
	{
		auto tmpInRoomPtr = m_CaShape.at(nSelectedIndex); //먼저 방의 포인터를 받아둠
		tmpInRoomPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		// 먼저 방안의 문 벡터 창문 벡터를 모두 동적할당 해제 후 삭제
		for (auto nIdx : dynamic_cast<RoomShape*>(m_CaShape[nSelectedIndex])->m_CaDoor)
		{
			nIdx->SetId(-600000);
			SAFE_DELETE(nIdx);
		}
		for (auto nIdx : dynamic_cast<RoomShape*>(m_CaShape[nSelectedIndex])->m_CaWindow)
		{
			nIdx->SetId(-600000);
			SAFE_DELETE(nIdx);
		}
		dynamic_cast<RoomShape*>(m_CaShape[nSelectedIndex])->m_CaDoor.clear();
		dynamic_cast<RoomShape*>(m_CaShape[nSelectedIndex])->m_CaWindow.clear();

		//////////////////////////////////////////////////////////////////////////
		// 전체 Shape 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제 후, 동적할당 해제
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
		{
			int nWasteValue = m_CaShape[i]->GetId();
			if (nWasteValue < -50000)
			{
				m_CaShape.erase(m_CaShape.begin() + i);

			}
		}
		SAFE_DELETE(tmpInRoomPtr);
	}
	//////////////////////////////////////////////////////////////////////////
	// [ 선택 요소 삭제 알고리즘 ]
	else if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(DoorShape))
	{
		// 선택된 방 안의 문의 벡터를 받아 둠
		// 주솟 값을 받아야함!!!! 저기 있는 벡터는 포인터 변수가 아님 꼭 기억해 놓을 것!
		auto tmpDoorPtr = &dynamic_cast<RoomShape *>(dynamic_cast<DoorShape*>(m_CaShape[nSelectedIndex])->pInRoomShapePointer)->m_CaDoor;
		// 선택된 문의 포인터
		auto tmpSelectedDoorPtr = m_CaShape.at(nSelectedIndex);

		tmpSelectedDoorPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		//  전체 Shape 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
		{
			int nWasteValue = m_CaShape[i]->GetId();
			if (nWasteValue < -50000)
			{
				m_CaShape.erase(m_CaShape.begin() + i);
				break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 문 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제
		for (long i = tmpDoorPtr->size() - 1; i >= 0; i--)
		{

			int nWasteValue = tmpDoorPtr->at(i)->GetId();
			if (nWasteValue < -50000)
			{
				tmpDoorPtr->erase(tmpDoorPtr->begin() + i);
				break;
			}
		}

		SAFE_DELETE(tmpSelectedDoorPtr);//  선택된 문 삭제, delete
	}
	else if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(WindowShape))
	{
		// 선택된 방 안의 창문의 벡터 포인터를 받아 둠
		auto tmpWindowPtr = &dynamic_cast<RoomShape *>(dynamic_cast<WindowShape*>(m_CaShape[nSelectedIndex])->pInRoomShapePointer)->m_CaWindow;
		// 선택된 창문의 포인터
		auto tmpSelectedWindowPtr = m_CaShape.at(nSelectedIndex);

		tmpSelectedWindowPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		// 전체 Shape 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
		{
			int nWasteValue = m_CaShape[i]->GetId();
			if (nWasteValue < -50000)
			{
				m_CaShape.erase(m_CaShape.begin() + i);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 창문 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제
		for (long i = tmpWindowPtr->size() - 1; i >= 0; i--)
		{
			int nWasteValue = tmpWindowPtr->at(i)->GetId();
			if (nWasteValue < -50000)
			{
				tmpWindowPtr->erase(tmpWindowPtr->begin() + i);
			}
		}
		SAFE_DELETE(tmpSelectedWindowPtr);//  선택된 문 삭제, delete
	}
	else
	{
		cout << "Delete Error\n";
		return MY_ERROR;
	}
	return MY_SUCCES;
}
int ShapeHandler::CopySelectedShape()
{
	cout << "Copy\n";

	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR)
	{
		cout << "Cant Copy, Anything Selected" << endl;
		return MY_ERROR;
	}
	else
	{
		Shape *tmpShape = m_CaShape[index];

		if (typeid(*tmpShape) == typeid(RoomShape)) // Room을 선택해서 복사하려 할 때!,  그룹화 복사 사용 안 함!
		{
			RoomShape *CNewCopyShape = new RoomShape(*dynamic_cast<RoomShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id를 다시 부여해야 함!

			CNewCopyShape->nX += 10;
			CNewCopyShape->nWidth += 10;
			CNewCopyShape->nY += 10;
			CNewCopyShape->nHeight += 10;

			if (CNewCopyShape->nX < 0)
			{
				CNewCopyShape->nX = 0;
			}
			if (CNewCopyShape->nWidth > 765)
			{
				CNewCopyShape->nWidth = 765;
			}
			if (CNewCopyShape->nY < 0)
			{
				CNewCopyShape->nY = 0;
			}
			if (CNewCopyShape->nHeight > 720)
			{
				CNewCopyShape->nHeight = 720;
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_ROOM; // Room 생성 상태로 바꿈
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // 기본 상태로 다시 되돌아옴!
		}
		else if (typeid(*tmpShape) == typeid(DoorShape)) // Door를 선택해서 복사하려 할 때!
		{
			DoorShape *CNewCopyShape = new DoorShape(*dynamic_cast<DoorShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id를 다시 부여해야 함!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_nDrawRange * 2) == CNewCopyShape->nWidth) //왼쪽 혹은 오른쪽에 있다면
			{
				//cout << "왼오\n";

				//////////////////////////////////////////////////////////////////////////
				// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nY += 10;
				CNewCopyShape->nHeight += 10;

				if (CNewCopyShape->nY < tmpRoomShape->nY)
				{
					CNewCopyShape->nY = tmpRoomShape->nY;
				}
				else if (CNewCopyShape->nHeight > tmpRoomShape->nHeight)
				{
					CNewCopyShape->nHeight = tmpRoomShape->nHeight;
				}
			}
			else if (CNewCopyShape->nY + (m_nDrawRange * 2) == CNewCopyShape->nHeight) //위쪽 혹은 아래쪽에 있다면
			{
				//cout << "위아래\n";

				//////////////////////////////////////////////////////////////////////////
				// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nX += 10;
				CNewCopyShape->nWidth += 10;

				if (CNewCopyShape->nX < tmpRoomShape->nX)
				{
					CNewCopyShape->nX = tmpRoomShape->nX;
				}
				else if (CNewCopyShape->nWidth > tmpRoomShape->nWidth)
				{
					CNewCopyShape->nWidth = tmpRoomShape->nWidth;
				}
			}
			else
			{
				cout << "창문, 문 단일 Copy Error\n";
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_DOOR; // Door 생성 상태로 바꿈
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // 기본 상태로 다시 되돌아옴!
		}
		else if (typeid(*tmpShape) == typeid(WindowShape)) // Window를 선택해서 복사하려 할 때!
		{
			WindowShape *CNewCopyShape = new WindowShape(*dynamic_cast<WindowShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id를 다시 부여해야 함!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_nDrawRange * 2) == CNewCopyShape->nWidth) //왼쪽 혹은 오른쪽에 있다면
			{
				//cout << "왼오\n";

				//////////////////////////////////////////////////////////////////////////
				// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nY += 10;
				CNewCopyShape->nHeight += 10;

				if (CNewCopyShape->nY < tmpRoomShape->nY)
				{
					CNewCopyShape->nY = tmpRoomShape->nY;
				}
				else if (CNewCopyShape->nHeight > tmpRoomShape->nHeight)
				{
					CNewCopyShape->nHeight = tmpRoomShape->nHeight;
				}
			}
			else if (CNewCopyShape->nY + (m_nDrawRange * 2) == CNewCopyShape->nHeight) //위쪽 혹은 아래쪽에 있다면
			{
				//cout << "위아래\n";

				//////////////////////////////////////////////////////////////////////////
				// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nX += 10;
				CNewCopyShape->nWidth += 10;

				if (CNewCopyShape->nX < tmpRoomShape->nX)
				{
					CNewCopyShape->nX = tmpRoomShape->nX;
				}
				else if (CNewCopyShape->nWidth > tmpRoomShape->nWidth)
				{
					CNewCopyShape->nWidth = tmpRoomShape->nWidth;
				}
			}
			else
			{
				cout << "창문, 문 단일 Copy Error\n";
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_WINDOW; // Window 생성 상태로 바꿈
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // 기본 상태로 다시 되돌아옴!
		}
		else
		{
			cout << "Copy Error\n" << endl;
		}
	}

	return MY_SUCCES;
}
int ShapeHandler::WheelSelectedShape(short zDelta)
{
	int index = GetCurrentSelectedIndex();
	if (index == MY_ERROR)
	{
		cout << "Cant Wheel, Anything Selected" << endl;
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape[index];

	if (typeid(*tmpShape) == typeid(RoomShape)) // Room을 선택해서 Scale하려 할 때!,  그룹화 Scale 사용 안 함!
	{
		if (zDelta > 100) //크게 할 때
		{
			if (tmpShape->nX < 5 || tmpShape->nWidth > 740 || tmpShape->nY < 5 || tmpShape->nHeight > 700)
			{
				return MY_ERROR;
			}

			bool bSuccess = TRUE;
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape);

			for (auto pIter : tmpRoomShape->m_CaDoor) //Room안의 Door!
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
				{
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
				{
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //위쪽이면
				{
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
				{
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else
				{
					cout << " 문 그룹 Wheel Error\n";
					bSuccess = FALSE;
					break;
				}
			}
			for (auto pIter : tmpRoomShape->m_CaWindow)
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
				{
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
				{
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //위쪽이면
				{
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
				{
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else
				{
					cout << " 창문 그룹 Wheel Error\n";
					bSuccess = FALSE;
					break;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 성공 방 크기 조정, 제일 마지막에
			if (bSuccess)
			{
				tmpShape->nX -= 2;
				tmpShape->nWidth += 2;
				tmpShape->nY -= 2;
				tmpShape->nHeight += 2;
			}
			else
			{
				cout << " 창문, 문 그룹 전체 Wheel Error\n";
			}
		}
		else //작게 할 때
		{
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape);
			bool bSuccess = TRUE;

			//////////////////////////////////////////////////////////////////////////
			// 전체 크기 체크
			if (tmpShape->nWidth - tmpShape->nX < 30 || tmpShape->nHeight - tmpShape->nY < 30)
			{
				return MY_ERROR;
			}

			//////////////////////////////////////////////////////////////////////////
			// 먼저 하나라도 걸리는 게 있는지 Check 해야 됨!! 하나라도 미리 작아지면 시스템 오류가 나기 때문
			// 아니면 나중에 Undo Redo 구현해서 되돌리게 해도 될 듯?
			for (auto pIter : tmpRoomShape->m_CaDoor) //Room안의 Door!
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX || pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //왼쪽, 오른쪽
				{
					if (tmpRoomShape->nHeight < pIter->nHeight + 5 || tmpRoomShape->nY > pIter->nY - 5)
					{
						bSuccess = FALSE;
						break;
					}
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY || pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //위쪽, 아래쪽
				{
					if (tmpRoomShape->nWidth < pIter->nWidth + 5 || tmpRoomShape->nX > pIter->nX - 5)
					{
						bSuccess = FALSE;
						break;
					}
				}
				else
				{
					cout << "Mouse Wheel 작아지게 Error" << endl;
					bSuccess = FALSE;
					break;
				}
			}
			for (auto pIter : tmpRoomShape->m_CaWindow)
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX || pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //왼쪽, 오른쪽
				{
					if (tmpRoomShape->nHeight < pIter->nHeight + 5 || tmpRoomShape->nY > pIter->nY - 5)
					{
						bSuccess = FALSE;
						break;
					}
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY || pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //위쪽, 아래쪽
				{
					if (tmpRoomShape->nWidth < pIter->nWidth + 5 || tmpRoomShape->nX > pIter->nX - 5)
					{
						bSuccess = FALSE;
						break;
					}
				}
				else
				{
					cout << "Mouse Wheel 작아지게 Error" << endl;
					bSuccess = FALSE;
					break;
				}
			}

			if (!bSuccess)
			{
				cout << " 창문, 문 그룹 전체 Wheel Error\n";
			}
			else
			{
				for (auto pIter : tmpRoomShape->m_CaDoor) //Room안의 Door!
				{
					if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
					{
						pIter->nX += 2;
						pIter->nWidth += 2;
					}
					else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
					{
						pIter->nX -= 2;
						pIter->nWidth -= 2;
					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //위쪽이면
					{
						pIter->nY += 2;
						pIter->nHeight += 2;
					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
					{
						pIter->nY -= 2;
						pIter->nHeight -= 2;
					}
					else // 사실 위에서 다 체크함
					{
						cout << "문 그룹 Wheel Error\n";
						bSuccess = FALSE;
						break;
					}
				}
				for (auto pIter : tmpRoomShape->m_CaWindow)
				{
					if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
					{
						pIter->nX += 2;
						pIter->nWidth += 2;
					}
					else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
					{
						pIter->nX -= 2;
						pIter->nWidth -= 2;

					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //위쪽이면
					{
						pIter->nY += 2;
						pIter->nHeight += 2;
					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
					{
						pIter->nY -= 2;
						pIter->nHeight -= 2;
					}
					else // 사실 위에서 다 체크함
					{
						cout << "창문 그룹 Wheel Error\n";
						bSuccess = FALSE;
						break;
					}
				}

				//////////////////////////////////////////////////////////////////////////
				// 성공 방 크기 조정, 제일 마지막에
				tmpShape->nX += 2;
				tmpShape->nWidth -= 2;
				tmpShape->nY += 2;
				tmpShape->nHeight -= 2;

			}
		}
	}
	else // 문이나 창문 단일 선택 Scale
	{
		RoomShape *tmpRoomShape;

		if (typeid(*tmpShape) == typeid(DoorShape)) //문일때
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->pInRoomShapePointer);
		}
		else	//창문일때
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->pInRoomShapePointer);
		}


		if (tmpShape->nX + (m_nDrawRange * 2) == tmpShape->nWidth && (tmpShape->nX + m_nDrawRange == tmpRoomShape->nX) || (tmpShape->nX + m_nDrawRange == tmpRoomShape->nWidth)) //왼쪽 혹은 오른쪽에 있다면
		{
			//cout << "왼오\n";

			//////////////////////////////////////////////////////////////////////////
			// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

			if (zDelta > 100)
			{
				if (!(tmpShape->nY - 2 < tmpRoomShape->nY))
				{
					tmpShape->nY -= 2;
				}
				if (!(tmpShape->nHeight + 2 > tmpRoomShape->nHeight))
				{
					tmpShape->nHeight += 2;
				}
			}
			else
			{
				if (!(tmpShape->nHeight - tmpShape->nY < 28))
				{
					tmpShape->nY += 2;
					tmpShape->nHeight -= 2;
				}
			}
		}
		else if (tmpShape->nY + (m_nDrawRange * 2) == tmpShape->nHeight  && (tmpShape->nY + m_nDrawRange == tmpRoomShape->nY) || (tmpShape->nY + m_nDrawRange == tmpRoomShape->nHeight)) //위쪽 혹은 아래쪽에 있다면
		{
			//cout << "위아래\n";

			//////////////////////////////////////////////////////////////////////////
			// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

			if (zDelta > 100)
			{
				if (!(tmpShape->nX - 2 < tmpRoomShape->nX))
				{
					tmpShape->nX -= 2;
				}
				if (!(tmpShape->nWidth + 2 > tmpRoomShape->nWidth))
				{
					tmpShape->nWidth += 2;
				}
			}
			else
			{
				if (!(tmpShape->nWidth - tmpShape->nX < 28))
				{
					tmpShape->nX += 2;
					tmpShape->nWidth -= 2;
				}
			}
		}
		else
		{
			cout << "창문, 문 단일 Copy Error\n";
		}
	}
	return MY_SUCCES;
}


/// 미완성
int ShapeHandler::RotateSelectedShape()
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape.at(index);


	cout << endl << "Shape 정보 출력 : " << endl;
	cout << "nX : " << to_string(tmpShape->nX) << endl;
	cout << "nY : " << to_string(tmpShape->nY) << endl;
	cout << "nWidth : " << to_string(tmpShape->nWidth) << endl;
	cout << "nHeight : " << to_string(tmpShape->nHeight) << endl;
	cout << endl << endl;


	return MY_SUCCES;
}
