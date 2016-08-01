// XDialogSave.cpp: implementation of the XDialogSave class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxggdom.h"
#include "XDialogSave.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define _strSavePrompt XResource::LoadWString(XIDS_SAVEFILE)
#include "XConnectFile.h"
#include "XFlashView.h"
#ifdef __APPLE__
XDialogSave::XDialogSave(XClientApply*pApply,XString8&url,XPCWSTR strPrompt,XU8Array&config)
#elif defined(_SYMBIAN)
XDialogSave::XDialogSave(XClientApply*pApply,XString8&url,XPCWSTR strPrompt,XU8Array&config)
#elif defined( __ANDROID__ )
XDialogSave::XDialogSave(XClientApply*pApply,XString8&url,XPCWSTR strPrompt,XU8Array&config)
#else
XDialogSave::XDialogSave(XClientApply*pApply,XString8&url,XPCWSTR strPrompt,XU8Array&config):
	XMessageBox(_strSavePrompt, strPrompt, XNULL, XCMD_FLASH_SAVE, XMB_STOP, m_nTextHeight+10)
#endif
{
	m_pClient=XNULL;
    m_strURL=url;
	m_pApply=pApply;
	m_nPecent=0;
	m_nTimes=0;

	XString8 strURL=url;
	XURL::Decode(strURL);
	XFileFinder::GetFileName(strURL,m_strName,XFALSE);//XTRUE);
	m_nCode=0;
	pApply->GetLocalPath(m_strFile);
	XFile::CreateFolder(m_strFile);
#ifdef __APPLE__
	m_strFile += "/";
#elif defined ( __ANDROID__ )
	m_strFile += "/";
#else
	m_strFile+='\\';
#endif
	m_strFile+=m_strName;
//#ifndef __ANDROID__	// zhengjl 2011.09.10
	m_strFile.ConvertToGB();
//#endif
	m_strFile+=".swf";

	//LOGE( "----->%s", m_strFile.GetData() );
  
	if(config.GetSize())
	{
		XU8Array data;
		int len=XClientApply::BinEncode(config.GetData(),config.GetSize(),data);
		if(len>0)
		{
			if(m_file.Open(m_strFile,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
				m_file.Write(data.GetData(),len);
		}
		/*XString8 strConfig;
		XConnectFile::GetConfigFile(m_strFile,strConfig);
		XFile file;
		if(file.Open(strConfig,XFile::XWRITE|XFile::XBINARY|XFile::XCREATE))
		{
			file.Write(config.GetData(),config.GetSize());
			file.Close();
		}*/
	}


}

#ifdef _SYMBIAN
XDialogSave::XDialogSave(XClientApply*pApply,XString8&url,XPCWSTR strPrompt)
//						XMessageBox(_strSavePrompt,strPrompt,XNULL,XCMD_FLASH_SAVE,
//								XMB_STOP,m_nTextHeight+10)
	{
	m_pClient=XNULL;
    m_strURL=url;
	m_pApply=pApply;
	m_nPecent=0;
	m_nTimes=0;

	XString8 strURL=url;
	XURL::Decode(strURL);
	XFileFinder::GetFileName(strURL,m_strName,XTRUE);//XTRUE);
	m_nCode=0;
	pApply->GetLocalPath(m_strFile);
	XFile::CreateFolder(m_strFile);

	m_strFile+='\\';
	m_strFile+=m_strName;
	m_nDownStatus = KDownSis;
//	m_strFile.ConvertToGB();
	}
#endif

XDialogSave::~XDialogSave()
{
	Release();
}

XU32 XDialogSave::DoModal(XWindow*pHandle)
{
   SetHandle(pHandle,m_nCommand);
   CONNECTREQ req;
   req.bForward=XTRUE;
   req.nMethod=XEM::GET;
   req.URL.SetURL(m_strURL);
   m_pClient=m_pApply->CreateClient(req,0,0,this);
   if(!m_pClient)
	   return XIDCANCEL;
   m_pClient->Connect(req,XCHARSET_GB2312);
   return 0;

}

XBOOL XDialogSave::OnPaint(XGraphics &g)
{
#ifdef __APPLE__
	return XTRUE;
#elif defined(_SYMBIAN)
	return XTRUE;
#elif defined( __ANDROID__ )
	return XTRUE;
#else
	XMessageBox::OnPaint(g);
	XRect rect;
	GetClientRect(rect);
	rect.InflateRect(-6,-6);
	rect.bottom-=XMAX(26,m_nTextHeight+4);
	rect.top=rect.bottom-m_nTextHeight;
	XColor c(GetSysColor(XCW_BUTTON));
	int v=rect.right;
	rect.right=rect.left+(rect.Width()*m_nPecent/100);
	g.DrawBar(rect,GetSysColor(XCW_SELECTBACK),16);
	rect.right=v;
	g.DrawFrame(rect,c,1);
	return XTRUE;
#endif
}

XBOOL XDialogSave::Handle(XU32 nOpera, XU32 pData1, XU32 pData2)
{
	if(nOpera==XWM_SOCKET)
	{
		XClient*pClient=(XClient*)pData2;
		if(pClient!=m_pClient)
			return XFALSE;
		switch(pData1)
		{
			case XClient::CONNECTED:
				 OnConnect(pClient);
				 break;
			case XClient::STARTED:
				 OnStarted(pClient);
				 break;
			case XClient::RECIVE:
				 OnRecive(pClient);
				 break;
			case XClient::FINISHED:
				 OnFinished(pClient);
				 break;
			case XClient::CANCELED:
				 OnFinished(pClient,XFALSE);
				 break;
		}
		pClient->PostOK();
		return XTRUE;
	}
	else if(nOpera==XWM_COMMAND&&pData1==XIDSTOP)
	{
		m_file.Close();
		XBOOL bOK=m_pClient->IsConnecting();

		m_pClient->Cancel();
		if(bOK)
		{
			XFile::RemoveFile(m_strFile);
		}

		EndDialog(XIDCANCEL);
		return XTRUE;
	}
#ifdef __APPLE__
	return XDialog::Handle(nOpera, pData1, pData2);
#elif defined(_SYMBIAN)
	return XDialog::Handle(nOpera, pData1, pData2);
#elif defined(__ANDROID__ )
	return XDialog::Handle(nOpera, pData1, pData2);
#else
	return XMessageBox::Handle(nOpera,pData1,pData2);
#endif
}

#ifdef __APPLE__
void XDialogSave::_setParent(XWindow* window)
{
	m_pParent = window;
}
#endif

#ifdef __ANDROID__
void XDialogSave::_setParent(XWindow* window)
{
	m_pParent = window;
}
#endif

void XDialogSave::OnConnect(XClient *pClient)
{
	m_nPecent = 5;
#ifdef __APPLE__
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p->PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_SAVE_PERCENT, m_nPecent, XMS_THIS);
#elif defined(_SYMBIAN)
	m_pHandle->PostMsg(XWM_SYMBIAN_COMMAND, XCMD_FLASH_SAVE_START, m_nPecent, XMS_TOPMOST);
#elif defined( __ANDROID__ )
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p-> PostMsg( ANDROID_FLASH_SAVE_PERCENT, m_nPecent, NULL, XMS_THIS);
#else
	Invalidate();
#endif
}

void XDialogSave::OnStarted(XClient *pClient)
{
    RESPONSEINFO*pInfo=pClient->GetResponseInfo();
	m_nPecent=10;
	switch(pInfo->nErrorCode)
	{
	case XERR_USER_CANCELED:
		 break;
	case XHTTP_STATUS_OK:
		 if(!m_file.IsValid())
		 {
			if(!m_file.Open(m_strFile,XFile::XWRITE|XFile::XBINARY|XFile::XCREATE))
				 m_pClient->Cancel();
		 }
		 break;
	case XHTTP_STATUS_REDIRECT_KEEP_VERB:
	case XHTTP_STATUS_MOVED:
	case XHTTP_STATUS_REDIRECT:
	case XHTTP_STATUS_REDIRECT_METHOD:
	case XHTTP_STATUS_USE_PROXY:
		 if(!pInfo->strLocation.IsEmpty() && m_nTimes<5)
		 {
			 CONNECTREQ req;
			 req.bForward=XTRUE;
			 req.nMethod=XEM::GET;
			 req.URL.SetURL(pInfo->strLocation);
			 pClient->Connect(req,pInfo->type.m_nCharset);
			 m_nTimes++;
		 }
		 break;
	}
#ifdef __APPLE__
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p->PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_SAVE_PERCENT, m_nPecent, XMS_THIS);
#elif defined(_SYMBIAN)
	m_pHandle->PostMsg(XWM_SYMBIAN_COMMAND, XCMD_LOAD_PERCENT, m_nPecent, XMS_TOPMOST);
#elif defined( __ANDROID__ )
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p->PostMsg(ANDROID_FLASH_SAVE_PERCENT, m_nPecent, NULL, XMS_THIS);
#else
	Invalidate();
#endif
	//m_status.SetPecent(CalcPecent());
}

