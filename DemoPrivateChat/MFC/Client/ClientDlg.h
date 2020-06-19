
// ClientDlg.h : header file
//

#pragma once
#define _AFXDLL
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define PORT 25000
#define WM_SOCKET WM_USER+2
#include "afxsock.h"
#include "PrivateChatDlg.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>;
using namespace std;

#define FLAG_SIGNUP 1
#define FLAG_LOGIN 2
#define FLAG_CHAT_PUBLIC 3
#define FLAG_NEW_USER 4
#define FLAG_LOGOUT 5
#define FLAG_CHAT_PRIVATE 6

class PrivateChatDlg;

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
	/*Biến*/
	SOCKET sClient;
	sockaddr_in servAdd;
	//CString Command;
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
	afx_msg void OnBnClickedSignup();
	afx_msg void OnBnClickedSend();	// gui tin nhan public

	CString IP;
	CString username;
	CString password;
	CString chatMessage;	// noi dung chat public muon gui di
	CListBox lstBoxChat;		// noi chung phong chat public
	CListBox lst_onluser;
	CString strItemSelected;

	afx_msg void OnLbnDblclkOnluser();
	afx_msg void OnBnClickedLogout();

	virtual void PostNcDestroy();
	virtual void OnCancel();

	// Xu li private chat
	//PrivateChatDlg* pPrvChatDlg = nullptr;
	vector< PrivateChatDlg*> listPrv;
	//unordered_set<CString> setOpenUserPrv;
	
	void PrivateChatDlgDelete(PrivateChatDlg* pPrvDlg);
	void sendPrvMsgToServer(CString s);
};
