
// ClientDlg.h : header file
//

#pragma once
#define _AFXDLL
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PORT 25000
#define WM_SOCKET WM_USER+2
#include "afxsock.h"
#include <string>
#include <vector>
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

	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	char* ConvertToChar(const CString& s);
	void mSend(CString Command);
	int mRecv(CString& Command);
	bool findSpace(string s);
	void ConnectToServer();
	vector<string> Split(CString s);

	afx_msg void OnLbnSelchangeChat();
	afx_msg void OnBnClickedLogin();
	CString IP;
	CString username;
	CString password;
	afx_msg void OnBnClickedSignup();
	CListBox lst_onluser;
	afx_msg void OnLbnDblclkOnluser();
	CString strItemSelected;
};
