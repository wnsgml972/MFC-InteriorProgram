// CDlgTab2.cpp: 구현 파일
//

#include "stdafx.h"
#include "InteriorProgram.h"
#include "MainFrm.h"
#include "InteriorProgramDoc.h"
#include "InteriorProgramView.h"
#include "CDlgTab2.h"
#include "afxdialogex.h"
#include "ShapeHandler.h"
#include "UserObjectShape.h"
#include "define.h"


// CDlgTab2 대화 상자

IMPLEMENT_DYNAMIC(CDlgTab2, CDialog)

CDlgTab2::CDlgTab2(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_IP_TABDLG2_DLG, pParent)
{

}

CDlgTab2::~CDlgTab2()
{
}

void CDlgTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_IP_EDIT_ON_RDO, (int &)m_OnRadio);
}


BEGIN_MESSAGE_MAP(CDlgTab2, CDialog)
	ON_BN_CLICKED(IDC_IP_EDIT_MT_BTN, &CDlgTab2::OnBnClickedIpEditMtBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_FALSE_BTN, &CDlgTab2::OnBnClickedIpEditFalseBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_TRUE_BTN, &CDlgTab2::OnBnClickedIpEditTrueBtn)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_IP_EDIT_ON_RDO, IDC_IP_EDIT_OFF_RDO, &CDlgTab2::OnBnClickedIpEditOnRdo)
END_MESSAGE_MAP()

