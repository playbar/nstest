// DibInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "DibInfoDlg.h"


// CDibInfoDlg 对话框

IMPLEMENT_DYNAMIC(CDibInfoDlg, CDialog)

CDibInfoDlg::CDibInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDibInfoDlg::IDD, pParent)
{
	m_pInfo = NULL;
	m_nSortType = 0;
}

CDibInfoDlg::~CDibInfoDlg()
{
	Release();
}

void CDibInfoDlg::Release()
{
	m_list.DeleteAllItems();
	_DIBINFO* pInfo = m_pInfo;
	while(pInfo)
	{
		_DIBINFO*pNext = pInfo->pNext;
		if(pInfo->pDib)
			delete pInfo->pDib;
		delete pInfo;
		pInfo = pNext;
	}
	m_pInfo = NULL;
}

void CDibInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_PREV, m_show);
}

void CDibInfoDlg::Reset()
{
	Release();
	AddList();
}


BEGIN_MESSAGE_MAP(CDibInfoDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDD_RESET, &CDibInfoDlg::OnBnClickedReset)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST, &CDibInfoDlg::OnLvnColumnclickList)
END_MESSAGE_MAP()

BOOL CDibInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pInfo = m_pPlayer->AS3GetDibInfo();
	m_list.InsertColumn(0,_T("图像"),LVCFMT_LEFT,160);
	m_list.InsertColumn(1,_T("显示"),LVCFMT_LEFT,80);
	m_list.InsertColumn(2,_T("引用数"),LVCFMT_LEFT,80);
	m_list.InsertColumn(3,_T("时间"),LVCFMT_LEFT,100);
	m_list.InsertColumn(4,_T("类型"),LVCFMT_LEFT,80);
	m_list.InsertColumn(5,_T("大小"),LVCFMT_LEFT,80);
	AddList();
	return TRUE;
}


class XSortInfo:public XSort
{
public:
	XSortInfo(int nType)
	{		
		m_nType = nType;
	}
	~XSortInfo()
	{
		//Release();
	}
	bool IsEmpty()
	{
		return m_data.GetSize()<=0;
	}

	
	void Add(_DIBINFO*info)
	{
		if(info == NULL) return;
		int mi=0;
		int si=Index(GetInfoData(info),mi);
		m_data.InsertAt(mi,info);
		//_MEMINFO*pInfo=NULL;
	}
	
	void RemoveAt(int index)
	{
		m_data.RemoveAt(index);
	}

	XU32 GetCount(){return m_data.GetSize();}
	_DIBINFO* GetAt(XINT i)
	{
		return m_data[i];
	}
protected:
	XINT Compare(void*iSrt,void*iDst,XU8 nMode)
	{
		return -CompareUInt((XU32)iSrt,(XU32)iDst);
	}
	void* GetInfoData(_DIBINFO*info)
	{
		switch(m_nType)
		{
		default:
			return (void*)info->pDib->Width();
		case 1:
			return (void*)info->nCacheTime;
		case 2:
			return (void*)info->nLockCount;
		case 3:
			return (void*)info->nLastTime;
		case 4:
			return (void*)info->nType;
		case 5:
			return (void*)info->pDib->m_nDataLength;
		}
	}
	void* GetData(XU32 nID)
	{
		if(nID>=GetCount()) return XNULL;
		return GetInfoData(m_data[nID]);
	}
	int m_nType;
	XVector<_DIBINFO*> m_data;
	
};
// CDibInfoDlg 消息处理程序

void CDibInfoDlg::AddList(bool bReset)
{
	this->m_show.SetDib(NULL);
	int iItem = 0;
	if(bReset||!m_pInfo)
		m_pInfo = m_pPlayer->AS3GetDibInfo();
	_DIBINFO*pInfo=m_pInfo->pNext;
	XU32 nLength  = 0;
	const PCTSTR _strType[]={_T("纹理"),_T("离屏"),_T("文字"),_T("形状"),_T("图片")};
	CString strTmp;
	XSortInfo infos(m_nSortType);
	while(pInfo)
	{
		infos.Add(pInfo);
		pInfo=pInfo->pNext;
	}
	for(int i=0;i<infos.GetCount();i++)
	{
		pInfo = infos.GetAt(i);
		strTmp.Format(_T("(%d X %d)"),pInfo->pDib->Width(),pInfo->pDib->Height());
		m_list.InsertItem(iItem,strTmp);
		strTmp.Format(_T("%d"),pInfo->nCacheTime);
		m_list.SetItemText(iItem,1,strTmp);//_strType[pInfo->nType]);
		strTmp.Format(_T("%d"),pInfo->nLockCount);
		m_list.SetItemText(iItem,2,strTmp);
		strTmp.Format(_T("%d"),pInfo->nUseTime);
		m_list.SetItemText(iItem,3,strTmp);
		//strTmp.Format(_T("%d"),
		m_list.SetItemText(iItem,4,_strType[pInfo->nType]);//pInfo->isAttach?_T("是"):_T("否"));
		strTmp.Format(_T("%d"),pInfo->pDib->m_nDataLength);
		m_list.SetItemText(iItem,5,strTmp);
		m_list.SetItemData(iItem,(DWORD)pInfo);
		iItem++;
		nLength+=pInfo->pDib->m_nDataLength;
		pInfo = pInfo->pNext;
	}
	strTmp.Format(_T("一共 %d k数据"),nLength/1000);
	SetDlgItemText(IDC_INFO,strTmp);
}

void CDibInfoDlg::OnClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
	int iItem = m_list.GetSelectionMark();
	if(iItem<0||iItem>=m_list.GetItemCount()) return;
	_DIBINFO* pInfo = (_DIBINFO*)m_list.GetItemData(iItem);
	if(pInfo == NULL) return;

	m_show.SetDib(pInfo->pDib);
}


void CDibInfoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	Release();	
	CDialog::OnClose();
}
void CDibInfoDlg::OnBnClickedReset()
{
	// TODO: 在此添加控件通知处理程序代码
	Reset();
}

void CDibInfoDlg::OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMListView = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_nSortType = pNMListView->iSubItem;
	this->m_list.DeleteAllItems();
	AddList(false);
	*pResult = 0;
}
