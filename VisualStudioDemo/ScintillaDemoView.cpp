#include "stdafx.h"
#include "VisualStudioDemo.h"
#include "ScintillaDemoDoc.h"
#include "ScintillaDemoView.h"
#include "FileManager.h"
#include "VSItems.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const TCHAR cppKeyWords[] = 
  _T("and and_eq asm auto bitand bitor bool break ")
  _T("case catch char class compl const const_cast continue ")
  _T("default delete do double dynamic_cast else enum explicit export extern false float for ")
  _T("friend goto if inline int long mutable namespace new not not_eq ")
  _T("operator or or_eq private protected public ")
  _T("register reinterpret_cast return short signed sizeof static static_cast struct switch ")
  _T("template this throw true try typedef typeid typename union unsigned using ")
  _T("virtual void volatile wchar_t while xor xor_eq ");


IMPLEMENT_DYNCREATE(CScintillaDemoView, CScintillaView)

BEGIN_MESSAGE_MAP(CScintillaDemoView, CScintillaView)
  /*
  ON_COMMAND(ID_OPTIONS_ADDMARKER, &CScintillaDemoView::OnOptionsAddmarker)
  ON_COMMAND(ID_OPTIONS_DELETEMARKER, &CScintillaDemoView::OnOptionsDeletemarker)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DELETEMARKER, &CScintillaDemoView::OnUpdateOptionsDeletemarker)
  ON_COMMAND(ID_OPTIONS_FIND_NEXTMARKER, &CScintillaDemoView::OnOptionsFindNextmarker)
  ON_COMMAND(ID_OPTIONS_FIND_PREVMARKER, &CScintillaDemoView::OnOptionsFindPrevmarker)
  ON_COMMAND(ID_OPTIONS_FOLD_MARGIN, &CScintillaDemoView::OnOptionsFoldMargin)
  ON_COMMAND(ID_OPTIONS_SELECTION_MARGIN, &CScintillaDemoView::OnOptionsSelectionMargin)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_SELECTION_MARGIN, &CScintillaDemoView::OnUpdateOptionsSelectionMargin)
  ON_COMMAND(ID_OPTIONS_VIEW_LINENUMBERS, &CScintillaDemoView::OnOptionsViewLinenumbers)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_VIEW_LINENUMBERS, &CScintillaDemoView::OnUpdateOptionsViewLinenumbers)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_ADDMARKER, &CScintillaDemoView::OnUpdateOptionsAddmarker)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_FOLD_MARGIN, &CScintillaDemoView::OnUpdateOptionsFoldMargin)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, &CScintillaDemoView::OnUpdateLine) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_STYLE, &CScintillaDemoView::OnUpdateStyle) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_FOLD, &CScintillaDemoView::OnUpdateFold) 
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, &CScintillaDemoView::OnUpdateInsert)
  */
  ON_WM_ACTIVATE()
	ON_COMMAND(ID_EDIT_FIND, &CScintillaDemoView::OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, &CScintillaDemoView::OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, &CScintillaDemoView::OnUpdateEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, &CScintillaDemoView::OnUpdateEditFind)
END_MESSAGE_MAP()


CScintillaDemoView::CScintillaDemoView()
{
  LoadMarginSettings();
}

void CScintillaDemoView::OnDraw(CDC* /*pDC*/)
{
  CScintillaDemoDoc * pDoc = (CScintillaDemoDoc*) GetDocument();
  ASSERT_VALID(pDoc);
}

CScintillaDemoDoc * CScintillaDemoView::GetDocument() const
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScintillaDemoDoc)));
  return static_cast<CScintillaDemoDoc*>(m_pDocument);
}

void CScintillaDemoView::SetAStyle(int style, COLORREF fore, COLORREF back, int size, const char* face) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

  rCtrl.StyleSetFore(style, fore);
  rCtrl.StyleSetBack(style, back);
  if (size >= 1)
    rCtrl.StyleSetSize(style, size);
  if (face) 
    rCtrl.StyleSetFont(style, face);
}

