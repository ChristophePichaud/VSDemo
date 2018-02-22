// ProjectProperties.cpp : implementation file
//

#include "stdafx.h"
#include "VisualStudioDemo.h"
#include "ProjectPropertiesDialog.h"
#include "afxdialogex.h"
#include "VSItems.h"

// CProjectPropertiesDialog dialog

IMPLEMENT_DYNAMIC(CProjectPropertiesDialog, CDialogEx)

CProjectPropertiesDialog::CProjectPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROPERTIES, pParent)
	, m_strFilePath(_T(""))
	, m_strWorkingDir(_T(""))
	, m_strSolutionName(_T(""))
	, m_bEmitDebugInformation(FALSE)
{

}

CProjectPropertiesDialog::~CProjectPropertiesDialog()
{
}

void CProjectPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFilePath);
	DDX_Text(pDX, IDC_EDIT4, m_strWorkingDir);
	DDX_Text(pDX, IDC_EDIT5, m_strSolutionName);
	DDX_Control(pDX, IDC_COMBO1, m_cbConfiguration);
	DDX_Control(pDX, IDC_COMBO2, m_cbPlatform);
	DDX_Check(pDX, IDC_CHECK_EMIT_DEBUG_INFORMATION, m_bEmitDebugInformation);
	DDX_Control(pDX, IDC_COMBO3, m_cbTarget);
}


BEGIN_MESSAGE_MAP(CProjectPropertiesDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BROWSE, &CProjectPropertiesDialog::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_PROJECT_BROWSE, &CProjectPropertiesDialog::OnBnClickedProjectBrowse)
	ON_BN_CLICKED(IDOK, &CProjectPropertiesDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CProjectPropertiesDialog message handlers

BOOL CProjectPropertiesDialog::OnInitDialog()
{
	USES_CONVERSION;

	CDialogEx::OnInitDialog();
	UpdateData(FALSE);
	UpdateData(TRUE);

	// TODO:  Add extra initialization here
	CVisualStudioDemoApp * pApp = (CVisualStudioDemoApp *)AfxGetApp();
	m_strFilePath = pApp->GetManager()->m_pSolution->_settings._compilerFilePath.c_str();
	m_strWorkingDir = pApp->GetManager()->m_pSolution->_properties._workingDirectory.c_str();
	m_strSolutionName = pApp->GetManager()->m_pSolution->_name.c_str();
	CString strConfiguration = W2T((LPTSTR) pApp->GetManager()->m_pSolution->_properties._configuration.c_str());
	CString strPlatform = W2T((LPTSTR) pApp->GetManager()->m_pSolution->_properties._platform.c_str());
	CString strTarget = W2T((LPTSTR) pApp->GetManager()->m_pSolution->_properties._target.c_str());

	LPCTSTR lpsz = W2T((LPTSTR)(LPCTSTR)CConfiguration::debug.c_str());
	m_cbConfiguration.AddString(lpsz);
	lpsz = W2T((LPTSTR)(LPCTSTR)CConfiguration::release.c_str());
	m_cbConfiguration.AddString(lpsz);
	m_cbConfiguration.SetCurSel(0);
	int index = m_cbConfiguration.FindStringExact(-1, strConfiguration);
	m_cbConfiguration.SetCurSel(index);

	lpsz = W2T((LPTSTR)(LPCTSTR)CPlatform::x86.c_str());
	m_cbPlatform.AddString(lpsz);
	lpsz = W2T((LPTSTR)(LPCTSTR)CPlatform::x64.c_str());
	m_cbPlatform.AddString(lpsz);
	lpsz = W2T((LPTSTR)(LPCTSTR)CPlatform::anycpu.c_str());
	m_cbPlatform.AddString(lpsz);
	m_cbPlatform.SetCurSel(0);
	index = m_cbPlatform.FindStringExact(-1, strPlatform);
	m_cbPlatform.SetCurSel(index);

	lpsz = W2T((LPTSTR)(LPCTSTR)CTarget::exe.c_str());
	m_cbTarget.AddString(lpsz);
	lpsz = W2T((LPTSTR)(LPCTSTR)CTarget::library.c_str());
	m_cbTarget.AddString(lpsz);
	lpsz = W2T((LPTSTR)(LPCTSTR)CTarget::winexe.c_str());
	m_cbTarget.AddString(lpsz);
	m_cbTarget.SetCurSel(0);
	index = m_cbTarget.FindStringExact(-1, strTarget);
	m_cbTarget.SetCurSel(index);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CProjectPropertiesDialog::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString strFileName = dlg.GetFileName();
	CString strPath = dlg.GetFolderPath();
	CString strFile;
	strFile.Format(_T("%s\\%s"), strPath, strFileName);
	m_strFilePath = strFile;
	UpdateData(FALSE);
}

void CProjectPropertiesDialog::OnBnClickedProjectBrowse()
{
	// TODO: Add your control notification handler code here
	CFolderPickerDialog dlg;
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString strPath = dlg.GetFolderPath();
	m_strWorkingDir = strPath;
	UpdateData(FALSE);
}


void CProjectPropertiesDialog::OnBnClickedOk()
{
	USES_CONVERSION;

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CVisualStudioDemoApp * pApp = (CVisualStudioDemoApp *)AfxGetApp();
	pApp->GetManager()->m_pSolution->_properties._workingDirectory = T2W((LPTSTR)(LPCTSTR)m_strWorkingDir);
	pApp->GetManager()->m_pSolution->_settings._compilerFilePath = T2W((LPTSTR)(LPCTSTR)m_strFilePath);
	pApp->GetManager()->m_pSolution->_name = T2W((LPTSTR)(LPCTSTR)m_strSolutionName);

	int index = m_cbConfiguration.GetCurSel();
	CString strConfiguration;
	m_cbConfiguration.GetLBText(index, strConfiguration);
	pApp->GetManager()->m_pSolution->_properties.SetConfiguration(T2W((LPTSTR)(LPCTSTR)strConfiguration));

	index = m_cbPlatform.GetCurSel();
	CString strPlatform;
	m_cbPlatform.GetLBText(index, strPlatform);
	pApp->GetManager()->m_pSolution->_properties.SetPlatform(T2W((LPTSTR)(LPCTSTR)strPlatform));

	index = m_cbTarget.GetCurSel();
	CString strTarget;
	m_cbTarget.GetLBText(index, strTarget);
	pApp->GetManager()->m_pSolution->_properties.SetTarget(T2W((LPTSTR)(LPCTSTR)strTarget));

	if (m_bEmitDebugInformation == TRUE)
		pApp->GetManager()->m_pSolution->_properties._emitDebugInformation = true;
	else
		pApp->GetManager()->m_pSolution->_properties._emitDebugInformation = false;


	/*
	AfxMessageBox(strConfiguration + _T(", ") + strPlatform + _T(", ") + strTarget);
	std::wstring s1 = pApp->GetManager()->m_pSolution->_properties._configuration;
	std::wstring s2 = pApp->GetManager()->m_pSolution->_properties._platform;
	std::wstring s3 = pApp->GetManager()->m_pSolution->_properties._target;
	*/

	CDialogEx::OnOK();
}
