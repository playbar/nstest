/*
 ============================================================================
 Name		: ZCSocketWriter.cpp
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CZCSocketWriter implementation
 ============================================================================
 */

#include "ZCSocketWriter.h"
#include <string.h>
#include <stdio.h>
#ifndef KErrWriteMsg
#	define KErrWriteMsg "ErrWrite::"
#endif

CZCSocketWriter::CZCSocketWriter(MWriterObserver &aObserver,RSocket &aSocket) :
	CActive(EPriorityStandard),iTempBuf(0,0),m_iObserver(aObserver),m_pSocket(aSocket) // Standard priority
	{
	iRequest=0;
	iState=0;
	m_iTimer=NULL;
	iWait=NULL;
	}

CZCSocketWriter* CZCSocketWriter::NewLC(MWriterObserver &aObserver,RSocket &aSocket)
	{
	CZCSocketWriter* self = new (ELeave) CZCSocketWriter(aObserver,aSocket);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CZCSocketWriter* CZCSocketWriter::NewL(MWriterObserver &aObserver,RSocket &aSocket)
	{
	CZCSocketWriter* self = CZCSocketWriter::NewLC(aObserver,aSocket);
	CleanupStack::Pop(); // self;
	return self;
	}

void CZCSocketWriter::ConstructL()
	{
	m_iTimer=CAfterTimer::NewL(*this);
	CActiveScheduler::Add(this); // Add to scheduler
	iWait=new CActiveSchedulerWait();
	}

CZCSocketWriter::~CZCSocketWriter()
	{
	Cancel(); // Cancel any request, if outstanding
	Release();
	delete iWait;
	delete m_iTimer;
	}

void CZCSocketWriter::DoCancel()
	{
	if(IsActive())
		m_pSocket.CancelSend();
	}

void CZCSocketWriter::StartL()
	{
	Cancel(); // Cancel any request, just to be sure
	iState |= KISSTART;
	}

int  CZCSocketWriter::Write(char *buf,int len)
	{
		if(IsActive())
			Cancel();
		iTempBuf.Set((TUint8*)buf,len,len);
		m_pSocket.Send(iTempBuf,0,iStatus,m_nSendLen);
		iRequest=EWrite;
		m_iTimer->StartL(2000000);
		SetActive();
		iWait->Start();
		return m_nSendLen();
	}


void CZCSocketWriter::Release()
	{
	Cancel();
	iState &= ~KISSTART;
	}


void CZCSocketWriter::RunL()
	{
	switch(iRequest)
		{
		case EWrite:
			{
			m_iTimer->Cancel();
			if(iWait->IsStarted())
				iWait->AsyncStop();
			int err=iStatus.Int();
			unsigned char msg[]=KErrWriteMsg;
			if(err==KErrNone)
				{
				m_iObserver.OnSend((unsigned char*)iTempBuf.Ptr(),err);
				}
			else
				{
				iState |= KWRITEERR;
				m_iObserver.OnSend(msg,err);
				}
			}
			break;
		}
	iRequest=0;
	}

TInt CZCSocketWriter::RunError(TInt aError)
	{
	return aError;
	}

void CZCSocketWriter::OnTimer(const unsigned char*,int err)
	{
	if(IsActive())
		{
		m_pSocket.CancelSend();
		unsigned char msg[20]=KErrWriteMsg;
		char temp[]="timeout";
		strcat((char*)msg,temp);
		m_iObserver.OnSend(msg,err);
		}
	}
