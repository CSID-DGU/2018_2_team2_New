// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "ChildView.h"

class CMainFrame : public CFrameWnd
{

public:
	CMainFrame();
protected:
	DECLARE_DYNAMIC(CMainFrame)

	// Attributes
public:

	// Operations
public:

	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CChildView    m_wndView;
	/////////////////////
	CButton Button_excel;
	CButton Button_config;
	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnExcelbutton();
	//afx_msg BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
	//afx_msg void OnExcelbutton();
};


