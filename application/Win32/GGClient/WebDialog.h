#pragma once
#include "webshow.h"


// CWebDialog 对话框

class CWebDialog : public CDialog
{
	DECLARE_DYNAMIC(CWebDialog)

public:
	virtual BOOL OnInitDialog();
	void ShowCnt();
	CWebDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWebDialog();

// 对话框数据
	enum { IDD = IDD_WEBDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWebshow m_webCnt;
};
