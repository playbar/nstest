// GGClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "GGClient.h"
#include "GGClientDlg.h"
#include "SetupDlg.h"
#include "PlayerDlg.h"
#include "XAcc.h"
#include "XFlashView.h"

#include "XSystem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
XString8 CGGClientDlg::m_strWorkPath;

#define FindKey(KEYS,KEY,ID)\
{		KEY.MakeLower();\
		for(int II=0;II<sizeof(KEYS)/sizeof(LPCSTR);II++)\
		{\
			CString V=KEYS[II];\
			V.MakeLower();\
			if(KEY==V) {ID=II;break;}\
		}\
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:	
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGGClientDlg dialog
CGGClientDlg* CGGClientDlg::instance=NULL;
CGGClientDlg::CGGClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGGClientDlg::IDD, pParent)
{
	instance = this;
	m_nTimer = 0;
	m_mousePoint.x = -1000;
	m_mousePoint.y = -1000;
	//{{AFX_DATA_INIT(CGGClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_pAnaDlg=NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hCursor=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_nActiveID=-1;
	m_bInitOK=false;
	m_bMove=XFALSE;
	m_nInfoTicks=0;
	m_bDown=XFALSE;
	m_nMode=0;
	m_nAccRotate=0;
	m_nAccRotateZ=30;
	m_bAcc=XFALSE;
	m_info.bEnuACC=XTRUE;
	m_info.nFontSize=2;
	m_bAccZ=XFALSE;
	m_nLockMode=ELK_NONE;
	m_bMouseDown=0;
	m_nPt2X=0;
	m_nPt2Y=0;
	m_pMemInfoDlg = NULL;
	m_pDibInfoDlg = NULL;
	//m_bEnuACC=XFALSE;

}

void CGGClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGGClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGGClientDlg, CDialog)
	//{{AFX_MSG_MAP(CGGClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_CONFIG, OnAppConfig)
	ON_WM_TIMER()
	ON_COMMAND(ID_APP_SOURCE, OnAppSource)
	ON_UPDATE_COMMAND_UI(ID_APP_SOURCE, OnUpdateAppSource)
	ON_UPDATE_COMMAND_UI(ID_APP_OUTPUT, OnUpdateAppOutput)
	ON_COMMAND(ID_APP_URL, OnAppUrl)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_APP_TREE, OnAppTree)
	ON_COMMAND(ID_APP_DEBUG, OnAppDebug)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_APP_MODE, OnAppMode)
	ON_COMMAND(ID_APP_FINDVAR, OnAppFindvar)
	ON_COMMAND(ID_APP_ANALY, OnAppAnaly)
	ON_COMMAND(ID_APP_OUTPUT,OnAppOutput)
	ON_COMMAND(ID_BINARY,OnAppBinary)
	ON_COMMAND(ID_APP_DIBINFO,OnDibInfo)
	ON_COMMAND(ID_APP_MEMINFO,OnMemInfo)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ACC, &CGGClientDlg::OnAcc)
	ON_UPDATE_COMMAND_UI(ID_ACC, &CGGClientDlg::OnUpdateAcc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGGClientDlg message handlers
/*class CCameraDlg : public CDialog
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = NULL)
	{
		
	}   // 标准构造函数
	virtual ~CCameraDlg(){}
	bool OpenTraceFile(const char* strWorkPath);

// 对话框数据
	enum { IDD = IDD_CAMERA };
	BOOL Create()
	{
		if(!CDialog::Create(IDD,NULL)) return FALSE;
		ShowWindow(SW_SHOW);		
		return TRUE;
	}	
	virtual CWnd* SetFocus()
	{
		return CDialog::SetFocus();
	}
protected:	
	DECLARE_MESSAGE_MAP()
};

IMPLEMENT_DYNAMIC(CCameraDlg, CDialog)

BEGIN_MESSAGE_MAP(CCameraDlg, CDialog)	
END_MESSAGE_MAP()

static CCameraDlg m_cameraDlg;*/

BOOL CGGClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//m_cameraDlg.Create();
	//XCameraDevice::m_hWnd=m_cameraDlg.m_hWnd;
	//XCameraDevice::m_hWnd=GetDlgItem(IDC_CAMERA)->m_hWnd;
	//XURL url;
	//url.SetURL("app://m.jqbar.com");
	LoadImage(m_trace.m_info.m_frame,XString8("gif@frame"));
	LoadImage(m_trace.m_info.m_frames,XString8("gif@frames"));
	LoadImage(m_trace.m_info.m_netrate,XString8("gif@netrate"));
	LoadImage(m_trace.m_info.m_memory,XString8("gif@memory"));
	m_trace.OpenTraceFile(m_strWorkPath);
	//int s=sizeof(XWindow);
	//s=sizeof(XEdit);

	m_nTick=0;
	CString strTxt;
	strTxt.LoadString(AFX_IDS_APP_TITLE);
	m_strTitle=XResource::LoadWString(XIDS_TITLE);
		//strTxt;
	SetWindowText(strTxt);

	//m_edit.Create(WS_CHILD|ES_AUTOHSCROLL,CRect(0,0,100,20),this,2000);
	//m_edit.ShowWindow(SW_HIDE);
	// Add "About..." menu item to system menu.
	//this->ModifyStyle(0,CS_IME);
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);


