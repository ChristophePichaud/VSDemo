#include "stdafx.h"
#include "VisualStudioDemo.h"
#include "mainfrm.h"
#include "FileViewBar.h"
#include "VSItems.h"
#include "ProjectPropertiesDialog.h"
#include "ProjectReferencesDialog.h"
#include "ScintillaDemoDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileViewBar

CFileViewBar::CFileViewBar()
{
}

CFileViewBar::~CFileViewBar()
{
}

BEGIN_MESSAGE_MAP(CFileViewBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_SOLUTION_OPEN, OnSolutionOpen)
	ON_COMMAND(ID_SOLUTION_OPEN_WITH, OnSolutionOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_ADDFILETOPROJECT, &CFileViewBar::OnAddFileToProject)
	ON_COMMAND(ID_REMOVEFILEFROMPROJECT, &CFileViewBar::OnRemoveFileFromProject)
//	ON_WM_LBUTTONDBLCLK()
//ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CFileViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create view:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("Failed to create solution explorer\n");
		return -1;      // fail to create
	}

	// Load view images:
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_SOLUTION_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_SOLUTION_EXPLORER, 0, 0, TRUE /* Is locked */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill view context(dummy code, don't seek here something magic :-)):
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileViewBar::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileViewBar::FillFileView()
{
	m_wndFileView.DeleteAllItems();

	_hRootProject = m_wndFileView.InsertItem(_T("Project"), 0, 0);
	m_wndFileView.SetItemState(_hRootProject, TVIS_BOLD, TVIS_BOLD);

	_hReferences = m_wndFileView.InsertItem(_T("References"), 0, 0, _hRootProject);

	_hSrc = m_wndFileView.InsertItem(_T("Source Files"), 0, 0, _hRootProject);

	m_wndFileView.Expand(_hRootProject, TVE_EXPAND);
	m_wndFileView.Expand(_hSrc, TVE_EXPAND);
}

void CFileViewBar::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Select clicked item:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_SOLUTION, point.x, point.y, this, TRUE);
}

void CFileViewBar::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileViewBar::OnProperties()
{
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	if (hItem == NULL)
		return;

	CString strText = m_wndFileView.GetItemText(hItem);
	if (strText == _T("Project"))
	{
		CProjectPropertiesDialog dlg = new CProjectPropertiesDialog();
		if (dlg.DoModal() == IDCANCEL)
			return;
	}

	if (strText == _T("References"))
	{
		CProjectReferencesDialog dlg = new CProjectReferencesDialog();
		if (dlg.DoModal() == IDCANCEL)
			return;

		for (auto element : dlg.m_vDataSelected)
		{
			std::shared_ptr<CAssemblyFile> pAsm = std::make_shared<CAssemblyFile>();
			pAsm->_name = element->_name;
			pAsm->_path = element->_path;

			CVisualStudioDemoApp * pApp = (CVisualStudioDemoApp *)AfxGetApp();
			pApp->GetManager()->m_pSolution->AddReferenceToProject(pAsm);
			pApp->GetManager()->UpdateSolution(pAsm);
		}

		m_wndFileView.Expand(_hReferences, TVE_EXPAND);
	}

}

void CFileViewBar::OnSolutionOpen()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	if (hItem == NULL)
		return;

	CCodeFile * pCode = (CCodeFile *)m_wndFileView.GetItemData(hItem);
	if (pCode == NULL)
		return;

	GetManager()->LoadFile(pCode);

	//CString str;
	//str.Format(_T("Open %s"), pCode->_path.c_str());
	//AfxMessageBox(str);
}

void CFileViewBar::OnSolutionOpenWith()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnDummyCompile()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnEditCut()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnEditCopy()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnEditClear()
{
	// TODO: Add your command handler code here
}

void CFileViewBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileViewBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileViewBar::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_SOLUTION_EXPLORER_24 : IDR_SOLUTION_EXPLORER, 0, 0, TRUE /* Locked */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}


void CFileViewBar::SetManager(std::shared_ptr<CFileManager> pManager)
{
	m_pManager = pManager;
}

void CFileViewBar::OnAddFileToProject()
{
	// TODO: Add your command handler code here
	CMainFrame * pFrame = (CMainFrame *)AfxGetMainWnd();
	// Get the active MDI child window.
	CMDIChildWnd *pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();
	if (pChild == nullptr)
		return;

	CView * pActiveView = pChild->GetActiveView();
	CDocument * pDoc = pActiveView->GetDocument();

	CString strFile = pDoc->GetTitle();
	CString strFileName = pDoc->GetPathName();

	std::shared_ptr<CCodeFile> cf = std::make_shared<CCodeFile>();
	cf->_name = strFile;
	cf->_path = strFileName;

	GetManager()->m_pSolution->AddFileToProject(cf);
	GetManager()->UpdateSolution(cf);
}

void CFileViewBar::UpdateSolution(std::shared_ptr<CCodeFile> cf)
{
	HTREEITEM hItem = m_wndFileView.InsertItem(cf->_name.c_str(), 1, 1, _hSrc);
	m_wndFileView.SetItemData(hItem, (DWORD_PTR)(cf.get()));
	m_wndFileView.Expand(_hRootProject, TVE_EXPAND);
	m_wndFileView.Expand(_hSrc, TVE_EXPAND);
}

void CFileViewBar::UpdateSolution(std::shared_ptr<CAssemblyFile> af)
{
	HTREEITEM item = m_wndFileView.InsertItem(af->_name.c_str(), 3, 3, _hReferences);
	m_wndFileView.SetItemData(item, (DWORD_PTR)(af.get()));
	m_wndFileView.Expand(_hReferences, TVE_EXPAND);
}

void CFileViewBar::OnRemoveFileFromProject()
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = m_wndFileView.GetSelectedItem();
	if (hItem == NULL)
		return;

	CCodeFile * pCode = (CCodeFile *) m_wndFileView.GetItemData(hItem);
	if (pCode == NULL)
		return;
	
	LPTSTR lpszExt = ::PathFindExtension(pCode->_name.c_str());
	CString strExt = lpszExt;
	if (strExt == _T(".dll"))
	{
		// HACK : the get item data cast returned a CAssemblyFile... qui a la même structure que CCodeFile
		CAssemblyFile * pFile = (CAssemblyFile *)pCode;
		GetManager()->m_pSolution->RemoveFileFromProject(pFile);
		
		/*CString str;
		str.Format(_T("references.size : %d"), GetManager()->m_pSolution->_project._references.size());
		AfxMessageBox(str);*/
	}
	else
	{
		GetManager()->m_pSolution->RemoveFileFromProject(pCode);
	}

	m_wndFileView.DeleteItem(hItem);
}
