#pragma once
#include "Shape.h"

class CComponentShape : public CShape
{
public:
	CComponentShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CComponentShape();


	// 기본 기능
	virtual bool AddShape() = 0;
	virtual bool DeleteShape() = 0;
	virtual bool CopyShape() = 0;
	virtual bool MoveShape(CPoint point) = 0;
	virtual bool MagneticShape() = 0;
	virtual bool EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus) = 0;
	virtual bool WheelShape(short zDelta) = 0;
};
