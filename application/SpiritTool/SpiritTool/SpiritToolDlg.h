
// SpiritToolDlg.h : 头文件
//

#pragma once

#include "PointerArray.h"
#include "SpiritData.h"
#include "SetDialog.h"
#include "ApkMake.h";
// CSpiritToolDlg 对话框
class CSpiritToolDlg : public CDialogEx , public CApkMake::MakeListener
{
// 构造
public:
	CSpiritToolDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CSpiritToolDlg();
// 对话框数据
	enum { IDD = IDD_SPIRITTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton2();

public:
	void onListen(int type,unsigned long data);

protected:
	CPointerArray<CSpiritData>	m_spiritList;	//精灵列表数据
public:
	CListBox m_iList;				//数据列表框
	CString m_iAppname;				//app名称
	CString m_iOutpath;				//输出路径
	CString m_iIconpath;			//图标路径
	CString m_iApkname;				//程序包名
	CString m_iPackagename;			//程序标识，主类包名
	CString m_iSpiritUA;			//精灵UA
	CString m_iChannelID;			//精灵渠道号
	CString m_iImgPath1;			//图片1路径
	CString m_iImgPath2;			//图片2路径
	CString m_iGifPath;				//gif图片路径
	CString m_iBWPath;				//百玩包图片路径
	BOOL m_bIsDesk;					//桌面图标
	CEdit m_iShow;					//消息显示窗口
private:
	CSetDialog m_iSetDialog;
public:
	//设置按钮
	afx_msg void OnBnClickedButtonset();
	//输出路径按钮
	afx_msg void OnBnClickedButtonch1();
	//图标路径按钮
	afx_msg void OnBnClickedButtonch2();
	//图片1按钮
	afx_msg void OnBnClickedButtonch3();
	//图片2按钮
	afx_msg void OnBnClickedButtonch4();
	//gif图片按钮
	afx_msg void OnBnClickedButtonch5();
	//百玩包路径按钮
	afx_msg void OnBnClickedButtonch6();
	//添加按钮
	afx_msg void OnBnClickedButtonadd();
	//listbox点击改变列表项焦点
	afx_msg void OnLbnSelchangeList1();
	//点击执行按钮
	afx_msg void OnBnClickedOk();
	//点击remove按钮
	afx_msg void OnBnClickedButton1();
};
