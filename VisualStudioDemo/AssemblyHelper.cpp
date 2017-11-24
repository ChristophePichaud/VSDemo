#include "stdafx.h"
#include "AssemblyHelper.h"
#include "VSItems.h"

CAssemblyHelper::CAssemblyHelper()
{
}


CAssemblyHelper::~CAssemblyHelper()
{
}

CString CAssemblyHelper::SearchDrive(const CString& strFile, const CString& strFilePath, const bool& bRecursive, std::vector<std::shared_ptr<CAssemblyFile>> & vData)
{
	CString strFoundFilePath;
	WIN32_FIND_DATA file;

	CString strPathToSearch = strFilePath;
	strPathToSearch += _T("\\");

	HANDLE hFile = FindFirstFile(strPathToSearch + _T("*"), &file);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString strTheNameOfTheFile = file.cFileName;

			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((strTheNameOfTheFile != ".") && (strTheNameOfTheFile != "..") && (bRecursive))
				{
					strFoundFilePath = SearchDrive(strFile, strPathToSearch + strTheNameOfTheFile, bRecursive, vData);
				}
			}
			else
			{
				strFoundFilePath = strPathToSearch + strTheNameOfTheFile; //strFile;

				LPTSTR lpszExt = ::PathFindExtension((LPCTSTR)strFoundFilePath);
				CString strExt = lpszExt;
				if (strExt != _T(".dll"))
					continue;
				
				int index = strFoundFilePath.Find(_T("System."));
				if (index == -1)
					continue;

				std::shared_ptr<CAssemblyFile> pAsm = std::make_shared<CAssemblyFile>();
				pAsm->_name = strTheNameOfTheFile;
				pAsm->_path = strFoundFilePath;

				vData.push_back(pAsm);
			}
		} while (FindNextFile(hFile, &file));

		FindClose(hFile);
	}

	return strFoundFilePath;
}

std::vector<std::shared_ptr<CAssemblyFile>> CAssemblyHelper::FillAssembliesFromDir(CString strPath)
{
	std::vector<std::shared_ptr<CAssemblyFile>> vec;
	SearchDrive(_T("*.dll"), strPath, true, vec);
	return vec;
}

CString CAssemblyHelper::GetWindowsDirectory()
{
	TCHAR lpszWinDir[MAX_PATH];
	::GetWindowsDirectory(lpszWinDir, MAX_PATH);
	CString str = lpszWinDir;
	return str;
}

CString CAssemblyHelper::GetFrameworkPath()
{
	CString strWinDir = CAssemblyHelper::GetWindowsDirectory();
	CString str;
	str.Format(_T("%s\\Microsoft.NET\\Framework\\v4.0.30319"), strWinDir);
	return str;
}

CString CAssemblyHelper::GetGACPath()
{
	CString strWinDir = CAssemblyHelper::GetWindowsDirectory();
	CString str;
	str.Format(_T("%s\\assembly"), strWinDir);
	return str;
}
