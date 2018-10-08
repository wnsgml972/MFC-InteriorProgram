#pragma once
#include "Shape.h"

struct MYICON_INFO
{
	int     nWidth;
	int     nHeight;
	int     nBitsPerPixel;
};

class CUserObjectShape : public CShape
{
public:
	CUserObjectShape(int nId, int nX = 10, int nY = 10, int nWidth = 50, int nHeight = 50);
	~CUserObjectShape();
	
	void SetImageIcon(int ID);	
	MYICON_INFO MyGetIconInfo(HICON hIcon);

	CShape *m_pInRoomShapePointer; // 추 후 그룹화 고민
	HICON icon;
};