#pragma once
#include "afxcmn.h"


// CDataDlg 对话框

class CDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataDlg)

public:
	CDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataDlg();

// 对话框数据
	enum { IDD = IDD_DATAPROC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetMax(int nMax);
	void SetProc(int nProc);
	CProgressCtrl m_proc;
};
