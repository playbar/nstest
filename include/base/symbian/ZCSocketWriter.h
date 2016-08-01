/*
 ============================================================================
 Name		: ZCSocketWriter.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CZCSocketWriter declaration
 ============================================================================
 */

#ifndef ZCSOCKETWRITER_H
#define ZCSOCKETWRITER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <ES_SOCK.H>	// For RSocket, link against:  esock.lib
#include <in_sock.h>    // For TInetAddr, link against: insock.lib
#include "state_sock.h"
//#include "MExceptionHandleEngine.h"
#include "CAfterTimer.h"

class CZCSocketWriter : public CActive,CAfterTimer::MTimerObserver
	{
public:
	class MWriterObserver
		{
		public:
			virtual void OnSend(const unsigned char *msg,int err)=0;
		};
public:
	// Cancel and destroy
	~CZCSocketWriter();

	// Two-phased constructor.
	static CZCSocketWriter* NewL(MWriterObserver &aObserver,RSocket &aSocket);

	// Two-phased constructor.
	static CZCSocketWriter* NewLC(MWriterObserver &aObserver,RSocket &aSocket);

public:
	// New functions
	// Function for making the initial request
//	void InitTCP(RSocketServ &aServer,MExceptionHandleEngine *ErrObject);
//	void Connect(TSockAddr &anAddr);
	int  Write(char *buf,int len);
//	void Stop();
	void Release();
	
	void OnTimer(const unsigned char* msg,int err);
	

private:
	// C++ constructor
	CZCSocketWriter(MWriterObserver &aObserver,RSocket &aSocket);

	// Second-phase constructor
	void ConstructL();
	
	void StartL();
	
//	void Fault();

private:
	// From CActive
	// Handle completion
	void RunL();

	// How to cancel me
	void DoCancel();

	// Override to handle leaves from RunL(). Default implementation causes
	// the active scheduler to panic.
	TInt RunError(TInt aError);

private:
	enum TZCSocketWriterState
		{
		EConnect=1,
		EStop,
		EWrite
		// Error condition
		};

private:
	char iState; // State of the active object
	int  iRequest;
	RSocket        		&m_pSocket;
	TPtr8          		iTempBuf;
	TSockXfrLength 		m_nSendLen;
	MWriterObserver     &m_iObserver;
	CAfterTimer    		*m_iTimer;
	CActiveSchedulerWait* iWait;
//	MExceptionHandleEngine *m_pErrEngine;
//	RTimer iTimer; // Provides async timing service

	};

#endif // ZCSOCKETWRITER_H