void CScintillaDemoView::DefineMarker(int marker, int markerType, COLORREF fore, COLORREF back) 
{
  CScintillaCtrl& rCtrl = GetCtrl();

  rCtrl.MarkerDefine(marker, markerType);
  rCtrl.MarkerSetFore(marker, fore);
  rCtrl.MarkerSetBack(marker, back);
}

void CScintillaDemoView::OnInitialUpdate() 
{
  //Let the base class do its thing
  CScintillaView::OnInitialUpdate();
  
  CScintillaCtrl& rCtrl = GetCtrl();

  //Setup the Lexer
  rCtrl.SetLexer(SCLEX_CPP);
  rCtrl.SetKeyWords(0, cppKeyWords);

  //Setup styles
  SetAStyle(STYLE_DEFAULT, RGB(0, 0, 0), RGB(0xff, 0xff, 0xff), 10, "Consolas"); //8, "Verdana");
  rCtrl.StyleClearAll();
  SetAStyle(SCE_C_DEFAULT, RGB(0, 0, 0));
  SetAStyle(SCE_C_COMMENT, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINE, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTLINEDOC, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORD, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_COMMENTDOCKEYWORDERROR, RGB(0, 0x80, 0));
  SetAStyle(SCE_C_NUMBER, RGB(0, 0x80, 0x80));
  SetAStyle(SCE_C_WORD, RGB(0, 0, 0x80));
  //rCtrl.StyleSetBold(SCE_C_WORD, 1);
  SetAStyle(SCE_C_STRING, RGB(163, 21, 21)); //RGB(0x80, 0, 0x80));
  SetAStyle(SCE_C_IDENTIFIER, RGB(0, 0, 0));
  SetAStyle(SCE_C_PREPROCESSOR, RGB(0x80, 0, 0));
  SetAStyle(SCE_C_OPERATOR, RGB(0x80, 0x80, 0));

  //Setup folding
  rCtrl.SetMarginWidthN(2, 16);
  rCtrl.SetMarginSensitiveN(2, TRUE);
  rCtrl.SetMarginTypeN(2, SC_MARGIN_SYMBOL);
  rCtrl.SetMarginMaskN(2, SC_MASK_FOLDERS);
  rCtrl.SetProperty(_T("fold"), _T("1"));

  //Setup markers:;
  DefineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0xFF));
  DefineMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));
  DefineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, RGB(0xff, 0xff, 0xff), RGB(0, 0, 0));

  //Setup auto completion
  rCtrl.AutoCSetSeparator(10); //Use a separator of line feed

  //Setup call tips
  rCtrl.SetMouseDwellTime(1000);

  //Enable Multiple selection
  rCtrl.SetMultipleSelection(TRUE);

#ifdef _DEBUG
  AfxDump(this);
#endif
}

std::shared_ptr<CFileManager> CScintillaDemoView::GetManager() const
{
	return GetDocument()->GetManager();
}

void CScintillaDemoView::OnOptionsAddmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  rCtrl.MarkerAdd(nLine, 0);
}

void CScintillaDemoView::OnUpdateOptionsAddmarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable((nBits & 0x1) == 0);	
}

void CScintillaDemoView::OnOptionsDeletemarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  rCtrl.MarkerDelete(nLine, 0);
}

void CScintillaDemoView::OnUpdateOptionsDeletemarker(CCmdUI* pCmdUI) 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nBits = rCtrl.MarkerGet(nLine);
  pCmdUI->Enable(nBits & 0x1);	
}

void CScintillaDemoView::OnOptionsFindNextmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nFoundLine = rCtrl.MarkerNext(nLine + 1, 0x1);
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void CScintillaDemoView::OnOptionsFindPrevmarker() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nFoundLine = rCtrl.MarkerPrevious(nLine - 1, 0x1);
  if (nFoundLine >= 0)
    rCtrl.GotoLine(nFoundLine);
  else
    MessageBeep(MB_ICONHAND);
}

void CScintillaDemoView::OnOptionsFoldMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nMarginWidth = rCtrl.GetMarginWidthN(2);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(2, 0);
  else
    rCtrl.SetMarginWidthN(2, 16);
}

void CScintillaDemoView::OnUpdateOptionsFoldMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(2) != 0);
}

