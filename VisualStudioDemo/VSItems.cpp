#include "stdafx.h"
#include "VSItems.h"
#include "VisualStudioDemo.h"
#include "ScintillaDemoDoc.h"

const std::wstring CConfiguration::debug = L"debug";
const std::wstring CConfiguration::release = L"release";
const std::wstring CPlatform::x86 = L"x86";
const std::wstring CPlatform::x64 = L"x64";
const std::wstring CPlatform::anycpu = L"anycpu";
const std::wstring CTarget::exe = L"exe";
const std::wstring CTarget::winexe = L"winexe";
const std::wstring CTarget::library = L"library";
const std::wstring CTarget::module = L"module";
const std::wstring CExtension::exe = L"exe";
const std::wstring CExtension::dll = L"dll";
const std::wstring CExtension::module = L".o";


CSolution::CSolution()
{
	_buildSucceeded = false;
}

CSolution::~CSolution()
{
}

void CSolution::AddFileToProject(std::shared_ptr<CCodeFile> cf)
{
	_project._files.push_back(cf);
}

void CSolution::AddReferenceToProject(std::shared_ptr<CAssemblyFile> af)
{
	_project._references.push_back(af);
}

void CSolution::RemoveFileFromProject(CCodeFile * pCode)
{
	for (std::vector<std::shared_ptr<CCodeFile>>::const_iterator it = _project._files.begin(); it != _project._files.end(); it++)
	{
		std::shared_ptr<CCodeFile> pFile = *it;
		if (pCode->_name == pFile->_name)
		{
			_project._files.erase(it);
			return;
		}
	}
}

void CSolution::RemoveFileFromProject(CAssemblyFile * pAsm)
{
	for (std::vector<std::shared_ptr<CAssemblyFile>>::const_iterator it = _project._references.begin(); it != _project._references.end(); it++)
	{
		std::shared_ptr<CAssemblyFile> pFile = *it;
		if (pAsm->_name == pFile->_name)
		{
			_project._references.erase(it);
			return;
		}
	}
}

CProject::CProject()
{
}

CProject::~CProject()
{
}

CCodeFile::CCodeFile()
{
}

CCodeFile::~CCodeFile()
{
}

CAssemblyFile::CAssemblyFile()
{
}

CAssemblyFile::~CAssemblyFile()
{
}

CProjectProperties::CProjectProperties()
{
}

CProjectProperties::~CProjectProperties()
{
}

void CProjectProperties::SetConfiguration(const std::wstring & config)
{
	if (config == CConfiguration::debug)
		_configuration = CConfiguration::debug;
	if (config == CConfiguration::release)
		_configuration = CConfiguration::release;
}

void CProjectProperties::SetPlatform(const std::wstring & platform)
{
	if (platform == CPlatform::anycpu)
		_platform = CPlatform::anycpu;
	if (platform == CPlatform::x64)
		_platform = CPlatform::x64;
	if (platform == CPlatform::x86)
		_platform = CPlatform::x86;
}

void CProjectProperties::SetTarget(const std::wstring & target)
{
	if (target == CTarget::exe)
		_target = CTarget::exe;
	if (target == CTarget::winexe)
		_target = CTarget::winexe;
	if (target == CTarget::library)
		_target = CTarget::library;
}

CGlobalSettings::CGlobalSettings()
{
}

CGlobalSettings::~CGlobalSettings()
{
}
