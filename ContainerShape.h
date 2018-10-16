#pragma once
#include "Shape.h"
#include "DependentShape.h"

class CContainerShape : public CShape
{
public:
	CContainerShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CContainerShape();


	// �⺻ ���
	virtual bool AddShape() = 0;
	virtual bool DeleteShape() = 0;
	virtual bool CopyShape() = 0;
	virtual bool MoveShape(CPoint point) = 0;
	virtual bool MagneticShape() = 0;
	virtual bool EditShape(int nX, int nY, int nWidth, int nHeight, int *m_nSubShapeStatus, int *m_nRememberShapeStatus) = 0;
	virtual bool WheelShape(short zDelta) = 0;

	vector<CDependentShape*> m_CaDependent;		// �ȿ� ������ �ִ�(�׷�ȭ �Ǿ� �ִ�) ��, â����!
};