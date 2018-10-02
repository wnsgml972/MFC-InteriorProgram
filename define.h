#pragma once

// Basic Macro
#define MY_ERROR -1
#define MY_SUCCES 1
#define _CRT_SECURE_NO_WARNINGS

#define NO_KEY					0		// if key == 0, none
#define NO_IDX					(-1)	// if index == -1, none

// Delete Macro
#define SAFE_DELETE(x)			{if(x){delete (x);(x)=nullptr;}}
#define SAFE_DELETE_ARRAY(x)	{if(x){delete [] (x);(x)=nullptr;}}

// For Loop Macro
#define REP(x, count)           for( long (x)=0; (x)<(count); ++(x) )
#define REPi(count)             REP(i, count)
#define REPj(count)             REP(j, count)
#define REPk(count)             REP(k, count)

#define FOR(x,start,count)      for( long (x)=(start); (x)<(count); ++(x) )
#define FORi(start, count)      FOR(i, start, count)
#define FORj(start, count)      FOR(j, start, count)
#define FORk(start, count)      FOR(k, start, count)

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