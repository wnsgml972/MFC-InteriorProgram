#pragma once

#include "Shape.h"
#include "FileManager.h"
#include "define.h"
#include "RoomShape.h"
#include "UserObjectShape.h"


class CShapeHandler
{
public:
	static CShapeHandler* GetInstance();
	~CShapeHandler();


	//////////////////////////////////////////////////////////////////////////
	/// CRUD
	void AddShape(int nX, int nY, int nWidth, int nHeight);
	bool DeleteShapeById(int nId); //delete p; undo() 해야 함
	void UpdateShapeById(int nId);
	int FindVectorIndexById(int nId);

	//////////////////////////////////////////////////////////////////////////
	/// 기능
	void Undo();
	void Redo();
	void Clear();
	void CopyShapeById(int nId);
	void Move(CPoint point);
	void Select(CPoint point);
	void SetDoorWindowRange(bool bDragFlag, CPoint &OldMousePoint,  CPoint &CurrentMousePoint);
	void MakeFiveReferenceVertex(int *naVertex, int tmpVal1, int tmpVal2);

	//////////////////////////////////////////////////////////////////////////
	/// Select Shape
	void InitSelect();
	int GetCurrentSelectedIndex();
	int HowManySelected();
	int RotateSelectedShape();
	int DeleteSelectedShape(); //delete p; undo() 해야 함
	int UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight);
	int CopySelectedShape(int nIndex = MY_ERROR);
	int WheelSelectedShape(short zDelta);
	int MagneticSelectedShape();

	vector<CShape*> m_CaShape;
	bool NoAddShape;

	int MakeAutoIncId();
	int m_nDrawRange;

private:
	CShapeHandler();
	static CShapeHandler *instance;

	stack<CShape*> m_CsShape;
	CFileManager *CFileManager;
	int m_nAutoIncId;
	int m_nSelectRange;
	int m_nDrawSelectRange;

	CShape *m_pRememberRoomPtrForDoorWindow;
	int m_RememberLocationForDoorWindow;
};

