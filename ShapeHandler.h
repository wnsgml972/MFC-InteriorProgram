#pragma once

#include "define.h"
#include "Shape.h"
#include "FileManager.h"

class ShapeHandler
{
public:
	static ShapeHandler* GetInstance();
	~ShapeHandler();


	//////////////////////////////////////////////////////////////////////////
	/// CRUD
	void AddShape(int nX, int nY, int nWidth, int nHeight);
	bool DeleteShapeById(int nId); //delete p; undo() �ؾ� ��
	void UpdateShapeById(int nId);
	int FindVectorIndexById(int nId);

	//////////////////////////////////////////////////////////////////////////
	/// ���
	void Undo();
	void Redo();
	void Clear();
	void CopyShapeById(int nId);
	void Move(CPoint point);
	void Select(CPoint point);
	void SetDoorWindowRange(bool bDragFlag, CPoint &OldMousePoint, CPoint &CurrentMousePoint);
	void MakeFiveReferenceVertex(int *naVertex, int tmpVal1, int tmpVal2);

	//////////////////////////////////////////////////////////////////////////
	/// Select Shape
	void InitSelect();
	int GetCurrentSelectedIndex();
	int HowManySelected();
	int RotateSelectedShape();
	int DeleteSelectedShape(); //delete p; undo() �ؾ� ��
	int UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight);
	int CopySelectedShape(int nIndex = MY_ERROR);
	int WheelSelectedShape(short zDelta);
	int MagneticSelectedShape();

	vector<Shape*> m_CaShape;
	bool NoAddShape;

	int MakeAutoIncId();
	int m_nDrawRange;

private:
	ShapeHandler();
	static ShapeHandler *instance;

	stack<Shape*> m_CsShape;
	FileManager *CFileManager;
	int m_nAutoIncId;
	int m_nSelectRange;
	int m_nDrawSelectRange;

	Shape *m_pRememberRoomPtrForDoorWindow;
	int m_RememberLocationForDoorWindow;
};

