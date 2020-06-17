
// ServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FLAG_SINGUP 1


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
public:
	afx_msg void OnBnClickedListen();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(BTN_LISTEN, &CAboutDlg::OnBnClickedListen)
END_MESSAGE_MAP()


// CServerDlg dialog



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LST_EVENT, lst_event);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// Add manually
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(BTN_LISTEN, &CServerDlg::OnBnClickedListen)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAboutDlg::OnBnClickedListen()
{
	SetDlgItemText(IDC_STATIC, _T("HELLO"));
}


LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	string res[2];
	ifstream is;
	ofstream os;

	UpdateData(TRUE);

	if (WSAGETSELECTERROR(lParam))
	{
		MessageBox(_T("WSAGETSELECTERROR"));
		// Display the error and close the socket
		closesocket(wParam);
	}

	switch (WSAGETSELECTEVENT(lParam)) {
		case FD_ACCEPT: {
			SockName* p = new SockName;
			p->sockClient = accept(wParam, NULL, NULL);
			pSock.push_back(p);
			lst_event.AddString(_T("1 Connect Sucess"));
			UpdateData(FALSE);
			break;
		}
		case FD_READ: {
			lst_event.AddString(_T("Co Client gui message"));
			// Update gia tri tu code len giao dien
			UpdateData(FALSE);
			int pos = -1;
			for (int i = 0; i < pSock.size(); i++) {
				if (pSock[i]->sockClient == wParam) {
					pos = i;
				}
			}
			

			CString temp;
			if (mRecv(wParam, temp) < 0)
				break;

			vector<string> res = Split(temp);
			int flag = stoi(res[0]); //Chuyển chuỗi thành số
			string username, pass, recvUser, recvPass;
			switch (flag) {
				case 1: { //Sign up
					recvUser = res[1];
					recvPass = res[2];

					is.open("user.txt");
					bool founded = false;
					while (!is.eof()) {
						is >> username >> pass;
						if (username == recvUser) {
							founded = true;
						}
					}
					is.close();

					if (founded) {
						MessageBox(_T("Trung Username. Moi chon Username khac!"));
						return 0;
					}

					os.open("user.txt", ios::app | ios::ate);
					os << recvUser << " " << recvPass << endl;
					os.close();
					break;
				}
			}
			break;
		}
		default:
			lst_event.AddString(_T("Vo ngoai le"));
	}
	return 0;
}

int CServerDlg::mRecv(SOCKET &sk, CString& Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	int buffLength;
	recv(sk, (char*)&buffLength, sizeof(int), 0);
	recv(sk, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

vector<string> CServerDlg::Split(CString src)
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

void CServerDlg::OnBnClickedListen()
{
	UpdateData();
	AfxSocketInit(nullptr);

	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_port = htons(PORT);
	serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockServer, (SOCKADDR*)&serverAdd, sizeof(serverAdd));
	listen(sockServer, 5);
	int err = WSAAsyncSelect(sockServer, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	if (err)
		MessageBox(_T("Cant call WSAAsyncSelect"));
	GetDlgItem(BTN_LISTEN)->EnableWindow(FALSE);
	pSock = vector<SockName*>(0);
}