void XDialogSave::OnRecive(XClient *pClient)
{
   RESPONSEINFO*pInfo=pClient->GetResponseInfo();
   XLock lock;
   if(pInfo->recBuf.LockRead(lock,0))
   {
	 int nSize = pInfo->recBuf.GetDataSize();
	 if(nSize <=0 )
	 {
		 lock.UnLock();
		 return;
	 }
	 XU8Array data;
	 data.SetSize(nSize+1);
	 pInfo->recBuf.Read(data,nSize);
	 lock.UnLock();
	 int nv = pInfo->nLength;
	 if(nv<=0)
		 nv=MAX_PACK;
	 m_nPecent=10+(90*pInfo->nTotalSize/nv);
	 if(m_nPecent>100)
		 m_nPecent=100;
	 m_file.Write(data.GetData(),nSize);

   }
#ifdef __APPLE__
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p->PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_SAVE_PERCENT, m_nPecent, XMS_THIS);
#elif defined(_SYMBIAN)
	m_pHandle->PostMsg(XWM_SYMBIAN_COMMAND, XCMD_LOAD_PERCENT, m_nPecent, XMS_TOPMOST);
#elif defined( __ANDROID__ )
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p->PostMsg(ANDROID_FLASH_SAVE_PERCENT, m_nPecent, NULL, XMS_THIS);
#else
   Invalidate();
