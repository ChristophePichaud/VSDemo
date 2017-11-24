#include "stdafx.h"
#include "OptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

IMPLEMENT_DYNAMIC(COptionsDlg, CMFCPropertySheet)

COptionsDlg::COptionsDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
:CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	SetLook(CMFCPropertySheet::PropSheetLook_Tree, 150 /* Tree control width */);
	SetIconsList(IDB_OPTIONSIMAGES, 16 /* Image width */);

	CMFCPropertySheetCategoryInfo* pCat1 = AddTreeCategory(_T("Environment"), 0, 1);

	AddPageToTree(pCat1, &m_Page11, -1, 2);
	AddPageToTree(pCat1, &m_Page12, -1, 2);

	CMFCPropertySheetCategoryInfo* pCat2 = AddTreeCategory(_T("Source Control"), 0, 1);

	AddPageToTree(pCat2, &m_Page21, -1, 2);
	AddPageToTree(pCat2, &m_Page22, -1, 2);

	CMFCPropertySheetCategoryInfo* pCat3 = AddTreeCategory(_T("Text Editor"), 0, 1);

	AddPageToTree(pCat3, &m_Page31, -1, 2);
	AddPageToTree(pCat3, &m_Page32, -1, 2);
}

COptionsDlg::~COptionsDlg()
{
}

BEGIN_MESSAGE_MAP(COptionsDlg, CMFCPropertySheet)
END_MESSAGE_MAP()

