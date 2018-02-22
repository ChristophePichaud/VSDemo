// ProjectReferencesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "VisualStudioDemo.h"
#include "ProjectReferencesDialog.h"
#include "afxdialogex.h"
#include "VSItems.h"
#include "AssemblyHelper.h"

// CProjectReferencesDialog dialog

IMPLEMENT_DYNAMIC(CProjectReferencesDialog, CDialogEx)

CProjectReferencesDialog::CProjectReferencesDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROJECTREFERENCESDIALOG, pParent)
{

}

CProjectReferencesDialog::~CProjectReferencesDialog()
{
}

void CProjectReferencesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ASM, m_lstAssemblies);
}


BEGIN_MESSAGE_MAP(CProjectReferencesDialog, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CProjectReferencesDialog::OnClickedFramework)
	ON_BN_CLICKED(IDC_RADIO2, &CProjectReferencesDialog::OnClickedGAC)
	ON_BN_CLICKED(IDC_FILL, &CProjectReferencesDialog::OnClickedFill)
	ON_BN_CLICKED(IDOK, &CProjectReferencesDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BROWSE_ASSEMBLY, &CProjectReferencesDialog::OnClickedBrowseAssembly)
END_MESSAGE_MAP()


// CProjectReferencesDialog message handlers


void CProjectReferencesDialog::OnClickedFramework()
{
	// TODO: Add your control notification handler code here
	m_bAssembliesInFramework = true;
}


void CProjectReferencesDialog::OnClickedGAC()
{
	// TODO: Add your command handler code here
	m_bAssembliesInFramework = false;
}

void CProjectReferencesDialog::OnClickedFill()
{
	USES_CONVERSION;

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// TODO: Add your control notification handler code here
	m_lstAssemblies.ResetContent();

	if (m_bAssembliesInFramework)
	{
		m_vData = CAssemblyHelper::FillAssembliesFromDir(CAssemblyHelper::GetFrameworkPath());
	}
	else
	{
		m_vData = CAssemblyHelper::FillAssembliesFromDir(CAssemblyHelper::GetGACPath());
	}

	for (auto element : m_vData)
	{
		LPTSTR lpsz = W2T((LPTSTR)element->_name.c_str());
		int index = m_lstAssemblies.AddString(lpsz);
		m_lstAssemblies.SetItemData(index, (DWORD_PTR)(element.get()));
	}
}

void CProjectReferencesDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

	// Get the indexes of all the selected items.
	int nCount = m_lstAssemblies.GetSelCount();
	CArray<int, int> ar;

	ar.SetSize(nCount);
	int count = m_lstAssemblies.GetSelItems(nCount, ar.GetData());

	// Dump the selection array.
	for (int c = 0; c < count; ++c)
	{
		int index = ar[c];
		CAssemblyFile *pAsm = (CAssemblyFile *)m_lstAssemblies.GetItemData(index);
		m_vDataSelected.push_back(pAsm);
	}
}


void CProjectReferencesDialog::OnClickedBrowseAssembly()
{
	USES_CONVERSION;
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString strFileName = dlg.GetFileName();
	CString strPath = dlg.GetFolderPath();
	CString strFile;
	strFile.Format(_T("%s\\%s"), strPath, strFileName);

	std::shared_ptr<CAssemblyFile> pAsm = std::make_shared<CAssemblyFile>();
	pAsm->_name = T2W((LPTSTR)(LPCTSTR)strFileName);
	pAsm->_path = T2W((LPTSTR)(LPCTSTR)strFile);

	CVisualStudioDemoApp * pApp = (CVisualStudioDemoApp *)AfxGetApp();
	pApp->GetManager()->m_pSolution->AddReferenceToProject(pAsm);
	pApp->GetManager()->UpdateSolution(pAsm);
}
