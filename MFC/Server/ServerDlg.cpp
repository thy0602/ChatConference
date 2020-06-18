
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
	ON_BN_CLICKED(BTN_EXIT, &CServerDlg::OnBnClickedExit)
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
			lst_event.AddString(_T("Connect Sucess"));
			UpdateData(FALSE);
			break;
		}
		case FD_READ: {
			// Update gia tri tu code len giao dien
			UpdateData(FALSE);

			// Tim socket da gui goi tin (trong ds socket luu trong vector pSock)
			int pos = -1;
			for (int i = 0; i < pSock.size(); i++) {
				if (pSock[i]->sockClient == wParam) {
					pos = i;
				}
			}
			
			// Lay noi dung goi tin client da gui
			CString temp;
			if (mRecv(wParam, temp) < 0)
				break;

			// Tach flag va content trong goi tin nhan duoc
			vector<string> res = Split(temp);
			int flag = stoi(res[0]);	//Chuyển chuỗi thành số
			
			// Kiem tra flag va thuc hien xu li chuc nang tuong ung
			string username, pass, recvUser, recvPass;
			switch (flag) {
				case FLAG_SIGNUP: { //Sign up
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
						temp = _T("1\r\n0");
						mSend(wParam, temp);
						return 0;
					}

					os.open("user.txt", ios::app | ios::ate);
					os << recvUser << " " << recvPass << endl;
					os.close();

					CString str = CString(recvUser.c_str()) + _T(" vua moi sign up!");
					lst_event.AddString(str);

					temp = _T("1\r\n1");
					mSend(wParam, temp);

					// Close Client
					closesocket(wParam);
					// Xoa thang sockClient moi vua them vao
					pSock.pop_back();
					break;
				}
				case FLAG_LOGIN: { // Login
					recvUser = res[1];
					recvPass = res[2];

					is.open("user.txt");
					bool founded = false;
					while (!is.eof()) {
						is >> username >> pass;
						if (username == recvUser && pass == recvPass) {
							founded = true;
							break;
						}
					}
					is.close();

					if (!founded) {
						temp = _T("2\r\n0");
						mSend(wParam, temp);

						// Close Client
						closesocket(wParam);
						// Xoa thang sockClient moi vua them vao
						pSock.pop_back();
						return 0;
					}

					temp = CString(recvUser.c_str()) + _T(" da dang nhap");
					lst_event.AddString(temp);

					// Set Name cho pSock[pos]
					pSock[pos]->Name = recvUser;

					// Gui thong bao login thanh cong cho client
					temp = _T("2\r\n1");
					mSend(wParam, temp);

					// Gui cac user dang online cho client moi login
					temp = _T("4");
					for (int i = 0; i < pSock.size(); i++) {
						temp += _T("\r\n") + CString(pSock[i]->Name.c_str());
					}
					mSend(wParam, temp);

					// Gui ten user moi vua online cho cac user da online truoc do
					for (int i = 0; i < pSock.size() - 1; i++) {
						temp = _T("4\r\n") + CString(pSock[pos]->Name.c_str());
						mSend(pSock[i]->sockClient, temp);
					}

					break;
				}
				case FLAG_CHAT_PUBLIC: { // Chat public
					// res[0]: flag
					// res[1]: username (ten cua client da gui chatMessage)
					// res[2]: chatMessage

					// Hien thi noi dung chat len list event cua server
					string chatText = res[1] + ": " + res[2]; // format: "Sender: message"
					lst_event.AddString(CString(chatText.c_str()));
					UpdateData(false);
					
					// Gui noi dung chat cho cac client con lai
					string commandTemp = "3\r\n" + res[1] + "\r\n" + res[2];
					CString command = CString((commandTemp.c_str()));
					for (int i = 0; i < pSock.size(); i++)
					{
						// pos la vi tri cua socket (trong vector pSock) da gui chat public
						if (i != pos)
						{
							mSend(pSock[i]->sockClient, command);
						}
					}
					break;
				}
			}
			break;
		}
		case FD_CLOSE: {
			// Tim socket da dong ket noi (trong ds socket luu trong vector pSock)
			int pos = -1;
			for (int i = 0; i < pSock.size(); i++) {
				if (pSock[i]->sockClient == wParam) {
					pos = i;
				}
			}

			// Hien thong bao user log out len list event cua server
			string text = pSock[pos]->Name + " logout";
			lst_event.AddString(CString(text.c_str()));

			// Gui goi tin cho cac client con lai de xoa ten user ra khoi ds online user
			string commandTemp = "5\r\n" + pSock[pos]->Name;
			CString command = CString((commandTemp.c_str()));
			for (int i = 0; i < pSock.size(); i++)
			{
				// pos la vi tri cua socket (trong vector pSock) da gui chat public
				if (i != pos)
				{
					mSend(pSock[i]->sockClient, command);
				}
			}

			// Dong ket noi
			closesocket(wParam);

			// Xoa socket ra khoi pSock
			delete pSock[pos];
			pSock.erase(pSock.begin() + pos);

			UpdateData(false);
			break;
		}
		default:
			lst_event.AddString(_T("Vo ngoai le"));
	}
	return 0;
}

void CServerDlg::mSend(SOCKET sk, CString Command)
{
	//MessageBox(_T("Da gui du lieu cho client"));
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sk, (char*)& Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);
	delete sendBuff;
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
	//pSock = vector<SockName*>(0);
}


void CServerDlg::OnBnClickedExit()
{
	INT_PTR i = MessageBox(_T("Ban muon dong server?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;

	// Xoa pSocket
	for (int i = 0; i < pSock.size(); i++)
	{
		closesocket(pSock[i]->sockClient);
		delete pSock[i];
	}

	OnCancel();
}
