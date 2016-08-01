// MeminfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "MeminfoDlg.h"


// CMeminfoDlg 对话框

IMPLEMENT_DYNAMIC(CMeminfoDlg, CDialog)

CMeminfoDlg::CMeminfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMeminfoDlg::IDD, pParent)
{
	m_nID = 0;
	m_pData = &m_data[0];
	memset(m_nSize,0,sizeof(m_nSize));
}

CMeminfoDlg::~CMeminfoDlg()
{
}

void CMeminfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CMeminfoDlg, CDialog)
	ON_BN_CLICKED(IDC_RESET, &CMeminfoDlg::OnBnClickedReset)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, OnColumnclickList)
	ON_BN_CLICKED(IDC_LAST, &CMeminfoDlg::OnBnClickedLast)
	ON_BN_CLICKED(IDC_NEXT, &CMeminfoDlg::OnBnClickedNext)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
END_MESSAGE_MAP()

BOOL CMeminfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list.InsertColumn(0,_T("实例"),LVCFMT_LEFT,180);
	m_list.InsertColumn(1,_T("数量"),LVCFMT_LEFT,80);
	m_list.InsertColumn(2,_T("新增"),LVCFMT_LEFT,80);
	m_list.InsertColumn(3,_T("内存"),LVCFMT_LEFT,120);
	m_list.InsertColumn(4,_T("新增"),LVCFMT_LEFT,120);
	OnBnClickedReset();
	return TRUE;
}

// CMeminfoDlg 消息处理程序

void CMeminfoDlg::OnBnClickedReset()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_data.Release();
	m_nID++;
	if(m_nID>=MAXINFO)
		m_nID=0;
	int id=m_nID-1;
	if(id<0) id=MAXINFO-1;
	XSortMemName*pLast = &m_data[id];
	m_pData = &m_data[m_nID];
	m_pData->Release();
	m_pPlayer->AS3GetMemInfo(m_pData);
	int i,nc=m_pData->GetCount();
	int nTotal = 0;
	for(i=0;i<nc;i++)
	{
		_MEMINFO*pInfo=m_pData->GetAt(i);
		nTotal+=pInfo->nTotalSize;
//	int mi;
		int si=pLast->Index((void*)pInfo->strName);
		if(si>=0)
		{
			_MEMINFO*pOld = pLast->GetAt(si);
			pInfo->nCountInc = pInfo->nCount-pOld->nCount;
			pInfo->nSizeInc = pInfo->nTotalSize-pOld->nTotalSize;
		}
		else
		{
			pInfo->nCountInc = pInfo->nCount;
			pInfo->nSizeInc = pInfo->nTotalSize;
		}
	}
	m_nSize[m_nID] = nTotal;
	Show();
}


