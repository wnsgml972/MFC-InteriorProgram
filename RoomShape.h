#pragma once

#include "BasicContainerShape.h"


class CRoomShape : public CBasicContainerShape
{
public:
	CRoomShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CRoomShape();


	virtual void SetOwnColor();
};