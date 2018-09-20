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
		MessageBox(L"숫자를 입력해주세요.");
	}
	else
	{
		int x = _ttoi(strX), y = _ttoi(strY), w = _ttoi(strWidth), h = _ttoi(strHeight);


		ShapeHandler::GetInstance()->m_CaShape[index]->SetRect(x, y, x + w, y + h);

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
		pView0->RedrawWindow();

	}
}

void CDlgTab2::SetDlgItem()
{
	int index = ShapeHandler::GetInstance()->GetCurrentSelectedIndex();

	if (index == MY_ERROR)
	{
		cout << "말도 안 됨\n" << endl;
		return;
	}
\

	CString strX = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nX));
	CString strY = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nY));
	CString strWidth = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nWidth - ShapeHandler::GetInstance()->m_CaShape[index]->nX));
	CString strHeight = FileManager::GetInstance()->StringToCString(to_string(ShapeHandler::GetInstance()->m_CaShape[index]->nHeight - ShapeHandler::GetInstance()->m_CaShape[index]->nY));

	SetDlgItemText(IDC_IP_EDIT_X_EDT, strX);
	SetDlgItemText(IDC_IP_EDIT_Y_EDT, strY);
	SetDlgItemText(IDC_IP_EDIT_WIDTH_EDT, strWidth);
	SetDlgItemText(IDC_IP_EDIT_HEIGHT_EDT, strHeight);


	// Radio default check
	((CButton*)GetDlgItem(IDC_IP_EDIT_ON_RDO))->SetCheck(1);
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
