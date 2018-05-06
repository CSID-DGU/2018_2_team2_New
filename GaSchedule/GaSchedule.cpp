// GaSchedule.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GaSchedule.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW//MFC�� ����� ���ÿ� ��� NEW�����ڸ� DEBUG_NEW�����ڷ� �����Ų��
#endif


// CGaScheduleApp

BEGIN_MESSAGE_MAP(CGaScheduleApp, CWinApp)//��κ��� event handle�� ���⼭ ó���ȴ�
	ON_COMMAND(ID_APP_ABOUT, &CGaScheduleApp::OnAppAbout)//ǥ�������� ���ʷ� �ϴ� ���� ���//ID_APP_ABOUT�̶� �˾� �޴��� �����ϸ� opAppAbout�̶�� �Լ��� �����϶�� �ǹ�
END_MESSAGE_MAP()//�޽��� ���� ���������� ��Ÿ���� ����
//���-��� ��ü�� ������ ID�� WindowsWM_COMMAND�޽����� ������ �޽����� ó���ϱ� ���� ��� �Լ� member.n�� ȣ���մϴ�.



// CGaScheduleApp construction

CGaScheduleApp::CGaScheduleApp()
{
}


// The one and only CGaScheduleApp object

CGaScheduleApp theApp;


// CGaScheduleApp initialization
/*
INITCOMMONCONTROLSEX InitCtrls;//�� �Լ��� ���� ��Ʈ���� �����ϴ� DLL�� COMCTL32.DLL�� ����� �ε�Ǿ������� üũ�ϰ� ���� �ε�Ǿ� ���� �ʴٸ� �ε��Ѵ�
�� �Լ��� �μ��� ������ ������ ���ϰ��� �����Ƿ� ���� �������� ���α׷� ���ο��� �� �Լ��� �� �� ȣ���ϱ⸸ �ϸ� �ȴ�
*/
BOOL CGaScheduleApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);// �� ����ü�� ũ�⸦ ����
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;//�ʱ�ȭ�ϰ��� �ϴ� ��Ʈ���� ������ �����ϴ� �÷��׸� �ش�
	//ICC_WIN95_CLASSES: Ȯ�� �޺� �ڽ�// �ִϸ���Ʈ, ���, ��Ű, ����Ʈ ��,..
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	SetRegistryKey(_T("Coolsoft\\GaClassSchedule"));//key�� ����
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	/*
	CMainFrame Ŭ������ ���� ������ �����츦 �����ϴ� Ŭ�����̴�
	���� ���� �������̽� ���α׷����� ���� ������ ������� ���α׷� ��ü�� ���δ� Ʋ�� ������ �Ѵ�
	�Ϲ������� ���α׷��� �޴��� ����, ���¹�, �ּ�ȭ/�ִ�ȭ/�ݱ� ��ư ���� ������ �ִ�
	*/
	if (!pFrame)//pFrame������ false����
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
/*�� �Լ��� ���� ��ȭ���ڰ� ����ڿ��� ��������
aboutDlg.DoModa();�� �ǹ̴� ���α׷����� about ��ȭ���ڸ� ������ ��� �� ��ȭ���ڸ� �����ϱ� �������� 
About��ȭ���ڸ� ��� �� ���α׷����� �ٸ� ����� ���� �� ���ٴ� ���̴�.
*/


// CGaScheduleApp message handlers

