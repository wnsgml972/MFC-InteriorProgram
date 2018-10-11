#pragma once

#include "define.h"
#include "Shape.h"
#include "DoorShape.h"
#include "WindowShape.h"
#include "UserObjectShape.h"


class CRoomShape : public CShape
{
public:
	CRoomShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CRoomShape();

	virtual bool AddShape();
	virtual void SetOwnColor();
	virtual int GetLocaInfo();
	virtual bool CopyShape();

	vector<CDoorShape*> m_CaDoor;		//�� ���� ���� ������ �ִ���
	vector<CWindowShape*> m_CaWindow; //�� ���� â���� ������ �ִ���

	vector<CUserObjectShape*> m_CaUserObject;  // �� �� �׷�ȭ ���
};