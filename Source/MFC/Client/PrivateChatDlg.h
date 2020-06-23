#pragma once

#include "ClientDlg.h"
// PrivateChatDlg dialog

class PrivateChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PrivateChatDlg)

public:
	PrivateChatDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~PrivateChatDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRV_CHAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	// luu con tro toi dialog da goi dialog nay
	CWnd* parent;
public:
	CString chatMessage;
	CListBox lstBoxPrivateChat;

	wchar_t path_file[MAX_PATH];
	bool bfile = false;

	bool ChooseFile();

	void updateSenderReceiver(CString sender, CString receiver);
	void receivePrvMsg(CString s);
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedBtnPrvSend();
	CString prvSender;
	CString prvReceiver;
	afx_msg void OnBnClickedBtnPrvFile();
};
