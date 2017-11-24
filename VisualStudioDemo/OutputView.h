#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList window

class COutputList : public CListBox
{
// Construction
public:
	COutputList();

// Implementation
public:
	virtual ~COutputList();

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnOutputGotoError();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputViewBar : public CDockablePane
{
// Construction
public:
	COutputViewBar();

	void UpdateFonts();

// Attributes
protected:
	COutputList m_wndOutput;

protected:
	void FillDebugWindow();
	void FillBuildWindow();
	void AdjusrHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputViewBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

