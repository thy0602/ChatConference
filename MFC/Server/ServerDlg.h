
// ServerDlg.h : header file
//

#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _AFXDLL
#include "afxwin.h"
#include "stdlib.h"
#include "time.h"
#include "afxsock.h"
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#define PORT 25000
#define WM_SOCKET WM_USER+1

#define FLAG_SIGNUP 1


// CServerDlg dialog
class CServerDlg : public CDialogEx
{
// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
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
	/*SockName chứa socket và tên của socket*/
	struct SockName {
		SOCKET sockClient;
		string Name;
	};

	/*Biến*/
	//SockName* pSock;
	vector<SockName*> pSock;
	SOCKET sockServer, sockClient;
	struct sockaddr_in serverAdd;

	/*Hàm khác*/
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	int mRecv(SOCKET &sk, CString& Command);
	void mSend(SOCKET sk, CString Command);
	vector<string> Split(CString src);

	/*Hàm onClicked và biến của UI*/
	afx_msg void OnBnClickedListen();
	CListBox lst_event;
};
