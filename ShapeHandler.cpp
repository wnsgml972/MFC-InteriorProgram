#include "stdafx.h"
#include "ShapeHandler.h"
#include "RoomShape.h"
#include "UserObjectShape.h"

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
	m_nSelectRange = 15;
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
	if (GlobalNum::nPaintStatus == PAINT_ROOM)
	{
		//////////////////////////////////////////////////////////////////////////
		// ������ �˻�, ���� ������
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
	else if (GlobalNum::nPaintStatus == PAINT_DOOR)
	{
		if (m_CaShape.size() == 0)
			return;

		//////////////////////////////////////////////////////////////////////////
		// ������ �˻�, ���� ������
		if (nX > nWidth)
		{
			swap(nX, nWidth);
		}
		if (nY > nHeight)
		{
			swap(nY, nHeight);
		}

		DoorShape *CDoorShape = new DoorShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);
		CDoorShape->m_nLocaInfo = m_RememberLocationForDoorWindow;
		CDoorShape->m_pInRoomShapePointer = m_pRememberRoomPtrForDoorWindow; // �ڽ��� �����ִ� Room�� ������

		/// ����!
		m_CaShape.push_back(CDoorShape);
		dynamic_cast<RoomShape*>(CDoorShape->m_pInRoomShapePointer)->m_CaDoor.push_back(CDoorShape);
	}
	else if (GlobalNum::nPaintStatus == PAINT_WINDOW)
	{
		if (m_CaShape.size() == 0)
			return;

		//////////////////////////////////////////////////////////////////////////
		// ������ �˻�
		if (nX > nWidth)
		{
			swap(nX, nWidth);
		}
		if (nY > nHeight)
		{
			swap(nY, nHeight);
		}

		WindowShape *CWindowShape = new WindowShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);
		CWindowShape->m_nLocaInfo = m_RememberLocationForDoorWindow;
		CWindowShape->m_pInRoomShapePointer = m_pRememberRoomPtrForDoorWindow; // �ڽ��� �����ִ� Room�� ������

		/// ����!
		m_CaShape.push_back(CWindowShape);
		dynamic_cast<RoomShape*>(CWindowShape->m_pInRoomShapePointer)->m_CaWindow.push_back(CWindowShape);
	}
	else if (GlobalNum::nPaintStatus == PAINT_USER_ADD)
	{
		//////////////////////////////////////////////////////////////////////////
		// ������ �˻�, ���� ������
		if (nX > nWidth)
		{
			swap(nX, nWidth);
		}
		if (nY > nHeight)
		{
			swap(nY, nHeight);
		}
		m_CaShape.push_back(new UserObjectShape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
	}
	else //error
	{
		cout << "Add Shape Error\n";
	}
}
bool ShapeHandler::DeleteShapeById(int nId) // ���� Room �̶�� �� �ȿ� �����ϴ� Door�� Window ��� ���Ϳ��� ���� �����ؾ� ��
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
/// ���
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
	for (auto iter : m_CaShape)
	{
		SAFE_DELETE(iter);
	}
	instance->m_CaShape.clear();
}

