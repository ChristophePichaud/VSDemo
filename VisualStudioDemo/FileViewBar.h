#pragma once

#include "ClassTreeWnd.h"
#include "FileManager.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileViewBar : public CDockablePane
{
// Construction
public:
	CFileViewBar();

	void AdjustLayout();
	void OnChangeVisualStyle();
	void SetManager(std::shared_ptr<CFileManager> pManager);
	void UpdateSolution(std::shared_ptr<CCodeFile> cf);
	void UpdateSolution(std::shared_ptr<CAssemblyFile> af);

public:
	std::shared_ptr<CFileManager> m_pManager;
	std::shared_ptr<CFileManager> GetManager() const
	{
		return m_pManager;
	}

public:
	HTREEITEM _hRootProject;
	HTREEITEM _hSrc;
	HTREEITEM _hReferences;

public:
	CClassTreeWnd m_wndFileView;

// Attributes
protected:
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

public:
	void FillFileView();

// Implementation
public:
	virtual ~CFileViewBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnSolutionOpen();
	afx_msg void OnSolutionOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnAddFileToProject();
	afx_msg void OnRemoveFileFromProject();
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

