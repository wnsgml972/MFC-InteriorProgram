#pragma once

// Namespace
#include <string>

class JunUtil
{
public:
	static std::string CStringToString(CString cstr);
	static CString StringToCString(std::string str);
	static bool isNumber(std::string str);
	static void SpecificAreaCapture(CWnd *pcwnd);
};

// Global Num 정적 변수 사용시 초기화를 꼭 cpp 파일에서 해줘야 함
class GlobalNum
{
public:
	static int nPaintStatus;
};
