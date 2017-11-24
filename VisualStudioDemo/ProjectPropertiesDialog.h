#pragma once
#include "afxwin.h"


// CProjectPropertiesDialog dialog

class CProjectPropertiesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectPropertiesDialog)

public:
	CProjectPropertiesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProjectPropertiesDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowse();
	CString m_strFilePath;
	virtual BOOL OnInitDialog();
	CString m_strWorkingDir;
	afx_msg void OnBnClickedProjectBrowse();
	afx_msg void OnBnClickedOk();
	CString m_strSolutionName;
	CComboBox m_cbConfiguration;
	CComboBox m_cbPlatform;
	BOOL m_bEmitDebugInformation;
	CComboBox m_cbTarget;
};
