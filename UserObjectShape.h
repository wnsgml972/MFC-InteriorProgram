#pragma once
#include "Shape.h"

struct MYICON_INFO
{
	int     nWidth;
	int     nHeight;
	int     nBitsPerPixel;
};

class UserObjectShape :
	public Shape
{
public:
	UserObjectShape(int nId, int nX = 10, int nY = 10, int nWidth = 50, int nHeight = 50);
	~UserObjectShape();
	
	void SetImageIcon(int ID);	
	MYICON_INFO MyGetIconInfo(HICON hIcon);



	Shape *m_pInRoomShapePointer; // 추 후 그룹화 고민
	HICON icon;
};