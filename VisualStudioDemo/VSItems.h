#pragma once
#include "ScintillaDemoView.h"

class CCodeFile;
class CAssemblyFile;
class CProjectProperties;
class CGlobalSettings;
class CProject;
class CSolution;

// Analyse des trucs à designer ...

class CProject
{
public:
	CProject();
	virtual ~CProject();

public:
	std::vector<std::shared_ptr<CCodeFile>> _files;
	std::vector<std::shared_ptr<CAssemblyFile>> _references;
	std::wstring _name;
}; 

class CCodeFile
{
public:
	CCodeFile();
	virtual ~CCodeFile();

public:
	std::wstring _name;
	std::wstring _path;
};
	
class CAssemblyFile
{
public:
	CAssemblyFile();
	virtual ~CAssemblyFile();

public:
	std::wstring _name;
	std::wstring _path;
};

class CProjectProperties
{
public:
	CProjectProperties();
	virtual ~CProjectProperties();

// wrappers
public:
	void SetConfiguration(const std::wstring & config);
	void SetPlatform(const std::wstring & platform);
	void SetTarget(const std::wstring & target);

public:
	std::wstring _workingDirectory;
	std::wstring _configuration;
	std::wstring _platform;
	std::wstring _target;
	bool _emitDebugInformation;
};

class CGlobalSettings
{
public:
	CGlobalSettings();
	virtual ~CGlobalSettings();

public:
	std::wstring _compilerFilePath;
};

class CSolution
{
public:
	CSolution();
	virtual ~CSolution();

public:
	std::wstring _name;
	CProject _project;
	CProjectProperties _properties;
	CGlobalSettings _settings;

public:
	void AddFileToProject(std::shared_ptr<CCodeFile> pCodeFile);
	void RemoveFileFromProject(CCodeFile * pCode);
	void AddReferenceToProject(std::shared_ptr<CAssemblyFile> af);
	void RemoveFileFromProject(CAssemblyFile * pAsm);
};

class CConfiguration
{
public:
	static const std::wstring debug;
	static const std::wstring release;
};

class CPlatform
{
public:
	static const std::wstring x86;
	static const std::wstring x64;
	static const std::wstring anycpu;
};

class CTarget
{
public:
	static const std::wstring exe;
	static const std::wstring winexe;
	static const std::wstring library;
	static const std::wstring module;
};

class CExtension
{
public:
	static const std::wstring exe;
	static const std::wstring dll;
	static const std::wstring module;
};
