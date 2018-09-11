
// InteriorProgramView.cpp: CInteriorProgramView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "InteriorProgram.h"
#endif

#include "InteriorProgramDoc.h"
#include "InteriorProgramView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInteriorProgramView

IMPLEMENT_DYNCREATE(CInteriorProgramView, CView)

BEGIN_MESSAGE_MAP(CInteriorProgramView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CInteriorProgramView 생성/소멸

CInteriorProgramView::CInteriorProgramView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC;
	m_bDragFlag = FALSE;
}

CInteriorProgramView::~CInteriorProgramView()
{
}

BOOL CInteriorProgramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CInteriorProgramView 그리기

void CInteriorProgramView::OnDraw(CDC* /*pDC*/)
{
	CInteriorProgramDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CInteriorProgramView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CInteriorProgramView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CInteriorProgramView 진단

#ifdef _DEBUG
void CInteriorProgramView::AssertValid() const
{
	CView::AssertValid();
}

void CInteriorProgramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInteriorProgramDoc* CInteriorProgramView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInteriorProgramDoc)));
	return (CInteriorProgramDoc*)m_pDocument;
}
#endif //_DEBUG


// CInteriorProgramView 메시지 처리기


int CInteriorProgramView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pCShapeHandler = ShapeHandler::GetInstance();

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}


void CInteriorProgramView::OnPaint()
{
	CBufferDC dc(this); // device context for painting
	CPen InitBlackPen;	//검은 Pen!
	CBrush InitWhiteBrush; //하얀 Brush!
	InitBlackPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	InitWhiteBrush.CreateSolidBrush(RGB(255, 255, 255));
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	/// 배경 눈금 그리기
	InitBackground(dc);

	// 검은색으로 초기화
	dc.SelectObject(InitBlackPen);

	/// 마우스 이동시 좌표 표시하기
	CheckPoint(dc);

	/// 벡터에 있던 것 먼저 다 그리기
	InitDrawVector(dc);

	// 검은색으로 초기화
	dc.SelectObject(InitBlackPen);

	//  PAINT_BASIC, PAINT_ROOM, PAINT_WINDOW, PAINT_DOOR,
	//  PAINT_TR1, PAINT_TR2, PAINT_TR3, PAINT_USER_ADD, PAINT_USER_WASH, PAINT_USER_TV
	switch (GlobalNum::getInstance()->nPaintStatus)
	{

//////////////////////////////////////////////////////////////////////////
/// Object Draw
	case PAINT_BASIC:
		break;
	case PAINT_ROOM:
		/// Room 그리기	
		DrawRoom(dc);

		break;
	case PAINT_WINDOW:
		
		break;
	case PAINT_DOOR:

		break;
	case PAINT_TR1:
		break;
	case PAINT_TR2:
		break;
	case PAINT_TR3:
		break;
	case PAINT_USER_ADD:

		break;
	case PAINT_USER_WASH:

		break;
	case PAINT_USER_TV:

		break;


//////////////////////////////////////////////////////////////////////////
/// Object Edit
	case PAINT_EDIT:
		break;
	case PAINT_EDIT_COLOR:
		break;
	case PAINT_EDIT_MT:
		break;
	case PAINT_EDIT_REDO:
		break;
	case PAINT_EDIT_UNDO:
		break;


//////////////////////////////////////////////////////////////////////////
/// Default

	default:
		break;
	}


}



void CInteriorProgramView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_bDragFlag)
	{
		if (point.x < 0)
		{
			point.x = 0;
		}
		if (point.x > 765)
		{
			point.x = 765;
		}
		if (point.y < 0)
		{
			point.y = 0;
		}
		if (point.y > 720)
		{
			point.y = 720;
		}
		m_CurrentMouse = point;
		m_pCShapeHandler->move(m_OldMouse.x, m_OldMouse.y, point.x, point.y); //Mouse Move
		RedrawWindow();
	}

	CView::OnMouseMove(nFlags, point);
}
void CInteriorProgramView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bDragFlag = false;
	if (point.x < 0)
	{
		point.x = 0;
	}
	if (point.x > 765)
	{
		point.x = 765;
	}
	if (point.y < 0)
	{
		point.y = 0;
	}
	if (point.y > 720)
	{
		point.y = 720;
	}
	m_CurrentMouse = point;

	switch (GlobalNum::getInstance()->nPaintStatus)
	{

		//////////////////////////////////////////////////////////////////////////
		/// Object Draw
	case PAINT_BASIC:
		break;
	case PAINT_ROOM:
		m_pCShapeHandler->AddShape(m_OldMouse.x, m_OldMouse.y, point.x, point.y);
		break;
	case PAINT_WINDOW:

		break;
	case PAINT_DOOR:

		break;
	case PAINT_TR1:

		break;
	case PAINT_TR2:

		break;
	case PAINT_TR3:

		break;
	case PAINT_USER_ADD:

		break;
	case PAINT_USER_WASH:

		break;
	case PAINT_USER_TV:

		break;


//////////////////////////////////////////////////////////////////////////
/// Object Edit
	case PAINT_EDIT:
		break;
	case PAINT_EDIT_COLOR:
		break;
	case PAINT_EDIT_MT:
		break;
	case PAINT_EDIT_REDO:
		break;
	case PAINT_EDIT_UNDO:
		break;


//////////////////////////////////////////////////////////////////////////
/// Default
	default:
		break;
	}

	RedrawWindow();
	ReleaseCapture();
	m_pCShapeHandler->move(m_OldMouse.x, m_OldMouse.y, point.x, point.y); //Mouse Move
	GlobalNum::getInstance()->nPaintStatus = PAINT_BASIC; //마우스를 때면 베이직으로 돌아옴
	CView::OnLButtonUp(nFlags, point);
}
void CInteriorProgramView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bDragFlag = true;
	if (point.x < 0)
	{
		point.x = 0;
	}
	if (point.x > 765)
	{
		point.x = 765;
	}
	if (point.y < 0)
	{
		point.y = 0;
	}
	if (point.y > 720)
	{
		point.y = 720;
	}

	//먼저 선택되어 있던 것을 모두 놔 버림
	m_pCShapeHandler->InitSelect();
	if (GlobalNum::getInstance()->nPaintStatus == PAINT_BASIC) //기본 상태에선 선택
	{
		m_pCShapeHandler->Select(point); 
	}


	m_CurrentMouse = point;
	m_OldMouse = point;
	RedrawWindow();
	SetCapture();


	CView::OnLButtonDown(nFlags, point);
}
void CInteriorProgramView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonDblClk(nFlags, point);
}


