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
/// 변환기
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

// 원하는 작업 영역 캡쳐!!!
// 개좋음~~~ 그냥 캡쳐 하고 싶은 Frame에다가 이 코드 넣으면 됨
void FileManager::SpecificAreaCapture(CWnd *pcwnd)
{
	// View::OnLButtonDown
	// 바탕 화면 윈도우 객체에 대한 포인터를 얻는다.

	CWnd* pWndDesktop = pcwnd->GetDesktopWindow();
	// 바탕 화면 윈도우 DC
	CWindowDC ScrDC(pWndDesktop);
	// 뷰 윈도우 DC
	CClientDC dc(pcwnd);

	// Rect를 사용해서 작업 영역에 대한 좌표를 얻고,
	CRect Rect;
	pcwnd->GetClientRect(&Rect);

	// 그 위치를 현재 윈도우 절대 좌표로 변경해 주자.
	pcwnd->GetWindowRect(&Rect);

	// CImage를 하나 만들고
	CImage Image;
	// 스캔을 시작할 x, y 위치와
	int sx = Rect.left;
	int sy = Rect.top;
	// 작업 영역에 대한 크기를 각각 임시로 저장해 두자.
	int cx = Rect.Width();
	int cy = Rect.Height();

	// 작업 영역의 크기만큼, 현재 바탕화면의 속성과 동일한 image를 생성한다.
	Image.Create(cx, cy, ScrDC.GetDeviceCaps(BITSPIXEL));
	// 이미지 DC에 현재 작업 영역의 절대 좌표를 사용해 그 크기만큼 저장하게 한다.
	CDC* pDC = CDC::FromHandle(Image.GetDC());
	pDC->BitBlt(0, 0, cx, cy, &ScrDC, sx, sy, SRCCOPY);
	Image.ReleaseDC();

	// 저장된 이미지를 원하는 파일명, 포멧타입을 지정해서 저장한다.
	Image.Save(TEXT("media\\capture\\image.png"), Gdiplus::ImageFormatJPEG);

	// 그 파일을 실행해 준다.
	// 응용 프로그램을 실행시킬 수 있음!
	ShellExecute(NULL, TEXT("open"), TEXT("media\\capture\\image.png"), NULL, NULL, SW_SHOW);
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

	CString strPath = _T("C:\\Users\\kjh0121\\source\\repos\\Project1\\PhoneBookGUI\\media\\마이다스주소록_20180214.csv");

	CString strStatus;

	//파일 열기
	ofstream file(strPath);

#pragma warning(push)
#pragma warning(disable: 4018)
	//파일 쓰기

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

	if (file.fail())  //만약 bad() 함수가 실패 하면..
	{
		return FALSE;
	}

	file.close(); //파일 입출력 완료 후 닫아준다.

	return TRUE;
}

boolean FileManager::LoadAsCsv()
{
	PhoneBook *CPhoneBook = PhoneBook::getInstance();

	CString strPath = _T("C:\\Users\\kjh0121\\source\\repos\\Project1\\PhoneBookGUI\\media\\마이다스주소록_20180214.csv");

	CString strName;
	CString strPhoneNumber;
	CString strTitle;
	CString strJob;


	//ifstream은 파일을 읽게 해주는 함수로써 ifstream (파일명 or 경로)
	ifstream file(strPath);

	if (file.fail())  //만약 bad() 함수가 실패 하면..
	{
		return FALSE;
	}

	CPhoneBook->m_CaShape.clear();

	while (file.good()) //eof, bad, fail 함수가 거짓을 반환할 때까지
	{
		vector<string> row = CsvReadRow(file, ',');

		if (!row[0].find("#")) //만약 csv 파일 안에 # 문자가 있을경우
		{
			continue; //그냥 건너 뛰어라
		}
		else //#문자가 없을 경우
		{
			strName = StringToCString(row[0]);
			strJob = StringToCString(row[1]);
			strTitle = StringToCString(row[2]);
			strPhoneNumber = StringToCString(row[3]);

			CPhoneBook->m_CaShape.push_back(new Person(CPhoneBook->m_AutoIncId++, strName, strPhoneNumber, strTitle, strJob));
		}
	}

	file.close(); //파일 입출력 완료 후 닫아준다.

	return TRUE;

}

vector<string> FileManager::CsvReadRow(istream &file, char cDelimiter)
{
	stringstream StringStream;
	bool bInquotes = false;  // 인용구, 큰 따옴표 같은 것 제거
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
		else if (!bInquotes && cGetFileChar == cDelimiter) // split 문자 만나면 stream 데이터를 벡터에 넣고 초기화
		{
			row.push_back(StringStream.str());
			StringStream.str("");
		}
		else if (!bInquotes && (cGetFileChar == '\r' || cGetFileChar == '\n')) // 다음 줄을 만나도 stream 데이터를 벡터에 넣고 초기화
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