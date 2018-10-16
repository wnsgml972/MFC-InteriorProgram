#include "stdafx.h"
#include "BasicContainerShape.h"
#include "ShapeHandler.h"

CBasicContainerShape::CBasicContainerShape(int nId, int nX, int nY, int nWidth, int nHeight)
	: CContainerShape(nId,  nX, nY, nWidth, nHeight)
{
	m_nType = TYPE_BASIC_CONTAINER;
}

CBasicContainerShape::~CBasicContainerShape()
{
	
}

bool CBasicContainerShape::AddShape()
{
	CShapeHandler *tmpShapeHandler = CShapeHandler::GetInstance();

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
	tmpShapeHandler->m_CaShape.push_back(this);
	return TRUE;
}

bool CBasicContainerShape::DeleteShape()
{
	auto tmpInRoomPtr = this; //���� �ڽ��� �����͸� �޾Ƶ�
	tmpInRoomPtr->SetId(-600000);

	//////////////////////////////////////////////////////////////////////////
	// ���� ����� Dependent ���͸� ��� �����Ҵ� ���� �� ����
	for (auto nIdx : m_CaDependent)
	{
		nIdx->SetId(-600000);
		SAFE_DELETE(nIdx);
	}
	m_CaDependent.clear();

	//////////////////////////////////////////////////////////////////////////
	// ��ü Shape ���Ϳ����� ���̵� ���� ������ ���� ���� ���� ��, �����Ҵ� ����
	for (long i = CShapeHandler::GetInstance()->m_CaShape.size() - 1; i >= 0; i--)
	{
		int nWasteValue = CShapeHandler::GetInstance()->m_CaShape[i]->GetId();
		if (nWasteValue < -50000)
		{
			CShapeHandler::GetInstance()->CShapeHandler::GetInstance()->m_CaShape.erase(CShapeHandler::GetInstance()->m_CaShape.begin() + i);
		}
	}
	SAFE_DELETE(tmpInRoomPtr);

	return TRUE;
}

bool CBasicContainerShape::CopyShape()
{
	int nRandLocation = static_cast<int>((rand() % 15)) + 1;

	nX += 10 + nRandLocation;
	nWidth += 10 + nRandLocation;
	nY += 10 + nRandLocation;
	nHeight += 10 + nRandLocation;

	SetOwnColor();
	CShapeHandler::GetInstance()->SetRange(nX, nY, nWidth, nHeight);
	CShapeHandler::GetInstance()->AddShape(this);

	return TRUE;
}

bool CBasicContainerShape::MoveShape(CPoint point)
{
	int tmpX, tmpY, tmpWidth, tmpHeight;


// �ؿ�ó�� �ٸ� �� ���ͳ� â�� ����ó�� �ѹ��� �ص� ������ �� �˾ƺ��� ���� ���� �׳� ���� �ϳ� ���س�
	tmpX = point.x + m_nMoveSubVal[0];
	tmpY = point.y + m_nMoveSubVal[1];
	tmpWidth = point.x + m_nMoveSubVal[2];
	tmpHeight = point.y + m_nMoveSubVal[3];

	// â���̳� ���� �׷����� �� DrawRange ���� �����ε� �������� �� ��!!
	if (tmpX < 0 + CShapeHandler::GetInstance()->m_nDrawSelectRange || tmpWidth > 765 - CShapeHandler::GetInstance()->m_nDrawSelectRange || 
		tmpY < 0 + CShapeHandler::GetInstance()->m_nDrawSelectRange || tmpHeight > 720 - CShapeHandler::GetInstance()->m_nDrawSelectRange) // �� ���� ���� ��
	{
		return FALSE;
	}
	else
	{
		nX = tmpX;
		nY = tmpY;
		nWidth = tmpWidth;
		nHeight = tmpHeight;

	}

	// �濡 ���� ���� ���� �ؾ��ϴ� �� �� Ŭ�� �� ���� ������ ������ �ٸ� â���̳� ���� �����̸� �� �ǹǷ�
	// ������ ���� üũ�ϰ� �����ع����� ��
	// �� ���� ����
	{
		//In Room Door, Window
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = 0; i < m_CaDependent.size(); i++)
#pragma warning(pop)
		{
			// �� ���� ���� ��
			if (point.x + m_CaDependent[i]->m_nMoveSubVal[0] < 0 || point.x + m_CaDependent[i]->m_nMoveSubVal[2] > 765 ||
				point.y + m_CaDependent[i]->m_nMoveSubVal[1] < 0 || point.y + m_CaDependent[i]->m_nMoveSubVal[3] > 720)
			{
				return FALSE;
			}
			else
			{
				m_CaDependent[i]->nX = point.x + m_CaDependent[i]->m_nMoveSubVal[0];
				m_CaDependent[i]->nY = point.y + m_CaDependent[i]->m_nMoveSubVal[1];
				m_CaDependent[i]->nWidth = point.x + m_CaDependent[i]->m_nMoveSubVal[2];
				m_CaDependent[i]->nHeight = point.y + m_CaDependent[i]->m_nMoveSubVal[3];
			}
		}
	}

	return TRUE;
}

