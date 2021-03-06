
// InteriorProgramView.h: CInteriorProgramView 클래스의 인터페이스
//

#pragma once

#include "BufferDC.h"
#include "ShapeHandler.h"
#include "RoomShape.h"
#include "MainFrm.h"
#include "ControlView.h"

class CInteriorProgramView : public CView
{
protected: // serialization에서만 만들어집니다.
	CInteriorProgramView() noexcept;
	DECLARE_DYNCREATE(CInteriorProgramView)

// 특성입니다.
public:
	CInteriorProgramDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CInteriorProgramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// 생성된 메시지 맵 함수
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
///커스텀
public:
	void InitDrawVector(CBufferDC& dc);
	void InitBackground(CBufferDC& dc);
	void DrawRoom(CBufferDC& dc);
	void DrawWindow(CBufferDC& dc);
	void DrawDoor(CBufferDC& dc);
	void DrawUserAdd(CBufferDC& dc);
	void CheckPoint(CBufferDC& dc);

	void PointCheck(CPoint &point);

private:
	CString m_strBGImgPath;
	CImage m_BGImg;
	CRect m_WinRect;
	CPoint m_CurrentMouse;
	CPoint m_OldMouse;

	bool m_bDragFlag;

	CShapeHandler *m_pCShapeHandler; //수정

	
public:
	afx_msg void OnEditEdit();
	afx_msg void OnEditDelete();
	afx_msg void OnEditRotate();
	afx_msg void OnEditMyCopy();
	afx_msg void OnEditAllDelete();
	afx_msg void OnEditAllCaptureMenu();
};

#ifndef _DEBUG  // InteriorProgramView.cpp의 디버그 버전
inline CInteriorProgramDoc* CInteriorProgramView::GetDocument() const
   { return reinterpret_cast<CInteriorProgramDoc*>(m_pDocument); }
#endif

