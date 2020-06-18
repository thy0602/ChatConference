﻿
// ClientDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg dialog



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
	, IP(_T(""))
	, username(_T(""))
	, password(_T(""))
	, strItemSelected(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, EDIT_IP, IP);
	DDX_Text(pDX, EDIT_USER, username);
	DDX_Text(pDX, EDIT_PASS, password);
	DDX_Control(pDX, LST_ONLUSER, lst_onluser);
	DDX_LBString(pDX, LST_ONLUSER, strItemSelected);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_LBN_SELCHANGE(LSTBOX_CHAT, &CClientDlg::OnLbnSelchangeChat)
	ON_BN_CLICKED(BTN_LOGIN, &CClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(BTN_SIGNUP, &CClientDlg::OnBnClickedSignup)
	ON_LBN_DBLCLK(LST_ONLUSER, &CClientDlg::OnLbnDblclkOnluser)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



LRESULT CClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam)) {
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ: {
			CString temp;
			if (mRecv(temp) < 0)
				break;
			
			vector<string> res =  Split(temp);
			int flag = stoi(res[0]); // chuyen chuoi thanh so
			switch (flag) {
				case 1: { //Sign up: 1 0 - Fail, 1 1 - Success
					int result = stoi(res[1]);
					if (result == 0) {
						MessageBox(_T("Dang ki that bai! Moi chon Username khac!"));
					}
					else {
						MessageBox(_T("Dang ki thanh cong! Moi ban dang nhap!"));
					}
					SetDlgItemTextW(EDIT_USER, _T(""));
					SetDlgItemTextW(EDIT_PASS, _T(""));
					break;
				}
				case 2: { //Login: 2 0 - Fail, 2 1 - Success
					int result = stoi(res[1]);
					if (result == 0) {
						MessageBox(_T("Sai Username hoac Password!"));
					}
					else {
						MessageBox(_T("Dang nhap thanh cong! Cung chat nao!"));

						GetDlgItem(BTN_SIGNUP)->EnableWindow(FALSE);
						GetDlgItem(BTN_LOGIN)->EnableWindow(FALSE);
						GetDlgItem(EDIT_IP)->EnableWindow(FALSE);
						GetDlgItem(EDIT_USER)->EnableWindow(FALSE);
						GetDlgItem(EDIT_PASS)->EnableWindow(FALSE);
					}
					break;
				}
				case 4: {
					for (int i = 1; i < res.size(); i++) {
						lst_onluser.AddString(CString(res[i].c_str()));
					}
					break;
				}
			}

		}
	}
	return 0;
}

char* CClientDlg::ConvertToChar(const CString& s)
{
	int nSize = s.GetLength();
	char* pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CClientDlg::mSend(CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);
	delete sendBuff;
}

int CClientDlg::mRecv(CString& Command)
{
	int buffLength;
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sClient, (char*)&buffLength, sizeof(int), 0);
	recv(sClient, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

bool CClientDlg::findSpace(string s)
{
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == ' ')
			return true;
	}
	return false;
}

void CClientDlg::ConnectToServer()
{
	UpdateData(TRUE);

	if (IP == "") {
		MessageBox(_T("Ban chua nhap dia chi IP"));
		return;
	}

	// Bổ sung
	AfxSocketInit(nullptr);

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}

	if (username == _T("") || password == _T(""))
	{
		MessageBox(_T("Ban chua dien Username hoac Password"));
		return;
	}

	string s1, s2;
	s1 = CW2A(username.GetString());
	s2 = CW2A(password.GetString());
	if (findSpace(s1) || findSpace(s2)) {
		MessageBox(_T("Username va Password khong duoc chua khoang trang"));
		return;
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);

	char* cIP = ConvertToChar(IP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(IP);

	if (servAdd.sin_addr.s_addr == INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBox(_T("Khong the ket noi den server."), _T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0], host->h_length);
		return;
	}

	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
	if (err == SOCKET_ERROR) {
		MessageBox(_T("Ket noi that bai"), _T("ERROR"), 0);
		return;
	}

	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
}

vector<string> CClientDlg::Split(CString src)
{
	string delimiter = "\r\n";
	string s = CW2A(src.GetString());
	int pos = 0;
	string token;
	vector<string> res;

	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		res.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	res.push_back(s);
	return res;
}

void CClientDlg::OnLbnSelchangeChat()
{
	// TODO: Add your control notification handler code here
}


void CClientDlg::OnBnClickedLogin()
{
	ConnectToServer();

	//Phai UpdateData thi bien cua EditText moi duoc cap nhat
	UpdateData(TRUE);

	CString temp;
	temp = _T("2\r\n") + username + _T("\r\n") + password;
	mSend(temp);
}

void CClientDlg::OnBnClickedSignup()
{
	ConnectToServer();

	//Phai UpdateData thi bien cua EditText moi duoc cap nhat
	UpdateData(TRUE);

	CString temp;
	temp = _T("1\r\n") + username + _T("\r\n") + password;
	mSend(temp);
}


void CClientDlg::OnLbnDblclkOnluser()
{
	lst_onluser.GetText(lst_onluser.GetCurSel(), strItemSelected);
	MessageBox(strItemSelected);
}