//	LoadConfig(m_info,TRUE);
	LoadConfig(m_info,FALSE);
	XWindow::m_nTextSize=m_info.nFontSize;

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_dataShow.Create(CDataDlg::IDD,this);
	//ShowDataProc(100);
	//m_dataShow.ShowWindow(SW_SHOW);
	//SetConfig();
	int W=CreateMobile();
	if(m_nMode)
		RotateArea(W);
	CreateACC(W);

	CRect &rect = m_info.sysRect[RC_WINDOW];
	m_view.Create(rect, &m_draw,this,m_strTitle,m_strWorkPath);
	m_view.Paint(m_draw);
	// TODO: Add extra initialization here
	SetTimer(2,50,NULL);

	m_bInitOK=TRUE;

	//XWindow::msHwnd = m_hWnd;
	//XWindow::msRect.top = rect.top;
	//XWindow::msRect.
	mpWinView = WindowView::sharedWindowView();
	mpWinView->CreateView( this->m_hWnd, rect.left, rect.top, rect.Width(), rect.Height() );

	//XSystem::m_strURL="http://bwtest.jqbar.com/product/AcceptData.aspx?val=9B69207BB65F9D7DBD543955481BD118E939837F602369C2CDEC257141C64A3FA12C38A330655D732F80CED05FA53F3116658F543088EAC3D543A22F1A4A25F373EB4932D6EB7E414AB80F3DF738DE5C01B841691E790D6D28F3DB5E2684268049D764933CA2CC1A8764A5FF0BC198F3329239D2D6D1F699E6479E496A1BD4B0&skipId=2";
	//ShowWeb();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGGClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGGClientDlg::OnPaint() 
{
	
	//CCDirector::sharedDirector()->drawScene();
	
	//drawNode->visit();
	//eglView->swapBuffers();
	//mpWinView->MakeCurrent( true );
	
	//mpWinView->MakeCurrent( false );
	

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	if(m_trace.GetSafeHwnd()==NULL)
	{
		CRect rect;
		GetWindowRect(rect);
		CreateTraceWindow(rect);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGGClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CGGClientDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//ccDrawLine( ccp( 10, 10), ccp( 200, 200 ) );
	//eglView->MakeCurrent( true );
	//drawNode->visit();
	//eglView->swapBuffers();
	//eglView->MakeCurrent( false );

	XRect rect;
	pDC->GetClipBox((CRect*)&rect);

	if(rect.IsRectEmpty()) return XTRUE;
	XGraphics g(*pDC);
	XPoint pt=rect.TopLeft();
	//m_draw.SetOrigin(0,0,XTRUE);
	//PaintACC(m_draw);
	//m_draw.Scroll(XPoint(0,0),XRect(20,20,200,200));

	g.Copy(m_draw,pt,rect);

	if(m_nActiveID>=0&&m_nActiveID!=RC_WINDOW)
	{
		CPoint ct=m_info.sysRect[m_nActiveID].CenterPoint();
		XPoint st(ct.x-(m_select.Width()>>1),
			      ct.y-(m_select.Height()>>1));
		g.DrawImage(m_select,st);
	}
	if(m_nLockMode&&m_bMouseDown)
	{
		XPoint pt(m_nPt2X-(m_select.Width()>>1),m_nPt2Y-(m_select.Height()>>1));
		g.DrawImage(m_select,pt);
	}

	return TRUE;
//	Region
	//return CDialog::OnEraseBkgnd(pDC);
}



BOOL CGGClientDlg::LoadText(XPCTSTR strFile, XString8 &str, BOOL bFile)
{
  if(bFile)
   {
	   XFile file;
	   if(!file.Open(strFile,XFile::XREAD|XFile::XBINARY)) return FALSE;
	   int l=file.GetLength();
	   if(l<=0) return FALSE;
	   //XString8 str;
	   str.SetSize(l+1);
	   file.Read(str,l);
	   //strTxt=str;
   }
   else
   {
	    //XString8 strFile;
		//str.Mid(strFile,id+1);
		//img.CreateFromResource(strFile);

		HRSRC hrsrc=::FindResourceA(NULL,strFile,"TXT");
		int nSize=SizeofResource(NULL,hrsrc);
		if(hrsrc==NULL) XFALSE;
		HGLOBAL hObj=::LoadResource(NULL,hrsrc);
		if(hObj==NULL) XFALSE;  
		void* pData=LockResource(hObj);
		//img.Append(pData,nSize);
		str.SetString((XPCTSTR)pData,nSize);
		UnlockResource(pData);
		//img.Create(hObj);
		//DeleteObject(hObj);
		//img.Final(XFALSE);
	   //return XResource::LoadText(str,strFile);
   }
   return TRUE;

}


static XPCTSTR _strKeysName[]={("System"),("Window"),("Call"),
		    ("Close"),("Left Soft"),
			("Right Soft"),("Menu"),("Clear"),
			("Left"),("Right"),("Up"),("Down"),
			("Enter"),("NUM0"),("NUM1"),
			("NUM2"),("NUM3"),("NUM4"),
			("NUM5"),("NUM6"),("NUM7"),
			("NUM8"),("NUM9"),("NUM+"),("NUM#"),("Phone"),("Mode"),("Select"),"ENUHAND","ENUACC","Font"};


BOOL CGGClientDlg::LoadConfig(BROWNSINFO&info,BOOL bDefault)
{
	
	//CString strTxt;
	XString8 str;
	if(bDefault)
		LoadText("UICONFIG",str,FALSE);
	else
	{
		XString8 strFile=m_strWorkPath;
		strFile+=("\\Config.txt");
		if(!LoadText(strFile,str,TRUE))
			LoadText(("UICONFIG"),str,FALSE);
	}
	XDom dom;
	dom.Reset(XCHARSET_GB2312,XCNT::CTEXT,XCNT::TVND_WAP_WML,XNULL);
	dom.PhaseXML((XU8*)str.GetData(),str.GetLength());
	XDomList list;
	dom.GetList(list,XDOM_SETVAR);
	info.bEnuHand=XFALSE;
	for(XU32 i=0;i<list.GetSize();i++)
	{
		XString8 strKey,strValue;
		if(!list[i]->GetAttrib(XEAB::NAME,strKey)) continue;
		if(!list[i]->GetAttrib(XEAB::VALUE,strValue)) continue;
		int id=strKey.IndexOf(_strKeysName,sizeof(_strKeysName)/sizeof(XPCTSTR));
		if(id<0) continue;
		switch(id)
		{
		case FONTSIZE:
			 info.nFontSize=strValue.ToInt();			 
			 break;
		case ENUACC:
			 info.bEnuACC=strValue.ToLogic();
			 break;
		case ENUHAND:
			 info.bEnuHand=strValue.ToLogic();	 			 
			 break;
		case SET_SELECT:
			 info.strSelect=strValue;
			 break;
		case SET_MOBILE:
			 info.strMobile=strValue;
			 break;
		case SET_MODE:
			 info.nMode=strValue.ToInt();
			 break;
		default:
			{
				XString8Array rcs;
				rcs.SplitString(strValue,',');
				if(rcs.GetSize()>=4)
				{
					info.sysRect[id].left=rcs[0].ToInt();
					info.sysRect[id].top=rcs[1].ToInt();
					info.sysRect[id].right=rcs[2].ToInt()+info.sysRect[id].left;
					info.sysRect[id].bottom=rcs[3].ToInt()+info.sysRect[id].top;
				}
			};
		}
	}
	/*CString strKey,strValue;
	int nID=-1,nTemp;
	strTxt=(XPCTSTR)str;
	while(GetKey(strTxt,strKey,strValue))
	{
		FindKey(_strKeys,strKey,nID);
		if(!strValue.IsEmpty())
		{
			switch(nID)
			{
			case KEY_HOST:
				 info.strHost=strValue;
				 break;
			case KEY_USERAGENT:
				 info.strUserAgent=strValue;
				 break;
			case KEY_PROFILE:
				 info.strProfile=strValue;
				 break;
			case KEY_ACCEPTS:
				 if(info.strAccepts.GetSize()==0)
					info.strAccepts.Add(strValue);
				 else
				 {
					BOOL bExist=FALSE;
					for(int i=0;i<info.strAccepts.GetSize();i++)
					{
						if(info.strAccepts[i]==strValue)
						{
							bExist=TRUE;
							break;
						}
					}
					if(!bExist) info.strAccepts.Add(strValue);
					//info.strAccepts+=_T("\r\n");
					//info.strAccepts+=strValue;
				 }
				 break;
			case KEY_HOME:
				 info.strHome=strValue;
				 break;
			case KEY_PROTOCOL:
				 nTemp=-1;
				 FindKey(_strProtocol,strValue,nTemp);
				 if(nTemp>=0)
					 info.nProtcol=nTemp;
				 break;
			case KEY_RETRIES:
				 info.nRetries=(BYTE)Atol(strValue,10);
				 break;
			case KEY_TIMEOUT:
				 info.nTimeout=(BYTE)Atol(strValue,10);
				 break;
			case KEY_PORT:
				 info.nPort=(WORD)Atol(strValue,10);
				 break;
			case KEY_GATEWAY:
				 info.strGameway=strValue;
				 break;
			case KEY_MOBILE:
				 info.strMobile=strValue;
				 break;
			case KEY_COLORS:
				{ CStringArray list;
				  SplitString(strValue,list,',');
				  for(int i=0;i<list.GetSize();i++)
				  {
					  if(i>=sizeof(info.nColor)/sizeof(COLORREF)) break;					  
					  if(list[i].GetLength()>0&&list[i][0]=='#')
					      info.nColor[i]=Atol(list[i],16);
					  else 
						  info.nColor[i]=Atol(list[i],10);
				  }
				}
				break;
			case KEY_RECT:
				{
					CStringArray list;
					SplitString(strValue,list,XCOLON);
					
					if(list.GetSize()>=2)
					{
						nTemp=-1;
						FindKey(_strKeysName,list[0],nTemp);
						if(nTemp>=0)
						{
							CString v=list[1];
							list.RemoveAll();
							SplitString(v,list,',');
							if(list.GetSize()>=4)
							{
							   info.sysRect[nTemp].left=Atol(list[0]);
							   info.sysRect[nTemp].top=Atol(list[1]);
							   info.sysRect[nTemp].right=Atol(list[2])+info.sysRect[nTemp].left;
							   info.sysRect[nTemp].bottom=Atol(list[3])+info.sysRect[nTemp].top;
							}
						}
					}
				}
				break;
			}
		}
	}*/
	return TRUE;
}

BOOL CGGClientDlg::GetKey(CString &strTxt, CString &strKey, CString &strValue, TCHAR c1, TCHAR c2)
{
   strKey.Empty();
   strValue.Empty();
   int id=strTxt.Find(c1);
   if(id>=0)
   {
	   if(id>0) strKey=strTxt.Left(id);
	   strTxt.Delete(0,id+1);
	   int id1=strKey.ReverseFind(_T('\n'));
	   if(id1>0)
		   strKey=strKey.Mid(id1+1);
	   strKey.TrimLeft();
	   strKey.TrimRight();
   }
   int id1=strTxt.Find(c2);
   if(id1>=0)
   {
	   int id2=strTxt.Find(c2,id1+1);
	   if(id2>=id1)
	   {
		   strValue=strTxt.Mid(id1+1,id2-id1-1);
		   strTxt.Delete(0,id2+1);
	   }
	   else
	   {
		   strValue=strTxt.Mid(id1+1);
		   strTxt.Empty();
	   }
   }
   return id>=0||id1>=0;

}

/*long CGGClientDlg::Atol(LPCTSTR v, BYTE nDec)
{
	 int nValue=0,j=0;
     while(1)
	 {
		  TCHAR c=v[j];
		  if(c==0) break;
		  j++;
	      nValue=nValue*nDec;
		  if(c>='0'&&c<='9') nValue+=(c-'0');
		  else if(c>='a'&&c<='f') nValue+=(c-'a'+10);
		  else if(c>='A'&&c<='F') nValue+=(c-'A'+10);
	 }
	 return nValue;
}*/

int CGGClientDlg::SplitString(CString &strTxt, CStringArray &list, TCHAR c)
{
	while(!strTxt.IsEmpty())
	{
		CString v;
		int id=strTxt.Find(c);
		if(id<0) {v=strTxt;strTxt.Empty();}
		else {v=strTxt.Left(id);strTxt.Delete(0,id+1);}
		v.TrimLeft();
		v.TrimRight();
		list.Add(v);
	}
	return list.GetSize();
}

//DEL void CGGClientDlg::SaveConfig(BROWNSINFO &info)
//DEL {
//DEL /*	CString strTxt,strColors;
//DEL 	for(int i=0;i<sizeof(info.nColor)/sizeof(COLORREF);i++)
//DEL 	{
//DEL 		CString strTmp;
//DEL 		strTmp.Format(_T("#%06x"),info.nColor[i]);
//DEL 		strTmp.MakeUpper();
//DEL 		if(!strColors.IsEmpty()) strColors+=_T(',');
//DEL 		strColors+=strTmp;
//DEL 	}
//DEL 	CString strAccepts;
//DEL 	for(i=0;i<info.strAccepts.GetSize();i++)
//DEL 	{
//DEL 		if(!strAccepts.IsEmpty())
//DEL 			strAccepts+="\r\n";
//DEL 		strAccepts+='"';
//DEL 		strAccepts+=info.strAccepts[i];
//DEL 		strAccepts+='"';
//DEL 	}
//DEL 	strTxt.Format(_T("%s=\"%s\"\r\n%s=\"%s\"\r\n%s=%s\r\n%s=\"%s\"\r\n%s=\"%s\"\r\n%s=\"%s\"\r\n%s=\"%d\"\r\n%s=\"%d\"\r\n%s=\"%d\"\r\n%s=\"%s\"\r\n%s=\"%s\"\r\n%s=\"%s\"\r\n%s="),
//DEL 				  _strKeys[KEY_USERAGENT],info.strUserAgent,
//DEL 				  _strKeys[KEY_PROFILE],info.strProfile,
//DEL 				  _strKeys[KEY_ACCEPTS],strAccepts,
//DEL 				  _strKeys[KEY_HOME],info.strHome,
//DEL 				  _strKeys[KEY_HOST],info.strHost,
//DEL 				  _strKeys[KEY_PROTOCOL],_strProtocol[info.nProtcol],
//DEL 				  _strKeys[KEY_RETRIES],info.nRetries,
//DEL 				  _strKeys[KEY_TIMEOUT],info.nTimeout,
//DEL 				  _strKeys[KEY_PORT],info.nPort,
//DEL 				  _strKeys[KEY_GATEWAY],info.strGameway,
//DEL 				  _strKeys[KEY_MOBILE],info.strMobile,
//DEL 				  _strKeys[KEY_COLORS],strColors,
//DEL 				  _strKeys[KEY_RECT]);
//DEL 	for(i=0;i<SYS_RECTS;i++)
//DEL 	{
//DEL 		CString strTmp;
//DEL 		strTmp.Format(_T("\"%s:%d,%d,%d,%d\"\r\n"),_strKeysName[i],
//DEL 					  info.sysRect[i].left,info.sysRect[i].top,
//DEL 					  info.sysRect[i].Width(),info.sysRect[i].Height());
//DEL 		if(i!=0) strTxt+=_T('\t');
//DEL 		strTxt+=strTmp;
//DEL 	}
//DEL 	CString strFile=m_strWorkPath+_T("\\Config.txt");
//DEL 	SaveText(strFile,strTxt);*/
//DEL 
//DEL }

//DEL BOOL CGGClientDlg::SaveText(XPCTSTR strFile, CString &strTxt)
//DEL {
//DEL    XString8 str(strTxt);
//DEL    XFile file;
//DEL    if(!file.Open(strFile,XFile::XWRITE|XFile::XCREATE)) return FALSE;
//DEL    file.Write(str,str.GetLength());
//DEL    return TRUE;
//DEL }

static int _nSysKeys[]={0,0,XK_CALL,
			 XK_CLOSE,XK_LEFTSOFT,
			 XK_RIGHTSOFT,
			 XK_MENU,
			 XK_CLEAR,
			 XK_LEFT,
			 XK_RIGHT,
			 XK_UP,
			 XK_DOWN,
			 XK_RETURN,
			 '0','1','2','3','4','5','6','7','8','9',
			 '+','#'};


void CGGClientDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nActiveID)
	{
	case RC_SYSTEM:
		{
			CMenu menu;
			menu.LoadMenu(IDR_SYSTEM);
			CMenu*pMenu=menu.GetSubMenu(0);
			ClientToScreen(&point);
			DeleteObject(m_hCursor);
			m_hCursor=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			SetCursor(m_hCursor);
			pMenu->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN,
							   point.x, point.y,
							   this);
			m_nActiveID=-1;
			Invalidate();
		}break;
	case RC_WINDOW:
		{
			CRect&rt=m_info.sysRect[RC_WINDOW];
			int cx=(rt.right-rt.left)/2+rt.left;
			int cy=(rt.bottom-rt.top)/2+rt.top;
			m_nDeltaX=(cx-point.x)*2;
			m_nDeltaY=(cy-point.y)*2;
			
			XPoint ptt(point.x,point.y);
			m_view.ThumbPoint(ptt);
			//m_view.MouseMove(ptt);
			OnMouseMove(ptt);
			XPoint ptt1(point.x,point.y);
			m_view.ThumbPoint(ptt1);
			OnMouseDown(ptt1);
			//m_view.MouseDown(ptt1);
		}
		SetCapture();
		break;
	case -1:
		m_downPoint=point; 
		m_nDownRotate=m_nAccRotate;
		m_nDownRotateZ=m_nAccRotateZ;
		m_bMove=XTRUE;
		SetCapture();
		break;
	default:
		
		//m_view.PostMessage(WM_MOBILE_KEY,m_nActiveID);
		{
			XKEYMSG msg={_nSysKeys[m_nActiveID],_nSysKeys[m_nActiveID],0,false,false,false};
			m_view.KeyDown(msg);
			m_nTick=XW_WAITTIME;
			m_bDown=XTRUE;
		}
		//Invalidate();
		break;
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CGGClientDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(GetCapture()==this) 
	//
		ReleaseCapture();
	if(m_bDown&&m_nActiveID>=0)
	{
		if(m_nActiveID!=RC_WINDOW)
		{
			XKEYMSG msg={_nSysKeys[m_nActiveID],_nSysKeys[m_nActiveID],0,false,false,false};
			m_view.KeyUp(msg);
		}
		m_bDown=FALSE;
		return;
	}
	m_bAcc=XFALSE;
	m_bDown=XFALSE;

	if(m_nActiveID==RC_WINDOW)
	{
		XPoint ptt(point.x,point.y);
		m_view.ThumbPoint(ptt);
		//m_view.MouseUp(XPoint(ptt));
		OnMouseUp(ptt);
	}
	//
	m_bMove=FALSE;
	CDialog::OnLButtonUp(nFlags, point);
}

void CGGClientDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//if(nFlags)
	{
		XPoint ptt(point.x,point.y);
		//m_view.ThumbPoint(ptt);
		//m_view.MouseMove(ptt);
		OnMouseMove(ptt);
	}
	if(GetCapture()==this)
	{
		if(m_bAcc&&m_info.bEnuACC)
		{
			int cx=m_accRect.Width()/2+m_accRect.left;
			int cy=m_accRect.Height()/2+m_accRect.top;
			if(m_bAccZ)
			{
				int dh=m_accRect.Height()/2;
				int ndh=-(point.y-m_downPoint.y)*90/dh;
				//if(ndh>90) ndh=90;
				//else if(ndh<-90) ndh=-90;
				m_nAccRotateZ=m_nDownRotateZ+ndh;
				if(m_nAccRotateZ<-90)
					m_nAccRotateZ=-90;
				else if(m_nAccRotateZ>90)
					m_nAccRotateZ=90;
			}
			else
			{
				double rd=::atan2((double)cy-m_downPoint.y,(double)m_downPoint.x-cx);
				double rn=::atan2((double)cy-point.y,(double)point.x-cx);
				m_nAccRotate=m_nDownRotate-(int)((rn-rd)*180/3.1415926);

				while(m_nAccRotate<0)
					m_nAccRotate+=360;
				while(m_nAccRotate>=360)
					m_nAccRotate-=360;
				if(m_nAccRotate<5&&m_nAccRotate>355)
					m_nAccRotate=0;
				if(m_nAccRotate>85&&m_nAccRotate<95)
					m_nAccRotate=90;
				if(m_nAccRotate>175&&m_nAccRotate<185)
					m_nAccRotate=180;
				if(m_nAccRotate>265&&m_nAccRotate<275)
					m_nAccRotate=270;
				
			}
			PaintACC(m_draw);
			InvalidateRect(m_accRect);
			//Invalidate();
			return;
		}
		if(!m_bMove) 
		{
			
			return;
		}
		CSize sz=point-m_downPoint;
		CRect rect;
		GetWindowRect(rect);
		rect.OffsetRect(sz);
		MoveWindow(rect);
		//RenderDraw();
		//Invalidate();
		return;
	}
	m_bAcc=m_accRect.PtInRect(point);
	if(m_bAcc)
	{
		int dx=((m_accRect.left+m_accRect.right)>>1)-point.x;
		if(dx<0) dx=-dx;
		if(dx<=12)
			m_bAccZ=XTRUE;
		else
			m_bAccZ=XFALSE;
	}
	//if(m_bAcc)
	//	int v=0;
	
	int nID=-1;
	for(int i=0;i<=RC_WELL;i++)
	{
		if(m_info.sysRect[i].PtInRect(point))
		{
			nID=i;
			break;
		}
	}
	if(m_nActiveID!=nID)
	{
		if(m_bDown)
		{
			XKEYMSG msg={_nSysKeys[m_nActiveID],_nSysKeys[m_nActiveID],0,false,false,false};
			m_view.KeyUp(msg);
		}
		m_bDown=XFALSE;
		m_nActiveID=nID;
		Invalidate();
//		RenderDraw();
		DeleteObject(m_hCursor);
		m_hCursor=m_nActiveID>=0?
			(m_nActiveID==RC_WINDOW?AfxGetApp()->LoadStandardCursor(IDC_ARROW):
			AfxGetApp()->LoadCursor(IDC_HANDPT)):
			AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		SetCursor(m_hCursor);
		Invalidate();
		return;
	}
	else if(nID<0)
	{
		m_bDown=XFALSE;
		//m_view.MouseMove(XPoint(point.x,point.y));
	 //point.x-=m_info.sysRect[RC_WINDOW].left;
	 //point.y-=m_info.sysRect[RC_WINDOW].top;

	 //m_view.OnMouseMove(nFlags,point);
	}
}

