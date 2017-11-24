#pragma once


// CFileNewDialog dialog

class CFileNewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFileNewDialog)

public:
	CFileNewDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileNewDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILENEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_strFileName;
};
