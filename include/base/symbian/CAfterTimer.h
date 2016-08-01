/*
 ============================================================================
 Name		: AfterTimer.h
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CtestTimer declaration
 ============================================================================
 */

#ifndef TESTTIMER_H
#define TESTTIMER_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
class CAfterTimer : public CActive
	{
public:
	class MTimerObserver
		{
		public:
		virtual void OnTimer(const unsigned char* msg,int err)=0;
		};
public:
	// Cancel and destroy
	~CAfterTimer();

	// Two-phased constructor.
	static CAfterTimer* NewL(MTimerObserver &aObserver);

	// Two-phased constructor.
	static CAfterTimer* NewLC(MTimerObserver &aObserver);

public:
	// New functions
	// Function for making the initial request
	void StartL(TTimeIntervalMicroSeconds32 aDelay);

private:
	// C++ constructor
	CAfterTimer(MTimerObserver &aObserver);

	// Second-phase constructor
	void ConstructL();

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
	enum TtestTimerState
		{
		EUninitialized, // Uninitialized
		EInitialized, // Initalized
		EError
		// Error condition
		};

private:
	TInt iState; // State of the active object
	RTimer iTimer; // Provides async timing service
	MTimerObserver &iObserver;
	};

#endif // TESTTIMER_H
