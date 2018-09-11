#pragma once

#define SAFE_DELETE(p) { if(p) {delete p; p = NULL;} }

#include <vector>
#include <stack>

using namespace std;

class GlobalNum
{
public:
	static GlobalNum* getInstance();

	int nPaintStatus;
	enum { PAINT_BASIC, PAINT_ROOM, PAINT_WINDOW, PAINT_DOOR,
		PAINT_TR1, PAINT_TR2, PAINT_TR3, PAINT_USER_ADD, PAINT_USER_WASH, PAINT_USER_TV
	
	}; // InteriorProgramView�� ����ȭ ������� ��...


private:
	GlobalNum();
	static GlobalNum *instance;
};