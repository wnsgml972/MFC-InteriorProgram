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
	m_nRememberIndexForDoorWindowVector = MY_ERROR;
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
		m_CaShape.push_back(CDoorShape);

		//문이 그려진 해당하는 Room의 벡터 안에 넣어야 됨,   remember 변수 사용ㅎㅎ
		dynamic_cast<RoomShape*>(m_CaShape.at(m_nRememberIndexForDoorWindowVector))->m_CaDoor.push_back(CDoorShape);

		// 자신이 Shape Vector에서는 어느 인덱스에 들어갔는지 기억해놈
		dynamic_cast<RoomShape*>(m_CaShape.at(m_nRememberIndexForDoorWindowVector))->m_CaDoor.back()->nShapeVectorIndex = m_CaShape.size() - 1;
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_WINDOW)
	{
		WindowShape *CWindowShape = new WindowShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);
		m_CaShape.push_back(CWindowShape);

		//문이 그려진 해당하는 Room의 벡터 안에 넣어야 됨,   remember 변수 사용ㅎㅎ
		dynamic_cast<RoomShape*>(m_CaShape.at(m_nRememberIndexForDoorWindowVector))->m_CaWindow.push_back(CWindowShape);

		// 자신이 Shape Vector에서는 어느 인덱스에 들어갔는지 기억해놈
		dynamic_cast<RoomShape*>(m_CaShape.at(m_nRememberIndexForDoorWindowVector))->m_CaWindow.back()->nShapeVectorIndex = m_CaShape.size() - 1;
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

			m_nRememberIndexForDoorWindowVector = i;
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

			m_nRememberIndexForDoorWindowVector = i;
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

			m_nRememberIndexForDoorWindowVector = i;
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

			m_nRememberIndexForDoorWindowVector = i;
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
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}

	if (typeid(*m_CaShape.at(index)) == typeid(RoomShape))
	{

/// 그룹화 삭제 알고리즘
// 찾아서 삭제하는데! 하나 삭제하면 전체 갯수가 줄어드므로 벡터의 인덱스가 바뀜! 
// 그렇기 때문에 다음 루프에서 또 삭제할 때 오류가 남
// 해결 방법으로 벡터의 맨 뒤에서부터 검색해 삭제할 것을 찾으면 됨
// 근ep 창문과 문을 섞어서 만들경우 결국 생성 인덱스가 섞여버림...
// 정렬해야 됨, 정렬해서 뒤에서부터 삭제해야 됨...
// 새로운 벡터를 생성해 값을 다 넣고 정렬한다음 삭제한다.

		vector <int> naDeleteIndexShape;

		//Door ShapeVector 속에서 찾아서 삭제
#pragma warning(push)
#pragma warning(disable: 4018)
		for (int i = dynamic_cast<RoomShape*>(m_CaShape.at(index))->m_CaDoor.size() -1; i >= 0 ; i--)
#pragma warning(pop)
		{
			naDeleteIndexShape.push_back(dynamic_cast<RoomShape*>(m_CaShape.at(index))->m_CaDoor.at(i)->nShapeVectorIndex);
		}

		//Window ShapeVector 속에서 찾아서 삭제
#pragma warning(push)
#pragma warning(disable: 4018)
		for (int i = dynamic_cast<RoomShape*>(m_CaShape.at(index))->m_CaWindow.size()-1; i >=0 ; i--)
#pragma warning(pop)
		{
			naDeleteIndexShape.push_back(dynamic_cast<RoomShape*>(m_CaShape.at(index))->m_CaWindow.at(i)->nShapeVectorIndex);
		}

		// 내림차순 정렬
		sort(naDeleteIndexShape.begin(), naDeleteIndexShape.end(), greater<int>());

#pragma warning(push)
#pragma warning(disable: 4018)
		for (int i = 0; i < naDeleteIndexShape.size(); i++)
#pragma warning(pop)
		{
			m_CaShape.erase(m_CaShape.begin() + naDeleteIndexShape.at(i));
		}
		//Room 삭제
		m_CaShape.erase(m_CaShape.begin() + index);
	}
	else
	{
		m_CaShape.erase(m_CaShape.begin() + index);
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