void CScintillaDemoView::OnOptionsSelectionMargin() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nMarginWidth = rCtrl.GetMarginWidthN(1);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(1, 0);
  else
    rCtrl.SetMarginWidthN(1, 16);
}

void CScintillaDemoView::OnUpdateOptionsSelectionMargin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(1) != 0);
}

void CScintillaDemoView::OnOptionsViewLinenumbers() 
{
  CScintillaCtrl& rCtrl = GetCtrl();
  int nMarginWidth = rCtrl.GetMarginWidthN(0);
  if (nMarginWidth)
    rCtrl.SetMarginWidthN(0, 0);
  else
    rCtrl.SetMarginWidthN(0, 32);
}

void CScintillaDemoView::OnUpdateOptionsViewLinenumbers(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(GetCtrl().GetMarginWidthN(0) != 0);
}

void CScintillaDemoView::OnUpdateInsert(CCmdUI* pCmdUI)
{
  CString sText;
  if (GetCtrl().GetOvertype())
    VERIFY(sText.LoadString(ID_INDICATOR_OVR));
  pCmdUI->SetText(sText);
}

void CScintillaDemoView::OnUpdateStyle(CCmdUI* pCmdUI)
{
   CScintillaCtrl& rCtrl = GetCtrl();
   Sci_Position nPos = rCtrl.GetCurrentPos();
   int nStyle = rCtrl.GetStyleAt(nPos);
   CString sLine;
   //sLine.Format(IDS_STYLE_INDICATOR, nStyle);
   pCmdUI->SetText(sLine);
} 

void CScintillaDemoView::OnUpdateFold(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nLevel = rCtrl.GetFoldLevel(nLine) & SC_FOLDLEVELNUMBERMASK;
  nLevel -= 1024;

  CString sLine;
  //sLine.Format(IDS_FOLD_INDICATOR, nLevel);
  pCmdUI->SetText(sLine);
} 

void CScintillaDemoView::OnUpdateLine(CCmdUI* pCmdUI)
{
  CScintillaCtrl& rCtrl = GetCtrl();
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  int nColumn = rCtrl.GetColumn(nPos);

  CString sLine;
  //sLine.Format(IDS_LINE_INDICATOR, nLine, nColumn, nPos);
  pCmdUI->SetText(sLine);
} 

