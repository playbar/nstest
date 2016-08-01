#pragma once
#include "afxcmn.h"


// CFlashBar 对话框

class CFlashBar : public CDialog
{
	DECLARE_DYNAMIC(CFlashBar)

public:
	void EnableItem();
	CFlashBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFlashBar();
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_FLASHBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();
	virtual void OnClose();
	afx_msg void OnQuit();
	afx_msg void OnRestart();
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnRotate();
	afx_msg void OnRadio();
	afx_msg void OnScore();
	afx_msg void OnPay();
	DECLARE_MESSAGE_MAP()
public:
	XFlashView* m_pView;

	CSliderCtrl m_zoomBar;

	afx_msg void OnNMCustomdrawZoombar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureZoombar(NMHDR *pNMHDR, LRESULT *pResult);
};
