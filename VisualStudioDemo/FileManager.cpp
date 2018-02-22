#include "stdafx.h"
#include "VSItems.h"
#include "FileManager.h"
#include "MainFrm.h"
#include "VisualStudioDemo.h"
#include "Helper.h"

CFileManager::CFileManager()
{
	m_pSolution = std::make_shared<CSolution>();
}


CFileManager::~CFileManager()
{
}

void CFileManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//ar << m_size;
		//ar << m_paperColor;
		//ar << m_lastPoint;
	}
	else
	{
		//ar >> m_size;
		//ar >> m_paperColor;
		//ar >> m_lastPoint;
	}

	//m_objects.Serialize(this, ar);
}

void CFileManager::UpdateSolution(std::shared_ptr<CCodeFile> cf)
{
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	pmf->UpdateSolution(cf);
}

void CFileManager::UpdateSolution(std::shared_ptr<CAssemblyFile> af)
{
	CWnd * p = AfxGetMainWnd();
	CMainFrame * pmf = (CMainFrame *)p;
	pmf->UpdateSolution(af);
}

DWORD CFileManager::ExecuteCommand(LPTSTR lpszCmd)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bCreated = FALSE;
	SECURITY_ATTRIBUTES sa;
	TCHAR szTemp[4096];

	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;

	// Information sur la sortie standard pour CreateProcess
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = NULL; //GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.wShowWindow = SW_SHOW; //SW_HIDE;                                   // IMPORTANT: hide subprocess console window

																// Create process
	bCreated = CreateProcess(NULL, lpszCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (bCreated == FALSE)
	{
		_stprintf(szTemp, _T("CreateProcess %s failed GetLastError()=%ld"), lpszCmd, GetLastError());
		return false;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	DWORD dwExitCode = 0;
	GetExitCodeProcess(pi.hProcess, &dwExitCode);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	// Fin du process OK

	return dwExitCode;
}

bool CFileManager::ExecuteCommand(LPTSTR lpszCmd, LPSTR *lpszBuffer, DWORD *dwBufferLen, DWORD *dwExit)
{
	USES_CONVERSION;

	static long lCommandCount = 0;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	BOOL bCreated = FALSE;
	SECURITY_ATTRIBUTES sa;
	TCHAR szTemp[4096];
	TCHAR szCmdOutFileName[255];
	TCHAR szCmdFileName[255];

	SYSTEMTIME stCmd, stCmdOut;
	memset(&stCmd, 0, sizeof(SYSTEMTIME));
	GetSystemTime(&stCmd);
	memset(&stCmdOut, 0, sizeof(SYSTEMTIME));
	GetSystemTime(&stCmdOut);

	InterlockedIncrement(&lCommandCount);

	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&sa, 0, sizeof(SECURITY_ATTRIBUTES));

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;

	TCHAR lpszEnv[255];
	DWORD dw = 255;
	::GetEnvironmentVariable(_T("TEMP"), lpszEnv, dw);
	CString strTempFolder = lpszEnv;

	//::CreateDirectory((LPCTSTR)strTempFolder, NULL);

	_stprintf(szCmdFileName, _T("%s\\cmd_%02d%02d%02d_%08ld.cmd"), strTempFolder, stCmd.wHour, stCmd.wMinute, stCmd.wSecond, lCommandCount);
	_stprintf(szCmdOutFileName, _T("%s\\cmd_%02d%02d%02d_%08ld_out.txt"), strTempFolder, stCmd.wHour, stCmd.wMinute, stCmd.wSecond, lCommandCount);

	// Crꢴion du fichier .CMD
	HANDLE hCmdFile = CreateFile(szCmdFileName, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hCmdFile == INVALID_HANDLE_VALUE)
	{
		_stprintf(szTemp, _T("CreateFile CMD %s failed GetLastError()=%ld"), szCmdFileName, GetLastError());
		return false;
	}

	DWORD dwSize = 0;
	char * lpszAnsiCmd = T2A(lpszCmd);
	//if (WriteFile(hCmdFile, lpszCmd, dwLen, &dwSize, NULL) == FALSE)
	//if (WriteFile(hCmdFile, lpszCmd, _tcslen(lpszCmd) * sizeof(TCHAR), &dwSize, NULL) == FALSE)
	if (WriteFile(hCmdFile, lpszAnsiCmd, strlen(lpszAnsiCmd) + 1, &dwSize, NULL) == FALSE)
	{
		_stprintf(szTemp, _T("WriteFile failed %s GetLastError()=%ld"), lpszCmd, GetLastError());
		return false;
	}
	CloseHandle(hCmdFile);
	// Fichier .CMD  OK

	// Cration du fichier de sortie OUT
	HANDLE hCmdOutFile = CreateFile(szCmdOutFileName, GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hCmdOutFile == INVALID_HANDLE_VALUE)
	{
		_stprintf(szTemp, _T("CreateFile CMDOUT %s failed GetLastError()=%ld"), szCmdOutFileName, GetLastError());
		return false;
	}

	// Information sur la sortie standard pour CreateProcess
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.hStdOutput = hCmdOutFile;
	si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.wShowWindow = SW_HIDE;						// IMPORTANT: hide subprocess console window

	// Create process
	bCreated = CreateProcess(NULL, szCmdFileName, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	if (bCreated == FALSE)
	{
		_stprintf(szTemp, _T("CreateProcess %s failed GetLastError()=%ld"), szCmdFileName, GetLastError());
		return false;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	/*DWORD*/ *dwExit = 0;
	GetExitCodeProcess(pi.hProcess, dwExit);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	// Fin du process OK

	FlushFileBuffers(hCmdOutFile);
	CloseHandle(hCmdOutFile);

	HANDLE hReadCmdOutFile;
	int iRetry = 0;
	while (iRetry<10)
	{
		iRetry++;

		hReadCmdOutFile = CreateFile(szCmdOutFileName, GENERIC_READ, 0, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hReadCmdOutFile == INVALID_HANDLE_VALUE)
		{
			//sprintf(szTemp, "CreateFile %s failed GetLastError()=%ld", szCmdOutFileName, GetLastError());
			//_Module.LogError(szTemp);
		}
		else
		{
			break;
		}
		Sleep(10);
		//printf("Retry  open %s ...\n", szCmdOutFileName);
	}
	if (hReadCmdOutFile == INVALID_HANDLE_VALUE)
	{
		_stprintf(szTemp, _T("CreateFile %s failed 10 times GetLastError()=%ld"), szCmdOutFileName, GetLastError());
		return false;
	}

	dwSize = GetFileSize(hReadCmdOutFile, NULL);

	*lpszBuffer = new char[dwSize + 1];
	memset(*lpszBuffer, 0, (dwSize + 1));

	DWORD dwRead = 0;
	if (ReadFile(hReadCmdOutFile, *lpszBuffer, dwSize, &dwRead, NULL) == FALSE)
	{
		_stprintf(szTemp, _T("ReadFile %s failed GetLastError()=%ld"), szCmdOutFileName, GetLastError());
		return false;
	}

	*dwBufferLen = dwSize;
	//OemToChar(*lpszBuffer, *lpszBuffer);

	CloseHandle(hReadCmdOutFile);
	// Fichiers .CMD  et OUT.TXT ࡤ굲uire
	//DeleteFile(szCmdFileName);
	//DeleteFile(szCmdOutFileName);

	if (*dwExit != 0)
		return false;

	return true;
}

void CFileManager::LoadFile(CCodeFile * pCode)
{
	CVisualStudioDemoApp * pApp = (CVisualStudioDemoApp *)AfxGetApp();
	LPTSTR lpszPath = (LPTSTR)(LPCTSTR) (pCode->_path.c_str());
	pApp->m_pDocTemplateCpp->OpenDocumentFile(lpszPath);
}

CString CFileManager::SearchDrive(const CString& strFile, const CString& strFilePath, const bool& bRecursive, const bool& bStopWhenFound, HTREEITEM parent)
{
	USES_CONVERSION;

	CWnd * pWnd = AfxGetMainWnd();
	CMainFrame * pMainFrame = (CMainFrame *)pWnd;

	CString strFoundFilePath;
	WIN32_FIND_DATA file;

	CString strPathToSearch = strFilePath;
	strPathToSearch += _T("\\");

	HANDLE hFile = FindFirstFile((strPathToSearch + "*"), &file);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString strTheNameOfTheFile = file.cFileName;

			// It could be a directory we are looking at
			// if so look into that dir
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((strTheNameOfTheFile != ".") && (strTheNameOfTheFile != "..") && (bRecursive))
				{
					HTREEITEM hItem = pMainFrame->m_wndFileView.m_wndFileView.InsertItem(strTheNameOfTheFile, 0, 0, parent);

					strFoundFilePath = SearchDrive(strFile, strPathToSearch + strTheNameOfTheFile, bRecursive, bStopWhenFound, hItem);

					if (!strFoundFilePath.IsEmpty() && bStopWhenFound)
						break;
				}
			}
			else
			{
				//if (strTheNameOfTheFile == strFile)
				{
					strFoundFilePath = strPathToSearch + strTheNameOfTheFile; //strFile;

																			  /// TODO
																			  // ADD TO COLLECTION TYPE
					std::shared_ptr<CCodeFile> cf = std::make_shared<CCodeFile>();
					cf->_name = T2W((LPTSTR)(LPCTSTR)strTheNameOfTheFile); //strFile;
					cf->_path = T2W((LPTSTR)(LPCTSTR)strFoundFilePath);

					pMainFrame->GetManager()->m_pSolution->AddFileToProject(cf);
					//this->UpdateSolution(cf);
					HTREEITEM hItem = pMainFrame->m_wndFileView.m_wndFileView.InsertItem(strTheNameOfTheFile /*cf->_name.c_str()*/, 1, 1, parent);
					pMainFrame->m_wndFileView.m_wndFileView.SetItemData(hItem, (DWORD_PTR)(cf.get()));

					if (bStopWhenFound)
						break;
				}
			}
		} while (FindNextFile(hFile, &file));

		FindClose(hFile);
	}

	return strFoundFilePath;
}

