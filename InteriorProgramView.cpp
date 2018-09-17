
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
	ON_COMMAND(ID_EDIT_EDIT, &CInteriorProgramView::OnEditEdit)
	ON_COMMAND(ID_EDIT_DELETE, &CInteriorProgramView::OnEditDelete)
	ON_COMMAND(ID_EDIT_ROTATE, &CInteriorProgramView::OnEditRotate)
	ON_COMMAND(ID_EDIT_MY_COPY, &CInteriorProgramView::OnEditMyCopy)
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
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
	CPoint temp = point;

	//screen 상에서 창을 막 움직여도 Client에 맞게 좌표값을 변경해줌 때문에 Temp로 받아놔야 함
	ClientToScreen(&point);		
	OnContextMenu(this, point);

	PointCheck(temp);

	//먼저 선택되어 있던 것을 모두 놔 버림
	m_pCShapeHandler->InitSelect();
	if (GlobalNum::getInstance()->nPaintStatus == PAINT_BASIC) //기본 상태에선 선택
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

	//  PAINT_BASIC, PAINT_ROOM, PAINT_WINDOW, PAINT_DOOR,
	//  PAINT_TR1, PAINT_TR2, PAINT_TR3, PAINT_USER_ADD, PAINT_USER_WASH, PAINT_USER_TV
	switch (GlobalNum::getInstance()->nPaintStatus)
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
		PointCheck(point);
		m_CurrentMouse = point;
		if (GlobalNum::getInstance()->nPaintStatus == PAINT_BASIC) //Mouse Move
		{
			m_pCShapeHandler->Move(m_CurrentMouse);
		}
		else if (GlobalNum::getInstance()->nPaintStatus == PAINT_WINDOW || GlobalNum::getInstance()->nPaintStatus == PAINT_DOOR)
		{
			m_pCShapeHandler->SetDoorWindowRange(m_bDragFlag, m_OldMouse ,m_CurrentMouse);
		}
		RedrawWindow();
	}

	CView::OnMouseMove(nFlags, point);
}
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


	switch (GlobalNum::getInstance()->nPaintStatus)
	{

//////////////////////////////////////////////////////////////////////////
/// Object Draw
	case PAINT_BASIC:
		m_pCShapeHandler->Move(m_CurrentMouse); //Mouse Move
		break;
	case PAINT_ROOM:
		m_pCShapeHandler->AddShape(tempX, tempY, tempWidth, tempHeight);
		break;
	case PAINT_WINDOW:
		m_pCShapeHandler->SetDoorWindowRange(m_bDragFlag, m_OldMouse ,m_CurrentMouse);
		m_pCShapeHandler->AddShape(m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y);
		break;
	case PAINT_DOOR:
		m_pCShapeHandler->SetDoorWindowRange(m_bDragFlag, m_OldMouse, m_CurrentMouse);
		m_pCShapeHandler->AddShape(m_OldMouse.x, m_OldMouse.y, m_CurrentMouse.x, m_CurrentMouse.y);
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
	GlobalNum::getInstance()->nPaintStatus = PAINT_BASIC; //마우스를 때면 베이직으로 돌아옴
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
	if (GlobalNum::getInstance()->nPaintStatus == PAINT_BASIC) //기본 상태에선 선택과 Move
	{
		m_pCShapeHandler->Select(point); 

		// Move를 위해 현재 선택된 위치와 기준 꼭지점과의 거리를 구함
		int index = m_pCShapeHandler->GetCurrentSelectedIndex();
		if (!(index == MY_ERROR))
		{
			Shape *tmpShape = m_pCShapeHandler->m_CaShape.at(index);

			if (typeid(*m_pCShapeHandler->m_CaShape.at(index)) == typeid(RoomShape))
			{
				RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(m_pCShapeHandler->m_CaShape.at(index));
				tmpRoomShape->m_nMoveSubVal[0] = tmpRoomShape->nX - point.x;
				tmpRoomShape->m_nMoveSubVal[1] = tmpRoomShape->nY - point.y;
				tmpRoomShape->m_nMoveSubVal[2] = tmpRoomShape->nWidth - point.x;
				tmpRoomShape->m_nMoveSubVal[3] = tmpRoomShape->nHeight - point.y;
#pragma warning(push)
#pragma warning(disable: 4018)
				for (long i = 0; i < tmpRoomShape->m_CaDoor.size(); i++)
#pragma warning(pop)
				{
					tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[0] = tmpRoomShape->m_CaDoor[i]->nX - point.x;
					tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[1] = tmpRoomShape->m_CaDoor[i]->nY - point.y;
					tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[2] = tmpRoomShape->m_CaDoor[i]->nWidth - point.x;
					tmpRoomShape->m_CaDoor[i]->m_nMoveSubVal[3] = tmpRoomShape->m_CaDoor[i]->nHeight - point.y;
				}
#pragma warning(push)
#pragma warning(disable: 4018)
				for (long i = 0; i < tmpRoomShape->m_CaWindow.size(); i++)
#pragma warning(pop)
				{
					tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[0] = tmpRoomShape->m_CaWindow[i]->nX - point.x;
					tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[1] = tmpRoomShape->m_CaWindow[i]->nY - point.y;
					tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[2] = tmpRoomShape->m_CaWindow[i]->nWidth - point.x;
					tmpRoomShape->m_CaWindow[i]->m_nMoveSubVal[3] = tmpRoomShape->m_CaWindow[i]->nHeight - point.y;
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
	else if (GlobalNum::getInstance()->nPaintStatus == PAINT_WINDOW || GlobalNum::getInstance()->nPaintStatus == PAINT_DOOR)
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
	SetFocus();
	cout << "dbClick\n" << endl;

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
void CInteriorProgramView::CheckPoint(CBufferDC & dc)
{
	CString strTemp = _T("");
	strTemp.Format(_T("%03d, %03d"), m_CurrentMouse.x, m_CurrentMouse.y);
	dc.TextOutW(10, 10, strTemp);
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


//////////////////////////////////////////////////////////////////////////
/// 마우스 우클릭 메뉴 이벤트
void CInteriorProgramView::OnEditEdit()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cout << "Edit\n";
}
void CInteriorProgramView::OnEditDelete()
{
	if (m_pCShapeHandler->DeleteSelectedShape() == MY_ERROR)
	{
		cout << "No Delete\n";
	}
	RedrawWindow();
}
void CInteriorProgramView::OnEditRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cout << "Rotate\n";
}
void CInteriorProgramView::OnEditMyCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cout << "Copy\n";
	
	int index = m_pCShapeHandler->GetCurrentSelectedIndex();

	if (index == MY_ERROR)
	{
		cout << "Cant Copy, Anything Selected" << endl;
	}
	else
	{
		Shape *tmpShape = m_pCShapeHandler->m_CaShape[index];

		if (typeid(*tmpShape) == typeid(RoomShape)) // Room을 선택해서 복사하려 할 때!,  그룹화 복사 사용 안 함!
		{
			RoomShape *CNewCopyShape = new RoomShape(* dynamic_cast<RoomShape*>(tmpShape));
			CNewCopyShape->SetId(m_pCShapeHandler->MakeAutoIncId()); // Id를 다시 부여해야 함!

			CNewCopyShape->nX +=10;
			CNewCopyShape->nWidth += 10;
			CNewCopyShape->nY += 10;
			CNewCopyShape->nHeight += 10;

			if (CNewCopyShape->nX < 0)
			{
				CNewCopyShape->nX = 0;
			}
			if (CNewCopyShape->nWidth > 765)
			{
				CNewCopyShape->nWidth = 765;
			}
			if (CNewCopyShape->nY < 0)
			{
				CNewCopyShape->nY = 0;
			}
			if (CNewCopyShape->nHeight > 720)
			{
				CNewCopyShape->nHeight = 720;
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_ROOM; // Room 생성 상태로 바꿈
			m_pCShapeHandler->AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // 기본 상태로 다시 되돌아옴!
			RedrawWindow();
		}
		else if (typeid(*tmpShape) == typeid(DoorShape)) // Door를 선택해서 복사하려 할 때!
		{
			DoorShape *CNewCopyShape = new DoorShape(*dynamic_cast<DoorShape*>(tmpShape));
			CNewCopyShape->SetId(m_pCShapeHandler->MakeAutoIncId()); // Id를 다시 부여해야 함!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_pCShapeHandler->m_nDrawRange * 2) == CNewCopyShape->nWidth) //왼쪽 혹은 오른쪽에 있다면
			{
				//cout << "왼오\n";

				//////////////////////////////////////////////////////////////////////////
				// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nY += 10;
				CNewCopyShape->nHeight += 10;

				if (CNewCopyShape->nY < tmpRoomShape->nY)
				{
					CNewCopyShape->nY = tmpRoomShape->nY;
				}
				else if (CNewCopyShape->nHeight > tmpRoomShape->nHeight)
				{
					CNewCopyShape->nHeight = tmpRoomShape->nHeight;
				}
			}
			else if (CNewCopyShape->nY + (m_pCShapeHandler->m_nDrawRange * 2) == CNewCopyShape->nHeight) //위쪽 혹은 아래쪽에 있다면
			{
				//cout << "위아래\n";

				//////////////////////////////////////////////////////////////////////////
				// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nX += 10;
				CNewCopyShape->nWidth += 10;

				if (CNewCopyShape->nX < tmpRoomShape->nX)
				{
					CNewCopyShape->nX = tmpRoomShape->nX;
				}
				else if (CNewCopyShape->nWidth > tmpRoomShape->nWidth)
				{
					CNewCopyShape->nWidth = tmpRoomShape->nWidth;
				}
			}
			else
			{
				cout << "창문, 문 단일 Copy Error\n";
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_DOOR; // Door 생성 상태로 바꿈
			m_pCShapeHandler->AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // 기본 상태로 다시 되돌아옴!
			RedrawWindow();
		}
		else if (typeid(*tmpShape) == typeid(WindowShape)) // Window를 선택해서 복사하려 할 때!
		{
			WindowShape *CNewCopyShape = new WindowShape(*dynamic_cast<WindowShape*>(tmpShape));
			CNewCopyShape->SetId(m_pCShapeHandler->MakeAutoIncId()); // Id를 다시 부여해야 함!
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->pInRoomShapePointer);

			if (CNewCopyShape->nX + (m_pCShapeHandler->m_nDrawRange * 2) == CNewCopyShape->nWidth) //왼쪽 혹은 오른쪽에 있다면
			{
				//cout << "왼오\n";

				//////////////////////////////////////////////////////////////////////////
				// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nY += 10;
				CNewCopyShape->nHeight += 10;

				if (CNewCopyShape->nY < tmpRoomShape->nY)
				{
					CNewCopyShape->nY = tmpRoomShape->nY;
				}
				else if (CNewCopyShape->nHeight > tmpRoomShape->nHeight)
				{
					CNewCopyShape->nHeight = tmpRoomShape->nHeight;
				}
			}
			else if (CNewCopyShape->nY + (m_pCShapeHandler->m_nDrawRange * 2) == CNewCopyShape->nHeight) //위쪽 혹은 아래쪽에 있다면
			{
				//cout << "위아래\n";

				//////////////////////////////////////////////////////////////////////////
				// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

				CNewCopyShape->nX += 10;
				CNewCopyShape->nWidth += 10;

				if (CNewCopyShape->nX < tmpRoomShape->nX)
				{
					CNewCopyShape->nX = tmpRoomShape->nX;
				}
				else if (CNewCopyShape->nWidth > tmpRoomShape->nWidth)
				{
					CNewCopyShape->nWidth = tmpRoomShape->nWidth;
				}
			}
			else
			{
				cout << "창문, 문 단일 Copy Error\n";
			}

			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_WINDOW; // Window 생성 상태로 바꿈
			m_pCShapeHandler->AddShape(CNewCopyShape->nX, CNewCopyShape->nY, CNewCopyShape->nWidth, CNewCopyShape->nHeight);
			GlobalNum::getInstance()->nPaintStatus = GlobalNum::getInstance()->PAINT_BASIC; // 기본 상태로 다시 되돌아옴!
			RedrawWindow();
		}
		else
		{
			cout << "Copy Error\n" << endl;
		}
	}
}


BOOL CInteriorProgramView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int index = m_pCShapeHandler->GetCurrentSelectedIndex();
	if (index == MY_ERROR)
	{
		cout << "Cant Wheel, Anything Selected" << endl;
		return TRUE;
	}

	Shape *tmpShape = m_pCShapeHandler->m_CaShape[index];

	if (typeid(*tmpShape) == typeid(RoomShape)) // Room을 선택해서 Scale하려 할 때!,  그룹화 Scale 사용 안 함!
	{
		if (zDelta > 100) //크게 할 때
		{
			if (tmpShape->nX < 5 || tmpShape->nWidth > 740 || tmpShape->nY < 5 || tmpShape->nHeight > 700)
				return TRUE;

			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape);

			for (auto pIter : tmpRoomShape->m_CaDoor) //Room안의 Door!
			{	
				if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
				{
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
				{
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nY) //위쪽이면
				{
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
				{
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else
				{
					cout << " 문 그룹 Wheel Error\n";
					return TRUE;
				}
			}
			for (auto pIter : tmpRoomShape->m_CaWindow)
			{
				if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
				{
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
				{
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nY) //위쪽이면
				{
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
				{
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else
				{
					cout << " 창문 그룹 Wheel Error\n";
					return TRUE;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 성공 방 크기 조정, 제일 마지막에
			tmpShape->nX -= 2;
			tmpShape->nWidth += 2;
			tmpShape->nY -= 2;
			tmpShape->nHeight += 2;
		}
		else //작게 할 때
		{
			RoomShape *tmpRoomShape = dynamic_cast<RoomShape*>(tmpShape);

			//////////////////////////////////////////////////////////////////////////
			// 전체 크기 체크
			if (tmpShape->nWidth - tmpShape->nX < 30 || tmpShape->nHeight - tmpShape->nY < 30 )
			{
				return TRUE;
			}

			for (auto pIter : tmpRoomShape->m_CaDoor) //Room안의 Door!
			{
				if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
				{
					if (tmpRoomShape->nHeight < pIter->nHeight || tmpRoomShape->nY > pIter->nY)
					{
						return TRUE;
					}
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
				{
					if (tmpRoomShape->nHeight < pIter->nHeight || tmpRoomShape->nY > pIter->nY)
					{
						return TRUE;
					}
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nY) //위쪽이면
				{
					if (tmpRoomShape->nWidth < pIter->nWidth || tmpRoomShape->nX > pIter->nX)
					{
						return TRUE;
					}
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
				{
					if (tmpRoomShape->nWidth < pIter->nWidth || tmpRoomShape->nX > pIter->nX)
					{
						return TRUE;
					}
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else
				{
					cout << " 문 그룹 Wheel Error\n";
					return TRUE;
				}
			}
			for (auto pIter : tmpRoomShape->m_CaWindow)
			{
				if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nX) //왼쪽이면
				{
					if (tmpRoomShape->nHeight < pIter->nHeight || tmpRoomShape->nY > pIter->nY)
					{
						return TRUE;
					}
					pIter->nX += 2;
					pIter->nWidth += 2;
				}
				else if (pIter->nX + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nWidth) //오른쪽이면
				{
					if (tmpRoomShape->nHeight < pIter->nHeight || tmpRoomShape->nY > pIter->nY)
					{
						return TRUE;
					}
					pIter->nX -= 2;
					pIter->nWidth -= 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nY) //위쪽이면
				{
					if (tmpRoomShape->nWidth < pIter->nWidth || tmpRoomShape->nX > pIter->nX)
					{
						return TRUE;
					}
					pIter->nY += 2;
					pIter->nHeight += 2;
				}
				else if (pIter->nY + m_pCShapeHandler->m_nDrawRange == tmpRoomShape->nHeight) //아래쪽이면
				{
					if (tmpRoomShape->nWidth < pIter->nWidth || tmpRoomShape->nX > pIter->nX)
					{
						return TRUE;
					}
					pIter->nY -= 2;
					pIter->nHeight -= 2;
				}
				else
				{
					cout << " 창문 그룹 Wheel Error\n";
					return TRUE;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 성공 방 크기 조정, 제일 마지막에
			tmpShape->nX += 2;
			tmpShape->nWidth -= 2;
			tmpShape->nY += 2;
			tmpShape->nHeight -= 2;
		}
	}
	else // 문이나 창문 단일 선택 Scale
	{
		RoomShape *tmpRoomShape;

		if (typeid(*tmpShape) == typeid(DoorShape)) //문일때
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<DoorShape*>(tmpShape)->pInRoomShapePointer);
		}
		else	//창문일때
		{
			tmpRoomShape = dynamic_cast<RoomShape*>(dynamic_cast<WindowShape*>(tmpShape)->pInRoomShapePointer);
		}
		

		if (tmpShape->nX + (m_pCShapeHandler->m_nDrawRange * 2) == tmpShape->nWidth) //왼쪽 혹은 오른쪽에 있다면
		{
			//cout << "왼오\n";

			//////////////////////////////////////////////////////////////////////////
			// X 축은 고정하고, Y축만 RoomShape의 범위 밖을 나가지 않게 고정함

			if (zDelta > 100)
			{
				if (!(tmpShape->nY < tmpRoomShape->nY))
				{
					tmpShape->nY -= 2;
				}
				if (!(tmpShape->nHeight > tmpRoomShape->nHeight))
				{
					tmpShape->nHeight += 2;
				}
			}
			else
			{
				if (!(tmpShape->nHeight - tmpShape->nY < 15))
				{
					tmpShape->nY += 2;
					tmpShape->nHeight -= 2;
				}
			}
		}
		else if (tmpShape->nY + (m_pCShapeHandler->m_nDrawRange * 2) == tmpShape->nHeight) //위쪽 혹은 아래쪽에 있다면
		{
			//cout << "위아래\n";

			//////////////////////////////////////////////////////////////////////////
			// Y 축은 고정하고, X축만 RoomShape의 범위 밖을 나가지 않게 고정함

			if (zDelta > 100)
			{
				if (!(tmpShape->nX < tmpRoomShape->nX))
				{
					tmpShape->nX -= 2;
				}
				if (!(tmpShape->nWidth > tmpRoomShape->nWidth))
				{
					tmpShape->nWidth += 2;
				}
			}
			else
			{
				if (!(tmpShape->nWidth - tmpShape->nX < 15))
				{
					tmpShape->nX += 2;
					tmpShape->nWidth -= 2;
				}
			}
		}
		else
		{
			cout << "창문, 문 단일 Copy Error\n";
		}
	}
	RedrawWindow();


	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
