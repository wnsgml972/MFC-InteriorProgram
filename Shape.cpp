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

Shape::Shape(Shape * CShape)
{
	this->nId = CShape->nId;
	this->nX = CShape->nX;
	this->nY = CShape->nY;
	this->nWidth = CShape->nWidth;
	this->nHeight = CShape->nHeight;

}

Shape::~Shape()
{
	printf("~Shape\n");
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

int Shape::GetId()
{
	return nId;
}

void Shape::SetId(int nId)
{
	this->nId = nId;
}
