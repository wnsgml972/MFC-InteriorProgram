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
	CUserObjectShape(int nId, int nX = 10, int nY = 10, int nWidth = 50, int nHeight = 50, int m_nIconID = 0);
	~CUserObjectShape();
	
	void SetImageIcon(int nImgIconID);
	MYICON_INFO MyGetIconInfo(HICON hIcon);

	virtual bool AddShape();
	virtual void SetOwnColor();
	virtual int GetLocaInfo();
	virtual bool CopyShape();

	int m_pInRoomShapeID; // 추 후 그룹화 고민
	HICON m_icon;
	int m_nIconID;
};