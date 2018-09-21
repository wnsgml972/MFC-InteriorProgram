#pragma once
#include "Shape.h"
class UserObjectShape :
	public Shape
{
public:
	UserObjectShape(int nId, int nX = 10, int nY = 10, int nWidth = 50, int nHeight = 50);
	~UserObjectShape();

	
	void SetImage();
	
	Shape *m_pInRoomShapePointer; // 추 후 그룹화 고민
};