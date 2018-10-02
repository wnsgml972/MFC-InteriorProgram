#pragma once
#include "Shape.h"



// =======================================



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
	
	Shape *m_pInRoomShapePointer; // �� �� �׷�ȭ ���
	HICON icon;

	MYICON_INFO MyGetIconInfo(HICON hIcon);

};