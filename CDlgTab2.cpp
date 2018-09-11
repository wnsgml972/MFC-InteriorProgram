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
	ON_BN_CLICKED(IDC_IP_EDIT_CLR_BTN, &CDlgTab2::OnBnClickedIpEditClrBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_EDIT_BTN, &CDlgTab2::OnBnClickedIpEditEditBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_UNDO_BTN, &CDlgTab2::OnBnClickedIpEditUndoBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_REDO_BTN, &CDlgTab2::OnBnClickedIpEditRedoBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_MT_BTN, &CDlgTab2::OnBnClickedIpEditMtBtn)
	ON_BN_CLICKED(IDC_IP_EDIT_COLOR_BTN, &CDlgTab2::OnBnClickedIpEditColorBtn)
END_MESSAGE_MAP()


// CDlgTab2 메시지 처리기
void CDlgTab2::OnBnClickedIpEditClrBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_EDIT_CLEAR;
	ShapeHandler::GetInstance()->Clear();



	//특정 View 포인터 얻는 법
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고...
	//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
	CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->m_wndSplitter.GetPane(0, 0); // 활성화된 뷰의 포인터 구한다.
	pView->RedrawWindow();
}
void CDlgTab2::OnBnClickedIpEditEditBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_EDIT;

}
void CDlgTab2::OnBnClickedIpEditUndoBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_EDIT_UNDO;

}
void CDlgTab2::OnBnClickedIpEditRedoBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_EDIT_REDO;

}
void CDlgTab2::OnBnClickedIpEditMtBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_EDIT_MT;

}
void CDlgTab2::OnBnClickedIpEditColorBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_EDIT_COLOR;

}