BOOL CGGClientDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(m_hCursor);
	return TRUE;
}

void CGGClientDlg::OnCancel() 
{
	// TODO: Add extra cleanup here	
	if(AfxMessageBox(_T("您确认退出系统?"),MB_YESNO|MB_ICONQUESTION)!=IDYES)
	 return;
	OnAppExit();
}

void CGGClientDlg::OnAppExit() 
{
	// TODO: Add your command handler code here
	m_bDown=XFALSE;
	if(m_pAnaDlg)
		CloseFlash();
	this->m_trace.ShowWindow(SW_HIDE);
	//if(AfxMessageBox(_T("您确认退出系统?"),MB_YESNO|MB_ICONQUESTION)!=IDYES)
	// return;
	m_view.Close();
//	SaveConfig(m_info);
	//m_view.Release();
	PostQuitMessage(0);	
}

void CGGClientDlg::OnAppAbout() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	Invalidate();
	dlg.DoModal();	
}

void CGGClientDlg::SetRgn(int w,int h)
{
	//return;

	int x, y;

	DWORD*pBuf=new DWORD[w*h];
	m_draw.GetBitmapData(pBuf,w*h*sizeof(DWORD));
	CRgn rgn;
	rgn.CreateRectRgn(0,0,0,0);
	for(y=0;y<h;y++)
	{
		int bx=-1;
		for(x=0;x<w;x++)
		{
		  XU32 c=pBuf[y*w+x];//m_draw.GetPixel(x,y);
		  if(c==0)
		  {
			if(bx>=0)
			{
				CRgn r;
				if(r.CreateRectRgn(bx,y,x,y+1))
				{
					rgn.CombineRgn(&rgn,&r,RGN_OR);
					r.DeleteObject();					
				}
				bx=-1;
			}
		  }
		  else
		  {
			  if(bx<0) bx=x;
		  }
		}
		if(bx>=0)
		{
			CRgn r;
			if(r.CreateRectRgn(bx,y,x,y+1))
			{
				rgn.CombineRgn(&rgn,&r,RGN_OR);
				r.DeleteObject();
			}
			bx=-1;
		}
	}
	delete pBuf;
	SetWindowRgn(rgn,XTRUE);//FALSE);
}

