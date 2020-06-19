// PrivateChatDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "PrivateChatDlg.h"
#include "afxdialogex.h"


// PrivateChatDlg dialog

IMPLEMENT_DYNAMIC(PrivateChatDlg, CDialogEx)

PrivateChatDlg::PrivateChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRV_CHAT, pParent)
	, chatMessage(_T(""))
	, parent(pParent)
	, prvSender(_T(""))
	, prvReceiver(_T(""))
{

}

PrivateChatDlg::~PrivateChatDlg()
{
}

void PrivateChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PRV_MSG, chatMessage);
	DDX_Control(pDX, IDC_LSTBOX_PRV_CHAT, lstBoxPrivateChat);
	DDX_Text(pDX, IDC_EDIT_PRV_SENDER, prvSender);
	DDX_Text(pDX, IDC_EDIT_PRV_RECEIVER, prvReceiver);
}


BEGIN_MESSAGE_MAP(PrivateChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_PRV_SEND, &PrivateChatDlg::OnBnClickedBtnPrvSend)
END_MESSAGE_MAP()


// PrivateChatDlg message handlers


void PrivateChatDlg::updateSenderReceiver(CString sender, CString receiver)
{
	prvSender = sender;
	prvReceiver = receiver;
	UpdateData(false);
}

void PrivateChatDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
}


void PrivateChatDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::PostNcDestroy();

	auto* pParent{ dynamic_cast<CClientDlg*>(parent) };
	if (pParent)
	{
		pParent->PrivateChatDlgDelete(this);
	}
}


void PrivateChatDlg::OnBnClickedBtnPrvSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(!chatMessage.IsEmpty())
	{
		auto* pParent{ dynamic_cast<CClientDlg*>(parent) };
		if (pParent)
		{
			pParent->demoPrivate(chatMessage);

			MessageBox(_T("Send successfully!"));
			chatMessage = _T("");
			UpdateData(false);
		}
	}
	else
	{
		MessageBox(_T("Message empty!"));
	}
}
