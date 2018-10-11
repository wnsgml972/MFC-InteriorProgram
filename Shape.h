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

	virtual int GetLocaInfo() = 0;
	virtual void SetOwnColor() = 0;
	virtual bool AddShape() = 0;
	virtual bool CopyShape() = 0;

	int nX, nY, nWidth, nHeight;
	bool bSelectedState;
	int nRed, nGreen, nBlue;
	int m_nMoveSubVal[4]; //Move에 사용함

private:
	int nId;
};

