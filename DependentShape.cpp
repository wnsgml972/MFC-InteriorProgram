#include "stdafx.h"
#include "DependentShape.h"
#include "ShapeHandler.h"

CDependentShape::CDependentShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CComponentShape(nId, nX, nY, nWidth, nHeight)
{
	m_nLocaInfo = LOCA_DEFAULT;
	m_nType = TYPE_DEPENDENT;
}

CDependentShape::~CDependentShape()
{
}

// �⺻ ���
bool CDependentShape::AddShape()
{
	CShapeHandler *tmpShapeHandler = CShapeHandler::GetInstance();

	if (tmpShapeHandler->m_CaShape.size() == 0)
		return FALSE;

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

	// Copy �� �� �� ���� �Ǵ°��� ���� ��!
	if (m_nLocaInfo <= 0)
		m_nLocaInfo = tmpShapeHandler->m_RememberLocationForDoorWindow;
	if (m_pInContainerShapeID < 0)
		m_pInContainerShapeID = tmpShapeHandler->m_pRememberRoomIDForDoorWindow; // �ڽ��� �����ִ� Room�� ID

	/// ����!
	tmpShapeHandler->m_CaShape.push_back(this);
	dynamic_cast<CBasicContainerShape*>(tmpShapeHandler->GetShapeByID(m_pInContainerShapeID))->m_CaDependent.push_back(this);

	return TRUE;
}

bool CDependentShape::DeleteShape()
{
	// ���õ� �� ���� ���� ���͸� �޾� ��
	// �ּ� ���� �޾ƾ���!!!! ���� �ִ� ���ʹ� ������ ������ �ƴ� �� ����� ���� ��!
	auto tmpDependentPtr = &dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID))->m_CaDependent;

	// ���õ� ���� ������, �� �� �ڽ�
	auto tmpSelectedDoorPtr = this;

	tmpSelectedDoorPtr->SetId(-600000);

	//////////////////////////////////////////////////////////////////////////
	//  ��ü CShape ���Ϳ����� ���̵� ���� ������ ���� ���� ����
	for (long i = CShapeHandler::GetInstance()->m_CaShape.size() - 1; i >= 0; i--)
	{
		int nWasteValue = CShapeHandler::GetInstance()->m_CaShape[i]->GetId();
		if (nWasteValue < -50000)
		{
			CShapeHandler::GetInstance()->m_CaShape.erase(CShapeHandler::GetInstance()->m_CaShape.begin() + i);
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Dependent ���Ϳ����� ���̵� ���� ������ ���� ���� ����
	for (long i = tmpDependentPtr->size() - 1; i >= 0; i--)
	{

		int nWasteValue = tmpDependentPtr->at(i)->GetId();
		if (nWasteValue < -50000)
		{
			tmpDependentPtr->erase(tmpDependentPtr->begin() + i);
			break;
		}
	}

	SAFE_DELETE(tmpSelectedDoorPtr);//  ���õ� �� ����, delete

	return TRUE;
}

bool CDependentShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;
	CBasicContainerShape *tmpBasicContainerShape = dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));


	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //���� Ȥ�� �����ʿ� �ִٸ�
	{
		// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		nY += 10 + nRandLocation;
		nHeight += 10 + nRandLocation;

		if (nY < tmpBasicContainerShape->nY)
		{
			nY = tmpBasicContainerShape->nY;
		}
		else if (nHeight > tmpBasicContainerShape->nHeight)
		{
			nHeight = tmpBasicContainerShape->nHeight;
		}
	}
	else if (GetLocaInfo() == LOCA_DOWN || GetLocaInfo() == LOCA_UP) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
	{
		// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		nX += 10 + nRandLocation;
		nWidth += 10 + nRandLocation;


		if (nX < tmpBasicContainerShape->nX)
		{
			nX = tmpBasicContainerShape->nX;
		}
		else if (nWidth > tmpBasicContainerShape->nWidth)
		{
			nWidth = tmpBasicContainerShape->nWidth;
		}
	}
	else
	{
		cout << "â��, �� ���� Copy Error\n";
		return FALSE;
	}

	SetOwnColor();
	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);


	return TRUE;
}

bool CDependentShape::MoveShape(CPoint point)
{
	// â�� ���� Ŭ������ ��!!!

	CBasicContainerShape *tmpBasicContainerShape = dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));
	int tmpX, tmpY, tmpWidth, tmpHeight;
	 
	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //���� Ȥ�� �����ʿ� �ִٸ�
	{
		//cout << "�޿�\n";

		//////////////////////////////////////////////////////////////////////////
		// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		tmpY = point.y + m_nMoveSubVal[1];
		tmpHeight = point.y + m_nMoveSubVal[3];

		if (tmpY <= tmpBasicContainerShape->nY || tmpHeight >= tmpBasicContainerShape->nHeight)
		{
			return FALSE;
		}
		else
		{
			nY = tmpY;
			nHeight = tmpHeight;
		}
	}
	else if (GetLocaInfo() == LOCA_UP || GetLocaInfo() == LOCA_DOWN) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
	{
		//cout << "���Ʒ�\n";

		//////////////////////////////////////////////////////////////////////////
		// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		tmpX = point.x + m_nMoveSubVal[0];
		tmpWidth = point.x + m_nMoveSubVal[2];

		if (tmpX <= tmpBasicContainerShape->nX || tmpWidth >= tmpBasicContainerShape->nWidth)
		{
			return FALSE;
		}
		else
		{
			nX = tmpX;
			nWidth = tmpWidth;
		}
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	return TRUE;
}

