
// InteriorProgramView.h: CInteriorProgramView 클래스의 인터페이스
//

#pragma once

#include "CBufferDC.h"
#include "define.h"
#include "ShapeHandler.h"
#include "RoomShape.h"

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
	void CheckPoint(CBufferDC& dc);

	void PointCheck(CPoint &point);

private:
	CString m_strBGImgPath;
	CImage m_BGImg;
	CRect m_WinRect;
	CPoint m_CurrentMouse;
	CPoint m_OldMouse;

	bool m_bDragFlag;

	ShapeHandler *m_pCShapeHandler;

	enum {
		PAINT_BASIC, PAINT_ROOM, PAINT_WINDOW, PAINT_DOOR,
		PAINT_TR1, PAINT_TR2, PAINT_TR3, PAINT_USER_ADD, PAINT_USER_WASH, PAINT_USER_TV,
		PAINT_EDIT, PAINT_EDIT_CLEAR, PAINT_EDIT_UNDO, PAINT_EDIT_REDO, PAINT_EDIT_MT, PAINT_EDIT_COLOR


	}; // define랑 동기화 시켜줘야 됨...
	
public:
	afx_msg void OnEditEdit();
	afx_msg void OnEditDelete();
	afx_msg void OnEditRotate();
	afx_msg void OnEditMyCopy();
};

#ifndef _DEBUG  // InteriorProgramView.cpp의 디버그 버전
inline CInteriorProgramDoc* CInteriorProgramView::GetDocument() const
   { return reinterpret_cast<CInteriorProgramDoc*>(m_pDocument); }
#endif

