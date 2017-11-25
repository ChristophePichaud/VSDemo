#pragma once
#include "VSItems.h"

class CFileManager : public CObject
{
public:
	CFileManager();
	virtual ~CFileManager();

public:
	void Serialize(CArchive& ar);
	void UpdateSolution(std::shared_ptr<CCodeFile> cf);
	void UpdateSolution(std::shared_ptr<CAssemblyFile> af);
	void LoadFile(CCodeFile * pCode);

public:
	bool ExecuteCommand(LPTSTR lpszCmd, LPSTR *lpszBuffer, DWORD *dwBufferLen);
	bool BuildTheSolution();
	CString GetSolutionName();
	bool SaveSolution();
	bool OpenSolution();
	void ClearSolution();

public:
	static CString SearchDrive(const CString& strFile, const CString& strFilePath, const bool& bRecursive, const bool& bStopWhenFound, HTREEITEM parent);

public:
	std::shared_ptr<CSolution> m_pSolution;
};

