#include "stdafx.h"
#include "Shape.h"


Shape::Shape(int nId, int nX, int nY, int nWidth, int nHeight)
{
	this->nId = nId;
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;

	bSelectedState = FALSE;
	nRed = 0;  nGreen = 0;  nBlue = 0;
}

Shape::~Shape()
{
}

void Shape::SetRect(int nX, int nY, int nWidth, int nHeight)
{
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;

	nRed = 0;  nGreen = 0;  nBlue = 0;
}

void Shape::SetColor(int nRed, int nGreen, int nBlue)
{
	this->nRed = nRed;
	this->nGreen = nGreen;
	this->nBlue = nBlue;
}
