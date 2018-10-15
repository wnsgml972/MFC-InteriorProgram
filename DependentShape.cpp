#include "stdafx.h"
#include "DependentShape.h"
#include "ShapeHandler.h"

CDependentShape::CDependentShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CComponentShape(nId, nX, nY, nWidth, nHeight)
{
	m_nLocaInfo = LOCA_DEFAULT;
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
	dynamic_cast<CRoomShape*>(tmpShapeHandler->GetShapeByID(m_pInContainerShapeID))->m_CaDependent.push_back(this);

	return TRUE;
}

bool CDependentShape::DeleteShape()
{
	return false;
}

bool CDependentShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;
	CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInContainerShapeID));


	if (GetLocaInfo() == LOCA_LEFT || GetLocaInfo() == LOCA_RIGHT) //���� Ȥ�� �����ʿ� �ִٸ�
	{
		// X ���� �����ϰ�, Y�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		nY += 10 + nRandLocation;
		nHeight += 10 + nRandLocation;

		if (nY < tmpRoomShape->nY)
		{
			nY = tmpRoomShape->nY;
		}
		else if (nHeight > tmpRoomShape->nHeight)
		{
			nHeight = tmpRoomShape->nHeight;
		}
	}
	else if (GetLocaInfo() == LOCA_DOWN || GetLocaInfo() == LOCA_UP) //���� Ȥ�� �Ʒ��ʿ� �ִٸ�
	{
		// Y ���� �����ϰ�, X�ุ RoomShape�� ���� ���� ������ �ʰ� ������

		nX += 10 + nRandLocation;
		nWidth += 10 + nRandLocation;


		if (nX < tmpRoomShape->nX)
		{
			nX = tmpRoomShape->nX;
		}
		else if (nWidth > tmpRoomShape->nWidth)
		{
			nWidth = tmpRoomShape->nWidth;
		}
	}
	else
	{
		cout << "â��, �� ���� Copy Error\n";
		return FALSE;
	}

	SetOwnColor();
	SetId(CShapeHandler::GetInstance()->MakeAutoIncId());
	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);


	return TRUE;
}

bool CDependentShape::MoveShape()
{
	return false;
}

bool CDependentShape::MagneticShape()
{
	return false;
}

bool CDependentShape::EditShape()
{
	return false;
}

bool CDependentShape::WheelShape()
{
	return false;
}

// �߰� ���
int CDependentShape::GetLocaInfo()
{
	return m_nLocaInfo;
}