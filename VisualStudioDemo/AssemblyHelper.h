#pragma once
#include "VSItems.h"

class CAssemblyHelper
{
public:
	CAssemblyHelper();
	virtual ~CAssemblyHelper();
public:
	static CString SearchDrive(const CString& strFile, const CString& strFilePath, const bool& bRecursive, std::vector<std::shared_ptr<CAssemblyFile>> & vData);
	static std::vector<std::shared_ptr<CAssemblyFile>> CAssemblyHelper::FillAssembliesFromDir(CString strPath);
	static CString GetWindowsDirectory();
	static CString GetFrameworkPath();
	static CString GetGACPath();
};


