#include "stdafx.h"
#include "ShapeHandler.h"
#include "WindowShape.h"
#include "DoorShape.h"
#include "ObjectShape.h"
#include "ShapeFactory.h"

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
1. MouseDown 시 선택된 Shape의 기준 꼭지점과 클릭한 point와의 차이점을 구해놓는다. (마우스를 땔 때 구함)
2. MouseMove에서 구해놓은 차이와 현재 드래그 되는 좌표값을 더해준다!,
		1) OnPaint에서 InitVector를 통해 계속해서 벡터에 있는 애들을 그리고 있다.
		2) 그러므로 Vector안에 있는 Shape의 좌표값만 바꿔주면 자동으로 바뀐 위치에서 그려지게 된다!
		3) 우리는 Vector안의 Shape 좌표 값만 MouseMove에서 계속 바꿔주면 OnPaint에서 알아서 그려준다.
3. 그 값은 InteriorProgramView를 넘으면 안 된다.
4. 그 외 프로그램 상 각종 예외처리
*/
void CShapeHandler::Move(CPoint point) 
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //아직 선택안됨
	{
		return;
	}

	// 단순한 개념 선택 된 도형을 움직인다.
	// 각각의 Move는 각 해당하는 Type의 클래스에 구현되어 있음
	m_CaShape.at(index)->MoveShape(point);

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
		if (!(m_CaShape.at(i)->m_nType == TYPE_BASIC_CONTAINER))
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
			// 모든 도형은 select state 상태를 FALSE로 바꿈
			m_CaShape.at(i)->bSelectedState = FALSE;
			
			// 모든 도형은 각자 자신의 색깔로 돌아감
			m_CaShape[i]->SetOwnColor();

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
int CShapeHandler::DeleteSelectedShape() 
{
	int nSelectedIndex = GetCurrentSelectedIndex();

	if (nSelectedIndex == MY_ERROR) //아직 선택안됨
	{
		return MY_ERROR;
	}

	// 만약 Room 이라면 그 안에 존재하는 Door와 Window 모두 벡터에서 같이 삭제해야 함
	//////////////////////////////////////////////////////////////////////////
	// [ 그룹화 삭제 알고리즘 ] + [ 선택 요소 삭제 알고리즘 ] 각자 Shape에 구현
	m_CaShape.at(nSelectedIndex)->DeleteShape();

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
		CShape *CNewCopyShape = CShapeFactory::GetInstance()->GetShapeWhenCopy(tmpShape);
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

	// 선택된 도형을 Wheel 한다.
	m_CaShape[index]->WheelShape(zDelta);

	return MY_SUCCES;
}
int CShapeHandler::MagneticSelectedShape()
{
	int index = GetCurrentSelectedIndex();
	if (index == MY_ERROR)
	{
		return MY_ERROR;
	}

	// 선택 된 도형을 Magnetic!
	m_CaShape[index]->MagneticShape();

	return MY_SUCCES;
}
int CShapeHandler::EditSelectedShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus)
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR)
	{
		return MY_ERROR;
	}

	if (!m_CaShape[index]->EditShape(nX, nY, nWidth, nHeight, m_nSubShapeStatus, m_nRememberShapeStatus))
		return MY_ERROR;

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

	cout << endl << "여러 정보 출력 : " << endl;
	cout << "전체 Size : " << to_string(m_CaShape.size()) << endl;
	cout << "선택된 갯 수 : " << to_string(HowManySelected()) << endl;
	cout << endl << endl;


	return MY_SUCCES;
}