/*
		- Move
1. MouseDown �� ���õ� Shape�� ���� �������� Ŭ���� point���� �������� ���س��´�.
2. MouseMove���� ���س��� ���̿� ���� �巡�� �Ǵ� ��ǥ���� �����ش�!,
		1) OnPaint���� InitVector�� ���� ����ؼ� ���Ϳ� �ִ� �ֵ��� �׸��� �ִ�.
		2) �׷��Ƿ� Vector�ȿ� �ִ� Shape�� ��ǥ���� �ٲ��ָ� �ڵ����� �ٲ� ��ġ���� �׷����� �ȴ�!
		3) �츮�� Vector���� Shape ��ǥ ���� MouseMove���� ��� �ٲ��ָ� OnPaint���� �˾Ƽ� �׷��ش�.
3. �� ���� InteriorProgramView�� ������ �� �ȴ�.
4. �� �� ���α׷� �� ���� ����ó��
*/
void ShapeHandler::Move(CPoint point) //door list�� window list�� ���� �����δ�.
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //���� ���þȵ�
	{
		return;
	}

	Shape *tmpShape = m_CaShape.at(index);
	int tmpX, tmpY, tmpWidth, tmpHeight;

	if (typeid(*tmpShape) == typeid(RoomShape)) //RoomShape Move�� �׷�ȭ �� ���� â���� ���� �������� ��!
	{
		RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(m_CaShape.at(index)); 		//Room

		// �ؿ�ó�� �ٸ� �� ���ͳ� â�� ����ó�� �ѹ��� �ص� ������ �� �˾ƺ��� ���� ���� �׳� ���� �ϳ� ���س�
		tmpX = point.x + tmpRoomShape->m_nMoveSubVal[0];
		tmpY = point.y + tmpRoomShape->m_nMoveSubVal[1];
		tmpWidth = point.x + tmpRoomShape->m_nMoveSubVal[2];
		tmpHeight = point.y + tmpRoomShape->m_nMoveSubVal[3];

		// â���̳� ���� �׷����� �� DrawRange ���� �����ε� �������� �� ��!!
		if (tmpX < 0 + m_nDrawSelectRange || tmpWidth > 765 - m_nDrawSelectRange || tmpY < 0 + m_nDrawSelectRange || tmpHeight > 720 - m_nDrawSelectRange) // �� ���� ���� ��
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

		// �濡 ���� ���� ���� �ؾ��ϴ� �� �� Ŭ�� �� ���� ������ ������ �ٸ� â���̳� ���� �����̸� �� �ǹǷ�
		// ������ ���� üũ�ϰ� �����ع����� ��
		// �� ���� ����
		{
			//In Room Door
#pragma warning(push)
#pragma warning(disable: 4018)
			for (long i = 0; i < tmpRoomShape->m_CaDoor.size(); i++)
#pragma warning(pop)
			{
				// �� ���� ���� ��
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
				// â�� ���� ���� ��
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
	}
	else if (typeid(*tmpShape) == typeid(DoorShape) || typeid(*tmpShape) == typeid(WindowShape))
	{
		// â�� ���� Ŭ������ ��!!!

		RoomShape *tmpRoomShape = nullptr;
		int nLocaInfo = -1;

		if (typeid(*tmpShape) == typeid(DoorShape))
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->m_pInRoomShapePointer);
			nLocaInfo = dynamic_cast<DoorShape*>(tmpShape)->m_nLocaInfo;
		}
		else if (typeid(*tmpShape) == typeid(WindowShape))
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->m_pInRoomShapePointer);
			nLocaInfo = dynamic_cast<WindowShape*>(tmpShape)->m_nLocaInfo;
		}
		else
		{
			cout << "Move �� ��ȯ Error\n";
		}

		if ( nLocaInfo == LOCA_LEFT  || nLocaInfo == LOCA_RIGHT) //���� Ȥ�� �����ʿ� �ִٸ�
		{
			//cout << "�޿�\n";

			//////////////////////////////////////////////////////////////////////////
			// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
		else if (nLocaInfo == LOCA_UP || nLocaInfo == LOCA_DOWN) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
		{
			//cout << "���Ʒ�\n";

			//////////////////////////////////////////////////////////////////////////
			// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
			cout << "â��, �� ���� Move Error\n";
		}
	}
	else if (typeid(*tmpShape) == typeid(UserObjectShape)) //RoomShape Move�� �׷�ȭ �� ���� â���� ���� �������� ��!
	{
	UserObjectShape *tmpShape = dynamic_cast<UserObjectShape*>(m_CaShape.at(index)); 		//Room

		// �ؿ�ó�� �ٸ� �� ���ͳ� â�� ����ó�� �ѹ��� �ص� ������ �� �˾ƺ��� ���� ���� �׳� ���� �ϳ� ���س�
		tmpX = point.x + tmpShape->m_nMoveSubVal[0];
		tmpY = point.y + tmpShape->m_nMoveSubVal[1];
		tmpWidth = point.x + tmpShape->m_nMoveSubVal[2];
		tmpHeight = point.y + tmpShape->m_nMoveSubVal[3];

		// â���̳� ���� �׷����� �� DrawRange ���� �����ε� �������� �� ��!!
		if (tmpX < 0 + m_nDrawSelectRange || tmpWidth > 765 - m_nDrawSelectRange || tmpY < 0 + m_nDrawSelectRange || tmpHeight > 720 - m_nDrawSelectRange) // �� ���� ���� ��
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
	// point�� current mouse point
	// ���� �ڿ������� �˻���� ��!  z-index
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		int nShapeTempX = m_CaShape.at(i)->nX;
		int nShapeTempY = m_CaShape.at(i)->nY;
		int nShapeTempWidth = m_CaShape.at(i)->nWidth;
		int nShapeTempHeight = m_CaShape.at(i)->nHeight;

		// �׸� �ϳ��� ���� �ϳ��� Range�� �����ϴ� �׸�� ���� ��
		// ���� ���������� ������ �����ذ�
		// �� -> ������ -> �Ʒ� -> ����
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

	// ���� �׷����� �� Room ������ ã��! ,���� �ڿ������� �˻���� ��!  z-index

	bool bAllNoTouch = TRUE; // ��� ������ ������ ����


#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		//Room�� �ƴϸ� Continue, Room�ȿ��� Door�� Window�� ã�ƾ� �ϱ� ����
		if (!(typeid(*m_CaShape.at(i)) == typeid(RoomShape)))
		{
			continue;
		}

		int nShapeTempX = m_CaShape.at(i)->nX;
		int nShapeTempY = m_CaShape.at(i)->nY;
		int nShapeTempWidth = m_CaShape.at(i)->nWidth;
		int nShapeTempHeight = m_CaShape.at(i)->nHeight;

		// �� ���� �������� ���� ���� ��
		// �׷����� ������ ���� �������� ã��
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

		// �׸� �ϳ��� ���� �ϳ��� Range�� �����ϴ� �׸�� ���� ��
		// ���� ���������� ������ �����ذ�
		// �� -> ������ -> �Ʒ� -> ����
		if (nShapeTempX - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempWidth + m_nDrawSelectRange && nShapeTempY - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempY + m_nDrawSelectRange)
		{
			// ��, y�� ����
			OldMousePoint.y = m_CaShape.at(i)->nY - m_nDrawRange;
			CurrentMousePoint.y = m_CaShape.at(i)->nY + m_nDrawRange;

			// x���� Room�� �Ѿ�� �ʰ� ����
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
			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;
			break;
		}
		else if (nShapeTempWidth - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempWidth + m_nDrawSelectRange && nShapeTempY - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempHeight + m_nDrawSelectRange)
		{
			// ������, x�� ����
			OldMousePoint.x = m_CaShape.at(i)->nWidth - m_nDrawRange;
			CurrentMousePoint.x = m_CaShape.at(i)->nWidth + m_nDrawRange;

			// y���� Room�� �Ѿ�� �ʰ� ����
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
			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;

			break;
		}
		else if (nShapeTempX - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempWidth + m_nDrawSelectRange && nShapeTempHeight - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempHeight + m_nDrawSelectRange)
		{
			// �Ʒ�, y�� ����
			OldMousePoint.y = m_CaShape.at(i)->nHeight - m_nDrawRange;
			CurrentMousePoint.y = m_CaShape.at(i)->nHeight + m_nDrawRange;


			// x���� Room�� �Ѿ�� �ʰ� ����
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
			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;

			break;
		}
		else if (nShapeTempX - m_nDrawSelectRange <= OldMousePoint.x && OldMousePoint.x <= nShapeTempX + m_nDrawSelectRange && nShapeTempY - m_nDrawSelectRange <= OldMousePoint.y && OldMousePoint.y <= nShapeTempHeight + m_nDrawSelectRange)
		{
			// ����, x�� ����
			OldMousePoint.x = m_CaShape.at(i)->nX - m_nDrawRange;
			CurrentMousePoint.x = m_CaShape.at(i)->nX + m_nDrawRange;

			// y���� Room�� �Ѿ�� �ʰ� ����
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
			m_pRememberRoomPtrForDoorWindow = m_CaShape.at(i);
			NoAddShape = FALSE;
			bAllNoTouch = FALSE;

			break;
		}
		else
		{
			if (!bDragFlag && bAllNoTouch) //�巡�� ���� �ƴ� ����
			{
				NoAddShape = TRUE;
				cout << "else!\n";
			}
		}
	}
}
void ShapeHandler::MakeFiveReferenceVertex(int *naVertex, int tmpVal1, int tmpVal2)
{
	naVertex[0] = tmpVal1;
	naVertex[2] = tmpVal1 + (tmpVal2 - tmpVal1) / 2;
	naVertex[4] = tmpVal2;

	naVertex[1] = tmpVal1 + (naVertex[2] - tmpVal1) / 2;
	naVertex[3] = naVertex[2] + (tmpVal2 - naVertex[2]) / 2;
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
			else if (typeid(*m_CaShape.at(i)) == typeid(UserObjectShape))
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

	if (index == MY_ERROR) //���� ���þȵ�
	{
		return MY_ERROR;
	}
	Shape *tmpShape = m_CaShape.at(index);
	tmpShape->SetRect(nX, nY, nWidth, nHeight);

	return MY_SUCCES;
}
int ShapeHandler::DeleteSelectedShape() // ���� Room �̶�� �� �ȿ� �����ϴ� Door�� Window ��� ���Ϳ��� ���� �����ؾ� ��
{
	int nSelectedIndex = GetCurrentSelectedIndex();

	if (nSelectedIndex == MY_ERROR) //���� ���þȵ�
	{
		return MY_ERROR;
	}
	//////////////////////////////////////////////////////////////////////////
	// [ �׷�ȭ ���� �˰��� ]
	if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(RoomShape))
	{
		auto tmpInRoomPtr = m_CaShape.at(nSelectedIndex); //���� ���� �����͸� �޾Ƶ�
		tmpInRoomPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		// ���� ����� �� ���� â�� ���͸� ��� �����Ҵ� ���� �� ����
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
		// ��ü Shape ���Ϳ����� ���̵� ���� ������ ���� ���� ���� ��, �����Ҵ� ����
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
	// [ ���� ��� ���� �˰��� ]
	else if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(DoorShape))
	{
		// ���õ� �� ���� ���� ���͸� �޾� ��
		// �ּ� ���� �޾ƾ���!!!! ���� �ִ� ���ʹ� ������ ������ �ƴ� �� ����� ���� ��!
		auto tmpDoorPtr = &dynamic_cast<RoomShape *>(dynamic_cast<DoorShape*>(m_CaShape[nSelectedIndex])->m_pInRoomShapePointer)->m_CaDoor;
		// ���õ� ���� ������
		auto tmpSelectedDoorPtr = m_CaShape.at(nSelectedIndex);

		tmpSelectedDoorPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		//  ��ü Shape ���Ϳ����� ���̵� ���� ������ ���� ���� ����
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
		// �� ���Ϳ����� ���̵� ���� ������ ���� ���� ����
		for (long i = tmpDoorPtr->size() - 1; i >= 0; i--)
		{

			int nWasteValue = tmpDoorPtr->at(i)->GetId();
			if (nWasteValue < -50000)
			{
				tmpDoorPtr->erase(tmpDoorPtr->begin() + i);
				break;
			}
		}

		SAFE_DELETE(tmpSelectedDoorPtr);//  ���õ� �� ����, delete
	}
	else if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(WindowShape))
	{
		// ���õ� �� ���� â���� ���� �����͸� �޾� ��
		auto tmpWindowPtr = &dynamic_cast<RoomShape *>(dynamic_cast<WindowShape*>(m_CaShape[nSelectedIndex])->m_pInRoomShapePointer)->m_CaWindow;
		// ���õ� â���� ������
		auto tmpSelectedWindowPtr = m_CaShape.at(nSelectedIndex);

		tmpSelectedWindowPtr->SetId(-600000);

		//////////////////////////////////////////////////////////////////////////
		// ��ü Shape ���Ϳ����� ���̵� ���� ������ ���� ���� ����
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
		{
			int nWasteValue = m_CaShape[i]->GetId();
			if (nWasteValue < -50000)
			{
				m_CaShape.erase(m_CaShape.begin() + i);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// â�� ���Ϳ����� ���̵� ���� ������ ���� ���� ����
		for (long i = tmpWindowPtr->size() - 1; i >= 0; i--)
		{
			int nWasteValue = tmpWindowPtr->at(i)->GetId();
			if (nWasteValue < -50000)
			{
				tmpWindowPtr->erase(tmpWindowPtr->begin() + i);
			}
		}
		SAFE_DELETE(tmpSelectedWindowPtr);//  ���õ� �� ����, delete
	}
	else if (typeid(*m_CaShape.at(nSelectedIndex)) == typeid(UserObjectShape))
	{
		UserObjectShape *tmpShape = dynamic_cast<UserObjectShape*>(m_CaShape.at(nSelectedIndex));
		m_CaShape.erase(m_CaShape.begin() + nSelectedIndex);
		SAFE_DELETE(tmpShape);//  ���õ� object ����, delete
	}
	else
	{
		cout << "Delete Error\n";
		return MY_ERROR;
	}
	return MY_SUCCES;
}
int ShapeHandler::CopySelectedShape(int nIndex)
{
	bool type = false;
	int nRandLocation = static_cast<int>((rand() % 10)) + 1;

	int index;
	if (nIndex == MY_ERROR)
	{
		index = GetCurrentSelectedIndex();
	}
	else
	{
		index = nIndex;
		type = true;
	}

	if (index == MY_ERROR)
	{
		return MY_ERROR;
	}
	else
	{
		Shape *tmpShape = m_CaShape[index];

		if (typeid(*tmpShape) == typeid(RoomShape) || typeid(*tmpShape) == typeid(UserObjectShape)) // Room�� �����ؼ� �����Ϸ� �� ��!,  �׷�ȭ ���� ��� �� ��!
		{
			Shape *CNewCopyShape;
			if (typeid(*tmpShape) == typeid(RoomShape))
			{
				CNewCopyShape = new RoomShape(*dynamic_cast<RoomShape*>(tmpShape));
			}
			else
			{
				CNewCopyShape = new UserObjectShape(*dynamic_cast<UserObjectShape*>(tmpShape));
			}
			CNewCopyShape->SetId(MakeAutoIncId()); // Id�� �ٽ� �ο��ؾ� ��!

			if (type)
			{
				CNewCopyShape->nX += 10 + nRandLocation;
				CNewCopyShape->nWidth += 10 + nRandLocation;
				CNewCopyShape->nY += 10 + nRandLocation;
				CNewCopyShape->nHeight += 10 + nRandLocation;
			}
			else
			{
				CNewCopyShape->nX += 10;
				CNewCopyShape->nWidth += 10;
				CNewCopyShape->nY += 10;
				CNewCopyShape->nHeight += 10;
			}

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

			if (typeid(*tmpShape) == typeid(RoomShape))
			{
				GlobalNum::nPaintStatus = PAINT_ROOM; // Room ���� ���·� �ٲ�
			}
			else
			{
				GlobalNum::nPaintStatus = PAINT_USER_ADD; // Room ���� ���·� �ٲ�
			}
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::nPaintStatus = PAINT_BASIC; // �⺻ ���·� �ٽ� �ǵ��ƿ�!
		}
		else if (typeid(*tmpShape) == typeid(DoorShape)) // Door�� �����ؼ� �����Ϸ� �� ��!
		{
			DoorShape *CNewCopyShape = new DoorShape(*dynamic_cast<DoorShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id�� �ٽ� �ο��ؾ� ��!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->m_pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_nDrawRange * 2) == CNewCopyShape->nWidth) //���� Ȥ�� �����ʿ� �ִٸ�
			{
				//cout << "�޿�\n";

				//////////////////////////////////////////////////////////////////////////
				// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

				if (type)
				{
					CNewCopyShape->nY += 10;
					CNewCopyShape->nHeight += 10;
				}
				else
				{
					CNewCopyShape->nY += 10 + nRandLocation;
					CNewCopyShape->nHeight += 10 + nRandLocation;
				}

				if (CNewCopyShape->nY < tmpRoomShape->nY)
				{
					CNewCopyShape->nY = tmpRoomShape->nY;
				}
				else if (CNewCopyShape->nHeight > tmpRoomShape->nHeight)
				{
					CNewCopyShape->nHeight = tmpRoomShape->nHeight;
				}
			}
			else if (CNewCopyShape->nY + (m_nDrawRange * 2) == CNewCopyShape->nHeight) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
			{
				//cout << "���Ʒ�\n";

				//////////////////////////////////////////////////////////////////////////
				// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

				if (type)
				{
					CNewCopyShape->nX += 10 + nRandLocation;
					CNewCopyShape->nWidth += 10 + nRandLocation;
				}
				else
				{
					CNewCopyShape->nX += 10;
					CNewCopyShape->nWidth += 10;
				}

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
				cout << "â��, �� ���� Copy Error\n";
			}

			GlobalNum::nPaintStatus = PAINT_DOOR; // Door ���� ���·� �ٲ�
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::nPaintStatus = PAINT_BASIC; // �⺻ ���·� �ٽ� �ǵ��ƿ�!
		}
		else if (typeid(*tmpShape) == typeid(WindowShape)) // Window�� �����ؼ� �����Ϸ� �� ��!
		{
			WindowShape *CNewCopyShape = new WindowShape(*dynamic_cast<WindowShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id�� �ٽ� �ο��ؾ� ��!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->m_pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_nDrawRange * 2) == CNewCopyShape->nWidth) //���� Ȥ�� �����ʿ� �ִٸ�
			{
				//cout << "�޿�\n";

				//////////////////////////////////////////////////////////////////////////
				// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

				if (type)
				{
					CNewCopyShape->nY += 10 + nRandLocation;
					CNewCopyShape->nHeight += 10 + nRandLocation;
				}
				else
				{
					CNewCopyShape->nY += 10;
					CNewCopyShape->nHeight += 10;
				}

				if (CNewCopyShape->nY < tmpRoomShape->nY)
				{
					CNewCopyShape->nY = tmpRoomShape->nY;
				}
				else if (CNewCopyShape->nHeight > tmpRoomShape->nHeight)
				{
					CNewCopyShape->nHeight = tmpRoomShape->nHeight;
				}
			}
			else if (CNewCopyShape->nY + (m_nDrawRange * 2) == CNewCopyShape->nHeight) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
			{
				//cout << "���Ʒ�\n";

				//////////////////////////////////////////////////////////////////////////
				// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������


				if (type)
				{
					CNewCopyShape->nX += 10 + nRandLocation;
					CNewCopyShape->nWidth += 10 + nRandLocation;
				}
				else 
				{
					CNewCopyShape->nX += 10;
					CNewCopyShape->nWidth += 10;
				}

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
				cout << "â��, �� ���� Copy Error\n";
			}

			GlobalNum::nPaintStatus = PAINT_WINDOW; // Window ���� ���·� �ٲ�
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::nPaintStatus = PAINT_BASIC; // �⺻ ���·� �ٽ� �ǵ��ƿ�!
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

	if (typeid(*tmpShape) == typeid(RoomShape)) // Room�� �����ؼ� Scale�Ϸ� �� ��!,  �׷�ȭ Scale ��� �� ��!
	{
		if (zDelta > 100) //ũ�� �� ��
		{
			if (tmpShape->nX < 5 || tmpShape->nWidth > 740 || tmpShape->nY < 5 || tmpShape->nHeight > 700)
			{
				return MY_ERROR;
			}

			bool bSuccess = TRUE;
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape);

			for (auto pIter : tmpRoomShape->m_CaDoor) //Room���� Door!
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //�����̸�
				{
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //�������̸�
				{
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //�����̸�
				{
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //�Ʒ����̸�
				{
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else
				{
					cout << " �� �׷� Wheel Error\n";
					bSuccess = FALSE;
					break;
				}
			}
			for (auto pIter : tmpRoomShape->m_CaWindow)
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //�����̸�
				{
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //�������̸�
				{
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //�����̸�
				{
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //�Ʒ����̸�
				{
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else
				{
					cout << " â�� �׷� Wheel Error\n";
					bSuccess = FALSE;
					break;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// ���� �� ũ�� ����, ���� ��������
			if (bSuccess)
			{
				tmpShape->nX -= 2;
				tmpShape->nWidth += 2;
				tmpShape->nY -= 2;
				tmpShape->nHeight += 2;
			}
			else
			{
				cout << " â��, �� �׷� ��ü Wheel Error\n";
			}
		}
		else //�۰� �� ��
		{
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape);
			bool bSuccess = TRUE;

			//////////////////////////////////////////////////////////////////////////
			// ��ü ũ�� üũ
			if (tmpShape->nWidth - tmpShape->nX < 30 || tmpShape->nHeight - tmpShape->nY < 30)
			{
				return MY_ERROR;
			}

			//////////////////////////////////////////////////////////////////////////
			// ���� �ϳ��� �ɸ��� �� �ִ��� Check �ؾ� ��!! �ϳ��� �̸� �۾����� �ý��� ������ ���� ����
			// �ƴϸ� ���߿� Undo Redo �����ؼ� �ǵ����� �ص� �� ��?
			for (auto pIter : tmpRoomShape->m_CaDoor) //Room���� Door!
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX || pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //����, ������
				{
					if (tmpRoomShape->nHeight <= pIter->nHeight  || tmpRoomShape->nY >= pIter->nY )
					{
						bSuccess = FALSE;
						break;
					}
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY || pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //����, �Ʒ���
				{
					if (tmpRoomShape->nWidth <= pIter->nWidth  || tmpRoomShape->nX >= pIter->nX )
					{
						bSuccess = FALSE;
						break;
					}
				}
				else
				{
					cout << "Mouse Wheel �۾����� Error" << endl;
					bSuccess = FALSE;
					break;
				}
			}
			for (auto pIter : tmpRoomShape->m_CaWindow)
			{
				if (pIter->nX + m_nDrawRange == tmpRoomShape->nX || pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //����, ������
				{
					if (tmpRoomShape->nHeight <= pIter->nHeight || tmpRoomShape->nY >= pIter->nY )
					{
						bSuccess = FALSE;
						break;
					}
				}
				else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY || pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //����, �Ʒ���
				{
					if (tmpRoomShape->nWidth <= pIter->nWidth || tmpRoomShape->nX >= pIter->nX)
					{
						bSuccess = FALSE;
						break;
					}
				}
				else
				{
					cout << "Mouse Wheel �۾����� Error" << endl;
					bSuccess = FALSE;
					break;
				}
			}

			if (!bSuccess)
			{
				cout << " â��, �� �׷� ��ü Wheel Error\n";
			}
			else
			{
				for (auto pIter : tmpRoomShape->m_CaDoor) //Room���� Door!
				{
					if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //�����̸�
					{
						pIter->nX += 2;
						pIter->nWidth += 2;
					}
					else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //�������̸�
					{
						pIter->nX -= 2;
						pIter->nWidth -= 2;
					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //�����̸�
					{
						pIter->nY += 2;
						pIter->nHeight += 2;
					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //�Ʒ����̸�
					{
						pIter->nY -= 2;
						pIter->nHeight -= 2;
					}
					else // ��� ������ �� üũ��
					{
						cout << "�� �׷� Wheel Error\n";
						bSuccess = FALSE;
						break;
					}
				}
				for (auto pIter : tmpRoomShape->m_CaWindow)
				{
					if (pIter->nX + m_nDrawRange == tmpRoomShape->nX) //�����̸�
					{
						pIter->nX += 2;
						pIter->nWidth += 2;
					}
					else if (pIter->nX + m_nDrawRange == tmpRoomShape->nWidth) //�������̸�
					{
						pIter->nX -= 2;
						pIter->nWidth -= 2;

					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nY) //�����̸�
					{
						pIter->nY += 2;
						pIter->nHeight += 2;
					}
					else if (pIter->nY + m_nDrawRange == tmpRoomShape->nHeight) //�Ʒ����̸�
					{
						pIter->nY -= 2;
						pIter->nHeight -= 2;
					}
					else // ��� ������ �� üũ��
					{
						cout << "â�� �׷� Wheel Error\n";
						bSuccess = FALSE;
						break;
					}
				}

				//////////////////////////////////////////////////////////////////////////
				// ���� �� ũ�� ����, ���� ��������
				tmpShape->nX += 2;
				tmpShape->nWidth -= 2;
				tmpShape->nY += 2;
				tmpShape->nHeight -= 2;

			}
		}
	}
	else if(typeid(*tmpShape) == typeid(DoorShape) || typeid(*tmpShape) == typeid(WindowShape)) // ���̳� â�� ���� ���� Scale
	{
		RoomShape *tmpRoomShape;

		if (typeid(*tmpShape) == typeid(DoorShape)) //���϶�
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->m_pInRoomShapePointer);
		}
		else	//â���϶�
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->m_pInRoomShapePointer);
		}


		if (tmpShape->nX + (m_nDrawRange * 2) == tmpShape->nWidth && (tmpShape->nX + m_nDrawRange == tmpRoomShape->nX) || (tmpShape->nX + m_nDrawRange == tmpRoomShape->nWidth)) //���� Ȥ�� �����ʿ� �ִٸ�
		{
			//cout << "�޿�\n";

			//////////////////////////////////////////////////////////////////////////
			// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
		else if (tmpShape->nY + (m_nDrawRange * 2) == tmpShape->nHeight && (tmpShape->nY + m_nDrawRange == tmpRoomShape->nY) || (tmpShape->nY + m_nDrawRange == tmpRoomShape->nHeight)) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
		{
			//cout << "���Ʒ�\n";

			//////////////////////////////////////////////////////////////////////////
			// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
			cout << "â��, �� ���� Copy Error\n";
		}
	}
	else //UserObject
	{
		if (zDelta > 100) //ũ�� �� ��
		{
			if (tmpShape->nX < 5 || tmpShape->nWidth > 740 || tmpShape->nY < 5 || tmpShape->nHeight > 700)
			{
				return MY_ERROR;
			}

			UserObjectShape *tmpRoomShape = dynamic_cast<UserObjectShape*>(tmpShape);

			tmpShape->nX -= 2;
			tmpShape->nWidth += 2;
			tmpShape->nY -= 2;
			tmpShape->nHeight += 2;		
		}
		else //�۰� �� ��
		{
			UserObjectShape *tmpRoomShape = dynamic_cast<UserObjectShape*>(tmpShape);

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
int ShapeHandler::MagneticSelectedShape()
{
	int index = GetCurrentSelectedIndex();
	if (index == MY_ERROR)
	{
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape[index];

	if (typeid(*tmpShape) == typeid(RoomShape)) //RoomShape Move�� �׷�ȭ �� ���� â���� ���� �������� ��!
	{
		RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape); 		//Room

		//��ü Shape�� ������ Shape�� �ִ���
		//�ڿ������� �˻��ؾ� ��!
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
		{
			if (tmpRoomShape == m_CaShape[i] || typeid(*m_CaShape[i]) != typeid(RoomShape)) // ���� ���ų� Room�� �ƴϸ� �ѱ�
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
			// �׸� �ϳ��� ���� �ϳ��� Range�� �����ϴ� �׸�� ���� ��
			// ���� ���������� ������ �����ذ�
			// �� -> �Ʒ� -> ���� -> ������ 
			// Select ���ؿ��� Magnetic Range�� ����� Any Shape�� �ټ� ��ǥ�� �� Range �ȿ� ���ٸ� �� �������� ����
			// 2. Select < Any
			// ���� ���� �˰������� �� �ݴ��� Any Shape�� ���ؿ��� Magnetic Range�� ����� ����
			// �� ���� �ȿ� Select Shape�� �ټ� ��ǥ�� ������ �Ȱ��� ������� ������ 

			MakeFiveReferenceVertex(nAnyReferenceX, m_CaShape[i]->nX, m_CaShape[i]->nWidth);
			MakeFiveReferenceVertex(nSelectedReferenceX, nShapeTempX, nShapeTempWidth);
			MakeFiveReferenceVertex(nAnyReferenceY, m_CaShape[i]->nY, m_CaShape[i]->nHeight);
			MakeFiveReferenceVertex(nSelectedReferenceY, nShapeTempY, nShapeTempHeight);

			// 5���� ������ ��� Magnetic Range�ȿ� ������ ã��
			for (int j = 0; j < 5; j++)
			{
				if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempY - m_nSelectRange <= m_CaShape[i]->nHeight && m_CaShape[i]->nHeight <= nShapeTempY + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nHeight - m_nSelectRange <= nShapeTempY && nShapeTempY <= m_CaShape[i]->nHeight + m_nSelectRange))
				{ // ��
					naDirectionControl[0]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempWidth - m_nSelectRange <= m_CaShape[i]->nX && m_CaShape[i]->nX <= nShapeTempWidth + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nX - m_nSelectRange <= nShapeTempWidth && nShapeTempWidth <= m_CaShape[i]->nX + m_nSelectRange))
				{ // ������
					naDirectionControl[1]++;
				}
				else if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempHeight - m_nSelectRange <= m_CaShape[i]->nY && m_CaShape[i]->nY <= nShapeTempHeight + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nY - m_nSelectRange <= nShapeTempHeight && nShapeTempHeight <= m_CaShape[i]->nY + m_nSelectRange))
				{ // �Ʒ�
					naDirectionControl[2]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempX - m_nSelectRange <= m_CaShape[i]->nWidth && m_CaShape[i]->nWidth <= nShapeTempX + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nWidth - m_nSelectRange <= nShapeTempX && nShapeTempX <= m_CaShape[i]->nWidth + m_nSelectRange))
				{ // ����
					naDirectionControl[3]++;
				}
			}

			// 3�� �̻� ������ ������ ����
			for (int j = 0; j < 4; j++)
			{
				if (naDirectionControl[j] >= 3)
				{
					int nSubMoving;
					switch (j)
					{
					case 0: //��
						//cout << "Mag ���� " << endl;
						nSubMoving = (tmpShape->nY - m_CaShape[i]->nHeight);
						tmpShape->nHeight -= nSubMoving; 
						tmpShape->nY = m_CaShape[i]->nHeight;

						/// �� ���� â���� ���� Update
						{
							//In Room Door
							for (auto CDoorShape : tmpRoomShape->m_CaDoor)
							{
								CDoorShape->nY -= nSubMoving;
								CDoorShape->nHeight -= nSubMoving;
							}
							//In Room Window
							for (auto CWindowShape : tmpRoomShape->m_CaWindow)
							{
								CWindowShape->nY -= nSubMoving;
								CWindowShape->nHeight -= nSubMoving;
							}
						}
						break;
					case 1: //������
						//cout << "Mag ������ " << endl;
						nSubMoving = (tmpShape->nWidth - m_CaShape[i]->nX);
						tmpShape->nX -= nSubMoving;
						tmpShape->nWidth = m_CaShape[i]->nX;

						/// �� ���� â���� ���� Update
						{
							//In Room Door
							for (auto CDoorShape : tmpRoomShape->m_CaDoor)
							{
								CDoorShape->nX -= nSubMoving;
								CDoorShape->nWidth -= nSubMoving;
							}
							//In Room Window
							for (auto CWindowShape : tmpRoomShape->m_CaWindow)
							{
								CWindowShape->nX -= nSubMoving;
								CWindowShape->nWidth -= nSubMoving;
							}
						}
						break;
					case 2: //�Ʒ�
						//cout << "Mag �Ʒ� " << endl;
						nSubMoving = (tmpShape->nHeight - m_CaShape[i]->nY);
						tmpShape->nY -= nSubMoving;
						tmpShape->nHeight = m_CaShape[i]->nY;

						/// �� ���� â���� ���� Update
						{
							//In Room Door
							for (auto CDoorShape : tmpRoomShape->m_CaDoor)
							{
								CDoorShape->nY -= nSubMoving;
								CDoorShape->nHeight -= nSubMoving;
							}
							//In Room Window
							for (auto CWindowShape : tmpRoomShape->m_CaWindow)
							{
								CWindowShape->nY -= nSubMoving;
								CWindowShape->nHeight -= nSubMoving;
							}
						}
						break;
					case 3: //����
						//cout << "Mag ���� " << endl;
						nSubMoving = (tmpShape->nX - m_CaShape[i]->nWidth);
						tmpShape->nWidth -= nSubMoving;
						tmpShape->nX = m_CaShape[i]->nWidth;

						/// �� ���� â���� ���� Update
						{
							//In Room Door
							for (auto CDoorShape : tmpRoomShape->m_CaDoor)
							{
								CDoorShape->nX -= nSubMoving;
								CDoorShape->nWidth -= nSubMoving;
							}
							//In Room Window
							for (auto CWindowShape : tmpRoomShape->m_CaWindow)
							{
								CWindowShape->nX -= nSubMoving;
								CWindowShape->nWidth -= nSubMoving;
							}
						}
						break;
					}
				}
			}
		}
	}
	else if (typeid(*tmpShape) == typeid(UserObjectShape))
	{
		UserObjectShape *tmpUserObjectShape = dynamic_cast<UserObjectShape*>(tmpShape); //UserObjectShape

	//��ü Shape�� ������ Shape�� �ִ���
	//�ڿ������� �˻��ؾ� ��!
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
		{
			if (tmpUserObjectShape == m_CaShape[i] || typeid(*m_CaShape[i]) != typeid(RoomShape)) // ���� ���ų� Room�� �ƴϸ� �ѱ�
			{
				continue;
			}

			int nShapeTempX = tmpUserObjectShape->nX;
			int nShapeTempWidth = tmpUserObjectShape->nWidth;
			int nShapeTempY = tmpUserObjectShape->nY;
			int nShapeTempHeight = tmpUserObjectShape->nHeight;

			int naDirectionControl[4] = { 0 };
			int nAnyReferenceX[5] = { 0 };
			int nSelectedReferenceX[5] = { 0 };
			int nAnyReferenceY[5] = { 0 };
			int nSelectedReferenceY[5] = { 0 };

			// 1. Select > Any
			// �׸� �ϳ��� ���� �ϳ��� Range�� �����ϴ� �׸�� ���� ��
			// ���� ���������� ������ �����ذ�
			// �� -> �Ʒ� -> ���� -> ������ 
			// Select ���ؿ��� Magnetic Range�� ����� Any Shape�� �ټ� ��ǥ�� �� Range �ȿ� ���ٸ� �� �������� ����
			// 2. Select < Any
			// ���� ���� �˰������� �� �ݴ��� Any Shape�� ���ؿ��� Magnetic Range�� ����� ����
			// �� ���� �ȿ� Select Shape�� �ټ� ��ǥ�� ������ �Ȱ��� ������� ������ 

			MakeFiveReferenceVertex(nAnyReferenceX, m_CaShape[i]->nX, m_CaShape[i]->nWidth);
			MakeFiveReferenceVertex(nSelectedReferenceX, nShapeTempX, nShapeTempWidth);
			MakeFiveReferenceVertex(nAnyReferenceY, m_CaShape[i]->nY, m_CaShape[i]->nHeight);
			MakeFiveReferenceVertex(nSelectedReferenceY, nShapeTempY, nShapeTempHeight);

			// 5���� ������ ��� Magnetic Range�ȿ� ������ ã��
			for (int j = 0; j < 5; j++)
			{
				if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempY - m_nSelectRange <= m_CaShape[i]->nHeight && m_CaShape[i]->nHeight <= nShapeTempY + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nHeight - m_nSelectRange <= nShapeTempY && nShapeTempY <= m_CaShape[i]->nHeight + m_nSelectRange))
				{ // ��
					naDirectionControl[0]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempWidth - m_nSelectRange <= m_CaShape[i]->nX && m_CaShape[i]->nX <= nShapeTempWidth + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nX - m_nSelectRange <= nShapeTempWidth && nShapeTempWidth <= m_CaShape[i]->nX + m_nSelectRange))
				{ // ������
					naDirectionControl[1]++;
				}
				else if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempHeight - m_nSelectRange <= m_CaShape[i]->nY && m_CaShape[i]->nY <= nShapeTempHeight + m_nSelectRange)
					|| (m_CaShape[i]->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= m_CaShape[i]->nWidth + m_nSelectRange) && (m_CaShape[i]->nY - m_nSelectRange <= nShapeTempHeight && nShapeTempHeight <= m_CaShape[i]->nY + m_nSelectRange))
				{ // �Ʒ�
					naDirectionControl[2]++;
				}
				else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempX - m_nSelectRange <= m_CaShape[i]->nWidth && m_CaShape[i]->nWidth <= nShapeTempX + m_nSelectRange)
					|| (m_CaShape[i]->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= m_CaShape[i]->nHeight + m_nSelectRange) && (m_CaShape[i]->nWidth - m_nSelectRange <= nShapeTempX && nShapeTempX <= m_CaShape[i]->nWidth + m_nSelectRange))
				{ // ����
					naDirectionControl[3]++;
				}
			}

			// 3�� �̻� ������ ������ ����
			for (int j = 0; j < 4; j++)
			{
				if (naDirectionControl[j] >= 3)
				{
					int nSubMoving;
					switch (j)
					{
					case 0: //��
						//cout << "Mag ���� " << endl;
						nSubMoving = (tmpShape->nY - m_CaShape[i]->nHeight);
						tmpShape->nHeight -= nSubMoving;
						tmpShape->nY = m_CaShape[i]->nHeight;

						break;
					case 1: //������
						//cout << "Mag ������ " << endl;
						nSubMoving = (tmpShape->nWidth - m_CaShape[i]->nX);
						tmpShape->nX -= nSubMoving;
						tmpShape->nWidth = m_CaShape[i]->nX;

						break;
					case 2: //�Ʒ�
						//cout << "Mag �Ʒ� " << endl;
						nSubMoving = (tmpShape->nHeight - m_CaShape[i]->nY);
						tmpShape->nY -= nSubMoving;
						tmpShape->nHeight = m_CaShape[i]->nY;

						break;
					case 3: //����
						//cout << "Mag ���� " << endl;
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


/// �̿ϼ�
int ShapeHandler::RotateSelectedShape()
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //���� ���þȵ�
	{
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape.at(index);	

	cout << endl << "Shape ���� ��� : " << endl;
	cout << "nX : " << to_string(tmpShape->nX) << endl;
	cout << "nY : " << to_string(tmpShape->nY) << endl;
	cout << "nWidth : " << to_string(tmpShape->nWidth) << endl;
	cout << "nHeight : " << to_string(tmpShape->nHeight) << endl;
	cout << endl << endl;


	return MY_SUCCES;
}