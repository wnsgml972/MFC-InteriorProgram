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
	void AddShape(Shape CShape);
	bool DeleteShapeById(int nId); //delete p; undo() 해야 함
	void UpdateShapeById(int nId);
	int FindIndexById(int nId);

	//////////////////////////////////////////////////////////////////////////
	/// 기능
	void Undo();
	void Redo();
	void CopyShapeById(int nId);


	//////////////////////////////////////////////////////////////////////////
	/// Select Shape
	void InitSelect();
	void Select(); //selected true로 바꾸기
	void MoveSelectedShape();
	void RotateSelectedShape();
	void DeleteSelectedShape(); //delete p; undo() 해야 함
	void UpdateSelectedShape();
	void CopySelectedShape();

private:
	ShapeHandler();
	static ShapeHandler *instance;

	stack<Shape*> m_CsPerson;
	vector<Shape*> m_CaPerson;
	FileManager *CFileManager;
	int m_nAutoIncId;
	int m_nClickRange;
	int m_nDrawRange;


};

