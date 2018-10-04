#include "stdafx.h"
#include "FileManager.h"
#include "ShapeHandler.h"

FileManager* FileManager::instance = NULL;


FileManager* FileManager::GetInstance()
{
	if (!instance)
	{
		instance = new FileManager();
	}
	return instance;
}
FileManager::FileManager()  { }
FileManager::~FileManager() { SAFE_DELETE(instance); }


//////////////////////////////////////////////////////////////////////////
// JSon �̿�
void FileManager::SaveAsJson()
{

}
void FileManager::LoadAsJson()
{

}


//////////////////////////////////////////////////////////////////////////
// csv �̿�
boolean FileManager::SaveAsCsv()
{
	ShapeHandler *CShapeHandler = ShapeHandler::GetInstance();

	CString strPath = _T("C:\\Users\\kjh0121\\source\\repos\\Project1\\PhoneBookGUI\\media\\���̴ٽ��ּҷ�_20180214.csv");

	CString strStatus;

	//���� ����
	ofstream file(strPath);

#pragma warning(push)
#pragma warning(disable: 4018)
	//���� ����

	for (int i = 0; i < CShapeHandler->m_CaShape.size(); i++)
	{
		if (CShapeHandler->m_CaShape.at(i) == nullptr)
		{
			continue;
		}
		// file�� ���� ���� ���� ���� !!  �߰�
// 		file << CStringToString(CShapeHandler->m_CaShape.at(i)->GetName()) << ',' << CStringToString(CShapeHandler->m_CaShape.at(i)->GetPhoneNumber())
// 			<< ',' << CStringToString(CShapeHandler->m_CaShape.at(i)->GetTitle()) << ',' << CStringToString(CShapeHandler->m_CaShape.at(i)->GetJob());
		file << '\n';
	}
#pragma warning(pop)

	if (file.fail())  //���� bad() �Լ��� ���� �ϸ�..
	{
		return FALSE;
	}

	file.close(); //���� ����� �Ϸ� �� �ݾ��ش�.

	return TRUE;
}
boolean FileManager::LoadAsCsv()
{
	ShapeHandler *CShapeHandler = ShapeHandler::GetInstance();

	CString strPath = _T("C:\\Users\\kjh0121\\source\\repos\\Project1\\PhoneBookGUI\\media\\���̴ٽ��ּҷ�_20180214.csv");

	CString strName;
	CString strPhoneNumber;
	CString strTitle;
	CString strJob;


	//ifstream�� ������ �а� ���ִ� �Լ��ν� ifstream (���ϸ� or ���)
	ifstream file(strPath);

	if (file.fail())  //���� bad() �Լ��� ���� �ϸ�..
	{
		return FALSE;
	}

	CShapeHandler->m_CaShape.clear();

	while (file.good()) //eof, bad, fail �Լ��� ������ ��ȯ�� ������
	{
		vector<string> row = CsvReadRow(file, ',');

		if (!row[0].find("#")) //���� csv ���� �ȿ� # ���ڰ� �������
		{
			continue; //�׳� �ǳ� �پ��
		}
		else //#���ڰ� ���� ���
		{
			// Add Shape!! �߰�

// 			strName = StringToCString(row[0]);
// 			strJob = StringToCString(row[1]);
// 			strTitle = StringToCString(row[2]);
// 			strPhoneNumber = StringToCString(row[3]);

			//CShapeHandler->m_CaShape.push_back(new Person(CShapeHandler->, strName, strPhoneNumber, strTitle, strJob));
		}
	}

	file.close(); //���� ����� �Ϸ� �� �ݾ��ش�.

	return TRUE;

}
vector<string> FileManager::CsvReadRow(istream &file, char cDelimiter)
{
	stringstream StringStream;
	bool bInquotes = false;  // �ο뱸, ū ����ǥ ���� �� ����
	vector<string> row; //relying on RVO

	while (file.good())
	{
		char cGetFileChar = file.get();

		if (!bInquotes && cGetFileChar == '"')
		{
			bInquotes = true;
		}
		else if (bInquotes && cGetFileChar == '"')
		{
			if (file.peek() == '"')
			{
				StringStream << (char)file.get();
			}
			else
			{
				bInquotes = false;
			}
		}
		else if (!bInquotes && cGetFileChar == cDelimiter) // split ���� ������ stream �����͸� ���Ϳ� �ְ� �ʱ�ȭ
		{
			row.push_back(StringStream.str());
			StringStream.str("");
		}
		else if (!bInquotes && (cGetFileChar == '\r' || cGetFileChar == '\n')) // ���� ���� ������ stream �����͸� ���Ϳ� �ְ� �ʱ�ȭ
		{
			if (file.peek() == '\n') { file.get(); }
			row.push_back(StringStream.str());
			return row;
		}
		else
		{
			StringStream << cGetFileChar;
		}
	}

	return row;
}