// TextDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "TextDlg.h"


// CTextDlg 对话框

IMPLEMENT_DYNAMIC(CTextDlg, CDialog)

CTextDlg::CTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextDlg::IDD, pParent)
	, m_strText(_T(""))
{
	m_bOK=FALSE;
}

CTextDlg::~CTextDlg()
{
}

void CTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT, m_strText);
	DDV_MaxChars(pDX, m_strText, 64000);
}


BEGIN_MESSAGE_MAP(CTextDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTextDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTextDlg 消息处理程序

void CTextDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bOK=TRUE;
	OnOK();
}



BOOL CTextDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_TEXT)->SetFocus();
	return XTRUE;
}
