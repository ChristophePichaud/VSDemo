#pragma once

#include "OptionsPages.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

class COptionsDlg : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(COptionsDlg)

// Construction
public:
	COptionsDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	COptionsPage1  m_Page11;
	COptionsPage12 m_Page12;
	COptionsPage21 m_Page21;
	COptionsPage22 m_Page22;
	COptionsPage31 m_Page31;
	COptionsPage32 m_Page32;

// Implementation
public:
	virtual ~COptionsDlg();

protected:
	DECLARE_MESSAGE_MAP()
};

