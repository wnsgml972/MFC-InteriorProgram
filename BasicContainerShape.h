#pragma once
#include "ContainerShape.h"

class CBasicContainerShape : public CContainerShape
{
public:
	CBasicContainerShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CBasicContainerShape();

	virtual bool AddShape();
	virtual bool DeleteShape();
	virtual bool CopyShape();
	virtual bool MoveShape(CPoint point);
	virtual bool MagneticShape();
	virtual bool EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus);
	virtual bool WheelShape(short zDelta);

	// ����
	int m_pInContainerShapeID;
	vector<CBasicContainerShape*> m_CaBasicContainer;
};