void CMeminfoDlg::ShowName()
{
	int i,nc=m_pData->GetCount();
	XSortMemName* pData=m_pData;
	
	CString strTmp;
	m_list.DeleteAllItems();
	int nTotal = 0;
	//int nTotalInc = 0;
	for(i=0;i<nc;i++)
	{
		_MEMINFO*pInfo=pData->GetAt(i);
		m_list.InsertItem(i,XString16(pInfo->strName));
		strTmp.Format(_T("%d"),pInfo->nCount);
		m_list.SetItemText(i,1,strTmp);
		strTmp.Format(_T("%d"),pInfo->nCountInc);
		m_list.SetItemText(i,2,strTmp);
		strTmp.Format(_T("%d"),pInfo->nTotalSize);
		m_list.SetItemText(i,3,strTmp);
		strTmp.Format(_T("%d"),pInfo->nSizeInc);
		m_list.SetItemText(i,4,strTmp);
		nTotal +=pInfo->nTotalSize;
		//nTotalInc +=pInfo->nSizeInc;
	}
	int id=m_nID-1;
	if(id<0) id=MAXINFO-1;
	int nTotalInc = m_nSize[m_nID]-m_nSize[id];
	strTmp.Format(_T("累计内存 %d(增加%d) k"),nTotal/1000,nTotalInc/1000);
	SetDlgItemText(IDC_INFO,strTmp);
}
void CMeminfoDlg::ShowCount()
{
	int i,nc=m_pData->GetCount();
	XSortMemCount sort;
	XSortMemCount*pData=&sort;
	for(i=0;i<nc;i++)
	{
		sort.Add(m_pData->GetAt(i));
	}
	CString strTmp;
	m_list.DeleteAllItems();
	int nTotal = 0;
	//int nTotalInc = 0;
	for(i=0;i<nc;i++)
	{
		_MEMINFO*pInfo=pData->GetAt(nc-i-1);
		m_list.InsertItem(i,XString16(pInfo->strName));
		strTmp.Format(_T("%d"),pInfo->nCount);
		m_list.SetItemText(i,1,strTmp);
		strTmp.Format(_T("%d"),pInfo->nCountInc);
		m_list.SetItemText(i,2,strTmp);
		strTmp.Format(_T("%d"),pInfo->nTotalSize);
		m_list.SetItemText(i,3,strTmp);
		strTmp.Format(_T("%d"),pInfo->nSizeInc);
		m_list.SetItemText(i,4,strTmp);
		nTotal +=pInfo->nTotalSize;
		//nTotalInc +=pInfo->nSizeInc;
	}
	int id=m_nID-1;
	if(id<0) id=MAXINFO-1;
	int nTotalInc = m_nSize[m_nID]-m_nSize[id];
	strTmp.Format(_T("累计内存 %d(增加%d) k"),nTotal/1000,nTotalInc/1000);
	SetDlgItemText(IDC_INFO,strTmp);
}

void CMeminfoDlg::ShowCountInc()
{
	int i,nc=m_pData->GetCount();
	XSortMemCountInc sort;
	XSortMemCountInc*pData=&sort;
	for(i=0;i<nc;i++)
	{
		sort.Add(m_pData->GetAt(i));
	}
	CString strTmp;
	m_list.DeleteAllItems();
	int nTotal = 0;
	//int nTotalInc = 0;
	for(i=0;i<nc;i++)
	{
		_MEMINFO*pInfo=pData->GetAt(nc-i-1);
		m_list.InsertItem(i,XString16(pInfo->strName));
		strTmp.Format(_T("%d"),pInfo->nCount);
		m_list.SetItemText(i,1,strTmp);
		strTmp.Format(_T("%d"),pInfo->nCountInc);
		m_list.SetItemText(i,2,strTmp);
		strTmp.Format(_T("%d"),pInfo->nTotalSize);
		m_list.SetItemText(i,3,strTmp);
		strTmp.Format(_T("%d"),pInfo->nSizeInc);
		m_list.SetItemText(i,4,strTmp);
		nTotal +=pInfo->nTotalSize;
		//nTotalInc +=pInfo->nSizeInc;
	}
	int id=m_nID-1;
	if(id<0) id=MAXINFO-1;
	int nTotalInc = m_nSize[m_nID]-m_nSize[id];
	strTmp.Format(_T("累计内存 %d(增加%d) k"),nTotal/1000,nTotalInc/1000);
	SetDlgItemText(IDC_INFO,strTmp);
}

void CMeminfoDlg::ShowSizeInc()
{
	int i,nc=m_pData->GetCount();
	XSortMemSizeInc sort;
	XSortMemSizeInc*pData=&sort;
	for(i=0;i<nc;i++)
	{
		sort.Add(m_pData->GetAt(i));
	}
	CString strTmp;
	m_list.DeleteAllItems();
	int nTotal = 0;
	//int nTotalInc = 0;
	for(i=0;i<nc;i++)
	{
		_MEMINFO*pInfo=pData->GetAt(nc-i-1);
		m_list.InsertItem(i,XString16(pInfo->strName));
		strTmp.Format(_T("%d"),pInfo->nCount);
		m_list.SetItemText(i,1,strTmp);
		strTmp.Format(_T("%d"),pInfo->nCountInc);
		m_list.SetItemText(i,2,strTmp);
		strTmp.Format(_T("%d"),pInfo->nTotalSize);
		m_list.SetItemText(i,3,strTmp);
		strTmp.Format(_T("%d"),pInfo->nSizeInc);
		m_list.SetItemText(i,4,strTmp);
		nTotal +=pInfo->nTotalSize;
		//nTotalInc +=pInfo->nSizeInc;
	}
	int id=m_nID-1;
	if(id<0) id=MAXINFO-1;
	int nTotalInc = m_nSize[m_nID]-m_nSize[id];
	strTmp.Format(_T("累计内存 %d(增加%d) k"),nTotal/1000,nTotalInc/1000);
	SetDlgItemText(IDC_INFO,strTmp);
}

