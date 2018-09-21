#pragma once

#define SAFE_DELETE(p) { if(p) {delete p; p = NULL;} }
#define MY_ERROR -1
#define MY_SUCCES 1
#define _CRT_SECURE_NO_WARNINGS

#include <vector>
#include <stack>
#include <typeinfo>
#include <algorithm>

using namespace std;

class GlobalNum
{
public:
	static GlobalNum* getInstance();

	int nPaintStatus;
	enum {
		PAINT_BASIC, PAINT_ROOM, PAINT_WINDOW, PAINT_DOOR,
		PAINT_USER_ADD, PAINT_USER_WASH, PAINT_USER_TV,		
	}; // InteriorProgramView¶û µ¿±âÈ­ ½ÃÄÑÁà¾ß µÊ...
	   // PAINT_BASICÀº Select¿Í Move, Magnetic ´ã´çÇÔ!

	enum {
		LOCA_DEFAULT, LOCA_UP, LOCA_RIGHT, LOCA_DOWN, LOCA_LEFT
	};

private:
	GlobalNum();
	static GlobalNum *instance;
};