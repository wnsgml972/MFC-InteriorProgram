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

	vector<DoorShape*> m_CaDoor;		//�� ���� ���� ������ �ִ���
	vector<WindowShape*> m_CaWindow; //�� ���� â���� ������ �ִ���

	vector<UserObjectShape*> m_CaUserOnject;  // �� �� �׷�ȭ ���
};