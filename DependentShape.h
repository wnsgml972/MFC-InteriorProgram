#pragma once
#include "ComponentShape.h"

class CDependentShape : public CComponentShape
{
public:
	CDependentShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CDependentShape();

	// 기본 기능
	virtual bool AddShape();
	virtual bool DeleteShape();
	virtual bool CopyShape();
	virtual bool MoveShape(CPoint point);
	virtual bool MagneticShape();
	virtual bool EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus);
	virtual bool WheelShape(short zDelta);

	// 추가 기능
	virtual int GetLocaInfo();


	// 변수
	int m_nLocaInfo;
	int m_pInContainerShapeID;
};