void CMeminfoDlg::ShowSize()
{
	int i,nc=m_pData->GetCount();
	XSortMemSize sort;
	XSortMemSize*pData=&sort;
	for(i=0;i<nc;i++)
	{
		sort.Add(m_pData->GetAt(i));
	}
	CString strTmp;
	m_list.DeleteAllItems();
	int nTotal = 0;
	//int nTotalInc = 0;
	for(i=0;i<nc;i++)
	{
		_MEMINFO*pInfo=pData->GetAt(nc-i-1);
		m_list.InsertItem(i,XString16(pInfo->strName));
		strTmp.Format(_T("%d"),pInfo->nCount);
		m_list.SetItemText(i,1,strTmp);
		strTmp.Format(_T("%d"),pInfo->nCountInc);
		m_list.SetItemText(i,2,strTmp);
		strTmp.Format(_T("%d"),pInfo->nTotalSize);
		m_list.SetItemText(i,3,strTmp);
		strTmp.Format(_T("%d"),pInfo->nSizeInc);
		m_list.SetItemText(i,4,strTmp);
		nTotal +=pInfo->nTotalSize;
		//nTotalInc +=pInfo->nSizeInc;
	}
	int id=m_nID-1;
	if(id<0) id=MAXINFO-1;
	int nTotalInc = m_nSize[m_nID]-m_nSize[id];
	strTmp.Format(_T("累计内存 %d(增加%d) k"),nTotal/1000,nTotalInc/1000);
	SetDlgItemText(IDC_INFO,strTmp);
}

void CMeminfoDlg::OnColumnclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_nSortMode = pNMListView->iSubItem;
	Show();
	*pResult = 0;
}
void CMeminfoDlg::OnBnClickedLast()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nID--;
	if(m_nID<0)
		m_nID=MAXINFO-1;
	m_pData = &m_data[m_nID];
	
	Show();
}

void CMeminfoDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nID++;
	if(m_nID>=MAXINFO)
		m_nID = 0;
	m_pData = &m_data[m_nID];
	
	Show();
}

void CMeminfoDlg::Show()
{
	m_list.DeleteAllItems();
	switch(m_nSortMode)
	{
	default:
	case 0:
		ShowName();
		break;
	case 1:
		ShowCount();
		break;
	case 2:
		ShowCountInc();
		break;
	case 3:
		ShowSize();
		break;
	case 4:
		ShowSizeInc();
		break;
	}
}
void CMeminfoDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int id=m_list.GetSelectionMark();
	if(id>=0)
	{
		CString text=m_list.GetItemText(id,0);
		XString16 s16(text);
		XString8  s8(s16);
		XSeekItem item(s8);
		m_pPlayer->AS3SeekItem(&item);
		if(item.m_nSaves)
		{
			TCHAR strTemp[MAX_PATH],strFile[MAX_PATH];
			::GetTempPath(MAX_PATH,strTemp);
			::GetTempFileName(strTemp,_T("sr"),100,strFile);
			CFile file;
			if(!file.Open(strFile,CFile::modeWrite|CFile::modeCreate))
				return;
			int i;
			for(i=0;i<item.m_nSaves;i++)
			{
				int l=strlen(item.m_saves[i]);
				file.Write(item.m_saves[i],l);
				file.Write("\r\n",2);
			}
			//file.Write(pView->m_strSource,pView->m_strSource.GetLength());
			file.Close();
			XString8 strCmd="Notepad ";
			strCmd+=strFile;
			WinExec(strCmd,SW_SHOW);
		}
	}
	*pResult = 0;
}