// CDlgTab1.cpp: 구현 파일
//

#include "stdafx.h"
#include "InteriorProgram.h"
#include "MainFrm.h"
#include "InteriorProgramDoc.h"
#include "InteriorProgramView.h"
#include "CDlgTab1.h"
#include "afxdialogex.h"
#include "define.h"
#include "ShapeHandler.h"
#include "ControlView.h"
#include "UserObjectShape.h"


// CDlgTab1 대화 상자

IMPLEMENT_DYNAMIC(CDlgTab1, CDialog)

CDlgTab1::CDlgTab1(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_IP_TABDLG1_DLG, pParent)
{

}

CDlgTab1::~CDlgTab1()
{
}

void CDlgTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTab1, CDialog)
	ON_BN_CLICKED(IDC_IP_ROOM_BTN, &CDlgTab1::OnBnClickedIpRoomBtn)
	ON_BN_CLICKED(IDC_IP_WINDOW_BTN, &CDlgTab1::OnBnClickedIpWindowBtn)
	ON_BN_CLICKED(IDC_IP_DOOR_BTN, &CDlgTab1::OnBnClickedIpDoorBtn)
	ON_BN_CLICKED(IDC_IP_ADD_BTN, &CDlgTab1::OnBnClickedIpAddBtn)
	ON_BN_CLICKED(IDC_IP_WASH_BTN, &CDlgTab1::OnBnClickedIpWashBtn)
	ON_BN_CLICKED(IDC_IP_TV_BTN, &CDlgTab1::OnBnClickedIpTvBtn)
END_MESSAGE_MAP()


// CDlgTab1 메시지 처리기

void CDlgTab1::OnBnClickedIpRoomBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_ROOM;
}
void CDlgTab1::OnBnClickedIpWindowBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_WINDOW;
}
void CDlgTab1::OnBnClickedIpDoorBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_DOOR;
}



void CDlgTab1::OnBnClickedIpWashBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC;

	ShapeHandler *tmpShapeHandler = ShapeHandler::GetInstance();
	
	UserObjectShape *CUserObjectShape = new UserObjectShape(tmpShapeHandler->MakeAutoIncId(), 10, 10, 50, 50);
	CUserObjectShape->SetColor(111, 128, 244);
	tmpShapeHandler->m_CaShape.push_back(CUserObjectShape);

	//다시 그리기
	ReDrawInteriorView();
}
void CDlgTab1::OnBnClickedIpTvBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC;

	//다시 그리기
	ReDrawInteriorView();
}

void CDlgTab1::OnBnClickedIpAddBtn() // 개발해야 함!
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC;

	//다시 그리기
	ReDrawInteriorView();
}

void CDlgTab1::ReDrawInteriorView()
{
	//특정 View 포인터 얻는 법
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고...
	//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
	CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->m_wndSplitter.GetPane(0, 0); // 활성화된 뷰의 포인터 구한다.
	pView->RedrawWindow();
}
