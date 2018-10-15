#include "stdafx.h"
#include "ShapeHandler.h"
#include "WindowShape.h"
#include "DoorShape.h"
#include "ObjectShape.h"

CShapeHandler* CShapeHandler::instance = NULL;

CShapeHandler* CShapeHandler::GetInstance()
{
	if (!instance) {
		instance = new CShapeHandler();
	}
	return instance;
}
int CShapeHandler::MakeAutoIncId()
{
	m_nAutoIncId += 1;
	return m_nAutoIncId;
}
CShapeHandler::CShapeHandler()
{
	m_CFileManager = CFileManager::GetInstance();
	m_nAutoIncId = 0;
	m_nSelectRange = 15;
	m_nDrawRange = 7;
	m_nDrawSelectRange = 7;
	NoAddShape = FALSE;
}
CShapeHandler::~CShapeHandler()
{
	SAFE_DELETE(instance);
}

//////////////////////////////////////////////////////////////////////////
/// CRUD
bool CShapeHandler::AddShape(CShape *CNewShape)
{
	if (NoAddShape == TRUE)
	{
		cout << "No Shape\n";
		NoAddShape = FALSE;
		return FALSE;
	}
	/*
	 - enum에 따른 분배 코드 지워가는 중! 가상 함수를 이용해 분배한다.
	   이것만 부르면 알아서 다 생성해줌
	 */
	CShape *pNewShape = CNewShape;
	pNewShape->AddShape();

	return TRUE;
}


