#include "stdafx.h"
#include "JunUtil.h"


//////////////////////////////////////////////////////////////////////////
// GlobalNum �ʱ�ȭ

int GlobalNum::nPaintStatus = 0;







//////////////////////////////////////////////////////////////////////////
// JunHee Util

// ���ڿ� ��Ʈ��
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


// ���ϴ� �۾� ���� ĸ��,  ĸ�� �ϰ� ���� ������ Frame Param ����
void JunUtil::SpecificAreaCapture(CWnd *pcwnd)
{
	// ���� ȭ�� ������ ��ü�� ���� �����͸� ��´�.
	CWnd* pWndDesktop = pcwnd->GetDesktopWindow();
	// ���� ȭ�� ������ DC
	CWindowDC ScrDC(pWndDesktop);
	// �� ������ DC
	CClientDC dc(pcwnd);

	// Rect�� ����ؼ� �۾� ������ ���� ��ǥ�� ���,
	CRect Rect;
	pcwnd->GetClientRect(&Rect);

	// �� ��ġ�� ���� ������ ���� ��ǥ�� ������ ����.
	pcwnd->GetWindowRect(&Rect);

	// CImage�� �ϳ� �����
	CImage Image;
	// ��ĵ�� ������ x, y ��ġ��
	int sx = Rect.left;
	int sy = Rect.top;
	// �۾� ������ ���� ũ�⸦ ���� �ӽ÷� ������ ����.
	int cx = Rect.Width();
	int cy = Rect.Height();

	// �۾� ������ ũ�⸸ŭ, ���� ����ȭ���� �Ӽ��� ������ image�� �����Ѵ�.
	Image.Create(cx, cy, ScrDC.GetDeviceCaps(BITSPIXEL));
	// �̹��� DC�� ���� �۾� ������ ���� ��ǥ�� ����� �� ũ�⸸ŭ �����ϰ� �Ѵ�.
	CDC* pDC = CDC::FromHandle(Image.GetDC());
	pDC->BitBlt(0, 0, cx, cy, &ScrDC, sx, sy, SRCCOPY);
	Image.ReleaseDC();

	// ����� �̹����� ���ϴ� ���ϸ�, ����Ÿ���� �����ؼ� �����Ѵ�.
	Image.Save(TEXT("media\\capture\\image.png"), Gdiplus::ImageFormatJPEG);

	// �� ������ ������ �ش�.
	// ���� ���α׷��� �����ų �� ����!
	ShellExecute(NULL, TEXT("open"), TEXT("media\\capture\\image.png"), NULL, NULL, SW_SHOW);
}