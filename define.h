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


// Delete Warning
//#pragma warning(disable:4018)  벡터 포문 돌릴 시 벡터에서 사이즈 관한 warning 안 뜨게 함


// Add Lib
#include <vector>
#include <stack>
#include <typeinfo>
#include <algorithm>
#include "JunUtil.h"


// Namespace
using namespace std;


// Enum List
const enum    // PAINT_BASIC은 Select와 Move, Magnetic 담당함!
{
	PAINT_BASIC = 0, PAINT_ROOM, PAINT_WINDOW, PAINT_DOOR, PAINT_OBJECT
}; 
const enum 
{
	LOCA_DEFAULT = 0, LOCA_UP, LOCA_RIGHT, LOCA_DOWN, LOCA_LEFT
};

const enum
{
	TYPE_DEFAULT = 0, TYPE_SUPER_CONTAINER, TYPE_BASIC_CONTAINER, TYPE_OBJECT, TYPE_DEPENDENT
};