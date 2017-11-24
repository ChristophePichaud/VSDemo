#pragma once

#include "ScintillaDocView.h"
#include "FileManager.h"

class CScintillaDemoDoc : public CScintillaDoc
{
protected: //create from serialization only
	CScintillaDemoDoc();
	DECLARE_DYNCREATE(CScintillaDemoDoc)

public:
	virtual ~CScintillaDemoDoc();
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Attributes
public:

	std::shared_ptr<CFileManager> GetManager() const
	{
		CVisualStudioDemoApp * pApp = (CVisualStudioDemoApp *)AfxGetApp();
		return pApp->GetManager();
	}

public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

protected:
	DECLARE_MESSAGE_MAP()
};
