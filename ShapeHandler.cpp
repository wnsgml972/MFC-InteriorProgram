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
	 - enum�� ���� �й� �ڵ� �������� ��! ���� �Լ��� �̿��� �й��Ѵ�.
	   �̰͸� �θ��� �˾Ƽ� �� ��������
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
/// ���
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
1. MouseDown �� ���õ� Shape�� ���� �������� Ŭ���� point���� �������� ���س��´�. (���콺�� �� �� ����)
2. MouseMove���� ���س��� ���̿� ���� �巡�� �Ǵ� ��ǥ���� �����ش�!,
		1) OnPaint���� InitVector�� ���� ����ؼ� ���Ϳ� �ִ� �ֵ��� �׸��� �ִ�.
		2) �׷��Ƿ� Vector�ȿ� �ִ� Shape�� ��ǥ���� �ٲ��ָ� �ڵ����� �ٲ� ��ġ���� �׷����� �ȴ�!
		3) �츮�� Vector���� Shape ��ǥ ���� MouseMove���� ��� �ٲ��ָ� OnPaint���� �˾Ƽ� �׷��ش�.
3. �� ���� InteriorProgramView�� ������ �� �ȴ�.
4. �� �� ���α׷� �� ���� ����ó��
*/
void CShapeHandler::Move(CPoint point) 
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //���� ���þȵ�
	{
		return;
	}

	// �ܼ��� ���� ���� �� ������ �����δ�.
	// ������ Move�� �� �ش��ϴ� Type�� Ŭ������ �����Ǿ� ����
	m_CaShape.at(index)->MoveShape(point);

}
void CShapeHandler::Select(CPoint point)
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
void CShapeHandler::SetDoorWindowRange(bool bDragFlag, CPoint &OldMousePoint, CPoint &CurrentMousePoint)
{
	// ���� �׷����� �� Room ������ ã��! ,���� �ڿ������� �˻���� ��!  z-index

	bool bAllNoTouch = TRUE; // ��� ������ ������ ����

#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		//Room�� �ƴϸ� Continue, Room�ȿ��� Door�� Window�� ã�ƾ� �ϱ� ����
		if (!(m_CaShape.at(i)->m_nType == TYPE_BASIC_CONTAINER))
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
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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
			m_pRememberRoomIDForDoorWindow = m_CaShape[i]->GetId();
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
			// ��� ������ select state ���¸� FALSE�� �ٲ�
			m_CaShape.at(i)->bSelectedState = FALSE;
			
			// ��� ������ ���� �ڽ��� ����� ���ư�
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

	if (index == MY_ERROR) //���� ���þȵ�
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

	if (nSelectedIndex == MY_ERROR) //���� ���þȵ�
	{
		return MY_ERROR;
	}

	// ���� Room �̶�� �� �ȿ� �����ϴ� Door�� Window ��� ���Ϳ��� ���� �����ؾ� ��
	//////////////////////////////////////////////////////////////////////////
	// [ �׷�ȭ ���� �˰��� ] + [ ���� ��� ���� �˰��� ] ���� Shape�� ����
	m_CaShape.at(nSelectedIndex)->DeleteShape();

	return MY_SUCCES;
}
int CShapeHandler::CopySelectedShape(int nIndex)
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;

	int index; //Copy, Paste ������ �̷��� ����
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

	// ���õ� ������ Wheel �Ѵ�.
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

	// ���� �� ������ Magnetic!
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

/// �̿ϼ�
int CShapeHandler::RotateSelectedShape()
{
	int index = GetCurrentSelectedIndex();

	if (index == MY_ERROR) //���� ���þȵ�
	{
		return MY_ERROR;
	}

	cout << endl << "���� ���� ��� : " << endl;
	cout << "��ü Size : " << to_string(m_CaShape.size()) << endl;
	cout << "���õ� �� �� : " << to_string(HowManySelected()) << endl;
	cout << endl << endl;


	return MY_SUCCES;
}