bool CDependentShape::MagneticShape() // ��� ���׳�ƽ�� �ʿ� ����!
{
	return FALSE;
}

bool CDependentShape::EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus)
{
	CBasicContainerShape *tmpBasicContainerShape = dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));;
	int tmpSub = MY_ERROR;


	if (GetLocaInfo() == LOCA_UP || GetLocaInfo() == LOCA_DOWN)
	{
		if (m_nSubShapeStatus[0] != MY_ERROR) //change X
		{
			if (nX <= tmpBasicContainerShape->nX) //�۾��� ��
			{
				tmpSub = m_nRememberShapeStatus[0] - tmpBasicContainerShape->nX;
				nX = tmpBasicContainerShape->nX;
				nWidth = m_nRememberShapeStatus[0] + m_nRememberShapeStatus[2] - tmpSub;
			}
			if (nX + m_nRememberShapeStatus[2] >= tmpBasicContainerShape->nWidth)  //Ŀ�� ��
			{
				tmpSub = tmpBasicContainerShape->nWidth - (m_nRememberShapeStatus[0] + m_nRememberShapeStatus[2]);
				nWidth = tmpBasicContainerShape->nWidth;
				nX = m_nRememberShapeStatus[0] + tmpSub;
			}
		}
		if (m_nSubShapeStatus[2] != MY_ERROR) //change Width
		{
			if (nWidth >= tmpBasicContainerShape->nWidth)
				nWidth = tmpBasicContainerShape->nWidth;
		}
	}
	else if (GetLocaInfo() == LOCA_RIGHT || GetLocaInfo() == LOCA_LEFT)
	{
		if (m_nSubShapeStatus[1] != MY_ERROR) //change Y
		{
			if (nY <= tmpBasicContainerShape->nY) //�۾��� ��
			{
				tmpSub = m_nRememberShapeStatus[1] - tmpBasicContainerShape->nY;
				nY = tmpBasicContainerShape->nY;
				nHeight = m_nRememberShapeStatus[1] + m_nRememberShapeStatus[3] - tmpSub;
			}
			if (nY + m_nRememberShapeStatus[3] >= tmpBasicContainerShape->nHeight)  //Ŀ�� ��
			{
				tmpSub = tmpBasicContainerShape->nHeight - (m_nRememberShapeStatus[1] + m_nRememberShapeStatus[3]);
				nHeight = tmpBasicContainerShape->nHeight;
				nY = m_nRememberShapeStatus[1] + tmpSub;
			}
		}
		if (m_nSubShapeStatus[3] != MY_ERROR) //change Height
		{
			if (nHeight >= tmpBasicContainerShape->nHeight)
				nHeight = tmpBasicContainerShape->nHeight;
		}
	}

	SetRect(nX, nY, nWidth, nHeight);

	return TRUE;
}

bool CDependentShape::WheelShape(short zDelta)
{
	// ���� �����ִ� ��!
	CBasicContainerShape *tmpBasicContainerShape =
		dynamic_cast<CBasicContainerShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));

	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //���� Ȥ�� �����ʿ� �ִٸ�
	{
		//////////////////////////////////////////////////////////////////////////
		// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		if (zDelta > 100)
		{
			if (!(nY - 2 < tmpBasicContainerShape->nY))
			{
				nY -= 2;
			}
			if (!(nHeight + 2 > tmpBasicContainerShape->nHeight))
			{
				nHeight += 2;
			}
		}
		else
		{
			if (!(nHeight - nY < 28))
			{
				nY += 2;
				nHeight -= 2;
			}
		}
	}
	else if (GetLocaInfo() == LOCA_UP || GetLocaInfo() == LOCA_DOWN) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
	{
		//////////////////////////////////////////////////////////////////////////
		// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		if (zDelta > 100)
		{
			if (!(nX - 2 < tmpBasicContainerShape->nX))
			{
				nX -= 2;
			}
			if (!(nWidth + 2 > tmpBasicContainerShape->nWidth))
			{
				nWidth += 2;
			}
		}
		else
		{
			if (!(nWidth - nX < 28))
			{
				nX += 2;
				nWidth -= 2;
			}
		}
	}
	else
	{
		cout << "â��, �� ���� Copy Error\n";
		return FALSE;
	}

	return TRUE;
}

// �߰� ���
int CDependentShape::GetLocaInfo()
{
	return m_nLocaInfo;
}