// CDlgTab1.cpp: 구현 파일
//

#include "stdafx.h"
#include "InteriorProgram.h"
#include "CDlgTab1.h"
#include "afxdialogex.h"
#include "define.h"


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
	ON_BN_CLICKED(IDC_IP_TR1_BTN, &CDlgTab1::OnBnClickedIpTr1Btn)
	ON_BN_CLICKED(IDC_IP_TR2_BTN, &CDlgTab1::OnBnClickedIpTr2Btn)
	ON_BN_CLICKED(IDC_IP_TR3_BTN, &CDlgTab1::OnBnClickedIpTr3Btn)
	ON_BN_CLICKED(IDC_IP_ADD_BTN, &CDlgTab1::OnBnClickedIpAddBtn)
	ON_BN_CLICKED(IDC_IP_WASH_BTN, &CDlgTab1::OnBnClickedIpWashBtn)
	ON_BN_CLICKED(IDC_IP_TV_BTN, &CDlgTab1::OnBnClickedIpTvBtn)
END_MESSAGE_MAP()


// CDlgTab2 메시지 처리기

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
void CDlgTab1::OnBnClickedIpTr1Btn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_TR1;
}
void CDlgTab1::OnBnClickedIpTr2Btn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_TR2;
}
void CDlgTab1::OnBnClickedIpTr3Btn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_TR3;
}
void CDlgTab1::OnBnClickedIpAddBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_USER_ADD;
}
void CDlgTab1::OnBnClickedIpWashBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_USER_WASH;
}
void CDlgTab1::OnBnClickedIpTvBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_USER_TV;
}
