/*
 ============================================================================
 Name		: MExceptionHandleEngine.h
 Author	  : 
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CMExceptionHandleEngine declaration
 ============================================================================
 */

#ifndef MEXCEPTIONHANDLEENGINE_H
#define MEXCEPTIONHANDLEENGINE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
 *  CMExceptionHandleEngine
 * 
 */
class MExceptionHandleEngine : public CBase
	{
public:
	virtual int ConnectException(); 
	virtual int SendException();
	virtual int RecvException();
	virtual int SockOpenException();
	virtual int DisconnectException();
	};

#endif // MEXCEPTIONHANDLEENGINE_H
