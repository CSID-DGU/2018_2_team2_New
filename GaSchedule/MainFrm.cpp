// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GaSchedule.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	//ON_COMMAND(ID_BUTTON_EXCEL, &CMainFrame::OnExcelbutton)
//	ON_WM_MBUTTONDBLCLK()
	//ON_COMMAND(ID_BUTTON_EXCEL, &CMainFrame::OnExcelbutton)
	//ON_COMMAND(ID_BUTTON_EXCEL, &CMainFrame::OnCommand)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}
/////////////////////////////
class MyButton : public CButton
{
	DECLARE_DYNAMIC(MyButton)
public:
	MyButton();
	virtual ~MyButton();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
};

IMPLEMENT_DYNAMIC(MyButton, CButton)
MyButton::MyButton() {}
MyButton::~MyButton() {}

BEGIN_MESSAGE_MAP(MyButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &MyButton::OnBnClicked)
END_MESSAGE_MAP()

void MyButton::OnBnClicked()
{
	AfxMessageBox(_T("나는 바보다"));
}
////////////////////////////
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	/////////////////////////////
	Button_excel.Create("EXCEL", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(400, 200, 600, 250), this, ID_BUTTON_EXCEL);
	Button_excel.ShowWindow(SW_SHOW);
	Button_config.Create("CONFIG", WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON, CRect(400, 280, 600, 330), this, 102);
	Button_config.ShowWindow(SW_SHOW);
	
	MyButton myButton;
	myButton.Create(("dd"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(400, 360, 600, 410), this, 103);
	myButton.ShowWindow(SW_SHOW);
	/*CDC dc; CString str;
	CRect rect(200, 300, 500, 400);
	str.Format(_T("Step 1. click the EXCEL button and select class.csg file"));
	dc.DrawText(str, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/

	/////////////////////////////
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
			sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

//////////////////////////////////////////////////////////////
/*void CMainFrame::OnExcelbutton()
{
	//if (wParam == 100)
		AfxMessageBox(_T("나는 바보다"));
}*/

/*
BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 100)
		AfxMessageBox(_T("나는 바보다"));
	return CFrameWnd::OnCommand(wParam, lParam);
}

/*
afx_msg void CMainFrame::OnExcelbutton()
{
	//if (wParam == 100)
		AfxMessageBox(_T("나는 바보다"));
	//return 0;
}*/
