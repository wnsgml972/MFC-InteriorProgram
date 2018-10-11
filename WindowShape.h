#pragma once

#include "Shape.h"

class CWindowShape : public CShape
{
public:
	CWindowShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CWindowShape();

	virtual bool AddShape();
	virtual void SetOwnColor();
	virtual int GetLocaInfo();
	virtual bool CopyShape();

	int m_pInRoomShapeID;
	int m_nLocaInfo;
};