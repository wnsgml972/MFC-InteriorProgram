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
	virtual bool MoveShape();
	virtual bool MagneticShape();
	virtual bool EditShape();
	virtual bool WheelShape();

	// 추가 기능
	void SetImageIcon(int nImgIconID);
	MYICON_INFO MyGetIconInfo(HICON hIcon);

	// 변수
	int m_pInContainerShapeID;
	int m_nIconID;
	HICON m_icon;
};