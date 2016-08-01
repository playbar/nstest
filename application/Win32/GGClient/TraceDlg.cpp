// TraceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "GGClientDlg.h"
#include "TraceDlg.h"
#include "XSystem.h"

// CTraceDlg 对话框

IMPLEMENT_DYNAMIC(CTraceDlg, CDialog)

BEGIN_MESSAGE_MAP(CTraceDlg, CDialog)
	//{{AFX_MSG_MAP(CGGClientDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ENABLEOUT, &CTraceDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CLEAR, &CTraceDlg::OnBnClickedClear)

END_MESSAGE_MAP()


CTraceDlg::CTraceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraceDlg::IDD, pParent)
{
	m_pEdit=NULL;
	m_bOutput=true;
	m_pTraceInfo = NULL;
	
}

bool CTraceDlg::OpenTraceFile(const char* strWorkPath)
{
	m_strTraceFile=CGGClientDlg::m_strWorkPath;
	m_strTraceFile+="\\GGClient.log";
	if(m_file.IsValid())
		m_file.Close();
	if(!m_file.Open(m_strTraceFile,XFile::XBINARY|XFile::XWRITE|XFile::XCREATE)) return false;
	XU8 utf8[]={0xEF,0xBB,0xBF};
	m_file.Write(utf8,3);
	m_file.Seek(0,XFile::XSEEK_END);
	return true;
}

CTraceDlg::~CTraceDlg()
{
	OnTimer(0);
	m_file.Close();
}

void CTraceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFOMATION, m_info);
}


BOOL CTraceDlg::CreateTrace(CRect&rect)
{
	if(!Create(IDD,NULL)) return FALSE;
	ShowWindow(SW_SHOW);
	m_pEdit=(CEdit*)GetDlgItem(IDC_OUTPUT);
	CRect r;
	GetWindowRect(r);
	r.top=rect.top+40;
	int w=r.Width(),h=r.Height();
	if(rect.left>w)
	{
		r.left=rect.left-w;
		r.right=r.left+w;
	}
	else
	{
		r.left=rect.right;
		r.right=r.left+w;
	}
	SetWindowPos(NULL,r.left,r.top,w,h,0);

	SetTimer(2,500,NULL);
	return TRUE;
}

void CTraceDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if(m_pEdit)	
	{
		m_pEdit->SetWindowPos(NULL,0,0,cx,cy-80,0);
		m_info.SetWindowPos(NULL,75,cy-78,cx-190,74,0);
		GetDlgItem(IDC_CLEAR)->SetWindowPos(NULL,4,cy-22,40,20,0);
		GetDlgItem(IDC_ENABLEOUT)->SetWindowPos(NULL,4,cy-80,70,20,0);
		CheckDlgButton(IDC_ENABLEOUT,m_bOutput?2:0);
		GetDlgItem(IDC_TOTALFRAME)->SetWindowPos(NULL,cx-110,cy-80,160,20,0);
		GetDlgItem(IDC_TOTALTEXTURES)->SetWindowPos(NULL,cx-110,cy-60,160,20,0);
		GetDlgItem(IDC_TOTALIMAGE)->SetWindowPos(NULL,cx-110,cy-40,160,20,0);
		GetDlgItem(IDC_TOTALBUFFER)->SetWindowPos(NULL,cx-110,cy-20,160,20,0);
	}
	// TODO: Add your message handler code here
}

void CTraceDlg::UpdateInfo()
{
	CString str;
	str.Format(_T("%d 个(%d.%#03d 兆)"),XSystem::_nTotalFrames,XSystem::_nTotalFrameMemory/1000000,(XSystem::_nTotalFrameMemory/1000)%1000);
	SetDlgItemText(IDC_TOTALFRAME,str);
	str.Format(_T("%d 个(%d.%#03d 兆)"),XSystem::_nTotalTextures,XSystem::_nTotalTextureMemory/1000000,(XSystem::_nTotalTextureMemory/1000)%1000);
	SetDlgItemText(IDC_TOTALTEXTURES,str);
	str.Format(_T("%d 个(%d.%#03d 兆)"),XSystem::_nTotalImages,XSystem::_nTotalImageMemory/1000000,(XSystem::_nTotalImageMemory/1000)%1000);
	SetDlgItemText(IDC_TOTALIMAGE,str);
	str.Format(_T("%d 个(%d.%#03d 兆)"),XSystem::_nTotalBuffers,XSystem::_nTotalBufferMemory/1000000,(XSystem::_nTotalBufferMemory/1000)%1000);
	SetDlgItemText(IDC_TOTALBUFFER,str);
}

void CTraceDlg::OnTimer(UINT nIDEvent)
{
	if(m_lock.Lock(1000))
	{
		CString strTmp;
		while(m_pTraceInfo)
		{
			TRACEINFO* pNext = m_pTraceInfo->pNext;
			strTmp=m_pTraceInfo->strTmp+strTmp;
			delete m_pTraceInfo;
			m_pTraceInfo = pNext;
		}
		m_lock.UnLock();
		if(!strTmp.IsEmpty()&&::IsWindow(m_pEdit->m_hWnd))
		{
			CString strTxt;
			m_pEdit->GetWindowText(strTxt);
		    strTxt+=strTmp;
			//strTxt+="\r\n";
			m_pEdit->SetWindowText(strTxt);
			int id=strTxt.GetLength();
			m_pEdit->SetSel(id,id);
		}
	}
}

void CTraceDlg::TraceText(const XU16*txt)
{
	if(!m_bOutput) return;
	if(m_lock.Lock())
	{
		TRACEINFO* pInfo=new TRACEINFO();
		pInfo->strTmp=CString(txt);
		pInfo->strTmp+="\r\n";
		pInfo->pNext=m_pTraceInfo;
		m_pTraceInfo=pInfo;
		m_lock.UnLock();
	}
	/*if(m_file.IsValid())
	{
		XString8 s(txt);
		m_file.Write(s.GetData(),s.GetLength());
		m_file.Write("\r\n",2);
	}
	if(!m_bOutput) return;
	if(!this->IsWindowVisible()) return;
	
	CString strTxt;
	m_pEdit->GetWindowText(strTxt);
	strTxt+=CString(txt);
	strTxt+="\r\n";
	m_pEdit->SetWindowText(strTxt);
	int id=strTxt.GetLength();
	m_pEdit->SetSel(id,id);*/
}

void CTraceDlg::OnCancel() 
{
	// TODO: Add extra cleanup here	
	ShowWindow(SW_HIDE);
}
void CTraceDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bOutput=!m_bOutput;
	//GetDlgItem(IDC_ENABLEOUT)->CheckWindow();
	CheckDlgButton(IDC_ENABLEOUT,m_bOutput?2:0);
}

void CTraceDlg::OnBnClickedClear()
{
	// TODO: 在此添加控件通知处理程序代码
	Clear();
}


