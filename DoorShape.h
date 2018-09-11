#pragma once
#include "Shape.h"
class DoorShape :
	public Shape
{
public:
	DoorShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~DoorShape();
};