bool CBasicContainerShape::MagneticShape() //CRoomShape Move�� �׷�ȭ �� ���� â���� ���� �������� ��!
{
	int m_nSelectRange = CShapeHandler::GetInstance()->m_nSelectRange;

	//��ü Shape�� ������ Shape�� �ִ���
	//�ڿ������� �˻��ؾ� ��!
#pragma warning(push)
#pragma warning(disable: 4018)
	for (long i = CShapeHandler::GetInstance()->m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		auto tmpIndexShape = CShapeHandler::GetInstance()->m_CaShape[i];

		if (this == tmpIndexShape || tmpIndexShape->m_nType != TYPE_BASIC_CONTAINER) // ���� ���ų� Room�� �ƴϸ� �ѱ�
		{
			continue;
		}
		int nShapeTempX = nX;
		int nShapeTempWidth = nWidth;
		int nShapeTempY = nY;
		int nShapeTempHeight = nHeight;

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

		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nAnyReferenceX, tmpIndexShape->nX, tmpIndexShape->nWidth);
		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nSelectedReferenceX, nShapeTempX, nShapeTempWidth);
		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nAnyReferenceY, tmpIndexShape->nY, tmpIndexShape->nHeight);
		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nSelectedReferenceY, nShapeTempY, nShapeTempHeight);

		// 5���� ������ ��� Magnetic Range�ȿ� ������ ã��
		for (int j = 0; j < 5; j++)
		{
			if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempY - m_nSelectRange <= tmpIndexShape->nHeight && tmpIndexShape->nHeight <= nShapeTempY + m_nSelectRange)
				|| (tmpIndexShape->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= tmpIndexShape->nWidth + m_nSelectRange) && (tmpIndexShape->nHeight - m_nSelectRange <= nShapeTempY && nShapeTempY <= tmpIndexShape->nHeight + m_nSelectRange))
			{ // ��
				naDirectionControl[0]++;
			}
			else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempWidth - m_nSelectRange <= tmpIndexShape->nX && tmpIndexShape->nX <= nShapeTempWidth + m_nSelectRange)
				|| (tmpIndexShape->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= tmpIndexShape->nHeight + m_nSelectRange) && (tmpIndexShape->nX - m_nSelectRange <= nShapeTempWidth && nShapeTempWidth <= tmpIndexShape->nX + m_nSelectRange))
			{ // ������
				naDirectionControl[1]++;
			}
			else if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempHeight - m_nSelectRange <= tmpIndexShape->nY && tmpIndexShape->nY <= nShapeTempHeight + m_nSelectRange)
				|| (tmpIndexShape->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= tmpIndexShape->nWidth + m_nSelectRange) && (tmpIndexShape->nY - m_nSelectRange <= nShapeTempHeight && nShapeTempHeight <= tmpIndexShape->nY + m_nSelectRange))
			{ // �Ʒ�
				naDirectionControl[2]++;
			}
			else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempX - m_nSelectRange <= tmpIndexShape->nWidth && tmpIndexShape->nWidth <= nShapeTempX + m_nSelectRange)
				|| (tmpIndexShape->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= tmpIndexShape->nHeight + m_nSelectRange) && (tmpIndexShape->nWidth - m_nSelectRange <= nShapeTempX && nShapeTempX <= tmpIndexShape->nWidth + m_nSelectRange))
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
					nSubMoving = (nY - tmpIndexShape->nHeight);
					nHeight -= nSubMoving;
					nY = tmpIndexShape->nHeight;

					/// �� ���� â���� ���� Update
					{
						//In Room Dependent
						for (auto DependentShape :m_CaDependent)
						{
							DependentShape->nY -= nSubMoving;
							DependentShape->nHeight -= nSubMoving;
						}
					}
					break;
				case 1: //������
					//cout << "Mag ������ " << endl;
					nSubMoving = (nWidth - tmpIndexShape->nX);
					nX -= nSubMoving;
					nWidth = tmpIndexShape->nX;

					/// �� ���� â���� ���� Update
					{
						//In Room Dependent
						for (auto DependentShape : m_CaDependent)
						{
							DependentShape->nX -= nSubMoving;
							DependentShape->nWidth -= nSubMoving;
						}
					}
					break;
				case 2: //�Ʒ�
					//cout << "Mag �Ʒ� " << endl;
					nSubMoving = (nHeight - tmpIndexShape->nY);
					nY -= nSubMoving;
					nHeight = tmpIndexShape->nY;

					/// �� ���� â���� ���� Update
					{
						//In Room Dependent
						for (auto DependentShape : m_CaDependent)
						{
							DependentShape->nY -= nSubMoving;
							DependentShape->nHeight -= nSubMoving;
						}
					}
					break;
				case 3: //����
					//cout << "Mag ���� " << endl;
					nSubMoving = (nX - tmpIndexShape->nWidth);
					nWidth -= nSubMoving;
					nX = tmpIndexShape->nWidth;

					/// �� ���� â���� ���� Update
					{
						//In Room Dependent
						for (auto DependentShape :m_CaDependent)
						{
							DependentShape->nX -= nSubMoving;
							DependentShape->nWidth -= nSubMoving;
						}
					}
					break;
				}
			}
		}
	}

	return TRUE;
}

