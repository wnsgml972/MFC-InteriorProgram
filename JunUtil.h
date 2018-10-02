#pragma once

// Namespace
#include <string>

class JunUtil
{
public:
	static std::string CStringToString(CString cstr);
	static CString StringToCString(std::string str);
	static bool isNumber(std::string s);
	static void SpecificAreaCapture(CWnd *pcwnd);
};


class GlobalNum
{
public:
	static int nPaintStatus;
};
