// MobileView.cpp: implementation of the CMobileView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GGClient.h"
#include "MobileView.h"
#include "GGClientDlg.h"
#include "Dom.h"
#include "TextDlg.h"
#include "XFlashView.h"
#include "..\..\..\Src\Base\win32\XCameraDevice.h"
#include "XAcc.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CTextDlg txtDlg;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMobileView::CMobileView()
{
   m_nCurType=0;
   m_bEnuHand=XFALSE;
}

CMobileView::~CMobileView()
{

}

class CCameraDlg : public CDialog
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
		ShowWindow(SW_HIDE);		
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

static CCameraDlg* m_pCameraDlg=NULL;

void CMobileView::StartCall()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	m_pCameraDlg = new CCameraDlg();
	m_pCameraDlg->Create();
	XCameraDevice::m_hWnd=m_pCameraDlg->m_hWnd;
	XAcc::m_hMainWnd=CGGClientDlg::instance->m_hWnd;
		//AfxGetMainWnd()->m_hWnd;
	XMainWnd::StartCall();
}

void CMobileView::ReleaseCall()
{
	if(m_pCameraDlg)
	{
		delete m_pCameraDlg;
		m_pCameraDlg=NULL;
	}
	CoUninitialize();
	XMainWnd::ReleaseCall();
}

XBOOL CMobileView::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	switch(nOpera)
	{
	case XCMD_FLASHCLOSE:
		((CGGClientDlg*)m_pWnd)->CloseFlash();
		break;
	case XCMD_FLASHOPEN:
		((CGGClientDlg*)m_pWnd)->OpenFlash();
		break;
	case XCMD_OUTOFMEMORY:
		AfxMessageBox(_T("内存不够了(或其他严重错误)，再来一次也许好点！"));
		break;
	case XCMD_UPDATEPROFILE:
		switch(pData1)
		{
			case XCMD_UPDATE_NETRATE:
				 ((CGGClientDlg*)m_pWnd)->AddNetData(pData2);
				 break;
			case XCMD_UPDATE_FRAMES:
				 ((CGGClientDlg*)m_pWnd)->UpdateFrames(pData2);
				 break;
			case XCMD_UPDATE_MEMORY:
				 ((CGGClientDlg*)m_pWnd)->UpdateMemory(pData2);
				 break;
		}
		break;

	case XWM_STARTEDIT:
		 //Create Full Edit Control Here
		{
			int nMaxLength=pData1>>16;
			XU8 nStyle=pData1&0xff;
			XPCWSTR pText=(XPCWSTR)pData2;
			txtDlg.m_strText=pText;
			if(txtDlg.DoModal()==IDOK)
				PostMsg(XWM_FINISHEDEDIT,(XU32)(XPCWSTR)txtDlg.m_strText,txtDlg.m_strText.GetLength());
			//Notify:Post Message(XWM_FINISHEDEDIT (XU32)WCHARS) //where edit finished
		}
		break;
	case XCMD_FLASHMODECMD:
		//if(pData1==XFLASH_CHANGEMODE)
		{
			//m_pWnd->PostMessage(WM_COMMAND,ID_APP_MODE);
			((CGGClientDlg*)m_pWnd)->SetMode(pData1);
		}
		break;
	case XWM_UPDATEMSG:
		switch(pData1)
		{//Apple ETC note here....
		case XCMD_UPDATE_FLASHBAR:
			 ((CGGClientDlg*)m_pWnd)->m_flashBar.EnableItem();
			 break;
		case XCMD_FLASH_LOADING_END:
			 ///
			 break;
		case XCMD_FLASH_START:
			 ///
			 break;
		case XCMD_UPDATE_WEBVIEW:
			 ((CGGClientDlg*)m_pWnd)->ShowWeb();
			 break;
		case XCMD_UPDATE_DATASTART:
			((CGGClientDlg*)m_pWnd)->ShowDataProc(pData2);
			 break;
		case XCMD_UPDATE_DATASET:
			((CGGClientDlg*)m_pWnd)->SetDataProc(pData2);
			 break;
		case XCMD_UPDATE_DATAEND:
			((CGGClientDlg*)m_pWnd)->HideDataProc();
			 break;
		}
		break;
	case XWM_REPAINT:
		//CCEGLView::sharedOpenGLView()->swapBuffers();
		if(m_pWnd!=XNULL)
		{
			XRect clip(m_clip);
			
			m_pDraw->SetOrigin(0,0);
			if(Paint(*m_pDraw))
			{
				clip+=m_rect.TopLeft();
				m_pWnd->InvalidateRect(CRect(clip));			

			}
			else
			{
				//clip-=m_rect.TopLeft();
				InvalidateRect(clip);
			}
				//PostMsg(XWM_REPAINT,0,(XU32)this,XMS_THIS);
		}break;
	case XWM_QUITAPP:
		m_pWnd->PostMessage(WM_COMMAND,ID_APP_EXIT,0);
		break;
	}
	return XMainWnd::Handle(nOpera,pData1,pData2);
}

