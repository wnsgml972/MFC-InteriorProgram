#pragma once

#include "DependentShape.h"

class CDoorShape : public CDependentShape
{
public:
	CDoorShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CDoorShape();

	virtual void SetOwnColor();
};