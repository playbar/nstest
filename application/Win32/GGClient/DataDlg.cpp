// DataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "DataDlg.h"


// CDataDlg 对话框

IMPLEMENT_DYNAMIC(CDataDlg, CDialog)

CDataDlg::CDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDlg::IDD, pParent)
{

}

CDataDlg::~CDataDlg()
{
}

void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROC, m_proc);
}


BEGIN_MESSAGE_MAP(CDataDlg, CDialog)

END_MESSAGE_MAP()

void CDataDlg::SetMax(int nMax)
{
	m_proc.SetRange32(0,nMax);
	m_proc.SetPos(0);
}

void CDataDlg::SetProc(int nProc)
{
	m_proc.SetPos(nProc);
}