CString CFileManager::GetSolutionName()
{
	USES_CONVERSION;

	CString strWorkingDir = W2T((LPTSTR)(m_pSolution->_properties._workingDirectory.c_str()));
	CString strSolutionName = W2T((LPTSTR)(m_pSolution->_name.c_str()));

	CString strIniFile;
	strIniFile.Format(_T("%s\\Solution_%s.ini"), strWorkingDir, strSolutionName);
	return strIniFile;
}

bool CFileManager::BuildTheSolution()
{
	USES_CONVERSION;

	CWnd * pWnd = AfxGetMainWnd();
	CMainFrame * pMainFrame = (CMainFrame *)pWnd;

	CString strCompilerFilePath = (LPTSTR)(LPCTSTR) m_pSolution->_settings._compilerFilePath.c_str();
	CString strWorkingDir = (LPTSTR)(LPCTSTR)m_pSolution->_properties._workingDirectory.c_str();
	CString strSolutionName = (LPTSTR)(LPCTSTR)m_pSolution->_name.c_str();
	//CString strProjectName = m_pSolution->_project._name.c_str();
	CString strTarget = (LPTSTR)(LPCTSTR)m_pSolution->_properties._target.c_str();
	CString strConfiguration = (LPTSTR)(LPCTSTR)m_pSolution->_properties._configuration.c_str();
	CString strPlatform = (LPTSTR)(LPCTSTR)m_pSolution->_properties._platform.c_str();
	bool bEmitDebuggingInfomration = m_pSolution->_properties._emitDebugInformation;

	//
	// Create Debug & Release folder
	//

	CString strDebugFolder;
	strDebugFolder.Format(_T("%s\\Debug"), strWorkingDir);
	::CreateDirectory((LPCTSTR)strDebugFolder, NULL);
	m_pSolution->_project._debugFolder = T2W((LPTSTR)(LPCTSTR)strDebugFolder);

	CString strReleaseFolder;
	strReleaseFolder.Format(_T("%s\\Release"), strWorkingDir);
	::CreateDirectory((LPCTSTR)strReleaseFolder, NULL);
	m_pSolution->_project._releaseFolder = T2W((LPTSTR)(LPCTSTR)strReleaseFolder);

	//
	// Determine a exe, dll or module
	//

	CString strExt;
	CString strTargetOpt;
	if (strTarget == CTarget::exe.c_str())
	{
		strTargetOpt = strTarget;
		strExt = CExtension::exe.c_str();
	}
	if (strTarget == CTarget::library.c_str())
	{
		strTargetOpt = strTarget;
		strExt = CExtension::dll.c_str();
	}
	if (strTarget == CTarget::winexe.c_str())
	{
		strTargetOpt = strTarget;
		strExt = CExtension::exe.c_str();
	}
	if (strTarget == CTarget::module.c_str())
	{
		strTargetOpt = strTarget;
		strExt = CExtension::module.c_str();
	}

	//
	// Debug mode
	//

	bool bDebug = false;
	if (strConfiguration == CConfiguration::debug.c_str())
		bDebug = true;
	else
		bDebug = false;

	//
	// Final assembly output
	//

	CString strOutputFolder;
	if (bDebug == true)
		strOutputFolder = strDebugFolder;
	else
		strOutputFolder = strReleaseFolder;

	CString strOutput;
	strOutput.Format(_T("%s\\%s.%s"), strOutputFolder, strSolutionName, strExt);
	m_pSolution->_project._outputAssembly = T2W((LPTSTR)(LPCTSTR)strOutput);

	//
	// Output for Roslyn
	//

	CString strCmd;
	strCmd.Format(_T("%s /target:%s /out:%s\\%s.%s "), strCompilerFilePath, strTargetOpt, strOutputFolder, strSolutionName, strExt);

	if (bDebug == true)
	{
		if (bEmitDebuggingInfomration == true)
			strCmd = strCmd + _T("/debug+ /debug:full ");
		else
			strCmd = strCmd + _T("/debug:full ");
	}

	//
	// Choose platform
	//

	CString strPlatformOpt;
	if (strPlatform == CPlatform::anycpu.c_str())
	{
		strPlatformOpt = strPlatform;
	}
	if (strPlatform == CPlatform::x86.c_str())
	{
		strPlatformOpt = strPlatform;
	}
	if (strPlatform == CPlatform::x64.c_str())
	{
		strPlatformOpt = strPlatform;
	}

	strCmd = strCmd + _T("/platform:") + strPlatformOpt + _T(" ");

	//
	// List of files to compile
	//

	for (auto file : m_pSolution->_project._files)
	{
		LPTSTR lpszExt = ::PathFindExtension(W2T((LPTSTR)(file->_name.c_str())));
		CString strExt = lpszExt;
		if (strExt != _T(".cs"))
			continue;

		CString strTemp;
		strTemp.Format(_T("%s "), file->_path.c_str());

		strCmd = strCmd + strTemp;
	}

	//
	// List of references to add
	//

	//if (m_pSolution->_project._references.size() != 0)
	//{
	//	strCmd = strCmd + _T(" /reference:");
	//}

	for (auto file : m_pSolution->_project._references)
	{
		CString strTemp;
		strTemp.Format(_T("/reference:%c%s%c "), '"', file->_path.c_str(), '"');

		strCmd = strCmd + strTemp;
	}

	CString strIniFile = GetSolutionName();
	::WritePrivateProfileString(_T("Solution"), _T("LastCompileCmd"), strCmd, strIniFile);

	// Clear Build Window
	pMainFrame->m_wndOutputView.ClearBuildWindow();

	// Copy references
	for (auto file : m_pSolution->_project._references)
	{
		LPCTSTR lpszSource = W2T((LPTSTR)(file->_path.c_str()));
		CString strDestination;
		strDestination.Format(_T("%s\\%s"), strOutputFolder, file->_name.c_str());
		LPCTSTR lpszDest = (LPCTSTR)strDestination;
		BOOL b = ::CopyFile(lpszSource, lpszDest, FALSE);
		if (b == FALSE)
		{
			DWORD dw = ::GetLastError();
		}
	}

	// TODO: Add your command handler code here
	char * lpszBuffer;
	DWORD dwCount = 0;
	DWORD dwExit = 0;
	ExecuteCommand((LPTSTR)(LPCTSTR)strCmd, &lpszBuffer, &dwCount, &dwExit);

	//::MessageBoxA(NULL, lpszBuffer, "Output", MB_OK);

	CString strMsg;
	strMsg.Format(_T("--------------------Configuration: %s - %s %s--------------------"), strSolutionName, strConfiguration, strPlatform);
	pMainFrame->m_wndOutputView.AddString(strMsg);

	std::string str = lpszBuffer;
	std::vector<std::string> v = CHelper::Split(str, "\r\n");
	for (std::string s : v)
	{
		if (s.empty())
			continue;

		CString line = CString(s.c_str());
		pMainFrame->m_wndOutputView.AddString(line);
	}

	//strMsg.Format(_T("Return code: %ld"), dwExit);
	//pMainFrame->m_wndOutputView.AddString(strMsg);

	strMsg.Format(_T("Creating output assembly => %s"), strOutput);
	pMainFrame->m_wndOutputView.AddString(strMsg);

	if (dwExit == 0)
	{
		strMsg.Format(_T("======= Build: 1 succeeded ========"));
		pMainFrame->m_wndOutputView.AddString(strMsg);
		
		m_pSolution->_buildSucceeded = true;
	}
	else
	{
		strMsg.Format(_T("======= Build: 1 failed ========"));
		pMainFrame->m_wndOutputView.AddString(strMsg);

		m_pSolution->_buildSucceeded = false;
	}

	return true;
}