void CInteriorProgramView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMButtonDown(nFlags, point);
}



//////////////////////////////////////////////////////////////////////////
/// --- repaint 시 깜빡임 없애기
/// 1. CBufferDC 클래스 생성!
/// 2. 여기 원래 주석하고 이거로 바꾸기
/// 3. CPaintDC 를 CBufferDC로 바꾸면 끝
BOOL CInteriorProgramView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/// OnPaint 함수들!
void CInteriorProgramView::InitDrawVector(CBufferDC & dc)
{
	CBrush InitBrush;
	InitBrush.CreateStockObject(NULL_BRUSH);
	dc.SelectObject(InitBrush);

#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < m_pCShapeHandler->m_CaShape.size(); i++)
#pragma warning(pop)
	{
		///색 초기화
		CPen ShapePen;
		int red = m_pCShapeHandler->m_CaShape.at(i)->nRed, green = m_pCShapeHandler->m_CaShape.at(i)->nGreen,blue = m_pCShapeHandler->m_CaShape.at(i)->nBlue;
		ShapePen.CreatePen(PS_SOLID, 1, RGB(m_pCShapeHandler->m_CaShape.at(i)->nRed, m_pCShapeHandler->m_CaShape.at(i)->nGreen, m_pCShapeHandler->m_CaShape.at(i)->nBlue));
		dc.SelectObject(ShapePen);

		dc.Rectangle(m_pCShapeHandler->m_CaShape.at(i)->nX, m_pCShapeHandler->m_CaShape.at(i)->nY, 
			m_pCShapeHandler->m_CaShape.at(i)->nWidth, m_pCShapeHandler->m_CaShape.at(i)->nHeight);
		ShapePen.DeleteObject();
	}

	InitBrush.DeleteObject();
}

void CInteriorProgramView::InitBackground(CBufferDC& dc)
{
	GetWindowRect(m_WinRect);

	CPen pen;	//pen!
	pen.CreatePen(PS_SOLID, 1, RGB(0xE2, 0xE2, 0xE2));

	//배경만큼 바둑선 그리기
	dc.SelectObject(pen);
	int tx = 40, ty = 40, lastX = 0;
	for (int y = 0; ; y++) {
		for (int x = 0; ; x++) {
			dc.Rectangle((x)*tx, (y)*ty, (tx*(x + 1)) + 1, (ty*(y + 1)) + 1);
			if ((tx*(x + 2)) + 1 > m_WinRect.Width())
			{
				x++; // 마지막 넘어서 하나만 더 그림
				lastX = x;
				dc.Rectangle((x)*tx, (y)*ty, (tx*(x + 1)) + 1, (ty*(y + 1)) + 1);
				break;
			}
		}
		if ((ty*(y + 2)) + 1 > m_WinRect.Height())
		{
			y++; // y도 하나 더 그림
			for (int x = 0; x < lastX; x++) {
				dc.Rectangle((x)*tx, (y)*ty, (tx*(x + 1)) + 1, (ty*(y + 1)) + 1);
			}
			break;
		}
	}
	pen.DeleteObject();
}
void CInteriorProgramView::DrawRoom(CBufferDC & dc)
{
	// 투명 사각형 그리기!
	CBrush RoomBrush;
	RoomBrush.CreateStockObject(NULL_BRUSH);
	dc.SelectObject(RoomBrush);
	dc.Rectangle(m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y);
	RoomBrush.DeleteObject();
}
void CInteriorProgramView::CheckPoint(CBufferDC & dc)
{
	CString strTemp = _T("");
	strTemp.Format(_T("%03d, %03d"), m_CurrentMouse.x, m_CurrentMouse.y);
	dc.TextOutW(10, 10, strTemp);
}

void CInteriorProgramView::CheckMessage()
{
	MessageBox(L"확인");
}
