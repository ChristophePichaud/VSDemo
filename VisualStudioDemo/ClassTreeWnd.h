#pragma once

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd window

class CClassTreeWnd : public CTreeCtrl
{
// Construction
public:
	CClassTreeWnd();

// Overrides
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CClassTreeWnd();

protected:
	DECLARE_MESSAGE_MAP()
};
