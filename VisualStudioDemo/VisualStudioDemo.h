#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "FileManager.h"

/////////////////////////////////////////////////////////////////////////////
// CVisualStudioDemoApp:
// See VisualStudioDemo.cpp for the implementation of this class
//

class CVisualStudioDemoApp : public CWinAppEx
{
public:
	CVisualStudioDemoApp();
	virtual ~CVisualStudioDemoApp();

	BOOL m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	CMultiDocTemplate* m_pDocTemplateCpp;
	CMultiDocTemplate* m_pStartDocTemplate;

private:
	HINSTANCE m_hSciDLL;

public:
	std::shared_ptr<CFileManager> m_pManager;
	std::shared_ptr<CFileManager> GetManager()
	{
		return m_pManager;
	}

	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL SaveAllModified();

	// Implementation
	afx_msg void OnAppAbout();
	afx_msg void OnHelpShowStart();
	afx_msg void OnViewAppLook();

	DECLARE_MESSAGE_MAP()
};

extern CVisualStudioDemoApp theApp;