bool CBasicContainerShape::EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus)
{
	int tmpSub = MY_ERROR;

	// Basic Container�� ������ �ִ� Dependent �˻�
	for (auto pIter : m_CaDependent)
	{
		int m_nLocaInfo = pIter->GetLocaInfo();

		if (m_nSubShapeStatus[0] != MY_ERROR) //change X
		{
			pIter->nX -= m_nSubShapeStatus[0];
			pIter->nWidth -= m_nSubShapeStatus[0];
		}
		if (m_nSubShapeStatus[1] != MY_ERROR) //change Y
		{
			pIter->nY -= m_nSubShapeStatus[1];
			pIter->nHeight -= m_nSubShapeStatus[1];
		}
		if (m_nSubShapeStatus[2] != MY_ERROR) //change Width
		{
			//////////////////////////////////////////////////////////////////////////
			/// tmpSub �� ���Ƽ� ���� ���ϱ�
			for (auto pTmpIter : m_CaDependent)
			{
				int tmpLoca = pTmpIter->GetLocaInfo();

				if (tmpLoca == LOCA_DOWN || tmpLoca == LOCA_UP)
				{
					if (nWidth <= pTmpIter->nWidth)
					{
						nWidth = pTmpIter->nWidth;
						tmpSub = m_nRememberShapeStatus[2] + m_nRememberShapeStatus[0] - nWidth;
					}
				}
			}

			if (m_nLocaInfo == LOCA_RIGHT)
			{
				if (m_nSubShapeStatus[2] < 0)
				{
					pIter->nX -= m_nSubShapeStatus[2];
					pIter->nWidth -= m_nSubShapeStatus[2];
				}
				else
				{
					pIter->nX -= tmpSub;
					pIter->nWidth -= tmpSub;
				}
			}

		}
		if (m_nSubShapeStatus[3] != MY_ERROR) //change Height
		{
			//////////////////////////////////////////////////////////////////////////
			/// tmpSub �� ���Ƽ� ���� ���ϱ�
			for (auto pTmpIter : m_CaDependent)
			{
				int tmpLoca = pTmpIter->GetLocaInfo();

				if (tmpLoca == LOCA_LEFT || tmpLoca == LOCA_RIGHT)
				{
					if (nHeight <= pTmpIter->nHeight)
					{
						nHeight = pTmpIter->nHeight;
						tmpSub = m_nRememberShapeStatus[3] + m_nRememberShapeStatus[1] - nHeight;
					}
				}
			}

			if (m_nLocaInfo == LOCA_DOWN)
			{
				if (m_nSubShapeStatus[3] < 0)
				{
					pIter->nY -= m_nSubShapeStatus[3];
					pIter->nHeight -= m_nSubShapeStatus[3];
				}
				else
				{
					pIter->nY -= tmpSub;
					pIter->nHeight -= tmpSub;
				}
			}
		}
	}

	SetRect(nX, nY, nWidth, nHeight);

	return TRUE;
}

