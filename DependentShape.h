#pragma once
#include "ComponentShape.h"

class CDependentShape : public CComponentShape
{
public:
	CDependentShape(int nId, int nX = 0, int nY = 0, int nWidth = 0, int nHeight = 0);
	~CDependentShape();

	// �⺻ ���
	virtual bool AddShape();
	virtual bool DeleteShape();
	virtual bool CopyShape();
	virtual bool MoveShape();
	virtual bool MagneticShape();
	virtual bool EditShape();
	virtual bool WheelShape();

	// �߰� ���
	virtual int GetLocaInfo();


	// ����
	int m_nLocaInfo;
	int m_pInContainerShapeID;
};