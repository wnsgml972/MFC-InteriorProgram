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
		m_CaShape.push_back(new RoomShape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_DOOR)
	{
		DoorShape *CDoorShape = new DoorShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);
		
		CDoorShape->pInRoomShapePointer = m_pRememberIndexForDoorWindowPointer; // 자신이 속해있는 Room의 포인터


		/// 삽입!
		m_CaShape.push_back(CDoorShape);
		dynamic_cast<RoomShape*>(CDoorShape->pInRoomShapePointer)->m_CaDoor.push_back(CDoorShape);
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_WINDOW)
	{
		WindowShape *CWindowShape = new WindowShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);

		CWindowShape->pInRoomShapePointer = m_pRememberIndexForDoorWindowPointer; // 자신이 속해있는 Room의 포인터

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
		tmpX = point.x + m_nMoveSubVal[0];
		tmpY = point.y + m_nMoveSubVal[1];
		tmpWidth = point.x + m_nMoveSubVal[2];
		tmpHeight = point.y + m_nMoveSubVal[3];

		if (tmpX < 0 || tmpWidth > 765 || tmpY < 0 || tmpHeight > 720)
		{
			return;
		}
		else
		{
			tmpShape->nX = tmpX;
			tmpShape->nY = tmpY;
			tmpShape->nWidth = tmpWidth;
			tmpShape->nHeight = tmpHeight;

		}
	}
	else if (typeid(*tmpShape) == typeid(DoorShape) || typeid(*tmpShape) == typeid(WindowShape))
	{
		tmpX = point.x + m_nMoveSubVal[0];
		tmpY = point.y + m_nMoveSubVal[1];
		tmpWidth = point.x + m_nMoveSubVal[2];
		tmpHeight = point.y + m_nMoveSubVal[3];

		if (tmpX < 0 || tmpWidth > 765 || tmpY < 0 || tmpHeight > 720)
		{
			return;
		}
		else
		{
			tmpShape->nX = tmpX;
			tmpShape->nY = tmpY;
			tmpShape->nWidth = tmpWidth;
			tmpShape->nHeight = tmpHeight;

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
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		//Room이 아니면 Continue
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
			break;
		}
		else
		{
			if (!bDragFlag) //드래그 중이 아닐 때만
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
int ShapeHandler::RotateSelectedShape()
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape.at(index);

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
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape.at(index);

	return MY_SUCCES;
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


