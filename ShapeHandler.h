#pragma once

#include "Shape.h"
#include "FileManager.h"
#include "define.h"
#include "RoomShape.h"
#include "UserObjectShape.h"

class CShape;
class CRoomShape;

class CShapeHandler
{
public:
	static CShapeHandler* GetInstance();
	virtual ~CShapeHandler();


	//////////////////////////////////////////////////////////////////////////
	/// CRUD
	bool AddShape(CShape *cShape);
	CShape* GetShapeByID(int nId);

	//////////////////////////////////////////////////////////////////////////
	/// 기능
	void Undo();
	void Redo();
	void Clear();
	void Move(CPoint point);
	void Select(CPoint point);
	void SetDoorWindowRange(bool bDragFlag, CPoint &OldMousePoint,  CPoint &CurrentMousePoint);
	void MakeFiveReferenceVertex(int *naVertex, int tmpVal1, int tmpVal2);
	void SetRange(int &nX, int &nY, int &nWidth, int &nHeight);

	//////////////////////////////////////////////////////////////////////////
	/// Select Shape
	void InitSelect();
	int GetCurrentSelectedIndex();
	int HowManySelected();
	int RotateSelectedShape(); // 개발 중
	int DeleteSelectedShape(); //delete p; undo() 해야 함
	int UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight);
	int CopySelectedShape(int nIndex = MY_ERROR);
	int WheelSelectedShape(short zDelta);
	int MagneticSelectedShape();

	vector<CShape*> m_CaShape;
	bool NoAddShape;

	int MakeAutoIncId();
	int m_nDrawRange;


	int m_pRememberRoomIDForDoorWindow;
	int m_RememberLocationForDoorWindow;

private:
	CShapeHandler();
	static CShapeHandler *instance;

	stack<CShape*> m_CsShape;
	CFileManager *CFileManager;
	int m_nAutoIncId;
	int m_nSelectRange;
	int m_nDrawSelectRange;

};