CShape * CShapeHandler::GetShapeByID(int nId)
{
	if (nId < 0)
		ASSERT(0);

#pragma warning(push)
#pragma warning(disable: 4018)
	for(int i=0; i<m_CaShape.size(); i++)
#pragma warning(pop)
	{
		if (m_CaShape[i]->GetId() == nId)
		{
			return m_CaShape[i];
		}
	}
	
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
/// 기능
void CShapeHandler::Undo()
{

}
void CShapeHandler::Redo()
{

}
void CShapeHandler::Clear()
{
	for (auto iter : m_CaShape)
	{
		SAFE_DELETE(iter);
	}
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
void CShapeHandler::Move(CPoint point) //door list와 window list를 같이 움직인다.
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return;
	}

	CShape *tmpShape = m_CaShape.at(index);
	int tmpX, tmpY, tmpWidth, tmpHeight;

	if (typeid(*tmpShape) == typeid(CRoomShape)) //RoomShape Move시 그룹화 된 문과 창문을 같이 움직여야 됨!
	{
		CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(m_CaShape.at(index)); 		//Room

		// 밑에처럼 다른 문 벡터나 창문 벡터처럼 한번에 해도 되지만 좀 알아보기 쉽게 방은 그냥 템프 하나 구해놈
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
		// 문 안의 도형
		{
			//In Room Door, Window
#pragma warning(push)
#pragma warning(disable: 4018)
			for (long i = 0; i < tmpRoomShape->m_CaDependent.size(); i++)
#pragma warning(pop)
			{
				// 문 범위 제어 문
				if (point.x + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[0] < 0 || point.x + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[2] > 765 || 
					point.y + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[1] < 0 || point.y + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[3] > 720)
				{
					return;
				}
				else
				{
					tmpRoomShape->m_CaDependent[i]->nX = point.x + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[0];
					tmpRoomShape->m_CaDependent[i]->nY = point.y + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[1];
					tmpRoomShape->m_CaDependent[i]->nWidth = point.x + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[2];
					tmpRoomShape->m_CaDependent[i]->nHeight = point.y + tmpRoomShape->m_CaDependent[i]->m_nMoveSubVal[3];
				}
			}
		}
	}
	else if (typeid(*tmpShape) == typeid(CDoorShape) || typeid(*tmpShape) == typeid(CWindowShape))
	{
		// 창문 문만 클릭했을 시!!!

		CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(GetShapeByID(dynamic_cast<CDependentShape*>(tmpShape)->m_pInContainerShapeID));
		

		if (dynamic_cast<CDependentShape*>(tmpShape)->GetLocaInfo() == LOCA_LEFT  || dynamic_cast<CDependentShape*>(tmpShape)->GetLocaInfo() == LOCA_RIGHT) //왼쪽 혹은 오른쪽에 있다면
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
		else if (dynamic_cast<CDependentShape*>(tmpShape)->GetLocaInfo() == LOCA_UP || dynamic_cast<CDependentShape*>(tmpShape)->GetLocaInfo() == LOCA_DOWN) //위쪽 혹은 아래쪽에 있다면
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
	else if (typeid(*tmpShape) == typeid(CObjectShape))  // 나중에 그룹화 할 듯
	{
		CObjectShape *tmpShape = dynamic_cast<CObjectShape*>(m_CaShape.at(index));

		// 밑에처럼 다른 문 벡터나 창문 벡터처럼 한번에 해도 되지만 좀 알아보기 쉽게 방은 그냥 템프 하나 구해놈
		tmpX = point.x + tmpShape->m_nMoveSubVal[0];
		tmpY = point.y + tmpShape->m_nMoveSubVal[1];
		tmpWidth = point.x + tmpShape->m_nMoveSubVal[2];
		tmpHeight = point.y + tmpShape->m_nMoveSubVal[3];

		// 창문이나 문이 그려지면 그 DrawRange 범위 밖으로도 나가지면 안 됨!!
		if (tmpX < 0 + m_nDrawSelectRange || tmpWidth > 765 - m_nDrawSelectRange || tmpY < 0 + m_nDrawSelectRange || tmpHeight > 720 - m_nDrawSelectRange) // 방 범위 제어 문
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
void CShapeHandler::Select(CPoint point)
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
void CShapeHandler::SetDoorWindowRange(bool bDragFlag, CPoint &OldMousePoint, CPoint &CurrentMousePoint)
{
	// 먼저 그려져야 할 Room 범위를 찾음! ,제일 뒤에서부터 검색햐야 함!  z-index

	bool bAllNoTouch = TRUE; // 어느 곳에도 속하지 않음

#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		//Room이 아니면 Continue, Room안에서 Door나 Window를 찾아야 하기 때문
		if (!(typeid(*m_CaShape.at(i)) == typeid(CRoomShape)))
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
			OldMousePoint.y = m_CaShape.at(i)->nY - m_nDrawRange;
			CurrentMousePoint.y = m_CaShape.at(i)->nY + m_nDrawRange;

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

			m_RememberLocationForDoorWindow = LOCA_UP;
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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

			m_RememberLocationForDoorWindow = LOCA_RIGHT;
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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

			m_RememberLocationForDoorWindow = LOCA_DOWN;
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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

			m_RememberLocationForDoorWindow = LOCA_LEFT;
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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
void CShapeHandler::MakeFiveReferenceVertex(int *naVertex, int tmpVal1, int tmpVal2)
{
	naVertex[0] = tmpVal1;
	naVertex[2] = tmpVal1 + (tmpVal2 - tmpVal1) / 2;
	naVertex[4] = tmpVal2;

	naVertex[1] = tmpVal1 + (naVertex[2] - tmpVal1) / 2;
	naVertex[3] = naVertex[2] + (tmpVal2 - naVertex[2]) / 2;
}
void CShapeHandler::SetRange(int & nX, int & nY, int & nWidth, int & nHeight)
{
	if (nX < 0)
	{
		nX = 0;
	}
	if (nWidth > 765)
	{
		nWidth = 765;
	}
	if (nY < 0)
	{
		nY = 0;
	}
	if (nHeight > 720)
	{
		nHeight = 720;
	}
}

//////////////////////////////////////////////////////////////////////////
/// Select Shape
void CShapeHandler::InitSelect()
{
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < m_CaShape.size(); i++)
#pragma warning(pop)
	{
		if (m_CaShape.at(i)->bSelectedState)
		{
			m_CaShape.at(i)->bSelectedState = FALSE;

			if (typeid(*m_CaShape.at(i)) == typeid(CRoomShape))
			{
				m_CaShape.at(i)->SetColor(0, 0, 0);
			}
			else if (typeid(*m_CaShape.at(i)) == typeid(CDoorShape))
			{
				m_CaShape.at(i)->SetColor(211, 162, 127);
			}
			else if (typeid(*m_CaShape.at(i)) == typeid(CWindowShape))
			{
				m_CaShape.at(i)->SetColor(80, 188, 223);
			}
			else if (typeid(*m_CaShape.at(i)) == typeid(CObjectShape))
			{
				m_CaShape.at(i)->SetColor(111, 128, 244);
			}
			else
			{
				cout << "InitSelected Error\n";
			}

		}
	}

}
int CShapeHandler::GetCurrentSelectedIndex()
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
int CShapeHandler::HowManySelected()
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
int CShapeHandler::UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight)
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}
	CShape *tmpShape = m_CaShape.at(index);
	tmpShape->SetRect(nX, nY, nWidth, nHeight);

	return MY_SUCCES;
}
int CShapeHandler::DeleteSelectedShape() // 만약 Room 이라면 그 안에 존재하는 Door와 Window 모두 벡터에서 같이 삭제해야 함
{
	int nSelectedIndex = GetCurrentSelectedIndex();

	if (nSelectedIndex == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}
	//////////////////////////////////////////////////////////////////////////
	// [ 그룹화 삭제 알고리즘 ]
	if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(CRoomShape))
	{
		auto tmpInRoomPtr = m_CaShape.at(nSelectedIndex); //먼저 방의 포인터를 받아둠
		tmpInRoomPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		// 먼저 방안의 Dependent 벡터를 모두 동적할당 해제 후 삭제
		for (auto nIdx : dynamic_cast<CRoomShape*>(m_CaShape[nSelectedIndex])->m_CaDependent)
		{
			nIdx->SetId(-600000);
			SAFE_DELETE(nIdx);
		}
		dynamic_cast<CRoomShape*>(m_CaShape[nSelectedIndex])->m_CaDependent.clear();

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
	else if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(CDoorShape) || typeid(*m_CaShape.at(nSelectedIndex)) == typeid(CWindowShape))
	{
		// 선택된 방 안의 문의 벡터를 받아 둠
		// 주솟 값을 받아야함!!!! 저기 있는 벡터는 포인터 변수가 아님 꼭 기억해 놓을 것!

		auto tmpDependentPtr = &dynamic_cast<CRoomShape*>(GetShapeByID(dynamic_cast<CDependentShape*>(m_CaShape[nSelectedIndex])->m_pInContainerShapeID))->m_CaDependent;
		// 선택된 문의 포인터
		auto tmpSelectedDoorPtr = m_CaShape.at(nSelectedIndex);

		tmpSelectedDoorPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		//  전체 CShape 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제
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
	}
	else if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(CObjectShape))
	{
	CObjectShape *tmpShape = dynamic_cast<CObjectShape*>(m_CaShape.at(nSelectedIndex));
		m_CaShape.erase(m_CaShape.begin() + nSelectedIndex);
		SAFE_DELETE(tmpShape);//  선택된 object 삭제, delete
	}
	else
	{
		cout << "Delete Error\n";
		return MY_ERROR;
	}
	return MY_SUCCES;
}
int CShapeHandler::CopySelectedShape(int nIndex)
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;

	int index; //Copy, Paste 때문에 이렇게 구현
	if (nIndex == MY_ERROR)
	{
		index = GetCurrentSelectedIndex();
	}
	else
	{
		index = nIndex;
	}

	if (index == MY_ERROR)
	{
		return MY_ERROR;
	}
	else
	{
		CShape *tmpShape = m_CaShape[index];
		CShape *CNewCopyShape;

		// 얘는 안에 들어가있는 문이나 창문은 복사 안 할꺼니까 이렇게 새로 만듬
		if (typeid(*tmpShape) == typeid(CRoomShape))
		{
			CNewCopyShape = new CRoomShape(tmpShape->GetId(), tmpShape->nX, tmpShape->nY, tmpShape->nWidth, tmpShape->nHeight);
		}
		else if (typeid(*tmpShape) == typeid(CObjectShape))
		{
			CNewCopyShape = new CObjectShape(tmpShape->GetId(), tmpShape->nX, tmpShape->nY, tmpShape->nWidth,
				tmpShape->nHeight, dynamic_cast<CObjectShape*>(tmpShape)->m_nIconID);
		}
		else if (typeid(*tmpShape) == typeid(CDoorShape))		// 얘는 어느 방에 들어가있는지도 복사되야 하니까 이렇게 복사함
		{
			CNewCopyShape = new CDoorShape(*dynamic_cast<CDoorShape*>(tmpShape));
		}
		else if (typeid(*tmpShape) == typeid(CWindowShape))	// 얘는 어느 방에 들어가있는지도 복사되야 하니까 이렇게 복사함
		{
			CNewCopyShape = new CWindowShape(*dynamic_cast<CWindowShape*>(tmpShape));
		}
		else
		{
			ASSERT(0);
		}

		CNewCopyShape->CopyShape();
	}

	return MY_SUCCES;
}
int CShapeHandler::WheelSelectedShape(short zDelta)
{
	int index = GetCurrentSelectedIndex();
	if (index == MY_ERROR)
	{
		cout << "Cant Wheel, Anything Selected" << endl;
		return MY_ERROR;
	}

	CShape *tmpShape = m_CaShape[index];

	if (typeid(*tmpShape) == typeid(CRoomShape)) // Room을 선택해서 Scale하려 할 때!,  그룹화 Scale 사용 안 함!
	{
		if (zDelta > 100) //크게 할 때
		{
			if (tmpShape->nX < 5 || tmpShape->nWidth > 740 || tmpShape->nY < 5 || tmpShape->nHeight > 700)
			{
				return MY_ERROR;
			}

			bool bSuccess = TRUE;
			CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(tmpShape);

			for (auto pIter : tmpRoomShape->m_CaDependent) //Room안의 Dependent!
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
			CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(tmpShape);
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
			for (auto pIter : tmpRoomShape->m_CaDependent) //Room안의 Dependent!
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX || pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //왼쪽, 오른쪽
				{
					if (tmpRoomShape->nHeight <= pIter->nHeight  || tmpRoomShape->nY >= pIter->nY )
					{
						bSuccess = FALSE;
						break;
					}
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY || pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //위쪽, 아래쪽
				{
					if (tmpRoomShape->nWidth <= pIter->nWidth  || tmpRoomShape->nX >= pIter->nX )
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
				for (auto pIter : tmpRoomShape->m_CaDependent) //Room안의 Dependent!
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

				//////////////////////////////////////////////////////////////////////////
				// 성공 방 크기 조정, 제일 마지막에
				tmpShape->nX += 2;
				tmpShape->nWidth -= 2;
				tmpShape->nY += 2;
				tmpShape->nHeight -= 2;

			}
		}
	}
	else if(typeid(*tmpShape) == typeid(CDoorShape) || typeid(*tmpShape) == typeid(CWindowShape)) // 문이나 창문 단일 선택 Scale
	{
		CRoomShape *tmpRoomShape;

		if (typeid(*tmpShape) == typeid(CDoorShape) || typeid(*tmpShape) == typeid(CWindowShape)) 
		{
			tmpRoomShape = dynamic_cast<CRoomShape*>(GetShapeByID(dynamic_cast<CDependentShape*>(tmpShape)->m_pInContainerShapeID));
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
		else if (tmpShape->nY + (m_nDrawRange * 2) == tmpShape->nHeight && (tmpShape->nY + m_nDrawRange == tmpRoomShape->nY) || (tmpShape->nY + m_nDrawRange == tmpRoomShape->nHeight)) //위쪽 혹은 아래쪽에 있다면
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
	else //Object
	{
		if (zDelta > 100) //크게 할 때
		{
			if (tmpShape->nX < 5 || tmpShape->nWidth > 740 || tmpShape->nY < 5 || tmpShape->nHeight > 700)
			{
				return MY_ERROR;
			}

			CObjectShape *tmpRoomShape = dynamic_cast<CObjectShape*>(tmpShape);

			tmpShape->nX -= 2;
			tmpShape->nWidth += 2;
			tmpShape->nY -= 2;
			tmpShape->nHeight += 2;		
		}
		else //작게 할 때
		{
			CObjectShape *tmpRoomShape = dynamic_cast<CObjectShape*>(tmpShape);

			if (tmpShape->nWidth - tmpShape->nX < 30 || tmpShape->nHeight - tmpShape->nY < 30)
			{
				return MY_ERROR;
			}

			tmpShape->nX += 2;
			tmpShape->nWidth -= 2;
			tmpShape->nY += 2;
			tmpShape->nHeight -= 2;

		}
	}


	return MY_SUCCES;
}
int CShapeHandler::MagneticSelectedShape()
{
	int index = GetCurrentSelectedIndex();
	if (index == MY_ERROR)
	{
		return MY_ERROR;
	}

	CShape *tmpShape = m_CaShape[index];

	if (typeid(*tmpShape) == typeid(CRoomShape)) //CRoomShape Move시 그룹화 된 문과 창문을 같이 움직여야 됨!
	{
		CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(tmpShape); 		//Room

		//전체 Shape중 인접한 Shape가 있는지
		//뒤에서부터 검색해야 함!
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
		{
			if (tmpRoomShape == m_CaShape[i] || typeid(*m_CaShape[i]) != typeid(CRoomShape)) // 서로 같거나 Room이 아니면 넘김
			{
				continue;
			}

			int nShapeTempX = tmpRoomShape->nX;
			int nShapeTempWidth = tmpRoomShape->nWidth;
			int nShapeTempY = tmpRoomShape->nY;
			int nShapeTempHeight = tmpRoomShape->nHeight;

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

			MakeFiveReferenceVertex(nAnyReferenceX, m_CaShape[i]->nX, m_CaShape[i]->nWidth);
			MakeFiveReferenceVertex(nSelectedReferenceX, nShapeTempX, nShapeTempWidth);
			MakeFiveReferenceVertex(nAnyReferenceY, m_CaShape[i]->nY, m_CaShape[i]->nHeight);
			MakeFiveReferenceVertex(nSelectedReferenceY, nShapeTempY, nShapeTempHeight);

			// 5개의 정점을 모두 Magnetic Range안에 들어가는지 찾음
			for (int j = 0; j < 5; j++)
			{
				if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempY - m_nSelectRange <= m_CaShape[i]->nHeight && m_CaShape[i]->nHeight <= nShapeTempY + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nHeight - m_nSelectRange <= nShapeTempY && nShapeTempY <= m_CaShape[i]->nHeight + m_nSelectRange))
				{ // 위
					naDirectionControl[0]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempWidth - m_nSelectRange <= m_CaShape[i]->nX && m_CaShape[i]->nX <= nShapeTempWidth + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nX - m_nSelectRange <= nShapeTempWidth && nShapeTempWidth <= m_CaShape[i]->nX + m_nSelectRange))
				{ // 오른쪽
					naDirectionControl[1]++;
				}
				else if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempHeight - m_nSelectRange <= m_CaShape[i]->nY && m_CaShape[i]->nY <= nShapeTempHeight + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nY - m_nSelectRange <= nShapeTempHeight && nShapeTempHeight <= m_CaShape[i]->nY + m_nSelectRange))
				{ // 아래
					naDirectionControl[2]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempX - m_nSelectRange <= m_CaShape[i]->nWidth && m_CaShape[i]->nWidth <= nShapeTempX + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nWidth - m_nSelectRange <= nShapeTempX && nShapeTempX <= m_CaShape[i]->nWidth + m_nSelectRange))
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
						nSubMoving = (tmpShape->nY - m_CaShape[i]->nHeight);
						tmpShape->nHeight -= nSubMoving; 
						tmpShape->nY = m_CaShape[i]->nHeight;

						/// 방 안의 창문과 문도 Update
						{
							//In Room Dependent
							for (auto DependentShape : tmpRoomShape->m_CaDependent)
							{
								DependentShape->nY -= nSubMoving;
								DependentShape->nHeight -= nSubMoving;
							}
						}
						break;
					case 1: //오른쪽
						//cout << "Mag 오른쪽 " << endl;
						nSubMoving = (tmpShape->nWidth - m_CaShape[i]->nX);
						tmpShape->nX -= nSubMoving;
						tmpShape->nWidth = m_CaShape[i]->nX;

						/// 방 안의 창문과 문도 Update
						{
							//In Room Dependent
							for (auto DependentShape : tmpRoomShape->m_CaDependent)
							{
								DependentShape->nX -= nSubMoving;
								DependentShape->nWidth -= nSubMoving;
							}
						}
						break;
					case 2: //아래
						//cout << "Mag 아래 " << endl;
						nSubMoving = (tmpShape->nHeight - m_CaShape[i]->nY);
						tmpShape->nY -= nSubMoving;
						tmpShape->nHeight = m_CaShape[i]->nY;

						/// 방 안의 창문과 문도 Update
						{
							//In Room Dependent
							for (auto DependentShape : tmpRoomShape->m_CaDependent)
							{
								DependentShape->nY -= nSubMoving;
								DependentShape->nHeight -= nSubMoving;
							}
						}
						break;
					case 3: //왼쪽
						//cout << "Mag 왼쪽 " << endl;
						nSubMoving = (tmpShape->nX - m_CaShape[i]->nWidth);
						tmpShape->nWidth -= nSubMoving;
						tmpShape->nX = m_CaShape[i]->nWidth;

						/// 방 안의 창문과 문도 Update
						{
							//In Room Dependent
							for (auto DependentShape : tmpRoomShape->m_CaDependent)
							{
								DependentShape->nX -= nSubMoving;
								DependentShape->nWidth -= nSubMoving;
							}
						}
						break;
					}
				}
			}
		}
	}
	else if (typeid(*tmpShape) == typeid(CObjectShape))
	{
	CObjectShape *tmpObjectShape = dynamic_cast<CObjectShape*>(tmpShape); //CUserObjectShape

	//전체 Shape중 인접한 Shape가 있는지
	//뒤에서부터 검색해야 함!
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
		{
			if (tmpObjectShape == m_CaShape[i] || typeid(*m_CaShape[i]) != typeid(CRoomShape)) // 서로 같거나 Room이 아니면 넘김
			{
				continue;
			}

			int nShapeTempX = tmpObjectShape->nX;
			int nShapeTempWidth = tmpObjectShape->nWidth;
			int nShapeTempY = tmpObjectShape->nY;
			int nShapeTempHeight = tmpObjectShape->nHeight;

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

			MakeFiveReferenceVertex(nAnyReferenceX, m_CaShape[i]->nX, m_CaShape[i]->nWidth);
			MakeFiveReferenceVertex(nSelectedReferenceX, nShapeTempX, nShapeTempWidth);
			MakeFiveReferenceVertex(nAnyReferenceY, m_CaShape[i]->nY, m_CaShape[i]->nHeight);
			MakeFiveReferenceVertex(nSelectedReferenceY, nShapeTempY, nShapeTempHeight);

			// 5개의 정점을 모두 Magnetic Range안에 들어가는지 찾음
			for (int j = 0; j < 5; j++)
			{
				if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempY - m_nSelectRange <= m_CaShape[i]->nHeight && m_CaShape[i]->nHeight <= nShapeTempY + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nHeight - m_nSelectRange <= nShapeTempY && nShapeTempY <= m_CaShape[i]->nHeight + m_nSelectRange))
				{ // 위
					naDirectionControl[0]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempWidth - m_nSelectRange <= m_CaShape[i]->nX && m_CaShape[i]->nX <= nShapeTempWidth + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nX - m_nSelectRange <= nShapeTempWidth && nShapeTempWidth <= m_CaShape[i]->nX + m_nSelectRange))
				{ // 오른쪽
					naDirectionControl[1]++;
				}
				else if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempHeight - m_nSelectRange <= m_CaShape[i]->nY && m_CaShape[i]->nY <= nShapeTempHeight + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nY - m_nSelectRange <= nShapeTempHeight && nShapeTempHeight <= m_CaShape[i]->nY + m_nSelectRange))
				{ // 아래
					naDirectionControl[2]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempX - m_nSelectRange <= m_CaShape[i]->nWidth && m_CaShape[i]->nWidth <= nShapeTempX + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nWidth - m_nSelectRange <= nShapeTempX && nShapeTempX <= m_CaShape[i]->nWidth + m_nSelectRange))
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
						nSubMoving = (tmpShape->nY - m_CaShape[i]->nHeight);
						tmpShape->nHeight -= nSubMoving;
						tmpShape->nY = m_CaShape[i]->nHeight;

						break;
					case 1: //오른쪽
						//cout << "Mag 오른쪽 " << endl;
						nSubMoving = (tmpShape->nWidth - m_CaShape[i]->nX);
						tmpShape->nX -= nSubMoving;
						tmpShape->nWidth = m_CaShape[i]->nX;

						break;
					case 2: //아래
						//cout << "Mag 아래 " << endl;
						nSubMoving = (tmpShape->nHeight - m_CaShape[i]->nY);
						tmpShape->nY -= nSubMoving;
						tmpShape->nHeight = m_CaShape[i]->nY;

						break;
					case 3: //왼쪽
						//cout << "Mag 왼쪽 " << endl;
						nSubMoving = (tmpShape->nX - m_CaShape[i]->nWidth);
						tmpShape->nWidth -= nSubMoving;
						tmpShape->nX = m_CaShape[i]->nWidth;

						break;
					}
				}
			}
		}
	}
	return MY_SUCCES;
}
/// 미완성
int CShapeHandler::RotateSelectedShape()
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}

	CShape *tmpShape = m_CaShape.at(index);	

	cout << endl << "Shape 정보 출력 : " << endl;
	cout << "nX : " << to_string(tmpShape->nX) << endl;
	cout << "nY : " << to_string(tmpShape->nY) << endl;
	cout << "nWidth : " << to_string(tmpShape->nWidth) << endl;
	cout << "nHeight : " << to_string(tmpShape->nHeight) << endl;
	cout << endl << endl;


	return MY_SUCCES;
}