XBOOL CMobileView::Create(CRect &rect,XGraphics*p,CWnd*pWnd,
						  XPCWSTR strTitle,XPCTSTR strPath)
{
	//m_draw.FromImage(*p);

	m_pWnd=pWnd;
	XString8 str;
	XRect rt(rect.left,rect.top,rect.right,rect.bottom);

	return XMainWnd::Create(rt,strTitle,p,strPath);
}

void CMobileView::PeekMessage()
{
	((CGGClientDlg*)m_pWnd)->PeekMessage();
}

void CMobileView::SetCursor(XU8 nType)
{
   if(m_nCurType==nType) return;
   
   m_nCurType=nType;
   CGGClientDlg*p=(CGGClientDlg*)m_pWnd;
   DeleteObject(p->m_hCursor);
   if(m_bEnuHand)
   {
		p->m_hCursor=AfxGetApp()->LoadCursor(IDC_THUMB);   
   }
   else
   {
	   switch(m_nCurType)
	   {
	   case XCUR_HAND:
			p->m_hCursor=AfxGetApp()->LoadCursor(IDC_HANDON);
			break;
	   case XCUR_HIDE:
			p->m_hCursor=AfxGetApp()->LoadCursor(IDC_HIDE);
			break;
	   case XCUR_INPUT:
			p->m_hCursor=AfxGetApp()->LoadStandardCursor(IDC_IBEAM);
			break;
	   case XCUR_ANCHOR:
			p->m_hCursor=AfxGetApp()->LoadCursor(IDC_HANDPT);
			break;
	   default:
			p->m_hCursor=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			break;
	   }
   }
   ::SetCursor(p->m_hCursor);
}

void CMobileView::SetConfig(BROWNSINFO &info)
{
	XU32 i;
	/*for(i=0;i<m_views.GetSize();i++)
	{
		m_views[i]->ReleaseClient();
	}
	for(i=0;i<m_clientApply.m_clients.GetSize();i++)
	{
		m_clientApply.m_clients[i]->Release(XTRUE);
		delete m_clientApply.m_clients[i];
	}
	m_clientApply.m_clients.RemoveAll();*/
	m_clientApply.ReleaseClient();

	m_bEnuHand=info.bEnuHand;
	m_clientApply.m_info.uPort=info.nPort;
	m_clientApply.m_info.nProtocol=info.nProtcol;
	m_clientApply.m_info.nRetries=info.nRetries;
	m_clientApply.m_info.nTimeout=info.nTimeout;
	m_clientApply.SetCID(XString8(info.strChannelID));
	XString8 strHost;
	XString8 strID(info.strClientID);
	m_clientApply.SetXID(strHost,strID,XFALSE);
	m_clientApply.m_info.strAccepts.RemoveAll();
	for(i=0;i<(XU32)info.strAccepts.GetSize();i++)
	{
		m_clientApply.m_info.strAccepts.Add((LPCTSTR)info.strAccepts[i]);
	}
	m_clientApply.m_info.strGateway=info.strGameway;
	m_clientApply.m_info.strProfile=info.strProfile;
	m_clientApply.m_info.strUserAgent=info.strUserAgent;//Modify User Agent
	m_clientApply.m_info.nLanguages.Add(XEnumLan::CHINESE);
	m_clientApply.m_info.nLanguages.Add(XEnumLan::ENGLISH);
	XString8 strType(info.strMobileType);
	m_clientApply.SetMobileType(strType);//m_info.strType=info.strMobileType;
	XU32 nColors[XCW_BUTTON+1];
	for(i=0;i<XCW_BUTTON+1;i++)
		nColors[i]=TRUECOLOR(info.nColor[i]);
	XWindow::SetColors(nColors);
}

void CMobileView::GetConfig(BROWNSINFO &info)
{
	XU32 i;
	info.nPort=m_clientApply.m_info.uPort;
	info.nProtcol=m_clientApply.m_info.nProtocol;
	info.nRetries=m_clientApply.m_info.nRetries;
	info.nTimeout=m_clientApply.m_info.nTimeout;
	info.strChannelID=m_clientApply.GetCID();
	info.strClientID=m_clientApply.GetXID();
	info.strMobileType=m_clientApply.m_info.strMobileType;
	info.strAccepts.RemoveAll();
	info.bEnuHand=m_bEnuHand;
	for(i=0;i<m_clientApply.m_info.strAccepts.GetSize();i++)
	{
		info.strAccepts.Add((XPCTSTR)m_clientApply.m_info.strAccepts[i]);
	}
	info.strGameway=m_clientApply.m_info.strGateway;
	info.strProfile=m_clientApply.m_info.strProfile;
	info.strUserAgent=m_clientApply.m_info.strUserAgent;
	m_clientApply.m_info.nLanguages.Add(XEnumLan::CHINESE);
	m_clientApply.m_info.nLanguages.Add(XEnumLan::ENGLISH);
	for(i=0;i<=XCW_BUTTON;i++)
		info.nColor[i]=TRUECOLOR(XWindow::GetSysColor(i));
}

