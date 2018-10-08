#pragma once
#include "Shape.h"
#include "DoorShape.h"
#include "WindowShape.h"
#include "UserObjectShape.h"
#include "define.h"


class CRoomShape : public CShape
{
public:
	CRoomShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CRoomShape();

	vector<CDoorShape*> m_CaDoor;		//몇 개의 문을 가지고 있는지
	vector<CWindowShape*> m_CaWindow; //몇 개의 창문을 가지고 있는지

	vector<CUserObjectShape*> m_CaUserObject;  // 추 후 그룹화 고민
};