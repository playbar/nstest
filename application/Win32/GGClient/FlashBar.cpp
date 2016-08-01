// FlashBar.cpp : 实现文件
//

#include "stdafx.h"
#include "GGClient.h"
#include "FlashBar.h"
#include "XFlashView.h"
#include "GGClientDlg.h"

// CFlashBar 对话框

IMPLEMENT_DYNAMIC(CFlashBar, CDialog)

CFlashBar::CFlashBar(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashBar::IDD, pParent)
	, m_pView(NULL)
{

}

CFlashBar::~CFlashBar()
{
}

void CFlashBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZOOMBAR, m_zoomBar);
}

void CFlashBar::OnClose()
{
	return;
}

BOOL CFlashBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_zoomBar.SetRange(0,400);
	m_zoomBar.SetPos(0);
	//(CGGClientDlg*)
	CheckRadioButton(IDC_RADIO1,IDC_RADIO3,((CGGClientDlg*)GetParent())->GetLockMode()+IDC_RADIO1);
	//SetTimer(2,1000,onTimer);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlashBar::EnableItem()
{
	if(m_pView)
		m_zoomBar.SetPos(m_pView->GetZoomPecent());
	GetDlgItem(IDC_QUIT)->EnableWindow(m_pView!=NULL);
	GetDlgItem(IDC_RESTART)->EnableWindow(m_pView!=NULL&&m_pView->CanRestart());
	GetDlgItem(IDC_ZOOMIN)->EnableWindow(m_zoomBar.GetPos()>0);
	GetDlgItem(IDC_ZOOMOUT)->EnableWindow(m_zoomBar.GetPos()<400);
	GetDlgItem(IDC_SCORE)->EnableWindow(m_pView&&m_pView->CanSendScore());
	GetDlgItem(IDC_PAY)->EnableWindow(m_pView&&m_pView->CanPay());
	m_zoomBar.EnableWindow(m_pView!=NULL);
	//GetDlgItem(
}

void CFlashBar::OnCancel() 
{
	// TODO: Add extra cleanup here	
	//if(AfxMessageBox(_T("您确认退出系统?"),MB_YESNO|MB_ICONQUESTION)!=IDYES)
	 return;
	//OnAppExit();
}

BEGIN_MESSAGE_MAP(CFlashBar, CDialog)
	ON_BN_CLICKED(IDC_QUIT, OnQuit)
	ON_BN_CLICKED(IDC_RESTART,OnRestart)
	ON_BN_CLICKED(IDC_ZOOMIN,OnZoomin)
	ON_BN_CLICKED(IDC_ZOOMOUT,OnZoomout)
	ON_BN_CLICKED(IDC_ROTATE,OnRotate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ZOOMBAR, &CFlashBar::OnNMCustomdrawZoombar)
	ON_BN_CLICKED(IDC_RADIO1,OnRadio)
	ON_BN_CLICKED(IDC_RADIO2,OnRadio)
	ON_BN_CLICKED(IDC_RADIO3,OnRadio)
	ON_BN_CLICKED(IDC_SCORE,OnScore)
	ON_BN_CLICKED(IDC_PAY,OnPay)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ZOOMBAR, &CFlashBar::OnNMReleasedcaptureZoombar)
END_MESSAGE_MAP()


void CFlashBar::OnPay()
{
	if(!m_pView) return;
	m_pView->DoPay();
}

void CFlashBar::OnScore()
{
	if(!m_pView) return;
	m_pView->SendScore();
}

void CFlashBar::OnZoomin()
{
	if(!m_pView) return;
	if(!m_pView->CanZoomin())
		return;
	m_pView->ZoomIn();
	m_pView->m_player->UpdateAll();
}

void CFlashBar::OnZoomout()
{
	if(!m_pView) return;
	if(!m_pView->CanZoomout())
		return;
	m_pView->ZoomOut();
	m_pView->m_player->UpdateAll();
}

void CFlashBar::OnRestart()
{
	if(!m_pView||!m_pView->CanRestart()) return;
	m_pView->Restart();
}

void CFlashBar::OnRadio()
{
	((CGGClientDlg*)GetParent())->SetLockMode(GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO3)-IDC_RADIO1);
}

void CFlashBar::OnQuit()
{
	if(!m_pView) return;
	m_pView->CloseFlash();
}

void CFlashBar::OnRotate()
{
	CGGClientDlg* pDlg = (CGGClientDlg*)GetParent();
	pDlg->SetMode(-1);
}

// CFlashBar 消息处理程序





void CFlashBar::OnNMCustomdrawZoombar(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(((CGGClientDlg*)GetParent())->GetLockMode())
		return;
	if(m_pView)
	{
		int oldV=m_pView->GetZoomPecent();
		int newV=m_zoomBar.GetPos();
		if(newV!=oldV)
		{
			m_pView->SetZoomPecent(newV);
		}
	}
}

void CFlashBar::OnNMReleasedcaptureZoombar(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_pView)
		m_pView->m_player->UpdateAll();
	*pResult = 0;
}