void CGGClientDlg::OnAppConfig() 
{
	// TODO: Add your command handler code here
	CSetupDlg dlg;
	dlg.m_pApply=m_view.GetClientApply();//m_clientApply;
	dlg.m_pDraw=m_view.GetDraw();
	m_view.GetConfig(m_info);
	dlg.SetInfo(m_info);
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetInfo(m_info);
		SetConfig();
	}
}



BOOL CGGClientDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bOK=CDialog::PreTranslateMessage(pMsg);
	/*XU32 nTimer = ::GetTickCount();
	if(nTimer>=50)
	{
		m_nTimer = nTimer;
		OnTimer(2);
	}*/
	//MSG msg=*pMsg;
	//TranslateMessage(&msg);
	//pMsg=&msg;
	if(pMsg->message==WM_KEYUP||pMsg->message==WM_KEYDOWN)
	{
		XKEYMSG msg={pMsg->wParam,pMsg->wParam,0,0,0,0};		
		msg.bShiftKey=GetKeyState(VK_SHIFT)>>7;
		msg.bCtrlKey=GetKeyState(VK_CONTROL)>>7;
		//if(msg.bShiftKey||msg.bCtrlKey)
		//	int v=0;
		switch(msg.nCharCode)
		{
		case VK_CONTROL: 
			 msg.nLocation=(pMsg->lParam&0x01000000)?2:1;break;
		case VK_SHIFT:
			 msg.nLocation=(pMsg->lParam&0x00100000)?2:1;break;
		default:
			 if(msg.nKeyCode>='A'&&msg.nKeyCode<='Z')
			 {
				 int vs=msg.bShiftKey+(GetKeyState(VK_CAPITAL)>>7);
				 if(vs==0||vs==2)
					 msg.nCharCode+='a'-'A';
			 }
		}
		if(pMsg->message==WM_KEYUP)
			m_view.KeyUp(msg);
		else
			m_view.KeyDown(msg);
	}
	//if(nKey>='A'&&nKey<='Z') nKey+='a'-'A';
	switch(pMsg->message)
	{
	case ACC_CMD_INIT:
		 if(m_acc.m_hWnd==NULL||!m_acc.IsWindowVisible())
			 OnAcc();
		 break;
	case WM_KEYUP:
		 break;
	case WM_KEYDOWN:
		 {
			 
			 //if(pMsg->wParam==VK_CONTROL&&
			//	pMsg->lParam&0x01000000)
			//	m_view.KeyDown(XK_RIGHTSOFT);
			// else
			 //if(pMsg->wParam=='R')
			 switch(pMsg->wParam)
			 {
			 case 'R':
				 SetMode(-1);
				 break;
			 case 113://F2
				 //if(!(GetKeyState(VK_SHIFT)>>7)) break;
				 if(!m_bMouseDown||m_nLockMode)
					 m_nLockMode=ELK_ENUMOVE;
				 break;
			 case 114://F3
				 //if(!(GetKeyState(VK_SHIFT)>>7)) break;
				 if(!m_bMouseDown||m_nLockMode != ELK_ENUZOOM )
				 {
					 m_nLockMode=ELK_ENUZOOM;
				 }
				 else
				 {
					 m_nLockMode = ELK_NONE;
				 }
				 break;
			 case 115:
				 if(m_nLockMode)
				 {
					XPoint pt(m_nPt2X,m_nPt2Y);
					m_view.MouseUp(pt,1);
				 }break;
			  case 116:
				 if(m_nLockMode)
				 {
					XPoint pt(m_nPt2X,m_nPt2Y);
					m_view.MouseDown(pt,1);
				 }break;
			 default:
				 m_nLockMode=ELK_NONE;
				 break;
			 }
			 //m_view.KeyDown(nKey);
		 }
		 break;
	case WM_CHAR:
		 m_view.Char(pMsg->wParam);
		 /*if(pMsg->hwnd==m_view.GetSafeHwnd())
		 {
			 BOOL bOK=CDialog::PreTranslateMessage(pMsg);
			 m_view.OnKey(pMsg);
			 return 1;
		 }
		 else if(pMsg->wParam==VK_RETURN)
		 {
			 PostMessage(WM_MOBILE_KEY,RC_GOGO);
			 return 1;
		 }*/
			//m_view.PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
		 break;
	}

	return bOK;
}

void CGGClientDlg::SetConfig()
{
	m_view.SetConfig(m_info);
	m_view.Invalidate();
}



void CGGClientDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CGGClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nInfoTicks<40)
	{
		m_nInfoTicks++;
	}
	else
	{
		m_nInfoTicks=0;
		m_trace.m_info.UpdateInfo();
		m_trace.UpdateInfo();
		//if(m_flashBar.GetSafeHwnd()&&m_flashBar.IsWindowVisible())
		//	m_flashBar.EnableItem();
	}
	if(m_bDown&&m_nActiveID>0&&m_nActiveID!=RC_WINDOW)
	{
		if(m_nTick>0) m_nTick--;
		else
		{
			m_nTick=XW_REPTIME;
			XKEYMSG msg={_nSysKeys[m_nActiveID],_nSysKeys[m_nActiveID],0,false,false,false};
			m_view.KeyDown(msg);
		}
	}
	m_view.TimeTick();

	if(m_info.bEnuACC)
	{
		double rt=m_nAccRotate*3.1415926/180;
		double rtz=m_nAccRotateZ*3.1415926/180;
		double sinv=sin(rt);
		double cosv=cos(rt);
		double sinz=sin(rtz);
		double cosz=cos(rtz);
		int x=(int)(10000*sinv);
		int y=(int)(10000*cosv*sinz);
		int z=(int)(10000*cosz);
		m_view.ACC(x,y,z);
	}


	CDialog::OnTimer(nIDEvent);
}



XBOOL CGGClientDlg::ContinueModal()
{
	m_view.OnIdle();
	return CDialog::ContinueModal();
}

