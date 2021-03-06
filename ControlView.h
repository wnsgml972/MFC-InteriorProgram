#pragma once

#include "DlgTab1.h"
#include "DlgTab2.h"

// ControlView 대화 상자

class ControlView : public CFormView
{
	DECLARE_DYNCREATE(ControlView)

public:
	ControlView();   // 표준 생성자입니다.
	virtual ~ControlView();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IP_CONTROLVIEW };

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TabCtrl;
	CDlgTab1 *m_pDlgTab1;
	CDlgTab2 *m_pDlgTab2;
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeIpTab1(NMHDR *pNMHDR, LRESULT *pResult);

//구현부입니다.
public:
	void ShowEditTab();
};