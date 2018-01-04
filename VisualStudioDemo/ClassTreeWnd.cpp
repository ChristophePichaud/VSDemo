#include "stdafx.h"
#include "VisualStudioDemo.h"
#include "ClassTreeWnd.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd

CClassTreeWnd::CClassTreeWnd()
{
}

CClassTreeWnd::~CClassTreeWnd()
{
}

BEGIN_MESSAGE_MAP(CClassTreeWnd, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CClassTreeWnd::OnNMDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassTreeWnd message handlers

BOOL CClassTreeWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


void CClassTreeWnd::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	//AfxMessageBox(_T("NM_DBLCLK"));

	HTREEITEM hItem = GetSelectedItem();
	if (hItem == NULL)
		return;

	CCodeFile * pCode = (CCodeFile *)GetItemData(hItem);
	if (pCode == NULL)
		return;

	CWnd * pWnd = AfxGetMainWnd();
	CMainFrame * pMainFrame = (CMainFrame *)pWnd;

	pMainFrame->GetManager()->LoadFile(pCode);
}
