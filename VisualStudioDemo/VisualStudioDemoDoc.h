#pragma once

class CVisualStudioDemoDoc : public CDocument
{
protected: // create from serialization only
	CVisualStudioDemoDoc();
	DECLARE_DYNCREATE(CVisualStudioDemoDoc)

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CVisualStudioDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CVisualStudioDemoDoc)
	afx_msg void OnDummySelectActiveConfiguration();
	afx_msg void OnDummyBuild();
	afx_msg void OnDummyCompile();
	afx_msg void OnDummyExecute();
	afx_msg void OnDummyGo();
	afx_msg void OnDummyRebuildAll();
	afx_msg void OnDummyRemoveAllBreakpoints();
	afx_msg void OnDummyClean();
	afx_msg void OnNewDialog();
	afx_msg void OnNewAccelerator();
	afx_msg void OnNewBitmap();
	afx_msg void OnNewCursor();
	afx_msg void OnNewIcon();
	afx_msg void OnNewMenu();
	afx_msg void OnNewStringTable();
	afx_msg void OnNewToolbar();
	afx_msg void OnNewVersion();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
