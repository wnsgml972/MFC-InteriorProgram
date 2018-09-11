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
	void Move(int nOldMouseX, int nOldMouseY, int CurrentMouseX, int CurrentMouseY);
	void Select(CPoint point); //selected true�� �ٲٱ�


	//////////////////////////////////////////////////////////////////////////
	/// Select Shape
	void InitSelect();
	int CurrentSelectedId();
	int HowManySelected();
	void RotateSelectedShape();
	void DeleteSelectedShape(); //delete p; undo() �ؾ� ��
	void UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight);
	void CopySelectedShape();

	vector<Shape*> m_CaShape;

private:
	ShapeHandler();
	static ShapeHandler *instance;

	stack<Shape*> m_CsShape;
	FileManager *CFileManager;
	int m_nAutoIncId;
	int m_nSelectRange;
	int m_nDrawRange;


};

