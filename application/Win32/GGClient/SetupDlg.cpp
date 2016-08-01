// SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GGClient.h"
#include "SetupDlg.h"
#include "GGClientDlg.h"
#include "TextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog


CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupDlg)
	m_strAccepts = _T("");
	m_strGateway = _T("");
	m_nProtocol = -1;
	m_nPort = 0;
	m_nTimeout = 0;
	m_strUserAgent = _T("");
	m_strProfile = _T("");
	m_strMobileType = _T("");
	m_nRetries = 0;
	m_bEnu = FALSE;
	m_bEnuACC = FALSE;
	//}}AFX_DATA_INIT
	//m_addBut.SetID(_T("IDI_ADD"));
	//m_delBut.SetID(_T("IDI_DEL"));
}


void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDlg)
	DDX_Control(pDX, IDC_BUTTON, m_butColor);
	//DDX_Control(pDX, IDC_MOBILELIST, m_list);
	//DDX_Control(pDX, IDC_DEL, m_delBut);
	//DDX_Control(pDX, IDC_ADD, m_addBut);
	DDX_Control(pDX, IDC_RETRYSPIN, m_retrySpin);
	DDX_Control(pDX, IDC_TIMESPIN, m_timeSpin);
	DDX_Control(pDX, IDC_ACTIVETEXT, m_activeText);
	DDX_Control(pDX, IDC_TEXTCOLOR, m_textColor);
	DDX_Control(pDX, IDC_LINKTEXT, m_linkText);
	DDX_Control(pDX, IDC_HITEXT, m_hiText);
	DDX_Control(pDX, IDC_HIBACK, m_hiBack);
	DDX_Control(pDX, IDC_BKCOLOR, m_bkColor);
	DDX_Text(pDX, IDC_ACCEPTS, m_strAccepts);
	DDX_Text(pDX, IDC_GATEWAY, m_strGateway);
	DDX_Text(pDX, IDC_MOBILETYPE,m_strMobileType);
	DDX_Radio(pDX, IDC_HTTP, m_nProtocol);
	DDX_Text(pDX, IDC_PORT, m_nPort);
	DDV_MinMaxUInt(pDX, m_nPort, 1, 65535);
	DDX_Text(pDX, IDC_TIMEOUT, m_nTimeout);
	DDV_MinMaxUInt(pDX, m_nTimeout, 0, 120);
	DDX_Text(pDX, IDC_USERAGENT, m_strUserAgent);
	DDX_Text(pDX, IDC_CHANNELID, m_strChannelID);
	DDX_Text(pDX, IDC_CLIENTID, m_strClientID);
	DDX_Text(pDX, IDC_PROFILE, m_strProfile);
	DDX_Text(pDX, IDC_RETRIES, m_nRetries);
	DDV_MinMaxUInt(pDX, m_nRetries, 0, 10);
	DDX_Check(pDX, IDC_ENU, m_bEnu);
	DDX_Check(pDX, IDC_ENUACC, m_bEnuACC);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_BROWNS,OnText)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg message handlers

void CSetupDlg::SetInfo(BROWNSINFO &info)
{
	m_bkColor.m_nColor=info.nColor[XCW_BACK]&0xffffff;
	m_textColor.m_nColor=info.nColor[XCW_TEXT]&0xffffff;
	m_activeText.m_nColor=info.nColor[XCW_ACTIVE]&0xffffff;
	m_hiText.m_nColor=info.nColor[XCW_SELECTTEXT]&0xffffff;
	m_hiBack.m_nColor=info.nColor[XCW_SELECTBACK]&0xffffff;
	m_linkText.m_nColor=info.nColor[XCW_LINK]&0xffffff;
	m_butColor.m_nColor=info.nColor[XCW_BUTTON]&0xffffff;
	m_nPort=info.nPort;
	m_bEnuACC=info.bEnuACC;
	m_bEnu=info.bEnuHand;
	m_nProtocol=info.nProtcol;
	m_nTimeout=info.nTimeout;
	m_strAccepts.Empty();
	for(int i=0;i<info.strAccepts.GetSize();i++)
	{
		m_strAccepts+=info.strAccepts[i];
		m_strAccepts+="\r\n";
	}
	m_strGateway=info.strGameway;
	m_strProfile=info.strProfile;
	m_strUserAgent=info.strUserAgent;
	m_strChannelID=info.strChannelID;
	m_strClientID=info.strClientID;
	m_nRetries=info.nRetries;
	m_strMobileType=info.strMobileType;
}

void CSetupDlg::GetInfo(BROWNSINFO &info)
{
	info.nColor[XCW_BACK]=m_bkColor.m_nColor|0xff000000;
	info.nColor[XCW_TEXT]=m_textColor.m_nColor|0xff000000;
	info.nColor[XCW_ACTIVE]=m_activeText.m_nColor|0xff000000;
	info.nColor[XCW_SELECTTEXT]=m_hiText.m_nColor|0xff000000;
	info.nColor[XCW_SELECTBACK]=m_hiBack.m_nColor|0xff000000;
	info.nColor[XCW_LINK]=m_linkText.m_nColor|0xff000000;
	info.nColor[XCW_BUTTON]=m_butColor.m_nColor|0xff000000;
	info.bEnuHand=m_bEnu;
	info.bEnuACC=m_bEnuACC;
	info.nPort=m_nPort;
	info.nProtcol=m_nProtocol;
	info.nTimeout=m_nTimeout;
	info.strChannelID=m_strChannelID;
	info.strClientID=m_strClientID;
	CStringArray list;
	CGGClientDlg::SplitString(m_strAccepts,list,'\n');
	info.strAccepts.RemoveAll();
	for(int i=0;i<list.GetSize();i++)
	  info.strAccepts.Add(list[i]);
	//info.strAccepts=m_strAccepts;
	info.strMobileType=m_strMobileType;
	info.strGameway=m_strGateway;
	info.strProfile=m_strProfile;
	info.strUserAgent=m_strUserAgent;
	info.nRetries=m_nRetries;

}


BOOL CSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_retrySpin.SetRange(0,10);
	m_timeSpin.SetRange(0,120);
	EnableItem();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSetupDlg::OnDefault() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox(_T("»Ö¸´Ä¬ÈÏÅäÖÃ?"),MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		BROWNSINFO info;
		CGGClientDlg::LoadConfig(info,TRUE);
		SetInfo(info);
		UpdateData(FALSE);
		Invalidate();
	}
}

void CSetupDlg::OnText()
{
	XWAPText text;
	m_pApply->MakeWAPText(text);
	CTextDlg dlg;
	XString16 s(text);
	dlg.m_strText=s;
	if(dlg.DoModal()==IDOK)
	{
		XString16 s(dlg.m_strText);
		XString8 txt(s);
		m_pApply->RemoveAllClick();
		m_pApply->RemoveAllMark();
		m_pApply->FromWAP(txt,XCHARSET_UTF8,m_pDraw);
	}
}

BOOL CSetupDlg::EnableItem()
{
	//CString strTxt;
	//m_list.GetWindowText(strTxt);
	//int id=m_list.FindString(0,strTxt);
	//m_addBut.EnableWindow(id<0&&!strTxt.IsEmpty());
	//m_delBut.EnableWindow(id>=0);
	return TRUE;
}

