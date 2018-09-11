#include "stdafx.h"
#include "ShapeHandler.h"
#include "RoomShape.h"

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
	m_nSelectRange = 25;
}
ShapeHandler::~ShapeHandler()
{
	SAFE_DELETE(instance);
}



//////////////////////////////////////////////////////////////////////////
/// CRUD
void ShapeHandler::AddShape(int nX, int nY, int nWidth, int nHeight)
{
	if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_ROOM)
	{
		m_CaShape.push_back(new RoomShape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_DOOR)
	{
		m_CaShape.push_back(new DoorShape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
		//room�� ���� �ȿ� �־�� ��
	}
	else if (GlobalNum::getInstance()->nPaintStatus == GlobalNum::getInstance()->PAINT_WINDOW)
	{
		m_CaShape.push_back(new WindowShape(m_nAutoIncId++, nX, nY, nWidth, nHeight));
		//room�� ���� �ȿ� �־�� ��
	}
	else
	{
		printf("error\n");
		//error
	}
}
bool ShapeHandler::DeleteShapeById(int nId) // ���� Room �̶�� �� �ȿ� �����ϴ� Door�� Window ��� ���Ϳ��� ���� �����ؾ� ��
{
	return false;
}
void ShapeHandler::UpdateShapeById(int nId)
{

}
int ShapeHandler::FindVectorIndexById(int nId)
{
	return 0;
}



//////////////////////////////////////////////////////////////////////////
/// ���
void ShapeHandler::CopyShapeById(int nId)
{

}
void ShapeHandler::Undo()
{

}
void ShapeHandler::Redo()
{

}

void ShapeHandler::Clear()
{
	instance->m_CaShape.clear();
}
void ShapeHandler::Move(int nOldMouseX, int nOldMouseY, int CurrentMouseX, int CurrentMouseY) //door list�� window list�� ���� �����δ�.
{

}
void ShapeHandler::Select(CPoint point)
{
	// ���� �ڿ������� �˻���� ��!  z-index
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = m_CaShape.size() - 1; i >= 0; i--)
#pragma warning(pop)
	{
		int nShapeTempX = m_CaShape.at(i)->nX;
		int nShapeTempY = m_CaShape.at(i)->nY;
		int nShapeTempWidth = m_CaShape.at(i)->nWidth;
		int nShapeTempHeight = m_CaShape.at(i)->nHeight;

		// �׸� �ϳ��� ���� �ϳ��� Range�� �����ϴ� �׸�� ���� ��
		// ���� ���������� ������ �����ذ�
		// �� -> ������ -> �Ʒ� -> ����
		if ((nShapeTempX - m_nSelectRange <= point.x && point.x <= nShapeTempWidth + m_nSelectRange && nShapeTempY - m_nSelectRange <= point.y && point.y <= nShapeTempY + m_nSelectRange) ||
			(nShapeTempWidth - m_nSelectRange <= point.x && point.x <= nShapeTempWidth + m_nSelectRange && nShapeTempY - m_nSelectRange <= point.y && point.y <= nShapeTempHeight + m_nSelectRange) ||
			(nShapeTempX - m_nSelectRange <= point.x && point.x <= nShapeTempWidth + m_nSelectRange && nShapeTempHeight - m_nSelectRange <= point.y && point.y <= nShapeTempHeight + m_nSelectRange) ||
			(nShapeTempX - m_nSelectRange <= point.x && point.x <= nShapeTempX + m_nSelectRange && nShapeTempY - m_nSelectRange <= point.y && point.y <= nShapeTempHeight + m_nSelectRange))
		{
			m_CaShape.at(i)->SetColor(255, 0, 0);
			m_CaShape.at(i)->bSelectedState = TRUE;
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/// Select Shape
void ShapeHandler::InitSelect()
{
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < m_CaShape.size(); i++)
#pragma warning(pop)
	{		
		if (m_CaShape.at(i)->bSelectedState)
		{
			m_CaShape.at(i)->bSelectedState = FALSE;
			m_CaShape.at(i)->SetColor(0, 0, 0);
		}
	}

}
int ShapeHandler::CurrentSelectedId()
{
	return 0;
}
int ShapeHandler::HowManySelected()
{
	return 0;
}
void ShapeHandler::RotateSelectedShape()
{
}
void ShapeHandler::DeleteSelectedShape() // ���� Room �̶�� �� �ȿ� �����ϴ� Door�� Window ��� ���Ϳ��� ���� �����ؾ� ��
{
}
void ShapeHandler::CopySelectedShape()
{

}
void ShapeHandler::UpdateSelectedShape(int nX, int nY, int nWidth, int nHeight)
{
#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < m_CaShape.size(); i++)
#pragma warning(pop)
	{
		if (m_CaShape.at(i)->bSelectedState == TRUE)
		{
			m_CaShape.at(i)->SetRect(nX, nY, nWidth, nHeight);
			break;
		}
	}
}