bool CFileManager::SaveSolution()
{
	USES_CONVERSION;

	CString strWorkingDir = W2T((LPTSTR)(m_pSolution->_properties._workingDirectory.c_str()));
	if (strWorkingDir.IsEmpty())	
	{
		AfxMessageBox(_T("Go to Project Properties and Set Working Folder for the Solution"));
		return false;
	}

	CString strSolutionName = W2T((LPTSTR)(m_pSolution->_name.c_str()));
	if (strSolutionName.IsEmpty())
	{
		AfxMessageBox(_T("Go to Project Properties and Set Solution Name"));
		return false;
	}

	CString strIniFile = GetSolutionName();

	BOOL bDelete = FALSE;
	bDelete = ::DeleteFile((LPCTSTR)strIniFile);

	::WritePrivateProfileString(_T("Solution"), _T("Name"), m_pSolution->_name.c_str(), strIniFile);
	::WritePrivateProfileString(_T("Solution"), _T("WorkingDir"), m_pSolution->_properties._workingDirectory.c_str(), strIniFile);
	::WritePrivateProfileString(_T("Solution"), _T("CompilerPath"), m_pSolution->_settings._compilerFilePath.c_str(), strIniFile);
	::WritePrivateProfileString(_T("Solution"), _T("Configuration"), m_pSolution->_properties._configuration.c_str(), strIniFile);
	::WritePrivateProfileString(_T("Solution"), _T("Platform"), m_pSolution->_properties._platform.c_str(), strIniFile);
	::WritePrivateProfileString(_T("Solution"), _T("Target"), m_pSolution->_properties._target.c_str(), strIniFile);

	int count = m_pSolution->_project._files.size();
	CString str;
	str.Format(_T("%d"), count);
	::WritePrivateProfileString(_T("Solution"), _T("FileCount"), str, strIniFile);

	count = 1;
	for (auto file : m_pSolution->_project._files)
	{
		CString strFile;
		strFile.Format(_T("%s "), file->_path.c_str());

		str.Format(_T("File_%d"), count);
		::WritePrivateProfileString(_T("Solution"), str, strFile, strIniFile);

		count++;
	}

	count = m_pSolution->_project._references.size();
	str.Format(_T("%d"), count);
	::WritePrivateProfileString(_T("Solution"), _T("ReferenceCount"), str, strIniFile);

	count = 1;
	for (auto file : m_pSolution->_project._references)
	{
		CString strFile;
		strFile.Format(_T("%s "), file->_path.c_str());

		str.Format(_T("Reference_%d"), count);
		::WritePrivateProfileString(_T("Solution"), str, strFile, strIniFile);

		count++;
	}

	return true;
}