bool CBasicContainerShape::WheelShape(short zDelta)
{
	//<ũ�� �� �� start>
	if (zDelta > 100)
	{
		if (nX < 5 || nWidth > 740 || nY < 5 || nHeight > 700)
		{
			return FALSE;
		}

		//Room���� Dependent! (��ü ���� �پ��ų� �þ�� �굵 ��ġ�� ���ؾ� ��)
		for (auto pIter : m_CaDependent)
		{
			if (pIter->GetLocaInfo() == LOCA_LEFT) //�����̸�
			{
				pIter->nX -= 2;
				pIter->nWidth -= 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_RIGHT) //�������̸�
			{
				pIter->nX += 2;
				pIter->nWidth += 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_UP) //�����̸�
			{
				pIter->nY -= 2;
				pIter->nHeight -= 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_DOWN) //�Ʒ����̸�
			{
				pIter->nY += 2;
				pIter->nHeight += 2;
			}
			else
			{
				cout << " �� �׷� Wheel Error\n";
				return FALSE;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ���� �� ũ�� ����, ���� ��������
		nX -= 2;
		nWidth += 2;
		nY -= 2;
		nHeight += 2;


	} // <ũ�� �� �� end />
	 // <�۰� �� �� start>
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// ��ü ũ�� üũ
		if (nWidth - nX < 30 || nHeight - nY < 30)
		{
			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		// �۾��� �� ���� Dependent Shape�� ������ ����!
		for (auto pIter : m_CaDependent) //Room���� Dependent!
		{
			if (pIter->GetLocaInfo() == LOCA_LEFT || pIter->GetLocaInfo() == LOCA_RIGHT) //����, ������
			{
				if (nHeight <= pIter->nHeight || nY >= pIter->nY)
				{
					return FALSE;
				}
			}
			else if (pIter->GetLocaInfo() == LOCA_UP || pIter->GetLocaInfo() == LOCA_DOWN) //����, �Ʒ���
			{
				if (nWidth <= pIter->nWidth || nX >= pIter->nX)
				{
					return FALSE;
				}
			}
			else
			{
				cout << "Mouse Wheel �۾����� Error" << endl;
				return FALSE;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// �ɸ��� �� ���ٸ� ���� Room���� Dependent ����! (��ü ���� �پ��ų� �þ�� �굵 ��ġ�� ���ؾ� ��)
		for (auto pIter : m_CaDependent)
		{
			if (pIter->GetLocaInfo() == LOCA_LEFT) //�����̸�
			{
				pIter->nX += 2;
				pIter->nWidth += 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_RIGHT) //�������̸�
			{
				pIter->nX -= 2;
				pIter->nWidth -= 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_UP) //�����̸�
			{
				pIter->nY += 2;
				pIter->nHeight += 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_DOWN) //�Ʒ����̸�
			{
				pIter->nY -= 2;
				pIter->nHeight -= 2;
			}
			else // ��� ������ �� üũ��
			{
				cout << "�� �׷� Wheel Error\n";
				return FALSE;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// ���� �� ũ�� ����, ���� ��������
		nX += 2;
		nWidth -= 2;
		nY += 2;
		nHeight -= 2;
	}


	return TRUE;
}