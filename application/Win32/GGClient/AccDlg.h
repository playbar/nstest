#pragma once
#include "accenu.h"


// CAccDlg 对话框

class CAccDlg : public CDialog
{
	DECLARE_DYNAMIC(CAccDlg)

public:
	CAccDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAccDlg();

// 对话框数据
	enum { IDD = IDD_ACC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(void);

	CAccEnu m_accEnu;
};
