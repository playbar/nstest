// XXObjectCamera.h: interface for the XXObjectCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTCAMERA_H__7BB083FF_8A3C_4FAE_AB9A_82BA75AF4B8F__INCLUDED_)
#define AFX_XXOBJECTCAMERA_H__7BB083FF_8A3C_4FAE_AB9A_82BA75AF4B8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectCamera : public XXObject  
{
public:
	XXObjectCamera(ScriptPlayer*pRoot);
	virtual ~XXObjectCamera();

};

#endif // !defined(AFX_XXOBJECTCAMERA_H__7BB083FF_8A3C_4FAE_AB9A_82BA75AF4B8F__INCLUDED_)
