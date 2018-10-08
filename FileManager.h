#pragma once


//////////////////////////////////////////////////////////////////////////
/// fole load, save ½Ã ÇÊ¿ä
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include "define.h"

class CFileManager
{
public:

	static CFileManager* GetInstance();

	~CFileManager();

	void SaveAsJson();
	void LoadAsJson();
	bool SaveAsCsv();
	bool LoadAsCsv();
	vector<string> CsvReadRow(istream &file, char cDelimiter);

private:
	CFileManager();
	static CFileManager *instance;
};