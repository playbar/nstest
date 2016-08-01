// XXObjectLocalConnection.h: interface for the XXObjectLocalConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTLOCALCONNECTION_H__55A7A1F3_3467_455D_89BD_9946F64E2FBF__INCLUDED_)
#define AFX_XXOBJECTLOCALCONNECTION_H__55A7A1F3_3467_455D_89BD_9946F64E2FBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectLocalConnection : public XXObject  
{
public:
	XXObjectLocalConnection(ScriptPlayer*pRoot);
	virtual ~XXObjectLocalConnection();

};

#endif // !defined(AFX_XXOBJECTLOCALCONNECTION_H__55A7A1F3_3467_455D_89BD_9946F64E2FBF__INCLUDED_)
