#pragma once

class CUndoBar;

/////////////////////////////////////////////////////////////////////////////
// CUndoListBox window

class CUndoListBox : public CListBox
{
// Construction
public:
	CUndoListBox(CUndoBar& bar);

// Attributes
protected:
	CUndoBar& m_Bar;

// Implementation
public:
	virtual ~CUndoListBox();

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
