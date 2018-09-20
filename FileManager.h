#pragma once

#include "define.h"

//////////////////////////////////////////////////////////////////////////
/// fole load, save ½Ã ÇÊ¿ä
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>

class FileManager
{
public:

	static FileManager* GetInstance();

	~FileManager();

	void SaveAsJson();
	void LoadAsJson();
	boolean SaveAsCsv();
	boolean LoadAsCsv();
	vector<string> CsvReadRow(istream &file, char cDelimiter);

	string CStringToString(CString cstr);
	CString StringToCString(string str);
	bool isNumber(string s);

private:
	FileManager();
	static FileManager *instance;
};