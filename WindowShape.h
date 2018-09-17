#pragma once
#include "Shape.h"
class WindowShape :	public Shape
{
public:
	WindowShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~WindowShape();

	Shape *pInRoomShapePointer;
	int nLocaInfo;
	enum {
		LOCA_DEFAULT, LOCA_UP, LOCA_RIGHT, LOCA_DOWN, LOCA_LEFT
	};
};

