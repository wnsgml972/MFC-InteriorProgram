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
	void SetDoorWindowRange(bool bDragFlag, CPoint &OldMousePoint, CPoint &CurrentMousePoint);

	//////////////////////////////////////////////////////////////////////////
	/// Select Shape
	void InitSelect();
	int GetCurrentSelectedIndex();
	int HowManySelected();
	int RotateSelectedShape();
	int DeleteSelectedShape(); //delete p; undo() 해야 함
	int UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight);
	int CopySelectedShape();

	vector<Shape*> m_CaShape;
	int m_nMoveSubVal[4]; //Move에 사용함
	bool NoAddShape;

private:
	ShapeHandler();
	static ShapeHandler *instance;

	stack<Shape*> m_CsShape;
	FileManager *CFileManager;
	int m_nAutoIncId;
	int m_nSelectRange;
	int m_nDrawRange;
	int m_nDrawSelectRange;
	int m_nRememberIndexForDoorWindowVector;
};

