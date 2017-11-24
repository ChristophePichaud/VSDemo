// FileNewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "VisualStudioDemo.h"
#include "FileNewDialog.h"
#include "afxdialogex.h"


// CFileNewDialog dialog

IMPLEMENT_DYNAMIC(CFileNewDialog, CDialogEx)

CFileNewDialog::CFileNewDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FILENEW, pParent)
	, m_strFileName(_T(""))
{

}

CFileNewDialog::~CFileNewDialog()
{
}

void CFileNewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFileName);
}


BEGIN_MESSAGE_MAP(CFileNewDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BROWSE, &CFileNewDialog::OnBnClickedBrowse)
	ON_BN_CLICKED(IDOK, &CFileNewDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFileNewDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFileNewDialog message handlers


void CFileNewDialog::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE);
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString strFileName = dlg.GetFileName();
	CString strPath = dlg.GetFolderPath();
	CString strFile;
	strFile.Format(_T("%s\\%s"), strPath, strFileName);
	//std::wstring filename = T2W((LPCTSTR)strFile);
	m_strFileName = strFile;
	UpdateData(FALSE);
}


void CFileNewDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CFileNewDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
