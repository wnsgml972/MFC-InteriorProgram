#pragma once

#include "DependentShape.h"

class CWindowShape : public CDependentShape
{
public:
	CWindowShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CWindowShape();

	virtual void SetOwnColor();
};