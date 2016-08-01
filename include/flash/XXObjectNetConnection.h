// XXObjectNetConnection.h: interface for the XXObjectNetConnection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXOBJECTNETCONNECTION_H__90763F1D_9100_40EC_9F97_6C92173CF30F__INCLUDED_)
#define AFX_XXOBJECTNETCONNECTION_H__90763F1D_9100_40EC_9F97_6C92173CF30F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XXObject.h"

class XXObjectNetConnection : public XXObject  
{
public:
	XXObjectNetConnection(ScriptPlayer*pRoot);
	virtual ~XXObjectNetConnection();

};

#endif // !defined(AFX_XXOBJECTNETCONNECTION_H__90763F1D_9100_40EC_9F97_6C92173CF30F__INCLUDED_)
