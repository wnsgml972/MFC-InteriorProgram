#include "stdafx.h"
#include "JunUtil.h"


//////////////////////////////////////////////////////////////////////////
// GlobalNum 초기화

int GlobalNum::nPaintStatus = 0;







//////////////////////////////////////////////////////////////////////////
// JunHee Util

// 문자열 컨트롤
std::string JunUtil::CStringToString(CString cstr)
{
	char caResult[100];

#pragma warning(push)
#pragma warning(disable: 4996)
	strcpy(caResult, CT2A(cstr));
	if (caResult == NULL)
	{
		strcpy(caResult, "");
	}
	caResult[strlen(caResult) + 1] = '\0';
#pragma warning(pop)

	return caResult;
}
CString JunUtil::StringToCString(std::string str)
{
	CString strResult;

	strResult = str.c_str();
	strResult += '\0';

	return strResult;
}
bool JunUtil::isNumber(std::string str)
{
	if (str.size() == 0)
		return false;

#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < str.size(); i++)
	{
#pragma warning(pop)
		if (( str[i] >= '0' && str[i] <= '9') == false) 
		{
			return false;
		}
	}
	return true;
}


// 원하는 작업 영역 캡쳐,  캡쳐 하고 싶은 영역의 Frame Param 전달
void JunUtil::SpecificAreaCapture(CWnd *pcwnd)
{
	// 바탕 화면 윈도우 객체에 대한 포인터를 얻는다.
	CWnd* pWndDesktop = pcwnd->GetDesktopWindow();
	// 바탕 화면 윈도우 DC
	CWindowDC ScrDC(pWndDesktop);
	// 뷰 윈도우 DC
	CClientDC dc(pcwnd);

	// Rect를 사용해서 작업 영역에 대한 좌표를 얻고,
	CRect Rect;
	pcwnd->GetClientRect(&Rect);

	// 그 위치를 현재 윈도우 절대 좌표로 변경해 주자.
	pcwnd->GetWindowRect(&Rect);

	// CImage를 하나 만들고
	CImage Image;
	// 스캔을 시작할 x, y 위치와
	int sx = Rect.left;
	int sy = Rect.top;
	// 작업 영역에 대한 크기를 각각 임시로 저장해 두자.
	int cx = Rect.Width();
	int cy = Rect.Height();

	// 작업 영역의 크기만큼, 현재 바탕화면의 속성과 동일한 image를 생성한다.
	Image.Create(cx, cy, ScrDC.GetDeviceCaps(BITSPIXEL));
	// 이미지 DC에 현재 작업 영역의 절대 좌표를 사용해 그 크기만큼 저장하게 한다.
	CDC* pDC = CDC::FromHandle(Image.GetDC());
	pDC->BitBlt(0, 0, cx, cy, &ScrDC, sx, sy, SRCCOPY);
	Image.ReleaseDC();

	// 저장된 이미지를 원하는 파일명, 포멧타입을 지정해서 저장한다.
	Image.Save(TEXT("media\\capture\\image.png"), Gdiplus::ImageFormatJPEG);

	// 그 파일을 실행해 준다.
	// 응용 프로그램을 실행시킬 수 있음!
	ShellExecute(NULL, TEXT("open"), TEXT("media\\capture\\image.png"), NULL, NULL, SW_SHOW);
}