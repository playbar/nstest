// XXObjectNetStream.h: interface for the XXObjectNetStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTNETSTREAM_H__EFBA754A_7DB8_4F5B_8116_CED7FBB2C43D__INCLUDED_)
#define AFX_XXOBJECTNETSTREAM_H__EFBA754A_7DB8_4F5B_8116_CED7FBB2C43D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectNetStream : public XXObject  
{
public:
	XXObjectNetStream(ScriptPlayer*pRoot);
	virtual ~XXObjectNetStream();

};

#endif // !defined(AFX_XXOBJECTNETSTREAM_H__EFBA754A_7DB8_4F5B_8116_CED7FBB2C43D__INCLUDED_)
