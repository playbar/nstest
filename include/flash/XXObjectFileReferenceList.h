// XXObjectFileReferenceList.h: interface for the XXObjectFileReferenceList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTFILEREFERENCELIST_H__8B0FBE7B_BA90_4B91_8A38_E90C40839A02__INCLUDED_)
#define AFX_XXOBJECTFILEREFERENCELIST_H__8B0FBE7B_BA90_4B91_8A38_E90C40839A02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectFileReferenceList : public XXObject  
{
public:
	XXObjectFileReferenceList(ScriptPlayer*pRoot);
	virtual ~XXObjectFileReferenceList();

};

#endif // !defined(AFX_XXOBJECTFILEREFERENCELIST_H__8B0FBE7B_BA90_4B91_8A38_E90C40839A02__INCLUDED_)
