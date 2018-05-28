// GaSchedule.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GaSchedule.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW//MFC는 디버그 모드시에 모든 NEW연산자를 DEBUG_NEW연산자로 변경시킨다
#endif


// CGaScheduleApp

BEGIN_MESSAGE_MAP(CGaScheduleApp, CWinApp)//대부분의 event handle이 여기서 처리된다
	ON_COMMAND(ID_APP_ABOUT, &CGaScheduleApp::OnAppAbout)//표준파일을 기초로 하는 문서 명령//ID_APP_ABOUT이란 팝업 메뉴를 선택하면 opAppAbout이라는 함수를 실행하라는 의미
END_MESSAGE_MAP()//메시지 맵의 마지막임을 나타내기 위함
//명령-대상 개체가 지정된 ID의 WindowsWM_COMMAND메시지를 받으면 메시지를 처리하기 위해 멤버 함수 member.n을 호출합니다.



// CGaScheduleApp construction

CGaScheduleApp::CGaScheduleApp()
{
}


// The one and only CGaScheduleApp object

CGaScheduleApp theApp;


// CGaScheduleApp initialization
/*
INITCOMMONCONTROLSEX InitCtrls;//이 함수는 공통 컨트롤을 제공하는 DLL인 COMCTL32.DLL이 제대로 로드되었는지를 체크하고 만약 로드되어 있지 않다면 로드한다
이 함수는 인수를 취하지 않으며 리턴값도 없으므로 메인 윈도우의 프로그램 선두에서 이 함수를 한 번 호출하기만 하면 된다
*/
BOOL CGaScheduleApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);// 이 구조체의 크기를 대입
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;//초기화하고자 하는 컨트롤의 종류를 지정하는 플래그를 준다
	//ICC_WIN95_CLASSES: 확장 콤보 박스// 애니메이트, 헤더, 핫키, 리스트 뷰,..
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("Coolsoft\\GaClassSchedule"));//key값 지정
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	/*
	CMainFrame 클래스는 메인 프레임 윈도우를 관리하는 클래스이다
	다중 문서 인터페이스 프로그램에서 메인 프레임 윈도우는 프로그램 전체를 감싸는 틀의 역할을 한다
	일반적으로 프로그램의 메뉴와 툴바, 상태바, 최소화/최대화/닫기 버튼 등을 가지고 있다
	*/
	if (!pFrame)//pFrame없으면 false리턴
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


// CGaScheduleApp message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CGaScheduleApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
/*이 함수에 의해 대화상자가 사용자에게 보여진다
aboutDlg.DoModa();의 의미는 프로그램에서 about 대화상자를 열었을 경우 이 대화상자를 종료하기 전까지는 
About대화상자를 벗어나 이 프로그램내의 다른 명령을 내릴 수 없다는 뜻이다.
*/


// CGaScheduleApp message handlers

