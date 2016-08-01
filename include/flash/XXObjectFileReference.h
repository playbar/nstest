// XXObjectFileReference.h: interface for the XXObjectFileReference class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTFILEREFERENCE_H__A46B159D_EAC4_4245_B04B_2345DA9394ED__INCLUDED_)
#define AFX_XXOBJECTFILEREFERENCE_H__A46B159D_EAC4_4245_B04B_2345DA9394ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"
class XXObjectFileReference:public XXObject
{
public:
	XXObjectFileReference(ScriptPlayer*pRoot);
	virtual ~XXObjectFileReference();

};

#endif // !defined(AFX_XXOBJECTFILEREFERENCE_H__A46B159D_EAC4_4245_B04B_2345DA9394ED__INCLUDED_)
