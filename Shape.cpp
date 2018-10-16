#include "stdafx.h"
#include "Shape.h"

CShape::CShape(int nId, int nX, int nY, int nWidth, int nHeight)
{
	this->nId = nId;
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;

	bSelectedState = FALSE;
	nRed = 0;  nGreen = 0;  nBlue = 0;	
	m_nType = TYPE_DEFAULT;
}

CShape::CShape(CShape * tmpShape)
{
	this->nId = tmpShape->nId;
	this->nX = tmpShape->nX;
	this->nY = tmpShape->nY;
	this->nWidth = tmpShape->nWidth;
	this->nHeight = tmpShape->nHeight;

	this->bSelectedState = bSelectedState;
	this->nRed = nRed;
	this->nGreen = nGreen;
	this->nBlue = nBlue;
}

CShape::~CShape()
{
	
}

void CShape::SetRect(int nX, int nY, int nWidth, int nHeight)
{
	this->nX = nX;
	this->nY = nY;
	this->nWidth = nWidth;
	this->nHeight = nHeight;

	nRed = 0;  nGreen = 0;  nBlue = 0;
}

void CShape::SetColor(int nRed, int nGreen, int nBlue)
{
	this->nRed = nRed;
	this->nGreen = nGreen;
	this->nBlue = nBlue;
}

int CShape::GetId()
{
	return nId;
}

void CShape::SetId(int nId)
{
	this->nId = nId;
}
