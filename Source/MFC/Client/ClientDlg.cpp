
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
	, chatMessage(_T(""))
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
	DDX_Text(pDX, EDIT_MSG, chatMessage);
	DDX_Control(pDX, LSTBOX_CHAT, lstBoxChat);
	DDX_Control(pDX, LST_ONLUSER, lst_onluser);
	DDX_Control(pDX, LST_FILENAME, lst_filename);
	DDX_LBString(pDX, LST_ONLUSER, strItemSelected);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_BN_CLICKED(BTN_LOGIN, &CClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(BTN_SIGNUP, &CClientDlg::OnBnClickedSignup)
	ON_BN_CLICKED(BTN_SEND, &CClientDlg::OnBnClickedSend)
	ON_LBN_DBLCLK(LST_ONLUSER, &CClientDlg::OnLbnDblclkOnluser)
	ON_BN_CLICKED(BTN_LOGOUT, &CClientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(BTN_FILE, &CClientDlg::OnBnClickedFile)
	ON_LBN_DBLCLK(LST_FILENAME, &CClientDlg::OnLbnDblclkFilename)
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
	// Gan IP mac dinh cho edit box
	GetDlgItem(EDIT_IP)->SetWindowTextW(_T("127.0.0.1"));
	GetDlgItem(BTN_LOGOUT)->EnableWindow(false);
	GetDlgItem(BTN_SEND)->EnableWindow(false);
	GetDlgItem(BTN_FILE)->EnableWindow(false);

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

static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{

	if (uMsg == BFFM_INITIALIZED)
	{
		std::string tmp = (const char*)lpData;
		std::cout << "path: " << tmp << std::endl;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
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

		vector<CString> res = Split(temp);
		int flag = _ttoi(res[0]); // chuyen chuoi thanh so
		switch (flag) {
		case FLAG_SIGNUP: { //Sign up: 1 0 - Fail, 1 1 - Success
			int result = _ttoi(res[1]);
			if (result == 0) {
				MessageBox(_T("Dang ki that bai! Moi chon Username khac!"));
			}
			else {
				MessageBox(_T("Dang ki thanh cong! Moi ban dang nhap!"));
			}
			// Close Client
			closesocket(sClient);

			SetDlgItemTextW(EDIT_USER, _T(""));
			SetDlgItemTextW(EDIT_PASS, _T(""));
			break;
		}
		case FLAG_LOGIN: { //Login: 2 0 - Fail, 2 1 - Success
			int result = _ttoi(res[1]);
			if (result == 0) {
				MessageBox(_T("Sai Username hoac Password!"));
				// Close Client
				closesocket(sClient);
			}
			else {
				MessageBox(_T("Dang nhap thanh cong! Cung chat nao!"));

				GetDlgItem(BTN_SIGNUP)->EnableWindow(FALSE);
				GetDlgItem(BTN_LOGIN)->EnableWindow(FALSE);
				GetDlgItem(EDIT_IP)->EnableWindow(FALSE);
				GetDlgItem(EDIT_USER)->EnableWindow(FALSE);
				GetDlgItem(EDIT_PASS)->EnableWindow(FALSE);

				GetDlgItem(EDIT_MSG)->EnableWindow(TRUE);
				GetDlgItem(BTN_SEND)->EnableWindow(TRUE);
				GetDlgItem(BTN_FILE)->EnableWindow(TRUE);
				GetDlgItem(BTN_LOGOUT)->EnableWindow(TRUE);
			}
			break;
		}
		case FLAG_CHAT_PUBLIC: {
			// res[0]: flag
			// res[1]: sender (username cua client da gui tin nhan public)
			// res[2]: noi dung tin nhan public

			// Hien thi noi dung chat len listbox chat
			CString chatText = res[1] + _T(": ") + res[2]; // format: "Sender: message"
			lstBoxChat.AddString(chatText);
			UpdateData(false);
			break;
		}
		case FLAG_NEW_USER: {
			// 4 - user1 - user2 - ...
			for (int i = 1; i < res.size(); i++) {
				lst_onluser.AddString(res[i]);
			}

			// Thong bao co user moi login
			if (res.size() == 2 && res[1] != username)
			{
				CString text = _T("*") + res[1] + _T(" da dang nhap!");
				lstBoxChat.AddString(text);
			}
			UpdateData(false);
			break;
		}
		case FLAG_LOGOUT: {
			// res[0]: flag
			// res[1]: user name cua client da log out

			// Xoa user name da log out ra khoi ds online user
			int idx = lst_onluser.FindString(0, res[1]);
			lst_onluser.DeleteString(idx);

			// Hien thong bao user da logout
			CString text = _T("*") + res[1] + _T(" da dang xuat!");
			lstBoxChat.AddString(text);

			// Thong bao res[1] da logout den Client dang chat private
			for (int i = 0; i < listPrv.size(); i++) {
				if (listPrv[i]->prvReceiver == res[1]) {
					CString temp = res[1] + " da logout!";
					listPrv[i]->receivePrvMsg(temp);
					listPrv[i]->GetDlgItem(IDC_EDIT_PRV_MSG)->EnableWindow(FALSE);
					listPrv[i]->GetDlgItem(IDC_BTN_PRV_SEND)->EnableWindow(FALSE);
					listPrv[i]->GetDlgItem(IDC_BTN_PRV_FILE)->EnableWindow(FALSE);
					listPrv[i]->UpdateData(FALSE);
					break;
				}
			}

			UpdateData(false);
			break;
		}
		case FLAG_CHAT_PRIVATE: { // Chat private
			// Dang: 6-sender-message
			CString sender = res[1];
			CString message = res[2];
			// Kiem tra xem cua so chat private voi sender da duoc mo chua
			int pos = -1;
			for (int i = 0; i < listPrv.size(); i++) {
				if (listPrv[i]->prvReceiver == sender) {
					pos = i;
					break;
				}
			}

			// TH cua so chat private da duoc mo
			CString messageContent = sender + _T(": ") + message;
			if (pos != -1) {
				listPrv[pos]->receivePrvMsg(messageContent);
				break;
			}

			// TH cua so chat private chua duoc mo
			PrivateChatDlg* pPrvChatDlg = new PrivateChatDlg(this);
			listPrv.push_back(pPrvChatDlg);
			pPrvChatDlg->Create(IDD_PRV_CHAT, this);
			pPrvChatDlg->ShowWindow(SW_SHOW);
			pPrvChatDlg->updateSenderReceiver(username, sender);
			pPrvChatDlg->receivePrvMsg(messageContent);
			break;
		}
		case FLAG_FILE_PUBLIC:
		{
			CString chatText = res[1] + res[2] + res[3]; // format: "Sender: message"
			lstBoxChat.AddString(chatText);
			lst_filename.AddString(res[2]);
			UpdateData(false);
			break;
		}break;
		case FLAG_REVFILE_PUBLIC:
		{
			s = s + _T("\\") + res[1];
			char* str = new char[30720]{ 0 };
			FILE* fp;
			fp = _wfopen(s, L"wb");
			if (fp != nullptr)
			{
				int size;
				while ((size = recv(sClient, str, 30720, 0)) > 0)
				{
					fwrite(str, 1, size, fp);
					Sleep(3);
				}
				fclose(fp);
			}
			delete[] str;

		}break;
		case FLAG_REVFILE_PRIVATE:
		{
			// res[1]: nguoi gui
			// res[2]: ten file

			// Kiem tra xem cua so chat private voi sender da duoc mo chua
			int pos = -1, check = 1;
			for (int i = 0; i < listPrv.size(); i++) {
				if (listPrv[i]->prvReceiver == res[1]) {
					pos = i;
					break;
				}
			}

			if (pos == -1) {
				// TH cua so chat private chua duoc mo
				PrivateChatDlg* pPrvChatDlg = new PrivateChatDlg(this);
				listPrv.push_back(pPrvChatDlg);
				pPrvChatDlg->Create(IDD_PRV_CHAT, this);
				pPrvChatDlg->ShowWindow(SW_SHOW);
				pPrvChatDlg->updateSenderReceiver(username, res[1]);
				pos = listPrv.size() - 1;
			}

			CString text = _T("Ban muon gui file ") + res[2] + (" khong?");
			INT_PTR i = listPrv[pos]->MessageBox(text, _T("Confirm"), MB_OKCANCEL);
			//neu chon cancel thi gui cho server biet la khong nhan
			if (i == IDCANCEL) {
				//check = 0;
				return 0;
			}
			//neu chon khac thi gui cho server biet la nhan file

			//pop up cua so chon noi luu ve cho sclient

			

			//send(sClient, (char*)&check, sizeof(int), 0);

			// MOI THEM
			CString Command = _T("12\r\n") + res[1] + _T("\r\n") + res[2];
			mSend(Command);

		}break;
		case 12: {
			// res[1]: nguoi gui
			// res[2]: ten file

			// Kiem tra xem cua so chat private voi sender da duoc mo chua
			int pos = -1, check = 1;
			for (int i = 0; i < listPrv.size(); i++) {
				if (listPrv[i]->prvReceiver == res[1]) {
					pos = i;
					break;
				}
			}

			if (pos == -1) {
				// TH cua so chat private chua duoc mo
				PrivateChatDlg* pPrvChatDlg = new PrivateChatDlg(this);
				listPrv.push_back(pPrvChatDlg);
				pPrvChatDlg->Create(IDD_PRV_CHAT, this);
				pPrvChatDlg->ShowWindow(SW_SHOW);
				pPrvChatDlg->updateSenderReceiver(username, res[1]);
				pos = listPrv.size() - 1;
			}

			CString temp = res[1] + _T(" sent file: [") + res[2] + _T("]");
			listPrv[pos]->lstBoxPrivateChat.AddString(temp);
			listPrv[pos]->UpdateData(FALSE);


			string saved_path;

			TCHAR path[MAX_PATH];

			const char* path_param = saved_path.c_str();

			BROWSEINFO bi = { 0 };
			bi.lpszTitle = _T("Browse for folder...");
			bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
			bi.lpfn = BrowseCallbackProc;
			bi.lParam = (LPARAM)path_param;

			LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

			if (pidl != 0)
			{
				//get the name of the folder and put it in path
				SHGetPathFromIDList(pidl, path);

				//free memory used
				IMalloc* imalloc = 0;
				if (SUCCEEDED(SHGetMalloc(&imalloc)))
				{
					imalloc->Free(pidl);
					imalloc->Release();
				}

				int i = 0;
				s = "";
				while (path[i] != '\0')
				{
					s = s + path[i];
					i++;
				};
			}

			// Gui 12-nguoi gui-ten file cho client
			CString Command = _T("12\r\n") + res[1] + _T("\r\n") + res[2];
			mSend(Command);

			s = s + _T("\\") + res[2];	//s la vi tri ma client chon de gui file
			char* str = new char[30720]{ 0 };
			FILE* fp;
			fp = _wfopen(s, L"wb");
			if (fp != nullptr)
			{
				int size;
				while ((size = recv(sClient, str, 30720, 0)) > 0)
				{
					fwrite(str, 1, size, fp);
					Sleep(1);
				}
				fclose(fp);
			}
			delete[] str;

			/*for (int i=0;i<listPrv.size();i++){
				if (listPrv[i]->prvReceiver == res[1]) {
					CString temp = res[1] + _T("sent file: [") + res[2] + _T("]");
					listPrv[i]->lstBoxPrivateChat.AddString(temp);
					listPrv[i]->UpdateData(FALSE);
					break;
				}
			}*/

		}break;
		} break;

		break;
	}
	case FD_CLOSE: {
		closesocket(wParam);
		MessageBox(_T("Server da dong ket noi!"));

		GetDlgItem(BTN_SEND)->EnableWindow(false);
		GetDlgItem(BTN_FILE)->EnableWindow(false);

		break;
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
	//MessageBox(_T("Da gui du lieu cho server"));
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);
	delete[] sendBuff;
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

bool CClientDlg::ChooseFile()
{
	OPENFILENAME ofn;

	ZeroMemory(&path_file, sizeof(path_file));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = _T("Any File\0*.*\0Text Files\0*.txt\0");
	ofn.lpstrFile = path_file;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = _T("Select a File, yo!");
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn))
		return false;
	else
		return true;
}

vector<CString> CClientDlg::Split(CString str)
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

string CClientDlg::CStringToString(CString s)
{
	return CW2A(s.GetString());
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


void CClientDlg::OnBnClickedSend()
{
	UpdateData(true);

	if (!bfile)
	{
		if (chatMessage.IsEmpty())
		{
			MessageBox(_T("Ban chua nhap tin nhan!"));
			return;
		}

		// Hien thi noi dung chat len list box
		lstBoxChat.AddString(username + _T(": ") + chatMessage);
		UpdateData(false);

		// Gui command qua cho server
		CString command = _T("3\r\n") + username + _T("\r\n") + chatMessage;
		mSend(command);
	}
	else
	{
		CString Command = _T("9\r\n") + chatMessage;
		mSend(Command);
		char* str = new char[30720]{ 0 };
		FILE* fp;
		fp = _wfopen(path_file, L"rb");	//pathfile la vi tri cua file

		if (fp != nullptr)	//mo duoc file
		{
			int size;
			while ((size = fread(str, 1, 30720, fp)) > 0)
			{
				int sizeleft = size;
				int sizerecv;
				while (sizeleft > 0)
				{
					sizerecv = send(sClient, str, sizeleft, 0);

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
		bfile = false;
	}

	chatMessage = _T("");
	UpdateData(false);
}

void CClientDlg::OnLbnDblclkOnluser()
{
	lst_onluser.GetText(lst_onluser.GetCurSel(), strItemSelected);
	// Khong the click chinh minh
	if (strItemSelected == username)
		return;

	// Neu cua so chat private dang duoc mo thi khong the mo nua
	for (int i = 0; i < listPrv.size(); i++) {
		if (listPrv[i]->prvReceiver == strItemSelected) {
			return;
		}
	}

	CString text = _T("Ban muon gui tin nhan rieng cho ") + strItemSelected + _T("?");
	INT_PTR i = MessageBox(text, _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;

	PrivateChatDlg* pPrvChatDlg = new PrivateChatDlg(this);
	listPrv.push_back(pPrvChatDlg);
	pPrvChatDlg->Create(IDD_PRV_CHAT, this);
	pPrvChatDlg->ShowWindow(SW_SHOW);
	pPrvChatDlg->updateSenderReceiver(username, strItemSelected);

}


void CClientDlg::OnBnClickedLogout()
{
	INT_PTR i = MessageBox(_T("Ban muon logout?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;

	GetDlgItem(BTN_LOGOUT)->EnableWindow(false);
	GetDlgItem(BTN_SEND)->EnableWindow(false);
	GetDlgItem(BTN_FILE)->EnableWindow(false);
	GetDlgItem(EDIT_MSG)->EnableWindow(false);

	GetDlgItem(BTN_LOGIN)->EnableWindow(true);
	GetDlgItem(BTN_SIGNUP)->EnableWindow(true);
	GetDlgItem(EDIT_IP)->EnableWindow(true);
	GetDlgItem(EDIT_USER)->EnableWindow(true);
	GetDlgItem(EDIT_PASS)->EnableWindow(true);

	// Xoa noi dung listbox online user
	lst_onluser.ResetContent();

	// Xoa noi dung listbox chat
	lstBoxChat.ResetContent();

	MessageBox(_T("Dang xuat thanh cong!"));

	// Dong ket noi
	closesocket(sClient);

	UpdateData(false);
}


void CClientDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::PostNcDestroy();

	// Dong ket noi
	closesocket(sClient);
}


void CClientDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	INT_PTR i = MessageBox(_T("Ban muon thoat chuong trinh?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;
	DestroyWindow();
}

void CClientDlg::PrivateChatDlgDelete(PrivateChatDlg* pPrvDlg)
{
	for (int i = 0; i < listPrv.size(); i++) {
		if (listPrv[i] == pPrvDlg) {
			delete listPrv[i];
			listPrv.erase(listPrv.begin() + i);
		}
	}
}

void CClientDlg::sendPrvMsgToServer(CString s)
{
	mSend(s);
}

int CClientDlg::sendPrvFileToServer(CString s, CString path_prvfile)
{
	mSend(s);

	int check;

	//recv(sClient, (char*)&check, sizeof(int), 0);

	//if (check == 0)
	//{
	//	return 0;
	//}

	char* str = new char[30720]{ 0 };
	FILE* fp;
	fp = _wfopen(path_prvfile, L"rb");	//pathfile la vi tri cua file

	if (fp != nullptr)	//mo duoc file
	{
		int size;
		while ((size = fread(str, 1, 30720, fp)) > 0)
		{
			int sizeleft = size;
			int sizerecv;
			while (sizeleft > 0)
			{
				sizerecv = send(sClient, str, sizeleft, 0);

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
	bfile = false;

	return 1;
}


void CClientDlg::OnBnClickedFile()
{
	bfile = ChooseFile();
	if (!bfile)	//neu bfile dung thi lam tiep - sai thi return
		return;
	int len = 0;
	while (path_file[len] != '\0')
		len++;

	CString tmp = L"";
	while (path_file[len] != '\\')
	{
		tmp = path_file[len] + tmp;
		len--;
	}
	chatMessage = tmp;
	UpdateData(FALSE);
}

void CClientDlg::OnLbnDblclkFilename()
{
	lst_filename.GetText(lst_filename.GetCurSel(), strItemSelected); //stritemselect là filename

	string saved_path;

	TCHAR path[MAX_PATH];

	const char* path_param = saved_path.c_str();

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = _T("Browse for folder...");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)path_param;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}

		int i = 0;
		s = "";
		while (path[i] != '\0')
		{
			s = s + path[i];
			i++;
		};
	}

	CString command = _T("10\r\n") + strItemSelected;
	mSend(command);
}
