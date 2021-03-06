#pragma once

// CDlgTab1 대화 상자

class CDlgTab1 : public CDialog
{
	DECLARE_DYNAMIC(CDlgTab1)

public:
	CDlgTab1(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTab1();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IP_TABDLG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIpRoomBtn();
	afx_msg void OnBnClickedIpWindowBtn();
	afx_msg void OnBnClickedIpDoorBtn();
	afx_msg void OnBnClickedIpAddBtn();
	afx_msg void OnBnClickedIpWashBtn();
	afx_msg void OnBnClickedIpTvBtn();

	void ReDrawInteriorView();	
};
