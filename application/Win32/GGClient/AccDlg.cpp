// AccDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "AccDlg.h"


// CAccDlg 对话框

IMPLEMENT_DYNAMIC(CAccDlg, CDialog)

CAccDlg::CAccDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccDlg::IDD, pParent)
{

}

CAccDlg::~CAccDlg()
{
}

void CAccDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOW, m_accEnu);
}


BEGIN_MESSAGE_MAP(CAccDlg, CDialog)

END_MESSAGE_MAP()


// CAccDlg 消息处理程序

BOOL CAccDlg::Create(void)
{
	if(!CDialog::Create(IDD,NULL)) return FALSE;
	this->m_accEnu.Init();
	ShowWindow(SW_SHOW);	
	return TRUE;
}

