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
	// 마지막 검사, 기준 재조정
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
	auto tmpInRoomPtr = this; //먼저 자신의 포인터를 받아둠
	tmpInRoomPtr->SetId(-600000);

	//////////////////////////////////////////////////////////////////////////
	// 먼저 방안의 Dependent 벡터를 모두 동적할당 해제 후 삭제
	for (auto nIdx : m_CaDependent)
	{
		nIdx->SetId(-600000);
		SAFE_DELETE(nIdx);
	}
	m_CaDependent.clear();

	//////////////////////////////////////////////////////////////////////////
	// 전체 Shape 벡터에서는 아이디 값이 쓰레기 값인 것을 삭제 후, 동적할당 해제
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


// 밑에처럼 다른 문 벡터나 창문 벡터처럼 한번에 해도 되지만 좀 알아보기 쉽게 방은 그냥 템프 하나 구해놈
	tmpX = point.x + m_nMoveSubVal[0];
	tmpY = point.y + m_nMoveSubVal[1];
	tmpWidth = point.x + m_nMoveSubVal[2];
	tmpHeight = point.y + m_nMoveSubVal[3];

	// 창문이나 문이 그려지면 그 DrawRange 범위 밖으로도 나가지면 안 됨!!
	if (tmpX < 0 + CShapeHandler::GetInstance()->m_nDrawSelectRange || tmpWidth > 765 - CShapeHandler::GetInstance()->m_nDrawSelectRange || 
		tmpY < 0 + CShapeHandler::GetInstance()->m_nDrawSelectRange || tmpHeight > 720 - CShapeHandler::GetInstance()->m_nDrawSelectRange) // 방 범위 제어 문
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

	// 방에 대한 것을 먼저 해야하는 게 방 클릭 후 범위 밖으로 나가면 다른 창문이나 문은 움직이면 안 되므로
	// 위에서 먼저 체크하고 리턴해버려야 함
	// 문 안의 도형
	{
		//In Room Door, Window
#pragma warning(push)
#pragma warning(disable: 4018)
		for (long i = 0; i < m_CaDependent.size(); i++)
#pragma warning(pop)
		{
			// 문 범위 제어 문
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

bool CBasicContainerShape::MagneticShape() //CRoomShape Move시 그룹화 된 문과 창문을 같이 움직여야 됨!
{
	int m_nSelectRange = CShapeHandler::GetInstance()->m_nSelectRange;

	//전체 Shape중 인접한 Shape가 있는지
	//뒤에서부터 검색해야 함!
#pragma warning(push)
#pragma warning(disable: 4018)
	for (long i = CShapeHandler::GetInstance()->m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		auto tmpIndexShape = CShapeHandler::GetInstance()->m_CaShape[i];

		if (this == tmpIndexShape || tmpIndexShape->m_nType != TYPE_BASIC_CONTAINER) // 서로 같거나 Room이 아니면 넘김
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
		// 네모를 하나의 라인 하나당 Range가 존재하는 네모로 봤을 때
		// 각각 위에서부터 범위를 조사해감
		// 위 -> 아래 -> 왼쪽 -> 오른쪽 
		// Select 기준에서 Magnetic Range를 만들고 Any Shape의 다섯 좌표가 그 Range 안에 들어간다면 그 방향으로 선택
		// 2. Select < Any
		// 위와 같은 알고리즘으로 그 반대의 Any Shape의 기준에서 Magnetic Range를 만들어 놓고
		// 그 범위 안에 Select Shape의 다섯 좌표가 들어가는지 똑같은 방식으로 조사함 

		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nAnyReferenceX, tmpIndexShape->nX, tmpIndexShape->nWidth);
		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nSelectedReferenceX, nShapeTempX, nShapeTempWidth);
		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nAnyReferenceY, tmpIndexShape->nY, tmpIndexShape->nHeight);
		CShapeHandler::GetInstance()->MakeFiveReferenceVertex(nSelectedReferenceY, nShapeTempY, nShapeTempHeight);

		// 5개의 정점을 모두 Magnetic Range안에 들어가는지 찾음
		for (int j = 0; j < 5; j++)
		{
			if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempY - m_nSelectRange <= tmpIndexShape->nHeight && tmpIndexShape->nHeight <= nShapeTempY + m_nSelectRange)
				|| (tmpIndexShape->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= tmpIndexShape->nWidth + m_nSelectRange) && (tmpIndexShape->nHeight - m_nSelectRange <= nShapeTempY && nShapeTempY <= tmpIndexShape->nHeight + m_nSelectRange))
			{ // 위
				naDirectionControl[0]++;
			}
			else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempWidth - m_nSelectRange <= tmpIndexShape->nX && tmpIndexShape->nX <= nShapeTempWidth + m_nSelectRange)
				|| (tmpIndexShape->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= tmpIndexShape->nHeight + m_nSelectRange) && (tmpIndexShape->nX - m_nSelectRange <= nShapeTempWidth && nShapeTempWidth <= tmpIndexShape->nX + m_nSelectRange))
			{ // 오른쪽
				naDirectionControl[1]++;
			}
			else if ((nShapeTempX - m_nSelectRange <= nAnyReferenceX[j] && nAnyReferenceX[j] <= nShapeTempWidth + m_nSelectRange) && (nShapeTempHeight - m_nSelectRange <= tmpIndexShape->nY && tmpIndexShape->nY <= nShapeTempHeight + m_nSelectRange)
				|| (tmpIndexShape->nX - m_nSelectRange <= nSelectedReferenceX[j] && nSelectedReferenceX[j] <= tmpIndexShape->nWidth + m_nSelectRange) && (tmpIndexShape->nY - m_nSelectRange <= nShapeTempHeight && nShapeTempHeight <= tmpIndexShape->nY + m_nSelectRange))
			{ // 아래
				naDirectionControl[2]++;
			}
			else if ((nShapeTempY - m_nSelectRange <= nAnyReferenceY[j] && nAnyReferenceY[j] <= nShapeTempHeight + m_nSelectRange) && (nShapeTempX - m_nSelectRange <= tmpIndexShape->nWidth && tmpIndexShape->nWidth <= nShapeTempX + m_nSelectRange)
				|| (tmpIndexShape->nY - m_nSelectRange <= nSelectedReferenceY[j] && nSelectedReferenceY[j] <= tmpIndexShape->nHeight + m_nSelectRange) && (tmpIndexShape->nWidth - m_nSelectRange <= nShapeTempX && nShapeTempX <= tmpIndexShape->nWidth + m_nSelectRange))
			{ // 왼쪽
				naDirectionControl[3]++;
			}
		}

		// 3개 이상 범위에 들어오면 성공
		for (int j = 0; j < 4; j++)
		{
			if (naDirectionControl[j] >= 3)
			{
				int nSubMoving;
				switch (j)
				{
				case 0: //위
					//cout << "Mag 위쪽 " << endl;
					nSubMoving = (nY - tmpIndexShape->nHeight);
					nHeight -= nSubMoving;
					nY = tmpIndexShape->nHeight;

					/// 방 안의 창문과 문도 Update
					{
						//In Room Dependent
						for (auto DependentShape :m_CaDependent)
						{
							DependentShape->nY -= nSubMoving;
							DependentShape->nHeight -= nSubMoving;
						}
					}
					break;
				case 1: //오른쪽
					//cout << "Mag 오른쪽 " << endl;
					nSubMoving = (nWidth - tmpIndexShape->nX);
					nX -= nSubMoving;
					nWidth = tmpIndexShape->nX;

					/// 방 안의 창문과 문도 Update
					{
						//In Room Dependent
						for (auto DependentShape : m_CaDependent)
						{
							DependentShape->nX -= nSubMoving;
							DependentShape->nWidth -= nSubMoving;
						}
					}
					break;
				case 2: //아래
					//cout << "Mag 아래 " << endl;
					nSubMoving = (nHeight - tmpIndexShape->nY);
					nY -= nSubMoving;
					nHeight = tmpIndexShape->nY;

					/// 방 안의 창문과 문도 Update
					{
						//In Room Dependent
						for (auto DependentShape : m_CaDependent)
						{
							DependentShape->nY -= nSubMoving;
							DependentShape->nHeight -= nSubMoving;
						}
					}
					break;
				case 3: //왼쪽
					//cout << "Mag 왼쪽 " << endl;
					nSubMoving = (nX - tmpIndexShape->nWidth);
					nWidth -= nSubMoving;
					nX = tmpIndexShape->nWidth;

					/// 방 안의 창문과 문도 Update
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

	// Basic Container가 가지고 있는 Dependent 검사
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
			/// tmpSub 다 돌아서 먼저 구하기
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
			/// tmpSub 다 돌아서 먼저 구하기
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
	//<크게 할 때 start>
	if (zDelta > 100)
	{
		if (nX < 5 || nWidth > 740 || nY < 5 || nHeight > 700)
		{
			return FALSE;
		}

		//Room안의 Dependent! (전체 룸이 줄어들거나 늘어나면 얘도 위치가 변해야 함)
		for (auto pIter : m_CaDependent)
		{
			if (pIter->GetLocaInfo() == LOCA_LEFT) //왼쪽이면
			{
				pIter->nX -= 2;
				pIter->nWidth -= 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_RIGHT) //오른쪽이면
			{
				pIter->nX += 2;
				pIter->nWidth += 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_UP) //위쪽이면
			{
				pIter->nY -= 2;
				pIter->nHeight -= 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_DOWN) //아래쪽이면
			{
				pIter->nY += 2;
				pIter->nHeight += 2;
			}
			else
			{
				cout << " 문 그룹 Wheel Error\n";
				return FALSE;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 성공 방 크기 조정, 제일 마지막에
		nX -= 2;
		nWidth += 2;
		nY -= 2;
		nHeight += 2;


	} // <크게 할 때 end />
	 // <작게 할 때 start>
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 전체 크기 체크
		if (nWidth - nX < 30 || nHeight - nY < 30)
		{
			return FALSE;
		}

		//////////////////////////////////////////////////////////////////////////
		// 작아질 수 없게 Dependent Shape를 가지고 있음!
		for (auto pIter : m_CaDependent) //Room안의 Dependent!
		{
			if (pIter->GetLocaInfo() == LOCA_LEFT || pIter->GetLocaInfo() == LOCA_RIGHT) //왼쪽, 오른쪽
			{
				if (nHeight <= pIter->nHeight || nY >= pIter->nY)
				{
					return FALSE;
				}
			}
			else if (pIter->GetLocaInfo() == LOCA_UP || pIter->GetLocaInfo() == LOCA_DOWN) //위쪽, 아래쪽
			{
				if (nWidth <= pIter->nWidth || nX >= pIter->nX)
				{
					return FALSE;
				}
			}
			else
			{
				cout << "Mouse Wheel 작아지게 Error" << endl;
				return FALSE;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// 걸리는 게 없다면 조절 Room안의 Dependent 조절! (전체 룸이 줄어들거나 늘어나면 얘도 위치가 변해야 함)
		for (auto pIter : m_CaDependent)
		{
			if (pIter->GetLocaInfo() == LOCA_LEFT) //왼쪽이면
			{
				pIter->nX += 2;
				pIter->nWidth += 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_RIGHT) //오른쪽이면
			{
				pIter->nX -= 2;
				pIter->nWidth -= 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_UP) //위쪽이면
			{
				pIter->nY += 2;
				pIter->nHeight += 2;
			}
			else if (pIter->GetLocaInfo() == LOCA_DOWN) //아래쪽이면
			{
				pIter->nY -= 2;
				pIter->nHeight -= 2;
			}
			else // 사실 위에서 다 체크함
			{
				cout << "문 그룹 Wheel Error\n";
				return FALSE;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 성공 방 크기 조정, 제일 마지막에
		nX += 2;
		nWidth -= 2;
		nY += 2;
		nHeight -= 2;
	}


	return TRUE;
}