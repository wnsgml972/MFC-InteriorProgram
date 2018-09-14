#pragma once

class Shape
{
public:
	Shape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	Shape(Shape *CShape);
	virtual ~Shape();

	void SetRect(int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	void SetColor(int nRed, int nGreen, int nBlue);
	
	int GetId();
	void SetId(int nId);

	int nX, nY, nWidth, nHeight;
	bool bSelectedState;
	int nRed, nGreen, nBlue;

private:
	int nId;
};

