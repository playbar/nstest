#pragma once


// CTextDlg 对话框

class CTextDlg : public CDialog
{
	DECLARE_DYNAMIC(CTextDlg)

public:
	BOOL m_bOK;
	CTextDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTextDlg();

// 对话框数据
	enum { IDD = IDD_TEXTEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strText;
	virtual BOOL OnInitDialog(void);
};
