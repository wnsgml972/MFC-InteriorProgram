#include "stdafx.h"
#include "FileManager.h"

FileManager* FileManager::instance = NULL;


FileManager* FileManager::GetInstance()
{
	if (!instance) {
		instance = new FileManager();
	}
	return instance;
}


//////////////////////////////////////////////////////////////////////////
/// ��ȯ��
string FileManager::CStringToString(CString cstr)
{
	char caResult[100];

#pragma warning(push)
#pragma warning(disable: 4996)
	strcpy(caResult, CT2A(cstr));
	if (caResult == NULL)
	{
		strcpy(caResult, "");
	}
	caResult[strlen(caResult) + 1] = '\0';
#pragma warning(pop)

	return caResult;
}

CString FileManager::StringToCString(string str)
{
	CString strResult;

	strResult = str.c_str();
	strResult += '\0';

	return strResult;
}

bool FileManager::isNumber(string s)
{
	if (s.size() == 0) 
		return false;

#pragma warning(push)
#pragma warning(disable: 4018)
	for (int i = 0; i < s.size(); i++) {
#pragma warning(pop)
		if ((s[i] >= '0' && s[i] <= '9') == false) {
			return false;
		}
	}
	return true;
}
FileManager::FileManager()
{

}
FileManager::~FileManager()
{
	SAFE_DELETE(instance);
}
void FileManager::SaveAsJson()
{

}
void FileManager::LoadAsJson()
{

}
/*
boolean FileManager::SaveAsCsv()
{
	PhoneBook *CPhoneBook = PhoneBook::getInstance();

	CString strPath = _T("C:\\Users\\kjh0121\\source\\repos\\Project1\\PhoneBookGUI\\media\\���̴ٽ��ּҷ�_20180214.csv");

	CString strStatus;

	//���� ����
	ofstream file(strPath);

#pragma warning(push)
#pragma warning(disable: 4018)
	//���� ����

	for (int i = 0; i < CPhoneBook->m_CaShape.size(); i++)
	{
		if (CPhoneBook->m_CaShape.at(i) == nullptr)
		{
			continue;
		}
		file << CStringToString(CPhoneBook->m_CaShape.at(i)->GetName()) << ',' << CStringToString(CPhoneBook->m_CaShape.at(i)->GetPhoneNumber())
			<< ',' << CStringToString(CPhoneBook->m_CaShape.at(i)->GetTitle()) << ',' << CStringToString(CPhoneBook->m_CaShape.at(i)->GetJob());
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
	PhoneBook *CPhoneBook = PhoneBook::getInstance();

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

	CPhoneBook->m_CaShape.clear();

	while (file.good()) //eof, bad, fail �Լ��� ������ ��ȯ�� ������
	{
		vector<string> row = CsvReadRow(file, ',');

		if (!row[0].find("#")) //���� csv ���� �ȿ� # ���ڰ� �������
		{
			continue; //�׳� �ǳ� �پ��
		}
		else //#���ڰ� ���� ���
		{
			strName = StringToCString(row[0]);
			strJob = StringToCString(row[1]);
			strTitle = StringToCString(row[2]);
			strPhoneNumber = StringToCString(row[3]);

			CPhoneBook->m_CaShape.push_back(new Person(CPhoneBook->m_AutoIncId++, strName, strPhoneNumber, strTitle, strJob));
		}
	}

	file.close(); //���� ����� �Ϸ� �� �ݾ��ش�.

	return TRUE;

}

vector<string> FileManager::CsvReadRow(istream &file, char cDelimiter)
{
	stringstream StringStream;
	bool bInquotes = false;  // �ο뱸, ū ����ǥ ���� �� ����
	vector<string> row;//relying on RVO

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
*/