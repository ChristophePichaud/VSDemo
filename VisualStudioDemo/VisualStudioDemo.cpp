#include "stdafx.h"
#include "VisualStudioDemo.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "VisualStudioDemoDoc.h"
#include "VisualStudioDemoView.h"
#include "AppLookDlg.h"
#include "ScintillaDocView.h"
#include "ScintillaDemoDoc.h"
#include "ScintillaDemoView.h"
#include "SourceCodeView.h"
#include "SplashWnd.h"
#include "Version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisualStudioDemoApp

BEGIN_MESSAGE_MAP(CVisualStudioDemoApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_SHOW_START, OnHelpShowStart)
	ON_COMMAND(ID_VIEW_APP_LOOK, OnViewAppLook)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualStudioDemoApp construction

CVisualStudioDemoApp::CVisualStudioDemoApp()
{
	m_pStartDocTemplate = NULL;
	m_pDocTemplateCpp = NULL;

	m_bHiColorIcons = FALSE;
}

CVisualStudioDemoApp::~CVisualStudioDemoApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVisualStudioDemoApp object

CVisualStudioDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVisualStudioDemoApp initialization

BOOL CVisualStudioDemoApp::InitInstance()
{
	USES_CONVERSION;

#ifdef USE_SCINTILLA
	//Load the scintilla dll
	m_hSciDLL = LoadLibrary(_T("SciLexer.dll"));
	if (m_hSciDLL == NULL)
	{
		AfxMessageBox(_T("Scintilla DLL is not installed, Please download the SciTE editor and copy the SciLexer.dll into this application's directory"));
		return FALSE;
	}
#endif
	
	// CG: The following block was added by the Splash Screen component.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CSplashWnd::EnableSplashScreen();

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


	// The File Manager
	m_pManager = std::make_shared<CFileManager>();

	// Settings
	DWORD dwCount = sizeof(TCHAR) * 255;
	TCHAR lpszCurrentDir[255];
	::GetCurrentDirectory(dwCount, lpszCurrentDir);

	CString strCSC;
	strCSC.Format(_T("%s\\Bin\\tools\\CSC.exe"), lpszCurrentDir);
	GetManager()->m_pSolution->_settings._compilerFilePath = T2W((LPTSTR)(LPCTSTR)strCSC);

	//AfxMessageBox(strCSC);

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Microsoft\\MFC\\Samples"));

	LoadStdProfileSettings();  // Load standard INI file options(including MRU)

	SetRegistryBase(_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();

	EnableUserTools(ID_TOOLS_ENTRY, ID_USER_TOOL1, ID_USER_TOOL10, RUNTIME_CLASS(CUserTool), IDR_MENU_ARGS, IDR_MENU_DIRS);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pDocTemplateCpp = new CMultiDocTemplate(IDR_DEVTYPE_CPP, 
								RUNTIME_CLASS(CScintillaDemoDoc), 
									RUNTIME_CLASS(CChildFrame), 
								RUNTIME_CLASS(CScintillaDemoView));

	AddDocTemplate(m_pDocTemplateCpp);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;

	EnableLoadWindowPlacement(FALSE);

	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->DragAcceptFiles();

	// The main window has been initialized, so show and update it.
	if (!ReloadWindowPlacement(pMainFrame))
	{
		pMainFrame->ShowWindow(m_nCmdShow);
		pMainFrame->UpdateWindow();
	}

	/*CAppLookDlg dlg(TRUE, m_pMainWnd);
	dlg.DoModal();*/

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strVersion;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strVersion(_T(""))
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_ABOUT_VERSION, m_strVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CVisualStudioDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// Customization load/save methods

void CVisualStudioDemoApp::PreLoadState()
{
	GetContextMenuManager()->AddMenu(_T("Edit Context menu"), IDR_CONTEXT_MENU);
	GetContextMenuManager()->AddMenu(_T("Resource"), IDR_POPUP_RESOURCE);
	GetContextMenuManager()->AddMenu(_T("Solution Explorer"), IDR_POPUP_SOLUTION);
}

void CVisualStudioDemoApp::LoadCustomState()
{
}

void CVisualStudioDemoApp::SaveCustomState()
{
}

/////////////////////////////////////////////////////////////////////////////
// CVisualStudioDemoApp message handlers

int CVisualStudioDemoApp::ExitInstance()
{
#ifdef USE_SCINTILLA	
	//Free up the Scintilla DLL
	if (m_hSciDLL)
		FreeLibrary(m_hSciDLL);
#endif

	return CWinAppEx::ExitInstance();
}

void CVisualStudioDemoApp::OnHelpShowStart()
{
	ASSERT_VALID(m_pStartDocTemplate);

	POSITION pos = m_pStartDocTemplate->GetFirstDocPosition();
	if (pos == NULL)
	{
		m_pStartDocTemplate->OpenDocumentFile(NULL);
		return;
	}

	CDocument* pDoc = m_pStartDocTemplate->GetNextDoc(pos);
	ASSERT_VALID(pDoc);

	pos = pDoc->GetFirstViewPosition();
	ASSERT(pos != NULL);

	CView* pView = pDoc->GetNextView(pos);
	ASSERT_VALID(pView);

	CFrameWnd* pFrame = pView->GetParentFrame();
	ASSERT_VALID(pFrame);

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);

	::SendMessage(pMainFrame->m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM) pFrame->GetSafeHwnd(), 0);
}

void CVisualStudioDemoApp::OnViewAppLook()
{
	CAppLookDlg dlg(FALSE, m_pMainWnd);
	dlg.DoModal();
}

BOOL CVisualStudioDemoApp::SaveAllModified()
{
	if (!CWinAppEx::SaveAllModified())
	{
		return FALSE;
	}

	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, m_pMainWnd);
	if (pMainFrame != NULL)
	{
		pMainFrame->SaveMDIState(GetRegSectionPath());
	}

	return TRUE;
}



BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	UpdateData(TRUE);

	m_strVersion.Format(_T("Version: %s"), ApplicationVersion.c_str());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
