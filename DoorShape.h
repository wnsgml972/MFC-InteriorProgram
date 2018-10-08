#pragma once
#include "Shape.h"
class CDoorShape : public CShape
{
public:
	CDoorShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CDoorShape();

	CShape *m_pInRoomShapePointer;
	int m_nLocaInfo;

};