void CGGClientDlg::PeekMessage()
{
	ASSERT(::IsWindow(m_hWnd)); // window must be created
	// for tracking the idle time state
	//MSG* pMsg = &AfxGetThread()->m_msgCur;
#if _MSC_VER<=1200
	MSG* pMsg = &AfxGetThread()->m_msgCur;
#else
	MSG* pMsg = AfxGetCurrentMessage();//&AfxGetThread()->AfxGetCurrentMessage();
#endif

	// acquire and dispatch messages until the modal state is done
	ContinueModal();

			// pump message, but quit on WM_QUIT
	if(::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		
		if (!AfxGetThread()->PumpMessage())
		{
			AfxPostQuitMessage(0);
			m_view.PostMsg(XWM_ENDDIALOG,0,0,XMS_NORMAL);
			return;
		}

	}			// show the window when certain special messages rec'd
}

void CGGClientDlg::LoadImage(XImage &img, XString8 &str)
{
	int id=str.Find('@');
	if(id>=0)
	{
		XString8 strFile;
		str.Mid(strFile,id+1);
		//img.CreateFromResource(strFile);

		HRSRC hrsrc=::FindResourceA(NULL,strFile,"GIF");
		int nSize=SizeofResource(NULL,hrsrc);
		if(hrsrc==NULL) return;
		HGLOBAL hObj=::LoadResource(NULL,hrsrc);
		if(hObj==NULL) return;  
		void* pData=LockResource(hObj);
		img.Append(pData,nSize);
		UnlockResource(pData);
		//img.Create(hObj);
		DeleteObject(hObj);
		img.Final(XFALSE);


	}
	else
	{
		XURL url;
		XString8 strPath=m_strWorkPath;
		strPath+="\\";
		url.SetURL(strPath);
		url.SetURL(str);
		img.Create(XString16(url.m_strURL));
	}
}

void CGGClientDlg::OnAppSource() 
{
	// TODO: Add your command handler code here
	m_view.ViewSource();	
}

void CGGClientDlg::OnUpdateAppSource(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
#ifndef _SOURCECODE
	pCmdUI->Enable(FALSE);
#endif
}

void CGGClientDlg::OnAppUrl() 
{
	// TODO: Add your command handler code here
	m_view.ViewURL();
}

BOOL CGGClientDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	XKEYMSG msg={XK_UP,XK_UP,0,false,false,false};
	if(zDelta>0)
		m_view.KeyDown(msg);
	else if(zDelta<0)
	{
		msg.nKeyCode=msg.nCharCode=XK_DOWN;
		m_view.KeyDown(msg);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CGGClientDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_info.bEnuACC)
	{
		if(m_accRect.PtInRect(point))
		{
			m_nAccRotate=0;
			PaintACC(m_draw);
			InvalidateRect(m_accRect);
			return;
		}
	}
	XPoint pt(point.x,point.y);
	m_view.DblClick(pt,0);
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CGGClientDlg::OnAppTree() 
{
	// TODO: Add your command handler code here
	m_view.ViewTree();
}

void CGGClientDlg::OnAppDebug() 
{
	// TODO: Add your command handler code here
	m_view.Layerout();
}

void CGGClientDlg::OnEditPaste() 
{
	// TODO: Add your command handler code here
	if(::IsClipboardFormatAvailable(CF_TEXT))
	{
			//BYTE* hNewDIB = NULL;
		if (OpenClipboard())
		{
		  BeginWaitCursor();
		  //hNewDIB = CopyHandle(::GetClipboardData(CF_TEXT));
		  //GetDocument()->PasteDIB(hNewDIB);		  
		  HANDLE h=::GetClipboardData(CF_TEXT);
		  if (h == NULL) return;
		  DWORD dwLen = ::GlobalSize((HGLOBAL) h);
		  XString8 str;
		  str.SetSize(dwLen+1);
		  void* lp     = ::GlobalLock((HGLOBAL) h);
		  memcpy(str.GetData(), lp, dwLen);		
		  ::GlobalUnlock(h);
		//}
		  CloseClipboard();
		  EndWaitCursor();
		  //Invalidate();
		  if(str.GetLength()>0)
		  {
			str.ConvertFrom(XCHARSET_GB2312);
			XString16 str16=str;  
			XPCWSTR pData=str16.GetData();
			for(XU32 i=0;i<str16.GetLength();i++)
				m_view.Char(pData[i]);
		  }

		}
	}

}

XBOOL CGGClientDlg::CreateMobile()
{
	XImage m;
	LoadImage(m,m_info.strMobile);
	LoadImage(m_select,m_info.strSelect);
	int cx=m.Width();
	int cy=m.Height();
	int W=cx;
	XPoint pt(0,0);

	
	if(m_nMode)
	{	  		
		int v=cx;
		cx=cy;
		cy=v;
		pt.x=-cy;//-10;
		//int w=m_accRect.Width();		
		//pt.y=100;//cy;//y+200;
		//pt.y=cy;
	}
	m_draw.Create(cx,cy);
	//RenderDraw();
	//XGraphics g(m_draw);
	m_draw.DrawImage(m,pt,m_nMode?-90:0);

	//PaintACC(m_draw);

	SetRgn(cx,cy);
	CRect rect;
	GetWindowRect(&rect);
	SetWindowPos(NULL,rect.left,rect.top,cx,cy,0);

	return W;
}

void CGGClientDlg::RotateArea(int W)
{
	//int W=m_nMode?m_draw.Height():m_draw.Width();
	for(int i=0;i<=RC_WELL;i++)
	{
		CRect&rect=m_info.sysRect[i];
		int w=rect.Width();
		int h=rect.Height();
		
		if(m_nMode)
		{
			int v=rect.left;
			rect.left=rect.top;
			rect.top=W-v-w+1;
		}
		else
		{
			int v=rect.left;
			rect.left=W-rect.top-h+1;
			rect.top=v;
		}
		rect.right=rect.left+h;
		rect.bottom=rect.top+w;
		//v=rect.right;
		//rect.right=rect.bottom;
		//rect.bottom=v;
		/*if(m_info.sysRect[i].PtInRect(point))
		{
			nID=i;
			break;
		}*/
	}
}

void CGGClientDlg::SetMode(int nMode)
{
	//return;
	if(nMode<0)
		nMode=!m_nMode;
	//else
	//	nMode = !nMode;
	if(m_nMode==nMode) 
		return;
	m_nMode=nMode;
	int W=CreateMobile();
	//Invalidate();
	
	RotateArea(W);
	CreateACC(W);
	XRect rect(m_info.sysRect[RC_WINDOW]);
	mpWinView->SetViewPos( rect.left, rect.top, rect.Width(), rect.Height());
	//Invalidate();
	m_view.MoveWindow(rect);
	
	Invalidate();
	//GetDesktopWindow()->Invalidate(XTRUE);//,XNULL,XTRUE);//,XNULL,XTRUE);
	//GetWindow()->Invalidate();
}

void CGGClientDlg::OnAppMode() 
{
	// TODO: Add your command handler code here
	SetMode(-1);
}

void CGGClientDlg::OnAppFindvar() 
{
	// TODO: Add your command handler code here
	m_view.ViewTextVars();
}

void CGGClientDlg::OpenFlash()
{
	mpWinView->ShowView( true );
	ShowFlashBar();
}

void CGGClientDlg::CloseFlash()
{
	mpWinView->ShowView( false );
	HideDataProc();
	HideFlashBar();
	if(m_pAnaDlg)
	{
		m_pAnaDlg->DestroyWindow();
		delete m_pAnaDlg;
		m_pAnaDlg=NULL;
	}
}

