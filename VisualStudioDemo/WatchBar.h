#pragma once

/////////////////////////////////////////////////////////////////////////////
// CWatchBar window

class CWatchBar : public CDockablePane
{
// Construction
public:
	CWatchBar();

// Attributes
protected:
	CFont m_Font;
	CMFCListCtrl m_wndWatch;

// Implementation
public:
	virtual ~CWatchBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

