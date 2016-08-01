#pragma once


// CSetDialog 对话框

class CSetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDialog)

public:
	CSetDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDialog();

// 对话框数据
	enum { IDD = IDD_SET_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void setSrcPath(const CString& srcpath);
	const CString getSrcPath();
	void setSdkPath(const CString& sdkpath);
	const CString getSdkPath();
private:
	bool makeFile();
	bool makeConfig();
	void readConfig();
public:
	CString m_iSrcPath;			//源码路径
	CString m_iSdkPath;			//SDK路径
	long m_nKeyDate;			//签名有效时间
public:
	//源码路径按钮
	afx_msg void OnBnClickedButton1();
	//sdk路径按钮
	afx_msg void OnBnClickedButton2();
	//应用按钮
	afx_msg void OnBnClickedOk();

};