void CGGClientDlg::OnAppBinary()
{
	if(m_view.GetPlayer()==XNULL) return;
	XSWFPlayer*player=m_view.GetPlayer();
	CPlayerDlg dlg(player);
	dlg.DoModal();
}

void CGGClientDlg::OnMemInfo()
{
#ifdef _ANALY
	if(m_view.GetPlayer()==XNULL) return;
	//CMeminfoDlg dlg;
	//dlg.m_pPlayer = m_view.GetPlayer();
	//dlg.DoModal();
	if(m_pMemInfoDlg == NULL)
	{
		m_pMemInfoDlg = new CMeminfoDlg();
	}
	//CDibInfoDlg dlg;
	m_pMemInfoDlg->m_pPlayer = m_view.GetPlayer();
	//dlg.DoModal();
	if(!m_pMemInfoDlg->GetSafeHwnd())
	{
		m_pMemInfoDlg->Create(CMeminfoDlg::IDD,this);
	}
	m_pMemInfoDlg->ShowWindow(SW_SHOW);
#endif
}

void CGGClientDlg::OnDibInfo()
{
#ifdef _ANALY
	if(m_view.GetPlayer()==XNULL) return;
	if(m_pDibInfoDlg == NULL)
	{
		m_pDibInfoDlg = new CDibInfoDlg();
	}
	//CDibInfoDlg dlg;
	m_pDibInfoDlg->m_pPlayer = m_view.GetPlayer();
	//dlg.DoModal();
	if(!m_pDibInfoDlg->GetSafeHwnd())
	{
		m_pDibInfoDlg->Create(CDibInfoDlg::IDD,this);
	}
	else
		m_pDibInfoDlg->Reset();
	m_pDibInfoDlg->ShowWindow(SW_SHOW);
#endif
}

void CGGClientDlg::ShowWeb()
{
	if(!m_webShow.GetSafeHwnd())
	{
		m_webShow.Create(CWebDialog::IDD,this);
		//m_webShow.OnInitDialog();
	}
	m_webShow.ShowWindow(SW_SHOW);
	m_webShow.ShowCnt();
}

void CGGClientDlg::ShowFlashBar()
{
	if(!m_flashBar.GetSafeHwnd())
	{
		m_flashBar.Create(CFlashBar::IDD,this);
	}
	XFlashView*pView=this->m_view.GetActiveView()->GetFlashView();
	m_flashBar.m_pView = pView;
	m_flashBar.m_zoomBar.SetPos(pView->GetZoomPecent());
	m_flashBar.ShowWindow(SW_SHOW);
	m_flashBar.EnableItem();
}

void CGGClientDlg::HideFlashBar()
{
	if(!m_flashBar.GetSafeHwnd()) return;
	m_flashBar.m_pView = NULL;
	m_flashBar.ShowWindow(SW_HIDE);
}

void CGGClientDlg::OnAppAnaly() 
{
	// TODO: Add your command handler code here
	if(m_view.GetPlayer()==XNULL) return;
	if(m_pAnaDlg==NULL)
		m_pAnaDlg=new CAnalyDialog();
#ifdef _ANALY
	_XCALLENTRY*pRoot=::_ANGetRoot();
	//if(pRoot==XNULL) return;
	CAnalyDialog&dlg=*m_pAnaDlg;
	
		
	dlg.m_pView=&m_view;
	dlg.m_pPlayer=m_view.GetPlayer();
	dlg.m_pRoot=pRoot;
	//dlg.DoModal();
	if(!dlg.GetSafeHwnd())
		dlg.Create( CAnalyDialog::IDD,this );
	dlg.ShowWindow(SW_SHOW);
#endif
}

static XU32 _nLast=0;

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//CWinApp*pApp=AfxGetApp();
	
	//HINSTANCE h=::AfxGetInstanceHandle();
	//::GetProcessId((HandlepApp->m_nThreadID);
	//XSystem::SetProcessID(::GetProcessIdOfThread(pApp->m_hThread));
		//(XU32)pApp->m_nThreadID);
	//XU32 v=XSystem::GetUsedMemory();

	// TODO: Add extra initialization here
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	CString strFmt,strFreeMemory;
	strFmt.LoadString(IDS_MEMORY);
	int nNew=MemStat.dwAvailPhys/ 1024L;
	strFreeMemory.Format(strFmt, nNew,_nLast?(nNew-_nLast):_nLast);
	SetDlgItemText(IDC_OUTPUT,strFreeMemory);
	_nLast=nNew;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define _RTTX(XX,YY) (cx+(XX)*cosv+(YY)*sinv+0.5)
#define _RTTY(XX,YY) (cy+(XX)*sinv-(YY)*cosv+0.5)

void CGGClientDlg::PaintACC(XGraphics &g)
{
	if(!m_info.bEnuACC) return;
	g.SetOrigin(0,0);
	CRect rect;
	GetClientRect(rect);
	g.SetClip(XRect(0,0,rect.Width(),rect.Height()));
	g.SetBackColor(0xffffffff);
	g.FillRectangle(m_accRect);
	int w=m_accRect.Width();
	int h=m_accRect.Height();
	int cx=w/2+m_accRect.left;
	int cy=h/2+m_accRect.top;
	
	g.SetColor(0xffefefef);
	g.DrawLine(cx-w/2,cy,cx+w/2,cy);
	g.DrawLine(cx,cy-h/2,cx,cy+h/2);

	double rt=m_nAccRotate*3.1415926/180;
	double rtz=m_nAccRotateZ*3.1415926/180;
	double sinv=sin(rt);
	double cosv=cos(rt);
	double sinz=sin(rtz);
	//XString16 str("ABC");
	//str.FromInt(m_nAccRotate);
	g.SetColor(m_nAccRotate?0xffaf0000:0xff000000);
//	g.DrawString(str,3,3);
	{
		int ww=w*3/8;
		int hh=(int)(h*3*sinz/12);
		int x=_RTTX(-ww,-hh);
		int y=_RTTY(-ww,-hh);
		int x1=_RTTX(ww,-hh);
		int y1=_RTTY(ww,-hh);
		g.DrawLine(x,y,x1,y1);
		x=x1;y=y1;
		x1=_RTTX(ww,hh);
		y1=_RTTY(ww,hh);
		g.DrawLine(x,y,x1,y1);
		x=x1;y=y1;
		x1=_RTTX(-ww,hh);
		y1=_RTTY(-ww,hh);
		g.DrawLine(x,y,x1,y1);
		x=x1;y=y1;
		x1=_RTTX(-ww,-hh);
		y1=_RTTY(-ww,-hh);
		g.DrawLine(x,y,x1,y1);
		ww-=6;
		hh-=4;
		int ww1=ww-4;
		x=_RTTX(-ww,-hh);
		y=_RTTY(-ww,-hh);
		x1=_RTTX(ww1,-hh);
		y1=_RTTY(ww1,-hh);
		g.DrawLine(x,y,x1,y1);
		x=x1;y=y1;
		x1=_RTTX(ww1,hh);
		y1=_RTTY(ww1,hh);
		g.DrawLine(x,y,x1,y1);
		x=x1;y=y1;
		x1=_RTTX(-ww,hh);
		y1=_RTTY(-ww,hh);
		g.DrawLine(x,y,x1,y1);
		x=x1;y=y1;
		x1=_RTTX(-ww,-hh);
		y1=_RTTY(-ww,-hh);
		g.DrawLine(x,y,x1,y1);

	}

	g.DrawFrame(m_accRect,0xFF7f7f7f);
}

