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
		
		CDoorShape->pInRoomShapePointer = m_pRememberIndexForDoorWindowPointer; // �ڽ��� �����ִ� Room�� ������


		/// ����!
		m_CaShape.push_back(CDoorShape);
		dynamic_cast<RoomShape*>(CDoorShape->pInRoomShapePointer)->m_CaDoor.push_back(CDoorShape);
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_WINDOW)
	{
		WindowShape *CWindowShape = new WindowShape(m_nAutoIncId++, nX, nY, nWidth, nHeight);

		CWindowShape->pInRoomShapePointer = m_pRememberIndexForDoorWindowPointer; // �ڽ��� �����ִ� Room�� ������

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
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		//Room�� �ƴϸ� Continue
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
			OldMousePoint.y = m_CaShape.at(i)->nY -m_nDrawRange;
			CurrentMousePoint.y = m_CaShape.at(i)->nY +m_nDrawRange;
			
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
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

			m_pRememberIndexForDoorWindowPointer = m_CaShape.at(i);
			break;
		}
		else
		{
			if (!bDragFlag) //�巡�� ���� �ƴ� ����
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

	if (index == MY_ERROR) //���� ���þȵ�
	{
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape.at(index);

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
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //���� ���þȵ�
	{
		return MY_ERROR;
	}

	Shape *tmpShape = m_CaShape.at(index);

	return MY_SUCCES;
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