//Some simple examples of implementing auto completion
void CScintillaDemoView::OnCharAdded(_Inout_ SCNotification* /*pSCNotification*/)
{
  CScintillaCtrl& rCtrl = GetCtrl();

  //Get the previous 13 characters and if it is "scintilla is " case insensitive
  //then display a list which allows "very cool", "easy" or the "way cool!!"
  Sci_Position nStartSel = rCtrl.GetSelectionStart();
  Sci_Position nEndSel = rCtrl.GetSelectionEnd();
  if ((nStartSel == nEndSel) && (nStartSel >= 13))
  {
    Sci_TextRange tr;
    tr.chrg.cpMin = static_cast<Sci_PositionCR>(nStartSel - 13);
    tr.chrg.cpMax = static_cast<Sci_PositionCR>(nEndSel);
    char sText[14];
    sText[0] = '\0';
    tr.lpstrText = sText;
    rCtrl.GetTextRange(&tr);

    //Display the auto completion list
    if (_strcmpi(sText, "scintilla is ") == 0)
    {
      //Display the auto completion list
      //rCtrl.AutoCSetOrder(SC_ORDER_PERFORMSORT);
      //int nOrder = rCtrl.AutoCGetOrder();
      rCtrl.AutoCShow(0, _T("very cool\neasy to use\nway cool!!"));
    }
  }
  
#ifdef _DEBUG
  //Test out some of the methods
  /*
  Sci_Position nPos = rCtrl.GetCurrentPos();
  int nStyle = rCtrl.GetStyleAt(nPos);
  int nSubStyleStart = rCtrl.GetSubStylesStart(nStyle);
  int nSubStyleLength = rCtrl.GetSubStylesLength(nStyle);
  rCtrl.AllocateSubStyles(nStyle, 2);
  nSubStyleStart = rCtrl.GetSubStylesStart(nStyle);
  nSubStyleLength = rCtrl.GetSubStylesLength(nStyle);
  int nDistanceToSecondaryStyles = rCtrl.DistanceToSecondaryStyles();
  UNREFERENCED_PARAMETER(nDistanceToSecondaryStyles);
  rCtrl.FreeSubStyles();
  int nLineTypesAllowed = rCtrl.GetLineEndTypesAllowed();
  int nLineTypesSupported = rCtrl.GetLineEndTypesSupported();
  UNREFERENCED_PARAMETER(nLineTypesSupported);
  int nLineTypesActive = rCtrl.GetLineEndTypesActive();
  UNREFERENCED_PARAMETER(nLineTypesActive);
  rCtrl.SetLineEndTypesAllowed(SC_LINE_END_TYPE_UNICODE);
  nLineTypesAllowed = rCtrl.GetLineEndTypesAllowed();
  rCtrl.AllocateExtendedStyles(3);
  rCtrl.ReleaseAllExtendedStyles();
  int nAutomaticFold = rCtrl.GetAutomaticFold();
  rCtrl.SetAutomaticFold(SC_AUTOMATICFOLD_SHOW | SC_AUTOMATICFOLD_CHANGE);
  nAutomaticFold = rCtrl.GetAutomaticFold();
  nPos = rCtrl.GetCurrentPos();
  int nLine = rCtrl.LineFromPosition(nPos);
  UNREFERENCED_PARAMETER(nLine);
  rCtrl.FoldAll(SC_FOLDACTION_TOGGLE);
  rCtrl.ScrollRange(40, 100);
  CString sLine1 = rCtrl.GetCurLine();
  CString sLine2 = rCtrl.GetLine(0);
  CString sProperty = rCtrl.GetProperty(_T("fold"));
  CString sProperty2 = rCtrl.GetPropertyExpanded(_T("fold"));
  CString sTempText = rCtrl.GetText(10);
  CString sFont = rCtrl.StyleGetFont(1);
  int nProperty = rCtrl.GetPropertyInt(_T("fold"), 0);
  nProperty;
  rCtrl.AppendText(2, _T("XY"));
  rCtrl.SetTargetStart(0);
  rCtrl.SetTargetEnd(10);
  rCtrl.ReplaceTarget(2, _T("XY"));
  CString sAutoCCurrentText = rCtrl.AutoCGetCurrentText();
  CString sLexerLanguage = rCtrl.GetLexerLanguage();
  CString sPropertyNames = rCtrl.PropertyNames();
  int nPropertyType = rCtrl.PropertyType(_T("fold.at.else"));
  nPropertyType;
  CString sPropertyDescription = rCtrl.DescribeProperty(_T("fold.at.else"));
  CString sDescribeKeyWordSets = rCtrl.DescribeKeyWordSets();
  rCtrl.SetWhitespaceSize(3);
  int nWhitespaceSize = rCtrl.GetWhitespaceSize();
  nWhitespaceSize;
  rCtrl.SetFontQuality(2);
  int nFontQuality = rCtrl.GetFontQuality();
  nFontQuality;
  rCtrl.SetFirstVisibleLine(3);
  rCtrl.SetMultiPaste(2);
  int nMultiPaste = rCtrl.GetMultiPaste();
  nMultiPaste;
  rCtrl.SetCaretSticky(1);
  int nCaretSticky = rCtrl.GetCaretSticky();
  nCaretSticky;
  CString sTag = rCtrl.GetTag(0);
  const char* pszPointer = rCtrl.GetCharacterPointer();
  pszPointer;
  rCtrl.SetSelectionNStart(0, 2);
  int nSelectionNStart = rCtrl.GetSelectionNStart(0);
  nSelectionNStart;
  int nSelectionNEnd = rCtrl.GetSelectionNEnd(0);
  nSelectionNEnd;
  rCtrl.SetAdditionalCaretsVisible(FALSE);
  BOOL bAdditionCaretVisible = rCtrl.GetAdditionalCaretsVisible();
  bAdditionCaretVisible;
  int nContractedFoldNext = rCtrl.ContractedFoldNext(0);
  nContractedFoldNext;
  rCtrl.VerticalCentreCaret();
  rCtrl.ChangeLexerState(0, -1);
  int nCharacters = rCtrl.CountCharacters(0, 100);
  UNREFERENCED_PARAMETER(nCharacters);
  BOOL bTest = rCtrl.GetSelectionEmpty();
  UNREFERENCED_PARAMETER(bTest);
  int nPhasesDraw = rCtrl.GetPhasesDraw();
  rCtrl.SetPhasesDraw(nPhasesDraw);
  rCtrl.AddTabStop(0, 40);
  rCtrl.AddTabStop(0, 60);
  int nNextTabStop = rCtrl.GetNextTabStop(0, 40);
  UNREFERENCED_PARAMETER(nNextTabStop);
  rCtrl.ClearTabStops(0);
  int nIME = rCtrl.GetIMEInteraction();
  rCtrl.SetIMEInteraction(nIME);
  rCtrl.CallTipSetPosStart(100);
  Sci_Position nPosition = rCtrl.PositionRelative(0, 3);
  UNREFERENCED_PARAMETER(nPosition);
  int nMulti = rCtrl.AutoCGetMulti();
  rCtrl.AutoCSetMulti(nMulti);
  BOOL bMouseSelection = rCtrl.GetMouseSelectionRectangularSwitch();
  rCtrl.SetMouseSelectionRectangularSwitch(bMouseSelection);
  rCtrl.DropSelectionN(0);
  rCtrl.SetRepresentation("\xe2\x84\xa6", "U+2126 \xe2\x84\xa6");
  char szRepresenation[32];
  szRepresenation[0] = '\0';
  rCtrl.GetRepresentation("\xe2\x84\xa6", szRepresenation);
  rCtrl.ClearRepresentation("\xe2\x84\xa6");
  nStyle = rCtrl.GetStyleFromSubStyle(2);
  int nPrimaryStyle = rCtrl.GetPrimaryStyleFromStyle(nStyle);
  UNREFERENCED_PARAMETER(nPrimaryStyle);
  rCtrl.SetEdgeMode(EDGE_BACKGROUND);
  rCtrl.SetEdgeColour(RGB(200, 0, 0));
  rCtrl.MultiEdgeAddLine(3, RGB(0, 200, 0));
  rCtrl.MultiEdgeClearAll();
  rCtrl.SetMouseWheelCaptures(TRUE);
  BOOL bCaptures = rCtrl.GetMouseWheelCaptures();
  UNREFERENCED_PARAMETER(bCaptures);
  rCtrl.ToggleFoldShowText(28, _T("PJ"));
  rCtrl.FoldDisplayTextSetStyle(SC_FOLDDISPLAYTEXT_BOXED);
  int nMode = rCtrl.GetTabDrawMode();
  rCtrl.SetTabDrawMode(SCTD_STRIKEOUT);
  nMode = rCtrl.GetTabDrawMode();
  rCtrl.UsePopUp(SC_POPUP_TEXT);
  int nFrame = rCtrl.GetCaretLineFrame();
  rCtrl.SetCaretLineFrame(nFrame);
  */
#endif  
  
  //As another example, get the previous 2 characters and if it is "res" case sensitive
  //then display a list which allows "resize", "restart" and "restore"
  if ((nStartSel == nEndSel) && (nStartSel >= 3))
  {
    Sci_TextRange tr;
    tr.chrg.cpMin = static_cast<Sci_PositionCR>(nStartSel - 3);
    tr.chrg.cpMax = static_cast<Sci_PositionCR>(nEndSel);
    char sText[4];
    sText[0] = '\0';
    tr.lpstrText = sText;
    rCtrl.GetTextRange(&tr);

    //Display the auto completion list
    if (strcmp(sText, "res") == 0)
    {
      //Display the auto completion list
      rCtrl.AutoCShow(3, _T("resize\nrestart\nrestore"));
    }
  }

  //As another example, get the previous 16 characters and if it is "SAMPLE_INDICATOR" case insensitive
  //then display that text with a squiggly underline
  if ((nStartSel == nEndSel) && (nStartSel >= 16))
  {
    Sci_TextRange tr;
    tr.chrg.cpMin = static_cast<Sci_PositionCR>(nStartSel - 16);
    tr.chrg.cpMax = static_cast<Sci_PositionCR>(nEndSel);
    char sText[17];
    sText[0] = '\0';
    tr.lpstrText = sText;
    rCtrl.GetTextRange(&tr);

    //Display the Sample indicator
    if (_strcmpi(sText, "sample_indicator") == 0)
    {
      //For demonstration purposes lets make the "C" Comment style into hotspots which turn red
      rCtrl.SetIndicatorCurrent(0);
      rCtrl.StyleSetHotSpot(SCE_C_COMMENT, TRUE);
      rCtrl.SetHotspotActiveFore(TRUE, RGB(255, 0, 0));

      //Show the indicator
      rCtrl.IndicatorFillRange(tr.chrg.cpMin, static_cast<int>(nStartSel));
    }
  }
}

