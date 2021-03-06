#pragma once

// CDlgTab2 대화 상자

class CDlgTab2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgTab2)

public:
	CDlgTab2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgTab2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IP_TABDLG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedIpEditMtBtn();
	afx_msg void OnBnClickedIpEditFalseBtn();
	afx_msg void OnBnClickedIpEditTrueBtn();
	afx_msg void OnBnClickedIpEditOnRdo(UINT value);
	UINT m_OnRadio;

	void SetDlgItem();
	int m_nRememberShapeStatus[4];  // 입력한 값을 미리 받아서 이게 바뀌어도 되는 값인 가를 검사하기 위해 기억해놔야 함
	int m_nSubShapeStatus[4];			// Width와 Height를 좌표상의 위치가 아닌 실제 Width와 Height로 변화하기 위한 차이 값

};
