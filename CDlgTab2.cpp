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
}


BEGIN_MESSAGE_MAP(CDlgTab2, CDialog)
	ON_BN_CLICKED(IDC_IP_EDIT_MT_BTN, &CDlgTab2::OnBnClickedIpEditMtBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_FALSE_BTN, &CDlgTab2::OnBnClickedIpEditFalseBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_TRUE_BTN, &CDlgTab2::OnBnClickedIpEditTrueBtn)
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

	//특정 View 포인터 얻는 법
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고...
	//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
	ControlView* pView = (ControlView*)pFrame->m_wndSplitter.GetPane(0, 1); // 활성화된 뷰의 포인터 구한다.

	pView->m_TabCtrl.DeleteAllItems();
	pView->m_TabCtrl.InsertItem(0, L"Object");
	pView->m_pDlgTab1->ShowWindow(SW_SHOW);
	pView->m_pDlgTab2->ShowWindow(SW_HIDE);
}
