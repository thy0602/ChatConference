
// ClientDlg.h : header file
//

#pragma once
#define _AFXDLL
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PORT 25000
#define WM_SOCKET WM_USER+2
#include "afxsock.h"
#include <string>
using namespace std;


// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	SOCKET sClient;
	sockaddr_in servAdd;
	CString Command;
	int flag;

	char* ConvertToChar(const CString& s);
	void mSend(CString Command);
	bool findSpace(string s);
	void ConnectToServer();

	afx_msg void OnLbnSelchangeChat();
	afx_msg void OnBnClickedLogin();
	CString IP;
	CString username;
	CString password;
	afx_msg void OnBnClickedSignup();
};
