#pragma once
#include "Shape.h"
#include "DoorShape.h"
#include "WindowShape.h"
#include "define.h"
#include "UserObjectShape.h"
class RoomShape :
	public Shape
{
public:
	RoomShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~RoomShape();

	vector<DoorShape*> m_CaDoor;		//몇 개의 문을 가지고 있는지
	vector<WindowShape*> m_CaWindow; //몇 개의 창문을 가지고 있는지

	vector<UserObjectShape*> m_CaUserOnject;  // 추 후 그룹화 고민
};