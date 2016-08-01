// XXObjectAccessibility.h: interface for the XXObjectAccessibility class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTACCESSIBILITY_H__9F231974_5018_4345_AAD8_4404A3EBF568__INCLUDED_)
#define AFX_XXOBJECTACCESSIBILITY_H__9F231974_5018_4345_AAD8_4404A3EBF568__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectAccessibility : public XXObject  
{
public:
	XXObjectAccessibility(ScriptPlayer*pRoot);
	virtual ~XXObjectAccessibility();

};

#endif // !defined(AFX_XXOBJECTACCESSIBILITY_H__9F231974_5018_4345_AAD8_4404A3EBF568__INCLUDED_)
