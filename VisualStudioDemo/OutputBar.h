#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList1 window

class COutputList1 : public CListBox
{
// Construction
public:
	COutputList1();

// Implementation
public:
	virtual ~COutputList1();

protected:
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnOutputGotoError();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COutputBar window

class COutputBar : public CDockablePane
{
// Construction
public:
	COutputBar();

// Attributes
protected:
	CMFCTabCtrl m_wndTabs;
	CFont m_Font;

	COutputList1 m_wndOutputBuild;
	COutputList1 m_wndOutputDebug;
	COutputList1 m_wndOutputFind1;
	COutputList1 m_wndOutputFind2;
	COutputList1 m_wndOutputSQL;

// Operations
public:
	void SetFlatTabs(BOOL bFlat = TRUE, BOOL bRepaint = TRUE)
	{
		m_wndTabs.SetFlatFrame(bFlat, bRepaint);
	}
	void AddString(CString strMessage);

public:
	void ClearBuildWindow();

protected:
	void FillBuildWindow();
	void FillDebugWindow();
	void AdjusrHorzScroll(CListBox& wndListBox);

// Implementation
public:
	virtual ~COutputBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};


