#pragma once

#include "define.h"

class CShape
{
public:
	CShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	CShape(CShape *tmpShape);
	virtual ~CShape();

	void SetRect(int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	void SetColor(int nRed, int nGreen, int nBlue);
	
	int GetId();
	void SetId(int nId);

	// All
	virtual bool AddShape() = 0;
	virtual bool DeleteShape() = 0;
	virtual bool CopyShape() = 0;
	virtual bool MoveShape(CPoint point) = 0;
	virtual bool MagneticShape() = 0;
	virtual bool EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus) = 0;
	virtual bool WheelShape(short zDelta) = 0;

	// Paint Shape
	virtual void SetOwnColor() { nRed = 0;  nGreen = 0;  nBlue = 0; };

	// Delete, 
	int nX, nY, nWidth, nHeight;
	bool bSelectedState;
	int nRed, nGreen, nBlue;
	int m_nMoveSubVal[4]; //Move에 사용함
	int m_nType;

private:
	int nId;
};

