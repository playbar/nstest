/*
 ============================================================================
 Name		: ZCSocketReader.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CZCSocketReader declaration
 ============================================================================
 */

#ifndef ZCSOCKETREADER_H
#define ZCSOCKETREADER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <ES_SOCK.H>	// For RSocket, link against:  esock.lib
#include <in_sock.h>    // For TInetAddr, link against: insock.lib
#include "state_sock.h"
#include "MExceptionHandleEngine.h"

#define KBUFSIZE       1024

class CZCSocketReader : public CActive
	{
public:
	class MReaderObserver
		{
		public:
			virtual void OnReceive(const unsigned char *mMsg,int Error)=0;
		};
	// Cancel and destroy
	~CZCSocketReader();

	// Two-phased constructor.
	static CZCSocketReader* NewL(MReaderObserver &aObserver,RSocket &aSocket,int space=KBUFSIZE);

	// Two-phased constructor.
	static CZCSocketReader* NewLC(MReaderObserver &aObserver,RSocket &aSocket,int space=KBUFSIZE);

public:
	// New functions
	// Function for making the initial request
//	void InitTCP(RSocketServ &aServer,MExceptionHandleEngine *ErrObject,int space=KBUFSIZE);
//	void Connect(TSockAddr &anAddr);  //connect the server

	int Read(char *buf,int len);
	int SetSpace(int space);
	int GetSpace();
	int GetDataSpace();
	int Check();
//	int Stop();
	int Release();
	inline char GetState(){return iState;}
	
private:
	// C++ constructor
	CZCSocketReader(MReaderObserver &aObserver,RSocket &aSocket);

	// Second-phase constructor
	void ConstructL(int space);
	
public:
	void StartL();   //set the temp buffer and start the machine
	
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
	enum TZCSocketReaderState
		{
		EConnect=1,
		EStop,
		ERead
		// Error condition
		};

private:
	int  iRequest; //state of the request
	char iState; // State of the active object
//	RTimer iTimer; // Provides async timing service
	RSocket 		&m_pSocket;
	TPtr8 			iTempBuf;
	unsigned char 	*m_pBuf;
//	HBufC8			*m_pBuf;			
	unsigned char 	*m_lWrite;
	int 			m_nMaxLen;
	int 			m_nDataLen;
	TSockXfrLength 	m_nRecvLen;
//	CActiveSchedulerWait* iWait;
//	MExceptionHandleEngine *m_pErrEngine;
	MReaderObserver &m_iObserver;
public:
	bool			m_bIsStart;
	};

#endif // ZCSOCKETREADER_H
