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
		// ������ �˻�
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
		// ������ �˻�
		if (nX > nWidth)
		{
			swap(nX, nWidth);
		}
		if (nY > nHeight)
		{
			swap(nY, nHeight);
		}

		DoorShape *CDoorShape = new DoorShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);

		CDoorShape->pInRoomShapePointer = m_pRememberRoomPtrForDoorWindow; // �ڽ��� �����ִ� Room�� ������


		/// ����!
		m_CaShape.push_back(CDoorShape);
		dynamic_cast<RoomShape*>(CDoorShape->pInRoomShapePointer)->m_CaDoor.push_back(CDoorShape);
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_WINDOW)
	{
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

		CWindowShape->pInRoomShapePointer = m_pRememberRoomPtrForDoorWindow; // �ڽ��� �����ִ� Room�� ������

		/// ����!
		m_CaShape.push_back(CWindowShape);
		dynamic_cast<RoomShape*>(CWindowShape->pInRoomShapePointer)->m_CaWindow.push_back(CWindowShape);
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
		RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(m_CaShape.at(index));

		// �ؿ�ó�� �ٸ� �� ���ͳ� â�� ����ó�� �ѹ��� �ص� ������ �� �˾ƺ��� ���� ���� �׳� ���� �ϳ� ���س�
		//Room
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
	else if (typeid(*tmpShape) == typeid(DoorShape) || typeid(*tmpShape) == typeid(WindowShape))
	{
		// â�� ���� Ŭ������ ��!!!

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
			cout << "Move �� ��ȯ Error\n";
		}

		if (tmpShape->nX + m_nDrawRange == tmpRoomShape->nX || tmpShape->nWidth - m_nDrawRange == tmpRoomShape->nWidth) //���� Ȥ�� �����ʿ� �ִٸ�
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
		else if (tmpShape->nY + m_nDrawRange == tmpRoomShape->nY || tmpShape->nHeight - m_nDrawRange == tmpRoomShape->nHeight) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
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
		auto tmpDoorPtr = &dynamic_cast<RoomShape *>(dynamic_cast<DoorShape*>(m_CaShape[nSelectedIndex])->pInRoomShapePointer)->m_CaDoor;
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
		auto tmpWindowPtr = &dynamic_cast<RoomShape *>(dynamic_cast<WindowShape*>(m_CaShape[nSelectedIndex])->pInRoomShapePointer)->m_CaWindow;
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

		if (typeid(*tmpShape) == typeid(RoomShape)) // Room�� �����ؼ� �����Ϸ� �� ��!,  �׷�ȭ ���� ��� �� ��!
		{
			RoomShape *CNewCopyShape = new RoomShape(*dynamic_cast<RoomShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id�� �ٽ� �ο��ؾ� ��!

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

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_ROOM; // Room ���� ���·� �ٲ�
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // �⺻ ���·� �ٽ� �ǵ��ƿ�!
		}
		else if (typeid(*tmpShape) == typeid(DoorShape)) // Door�� �����ؼ� �����Ϸ� �� ��!
		{
			DoorShape *CNewCopyShape = new DoorShape(*dynamic_cast<DoorShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id�� �ٽ� �ο��ؾ� ��!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_nDrawRange * 2) == CNewCopyShape->nWidth) //���� Ȥ�� �����ʿ� �ִٸ�
			{
				//cout << "�޿�\n";

				//////////////////////////////////////////////////////////////////////////
				// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
			else if (CNewCopyShape->nY + (m_nDrawRange * 2) == CNewCopyShape->nHeight) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
			{
				//cout << "���Ʒ�\n";

				//////////////////////////////////////////////////////////////////////////
				// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
				cout << "â��, �� ���� Copy Error\n";
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_DOOR; // Door ���� ���·� �ٲ�
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // �⺻ ���·� �ٽ� �ǵ��ƿ�!
		}
		else if (typeid(*tmpShape) == typeid(WindowShape)) // Window�� �����ؼ� �����Ϸ� �� ��!
		{
			WindowShape *CNewCopyShape = new WindowShape(*dynamic_cast<WindowShape*>(tmpShape));
			CNewCopyShape->SetId(MakeAutoIncId()); // Id�� �ٽ� �ο��ؾ� ��!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_nDrawRange * 2) == CNewCopyShape->nWidth) //���� Ȥ�� �����ʿ� �ִٸ�
			{
				//cout << "�޿�\n";

				//////////////////////////////////////////////////////////////////////////
				// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
			else if (CNewCopyShape->nY + (m_nDrawRange * 2) == CNewCopyShape->nHeight) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
			{
				//cout << "���Ʒ�\n";

				//////////////////////////////////////////////////////////////////////////
				// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

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
				cout << "â��, �� ���� Copy Error\n";
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_WINDOW; // Window ���� ���·� �ٲ�
			AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // �⺻ ���·� �ٽ� �ǵ��ƿ�!
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
	else // ���̳� â�� ���� ���� Scale
	{
		RoomShape *tmpRoomShape;

		if (typeid(*tmpShape) == typeid(DoorShape)) //���϶�
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->pInRoomShapePointer);
		}
		else	//â���϶�
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->pInRoomShapePointer);
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