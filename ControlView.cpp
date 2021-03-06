// ControlView.cpp: 구현 파일
//

#include "stdafx.h"
#include "InteriorProgram.h"
#include "ControlView.h"
#include "afxdialogex.h"
#include "define.h"

// ControlView 대화 상자

IMPLEMENT_DYNCREATE(ControlView, CFormView)

ControlView::ControlView()
	: CFormView(ControlView::IDD)
{
	m_pDlgTab1 = NULL;
	m_pDlgTab2 = NULL;
}

ControlView::~ControlView()
{
	SAFE_DELETE(m_pDlgTab1);
	SAFE_DELETE(m_pDlgTab2);
}

void ControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_TAB1_TAB, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(ControlView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_IP_TAB1_TAB, &ControlView::OnTcnSelchangeIpTab1)
END_MESSAGE_MAP()


#ifdef _DEBUG
void ControlView::AssertValid() const
{
	CFormView::AssertValid();
}
void ControlView::Dump(CDumpContext & dc) const
{
	CFormView::Dump(dc);
}
#endif


// ControlView 메시지 처리기


void ControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	m_TabCtrl.DeleteAllItems();
	m_TabCtrl.InsertItem(0, L"Object");
	//m_TabCtrl.InsertItem(1, L"Edit");

	CRect CRect;

	m_pDlgTab1 = new CDlgTab1;
	m_pDlgTab1->Create(IDD_IP_TABDLG1_DLG, &m_TabCtrl);
	m_pDlgTab1->GetWindowRect(&CRect);
	m_pDlgTab1->MoveWindow(5, 25, CRect.Width(), CRect.Height());
	m_pDlgTab1->ShowWindow(SW_SHOW);

 	m_pDlgTab2 = new CDlgTab2;
 	m_pDlgTab2->Create(IDD_IP_TABDLG2_DLG, &m_TabCtrl);
 	m_pDlgTab2->GetWindowRect(&CRect);
 	m_pDlgTab2->MoveWindow(5, 25, CRect.Width(), CRect.Height());
 	m_pDlgTab2->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}


void ControlView::OnTcnSelchangeIpTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int select = m_TabCtrl.GetCurSel();

	switch (select)
	{
	case 0:
		m_pDlgTab1->ShowWindow(SW_SHOW);
		m_pDlgTab2->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pDlgTab1->ShowWindow(SW_HIDE);
		m_pDlgTab2->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}

void ControlView::ShowEditTab()
{
	m_TabCtrl.DeleteAllItems();
	m_TabCtrl.InsertItem(0, L"Edit");
	m_pDlgTab1->ShowWindow(SW_HIDE);
	m_pDlgTab2->ShowWindow(SW_SHOW);
}