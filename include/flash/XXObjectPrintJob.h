// XXObjectPrintJob.h: interface for the XXObjectPrintJob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTPRINTJOB_H__78F4BB84_FB8C_4803_B173_E861921DA0B7__INCLUDED_)
#define AFX_XXOBJECTPRINTJOB_H__78F4BB84_FB8C_4803_B173_E861921DA0B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectPrintJob : public XXObject  
{
public:
	XXObjectPrintJob(ScriptPlayer*pRoot);
	virtual ~XXObjectPrintJob();

};

#endif // !defined(AFX_XXOBJECTPRINTJOB_H__78F4BB84_FB8C_4803_B173_E861921DA0B7__INCLUDED_)
