#pragma once
#include "ContainerShape.h"

class CSuperContainerShape : public CContainerShape
{
public:
	CSuperContainerShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CSuperContainerShape();

	virtual bool AddShape();
	virtual bool DeleteShape();
	virtual bool CopyShape();
	virtual bool MoveShape();
	virtual bool MagneticShape();
	virtual bool EditShape();
	virtual bool WheelShape();

};