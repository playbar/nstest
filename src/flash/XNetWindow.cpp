#include "StdAfxflash.h"
#include "splayer.h"
#include "XNetWindow.h"

#include "ShellCore.h"
#include "avm2.h"
#include "AS3EventDispatcher.h"

using namespace avmshell;

XNetWindow::XNetWindow(XSWFPlayer*player)
{
	m_pPlayer=player;
	m_nCmdSet=0;
	m_nCmdGet=0;
}

XNetWindow::~XNetWindow()
{
}

XBOOL XNetWindow::Handle(XU32 nOperator,XU32 nParam1,XU32 nParam2)
{
	switch (nOperator)
	{
	case XWM_SOCKET:
		//switch(nParam1)
		// {
		//	 break;
		 //}
		 return AddCommand(XWM_SOCKET,nParam1,nParam2);
		//return HandleSocketMsg(nParam1,(XClient*)nParam2);
	}
	//lock.UnLock();
	return XWindow::Handle(nOperator,nParam1,nParam2);
}

void XNetWindow::CallBack()
{
	//LOGWHERE();
	//XLock lock(&m_ptLock);
	//if(lock.Lock(XSHORTTIME))
	{
		while(m_nCmdGet!=m_nCmdSet)
		{
			switch(m_nCmds[m_nCmdGet])
			{
			case XWM_SOCKET:
				 HandleSocketMsg(m_nCmds[m_nCmdGet+1],(XClient*)m_nCmds[m_nCmdGet+2]);
				 break;
			}
			m_nCmdGet+=3;
			if(m_nCmdGet>=MAX_CMDS)
				m_nCmdGet-=MAX_CMDS;
		}
		//if(m_pDom->Layerout(m_draw))
		//	Invalidate();		
		//lock.UnLock();
	}	
}

XBOOL XNetWindow::HandleSocketMsg(XU32 nCmd, XClient *pClient)
{
	//XLock lock(&m_ptLock);
	//if(!lock.Lock(XSHORTTIME))
	//	return XFALSE;
   ShellCore*m_pCore=m_pPlayer->m_pAVM2->GetCore();
   TRY(m_pCore,kCatchAction_ReportAsError)
	{
		//XSWFPlayer::m_pInstance->SetEnterFrame();
		switch(nCmd)
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
		//XSWFPlayer::m_pInstance->ClearEnterFrame();
   }
	CATCH(Exception *exception)
	{
		_TraceInt(exception,"Socket Command",nCmd);
		//AvmAssert(false);
	}
	END_CATCH
	END_TRY
	//lock.UnLock();
	pClient->PostOK();
	return XTRUE;
}

void XNetWindow::OnConnect(XClient *pClient)
{
	EventDispatcherObject *pObject = (EventDispatcherObject *)pClient->GetNotifyID();
	if(!pObject) return;
	pObject->OnConnected();//OnEvent(XAS3_EVENT_connect,false,pObject);
#ifdef _WINEMU_DEBUGFILE
	XString16 s("=====>Socket conected ok:");
	s+=XString16(pClient->GetConnectInfo()->URL.m_strHost);
	Trace(s);
	XXVar strFile=m_pPlayer->m_strWorkPath;
	strFile.StringAdd("Datas\\SOCKET");
	strFile.StringAdd(XXVar((int)pObject));
	strFile.StringAdd(".dat");
	pObject->m_file.Open(strFile.strTxt,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE);
#endif
}

void XNetWindow::OnStarted(XClient *pClient)
{
	
}

void XNetWindow::OnRecive(XClient *pClient)
{
	EventDispatcherObject *pObject = (EventDispatcherObject *)pClient->GetNotifyID();
    if (!pObject)return;
	RESPONSEINFO*pInfo=pClient->GetResponseInfo();
    XLock lock;
	int nSize=0;
    if(pInfo->recBuf.LockRead(lock,0))
    {
		nSize=pInfo->recBuf.GetDataSize();
		if(nSize<=0) {lock.UnLock();return;}
		//XU8Array data;
		//data.SetSize(nSize+1);
		//pInfo->recBuf.Read(data,nSize);
		XU8* pData=pObject->GetInBuffer(nSize);
		//pObject->ReceiveData(pData,nSize);
		pInfo->recBuf.Read(pData,nSize);
		lock.UnLock();
#ifdef _WINEMU_DEBUGFILE
		XString16 s("=====>Socket recive ");
		XString16 si;
		si.FromInt(nSize);
		s+=si;
		s+=XString16(" bytes from:");
		s+=XString16(pClient->GetConnectInfo()->URL.m_strHost);
		Trace(s);
		if(pData&&pObject->m_file.IsValid())
		{
			pObject->m_file.Write(pData,nSize);
		}
#endif
	}
	
	pObject->OnRecieve(nSize);
	//SWFPlayer::m_pInstance->ClearEnterFrame();
		//OnProgressEvent(XAS3_EVENT_socketData,nSize,0);//OnSocketData(nSize);
}

void XNetWindow::OnFinished(XClient *pClient,XBOOL finished /* = XTRUE */)
{

	EventDispatcherObject *pObject = (EventDispatcherObject *)pClient->GetNotifyID();
	if(!pObject) return;
	if(finished)
		pObject->OnClose(NULL);//OnEvent(XAS3_EVENT_close,false,pObject);
		//pObject->OnClose();
	else
		pObject->OnClose("Socket Error");//OnErrorEvent("Socket Error");
		//pObject->OnError("Socket Error");
#ifdef _WINEMU_DEBUGFILE
	XString16 s("=====>Socket has been disconnected:");
	s+=XString16(pClient->GetConnectInfo()->URL.m_strHost);
	Trace(s);
	pObject->m_file.Close();
	//XString8 strFile=m_pPlayer->m_strWorkPath;
	//strFile+="Datas\\SOCKET";
	//XStrin

#endif
}

