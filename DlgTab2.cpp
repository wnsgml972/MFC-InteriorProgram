// CDlgTab2.cpp: 구현 파일
//

#include "stdafx.h"
#include "InteriorProgram.h"
#include "MainFrm.h"
#include "InteriorProgramDoc.h"
#include "InteriorProgramView.h"
#include "afxdialogex.h"
#include "DlgTab2.h"
#include "WindowShape.h"
#include "DoorShape.h"
#include "ObjectShape.h"

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
	else if (!JunUtil::isNumber(JunUtil::CStringToString(strX)) || !JunUtil::isNumber(JunUtil::CStringToString(strY)) ||
		!JunUtil::isNumber(JunUtil::CStringToString(strWidth)) || !JunUtil::isNumber(JunUtil::CStringToString(strHeight)))
	{
		MessageBox(L"숫자만 입력해주세요.");
	}
	else
	{
		int nX = _ttoi(strX);
		int nY = _ttoi(strY);
		int nWidth = _ttoi(strWidth) + _ttoi(strX);
		int nHeight = _ttoi(strHeight) + _ttoi(strY);

		// 크기 제어
		{ 
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
		}

		// 선택 된 Shape Edit!
		{
			if (CShapeHandler::GetInstance()
				->EditSelectedShape(nX, nY, nWidth, nHeight, m_nSubShapeStatus, m_nRememberShapeStatus) == MY_ERROR)
				MessageBox(_T("범위 초과"));
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
			CShapeHandler::GetInstance()->InitSelect();
			pView0->RedrawWindow();
		}
	}
}
void CDlgTab2::SetDlgItem()
{
	int index = CShapeHandler::GetInstance()->GetCurrentSelectedIndex();

	if (index == MY_ERROR)
	{
		return;
	}
	
	for (int i=0; i<4; i++)
		m_nSubShapeStatus[i] = MY_ERROR;

	// EDT default check
	{
		CString strX = JunUtil::StringToCString(to_string(CShapeHandler::GetInstance()->m_CaShape[index]->nX));
		CString strY = JunUtil::StringToCString(to_string(CShapeHandler::GetInstance()->m_CaShape[index]->nY));
		CString strWidth = JunUtil::StringToCString(to_string(CShapeHandler::GetInstance()->m_CaShape[index]->nWidth - CShapeHandler::GetInstance()->m_CaShape[index]->nX));
		CString strHeight = JunUtil::StringToCString(to_string(CShapeHandler::GetInstance()->m_CaShape[index]->nHeight - CShapeHandler::GetInstance()->m_CaShape[index]->nY));

		m_nRememberShapeStatus[0] = CShapeHandler::GetInstance()->m_CaShape[index]->nX;
		m_nRememberShapeStatus[1] = CShapeHandler::GetInstance()->m_CaShape[index]->nY;
		m_nRememberShapeStatus[2] = CShapeHandler::GetInstance()->m_CaShape[index]->nWidth - CShapeHandler::GetInstance()->m_CaShape[index]->nX;
		m_nRememberShapeStatus[3] = CShapeHandler::GetInstance()->m_CaShape[index]->nHeight - CShapeHandler::GetInstance()->m_CaShape[index]->nY;

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
		if (CShapeHandler::GetInstance()->m_CaShape[index]->m_nType == TYPE_DEPENDENT)
		{
			CDependentShape *tmpShape = dynamic_cast<CDependentShape*>(CShapeHandler::GetInstance()->m_CaShape[index]);
			if (tmpShape->GetLocaInfo() == LOCA_UP)
			{
				GetDlgItem(IDC_IP_EDIT_Y_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_HEIGHT_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->GetLocaInfo() == LOCA_RIGHT)
			{
				GetDlgItem(IDC_IP_EDIT_X_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_WIDTH_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->GetLocaInfo() == LOCA_DOWN)
			{
				GetDlgItem(IDC_IP_EDIT_Y_EDT)->EnableWindow(FALSE);
				GetDlgItem(IDC_IP_EDIT_HEIGHT_EDT)->EnableWindow(FALSE);
			}
			else if (tmpShape->GetLocaInfo() == LOCA_LEFT)
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
