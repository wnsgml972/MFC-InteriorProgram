#pragma once
#include "ComponentShape.h"

struct MYICON_INFO
{
	int     nWidth;
	int     nHeight;
	int     nBitsPerPixel;
};

class CObjectShape : public CComponentShape
{
public:
	CObjectShape(int nId, int nX = 10, int nY = 10, int nWidth = 50, int nHeight = 50, int m_nIconID = 0);
	~CObjectShape();

	// 기본 기능
	virtual bool AddShape();
	virtual bool DeleteShape();
	virtual bool CopyShape();
	virtual bool MoveShape(CPoint point);
	virtual bool MagneticShape();
	virtual bool EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus);
	virtual bool WheelShape(short zDelta);

	// 추가 기능
	void SetImageIcon(int nImgIconID);
	MYICON_INFO MyGetIconInfo(HICON hIcon);
	virtual void SetOwnColor();

	// 변수
	int m_pInContainerShapeID;
	int m_nIconID;
	HICON m_icon;
};