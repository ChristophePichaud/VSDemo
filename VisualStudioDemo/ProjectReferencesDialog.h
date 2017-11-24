#pragma once
#include "afxwin.h"


// CProjectReferencesDialog dialog

class CProjectReferencesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CProjectReferencesDialog)

public:
	CProjectReferencesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProjectReferencesDialog();

public:
	bool m_bAssembliesInFramework = true;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REFERENCES };
#endif

public:
	std::vector<std::shared_ptr<CAssemblyFile>> m_vData;
	std::vector<CAssemblyFile *> m_vDataSelected;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_lstAssemblies;
	afx_msg void OnClickedFramework();
	afx_msg void OnClickedGAC();
	afx_msg void OnClickedFill();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedBrowseAssembly();
};
