
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
	DDX_Control(pDX, LST_CLIENTS, lst_clients);
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
			vector<CString> res = Split(temp);
			int flag = _ttoi(res[0]);	//Chuyển chuỗi thành số
			
			// Kiem tra flag va thuc hien xu li chuc nang tuong ung
			string username, pass, recvUser, recvPass;
			switch (flag) {
				case FLAG_SIGNUP: { //Sign up
					recvUser = CStringToString(res[1]);
					recvPass = CStringToString(res[2]);

					is.open("user.txt");
					bool founded = false;
					while (!is.eof()) {
						is >> username >> pass;
						if (username == recvUser) {
							founded = true;
						}
					}
					is.close();

					// Xoa thang sockClient moi vua them vao
					pSock.pop_back();

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
					
					break;
				}
				case FLAG_LOGIN: { // Login
					recvUser = CStringToString(res[1]);
					recvPass = CStringToString(res[2]);

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
						//Xoa thang sockClient moi vua them vao
						pSock.pop_back();

						temp = _T("2\r\n0");
						mSend(wParam, temp);

						return 0;
					}

					temp = CString(recvUser.c_str()) + _T(" da dang nhap");
					lst_event.AddString(temp);

					// Them Client moi vao Connected Clients
					lst_clients.AddString(CString(recvUser.c_str()));

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

					UpdateData(FALSE);

					break;
				}
				case FLAG_CHAT_PUBLIC: { // Chat public
					// res[0]: flag
					// res[1]: username (ten cua client da gui chatMessage)
					// res[2]: chatMessage

					// Hien thi noi dung chat len list event cua server
					CString chatText = res[1] + _T(": ") + res[2]; // format: "Sender: message"
					lst_event.AddString(chatText);
					UpdateData(false);
					
					// Gui noi dung chat cho cac client con lai
					CString commandTemp = _T("3\r\n") + res[1] + _T("\r\n") + res[2];
					CString command = commandTemp;
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
				case FLAG_CHAT_PRIVATE: {
					// Dang: 6-receiver-message
					CString receiver = res[1];
					CString message = res[2];

					CString temp;
					temp = CString(pSock[pos]->Name.c_str()) + " to " + receiver + " (privately): " + message;
					lst_event.AddString(temp);

					for (int i = 0; i < pSock.size(); i++) {
						if (CStringToString(receiver) == pSock[i]->Name) {
							temp = _T("6\r\n") + CString(pSock[pos]->Name.c_str()) + _T("\r\n") + message;
							mSend(pSock[i]->sockClient, temp);
							break;
						}
					}
					UpdateData(FALSE);
					break;
				}
				case FLAG_REVFILE_PUBLIC:
				{
					char* str = new char[30720]{ 0 };
					FILE* fp;
					fp = _wfopen(res[1], L"wb");

					long sizereceive = 0;
					if (fp != nullptr)
					{
						int size;
						while ((size = recv(wParam, str, 30720, 0)) > 0)
						{
							fwrite(str, 1, size, fp);
							Sleep(1);
						}
						fclose(fp);
					}

					delete[] str;

					//sau khi tai ve server thi bao cho nguoi dung gui thanh cong va tai len listfilename

					CString name;
					for(int i = 0; i < pSock.size(); i++)
						if (pSock[i]->sockClient == wParam)
						{
							name = pSock[i]->Name.c_str();
							break;
						}
					for(int i = 0; i < pSock.size(); i++)
						if (pSock[i]->sockClient == wParam)
						{
							CString chatText = name + _T(" sent file: [") + res[1] + _T("]"); // format: "Sender: message"
							lst_event.AddString(chatText);

							chatText = _T("9\r\nYou sent file: [\r\n") + res[1] + _T("\r\n]");
							mSend(pSock[i]->sockClient, chatText);
						}
						else
						{
							CString chatText =_T("9\r\n") + name + _T(" sent file[\r\n") + res[1] + _T("\r\n]");
							mSend(pSock[i]->sockClient, chatText);
						}

					UpdateData(FALSE);
				}break;
				case FLAG_REVFILE_PRIVATE:
				{
					// res[1]: nguoi nhan
					// res[2]: ten file

					int index = -1;

					for (int i = 0; i < pSock.size(); i++)
					{
						if (CString(pSock[i]->Name.c_str()) == res[1])
						{
							index = i;
							break;
						}
					}

					//CString Command = _T("11\r\n") + CString(pSock[pos]->Name.c_str()) + _T("\r\n") + res[2];

					//int check;
					//mSend(pSock[index]->sockClient, Command);
					//recv(pSock[index]->sockClient, (char*)&check, sizeof(int), 0);

					/*if (check == 0)
					{
						send(pSock[pos]->sockClient, (char*)&check, sizeof(int), 0);
						Command = CString(pSock[pos]->Name.c_str()) + _T(" denied sending file to ") + res[1] + _T(" (privately): [") + res[2] + _T("]");
						lst_event.AddString(Command);
						break;
					}
					send(pSock[pos]->sockClient, (char*)&check, sizeof(int), 0);*/

					CString s = _T("PrvFile\\") + res[2]; //res[2] la ten file
					char* str = new char[30720]{ 0 };
					FILE* fp;
					fp = _wfopen(s, L"wb");

					long sizereceive = 0;
					if (fp != nullptr)
					{
						int size;
						while ((size = recv(wParam, str, 30720, 0)) > 0)
						{
							fwrite(str, 1, size, fp);
							Sleep(1);
						}
						fclose(fp);
					}
					delete[] str;
					
					//Gui den client nhan file: 12-nguoi gui-tenfile 
					CString Command = _T("12\r\n") + CString(pSock[pos]->Name.c_str()) + _T("\r\n") + res[2];
					mSend(pSock[index]->sockClient, Command);



					//Command = CString(pSock[pos]->Name.c_str()) + _T(" sent file to ") + res[1] + _T(" (privately): [") + res[2] + _T("]");
					//lst_event.AddString(Command);

				}break;
				case FLAG_SENFILE_PUBLIC:
				{
					CString Command = _T("10\r\n") + res[1];
					mSend(wParam, Command);
					
					char* str = new char[30720]{ 0 };
					FILE* fp;
					fp = _wfopen(res[1], L"rb");	//pathfile la vi tri cua file

					if (fp != nullptr)	//mo duoc file
					{
						int size;
						while ((size = fread(str, 1, 30720, fp)) > 0)
						{
							int sizeleft = size;
							int sizerecv;
							while (sizeleft > 0)
							{
								sizerecv = send(wParam, str, sizeleft, 0);

								if (sizerecv > 0)
								{
									sizeleft -= sizerecv;
									str += sizerecv;
								}
							}
							str -= size;
						}
						fclose(fp);
					}
					delete[] str;

					UpdateData(FALSE);
				}break;
				case FLAG_SENFILE_PRIVATE: { //tai file private tu server
					// res[1]: nguoi gui
					// res[2]: ten file

					CString s = _T("PrvFile\\") + res[2]; //res[2] la ten file
					FILE* fp;
					char*  str = new char[30720]{ 0 };
					fp = _wfopen(s, L"rb");	//s la vi tri cua file
					if (fp != nullptr)	//mo duoc file
					{
						int size;
						while ((size = fread(str, 1, 30720, fp)) > 0)
						{
							int sizeleft = size;
							int sizerecv;
							while (sizeleft > 0)
							{
								sizerecv = send(wParam, str, sizeleft, 0);

								if (sizerecv > 0)
								{
									sizeleft -= sizerecv;
									str += sizerecv;
								}
							}
							str -= size;
						}
						fclose(fp);
					}
					delete[] str;

					CString temp = res[1] + _T(" sent file [") + res[2] + ("] to ") + CString(pSock[pos]->Name.c_str());
					lst_event.AddString(temp);
					UpdateData(FALSE);
				}break;
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
			
			// Neu khong co trong pSock thi break
			if (pos == -1)
				break;
			
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

			// Xoa client da dong ket noi trong Connected Clients
			lst_clients.DeleteString(pos);

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

vector<CString> CServerDlg::Split(CString str)
{
	vector<CString> res;
	int nTokenPos = 0;
	CString strToken = str.Tokenize(_T("\r\n"), nTokenPos);

	while (!strToken.IsEmpty())
	{
		res.push_back(strToken);
		strToken = str.Tokenize(_T("\r\n"), nTokenPos);
	}
	return res;
}

string CServerDlg::CStringToString(CString s)
{
	return CW2A(s.GetString());
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
	OnCancel();
}


void CServerDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	INT_PTR i = MessageBox(_T("Ban muon dong server?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;

	// Xoa pSocket
	for (int i = 0; i < pSock.size(); i++)
	{
		closesocket(pSock[i]->sockClient);
		delete pSock[i];
	}

	CDialogEx::OnCancel();
}
