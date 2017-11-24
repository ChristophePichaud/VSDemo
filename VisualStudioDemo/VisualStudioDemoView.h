#pragma once

class CVisualStudioDemoView : public CEditView
{
protected: // create from serialization only
	CVisualStudioDemoView();
	DECLARE_DYNCREATE(CVisualStudioDemoView)

	// Attributes
public:
	CVisualStudioDemoDoc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CVisualStudioDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void VerifyFindString(CMFCToolBarComboBoxButton* pFindCombo, CString& strFindText);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditTogglebreakpoint();
	afx_msg void OnFind();
	afx_msg void OnFilePrintPreview();
};

#ifndef _DEBUG  // debug version in VisualStudioDemoView.cpp
inline CVisualStudioDemoDoc* CVisualStudioDemoView::GetDocument() const { return reinterpret_cast<CVisualStudioDemoDoc*>(m_pDocument); }
#endif

