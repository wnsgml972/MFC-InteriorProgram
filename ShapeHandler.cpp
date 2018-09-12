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
	m_nSelectRange = 25;
}
ShapeHandler::~ShapeHandler()
{
	SAFE_DELETE(instance);
}



//////////////////////////////////////////////////////////////////////////
/// CRUD
void ShapeHandler::AddShape(int nX, int nY, int nWidth, int nHeight)
{
	if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_ROOM)
	{
		m_CaShape.push_back(new RoomShape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_DOOR)
	{
		DoorShape *CDoorShape = new DoorShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);
		m_CaShape.push_back(CDoorShape);
		//room의 벡터 안에 넣어야 됨,  수정중
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_WINDOW)
	{
		WindowShape *CWindowShape = new WindowShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);
		m_CaShape.push_back(CWindowShape);
		//room의 벡터 안에 넣어야 됨,  수정중
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
2. MouseMove에서 구해놓은 차이와 현재 드래그 되는 좌표값을 더해준다!
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

	if (typeid(*tmpShape) == typeid(RoomShape))
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

	Shape *tmpShape = m_CaShape.at(index);

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