#endif
}

void XDialogSave::OnFinished(XClient *pClient, XBOOL bOK)
{
	m_file.Close();
#ifdef __APPLE__
	m_nPecent = 100;
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p->PostMsg(XWM_APPLE_MESSAGE, XCMD_FLASH_SAVE_PERCENT, 101, XMS_THIS);
#elif defined(_SYMBIAN)
	m_nPecent=100;
	m_strTitle+=XResource::LoadWString(bOK?XIDS_SUCCESS:XIDS_FAILED);
//	for(int i=0;i<3;i++)
//		m_buts[i].SetTitle(XResource::LoadWString(XIDS_FINISHED));	
//	Invalidate();
	if(bOK)
		m_pHandle->PostMsg(XWM_SYMBIAN_COMMAND, XCMD_FLASH_SAVE_END, m_nPecent, XMS_TOPMOST);
#elif defined( __ANDROID__ )
	m_nPecent = 100;
	XWindow* p = GetParent();
	while (p->GetParent())
	{
		p = p->GetParent();
	}
	p->PostMsg(ANDROID_FLASH_SAVE_PERCENT, 101, NULL, XMS_THIS);
#else
	m_nPecent=100;
	m_strTitle+=XResource::LoadWString(bOK?XIDS_SUCCESS:XIDS_FAILED);
	for(int i=0;i<3;i++)
		m_buts[i].SetTitle(XResource::LoadWString(XIDS_FINISHED));
	Invalidate();
#endif
	if(!bOK)
	{
		XFile::RemoveFile(m_strFile);
	}

}

void XDialogSave::Release()
{
	m_pClient->Cancel();
}

void XDialogSave::GetPrompt(XString8 &url, XString16 &strTxt)
{
	XString8 strName;
	XString8 strPath=url;
	XURL::Decode(strPath);
	XFileFinder::GetFileName(strPath,strName);
 	strTxt=_strSavePrompt;
 	strTxt+=XString16("[");
 	strTxt+=XString16(strName);
 	strTxt+=XString16("]");
}
