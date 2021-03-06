//
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
#include <Gdiplus.h>
#include "WindowShape.h"
#include "DoorShape.h"
#include "ObjectShape.h"
#include "ShapeFactory.h"

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
	ON_COMMAND(ID_EDIT_EDOT_MENU, &CInteriorProgramView::OnEditEdit)
	ON_COMMAND(ID_EDIT_DELETE_MENU, &CInteriorProgramView::OnEditDelete)
	ON_COMMAND(ID_EDIT_ROTATE_MENU, &CInteriorProgramView::OnEditRotate)
	ON_COMMAND(ID_EDIT_MY_COPY_MENU, &CInteriorProgramView::OnEditMyCopy)
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_ALL_DELETE_MENU, &CInteriorProgramView::OnEditAllDelete)
	ON_COMMAND(ID_EDIT_ALL_CAPTURE_MENU, &CInteriorProgramView::OnEditAllCaptureMenu)
END_MESSAGE_MAP()

// CInteriorProgramView 생성/소멸
CInteriorProgramView::CInteriorProgramView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	GlobalNum::nPaintStatus = PAINT_BASIC;
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


// View 생성 이벤트
int CInteriorProgramView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pCShapeHandler = CShapeHandler::GetInstance();

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}

// 그래픽 OnPaint
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

	//  PAINT_BASIC, PAINT_ROOM, PAINT_WINDOW, PAINT_DOOR
	switch (GlobalNum::nPaintStatus)
	{
//////////////////////////////////////////////////////////////////////////
/// Object Draw
	case PAINT_BASIC:
		break;
	case PAINT_ROOM:
		DrawRoom(dc);
		break;
	case PAINT_WINDOW:
		DrawWindow(dc);
		break;
	case PAINT_DOOR:
		DrawDoor(dc);
		break;
	default:
		cout << "Paint Error" << endl;
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
// 이벤트 처리기


// 마우스 관련 함수들
void CInteriorProgramView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (m_bDragFlag)
	{
		PointCheck(point);
		m_CurrentMouse = point;
		if (GlobalNum::nPaintStatus == PAINT_BASIC) //Mouse Move
		{
			m_pCShapeHandler->Move(m_CurrentMouse);
			m_pCShapeHandler->MagneticSelectedShape();
		}
		else if (GlobalNum::nPaintStatus == PAINT_WINDOW || GlobalNum::nPaintStatus == PAINT_DOOR)
		{
			m_pCShapeHandler->SetDoorWindowRange(m_bDragFlag, m_OldMouse ,m_CurrentMouse);
		}
		RedrawWindow();
	}

	CView::OnMouseMove(nFlags, point);
}
// 마우스 왼쪽 버튼 클릭
void CInteriorProgramView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	m_bDragFlag = false;
	PointCheck(point);
	m_CurrentMouse = point;

	// 각각 네모를 그리는 위치가 다름!!
	// 때문에 shape의 x,y,width,height 의 값은 통상적으로 왼쪽위에서 아래로 그릴때의 값으로 정함
	// 즉 기준 꼭지점을 왼쪽 위로 함
	int tempX, tempY, tempWidth, tempHeight; 
	if (m_OldMouse.x >= m_CurrentMouse.x)
	{
		tempX = m_CurrentMouse.x;
		tempWidth = m_OldMouse.x;
	}
	else
	{
		tempX = m_OldMouse.x;
		tempWidth = m_CurrentMouse.x;
	}
	if (m_OldMouse.y >= m_CurrentMouse.y)
	{
		tempY = m_CurrentMouse.y;
		tempHeight = m_OldMouse.y;
	}
	else
	{
		tempY = m_OldMouse.y;
		tempHeight = m_CurrentMouse.y;
	}


	switch (GlobalNum::nPaintStatus)
	{

//////////////////////////////////////////////////////////////////////////
/// Object Draw
	case PAINT_BASIC:
		m_pCShapeHandler->Move(m_CurrentMouse); //Mouse Move
		m_pCShapeHandler->MagneticSelectedShape(); //Magnetic Object
		// 혹시 편집하고 있을 때 다시 편집창 설정
		{
			//특정 View 포인터 얻는 법
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고
			//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
			ControlView* pView = (ControlView*)pFrame->m_wndSplitter.GetPane(0, 1); // 활성화된 뷰의 포인터 구한다.
			pView->m_pDlgTab2->SetDlgItem();
		}
		break;
	case PAINT_ROOM:
		m_pCShapeHandler->AddShape(CShapeFactory::GetInstance()->GetShapeWhenPaint(GlobalNum::nPaintStatus, tempX, tempY, tempWidth, tempHeight));
		break;
	case PAINT_WINDOW:
	case PAINT_DOOR:		
		m_pCShapeHandler->SetDoorWindowRange(m_bDragFlag, m_OldMouse ,m_CurrentMouse);
		m_pCShapeHandler->AddShape(CShapeFactory::GetInstance()->GetShapeWhenPaint(GlobalNum::nPaintStatus, m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y));
		break;
	default:
		cout << "On L Button Up Error" << endl;
		break;
	}

	RedrawWindow();
	ReleaseCapture();
	GlobalNum::nPaintStatus = PAINT_BASIC; //마우스를 때면 베이직으로 돌아옴
	CView::OnLButtonUp(nFlags, point);
}
void CInteriorProgramView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bDragFlag = true;
	PointCheck(point);


	m_CurrentMouse = point;
	m_OldMouse = point;


	//먼저 선택되어 있던 것을 모두 놔 버림
	m_pCShapeHandler->InitSelect();
	if (GlobalNum::nPaintStatus == PAINT_BASIC) //기본 상태에선 선택과 Move
	{
		m_pCShapeHandler->Select(point); 

		// Move를 위해 현재 선택된 위치와 기준 꼭지점과의 거리를 구함
		int index = m_pCShapeHandler->GetCurrentSelectedIndex();
		if (!(index == MY_ERROR))
		{
			CShape *tmpShape = m_pCShapeHandler->m_CaShape.at(index);

			if (m_pCShapeHandler->m_CaShape.at(index)->m_nType == TYPE_BASIC_CONTAINER) // Room Shape 일 때
			{
				CBasicContainerShape *tmpBasicContainerShape = dynamic_cast<CBasicContainerShape*>(m_pCShapeHandler->m_CaShape.at(index));
				tmpBasicContainerShape->m_nMoveSubVal[0] = tmpBasicContainerShape->nX - point.x;
				tmpBasicContainerShape->m_nMoveSubVal[1] = tmpBasicContainerShape->nY - point.y;
				tmpBasicContainerShape->m_nMoveSubVal[2] = tmpBasicContainerShape->nWidth - point.x;
				tmpBasicContainerShape->m_nMoveSubVal[3] = tmpBasicContainerShape->nHeight - point.y;
#pragma warning(push)
#pragma warning(disable: 4018)
				for (long i = 0; i < tmpBasicContainerShape->m_CaDependent.size(); i++)
#pragma warning(pop)
				{
					tmpBasicContainerShape->m_CaDependent[i]->m_nMoveSubVal[0] = tmpBasicContainerShape->m_CaDependent[i]->nX - point.x;
					tmpBasicContainerShape->m_CaDependent[i]->m_nMoveSubVal[1] = tmpBasicContainerShape->m_CaDependent[i]->nY - point.y;
					tmpBasicContainerShape->m_CaDependent[i]->m_nMoveSubVal[2] = tmpBasicContainerShape->m_CaDependent[i]->nWidth - point.x;
					tmpBasicContainerShape->m_CaDependent[i]->m_nMoveSubVal[3] = tmpBasicContainerShape->m_CaDependent[i]->nHeight - point.y;
				}
			}
			else
			{
				tmpShape->m_nMoveSubVal[0] = tmpShape->nX - point.x;
				tmpShape->m_nMoveSubVal[1] = tmpShape->nY - point.y;
				tmpShape->m_nMoveSubVal[2] = tmpShape->nWidth - point.x;
				tmpShape->m_nMoveSubVal[3] = tmpShape->nHeight - point.y;
			}
		}
	}
	// Door나 Window는 마그네틱, 스냅, 크기 제어 해줘야 함
	else if (GlobalNum::nPaintStatus == PAINT_WINDOW || GlobalNum::nPaintStatus == PAINT_DOOR)
	{
		m_pCShapeHandler->SetDoorWindowRange(m_bDragFlag, m_OldMouse, m_CurrentMouse);
	}


	RedrawWindow();
	SetCapture();

	CView::OnLButtonDown(nFlags, point);
}
void CInteriorProgramView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	cout << "dbClick\n" << endl;

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_pCShapeHandler->GetCurrentSelectedIndex() == MY_ERROR)
	{
		cout << "No Edit\n";
		return;
	}

	//특정 View 포인터 얻는 법
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고
	//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
	ControlView* pView = (ControlView*)pFrame->m_wndSplitter.GetPane(0, 1); // 활성화된 뷰의 포인터 구한다.
	pView->m_pDlgTab2->SetDlgItem();

	pView->ShowEditTab();


	CView::OnLButtonDblClk(nFlags, point);
}
// 마우스 오른쪽 버튼 클릭 + 메뉴
void CInteriorProgramView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	CPoint temp = point;

	//screen 상에서 창을 막 움직여도 Client에 맞게 좌표값을 변경해줌 때문에 Temp로 받아놔야 함
	ClientToScreen(&point);
	OnContextMenu(this, point);

	PointCheck(temp);

	//먼저 선택되어 있던 것을 모두 놔 버림
	m_pCShapeHandler->InitSelect();
	if (GlobalNum::nPaintStatus == PAINT_BASIC) //기본 상태에선 선택
	{
		m_pCShapeHandler->Select(temp);
	}
	RedrawWindow();
}
void CInteriorProgramView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
// 마우스 가운데 버튼
void CInteriorProgramView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnMButtonDown(nFlags, point);
}
// 마우스 휠
BOOL CInteriorProgramView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pCShapeHandler->WheelSelectedShape(zDelta) == MY_ERROR)
	{
		cout << "No Wheel\n";
		return TRUE;
	}
	RedrawWindow();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
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
		ShapePen.CreatePen(PS_SOLID, 1, RGB(m_pCShapeHandler->m_CaShape.at(i)->nRed, m_pCShapeHandler->m_CaShape.at(i)->nGreen, m_pCShapeHandler->m_CaShape.at(i)->nBlue));
		dc.SelectObject(ShapePen);

		if (m_pCShapeHandler->m_CaShape[i]->m_nType == TYPE_OBJECT) // Object는 Icon 나머지는 Paint
		{
			dc.Rectangle(m_pCShapeHandler->m_CaShape.at(i)->nX, m_pCShapeHandler->m_CaShape.at(i)->nY,
				m_pCShapeHandler->m_CaShape.at(i)->nWidth, m_pCShapeHandler->m_CaShape.at(i)->nHeight);
			dc.DrawIcon(m_pCShapeHandler->m_CaShape.at(i)->nX, m_pCShapeHandler->m_CaShape.at(i)->nY, 
				dynamic_cast<CObjectShape*>(m_pCShapeHandler->m_CaShape[i])->m_icon);
		}
		else
		{
			dc.Rectangle(m_pCShapeHandler->m_CaShape.at(i)->nX, m_pCShapeHandler->m_CaShape.at(i)->nY,
				m_pCShapeHandler->m_CaShape.at(i)->nWidth, m_pCShapeHandler->m_CaShape.at(i)->nHeight);
		}

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
	if (m_bDragFlag)
	{
		// 투명 사각형 그리기!
		CBrush RoomBrush;
		RoomBrush.CreateStockObject(NULL_BRUSH);
		dc.SelectObject(RoomBrush);

		///색 초기화
		CPen ShapePen;
		ShapePen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		dc.SelectObject(ShapePen);

		dc.Rectangle(m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y);
		RoomBrush.DeleteObject();
	}
}
void CInteriorProgramView::DrawWindow(CBufferDC & dc)
{
	if (m_bDragFlag)
	{
		// 투명 사각형 그리기!
		CBrush RoomBrush;
		RoomBrush.CreateStockObject(NULL_BRUSH);
		dc.SelectObject(RoomBrush);

		///색 초기화
		CPen ShapePen;
		ShapePen.CreatePen(PS_SOLID, 1, RGB(80, 188, 223));
		dc.SelectObject(ShapePen);

		dc.Rectangle(m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y);
		RoomBrush.DeleteObject();

	}
}
void CInteriorProgramView::DrawDoor(CBufferDC & dc)
{
	if (m_bDragFlag)
	{
		// 투명 사각형 그리기!
		CBrush RoomBrush;
		RoomBrush.CreateStockObject(NULL_BRUSH);
		dc.SelectObject(RoomBrush);

		///색 초기화
		CPen ShapePen;
		ShapePen.CreatePen(PS_SOLID, 1, RGB(211, 162, 127));
		dc.SelectObject(ShapePen);

		dc.Rectangle(m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y);
		RoomBrush.DeleteObject();
	}
}
void CInteriorProgramView::DrawUserAdd(CBufferDC & dc)
{
	//https://msdn.microsoft.com/ko-kr/library/9xc4z2c7.aspx  CImageList 사용 법

	if (m_bDragFlag)
	{
		dc.Rectangle(m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y);
	}
}
void CInteriorProgramView::PointCheck(CPoint & point) //Point의 범위가 InteriorProgramView의 범위 밖으로 나가지 않게 막아줌
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
}
void CInteriorProgramView::CheckPoint(CBufferDC & dc)
{
	CString strTemp = _T("");
	strTemp.Format(_T("%03d, %03d"), m_CurrentMouse.x, m_CurrentMouse.y);
	dc.TextOutW(10, 10, strTemp);
}

