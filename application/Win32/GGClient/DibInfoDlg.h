#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DibShow.h"

// CDibInfoDlg 对话框

class CDibInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CDibInfoDlg)

public:
	XSWFPlayer* m_pPlayer;
	CDibInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDibInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIBINFO };

protected:
	void Release();
	int  m_nSortType;
	CDibShow m_show;
	_DIBINFO* m_pInfo;
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	void Reset();
	CListCtrl m_list;
	void AddList(bool bReset=true);
	
	afx_msg void OnBnClickedReset();
	afx_msg void OnLvnColumnclickList(NMHDR *pNMHDR, LRESULT *pResult);
};