//A simple example of call tips
void CScintillaDemoView::OnDwellStart(_Inout_ SCNotification* pSCNotification)
{
  CScintillaCtrl& rCtrl = GetCtrl();

  //Only display the call tip if the scintilla window has focus
  CWnd* pFocusWnd = GetFocus();
  if (pFocusWnd)
  {
    if (pFocusWnd->GetSafeHwnd() == rCtrl.GetSafeHwnd())
    {
      //Get the previous 7 characters and next 7 characters around
      //the current dwell position and if it is "author " case insensitive
      //then display "PJ Naughter" as a call tip
      Sci_TextRange tr;
      tr.chrg.cpMin = static_cast<Sci_PositionCR>(max(0, pSCNotification->position - 7));
      tr.chrg.cpMax = static_cast<Sci_PositionCR>(min(pSCNotification->position + 7, rCtrl.GetLength()));
      char sText[15];
      sText[0] = '\0';
      tr.lpstrText = sText;
      rCtrl.GetTextRange(&tr);

      //Display the call tip
      _strupr_s(sText, sizeof(sText));
      if (strstr(sText, "AUTHOR "))
        rCtrl.CallTipShow(pSCNotification->position, _T("PJ Naughter"));
    }
  }
}

void CScintillaDemoView::OnDwellEnd(_Inout_ SCNotification* /*pSCNotification*/)
{
  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
  if (rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void CScintillaDemoView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
  //Let the base class do its thing
  CScintillaView::OnActivate(nState, pWndOther, bMinimized);

  CScintillaCtrl& rCtrl = GetCtrl();

  //Cancel any outstanding call tip
  if (nState == WA_INACTIVE && rCtrl.CallTipActive())
    rCtrl.CallTipCancel();
}

void CScintillaDemoView::OnModifyAttemptRO(_Inout_ SCNotification* /*pSCNotification*/)
{
  //if (AfxMessageBox(IDP_ALLOW_MODIFY_READONLY_FILE, MB_YESNO) == IDYES)
    GetCtrl().SetReadOnly(FALSE);
}

void CScintillaDemoView::OnModified(_Inout_ SCNotification* pSCNotification)
{
  if (pSCNotification->modificationType & SC_MOD_INSERTCHECK)
  {
  #ifdef _DEBUG
    //Just some demo code to test out SCI_CHANGEINSERTION
    CScintillaCtrl& rCtrl = GetCtrl();
    if (strcmp(pSCNotification->text, "Dublin") == 0)
      rCtrl.ChangeInsertion(18, _T("Capital of Ireland"));
  #endif
  }
}

std::unique_ptr<CScintillaCtrl> CScintillaDemoView::CreateControl()
{
#if _MSC_VER >= 1800
  return std::make_unique<CScintillaDemoCtrl>();
#else
  return std::unique_ptr<CScintillaCtrl>(new CScintillaDemoCtrl());
#endif
}

void CScintillaDemoView::OnEditFind()
{
	// TODO: Add your command handler code here
	CScintillaView::OnEditFindReplace(TRUE);
}


void CScintillaDemoView::OnEditReplace()
{
	// TODO: Add your command handler code here
	CScintillaView::OnEditFindReplace(FALSE);
}

void CScintillaDemoView::OnUpdateEditFind(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(GetCtrl().GetTextLength() != 0);
}
