#pragma once
#include "Shape.h"
class UserObjectShape :
	public Shape
{
public:
	UserObjectShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~UserObjectShape();
};