void CDlgTab2::OnBnClickedIpEditMtBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}
void CDlgTab2::OnBnClickedIpEditFalseBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//특정 View 포인터 얻는 법
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고...
	//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
	ControlView* pView = (ControlView*)pFrame->m_wndSplitter.GetPane(0, 1); // 활성화된 뷰의 포인터 구한다.
	
	// 되돌아가기
	pView->m_TabCtrl.DeleteAllItems();
	pView->m_TabCtrl.InsertItem(0, L"Object");
	pView->m_pDlgTab1->ShowWindow(SW_SHOW);
	pView->m_pDlgTab2->ShowWindow(SW_HIDE);
}
void CDlgTab2::OnBnClickedIpEditTrueBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = ShapeHandler::GetInstance()->GetCurrentSelectedIndex();

	if (index == MY_ERROR)
	{
		cout << "말도 안 됨\n" << endl;
		return;
	}

	CString strX;
	CString strY;
	CString strWidth;
	CString strHeight;

	GetDlgItemText(IDC_IP_EDIT_X_EDT, strX);
	GetDlgItemText(IDC_IP_EDIT_Y_EDT, strY);
	GetDlgItemText(IDC_IP_EDIT_WIDTH_EDT, strWidth);
	GetDlgItemText(IDC_IP_EDIT_HEIGHT_EDT, strHeight);

	//공백제거
	strX.Replace(_T(" "), NULL);
	strY.Replace(_T(" "), NULL); 
	strWidth.Replace(_T(" "), NULL); 
	strHeight.Replace(_T(" "), NULL); 

	if (!strX.Compare(L"") || !strY.Compare(L"") || !strWidth.Compare(L"") || !strHeight.Compare(L""))
	{
		MessageBox(L"공백은 입력할 수 없습니다.");
	}
	else if (!FileManager::GetInstance()->isNumber(FileManager::GetInstance()->CStringToString(strX)) || !FileManager::GetInstance()->isNumber(FileManager::GetInstance()->CStringToString(strY)) ||
		!FileManager::GetInstance()->isNumber(FileManager::GetInstance()->CStringToString(strWidth)) || !FileManager::GetInstance()->isNumber(FileManager::GetInstance()->CStringToString(strHeight)))
	{
		MessageBox(L"숫자만 입력해주세요.");
	}
	else
	{
		// 크기 제어
		{ 
			int nX = _ttoi(strX);
			int nY = _ttoi(strY);
			int nWidth = _ttoi(strWidth) + _ttoi(strX);
			int nHeight = _ttoi(strHeight) + _ttoi(strY);

			int tmpShapeStatus[4] = { _ttoi(strX) , _ttoi(strY) ,_ttoi(strWidth) ,_ttoi(strHeight) };

			for (int i = 0; i < 4; i++)
			{
				if (m_nRememberShapeStatus[i] != tmpShapeStatus[i])
				{
					m_nSubShapeStatus[i] = m_nRememberShapeStatus[i] - tmpShapeStatus[i];
				}
			}

			if (nX < 5 || nWidth > 757 || nY < 5 || nHeight > 712 || _ttoi(strWidth) == 0 || _ttoi(strHeight) == 0)
			{
				MessageBox(_T("범위 초과"));
				return;
			}

			Shape *tmpShape = ShapeHandler::GetInstance()->m_CaShape[index];
			int m_nDrawRange = ShapeHandler::GetInstance()->m_nDrawRange;

			// Room을 선택해서 Scale하려 할 때!,  그룹화 Edit 사용 안 함!
			if (typeid(*tmpShape) == typeid(RoomShape))
			{
				RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape);
				int tmpSub = MY_ERROR;

				// 한 꺼번에 검사하기위해 하나의 벡터에 합쳐서 사용함
				vector<Shape*> tmpVector;
				tmpVector.insert(tmpVector.end(), tmpRoomShape->m_CaDoor.begin(), tmpRoomShape->m_CaDoor.end());
				tmpVector.insert(tmpVector.end(), tmpRoomShape->m_CaWindow.begin(), tmpRoomShape->m_CaWindow.end());

				for (auto pIter : tmpVector)
				{
					int m_nLocaInfo = MY_ERROR;
					int LOCA_DOWN, LOCA_UP, LOCA_RIGHT, LOCA_LEFT;

					if (typeid(*pIter) == typeid(DoorShape))
					{
						m_nLocaInfo = dynamic_cast<DoorShape*>(pIter)->m_nLocaInfo;
						LOCA_DOWN = dynamic_cast<DoorShape*>(pIter)->LOCA_DOWN;
						LOCA_UP = dynamic_cast<DoorShape*>(pIter)->LOCA_UP;
						LOCA_RIGHT = dynamic_cast<DoorShape*>(pIter)->LOCA_RIGHT;
						LOCA_LEFT = dynamic_cast<DoorShape*>(pIter)->LOCA_LEFT;
					}
					else if (typeid(*pIter) == typeid(WindowShape))
					{
						m_nLocaInfo = dynamic_cast<WindowShape*>(pIter)->m_nLocaInfo;
						LOCA_DOWN = dynamic_cast<WindowShape*>(pIter)->LOCA_DOWN;
						LOCA_UP = dynamic_cast<WindowShape*>(pIter)->LOCA_UP;
						LOCA_RIGHT = dynamic_cast<WindowShape*>(pIter)->LOCA_RIGHT;
						LOCA_LEFT = dynamic_cast<WindowShape*>(pIter)->LOCA_LEFT;
					}

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
						for (auto pTmpIter : tmpVector)
						{
							int tmpLoca = MY_ERROR;
							if (typeid(*pTmpIter) == typeid(DoorShape))
							{
								tmpLoca = dynamic_cast<DoorShape*>(pTmpIter)->m_nLocaInfo;
							}
							else if (typeid(*pTmpIter) == typeid(WindowShape))
							{
								tmpLoca = dynamic_cast<WindowShape*>(pTmpIter)->m_nLocaInfo;
							}
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
						for (auto pTmpIter : tmpVector)
						{
							int tmpLoca = MY_ERROR;
							if (typeid(*pTmpIter) == typeid(DoorShape))
							{
								tmpLoca = dynamic_cast<DoorShape*>(pTmpIter)->m_nLocaInfo;
							}
							else if (typeid(*pTmpIter) == typeid(WindowShape))
							{
								tmpLoca = dynamic_cast<WindowShape*>(pTmpIter)->m_nLocaInfo;
							}
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
			}
			// 문이나 창문 선택 Edit
			else if (typeid(*tmpShape) == typeid(DoorShape) || (typeid(*tmpShape) == typeid(WindowShape)))
			{
				RoomShape *tmpRoomShape;
				int tmpSub = MY_ERROR;

				int m_nLocaInfo = MY_ERROR;
				int LOCA_DOWN, LOCA_UP, LOCA_RIGHT, LOCA_LEFT;

				if (typeid(*tmpShape) == typeid(DoorShape)) //문일때
				{
					tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->m_pInRoomShapePointer);

					m_nLocaInfo = dynamic_cast<DoorShape*>(tmpShape)->m_nLocaInfo;
					LOCA_DOWN = dynamic_cast<DoorShape*>(tmpShape)->LOCA_DOWN;
					LOCA_UP = dynamic_cast<DoorShape*>(tmpShape)->LOCA_UP;
					LOCA_RIGHT = dynamic_cast<DoorShape*>(tmpShape)->LOCA_RIGHT;
					LOCA_LEFT = dynamic_cast<DoorShape*>(tmpShape)->LOCA_LEFT;
				}
				else	//창문일때
				{
					tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->m_pInRoomShapePointer);

					m_nLocaInfo = dynamic_cast<WindowShape*>(tmpShape)->m_nLocaInfo;
					LOCA_DOWN = dynamic_cast<WindowShape*>(tmpShape)->LOCA_DOWN;
					LOCA_UP = dynamic_cast<WindowShape*>(tmpShape)->LOCA_UP;
					LOCA_RIGHT = dynamic_cast<WindowShape*>(tmpShape)->LOCA_RIGHT;
					LOCA_LEFT = dynamic_cast<WindowShape*>(tmpShape)->LOCA_LEFT;
				}

				if (m_nLocaInfo == LOCA_UP || m_nLocaInfo == LOCA_DOWN)
				{
					if (m_nSubShapeStatus[0] != MY_ERROR) //change X
					{
						if (nX <= tmpRoomShape->nX) //작아질 때
						{
							tmpSub = m_nRememberShapeStatus[0] - tmpRoomShape->nX;
							nX = tmpRoomShape->nX;
							nWidth = m_nRememberShapeStatus[0] + m_nRememberShapeStatus[2] - tmpSub;
						}
						if (nX + m_nRememberShapeStatus[2] >= tmpRoomShape->nWidth)  //커질 때
						{
							tmpSub = tmpRoomShape->nWidth - (m_nRememberShapeStatus[0] + m_nRememberShapeStatus[2]);
							nWidth = tmpRoomShape->nWidth;
							nX = m_nRememberShapeStatus[0] + tmpSub;
						}
					}
					if (m_nSubShapeStatus[2] != MY_ERROR) //change Width
					{
						if (nWidth >= tmpRoomShape->nWidth)
							nWidth = tmpRoomShape->nWidth;
					}
				}
				else if (m_nLocaInfo == LOCA_RIGHT || m_nLocaInfo == LOCA_LEFT)
				{
					if (m_nSubShapeStatus[1] != MY_ERROR) //change Y
					{
						if (nY <= tmpRoomShape->nY) //작아질 때
						{
							tmpSub = m_nRememberShapeStatus[1] - tmpRoomShape->nY;
							nY = tmpRoomShape->nY;
							nHeight = m_nRememberShapeStatus[1] + m_nRememberShapeStatus[3] - tmpSub;
						}
						if (nY + m_nRememberShapeStatus[3] >= tmpRoomShape->nHeight)  //커질 때
						{
							tmpSub = tmpRoomShape->nHeight - (m_nRememberShapeStatus[1] + m_nRememberShapeStatus[3]);
							nHeight = tmpRoomShape->nHeight;
							nY = m_nRememberShapeStatus[1] + tmpSub;
						}
					}
					if (m_nSubShapeStatus[3] != MY_ERROR) //change Height
					{
						if (nHeight >= tmpRoomShape->nHeight)
							nHeight = tmpRoomShape->nHeight;
					}
				}
			}
			// UserObject
			else
			{
				// 이미지 크기 바꾸기~
				UserObjectShape *tmpUserObjectShape = dynamic_cast<UserObjectShape*>(tmpShape);
				if (m_nSubShapeStatus[2] != MY_ERROR) //change Width
				{

				}
				if (m_nSubShapeStatus[3] != MY_ERROR) //change Height
				{

				}
			}
			// 마지막은 SetRect
			ShapeHandler::GetInstance()->m_CaShape[index]->SetRect(nX, nY, nWidth, nHeight);
		}


		// 화면 전환
		{
			//특정 View 포인터 얻는 법
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고...
			//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
			ControlView* pView = (ControlView*)pFrame->m_wndSplitter.GetPane(0, 1); // 활성화된 뷰의 포인터 구한다.

			pView->m_TabCtrl.DeleteAllItems();
			pView->m_TabCtrl.InsertItem(0, L"Object");
			pView->m_pDlgTab1->ShowWindow(SW_SHOW);
			pView->m_pDlgTab2->ShowWindow(SW_HIDE);

			// InteriorProgramView 
			CInteriorProgramView* pView0 = (CInteriorProgramView*)pFrame->m_wndSplitter.GetPane(0, 0); // 활성화된 뷰의 포인터 구한다.

			// 화면이 전환하기 전 선택을 놔버림!
			ShapeHandler::GetInstance()->InitSelect();
			pView0->RedrawWindow();
		}
	}
}

