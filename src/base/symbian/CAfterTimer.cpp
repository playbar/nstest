/*
 ============================================================================
 Name		: testTimer.cpp
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CtestTimer implementation
 ============================================================================
 */

#include "CAfterTimer.h"

CAfterTimer::CAfterTimer(MTimerObserver &aObserver) :
	CActive(EPriorityStandard),iObserver(aObserver) // Standard priority
	{
	}

CAfterTimer* CAfterTimer::NewLC(MTimerObserver &aObserver)
	{
	CAfterTimer* self = new (ELeave) CAfterTimer(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CAfterTimer* CAfterTimer::NewL(MTimerObserver &aObserver)
	{
	CAfterTimer* self = CAfterTimer::NewLC(aObserver);
	CleanupStack::Pop(); // self;
	return self;
	}

void CAfterTimer::ConstructL()
	{
//	User::LeaveIfError(iTimer.CreateLocal()); // Initialize timer
	iTimer.CreateLocal();
	CActiveScheduler::Add(this); // Add to scheduler
	}

CAfterTimer::~CAfterTimer()
	{
	Cancel(); // Cancel any request, if outstanding
	iTimer.Close(); // Destroy the RTimer object
	// Delete instance variables if any
	}

void CAfterTimer::DoCancel()
	{
	if(IsActive())
		iTimer.Cancel();
	}

void CAfterTimer::StartL(TTimeIntervalMicroSeconds32 aDelay)
	{
	Cancel(); // Cancel any request, just to be sure
	iState = EUninitialized;
	iTimer.After(iStatus, aDelay); // Set for later
	SetActive(); // Tell scheduler a request is active
	}

void CAfterTimer::RunL()
	{
	unsigned char msg[] = "timeout";
	iObserver.OnTimer(msg,0);
	}

TInt CAfterTimer::RunError(TInt aError)
	{
	return aError;
	}