//DEL void CMobileView::ResetConfig()
//DEL {
//DEL 	m_clientApply.ResetConfig(XTRUE);
//DEL }

void CMobileView::ViewSource()
{
#ifdef _SOURCECODE
	XDomView*pView=GetActiveView();
	TCHAR strTemp[MAX_PATH],strFile[MAX_PATH];
	::GetTempPath(MAX_PATH,strTemp);
	::GetTempFileName(strTemp,_T("sr"),100,strFile);
	CFile file;
	if(!file.Open(strFile,CFile::modeWrite|CFile::modeCreate))
	   return;
	file.Write(pView->m_strSource,pView->m_strSource.GetLength());
	file.Close();
	XString8 strCmd="Notepad ";
	strCmd+=strFile;
	WinExec(strCmd,SW_SHOW);
#endif
}

void CMobileView::ViewURL()
{
    XDomView*pView=GetActiveView();
	TCHAR strTemp[MAX_PATH],strFile[MAX_PATH];
	::GetTempPath(MAX_PATH,strTemp);
	::GetTempFileName(strTemp,_T("sr"),100,strFile);
	CFile file;
	if(!file.Open(strFile,CFile::modeWrite|CFile::modeCreate))
	   return;
	XString8 str=pView->GetConnectURL().m_strURL;
	str+="\r\n";
	//str+=pView->
	file.Write(str,str.GetLength());
	file.Close();
	XString8 strCmd="Notepad ";
	strCmd+=strFile;
	WinExec(strCmd,SW_SHOW);

}

void CMobileView::ViewTree()
{
	XDomView*pView=GetActiveView();
	CDom dlg(pView->GetDom()->GetDoc());
	dlg.DoModal();
}

void CMobileView::Layerout()
{
    GetActiveView()->Layerout();
}

void CMobileView::ViewTextVars()
{
	XDomView*pView=GetActiveView();
	if(pView==XNULL) return;
	XSWFPlayer*player=pView->GetPlayer();
	if(player==XNULL) return;
	XSObjectArray list;
	if(!pView->GetTextEdits(list)) return;
	XString8 strTxt="名字\t路径\t内容\t对象\t\r\n";

	
	int i,nc=list.GetSize();
	for(i=0;i<nc;i++)
	{
		SObject*pObj=list[i];
		if(!pObj->character||pObj->character->type!=editTextChar) continue;
		EditText*p=pObj->editText;
		if(!p->m_variable.IsNull()) //continue;
			strTxt+=p->m_variable.strTxt;
		strTxt+="\t";
		XXVar var;
		//char*strPath=
		player->GetTargetPath(pObj->thread->rootObject,var);
		if(var.IsString())//strPath)
		{
			XString8 strT=var.strTxt;//strPath;
			strT.ConvertToGB();
			strTxt+=strT;
		}

		strTxt+="\t";
//		FreeStr(strPath);
		//char*str=p->GetBuffer();
		XXVar str;
		p->GetBuffer(str);
		//if(str)
		{
			XString8 strT=str.strTxt;
			strT.ConvertToGB();
			strTxt+=strT;
		}
		strTxt+="\t";
		if(!pObj->name.IsNull())
		{
			XString8 strT=pObj->name.strTxt;
			strT.ConvertToGB();
			strTxt+=strT;
		}
//		FreeStr(str);
		strTxt+="\r\n";
	}
	
	

	TCHAR strTemp[MAX_PATH],strFile[MAX_PATH];
	::GetTempPath(MAX_PATH,strTemp);
	::GetTempFileName(strTemp,_T("sr"),100,strFile);
	CFile file;
	if(!file.Open(strFile,CFile::modeWrite|CFile::modeCreate))
	   return;
	file.Write(strTxt,strTxt.GetLength());
	file.Close();
	XString8 strCmd="Notepad ";
	strCmd+=strFile;
	WinExec(strCmd,SW_SHOW);
}

void CMobileView::ThumbPoint(XPoint&pt)
{
	if(!m_bEnuHand) return;
	int rx=rand()%20;
	int ry=rand()%20;
	pt.x+=rx-10;
	pt.y+=rx-10;
}

void CMobileView::QuitView()
{
	XPCWSTR _strQuitSystem=XResource::LoadWString(XIDS_QUITSYSTEM);
	if(m_pBox)
		 delete m_pBox;
	m_pBox=XMessageBox::Message(_strQuitSystem,this,XCMD_QUIT,XMB_YESNO|XMB_ICONQUESTION);
}

void CMobileView::TraceText(const XU16*msg)
{
	if(m_pWnd)
	{
		((CGGClientDlg*)m_pWnd)->Trace(msg);
	}
}