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

// Global Num ���� ���� ���� �ʱ�ȭ�� �� cpp ���Ͽ��� ����� ��
class GlobalNum
{
public:
	static int nPaintStatus;
};