void CGGClientDlg::CreateACC(int cx)
{
	if(!m_info.bEnuACC) return;
	int w=cx/4;
	int br=cx/12;
	if(w+br>m_info.sysRect[RC_WINDOW].top-10)
		w=m_info.sysRect[RC_WINDOW].top-10-br;
	m_accRect.right=cx-br;
	m_accRect.left=cx-br-w;
	m_accRect.top=br;
	m_accRect.bottom=m_accRect.top+w;

	if(m_nMode)
	{	  
		w=cx/4;
		if(w+br>m_info.sysRect[RC_WINDOW].left-10)
			w=m_info.sysRect[RC_WINDOW].left-10-br;
		m_accRect.left=br;
		m_accRect.top=br;
		m_accRect.right=m_accRect.left+w;
		m_accRect.bottom=m_accRect.top+w;
	}
	PaintACC(m_draw);
}

void CGGClientDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	if(m_bInitOK)
	{
		XWindow*pWnd=m_view.GetActiveView();
		if(pWnd)
		{
			pWnd->SetFocus(nState);
			
		}
	}
	/*if(nState)
		m_view.SetFocus(XTRUE);
	else
		m_view.SetFocus(XFALSE);*/
	// TODO: Add your message handler code here
	
}

void CGGClientDlg::OnUpdateAppOutput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_trace.GetSafeHwnd()==NULL||!m_trace.IsWindowVisible())
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(2);
}

void CGGClientDlg::OnMouseDown(XPoint&pt)
{
	m_bMouseDown=1;
	m_view.MouseDown(XPoint(pt),0);
	switch(m_nLockMode)
	{
	case ELK_NONE:		 
		 break;
	case ELK_ENUMOVE:
	case ELK_ENUZOOM:
		{
			CRect&rt=m_info.sysRect[RC_WINDOW];
			int cx=(rt.right-rt.left)/2+rt.left;
			int cy=(rt.bottom-rt.top)/2+rt.top;
			int dx=cx-pt.x;
			int dy=cy-pt.y;
			m_nPt2X=cx+dx;
			m_nPt2Y=cy+dy;
			//InvalidateRect(CRect(m_nPt2X-10,m_nPt2X-10,m_nPt2X+10,m_nPt2X+10));
			Invalidate();
			XPoint ptt(cx+dx,cy+dy);
			m_nDeltaX = ptt.x-pt.x;
			m_nDeltaY = ptt.y-pt.y;
			m_view.MouseDown(ptt,1);
			
		}
		 break;
	}
}
void CGGClientDlg::OnMouseMove(XPoint&pt)
{
	//if(m_bMouseDown)
	if(m_mousePoint.x!=pt.x||
	   m_mousePoint.y!=pt.y)
		m_view.MouseMove(XPoint(pt),0);
	m_mousePoint = pt;
	if(!m_bMouseDown) return;
	switch(m_nLockMode)
	{
	case ELK_NONE:		 
		 break;
	case ELK_ENUMOVE:
		{
			XPoint ptt(pt.x+m_nDeltaX,pt.y+m_nDeltaY);
			m_nPt2X=pt.x+m_nDeltaX;
			m_nPt2Y=pt.y+m_nDeltaY;
			InvalidateRect(CRect(m_nPt2X-10,m_nPt2X-10,m_nPt2X+10,m_nPt2X+10));
			m_view.MouseMove(ptt,1);
			
		}break;
	case ELK_ENUZOOM:
		{
			CRect&rt=m_info.sysRect[RC_WINDOW];
			int cx=(rt.right-rt.left)/2+rt.left;
			int cy=(rt.bottom-rt.top)/2+rt.top;
			int dx=cx-pt.x;
			int dy=cy-pt.y;
			m_nPt2X=cx+dx;
			m_nPt2Y=cy+dy;
			//InvalidateRect(CRect(m_nPt2X-10,m_nPt2X-10,m_nPt2X+10,m_nPt2X+10));
			Invalidate();
			XPoint ptt(cx+dx,cy+dy);
			m_view.MouseMove(ptt,1);
			//m_nDeltaX=dx*2;
			//m_nDeltaY=dy*2;
		}break;
	}
}
void CGGClientDlg::OnMouseUp(XPoint&pt)
{
	m_view.MouseUp(XPoint(pt),0);
	if(!m_bMouseDown) return;
	m_bMouseDown=0;
	switch(m_nLockMode)
	{
	case ELK_NONE:		 
		 break;
	case ELK_ENUMOVE:
		{
			XPoint ptt(pt.x+m_nDeltaX,pt.y+m_nDeltaY);
			m_nPt2X=pt.x+m_nDeltaX;
			m_nPt2Y=pt.y+m_nDeltaY;
			m_view.MouseUp(ptt,1);
		}break;
	case ELK_ENUZOOM:
		{
			CRect&rt=m_info.sysRect[RC_WINDOW];
			int cx=(rt.right-rt.left)/2+rt.left;
			int cy=(rt.bottom-rt.top)/2+rt.top;
			int dx=cx-pt.x;
			int dy=cy-pt.y;
			m_nPt2X=cx+dx;
			m_nPt2Y=cy+dy;
			//InvalidateRect(CRect(m_nPt2X-10,m_nPt2X-10,m_nPt2X+10,m_nPt2X+10));
			Invalidate();
			XPoint ptt(cx+dx,cy+dy);
			m_nPt2X=cx+dx;
			m_nPt2Y=cy+dy;
			m_view.MouseUp(ptt,1);
			m_nDeltaX=dx*2;
			m_nDeltaY=dy*2;
		}break;	
	}
	
}

void CGGClientDlg::OnAppOutput()
{
	BOOL bClear=m_trace.IsWindowVisible();
	m_trace.ShowWindow(SW_SHOW);
	if(bClear)
		m_trace.Clear();
}
void CGGClientDlg::OnAcc()
{
	// TODO: 在此添加命令处理程序代码
	if(m_acc.GetSafeHwnd()==NULL)
	{
		m_acc.Create();
	}
	else
	{
		if(m_acc.IsWindowVisible())
			m_acc.ShowWindow(SW_HIDE);
		else
			m_acc.ShowWindow(SW_SHOW);
	}
}

void CGGClientDlg::OnUpdateAcc(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(m_acc.GetSafeHwnd()==NULL||!m_acc.IsWindowVisible())
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(2);
}

void CGGClientDlg::ShowDataProc(int nMax)
{
	CRect pr,cr;
	this->GetWindowRect(pr);
	m_dataShow.GetWindowRect(cr);
	int x=(pr.Width()-cr.Width())/2+pr.left;
	int y=(pr.Height()-cr.Height())/2+pr.top;
	m_dataShow.SetWindowPos(NULL,x,y,0,0,SWP_NOSIZE);
	m_dataShow.ShowWindow(SW_SHOW);
	m_dataShow.SetMax(nMax);
}
void CGGClientDlg::SetDataProc(int nProc)
{
	m_dataShow.SetProc(nProc);
}
void CGGClientDlg::HideDataProc()
{
	m_dataShow.ShowWindow(SW_HIDE);
}