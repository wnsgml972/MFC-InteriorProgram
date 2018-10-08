#pragma once

class CBufferDC : public CDC
{

private:
	CBufferDC() { }
	CBufferDC(const CBufferDC &src) { }
	CBufferDC& operator=(const CBufferDC &src) { }

protected:
	BOOL Attach(HDC hDC);
	HDC Detach();

private:
	CWnd*     m_pParent;            //��� �����쿡 ���� ������
	CDC*      m_pTarget;            //��� ������ DC�� ���� ������
	PAINTSTRUCT   m_PaintStruct;
	CRect     m_RcClient, m_RcWindow;     //��� �������� ũ�� ����

	CDC       m_MemoryDC;           //���� DC
	CBitmap     m_MemoryBmp, *m_pOldMemoryBmp;  //���۸��� ���� ��Ʈ��

public:
	CBufferDC(CWnd *pParent);
	~CBufferDC();

public:
	inline CRect ClientRect() const { return m_RcClient; }
	inline CRect WindowRect() const { return m_RcWindow; }
	inline CRect UpdateRect() const { return m_PaintStruct.rcPaint; }

	operator HDC() const { return m_MemoryDC.m_hDC; }       //  DC handle for API functions
};