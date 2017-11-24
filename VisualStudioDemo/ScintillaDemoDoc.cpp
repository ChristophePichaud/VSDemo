#include "stdafx.h"
#include "VisualStudioDemo.h"
#include "ScintillaDemoDoc.h"
#include "FileNewDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CScintillaDemoDoc, CScintillaDoc)

BEGIN_MESSAGE_MAP(CScintillaDemoDoc, CScintillaDoc)
END_MESSAGE_MAP()

CScintillaDemoDoc::CScintillaDemoDoc()
{
}

CScintillaDemoDoc::~CScintillaDemoDoc()
{
}

#ifdef _DEBUG
void CScintillaDemoDoc::AssertValid() const
{
	CScintillaDoc::AssertValid();
}

void CScintillaDemoDoc::Dump(CDumpContext& dc) const
{
	CScintillaDoc::Dump(dc);
}
#endif //_DEBUG

BOOL CScintillaDemoDoc::OnNewDocument()
{
	CFileNewDialog dlg;
	if (dlg.DoModal() == IDCANCEL)
		return FALSE;

	CString strFN = dlg.m_strFileName;

	LPTSTR lpszFileName;
	lpszFileName = PathFindFileName(strFN);

	CFile file((LPCTSTR)strFN, CFile::modeCreate);
	file.Close();

	if (PathFileExists((LPCTSTR)strFN) == FALSE)
	{
		AfxMessageBox(_T("The filename is not correct !"));
		return FALSE;
	}

	this->SetModifiedFlag(FALSE);

	std::shared_ptr<CCodeFile> cf = std::make_shared<CCodeFile>();
	cf->_name = lpszFileName;
	cf->_path = strFN;

	this->SetTitle(lpszFileName);

	GetManager()->m_pSolution->AddFileToProject(cf);
	GetManager()->UpdateSolution(cf);

	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

void CScintillaDemoDoc::Serialize(CArchive& ar)
{
	CScintillaDoc::Serialize(ar);
}