#include "stdafx.h"
#include "WindowShape.h"
#include "RoomShape.h"
#include "ShapeHandler.h"

CWindowShape::CWindowShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CShape(nId, nX, nY, nWidth, nHeight)
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
	m_nLocaInfo = 0; //LOCA_DEFAULT
}

CWindowShape::~CWindowShape()
{
	printf("~WindowShape\n");
}

bool CWindowShape::AddShape()
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
	if (m_pInRoomShapeID < 0)
		m_pInRoomShapeID = tmpShapeHandler->m_pRememberRoomIDForDoorWindow; // �ڽ��� �����ִ� Room�� ID


	/// ����!
	tmpShapeHandler->m_CaShape.push_back(this);	
	dynamic_cast<CRoomShape*>(tmpShapeHandler->GetShapeByID(m_pInRoomShapeID))->m_CaWindow.push_back(this);

	return TRUE;
}

void CWindowShape::SetOwnColor()
{
	nRed = 80;
	nGreen = 188;
	nBlue = 223;
}

int CWindowShape::GetLocaInfo()
{
	return m_nLocaInfo;
}

bool CWindowShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;
	CRoomShape *tmpRoomShape = dynamic_cast<CRoomShape*>(CShapeHandler::GetInstance()->GetShapeByID(m_pInRoomShapeID));


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