void CDlgTab2::SetDlgItem()
{
	int index = ShapeHandler::GetInstance()->GetCurrentSelectedIndex();

	if (index == MY_ERROR)
	{
		return;
	}
	
	for (int i=0; i<4; i++)
		m_nSubShapeStatus[i] = MY_ERROR;

	// EDT default check
	{
		CString strX = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nX));
		CString strY = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nY));
		CString strWidth = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nWidth - ShapeHandler::GetInstance()->m_CaShape[index]->nX));
		CString strHeight = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nHeight - ShapeHandler::GetInstance()->m_CaShape[index]->nY));

		m_nRememberShapeStatus[0] = ShapeHandler::GetInstance()->m_CaShape[index]->nX;
		m_nRememberShapeStatus[1] = ShapeHandler::GetInstance()->m_CaShape[index]->nY;
		m_nRememberShapeStatus[2] = ShapeHandler::GetInstance()->m_CaShape[index]->nWidth - ShapeHandler::GetInstance()->m_CaShape[index]->nX;
		m_nRememberShapeStatus[3] = ShapeHandler::GetInstance()->m_CaShape[index]->nHeight - ShapeHandler::GetInstance()->m_CaShape[index]->nY;

		SetDlgItemText(IDC_IP_EDIT_X_EDT, strX);
		SetDlgItemText(IDC_IP_EDIT_Y_EDT, strY);
		SetDlgItemText(IDC_IP_EDIT_WIDTH_EDT, strWidth);
		SetDlgItemText(IDC_IP_EDIT_HEIGHT_EDT, strHeight);
	}

	// Radio default check
	{
		((CButton*)GetDlgItem(IDC_IP_EDIT_ON_RDO))->SetCheck(1);
	}

	// EDT enable check
	{
		GetDlgItem(IDC_IP_EDIT_X_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_IP_EDIT_WIDTH_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_IP_EDIT_Y_EDT)->EnableWindow(TRUE);
		GetDlgItem(IDC_IP_EDIT_HEIGHT_EDT)->EnableWindow(TRUE);
		if (typeid(*ShapeHandler::GetInstance()->m_CaShape[index]) == typeid(DoorShape))
		{
			DoorShape *tmpShape = dynamic_cast<DoorShape*>(ShapeHandler::GetInstance()->m_CaShape[index]);
			if (tmpShape->m_nLocaInfo == tmpShape->LOCA_UP)
			{
				GetDlgItem(IDC_IP_EDIT_Y_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_HEIGHT_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->m_nLocaInfo == tmpShape->LOCA_RIGHT)
			{
				GetDlgItem(IDC_IP_EDIT_X_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_WIDTH_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->m_nLocaInfo == tmpShape->LOCA_DOWN)
			{
				GetDlgItem(IDC_IP_EDIT_Y_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_HEIGHT_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->m_nLocaInfo == tmpShape->LOCA_LEFT)
			{
				GetDlgItem(IDC_IP_EDIT_X_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_WIDTH_EDT)->EnableWindow(FALSE);
			}
		}
		else if (typeid(*ShapeHandler::GetInstance()->m_CaShape[index]) == typeid(WindowShape))
		{
			WindowShape *tmpShape = dynamic_cast<WindowShape*>(ShapeHandler::GetInstance()->m_CaShape[index]);
			if (tmpShape->m_nLocaInfo == tmpShape->LOCA_UP)
			{
				GetDlgItem(IDC_IP_EDIT_Y_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_HEIGHT_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->m_nLocaInfo == tmpShape->LOCA_RIGHT)
			{
				GetDlgItem(IDC_IP_EDIT_X_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_WIDTH_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->m_nLocaInfo == tmpShape->LOCA_DOWN)
			{
				GetDlgItem(IDC_IP_EDIT_Y_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_HEIGHT_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->m_nLocaInfo == tmpShape->LOCA_LEFT)
			{
				GetDlgItem(IDC_IP_EDIT_X_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_WIDTH_EDT)->EnableWindow(FALSE);
			}
		}
	}
}


void CDlgTab2::OnBnClickedIpEditOnRdo(UINT value)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	switch (m_OnRadio)
	{
	case 0:
		cout << "On" << endl;
		break;
	case 1:
		cout << "Off" << endl;
		break;
	default:
		break;
	}

	UpdateData(FALSE);
}
