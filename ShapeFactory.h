#pragma once
#include "Shape.h"

class CShapeFactory
{
public:
	static CShapeFactory* GetInstance();
	virtual ~CShapeFactory();

	CShape* GetShapeWhenPaint(const int& PAINT_TYPE, int nX = 5, int nY = 5, int nWidth = 5, int nHeight = 5);
	CShape* GetShapeWhenCopy(CShape* tmpShape);

private:
	CShapeFactory();
	static CShapeFactory *instance;
};