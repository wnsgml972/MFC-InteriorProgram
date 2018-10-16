#include "stdafx.h"
#include "ShapeFactory.h"
#include "RoomShape.h"
#include "ObjectShape.h"
#include "DoorShape.h"
#include "WindowShape.h"
#include "ShapeHandler.h"


CShapeFactory* CShapeFactory::instance = NULL;

CShapeFactory::CShapeFactory() { }

CShapeFactory* CShapeFactory::GetInstance()
{
	if (!instance) {
		instance = new CShapeFactory();
	}
	return instance;
}

CShapeFactory::~CShapeFactory()
{
	SAFE_DELETE(instance);
}

CShape * CShapeFactory::GetShapeWhenPaint(const int & PAINT_TYPE, int nX, int nY, int nWidth, int nHeight)
{
	CShape* CNewShape = nullptr;

	switch (PAINT_TYPE)
	{
	case PAINT_ROOM:
		CNewShape = new CRoomShape(CShapeHandler::GetInstance()->MakeAutoIncId(), nX, nY, nWidth, nHeight);
		break;
	case PAINT_WINDOW:
		CNewShape = new CWindowShape(CShapeHandler::GetInstance()->MakeAutoIncId(), nX, nY, nWidth, nHeight);
		break;
	case PAINT_DOOR:
		CNewShape = new  CDoorShape(CShapeHandler::GetInstance()->MakeAutoIncId(), nX, nY, nWidth, nHeight);
		break;
	case PAINT_OBJECT:
		CNewShape = new CObjectShape(CShapeHandler::GetInstance()->MakeAutoIncId());
		break;
	default:
		ASSERT(0);
	}

	return CNewShape;
}

CShape * CShapeFactory::GetShapeWhenCopy(CShape *tmpShape)
{
	CShape *CNewCopyShape = nullptr;

	if (typeid(*tmpShape) == typeid(CRoomShape))
	{
		CNewCopyShape = new CRoomShape(tmpShape->GetId(), tmpShape->nX, tmpShape->nY, tmpShape->nWidth, tmpShape->nHeight);
	}
	else if (typeid(*tmpShape) == typeid(CObjectShape))
	{
		CNewCopyShape = new CObjectShape(tmpShape->GetId(), tmpShape->nX, tmpShape->nY, tmpShape->nWidth,
			tmpShape->nHeight, dynamic_cast<CObjectShape*>(tmpShape)->m_nIconID);
	}
	else if (typeid(*tmpShape) == typeid(CDoorShape))		// 얘는 어느 방에 들어가있는지도 복사되야 하니까 이렇게 복사함
	{
		CNewCopyShape = new CDoorShape(*dynamic_cast<CDoorShape*>(tmpShape));
	}
	else if (typeid(*tmpShape) == typeid(CWindowShape))	// 얘는 어느 방에 들어가있는지도 복사되야 하니까 이렇게 복사함
	{
		CNewCopyShape = new CWindowShape(*dynamic_cast<CWindowShape*>(tmpShape));
	}
	else
	{
		ASSERT(0);
	}

	CNewCopyShape->SetId(CShapeHandler::GetInstance()->MakeAutoIncId());
	return CNewCopyShape;
}
