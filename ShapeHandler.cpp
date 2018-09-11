#include "stdafx.h"
#include "ShapeHandler.h"

ShapeHandler* ShapeHandler::instance = NULL;

ShapeHandler* ShapeHandler::GetInstance()
{
	if (!instance) {
		instance = new ShapeHandler();
	}
	return instance;
}
ShapeHandler::ShapeHandler()
{
	CFileManager = FileManager::GetInstance();
	m_nAutoIncId = 0;
}
ShapeHandler::~ShapeHandler()
{
	SAFE_DELETE(instance);
}



//////////////////////////////////////////////////////////////////////////
/// CRUD
void ShapeHandler::AddShape(int nX, int nY, int nWidth, int nHeight)
{
	m_CaPerson.push_back(new Shape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
}
void ShapeHandler::AddShape(Shape CShape)
{
	m_CaPerson.push_back(new Shape(m_nAutoIncId++, CShape.nX, CShape.nY, CShape.nWidth, CShape.nHeight));
}
bool ShapeHandler::DeleteShapeById(int nId)
{
	return false;
}
void ShapeHandler::UpdateShapeById(int nId)
{

}
int ShapeHandler::FindIndexById(int nId)
{
	return 0;
}



//////////////////////////////////////////////////////////////////////////
/// ±â´É
void ShapeHandler::CopyShapeById(int nId)
{
}
void ShapeHandler::Undo()
{
}
void ShapeHandler::Redo()
{
}


//////////////////////////////////////////////////////////////////////////
/// Select Shape
void ShapeHandler::InitSelect()
{
}
void ShapeHandler::Select()
{
}
void ShapeHandler::MoveSelectedShape()
{
}
void ShapeHandler::RotateSelectedShape()
{
}
void ShapeHandler::DeleteSelectedShape()
{
}
void ShapeHandler::UpdateSelectedShape()
{
}
void ShapeHandler::CopySelectedShape()
{
}