//////////////////////////////////////////////////////////////////////////
/// 마우스 우클릭 메뉴 이벤트
void CInteriorProgramView::OnEditEdit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_pCShapeHandler->GetCurrentSelectedIndex() == MY_ERROR)
	{
		cout << "No Edit\n";
		return;
	}

	//특정 View 포인터 얻는 법
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();  // 프레임 윈도우 포인터 구하고
	//CInteriorProgramView* pView = (CInteriorProgramView*)pFrame->GetActiveView(); // 활성화된 뷰의 포인터 구한다.
	ControlView* pView = (ControlView*)pFrame->m_wndSplitter.GetPane(0, 1); // 활성화된 뷰의 포인터 구한다.
	pView->m_pDlgTab2->SetDlgItem();

	pView->ShowEditTab();
}
void CInteriorProgramView::OnEditRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_pCShapeHandler->RotateSelectedShape() == MY_ERROR)
	{
		cout << "No Rotate\n";
	}

	cout << "Not yet" << endl;
}
void CInteriorProgramView::OnEditDelete()
{
	if (m_pCShapeHandler->DeleteSelectedShape() == MY_ERROR)
	{
		cout << "No Delete\n";
	}
	RedrawWindow();
}
void CInteriorProgramView::OnEditMyCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_pCShapeHandler->CopySelectedShape() == MY_ERROR)
	{
		cout << "No Copy\n";
	}
	RedrawWindow();
}
void CInteriorProgramView::OnEditAllDelete()
{
	CShapeHandler::GetInstance()->Clear();

	RedrawWindow();
}
void CInteriorProgramView::OnEditAllCaptureMenu()
{
	JunUtil::SpecificAreaCapture(this);
}