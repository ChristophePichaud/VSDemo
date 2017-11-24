#pragma once

#include "ClassTreeWnd.h"

class CResourceViewBar : public CDockablePane
{
// Construction
public:
	CResourceViewBar();

// Attributes
protected:
	CClassTreeWnd m_wndResourceView;
	CImageList m_ResourceViewImages;

public:
	void OnChangeVisualStyle();

protected:
	void FillResourceView();

// Implementation
public:
	virtual ~CResourceViewBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};