bool CFileManager::OpenSolution()
{
	USES_CONVERSION;

	CWnd * pWnd = AfxGetMainWnd();
	CMainFrame * pMainFrame = (CMainFrame *)pWnd;

	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDCANCEL)
		return false;

	this->ClearSolution();

	CString strFileName = dlg.GetFileName();
	CString strPath = dlg.GetFolderPath();
	CString strIniFile;
	strIniFile.Format(_T("%s\\%s"), strPath, strFileName);

	TCHAR lpszBuffer[255];
	DWORD dwCount = sizeof(TCHAR) * 255;
	::GetPrivateProfileString(_T("Solution"), _T("Name"), NULL, lpszBuffer, dwCount, strIniFile);
	m_pSolution->_name = T2W(lpszBuffer);
	::GetPrivateProfileString(_T("Solution"), _T("WorkingDir"), NULL, lpszBuffer, dwCount, strIniFile);
	m_pSolution->_properties._workingDirectory = T2W(lpszBuffer);
	::GetPrivateProfileString(_T("Solution"), _T("CompilerPath"), NULL, lpszBuffer, dwCount, strIniFile);
	m_pSolution->_settings._compilerFilePath = T2W(lpszBuffer);
	::GetPrivateProfileString(_T("Solution"), _T("FileCount"), NULL, lpszBuffer, dwCount, strIniFile);
	int count = _tstoi(lpszBuffer);
	::GetPrivateProfileString(_T("Solution"), _T("Configuration"), NULL, lpszBuffer, dwCount, strIniFile);
	m_pSolution->_properties.SetConfiguration(T2W(lpszBuffer));
	::GetPrivateProfileString(_T("Solution"), _T("Platform"), NULL, lpszBuffer, dwCount, strIniFile);
	m_pSolution->_properties.SetPlatform(T2W(lpszBuffer));
	::GetPrivateProfileString(_T("Solution"), _T("Target"), NULL, lpszBuffer, dwCount, strIniFile);
	m_pSolution->_properties.SetTarget(T2W(lpszBuffer));
	::GetPrivateProfileString(_T("Solution"), _T("ReferenceCount"), NULL, lpszBuffer, dwCount, strIniFile);
	int countRef = _tstoi(lpszBuffer);

	for (int c = 1; c <= count; ++c)
	{
		CString str;
		str.Format(_T("File_%d"), c);

		::GetPrivateProfileString(_T("Solution"), str, NULL, lpszBuffer, dwCount, strIniFile);

		std::shared_ptr<CCodeFile> cf = std::make_shared<CCodeFile>();
		cf->_name = T2W(::PathFindFileName(lpszBuffer));
		cf->_path = T2W(lpszBuffer);

		m_pSolution->AddFileToProject(cf);
		this->UpdateSolution(cf);
	}

	for (int c = 1; c <= countRef; ++c)
	{
		CString str;
		str.Format(_T("Reference_%d"), c);

		::GetPrivateProfileString(_T("Solution"), str, NULL, lpszBuffer, dwCount, strIniFile);

		std::shared_ptr<CAssemblyFile> af = std::make_shared<CAssemblyFile>();
		af->_name = T2W(::PathFindFileName(lpszBuffer));
		af->_path = T2W(lpszBuffer);

		m_pSolution->AddReferenceToProject(af);
		pMainFrame->UpdateSolution(af);
	}

	return true;
}

void CFileManager::ClearSolution()
{
	CWnd * pWnd = AfxGetMainWnd();
	CMainFrame * pMainFrame = (CMainFrame *)pWnd;

	// TreeView
	pMainFrame->m_wndFileView.FillFileView();
	// Solution items
	m_pSolution->_project._files.clear();
	m_pSolution->_project._references.clear();
	m_pSolution->_name = L"";
	m_pSolution->_properties._workingDirectory = L"";
}

void CFileManager::RunProgram()
{
	USES_CONVERSION;

	CString str = W2T((LPTSTR)m_pSolution->_project._outputAssembly.c_str());
	ExecuteCommand((LPTSTR)(LPCTSTR)str);
}
