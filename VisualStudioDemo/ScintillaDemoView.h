#pragma once
#include "ScintillaCtrl.h"
#include "ScintillaDocView.h"

class CFileManager;
class CScintillaDemoDoc;
class CScintillaCtrl;
class CScintillaView;


class CScintillaDemoCtrl : public CScintillaCtrl
{
public:
};

class CScintillaDemoView : public CScintillaView
{
protected: //create from serialization only
  CScintillaDemoView();
  DECLARE_DYNCREATE(CScintillaDemoView)

//Attributes
public:
	CScintillaDemoDoc * GetDocument() const;
	std::shared_ptr<CFileManager> GetManager() const;

  virtual void OnDraw(CDC* pDC);  //overridden to draw this view
  virtual void OnInitialUpdate();

protected:
  afx_msg void OnOptionsAddmarker();
  afx_msg void OnOptionsDeletemarker();
  afx_msg void OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI);
  afx_msg void OnOptionsFindNextmarker();
  afx_msg void OnOptionsFindPrevmarker();
  afx_msg void OnOptionsFoldMargin();
  afx_msg void OnOptionsSelectionMargin();
  afx_msg void OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI);
  afx_msg void OnOptionsViewLinenumbers();
  afx_msg void OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOptionsAddmarker(CCmdUI* pCmdUI);
  afx_msg void OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI);
  afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
  afx_msg void OnUpdateStyle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFold(CCmdUI* pCmdUI);
  afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

  DECLARE_MESSAGE_MAP()
    
  virtual void OnCharAdded(_Inout_ SCNotification* pSCNotification);
  virtual void OnDwellStart(_Inout_ SCNotification* pSCNotification);
  virtual void OnDwellEnd(_Inout_ SCNotification* pSCNotification);
  virtual void OnModifyAttemptRO(_Inout_ SCNotification* pSCNotification);
  virtual void OnModified(_Inout_ SCNotification* pSCNotification);
  virtual std::unique_ptr<CScintillaCtrl> CreateControl();

  void SetAStyle(int style, COLORREF fore, COLORREF back=RGB(0xff, 0xff, 0xff), int size=-1, const char *face=0);
  void DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back);
public:
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
//	afx_msg void OnUpdateEditReplace(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);
};


